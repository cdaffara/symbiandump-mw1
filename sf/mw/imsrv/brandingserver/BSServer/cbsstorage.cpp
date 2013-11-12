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



#include "cbsstorage.h"
#include "bsimportconstants.h"
#include "DebugTrace.h"


#include <s32strm.h>
#include <utf.h>
#include <mbselement.h>

//#include "mbsimportlogger.h"
//#include "importlogwriter.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CBSStorage::NewL
// ---------------------------------------------------------------------------
//
CBSStorage* CBSStorage::NewL()
    {
    CBSStorage* self = NewLC();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// CBSStorage::NewLC
// ---------------------------------------------------------------------------
//
CBSStorage* CBSStorage::NewLC()
    {
    CBSStorage* self = new (ELeave) CBSStorage();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CBSStorage::ConstructL
// ---------------------------------------------------------------------------
//
void CBSStorage::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// CBSStorage::CBSStorage
// ---------------------------------------------------------------------------
//
CBSStorage::CBSStorage()
    {
    }

// ---------------------------------------------------------------------------
// CBSStorage::~CBSStorage
// ---------------------------------------------------------------------------
//
CBSStorage::~CBSStorage()
    {
    iElements.Close();
    delete iAppId;
    delete iStorageId;
    delete iLanguage;
    delete iFilename;
    }

// ---------------------------------------------------------------------------
// CBSStorage::AppendElement
// ---------------------------------------------------------------------------
//
void CBSStorage::AppendElementsL( MBSElement* aElement )
    {
    if( !aElement )
        {
		// Ignore empty
	    return;
        }

	TPtrC8 id = aElement->ElementId();
	
	// check if the id is unique
	TInt count = iElements.Count();
	
	for( TInt i = 0; i < count; i++ )
		{
		if( 0 == iElements[i]->ElementId().Compare( id ) )
			{
			// duplicate found
			User::Leave( KErrAlreadyExists );
			}
		}

	iElements.AppendL( aElement );
    }


// ---------------------------------------------------------------------------
// CBSStorage::ReplacesElementL
// ---------------------------------------------------------------------------
//
void CBSStorage::ReplaceElementL( MBSElement* aElement )
	{
    if( !aElement )
        {
		// Ignore empty
	    return;
        }

	TPtrC8 id = aElement->ElementId();
	
	// check if the id is unique
	TInt count = iElements.Count();
	
	for( TInt i = 0; i < count; i++ )
		{
		if( 0 == iElements[i]->ElementId().Compare( id ) )
			{
			// item found
			MBSElement* element = iElements[i];
			iElements.Remove( i );
			delete element;
			// put the new element
			iElements.AppendL( aElement );
			return;
			}
		}
	// the element was not found from this storage
	User::Leave( KErrNotFound );
	}


// ---------------------------------------------------------------------------
// CBSStorage::ElementCount
// ---------------------------------------------------------------------------
//
TInt CBSStorage::ElementCount()
    {
    return iElements.Count();
    }

// ---------------------------------------------------------------------------
// CBSStorage::ExternalizeL
// ---------------------------------------------------------------------------
//
void CBSStorage::ExternalizeL( RWriteStream& aWriteStream )
    {
    if( !VerifyHeader() || !VerifyDataL( iElements ) )
        {
        // Header not complete. Will not write to stream
        User::Leave( KErrCorrupt );
        }
    // Write header
    aWriteStream.WriteInt16L( iVersion );

    // Write elements 
    TInt count = iElements.Count();
    aWriteStream.WriteInt16L( count );
    for( TInt i = 0; i < count; i++ )
        {
        iElements[i]->ExternalizeL( aWriteStream );
        }
    }

// ---------------------------------------------------------------------------
// CBSStorage::SetVersion
// ---------------------------------------------------------------------------
//
void CBSStorage::SetVersion( TInt aVersion )
    {
    iVersion = aVersion;
    }

// ---------------------------------------------------------------------------
// CBSStorage::SetApplicationIdL
// ---------------------------------------------------------------------------
//
void CBSStorage::SetApplicationIdL( const TDesC& aAppId )
    {
    HBufC* tmp = aAppId.AllocL();
    
    delete iAppId;
    iAppId = tmp;
    }

// ---------------------------------------------------------------------------
// CBSStorage::SetStorageIdL
// ---------------------------------------------------------------------------
//
void CBSStorage::SetStorageIdL( const TDesC& aStorageId )
    {
    HBufC* tmp = aStorageId.AllocL();
    delete iStorageId;
    iStorageId = tmp;
    }

// ---------------------------------------------------------------------------
// CBSStorage::SetLanguageL
// ---------------------------------------------------------------------------
//
void CBSStorage::SetLanguageL( TLanguage aLanguage )
    {
    TBuf<KLangBufLength> buffer;
    buffer.AppendNum( aLanguage );
    
    HBufC* tmp = buffer.AllocL();
    delete iLanguage;
    iLanguage = tmp;
    }

// ---------------------------------------------------------------------------
// CBSStorage::VerifyHeader
// ---------------------------------------------------------------------------
//
TBool CBSStorage::VerifyHeader()
    {
    // not checking version as it is been consider not to use. - Pankaj - 31 may 07
    //return ( iAppId && iStorageId && iVersion > 0 && iLanguage );
    return ( iAppId && iStorageId && iLanguage );
    }


// ---------------------------------------------------------------------------
// CBSStorage::VerifyDataL
// ---------------------------------------------------------------------------
//
TBool CBSStorage::VerifyDataL( TArray<MBSElement*> aArray,
                                    TBool aIgnoreEmptyID /* = EFalse */ )
    {
    // Check that elements have unique ID's in their own namespace
    TInt count = aArray.Count();
    for( TInt i = 0; i < count; i++ )
        {
        // Verify inside list elements
        if( aArray[i]->ElementType() == EBSList )
            {
            TArray<MBSElement*> listArray = aArray[i]->GetStructureL();
            if( !VerifyDataL( listArray, ETrue ) )
                {
                return EFalse;
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
                TRACE( T_LIT("ERROR: Found duplicate element ID (%S)!"), &uniptr );
                CleanupStack::PopAndDestroy();
                return EFalse;
                }
            }
        }

    // All elements ok
    return ETrue;
    }


// ---------------------------------------------------------------------------
// CBSStorage::VerifyDataL
// ---------------------------------------------------------------------------
//
TBool CBSStorage::VerifyDataL( RBSObjOwningPtrArray<MBSElement>& aArray,
                                    TBool aIgnoreEmptyID /* = EFalse */ )
    {
    return VerifyDataL( aArray.Array(), aIgnoreEmptyID );
    }


// ---------------------------------------------------------------------------
// CBSStorage::ProposeFileNameL
// ---------------------------------------------------------------------------
//
TPtrC CBSStorage::ProposeFileNameL()
    {
    if( !iFilename )
        {
        User::Leave( KErrNotReady );
        }
    return iFilename->Des();
    }

// ---------------------------------------------------------------------------
// CBSStorage::ProposeFileNameL
// ---------------------------------------------------------------------------
//
TPtrC CBSStorage::ProposedDirL()
    {
    if( !iFilename )
        {
        User::Leave( KErrNotReady );
        }
    return iDir;
    }


// ---------------------------------------------------------------------------
// CBSStorage::GetListOfFiles()
// ---------------------------------------------------------------------------
//
void CBSStorage::GetListOfFilesL( RArray<TPtrC>& aFileList )
    {
    TInt count = iElements.Count();
    for( TInt i = 0; i < count; i++ )
        {
        AppendFilesL( iElements[i], aFileList );
        }
    }

// ---------------------------------------------------------------------------
// CBSStorage::AppendFiles()
// ---------------------------------------------------------------------------
//
void CBSStorage::AppendFilesL( MBSElement* aElement, RArray<TPtrC>& aFileList )
    {
    if( !aElement )
        {
        // invalid element
        return;
        }

    TBSElementType type = aElement->ElementType();
    if( EBSList == type )
        {
        TArray<MBSElement*> list = aElement->GetStructureL();

        TInt count = list.Count();
        for( TInt i = 0; i < count; i++ )
            {
            AppendFilesL( list[i], aFileList );
            }
        }
    else if( EBSFile == type )
        {
        TPtrC ptr( aElement->TextDataL() );
        aFileList.Append( aElement->TextDataL() );
        }
    }

// End of file
