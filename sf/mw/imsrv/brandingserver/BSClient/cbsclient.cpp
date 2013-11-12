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
* Description:  CBSClient.cpp
*
*/



//  INCLUDE FILES
#include "cbsclient.h"
#include "bsclientdefs.h"
#include "bsprocessstarter.h"
#include "bsserverdefs.h"
#include "mbselement.h"
#include "bselementfactory.h"
#include "cbsbitmap.h"
#include "cbsbrandobserver.h"
#include <e32base.h>
#include <s32mem.h>
#include "DebugTrace.h"

// ==============================================================
// ======================== CLIENT ==============================
// ==============================================================

// Destructor
RBSClient::~RBSClient()
    {
#if _BullseyeCoverage
    cov_write();
#endif
    }

// C++ default constructor can NOT contain any code, that
// might leave.
//
RBSClient::RBSClient()
    {
    }


// -----------------------------------------------------------------------------
// RBSClient::Connect()
// -----------------------------------------------------------------------------
//
TInt RBSClient::Connect()
    {
    TRACE( T_LIT("RBSClient::Connect() begin") );
    TFileName fullExePath;
    BSProcessStarter::FullExePathForClienLocation( KBSServerExe,
                                                 fullExePath );

    return BSProcessStarter::ConnectToServer( fullExePath,
                                            KNullDesC,
                                            NULL,
                                            *this,
                                            KBSServerName,
                                            TVersion( KBSVersionMajor,
                                                      KBSVersionMinor,
                                                      KBSVersionBuild ),
                                            KBSMsgSlotCount );
    }


// -----------------------------------------------------------------------------
// RBSClient::Close()
// -----------------------------------------------------------------------------
//
void RBSClient::Close()
    {
    TRACE( T_LIT("RBSClient::Close() begin") );
    RSessionBase::Close();
    }


// -----------------------------------------------------------------------------
// RBSClient::Close()
// -----------------------------------------------------------------------------
//
void RBSClient::StartTransactionL(  const TDesC8& aApplicationId,
        				  const TDesC8& aBrandId,
        				  const TDesC8& aDefaultBrandId,
        				  TLanguage aLanguage,
        				  TTransactionType aType,
        				  TInt aReserved )
	{
	TRACE( T_LIT("RBSClient::StartTransactionL() begin") );
	InitClientL( aApplicationId, aBrandId, aDefaultBrandId, 
				 aLanguage, aType, aReserved );
	}

// -----------------------------------------------------------------------------
// RBSClient::Close()
// -----------------------------------------------------------------------------
//
TInt RBSClient::StopTransactionL( const TDesC8& /*aApplicationId*/,
        				  const TDesC8& /*aBrandId*/,
        				  TLanguage /*aLanguage*/,
        				  TInt /*aReserved*/ )
	{
	TRACE( T_LIT("RBSClient::StopTransactionL() begin") );
	TInt returnValue = 0;
    TPckg<TInt> pack( returnValue );
    TIpcArgs args( &pack );
    TInt err = SendReceive( EBSStopTransaction, args );
    User::LeaveIfError( err );
	return returnValue;
	}

// -----------------------------------------------------------------------------
// RBSClient::Close()
// -----------------------------------------------------------------------------
//
void RBSClient::CancelTransactionL( const TDesC8& aApplicationId,
        				  const TDesC8& aBrandId,
        				  TLanguage aLanguage,
        				  TInt aReserved )
	{
	TIpcArgs args = CreateArgumentsL( aApplicationId, aBrandId,
					  				  aLanguage, aReserved );
					  				  
	TInt err = SendReceive( EBSCancelTransaction, args );
	User::LeaveIfError( err );
	}

