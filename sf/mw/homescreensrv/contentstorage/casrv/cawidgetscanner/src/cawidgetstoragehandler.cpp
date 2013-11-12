/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of "Eclipse Public License v1.0"
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 *
 * Initial Contributors:
 * Nokia Corporation - initial contribution.
 *
 * Contributors:
 *
 * Description:
 *
 */
// INCLUDE FILES

#include <badesca.h>
#include <usif/scr/screntries.h>
#include <hbtextresolversymbian.h>

#include "widgetscannerutils.h"
#include "cawidgetstoragehandler.h"
#include "cadef.h"
#include "cainnerentry.h"
#include "cainnerquery.h"
#include "castorageproxy.h"
#include "caarraycleanup.inl"
#include "calocalizationentry.h"
#include "cawidgetscannerparser.h"
#include "cawidgetscannerdef.h"

using namespace Usif;

// ============================ MEMBER FUNCTIONS ============================z===

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCaWidgetStorageHandler::CCaWidgetStorageHandler( CCaStorageProxy* aStorage,
        Usif::RSoftwareComponentRegistry& aSoftwareRegistry, RFs& aFs ) :
            iSoftwareRegistry(aSoftwareRegistry)
    {
    iStorage = aStorage;
    iFs = aFs;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCaWidgetStorageHandler::ConstructL()
    {
    iParser = CCaWidgetScannerParser::NewL( iFs );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCaWidgetStorageHandler* CCaWidgetStorageHandler::NewL(
        CCaStorageProxy* aStorage,
        Usif::RSoftwareComponentRegistry& aSoftwareRegistry, RFs& aFs )
    {
    CCaWidgetStorageHandler* self = NewLC( aStorage, aSoftwareRegistry, aFs );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCaWidgetStorageHandler* CCaWidgetStorageHandler::NewLC(
        CCaStorageProxy* aStorage,
        Usif::RSoftwareComponentRegistry& aSoftwareRegistry, RFs& aFs )
    {
    CCaWidgetStorageHandler* self = new ( ELeave ) CCaWidgetStorageHandler(
            aStorage, aSoftwareRegistry, aFs );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCaWidgetStorageHandler::~CCaWidgetStorageHandler()
    {
    delete iParser;
    iWidgetDBCache.ResetAndDestroy();
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetStorageHandler::SynchronizeL()
    {
    FetchWidgetsL();
    AddWidgetsL( iParser->WidgetsScanL( iWidgetDBCache ) );
    RemoveWidgetsL();
    MakeNotEmptyCollectionsVisibleL();
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetStorageHandler::AddL( const CCaWidgetDescription* aWidget )
    {
    CCaInnerEntry* entry = aWidget->GetEntryLC();
    UpdateCompIdAndRemovableFlagL( aWidget->GetManifestFilePathName(), *entry );
    iStorage->AddL( entry );

    SetLocalizationsL( aWidget, entry->GetId() );

    if ( entry->GetFlags() & ERemovable )
        {
        AddWidgetToDownloadCollectionL( entry );
        }
    CleanupStack::PopAndDestroy( entry );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetStorageHandler::UpdateL( const CCaWidgetDescription* aWidget,
        TUint aEntryId )
    {
    CCaInnerEntry* entry = aWidget->GetEntryLC();
    UpdateCompIdAndRemovableFlagL( aWidget->GetManifestFilePathName(), *entry );
    entry->SetId( aEntryId );
    if ( !aWidget->IsMissing() && aWidget->IsUsed() )
        {
        entry->SetFlags( entry->GetFlags() & ~EUsed );
        }
    TItemAppearance itemAppearanceChange = EItemAppearanceNotChanged;
    if ( ( entry->GetFlags() & EMissing ) || !( entry->GetFlags() & EVisible ) )
        {
        itemAppearanceChange = EItemAppeared;
        }
    entry->SetFlags( entry->GetFlags() & ~EMissing | EVisible );
    iStorage->AddL( entry, EFalse, itemAppearanceChange );

    SetLocalizationsL( aWidget, entry->GetId() );

    if ( !aWidget->IsMissing() && ( entry->GetFlags() & ERemovable ) )
        {
        AddWidgetToDownloadCollectionL( entry );
        }
    CleanupStack::PopAndDestroy( entry );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetStorageHandler::AddWidgetsL( const RWidgetArray& aWidgets )
    {
    const TInt newWidgetsCount = aWidgets.Count();
    
    for ( TInt i = 0; i < newWidgetsCount; ++i )
        {
        CCaWidgetDescription *const newWidget = aWidgets[i];
        
        newWidget->LocalizeTextsL();
        
        const TInt dbCacheIndex = 
            iWidgetDBCache.Find( 
                newWidget, CCaWidgetDescription::CompareUri );
        
        if ( dbCacheIndex != KErrNotFound )
            {
            CCaWidgetDescription *const cachedWidget = 
                iWidgetDBCache[dbCacheIndex];
            
            cachedWidget->SetValid( ETrue );
            
            const TBool cachedDifferentThanNew = 
                !cachedWidget->Compare( *newWidget );
            
            if ( cachedDifferentThanNew || cachedWidget->IsMissing() )
                {
                newWidget->SetFlag( EMissing, cachedWidget->IsMissing() );
                newWidget->SetFlag( EUsed,  cachedWidget->IsUsed() );
                newWidget->SetFlag( EVisible, cachedWidget->IsVisible() );
                
                UpdateL( newWidget, cachedWidget->GetEntryId() );
                }
            }
        else
            {
            AddL( newWidget );
            }
        }
    HbTextResolverSymbian::Init( _L(""), KLocalizationFilepathZ );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetStorageHandler::RemoveWidgetsL()
    {
    const TInt cacheCount = iWidgetDBCache.Count();
    for ( TInt i = 0; i < cacheCount; ++i)
        {
        CCaWidgetDescription *const cachedWidget = iWidgetDBCache[i];
        if ( !cachedWidget->IsValid() )
            {
            if ( cachedWidget->GetMmcId() != KNullDesC )
                {
                RBuf currentMmcId;
                currentMmcId.CreateL( KMassStorageIdLength );
                currentMmcId.CleanupClosePushL();
                WidgetScannerUtils::CurrentMmcId( iFs, currentMmcId );
                if( cachedWidget->GetMmcId() == currentMmcId ||
                        ( cachedWidget->GetMmcId() == KCaMassStorage() &&
                        MassStorageNotInUse() ) )
                    {
                    //item was uninstalled so we remove its mmc id
                    cachedWidget->RemoveMmcId();
                    ClearVisibleFlagL( cachedWidget );
                    }
                else
                    {
                    SetMissingFlagL( cachedWidget );
                    }
                CleanupStack::PopAndDestroy( &currentMmcId );
                }
            else
                {
                //item was uninstalled so we remove its mmc id
                ClearVisibleFlagL( cachedWidget );
                }
            }
        }
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetStorageHandler::AddWidgetToDownloadCollectionL(
        const CCaInnerEntry* aEntry )
    {
    CCaInnerQuery* queryDownload = CCaInnerQuery::NewLC();
    CDesC16ArrayFlat* downloadType = new ( ELeave ) CDesC16ArrayFlat(
            KGranularityOne );
    CleanupStack::PushL( downloadType );
    downloadType->AppendL( KCaTypeCollectionDownload );
    queryDownload->SetEntryTypeNames( downloadType );//query takes ownership
    CleanupStack::Pop( downloadType );
    queryDownload->SetRole( EGroupEntryRole );

    RArray<TInt> idsGroup;
    CleanupClosePushL( idsGroup );
    iStorage->GetEntriesIdsL( queryDownload, idsGroup );

    TCaOperationParams operParams;
    operParams.iOperationType = TCaOperationParams::EPrepend;
    operParams.iGroupId = idsGroup[0];
    operParams.iBeforeEntryId = 0;
    CleanupStack::PopAndDestroy( &idsGroup );

    RArray<TInt> idsEntry;
    CleanupClosePushL( idsEntry );
    idsEntry.Append( aEntry->GetId() );
    iStorage->OrganizeL( idsEntry, operParams );

    CleanupStack::PopAndDestroy( &idsEntry );
    CleanupStack::PopAndDestroy( queryDownload );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetStorageHandler::FetchWidgetsL()
    {
    CCaInnerQuery* query = CCaInnerQuery::NewLC();
    CDesC16ArrayFlat* widgetType = new ( ELeave ) CDesC16ArrayFlat(
            KGranularityOne );
    CleanupStack::PushL( widgetType );
    widgetType->AppendL( KCaTypeWidget );
    query->SetEntryTypeNames( widgetType );//transfers ownership to query
    CleanupStack::Pop( widgetType );

    RPointerArray<CCaInnerEntry> entries;
    CleanupResetAndDestroyPushL( entries );
    iStorage->GetEntriesL( query, entries );

    iWidgetDBCache.ResetAndDestroy();
    for ( TInt i = 0; i < entries.Count(); i++ )
        {
        CCaWidgetDescription* widget = CCaWidgetDescription::NewLC( entries[i] );
        iWidgetDBCache.AppendL( widget ); //iWidgets takes ownership
        CleanupStack::Pop( widget );
        }
    CleanupStack::PopAndDestroy( &entries );
    CleanupStack::PopAndDestroy( query );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetStorageHandler::SetMissingFlagL(
        const CCaWidgetDescription* aWidget )
    {
    if ( !aWidget->IsMissing() )
        {
        CCaInnerEntry* entry = aWidget->GetEntryLC();
        entry->SetFlags( ( entry->GetFlags() | EMissing ) & ~EUninstall );
        if ( aWidget->IsUsed() )
            {
            entry->SetFlags( entry->GetFlags() | EUsed );
            }
        iStorage->AddL( entry, EFalse, EItemDisappeared );
        CleanupStack::PopAndDestroy( entry );
        }
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetStorageHandler::ClearVisibleFlagL(
        const CCaWidgetDescription* aWidget )
    {
    if ( aWidget->IsVisible() )
        {
        CCaInnerEntry* entry = aWidget->GetEntryLC();
        entry->SetFlags( entry->GetFlags()
                & ~EUninstall
                & ~EVisible 
                & ~EMissing 
                & ~EUsed );
        iStorage->AddL( entry, EFalse, EItemDisappeared );
        CleanupStack::PopAndDestroy( entry );
        }
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
TBool CCaWidgetStorageHandler::MassStorageNotInUse()
    {
    TBool massStorageNotInUse( ETrue );
    TInt drive;
    if( DriveInfo::GetDefaultDrive( 
            DriveInfo::EDefaultMassStorage, drive ) == KErrNone )
        {
        TVolumeInfo volumeInfo;
        if( iFs.Volume( volumeInfo, drive ) == KErrNotReady )
            {
            massStorageNotInUse = EFalse;
            }
        }
    return massStorageNotInUse;
    }
// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetStorageHandler::SetLocalizationsL(
        const CCaWidgetDescription* aWidget, TInt aEntryId )
    {
    RBuf filename;
    filename.CreateL( aWidget->GetTranslationFileName().Length() + 1 ); //1 for _
    CleanupClosePushL( filename );
    filename.Copy( aWidget->GetTranslationFileName() );
    filename.Append( KWidgetScannerUnderline );

    // prepare localizations
    if ( aWidget->GetTitle().Length() > 0 && aWidget->GetTitle().Compare(
            aWidget->GetStringIdTitle() ) )
    // lets do not add localization when key and value are identical
        {
        CCaLocalizationEntry* titleEntry = CCaLocalizationEntry::NewL();
        CleanupStack::PushL( titleEntry );
        titleEntry->SetTableNameL( KLocalizationCaEntry );
        titleEntry->SetAttributeNameL( KLocalizationEnText );
        titleEntry->SetStringIdL( aWidget->GetStringIdTitle() );
        titleEntry->SetQmFilenameL( filename );
        titleEntry->SetRowId( aEntryId );
        iStorage->AddLocalizationL( *titleEntry );
        CleanupStack::PopAndDestroy( titleEntry );
        }
    if ( aWidget->GetDescription().Length() > 0 &&
            aWidget->GetDescription().Compare(
                    aWidget->GetStringIdDescription() ) )
    // lets do not add localization when key and value are identical
        {
        CCaLocalizationEntry* descEntry = CCaLocalizationEntry::NewL();
        CleanupStack::PushL( descEntry );
        descEntry->SetTableNameL( KLocalizationCaEntry );
        descEntry->SetAttributeNameL( KLocalizationEnDescription );
        descEntry->SetStringIdL( aWidget->GetStringIdDescription() );
        descEntry->SetQmFilenameL( filename );
        descEntry->SetRowId( aEntryId );
        iStorage->AddLocalizationL( *descEntry );
        CleanupStack::PopAndDestroy( descEntry );
        }
    CleanupStack::PopAndDestroy( &filename );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetStorageHandler::UpdateCompIdAndRemovableFlagL(
        const TDesC& aManifestFilePathName, CCaInnerEntry& aEntry ) const
    {
    RArray<TComponentId> componentIds;
    CleanupClosePushL( componentIds );

    CComponentFilter* const fileNameFilter = CComponentFilter::NewLC();
    fileNameFilter->SetFileL( aManifestFilePathName );
    iSoftwareRegistry.GetComponentIdsL( componentIds, fileNameFilter );

    CleanupStack::PopAndDestroy( fileNameFilter );

    if( componentIds.Count() == 1 )
        {
        RBuf newComponentId;
        newComponentId.CleanupClosePushL();
        newComponentId.CreateL( sizeof(TComponentId) + 1 );
        newComponentId.AppendNum( componentIds[0] );

        RBuf oldComponentId;
        oldComponentId.CleanupClosePushL();
        oldComponentId.CreateL( KCaMaxAttrValueLen );

        const TBool componentIdAttributeFound = aEntry.FindAttribute(
                KCaComponentId, oldComponentId );

        if( !componentIdAttributeFound || oldComponentId.Compare(
                newComponentId ) != 0 )
            {
            // 'add' or 'update' the component id attribute value
            aEntry.AddAttributeL( KCaComponentId, newComponentId );
            }
        CComponentEntry* entry = CComponentEntry::NewLC();
        iSoftwareRegistry.GetComponentL( componentIds[0] , *entry );
        if( entry->IsRemovable() )
            {
            aEntry.SetFlags( aEntry.GetFlags() | ERemovable );
            }
        CleanupStack::PopAndDestroy( entry );
        CleanupStack::PopAndDestroy( &oldComponentId );
        CleanupStack::PopAndDestroy( &newComponentId );
        }

    CleanupStack::PopAndDestroy( &componentIds );
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CCaWidgetStorageHandler::MakeNotEmptyCollectionsVisibleL()
    {
    RPointerArray<CCaInnerEntry> resultArray;
    CleanupResetAndDestroyPushL( resultArray );
    CCaInnerQuery* hiddenCollectionsQuery = CCaInnerQuery::NewLC();
    CDesC16ArrayFlat* entryType = new ( ELeave ) CDesC16ArrayFlat(
            KGranularityOne );
    CleanupStack::PushL( entryType );
    entryType->AppendL( KCaTypeCollection );
    hiddenCollectionsQuery->SetEntryTypeNames( entryType );
    hiddenCollectionsQuery->SetFlagsOff( EVisible );
    iStorage->GetEntriesL( hiddenCollectionsQuery, resultArray );
    CleanupStack::Pop( entryType );
    CleanupStack::PopAndDestroy( hiddenCollectionsQuery );
    if( resultArray.Count()>0 )
        {
        for( TInt i=0; i<resultArray.Count(); i++ )
            {
            // for any not visible collection
            MakeCollectionVisibleIfHasVisibleEntryL( resultArray[i] );
            }
        }
    CleanupStack::PopAndDestroy( &resultArray );
   }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CCaWidgetStorageHandler::MakeCollectionVisibleIfHasVisibleEntryL(
        CCaInnerEntry* aEntry )
    {
    RPointerArray<CCaInnerEntry> resultEntriesArray;
    CleanupResetAndDestroyPushL( resultEntriesArray );
    CCaInnerQuery* visibleEntriesQuery = CCaInnerQuery::NewLC();
    visibleEntriesQuery->SetParentId( aEntry->GetId() );
    visibleEntriesQuery->SetFlagsOn( EVisible );
    visibleEntriesQuery->SetFlagsOff( EMissing );
    iStorage->GetEntriesL( visibleEntriesQuery, resultEntriesArray );
    if( resultEntriesArray.Count()>0 )
        {
        // set collection visible if hidden
        if( !( aEntry->GetFlags() & EVisible ) )
            {
            aEntry->SetFlags( aEntry->GetFlags() | EVisible );
            // update here this collection
            iStorage->AddL( aEntry );
            }
        }
    CleanupStack::PopAndDestroy( visibleEntriesQuery );
    CleanupStack::PopAndDestroy( &resultEntriesArray );
    }

//  End of File
