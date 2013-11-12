/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  CBSBrandHandler.cpp
*
*/



//  INCLUDE FILES

#include "cbsbrandhandler.h"
#include "bselementfactory.h"
#include "DebugTrace.h"
#include "cbsstoragemanager.h"
#include "cbsbitmap.h"
#include "bsimportconstants.h"

#include <e32base.h>
#include <utf.h>
#include <s32file.h>

void Panic(TInt aPanic)
    {
    _LIT( KPanic, "CBS" );
    User::Panic( KPanic, aPanic );
    }

// Two-phased constructor.
CBSBrandHandler* CBSBrandHandler::NewL( const TDesC& aApplicationId,
									    const TDesC& aBrandId, 
									    const TDesC& aDefaultBrandId, 
									    TLanguage aLanguage,
									    CBSSession* aSession,
									    TInt aReserved )
    {
    CBSBrandHandler* self = new ( ELeave ) CBSBrandHandler( aLanguage, aReserved ) ;
    CleanupStack::PushL( self );
    self->ConstructL( aApplicationId, aBrandId, aDefaultBrandId, aSession );
    CleanupStack::Pop( self );  //self
    return self;
    }

// Symbian OS default constructor can leave.
void CBSBrandHandler::ConstructL( const TDesC& aApplicationId,
						 		  const TDesC& aBrandId,
						 		  const TDesC& aDefaultBrandId,
						 		  CBSSession* aSession )
    {
	iApplicationId = aApplicationId.AllocL();
	iBrandId = aBrandId.AllocL();
	iDefaultBrandId = aDefaultBrandId.AllocL();
	iSession = aSession;
	
	User::LeaveIfError( iFs.Connect() );
	
	iHandle = new(ELeave) RFile(); // CSI: 74 # this needs to be like this

	isDefaultBrandUsed = ETrue;
	iStorageManager = CBSStorageManager::NewL( iSession, KNullDesC );
    TInt err = -1;
    TRAP (err, iStorageManager->BrandHandleL( *iApplicationId,
                                   *iBrandId, iLanguage,
                                   *iHandle,
                                   iReserved ));


    if (err != KErrNone)
        {
	iStorageManager->BrandHandleL( *iApplicationId,
                                           *iDefaultBrandId, iLanguage,
								   *iHandle,
								   iReserved );
        }
	VerifyVersionL();
    }

// Destructor
CBSBrandHandler::~CBSBrandHandler()
    {
    delete iDefaultBrand;
    delete iApplicationId;
    delete iBrandId;
    delete iDefaultBrandId;
  	if( iHandle )
  		{
  		iHandle->Close();
		delete iHandle;
		iHandle = NULL;
  		}

    delete iStorageManager;
  
    iFs.Close();
    }

// C++ default constructor can NOT contain any code, that
// might leave.
//
CBSBrandHandler::CBSBrandHandler( TLanguage aLanguage,
						 		  TInt aReserved )
: iLanguage( aLanguage ), iReserved( aReserved)
	{
	}



TInt CBSBrandHandler:: isBrandUpdateRequiredL ()
{
	TRACE( T_LIT( "isBrandUpdateRequired  entered"));
	TInt updateRequired = -1;
	if (isDefaultBrandUsed)
		{
		TRACE( T_LIT( "isBrandUpdateRequired  isDefaultBrandused is TRUE."));
		/* default brand is used, so can check if the actual brand is installed by anychance or not */
		updateRequired = iStorageManager->isActualBrandInstalledL (*iApplicationId, *iBrandId, iLanguage );
		if (1 == updateRequired)
			{
				TRACE( T_LIT( "isBrandUpdateRequired  isDefaultBrandused returned 1, so update required, setting defaultbrand FALSE."));
				isDefaultBrandUsed = EFalse;
			}
		}
	TRACE( T_LIT( "isBrandUpdateRequired  isDefaultBrandused leaving.."));		
	return updateRequired;
}