// -----------------------------------------------------------------------------
// RBSClient::InitClientL()
// -----------------------------------------------------------------------------
//
void RBSClient::InitClientL( const TDesC8& aApplicationId,
        				  const TDesC8& aBrandId,
        				  const TDesC8& aDefaultBrandId,
        				  TLanguage aLanguage,
        				  TTransactionType aType,
        				  TInt aReserved )
	{
	TRACE( T_LIT("RBSClient::InitClientL() begin aAppId[%S],aBrandId[%S],TAType[%d]"),&aApplicationId,&aBrandId,aType );
	CBufFlat* flatBuf = CBufFlat::NewL( aDefaultBrandId.Length() + aBrandId.Length() + 2 );
	CleanupStack::PushL( flatBuf );

 	RBufWriteStream stream;
	CleanupClosePushL( stream );
	stream.Open( *flatBuf );
	stream.WriteInt16L( aBrandId.Length() );
	stream.WriteL( aBrandId );
	stream.WriteInt16L( aDefaultBrandId.Length() );
	stream.WriteL( aDefaultBrandId );

    TPtr8 ptrElement( flatBuf->Ptr(0) );

	// set the arguments for the message
	TIpcArgs args(  &aApplicationId );
    args.Set( 1,  &ptrElement );
    args.Set( 2, aLanguage );
    args.Set( 3, aReserved );
    
	TInt operation = 0;
	switch( aType )
		{
		case EBSTxInstall:
			{
			operation = EBSInitInstall;
			break;
			}
		case EBSTxUninstall:
			{
			operation = EBSInitUninstall;
			break;
			}
		case EBSTxAppend:
			{
			operation = EBSInitAppend;
			break;
			}
		case EBSTxReplace:
			{
			operation = EBSInitReplace;
			break;
			}
		case EBSTxAccess:
			{
			operation = EBSInitAccess;
			break;
			}
		default:
			{
			User::Leave( KErrArgument );
			break;
			}
		}		
	TInt err = SendReceive( operation, args );
	TRACE( T_LIT("RBSClient::InitClientL() 1st SendReceive err=%d"),err );
	CleanupStack::PopAndDestroy( 2, flatBuf ); // stream, flatBuf
	
	User::LeaveIfError( err );

	User::LeaveIfError( SendReceive( EBSStartTransaction ) );
	TRACE( T_LIT("RBSClient::InitClientL() end") );
	}
   
// -----------------------------------------------------------------------------
// RBSClient::CreateArgumentsL()
// -----------------------------------------------------------------------------
//
TIpcArgs RBSClient::CreateArgumentsL( const TDesC8& aApplicationId,
        				  const TDesC8& aBrandId,
        				  TLanguage aLanguage,
        				  TInt aReserved )
	{
	// set the arguments for the message
	TIpcArgs args(  &aApplicationId );
    args.Set( 1,  &aBrandId );
    args.Set( 2, aLanguage );
    args.Set( 3, aReserved );
    
    return args;
	}


// -----------------------------------------------------------------------------
// RBSClient::InsertL()
// -----------------------------------------------------------------------------
//
void RBSClient::InsertL( MBSElement* aElement )
	{
	SendUpdateL( aElement, EBSInstall );
	}


// -----------------------------------------------------------------------------
// RBSClient::ReplaceL()
// -----------------------------------------------------------------------------
//
void RBSClient::ReplaceL( MBSElement* aElement )
	{
	SendUpdateL( aElement, EBSReplace );
	}
	
// -----------------------------------------------------------------------------
// RBSClient::AppendL()
// -----------------------------------------------------------------------------
//
void RBSClient::AppendL( MBSElement* aElement )
	{
	SendUpdateL( aElement, EBSAppend );
	}

// -----------------------------------------------------------------------------
// RBSClient::SendUpdateL()
// -----------------------------------------------------------------------------
//
void RBSClient::SendUpdateL( MBSElement* aElement, TBSMessages aMessage )
	{
	// FIXME magic number
	CBufFlat* flatBuf = CBufFlat::NewL( 128 );
	CleanupStack::PushL( flatBuf );

 	RBufWriteStream elementStream;
	CleanupClosePushL( elementStream );
	elementStream.Open( *flatBuf );

	aElement->ExternalizeL( elementStream );

    TPtr8 ptrElement( flatBuf->Ptr(0) );
	TIpcArgs args( &ptrElement );
	
	TInt err = SendReceive( aMessage, args );

	CleanupStack::PopAndDestroy(); // elementStream
	CleanupStack::PopAndDestroy( flatBuf );
	User::LeaveIfError( err );
	}

// -----------------------------------------------------------------------------
// RBSClient::GetTextL()
// -----------------------------------------------------------------------------
//
void RBSClient::GetTextL( const TDesC8& aId, HBufC*& aValue )
    {
	TInt size = 0;
    TPckg<TInt> pack( size );
    
	TIpcArgs args( &aId );
	args.Set( 1, &pack );
    TInt err = SendReceive( EBSPrepareText, args );
    User::LeaveIfError( err );
    
	aValue = HBufC::NewL( size );
	TPtr ptrBuf( aValue->Des() );

    args.Set( 2, &ptrBuf );    
    err = SendReceive( EBSGetText, args );
    
    if( err )
    	{
    	delete aValue;
    	aValue = NULL;
    	User::Leave( err );
    	}
    }

