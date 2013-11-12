/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:   Stores element data and writes it to stream
*
*/



#include <s32strm.h>
#include <utf.h>
#include <cbsfactory.h>
#include <mbsupdater.h>
#include <mbselement.h>

#include "cbsimportstorage.h"
#include "cbsimportconstants.h"
#include "importlogwriter.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CBSImportStorage::NewL
// ---------------------------------------------------------------------------
//
CBSImportStorage* CBSImportStorage::NewL()
    {
    CBSImportStorage* self = NewLC();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// CBSImportStorage::NewLC
// ---------------------------------------------------------------------------
//
CBSImportStorage* CBSImportStorage::NewLC()
    {
    CBSImportStorage* self = new (ELeave) CBSImportStorage();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CBSImportStorage::ConstructL
// ---------------------------------------------------------------------------
//
void CBSImportStorage::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// CBSImportStorage::CBSImportStorage
// ---------------------------------------------------------------------------
//
CBSImportStorage::CBSImportStorage()
    {
    }
    
// ---------------------------------------------------------------------------
// CBSImportStorage::~CBSImportStorage
// ---------------------------------------------------------------------------
//
CBSImportStorage::~CBSImportStorage()
    {
    iElements.ResetAndDestroy();
    delete iAppId;
    delete iStorageId;
    delete iFilename;
    }

// ---------------------------------------------------------------------------
// CBSImportStorage::AppendElement
// ---------------------------------------------------------------------------
//
TInt CBSImportStorage::AppendElement( MBSElement* aElement )
    {
    if( aElement )
        {
        return iElements.Append( aElement );
        }
    // Ignore empty
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CBSImportStorage::ElementCount
// ---------------------------------------------------------------------------
//
TInt CBSImportStorage::ElementCount()
    {
    return iElements.Count();
    }
    
// ---------------------------------------------------------------------------
// CBSImportStorage::Element
// ---------------------------------------------------------------------------
//
MBSElement* CBSImportStorage::Element( TInt aIndex )
    {
    return iElements[ aIndex ];
    }

// ---------------------------------------------------------------------------
// CBSImportStorage::FlushtoServerL
// ---------------------------------------------------------------------------
//
void CBSImportStorage::FlushToServerL()
    {
    if( !VerifyHeader() || !VerifyDataL( iElements.Array() ) )
        {
        // Header not complete. Will not flush
        User::Leave( KErrCorrupt );
        }

    // Create access to server
    CBSFactory* factory = CBSFactory::NewL( *iStorageId, *iAppId );
    CleanupStack::PushL( factory );
    MBSUpdater* updater = factory->CreateUpdaterLC();
    //updater->StartTransactionL( *iStorageId, iLanguage, iVersion );
    updater->StartTransactionL( *iStorageId, iLanguage );
    
    // Flush data
    TInt count = iElements.Count();
    for( TInt i = 0; i < count; i++ )
        {
        TRAPD( err, updater->InsertElementL( iElements[i] ) );
        if( err == KErrAlreadyExists )
            {
            err = KErrNone;
            TRAP( err, updater->ReplaceElementL( iElements[i] ) );
            }
        User::LeaveIfError( err );
        }
    
    // Cleanup
    updater->StopTransactionL();
    CleanupStack::PopAndDestroy( 2 ); // factory, updater
    }
    
// ---------------------------------------------------------------------------
// CBSImportStorage::SetVersion
// ---------------------------------------------------------------------------
//
void CBSImportStorage::SetVersion( TInt aVersion )
    {
    iVersion = aVersion;
    }

// ---------------------------------------------------------------------------
// CBSImportStorage::SetApplicationIdL
// ---------------------------------------------------------------------------
//
void CBSImportStorage::SetApplicationIdL( const TDesC& aAppId )
    {
    HBufC8* id = HBufC8::NewLC( aAppId.Length() );
    TPtr8 idPtr( id->Des() );
    CnvUtfConverter::ConvertFromUnicodeToUtf8( idPtr, aAppId );
    CleanupStack::Pop( id );
    delete iAppId;
    iAppId = id;   
    }

// ---------------------------------------------------------------------------
// CBSImportStorage::SetStorageIdL
// ---------------------------------------------------------------------------
//
void CBSImportStorage::SetStorageIdL( const TDesC& aStorageId )
    {
    HBufC8* id = HBufC8::NewLC( aStorageId.Length() );
    TPtr8 idPtr( id->Des() );
    CnvUtfConverter::ConvertFromUnicodeToUtf8( idPtr, aStorageId );
    CleanupStack::Pop( id );
    delete iStorageId;
    iStorageId = id;   
    }

// ---------------------------------------------------------------------------
// CBSImportStorage::SetLanguageL
// ---------------------------------------------------------------------------
//
void CBSImportStorage::SetLanguageL( TLanguage aLanguage )
    {
    iLanguage = aLanguage;
    }

// ---------------------------------------------------------------------------
// CBSImportStorage::VerifyHeader
// ---------------------------------------------------------------------------
//
TBool CBSImportStorage::VerifyHeader()
    {
    // not checking version as it is been consider not to use. - Pankaj - 31 may 07
    //return ( iAppId && iStorageId && iVersion > 0 && iLanguage >= ELangTest );
    return ( iAppId && iStorageId && iLanguage >= ELangTest  );
    
    }


// ---------------------------------------------------------------------------
// CBSImportStorage::VerifyDataL
// ---------------------------------------------------------------------------
//
TBool CBSImportStorage::VerifyDataL( TArray<MBSElement*> aArray, 
                                     TBool aIgnoreEmptyID /* = EFalse */ )
    {
    // Check that elements have unique ID's in their own namespace
    TInt count = aArray.Count();
    for( TInt i = 0; i < count; i++ )
        {
        // Verify inside list elements
        if( aArray[i]->ElementType() == EBSList )
            {
            if( !VerifyDataL( aArray[i]->GetStructureL(), ETrue ) )
                {
                return false;
                }
            }

        if( aIgnoreEmptyID && 
            aArray[i]->ElementId().CompareC( KNullDesC8() ) == 0 )
            {
            // ignore empty id
            continue;
            }
        
        // Check that sibling elements have unique ID
        for( TInt j = i + 1; j < count; j++ ) 
            {
            if( aArray[i]->ElementId().CompareC( aArray[j]->ElementId() ) == 0 )
                {
                // convert elementID to 16-bit for event logger
                TPtrC8 id( aArray[i]->ElementId() );
                HBufC* conv = HBufC::NewLC( id.Length() );
                TPtr uniptr( conv->Des() );
                CnvUtfConverter::ConvertToUnicodeFromUtf8( uniptr, id );                
                IMPORT_DP( D_IMPORT_LIT("ERROR: Found duplicate element ID (%S)!"), &uniptr );
                CleanupStack::PopAndDestroy();
                return false;
                }
            }
        }
    
    // All elements ok
    return true;
    }

// ---------------------------------------------------------------------------
// CBSImportStorage::Compare
// ---------------------------------------------------------------------------
//
TInt CBSImportStorage::Compare( const CBSImportStorage* aStorage ) const
    {
    if( iVersion != aStorage->iVersion || iLanguage != aStorage->iLanguage )
        {
        // no match
        return EFalse;
        }
    if( iAppId->Compare( aStorage->iAppId->Des() ) != 0 )
        {
        // no match
        return EFalse;
        }
    if( iStorageId->Compare( aStorage->iStorageId->Des() ) != 0 )
        {
        // no match
        return EFalse;
        }
    // they match
    return ETrue;
    }
    
// ---------------------------------------------------------------------------
// CBSImportStorage::TakeElementsL
// ---------------------------------------------------------------------------
//
void CBSImportStorage::TakeElements( CBSImportStorage* aStorage )
    {
    TInt count = aStorage->iElements.Count();
    for( TInt i = 0; i < count; i++ )
        {
        iElements.Append( aStorage->iElements[i] );
        }
    aStorage->iElements.Reset();
    }
    
// End of file