// -----------------------------------------------------------------------------
// CBSBrandHandler::SetDefaultBrandIdL()
// -----------------------------------------------------------------------------
//
void CBSBrandHandler::SetDefaultBrandIdL( const TDesC8& aBrandId )
    {
    HBufC* temp = CnvUtfConverter::ConvertToUnicodeFromUtf8L( aBrandId );
    delete iDefaultBrand;
    iDefaultBrand = temp;
    }

// -----------------------------------------------------------------------------
// CBSBrandHandler::GetTextL()
// -----------------------------------------------------------------------------
//
HBufC* CBSBrandHandler::GetTextL( const TDesC8& aId )
	{
	TRACE( T_LIT( "CBranding::GetTextL begin [%S]"), &aId);
	
	MBSElement* element = ReadElementLC( aId );

	HBufC* returnValue = element->TextDataL().AllocL();

	CleanupStack::PopAndDestroy(); // element
	TRACE( T_LIT( "CBranding::GetTextL end") );
    return returnValue;
	}

// -----------------------------------------------------------------------------
// CBSBrandHandler::GetBufferL()
// -----------------------------------------------------------------------------
//
HBufC8* CBSBrandHandler::GetBufferL( const TDesC8& aId )
	{
	TRACE( T_LIT( "CBSBrandHandler::GetBufferL begin") );
	
	MBSElement* element = ReadElementLC( aId );

	HBufC8* returnValue = element->BufferDataL().AllocL();

	CleanupStack::PopAndDestroy(); // element
	TRACE( T_LIT( "CBSBrandHandler::GetBufferL end") );
    return returnValue;
	}

// -----------------------------------------------------------------------------
// CBSBrandHandler::GetIntL()
// -----------------------------------------------------------------------------
//
TInt CBSBrandHandler::GetIntL( const TDesC8& aId )
	{
    TRACE( T_LIT( "CBSBrandHandler::GetIntL begin") );
	TInt value = 0;

	MBSElement* element = ReadElementLC( aId );

	value = element->IntDataL();

	CleanupStack::PopAndDestroy(); // element

	TRACE( T_LIT( "CBSBrandHandler::GetIntL end") );
    return value;
	}

// -----------------------------------------------------------------------------
// CBSBrandHandler::GetFileL()
// -----------------------------------------------------------------------------
//
void CBSBrandHandler::GetFileL( const TDesC8& aId, RFile& aFile )
	{
	TRACE( T_LIT( "CBSBrandHandler::GetFileL begin aId[%S] "), &aId );
	RFile file;
	User::LeaveIfError( iFs.ShareProtected() );

	if (iLanguage >= 100)
		User::LeaveIfError (KErrNotFound);
	HBufC* fileName = GetTextL( aId );

    TBuf<KLangBufLength> buffer;
// append leading zero only if language code is <10.
    if ( 10 > iLanguage )
        {
    	buffer.AppendNum( KLeadingZero );
        }		

    buffer.AppendNum( iLanguage );

	TInt err = -1;
	TRAP (err, iStorageManager->FileElementHandleL( *iApplicationId,
						     			 *iBrandId,
						     			 *fileName,
						     			 buffer,
						     			 file ));

	if (KErrNone != err)
	    {
		/* if the file is not found in the default brand also, then leave */
	    iStorageManager->FileElementHandleL( *iApplicationId,
	                                         *iDefaultBrandId,
	                                         *fileName,
	                                         buffer,
	                                         file);

		TRACE( T_LIT( "CBSBrandHandler::GetFileL found in default brand") );
	    
	    }

	aFile = file;
    TRACE( T_LIT( "CBSBrandHandler::GetFileL end") );
	}