// -----------------------------------------------------------------------------
// RBSClient::GetIntL()
// -----------------------------------------------------------------------------
//
void RBSClient::GetIntL( const TDesC8& aId, TInt& aValue )
    {
    TPckg<TInt> pack( aValue );
    TIpcArgs args( &aId);
    args.Set( 1, &pack );
    TInt err = SendReceive( EBSGetInt, args );
    User::LeaveIfError( err );
    }

void RBSClient :: isBrandUpdateRequiredL (TInt & aUpdateRequired)
{
	//TIpcArgs args;

    TPckg<TInt> pack (aUpdateRequired);
	//args.Set (0, &pack);
	TInt err = SendReceive (EBSIsBrandUpdateRequired, TIpcArgs (&pack));

	User :: LeaveIfError (err);
}
// -----------------------------------------------------------------------------
// RBSClient::GetTextL()
// -----------------------------------------------------------------------------
//
void RBSClient::GetBufferL( const TDesC8& aId, HBufC8*& aValue )
    {
	TInt size = 0;
    TPckg<TInt> pack( size );
    
	TIpcArgs args( &aId );
	args.Set( 1, &pack );
    TInt err = SendReceive( EBSPrepareBuffer, args );
    User::LeaveIfError( err );
    
	aValue = HBufC8::NewL( size );
	TPtr8 ptrBuf( aValue->Des() );

    args.Set( 2, &ptrBuf );    
    err = SendReceive( EBSGetBuffer, args );
    
    if( err )
    	{
    	delete aValue;
    	aValue = NULL;
    	User::Leave( err );
    	}
    }

// -----------------------------------------------------------------------------
// RBSClient::GetSeveralL()
// -----------------------------------------------------------------------------
//
MBSElement* RBSClient::GetSeveralL( MDesC8Array& aIds )
	{
	TInt size = 0;

    TPckg<TInt> pack( size );
    TIpcArgs args;
    args.Set( 0, &pack );


	// FIXME magic number
	CBufFlat* flatBuf = CBufFlat::NewL( 128 );
	CleanupStack::PushL( flatBuf );

 	RBufWriteStream idStream;
	CleanupClosePushL( idStream );
	idStream.Open( *flatBuf );

	ExternalizeIdArrayL( idStream, aIds );
	
	TPtr8 flatPtr = flatBuf->Ptr(0);
	args.Set( 1, &flatPtr );
	// get the size of the data
	TInt err = SendReceive( EBSPrepareSeveral, args );

	CleanupStack::PopAndDestroy(); // idStream
	CleanupStack::PopAndDestroy( flatBuf );
	User::LeaveIfError( err );

    HBufC8* buf = HBufC8::NewLC( size );
    TPtr8 ptrBuf( buf->Des() );

    TIpcArgs msgArgs;
    msgArgs.Set( 0, &ptrBuf );

	err = SendReceive( EBSGetSeveral, msgArgs );

	TInt length = buf->Size();

	RDesReadStream readStream;
	CleanupClosePushL( readStream );
	readStream.Open( *buf );

	MBSElement* returnValue = InternalizeElementL( readStream );
	
	CleanupStack::PopAndDestroy(); // readStream
	CleanupStack::PopAndDestroy( buf ); // buf

	User::LeaveIfError( err );
	return returnValue;
	}


// -----------------------------------------------------------------------------
// RBSClient::GetFileL()
// -----------------------------------------------------------------------------
//
void RBSClient::GetFileL( const TDesC8& aId, RFile& aFile )
	{
	TInt fsh;

	TPckgBuf<TInt> fh;

	TIpcArgs args( &fh );
	args.Set( 2, &aId );

	fsh = SendReceive( EBSGetFile, args );
	TInt err = aFile.AdoptFromServer( fsh, fh() );

	User::LeaveIfError( err );
	}

