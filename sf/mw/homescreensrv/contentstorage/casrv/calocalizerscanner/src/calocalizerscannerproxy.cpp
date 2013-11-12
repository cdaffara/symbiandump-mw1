/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: CCaLocalizerScannerProxy.cpp
 *
 */

#include <badesca.h>
#include <f32file.h>
#include <qtranslator.h>
#include <calocalizationentry.h>
#include <hbtextresolversymbian.h>

//#include "cainternaltypes.h"
#include "caarraycleanup.inl"
#include "calocalizerscannerproxy.h"
#include "castorageproxy.h"
#include "cadef.h"
#include "cainnerquery.h"
#include "cainnerentry.h"
#include "casqlcommands.h"
#include "cautils.h"



// ---------------------------------------------------------------------------
// CCaLocalizerScannerProxy::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCaLocalizerScannerProxy* CCaLocalizerScannerProxy::NewL(
        CCaStorageProxy* aStorageProxy )
    {
    CCaLocalizerScannerProxy* self = CCaLocalizerScannerProxy::NewLC(
        aStorageProxy );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CCaLocalizerScannerProxy::NewLC
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCaLocalizerScannerProxy* CCaLocalizerScannerProxy::NewLC(
        CCaStorageProxy* aStorageProxy )
    {
    CCaLocalizerScannerProxy* self = new( ELeave ) CCaLocalizerScannerProxy(
        aStorageProxy );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CCaLocalizerScannerProxy::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CCaLocalizerScannerProxy::ConstructL()
    {
	iRecentQmFile.CreateL( KCaMaxAttrLenght );
    UpdateLocalNamesL();
    }

// ---------------------------------------------------------------------------
// CCaLocalizerScannerProxy::CCaLocalizerScannerProxy
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CCaLocalizerScannerProxy::CCaLocalizerScannerProxy(
        CCaStorageProxy* aStorageProxy ) :
    iStorageProxy( aStorageProxy )
    {
    }

// ---------------------------------------------------------------------------
// CCaLocalizerScannerProxy::~CCaLocalizerScannerProxy
// Destructor.
// ---------------------------------------------------------------------------
//
CCaLocalizerScannerProxy::~CCaLocalizerScannerProxy()
    {
	iRecentQmFile.Close();
    }

// ---------------------------------------------------------------------------
// CCaLocalizerScannerProxy::GetLocalizationRowsL
// ---------------------------------------------------------------------------
//
void CCaLocalizerScannerProxy::GetLocalizationRowsL( RPointerArray<
        CCaLocalizationEntry>& aLocals )
    {
    if( iStorageProxy )
        {
        iStorageProxy->GetLocalizationsL( aLocals );
        }
    }

// ---------------------------------------------------------------------------
// CCaLocalizerScannerProxy::UpdateLocalNamesL
// ---------------------------------------------------------------------------
//
void CCaLocalizerScannerProxy::UpdateLocalNamesL()
    {
    RPointerArray<CCaLocalizationEntry> locals;
    CleanupResetAndDestroyPushL( locals );
    RPointerArray<CCaInnerEntry> entries;
    CleanupResetAndDestroyPushL( entries );
    RArray<TInt> ids;
    CleanupClosePushL( ids );
    //gets all localizations
    GetLocalizationRowsL( locals );
    TInt locCount = locals.Count();
    for( TInt i = 0; i < locCount; i++ )
        {
        ids.Append( locals[i]->GetRowId() );
        }    
    CCaInnerQuery* query = CCaInnerQuery::NewLC();
    query->SetIdsL( ids );
    iStorageProxy->GetEntriesL( query, entries ); 
    CleanupStack::PopAndDestroy( query );
    
    HBufC16* localizedName;
    for( TInt i = 0; i < locCount; i++ )
        {
        localizedName = GetLocalizedNameLC( locals[i] );
        
        if( locals[i]->GetAttributeName().Compare( KColumnEnText) == 0
                && localizedName->Compare(
                    GetEntryText( entries, locals[i]->GetRowId() ) ) != 0 
                ||
            locals[i]->GetAttributeName().Compare( KColumnEnDescription) == 0
                && localizedName->Compare(
                    GetEntryDescription( entries, locals[i]->GetRowId() ) ) != 0
                ||
            ( locals[i]->GetAttributeName().Compare( KShortName ) == 0
                || locals[i]->GetAttributeName().Compare( KTitleName ) == 0 )
                && localizedName->Compare(
                    GetAttributeName( entries,
                        locals[i]->GetRowId(),
                        locals[i]->GetAttributeName() ) ) != 0 )
            // translations different than text
            {
            locals[i]->SetLocalizedStringL( *localizedName );
            iStorageProxy->LocalizeEntryL( *( locals[i] ) );
            }
        CleanupStack::PopAndDestroy( localizedName );
        }
    HbTextResolverSymbian::Init( _L(""), KLocalizationFilepathZ );
   
    CleanupStack::PopAndDestroy( &ids );
    CleanupStack::PopAndDestroy( &entries );
    CleanupStack::PopAndDestroy( &locals );
    }


// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
HBufC* CCaLocalizerScannerProxy::GetLocalizedNameLC(
		const CCaLocalizationEntry* aLocEntry)
    {
	if( iRecentQmFile.Compare( aLocEntry->GetQmFilename() ) )
		{
		MenuUtils::InitTextResolverSymbianL(aLocEntry->GetQmFilename());
        // keeping last qm filename to avoid another initialization
	    iRecentQmFile.Close();
	    iRecentQmFile.CreateL( aLocEntry->GetQmFilename().Length() );
	    iRecentQmFile.Copy( aLocEntry->GetQmFilename() );
		}
	return HbTextResolverSymbian::LoadLC( aLocEntry->GetStringId() );
    }

// ---------------------------------------------------------------------------
//  
// ---------------------------------------------------------------------------
//
const TDesC& CCaLocalizerScannerProxy::GetEntryText(
        const RPointerArray<CCaInnerEntry>& aEntries, TInt aId )
    {
    TInt entriesCount = aEntries.Count();
    for( TInt i=0; i < entriesCount; i++ )
        {
        if( aEntries[i]->GetId() == aId )
            {
            return aEntries[i]->GetText();
            }
        }
    return KNullDesC();
    }

// ---------------------------------------------------------------------------
//  
// ---------------------------------------------------------------------------
//
const TDesC& CCaLocalizerScannerProxy::GetEntryDescription(
        const RPointerArray<CCaInnerEntry>& aEntries, TInt aId )
    {
    TInt entriesCount = aEntries.Count();
    for( TInt i=0; i < entriesCount; i++ )
        {
        if( aEntries[i]->GetId() == aId )
            {
            return aEntries[i]->GetDescription();
            }
        }
    return KNullDesC();
    }

// ---------------------------------------------------------------------------
//  
// ---------------------------------------------------------------------------
//
const TPtrC CCaLocalizerScannerProxy::GetAttributeName(
        const RPointerArray<CCaInnerEntry>& aEntries,
        TInt aId,
        const TDesC& aAttrName )
    {
    TBool notFound( ETrue );
    TPtrC attrValue;
    TInt entriesCount = aEntries.Count();
    for( TInt i=0; i < entriesCount && notFound; i++ )
        {
        if( aEntries[i]->GetId() == aId &&
            aEntries[i]->GetAttributes().Find(aAttrName, attrValue) )
            {
            notFound = EFalse;
            }
        }
    return attrValue;
    }