// -----------------------------------------------------------------------------
// CBSBrandHandler::GetSeveralL()
// -----------------------------------------------------------------------------
//
MBSElement* CBSBrandHandler::GetSeveralL( RBSObjOwningPtrArray<HBufC8>& aIds )
	{
	MBSElement* returnValue = NULL;
	TInt count = aIds.Count();
	
	RBSObjOwningPtrArray<MBSElement> listData;
	CleanupClosePushL( listData );
	for(TInt i = 0; i < count; i++ )
		{
		MBSElement* subElement = ReadElementLC( *aIds[ i ] );
		listData.AppendL( subElement );
		CleanupStack::Pop( subElement );
		}
	returnValue = BSElementFactory::CreateBSElementL( KNullDesC8,
													  EBSList,
													  listData );
	CleanupStack::Pop(); // listData
	return returnValue;
	}


// -----------------------------------------------------------------------------
// CBSBrandHandler::GetStructureL()
// -----------------------------------------------------------------------------
//
MBSElement* CBSBrandHandler::GetStructureL( TDesC8& aId )
	{
	MBSElement* returnValue = NULL;

	TRACE( T_LIT( "CBSBrandHandler::GetStructureL begin") );
	
	returnValue = ReadElementLC( aId );

	CleanupStack::Pop(); // element

	TRACE( T_LIT( "CBSBrandHandler::GetStructureL end") );

	return returnValue;
	}


// -----------------------------------------------------------------------------
// CBSBrandHandler::ReadElementLC()
// -----------------------------------------------------------------------------
//
MBSElement* CBSBrandHandler::ReadElementLC( const TDesC8& aId, TBool aForceDefault /*= EFalse*/ )
	{
	TRACE( T_LIT( "CBSBrandHandler::ReadElementLC begin aId"));

	if( aForceDefault )
		{
		TRACE( T_LIT( "CBSBrandHandler::ReadElementLC default brand"));
		iStorageManager->BrandHandleL( *iApplicationId,
									   *iDefaultBrandId, iLanguage,
									   *iHandle,
									   iReserved );		
		}
	else
		{
		TInt err = -1;
		TRAP (err, iStorageManager->BrandHandleL( *iApplicationId,
									   *iBrandId, iLanguage,
									   *iHandle,
									   iReserved ));
		if (KErrNone != err)
			{
			iStorageManager->BrandHandleL( *iApplicationId,
										   *iDefaultBrandId, iLanguage,
										   *iHandle,
										   iReserved ); 	
			}
		}
	
	RFileReadStream stream;
	stream.Attach( *iHandle );
	CleanupClosePushL( stream );
	
	VerifyVersionL( stream );
	
	TInt count = stream.ReadInt16L();
	
	MBSElement* returnValue = NULL;

	for( TInt i = 0; i < count; i++ )
		{
		TRAPD( err, returnValue = ReadStreamL( aId, stream ) );
		
		if( err == KErrEof )
			{
			TRACE( T_LIT( "CBSBrandHandler::ReadElementLC EOF!") );
			// the id is not found in this file
			User::Leave( KErrNotFound );
			}
		if( returnValue )
			{
			TRACE( T_LIT( "CBSBrandHandler::ReadElementLC ELEMENT FOUND.. at position %d"), i);
			// we found what we are looking for
			break;
			}
		}
		
	CleanupStack::PopAndDestroy( &stream ); // stream	
	
	TBool popElementFromCleanupStack( EFalse );
	
	/* If retur value is not found and if its read the actual brand, then try in default brand as well. aForceDefault will decide that. */
	if( !returnValue && !aForceDefault)
		{
		TRACE( T_LIT( "CBSBrandHandler::ReadElementLC force default is true") );

		// the element was not found
		// try the default brand if it's not the same as wanted brand
		if( 0 != iBrandId->Compare( *iDefaultBrandId ) )
			{
			TRACE( T_LIT( "CBSBrandHandler::ReadElementLC calling READELEMENTLC again") );

			/* Call ReadElementLC wiht aForceDefault set to TRUE */
			returnValue = ReadElementLC( aId, ETrue );

			if ( returnValue )
			    {
				TRACE( T_LIT( "CBSBrandHandler::ReadElementLC VALUE IS FOUND!!!") );
			    popElementFromCleanupStack = ETrue;
			    }
			else
				{
				TRACE( T_LIT( "CBSBrandHandler::ReadElementLC VALUE IS NOT FOUND!!!") );
				CleanupStack :: Pop (returnValue);
				}
			}
		if( !returnValue )
			{
			TRACE( T_LIT( "CBSBrandHandler::ReadElementLC VALUE not FOUND LEAVING WITH -1 !!!") );
			User::Leave( KErrNotFound );			
			}
		}
	
	CleanupClosePushL( *returnValue );
    // since we make one function call to ReadElementLC in case the default 
	// brand id is used to retrieved the element, we have to pop one returnValue
	// pointer from CleanupStack (otherwise we have two identical pointers on 
	// the stack!!!)
	if ( popElementFromCleanupStack )
   		{
   	 	CleanupStack::Pop( returnValue );
    	}
	
	TRACE( T_LIT( "CBSBrandHandler::ReadElementLC end ") );
	return returnValue;
	}