// -----------------------------------------------------------------------------
// RBSClient::GetStructureL()
// -----------------------------------------------------------------------------
//
MBSElement* RBSClient::GetStructureL( const TDesC8& aId )
	{
	TInt size = 0;

    TPckg<TInt> pack( size );
    TIpcArgs args;
    args.Set( 0, &pack );

	args.Set( 1, &aId );
	// get the size of the data
	TInt err = SendReceive( EBSPrepareStructure, args );

	User::LeaveIfError( err );


    HBufC8* buf = HBufC8::NewLC( size );
    TPtr8 ptrBuf( buf->Des() );

    TIpcArgs msgArgs;
    msgArgs.Set( 0, &ptrBuf );

	err = SendReceive( EBSGetStructure, msgArgs );

	TInt length = buf->Size();

	RDesReadStream readStream;
	CleanupClosePushL( readStream );
	readStream.Open( *buf );

	MBSElement* returnValue = InternalizeElementL( readStream );
	CleanupStack::PopAndDestroy(); // readStream
	CleanupStack::PopAndDestroy( buf ); // buf

	User::LeaveIfError( err );

	return returnValue;
	}

// -----------------------------------------------------------------------------
// RBSClient::InternalizeElementL()
// -----------------------------------------------------------------------------
//
MBSElement* RBSClient::InternalizeElementL( RReadStream& aStream )
    {
    MBSElement* returnValue= NULL;
    // Write common header for all elements
    TBSElementType type = (TBSElementType)aStream.ReadInt16L();
    
    TInt idSize = aStream.ReadInt16L();

	HBufC8* elementId = HBufC8::NewLC( idSize );
	TPtr8 elementIdPtr = elementId->Des();

    if( idSize > 0 )
        {
        // read ID only if it's defined
        aStream.ReadL( elementIdPtr, idSize );
        elementIdPtr.SetLength( idSize );// Set length
        }
        
// Ptr() returns a pointer to the start address of decriptor data,
// TPtrC constructor then parses the data until null terminator met.
// This results in wrong descriptor length & bad data!
// Solution is either 
// 1) to use PtrZ() which appends a zero terminator,
// or 2) pass to constructor an object that has length data.
// Option 2) is less prone to errors, so use it.
// In general, assignment operator with descriptors should be avoided!
// So use TPtrC16(const TDesC16 &aDes) instead 
// of TPtrC16(const TUint16 *aString)!
    TPtrC8 idPtrC( *elementId );//idPtrC creation moved here so it will be updated correctly.

	if( elementId->Length() == 0 )
		{
		CleanupStack::PopAndDestroy( elementId );
		elementId = NULL;
		idPtrC.Set( KNullDesC8 );
		}
		
    // Write element type specific data
    switch( type )
        {
        case EBSInt:
            {
            TInt intData = aStream.ReadInt16L();
			returnValue = BSElementFactory::CreateBSElementL( idPtrC,
															  EBSInt,
															  intData );					

            break;
            }
        case EBSText:     // flowthrough
        case EBSFile:
            {
			TInt textSize = aStream.ReadInt16L();
			HBufC* textData = HBufC::NewLC( textSize );

			TPtr textPtr = textData->Des();
			aStream.ReadL( textPtr, textSize );
			
			returnValue = BSElementFactory::CreateBSElementL( idPtrC, 
															  type,
															  *textData );
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
				MBSElement* subElement = InternalizeElementL( aStream );
				CleanupClosePushL( *subElement );
				listData.AppendL( subElement );
				CleanupStack::Pop(); // subElement
				}

			returnValue = BSElementFactory::CreateBSElementL( idPtrC, 
															  EBSList,
															  listData );

			CleanupStack::Pop(); // listData

            break;
            }

        case EBSBuffer:
        	{
			TInt bufferSize = aStream.ReadInt16L();
			HBufC8* buffeData = HBufC8::NewLC( bufferSize );

			TPtr8 bufferPtr = buffeData->Des();
			aStream.ReadL( bufferPtr, bufferSize );

			returnValue = BSElementFactory::CreateBSElementL( idPtrC, 
															  EBSBuffer,
															  *buffeData );

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

			CBSBitmap* bitmap = CBSBitmap::NewLC( bitmapId,
												  maskId,
												  skinId,
												  skinMaskId,
												  fileIdPtr );
												  
			returnValue = BSElementFactory::CreateBSElementL( idPtrC, 
															  EBSBitmap,
															  bitmap );
			CleanupStack::Pop( bitmap ); // bitmap
			CleanupStack::PopAndDestroy( fileId ); // bitmap
			
            break;
            }

        default:
            {
            // unknown type!
            User::Leave( KErrCorrupt );
            break;
            }
        }
        
	if( elementId )
		{
		CleanupStack::PopAndDestroy( elementId );
		}
        
	return returnValue;
    }
    
    
