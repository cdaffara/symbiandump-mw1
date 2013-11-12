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

#include <driveinfo.h>
#include <hbtextresolversymbian.h>

#include "castorage.h"
#include "castorageproxy.h"
#include "castoragefactory.h"
#include "cainnerentry.h"
#include "cainnerquery.h"
#include "caarraycleanup.inl"
#include "calocalizationentry.h"
#include "casrvsession.h"



// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CCaStorageProxy::CCaStorageProxy()
    {
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCaStorageProxy::ConstructL()
    {
    iStorage = CaStorageFactory::NewDatabaseL();
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CCaStorageProxy* CCaStorageProxy::NewL()
    {
    CCaStorageProxy* self = CCaStorageProxy::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CCaStorageProxy* CCaStorageProxy::NewLC()
    {
    CCaStorageProxy* self = new ( ELeave ) CCaStorageProxy();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CCaStorageProxy::~CCaStorageProxy()
    {
    iTitleUserColName.Close();
    delete iStorage;
    iHandlerNotifier.Close();
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaStorageProxy::GetEntriesL(const CCaInnerQuery* aQuery,
        RPointerArray<CCaInnerEntry>& aResultContainer )
    {
    iStorage->GetEntriesL( aQuery, aResultContainer );
    if( aResultContainer.Count() == 1 
            && aResultContainer[0]->
            GetEntryTypeName().Compare( KCaTypeCollection ) == KErrNone )
        {
        TPtrC titleName;
        if( !aResultContainer[0]->FindAttribute( KCaAttrTitleName, titleName ) )
            {
            AddTitleNameL( aResultContainer[0] );
            }
        }
    
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaStorageProxy::GetEntriesIdsL(const CCaInnerQuery* aQuery,
        RArray<TInt>& aResultIdArray)
    {
    iStorage->GetEntriesIdsL( aQuery, aResultIdArray );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaStorageProxy::AddL( CCaInnerEntry* aEntry,
    TBool aUpdate,
    TItemAppearance aItemAppearanceChange )
    {
    TChangeType changeType = EAddChangeType;
    RArray<TInt> parentArray;
    CleanupClosePushL( parentArray );
          
    if( aEntry->GetId() > 0 )
        {
        changeType = EUpdateChangeType;
        RArray<TInt> id;
        CleanupClosePushL( id );
        id.AppendL( aEntry->GetId() );
        if (aItemAppearanceChange == EItemUninstallProgressChanged)
            {
            // no need to search for parent parents for uninstall 
            // progress change
            iStorage->GetParentsIdsL( id, parentArray, EFalse );
            }
        else
            {
            iStorage->GetParentsIdsL( id, parentArray );
            }
        CleanupStack::PopAndDestroy( &id );
        }

    if( aItemAppearanceChange==EItemDisappeared )
        {
        changeType = ERemoveChangeType;
        }
    else if( aItemAppearanceChange==EItemAppeared )
        {
        changeType = EAddChangeType;
        }
    
    // do not update entry in db with uninstall progress
    if (aItemAppearanceChange != EItemUninstallProgressChanged)
        {
        RPointerArray<CCaLocalizationEntry> localizations;
	    CleanupResetAndDestroyPushL( localizations );
        CCaLocalizationEntry* tempLocalization = NULL;
        if( aEntry->isLocalized( CCaInnerEntry::ENameLocalized ) )		
            {
            tempLocalization = LocalizeTextL( aEntry );
            if( tempLocalization )
                {
                localizations.Append( tempLocalization );
                tempLocalization = NULL;
                }
            }
        if( aEntry->isLocalized( CCaInnerEntry::EDescriptionLocalized ) )
            {
            tempLocalization = LocalizeDescriptionL( aEntry );
            if (tempLocalization)
                {
                localizations.Append(tempLocalization);
                tempLocalization = NULL;
                }
            }
        
        iStorage->AddL( aEntry, aUpdate );
        
        for( TInt j =0; j < localizations.Count(); j++ )
            {
            localizations[j]->SetRowId( aEntry->GetId() );
            AddLocalizationL( *( localizations[j] ) );
            }
		if( localizations.Count() > 0 )
            {
            HbTextResolverSymbian::Init( _L(""), KLocalizationFilepathZ );
            }
		 CleanupStack::PopAndDestroy( &localizations );
        }
    
        
    for( TInt i = 0; i < iHandlerNotifier.Count(); i++ )
        {
        iHandlerNotifier[i]->EntryChanged( aEntry, changeType, parentArray );
        }
    CleanupStack::PopAndDestroy( &parentArray );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaStorageProxy::RemoveL( const RArray<TInt>& aEntryIds )
    {
    CCaInnerQuery* query = CCaInnerQuery::NewLC();
    query->SetIdsL( aEntryIds );
    RPointerArray<CCaInnerEntry> resultContainer;
    CleanupResetAndDestroyPushL( resultContainer );
    RArray<TInt> parentArray;
    CleanupClosePushL( parentArray );
    if( aEntryIds.Count() > 0 )
        {
        iStorage->GetEntriesL( query, resultContainer );
        iStorage->GetParentsIdsL( aEntryIds, parentArray );
        }
    iStorage->RemoveL( aEntryIds );
    for( TInt i( 0 ); i < resultContainer.Count(); i++ )
        {
        for( TInt j( 0 ); j < iHandlerNotifier.Count(); j++ )
            {
            iHandlerNotifier[j]->EntryChanged( resultContainer[i],
                    ERemoveChangeType, parentArray );
            }
        }
    CleanupStack::PopAndDestroy( &parentArray );
    CleanupStack::PopAndDestroy( &resultContainer );
    CleanupStack::PopAndDestroy( query );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaStorageProxy::OrganizeL( const RArray<TInt>& aEntryIds,
        TCaOperationParams aParams )
    {
    iStorage->OrganizeL( aEntryIds, aParams );
    RArray<TInt> parentArray;
    CleanupClosePushL( parentArray );
    parentArray.AppendL( aParams.iGroupId );
    iStorage->GetParentsIdsL( parentArray, parentArray );
    for( TInt i = 0; i < iHandlerNotifier.Count(); i++ )
        {
        iHandlerNotifier[i]->GroupContentChanged( parentArray );
        }
    CleanupStack::PopAndDestroy( &parentArray );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaStorageProxy::TouchL( CCaInnerEntry* aEntry )
    {
    CCaInnerQuery* touchQuery = CCaInnerQuery::NewLC();

    TInt entryId = aEntry->GetId();

    if ( entryId == 0 && aEntry->GetUid() != 0)
        {
        CCaInnerQuery* idQuery = CCaInnerQuery::NewLC();
        idQuery->SetUid( static_cast<TUint>( aEntry->GetUid()) );

        RArray<TInt> idArray;
        CleanupClosePushL( idArray );

        iStorage->GetEntriesIdsL( idQuery, idArray );

        if (idArray.Count() == 1 )
            {
            entryId = idArray[0];
            aEntry->SetId( entryId );
            }

        CleanupStack::PopAndDestroy( &idArray );
        CleanupStack::PopAndDestroy( idQuery );
        }

    RArray<TInt> id;
    CleanupClosePushL( id );
    id.AppendL( entryId );
    touchQuery->SetIdsL( id );
    RPointerArray<CCaInnerEntry> resultArray;
    CleanupResetAndDestroyPushL( resultArray );
    iStorage->GetEntriesL( touchQuery, resultArray );
    iStorage->TouchL( entryId, aEntry->GetFlags() & ERemovable );
    for( TInt i = 0; i < iHandlerNotifier.Count(); i++ )
        {
        iHandlerNotifier[i]->EntryTouched( entryId );
        }
    if( resultArray.Count() > 0 )
        {
        if( !( resultArray[0]->GetFlags() & EUsed ) )
            {
            RArray<TInt> parentArray;
            CleanupClosePushL( parentArray );
            iStorage->GetParentsIdsL( id, parentArray );
            for( TInt i = 0; i < iHandlerNotifier.Count(); i++ )
                {
                resultArray[0]->SetFlags( 
                        resultArray[0]->GetFlags() | EUsed );
                iHandlerNotifier[i]->EntryChanged( resultArray[0],
                        EUpdateChangeType,
                        parentArray );
                }
            CleanupStack::PopAndDestroy( &parentArray );
            }
        }
    CleanupStack::PopAndDestroy( &resultArray );
    CleanupStack::PopAndDestroy( &id );
    CleanupStack::PopAndDestroy( touchQuery );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaStorageProxy::GetLocalizationsL(
        RPointerArray<CCaLocalizationEntry>& aResultArray )
    {
    iStorage->GetLocalizationsL( aResultArray );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaStorageProxy::LocalizeEntryL(
        CCaLocalizationEntry& aLocalization )
    {
    iStorage->LocalizeEntryL( aLocalization );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaStorageProxy::AddLocalizationL(
		const CCaLocalizationEntry& aLocalization)
	{
	iStorage->AddLocalizationL(aLocalization);
	}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaStorageProxy::DbPropertyL( const TDesC& aProperty,
        TDes& aPropertyValue )
    {
    iStorage->DbPropertyL( aProperty, aPropertyValue );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaStorageProxy::SetDBPropertyL( const TDesC& aProperty,
        const TDesC& aPropertyValue )
    {
    iStorage->SetDBPropertyL( aProperty, aPropertyValue );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaStorageProxy::CustomSortL( const RArray<TInt>& aEntryIds,
        const TInt aGroupId )
    {
    iStorage->CustomSortL( aEntryIds, aGroupId );

    RArray<TInt> parentArray;
    CleanupClosePushL( parentArray );
    parentArray.AppendL( aGroupId );
    iStorage->GetParentsIdsL( parentArray, parentArray );
    for( TInt i = 0; i < iHandlerNotifier.Count(); i++ )
        {
        iHandlerNotifier[i]->GroupContentChanged( parentArray );
        }
    CleanupStack::PopAndDestroy( &parentArray );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
#ifdef COVERAGE_MEASUREMENT
#pragma CTC SKIP
#endif //COVERAGE_MEASUREMENT (calls another method)

EXPORT_C void CCaStorageProxy::SaveDatabaseL()
    {
    iStorage->SaveDatabaseL();
    }
#ifdef COVERAGE_MEASUREMENT
#pragma CTC ENDSKIP
#endif //COVERAGE_MEASUREMENT


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
#ifdef COVERAGE_MEASUREMENT
#pragma CTC SKIP
#endif //COVERAGE_MEASUREMENT (calls another method)

EXPORT_C void CCaStorageProxy::RestoreDatabaseL()
    {
    iStorage->RestoreDatabaseL();
    }
#ifdef COVERAGE_MEASUREMENT
#pragma CTC ENDSKIP
#endif //COVERAGE_MEASUREMENT


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
#ifdef COVERAGE_MEASUREMENT
#pragma CTC SKIP
#endif //COVERAGE_MEASUREMENT (calls another method)

EXPORT_C void CCaStorageProxy::LoadDataBaseFromRomL()
    {
    iStorage->LoadDataBaseFromRomL();
    }
#ifdef COVERAGE_MEASUREMENT
#pragma CTC ENDSKIP
#endif //COVERAGE_MEASUREMENT

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CCaStorageProxy::AddSessionL( MCaSessionNorifier* aHandlerNotifier )
    {
    __ASSERT_ALWAYS(
            iHandlerNotifier.Find( aHandlerNotifier ) == KErrNotFound,
            User::Invariant() );
    iHandlerNotifier.AppendL( aHandlerNotifier );
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CCaStorageProxy::RemoveSession( MCaSessionNorifier* aHandlerNotifier )
    {
    TInt i = iHandlerNotifier.Find( aHandlerNotifier );
    if( i != KErrNotFound )
        {
        iHandlerNotifier.Remove( i );
        }
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CCaLocalizationEntry* CCaStorageProxy::LocalizeTextL( CCaInnerEntry* aEntry )
    {
	CCaLocalizationEntry* result = NULL;
	TInt textLength = aEntry->GetText().Length();
	if( textLength > 0 )
		{
		TChar delimiter = '/'; // cannot add it as global
		RBuf title;
		CleanupClosePushL( title );
		title.CreateL( aEntry->GetText() );
		TInt pos = title.LocateReverse( delimiter );
		if ( pos > 0 && pos + 1 < textLength )   // 1 is for delimiters
			{
			TPtrC16 logString = title.Mid( pos + 1 ); 
			TInt qmFileNameLength = 
			        textLength - charsToFilename - 1 - logString.Length();
			TPtrC16 qmFile = title.Mid( charsToFilename, qmFileNameLength );
			if ( InitializeTranslatorL( qmFile ) )
				{
			    result = CCaLocalizationEntry::NewLC();			
				HBufC* translatedString = 
				        HbTextResolverSymbian::LoadLC( logString );
				if ( translatedString->Compare( logString ) )
					{
					result->SetStringIdL( logString );
					aEntry->SetTextL( *translatedString );
					if( translatedString )
						{
					    CleanupStack::PopAndDestroy( translatedString );
						}
					result->SetTableNameL( KLocalizationCaEntry );
					result->SetAttributeNameL( KLocalizationEnText );
					result->SetQmFilenameL( qmFile );
					result->SetRowId( aEntry->GetId() ? 0 : aEntry->GetId() ); // must be added when present
					CleanupStack::Pop( result );
					}
				else 
					{
					CleanupStack::PopAndDestroy(translatedString);
					CleanupStack::PopAndDestroy(result);
					result = NULL;
					}
				}
			}
		CleanupStack::PopAndDestroy( &title );
		}
	return result;
	}

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CCaLocalizationEntry* CCaStorageProxy::LocalizeDescriptionL( CCaInnerEntry* aEntry )
    {
	CCaLocalizationEntry* result = NULL;
	TInt dscLength = aEntry->GetDescription().Length();
	if( dscLength )
		{
		TChar delimiter = '/'; // cannot add it as global
		RBuf description;
		CleanupClosePushL( description );
		description.CreateL( aEntry->GetDescription() );
		TInt pos = description.LocateReverse( delimiter );
		if ( pos > 0 && pos + 1 < dscLength )   // 1 is for delimiters
			{
			TPtrC16 logString = description.Mid(pos + 1);
			TInt qmFileNameLength = dscLength - charsToFilename - 1 - logString.Length();
			TPtrC16 qmFile = description.Mid(charsToFilename, qmFileNameLength);
			if ( InitializeTranslatorL( qmFile ) )
				{
			    result = CCaLocalizationEntry::NewLC();
				HBufC* translatedString = HbTextResolverSymbian::LoadLC( logString );
				if ( translatedString->Compare( logString ) )
					{
					result->SetStringIdL( logString );
					aEntry->SetDescriptionL( *translatedString );
					CleanupStack::PopAndDestroy( translatedString );
					result->SetTableNameL( KLocalizationCaEntry );
					result->SetAttributeNameL( KLocalizationEnDescription );
					result->SetQmFilenameL( qmFile );
					result->SetRowId( aEntry->GetId() ? 0 : aEntry->GetId() ); // must be added when present
					CleanupStack::Pop( result );
					}
				else 
					{
				    CleanupStack::PopAndDestroy( translatedString );
				    CleanupStack::PopAndDestroy( result );
				    result = NULL;
					}
				
				}
			}
		CleanupStack::PopAndDestroy( &description );
		}
	
	return result;
	}

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CCaStorageProxy::AddTitleNameL( CCaInnerEntry* aEntry )
    {
    if( !iTitleUserColName.Length() )
        {
        if ( InitializeTranslatorL( KCaQmFile ) )
            {
            HBufC* translatedString = 
                    HbTextResolverSymbian::LoadLC( KDefaultLocTitleName );
            if ( translatedString->Compare( KDefaultLocTitleName ) )
                {
                iTitleUserColName.CreateL( *translatedString );
                }
            CleanupStack::PopAndDestroy( translatedString );
            }
        }
    if( iTitleUserColName.Length() )
        {
        aEntry->AddAttributeL( KCaAttrTitleName, iTitleUserColName );
        }
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
TBool CCaStorageProxy::InitializeTranslatorL( const TDesC& aQmFilename )
    {
	TBool result = HbTextResolverSymbian::Init( aQmFilename, KLocalizationFilepathC );
	if ( !result )
		{
		// this should not be called too often 
		TChar currentDriveLetter;
		TDriveList driveList;
		RFs fs;
		CleanupClosePushL( fs );
		User::LeaveIfError( fs.Connect() );
		User::LeaveIfError( fs.DriveList( driveList ) );

		RBuf path;
		CleanupClosePushL( path );
		path.CreateL( KLocalizationFilepath().Length() + 1 );

		for ( TInt driveNr = EDriveY; driveNr >= EDriveA; driveNr-- )
			{
			if ( driveList[driveNr] )
				{
				User::LeaveIfError(fs.DriveToChar( driveNr, currentDriveLetter ));
				path.Append( currentDriveLetter );
				path.Append( KLocalizationFilepath );
				if (HbTextResolverSymbian::Init( aQmFilename, path ))
					{
				    result = ETrue;
					break;
					}
				}
			path.Zero();
			}
		CleanupStack::PopAndDestroy( &path );
		CleanupStack::PopAndDestroy( &fs );
		
		if( !result )
			{
		    result = HbTextResolverSymbian::Init( aQmFilename, KLocalizationFilepathZ );
			}
		}
	return result;
	}