// -----------------------------------------------------------------------------
// CBSBrandHandler::VerifyVersionL()
// -----------------------------------------------------------------------------
//
void CBSBrandHandler::VerifyVersionL( RFileReadStream& aStream )
	{
	TInt version = aStream.ReadInt16L();
	if( version != iReserved )
		{
		User::Leave( KErrArgument );
		}
	}


// -----------------------------------------------------------------------------
// CBSBrandHandler::VerifyVersionL()
// -----------------------------------------------------------------------------
//
void CBSBrandHandler::VerifyVersionL()
	{
	if( !iHandle )
		{
		User::Leave( KErrNotReady );
		}
	RFileReadStream stream;
	stream.Attach( *iHandle );
	CleanupClosePushL( stream );
	
	VerifyVersionL( stream );
	
	CleanupStack::PopAndDestroy(); // stream
	}

// -----------------------------------------------------------------------------
// CBSBrandHandler::ReadStreamL()
// -----------------------------------------------------------------------------
//
MBSElement* CBSBrandHandler::ReadStreamL( const TDesC8& aId, RFileReadStream& aStream,
										  TBool aAllowEmptyId /* = EFalse */ )
	{
	TRACE( T_LIT( "CBSBrandHandler::ReadStreamL BEGIN"));
	TBSElementType type = (TBSElementType)aStream.ReadInt16L();
	MBSElement* returnValue = NULL;
	
	TInt idSize = aStream.ReadInt16L();
	
	HBufC8* elementId = HBufC8::NewLC( idSize );
	TPtr8 elementIdPtr = elementId->Des();

	if( idSize == 0 && aAllowEmptyId )
		{
		// we don't read empty ID
		}
	else
		{
		aStream.ReadL( elementIdPtr, idSize );
        elementIdPtr.SetLength( idSize );// Set length
		}
	

	TBool match = EFalse;
	if( aAllowEmptyId || ( 0 == elementIdPtr.Compare( aId ) ) )
		{
		match = ETrue;
		}
		
    TPtrC8 idPtrC( *elementId );// idPtrC creation moved here so it will be updated correctly.
	if( elementId->Length() == 0 )
		{
		CleanupStack::PopAndDestroy( elementId );
		elementId = NULL;
		idPtrC.Set( KNullDesC8 );
		}

	switch( type )
		{
		case EBSInt:
			{
			TInt intData = aStream.ReadInt16L();
			TRACE( T_LIT( "CBSBrandHandler::ReadStreamL type INT"));
			if( match )
				{
                // Codescanner warning: neglected to put variable on cleanup stack (id:35)
                // This method cannot leave after this line
				returnValue = BSElementFactory::CreateBSElementL( idPtrC, // CSI: 35 # See above
																  EBSInt,
																  intData ); 
				}
			break;
			}		
		case EBSText:
		case EBSFile: // flow through
			{
			TInt textSize = aStream.ReadInt16L();
			HBufC* textData = HBufC::NewLC( textSize );

			TPtr textPtr = textData->Des();
			aStream.ReadL( textPtr, textSize );

			TRACE( T_LIT( "CBSBrandHandler::ReadStreamL type TEXT/ FILE"));
			if( match )
				{
                // Codescanner warning: neglected to put variable on cleanup stack (id:35)
                // This method cannot leave after this line
				returnValue = BSElementFactory::CreateBSElementL( idPtrC, // CSI: 35 # See above
																  type,
																  *textData ); 
				}

			CleanupStack::PopAndDestroy( textData );
			break;
			}
		case EBSList:
			{
			RBSObjOwningPtrArray<MBSElement> listData;
			CleanupClosePushL( listData );
			TInt count = aStream.ReadInt16L();
			
			for( TInt i = 0; i < count; i++ )
				{
				MBSElement* subElement = ReadStreamL( KNullDesC8, aStream, ETrue );
				CleanupDeletePushL( subElement );
				listData.AppendL( subElement );
				CleanupStack::Pop(); // subElement
				}
				
			if( match )
				{
                // Codescanner warning: neglected to put variable on cleanup stack (id:35)
                // This method cannot leave after this line
				returnValue = BSElementFactory::CreateBSElementL( idPtrC, // CSI: 35 # See above
																  EBSList,
																  listData ); 
				}
			CleanupStack::Pop(); // listData
			break;
			}
		case EBSBuffer:
			{
			TInt bufferSize = aStream.ReadInt16L();
			HBufC8* buffeData = HBufC8::NewLC( bufferSize );

			TPtr8 bufferPtr = buffeData->Des();
			aStream.ReadL( bufferPtr, bufferSize );

			if( match )
				{
                // Codescanner warning: neglected to put variable on cleanup stack (id:35)
                // This method cannot leave after this line				
				returnValue = BSElementFactory::CreateBSElementL( idPtrC, // CSI: 35 # See above
																  EBSBuffer,
																  *buffeData ); 
				}

			CleanupStack::PopAndDestroy( buffeData );
			break;
			}
		case EBSBitmap:
			{
			TInt length = aStream.ReadInt16L();
			HBufC8* fileId = HBufC8::NewLC( length );
			
			TPtr8 fileIdPtr = fileId->Des();
			aStream.ReadL( fileIdPtr, length );
			
			TInt bitmapId = aStream.ReadInt16L();
			TInt maskId = aStream.ReadInt16L();
			TInt skinId = aStream.ReadInt16L();
			TInt skinMaskId = aStream.ReadInt16L();

			TRACE( T_LIT( "CBSBrandHandler::ReadStreamL type BITMAP .. bitmap ID is [%d]"), bitmapId);
			if( match )
				{
				CBSBitmap* bitmap = CBSBitmap::NewLC( bitmapId,
													  maskId,
													  skinId,
													  skinMaskId,
													  fileIdPtr );

                // Codescanner warning: neglected to put variable on cleanup stack (id:35)
                // This method cannot leave after this line
				returnValue = BSElementFactory::CreateBSElementL( idPtrC, // CSI: 35 # See above
																  EBSBitmap,
																  bitmap ); 
				CleanupStack::Pop( bitmap ); 
				}
		    CleanupStack::PopAndDestroy( fileId ); 

			break;
			}
		default:
			{
			TRACE( T_LIT( "CBSBrandHandler::ReadStreamL type DEFAULT : corrupt"));
			User::Leave( KErrCorrupt );
			break;
			}
		}

	if( elementId )
		{
		CleanupStack::PopAndDestroy( elementId );
		}

	TRACE( T_LIT( "CBSBrandHandler::ReadStreamL END"));
	return returnValue;
	}

//  END OF FILE