// -----------------------------------------------------------------------------
// RBSClient::InternalizeElementL()
// -----------------------------------------------------------------------------
//
void RBSClient::ExternalizeIdArrayL( RWriteStream& aStream, MDesC8Array& aArray )
	{
	TInt count = aArray.MdcaCount();
	aStream.WriteInt16L( count );
	
	for( TInt i = 0; i < count; i++ )
		{
		TPtrC8 ptr = aArray.MdcaPoint(i);
		aStream.WriteInt16L( aArray.MdcaPoint( i ).Length() );
		aStream.WriteL( aArray.MdcaPoint( i ) );
		}
	
	}

// -----------------------------------------------------------------------------
// RBSClient::RemoveBrandL()
// -----------------------------------------------------------------------------
//
void RBSClient::RemoveBrandL( const TDesC8& aApplicationId,
								   const TDesC8& aBrandId )
	{
	// set the arguments for the message
	TIpcArgs args(  &aApplicationId );
	args.Set( 1, &aBrandId );
	TInt err = SendReceive( EBSRemoveBrand, args );
	User::LeaveIfError( err );
	}

// -----------------------------------------------------------------------------
// RBSClient::RemoveBrandsL()
// -----------------------------------------------------------------------------
//
void RBSClient::RemoveBrandsL( const TDesC8& aApplicationId )
	{
	// set the arguments for the message
	TIpcArgs args(  &aApplicationId );
		
	TInt err = SendReceive( EBSRemoveApplication, args );
	User::LeaveIfError( err );
	}

// -----------------------------------------------------------------------------
// RBSClient::RegisterObserverL()
// -----------------------------------------------------------------------------
//
void RBSClient::RegisterObserverL( MBSBrandChangeObserver* aObserver, MBSBackupRestoreStateObserver* aBackupStateObserver )
	{
	CBSBrandObserver* tempObserver = CBSBrandObserver::NewL( aObserver, aBackupStateObserver, this );
	if( iObserver )
		{
		delete iObserver;
		}
	iObserver = tempObserver;
	}

// -----------------------------------------------------------------------------
// RBSClient::UnRegisterObserverL()
// -----------------------------------------------------------------------------
//
void RBSClient::UnRegisterObserverL( MBSBrandChangeObserver* /*aObserver*/, MBSBackupRestoreStateObserver* /*aBackupStateObserver*/ )
	{
	delete iObserver;
	iObserver = NULL;
	}

// -----------------------------------------------------------------------------
// RBSClient::UnRegisterObserverL()
// -----------------------------------------------------------------------------
//
void RBSClient::RegisterObserverToServerL( TRequestStatus& aStatus )
	{
	TIpcArgs args;
	SendReceive( EBSObserveBrand, args, aStatus );
	}

// -----------------------------------------------------------------------------
// RBSClient::GetNewVersionL()
// -----------------------------------------------------------------------------
//
TInt RBSClient::GetNewVersionL()
	{
    return GetValueL( EBSObserveGetNewVersion);
	}

// -----------------------------------------------------------------------------
// RBSClient::GetValueL()
// -----------------------------------------------------------------------------
//
TInt RBSClient::GetValueL(TInt msg)
	{
	TInt returnValue = 0;
    TPckg<TInt> pack( returnValue );
    TIpcArgs args( &pack );
    TInt err = SendReceive( msg, args );
    User::LeaveIfError( err );	
    return returnValue;
	}

// -----------------------------------------------------------------------------
// RBSClient::GetBackupStateL()
// -----------------------------------------------------------------------------
//
TInt RBSClient::GetBackupStateL()	
	{
    return GetValueL( EBSObserveGetBackupState);
	}

// -----------------------------------------------------------------------------
// RBSClient::GetBackupRestoreL()
// -----------------------------------------------------------------------------
//
TInt RBSClient::GetBackupRestoreL()	
	{
    return GetValueL( EBSObserveGetChange);
	}

//  END OF FILE

