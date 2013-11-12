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
* Description:  CBSSession.cpp
*
*/



//  INCLUDE FILES
#include <e32base.h>
#include <apgcli.h>

#include "cbssession.h"
#include "cbsserver.h"
#include "DebugTrace.h"
#include "cbsbrandhandler.h"
#include "cbsstoragemanager.h"
#include "cbsstorage.h"
#include "cbsbitmap.h"
//#include "cbselement.h"
#include "rbsobjowningptrarray.h"
#include "bselementfactory.h"
#include "bsimportconstants.h"
#include <badesca.h>
#include <s32buf.h>
#include <s32mem.h>
#include <utf.h>
#include <e32property.h>

// ==============================================================
// ======================== SESSION =============================
// ==============================================================

// Two-phased constructor.
CBSSession* CBSSession::NewL()
    {
    TRACE( T_LIT("CBSSession::NewL begin") );
    CBSSession* self = new( ELeave ) CBSSession();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    TRACE( T_LIT("CBSSession::NewL end") );
    return self;
    }

// Symbian OS default constructor can leave.
void CBSSession::ConstructL()
	{
	}


// destructor
CBSSession::~CBSSession()
    {
    TRACE( T_LIT("CBSSession[%d]::~CBSSession()"), this );

    if( Server() )
        {
        Server()->SessionDied( this );
        }
	delete iText;
	delete iBuffer;
	delete iBranding;
	delete iSeveralData;
	
#ifdef __WINSCW__
	if( iStorageManager )
		{
		iStorageManager->WriteIbyFiles();
		}
#endif
	delete iStorageManager;
    delete iSessionInfo;

    }


// C++ default constructor can NOT contain any code, that
// might leave.
//
CBSSession::CBSSession()
    {
    }

// ---------------------------------------------------------
// CBSSession::CreateL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::CreateL()
    {
    TRACE( T_LIT("CBSSession[%d]::CreateL()"), this );
    Server()->SessionCreatedL( this );
    }

// ---------------------------------------------------------
// CBSSession::ServiceL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::ServiceL( const RMessage2 &aMessage )
    {

    /*if( !iSessionInfo )
        {
        TRAPD( err, ExtractInfoL( aMessage ) );
        TRACE( T_LIT("CBSSession::ServiceL() ExtractInfoL returned with [%d]"), err );
        }
	*/
    if( DispatchMessageL( aMessage ) )
        {
        if( !iMessageCompleted )
        	{
        	aMessage.Complete( KErrNone );
        	}
        }
    }

// ---------------------------------------------------------
// CBSSession::ServiceError
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::ServiceError( const RMessage2& aMessage,
                                      TInt aError )
    {
    aMessage.Complete( aError );
    }

// ---------------------------------------------------------
// CBSSession::HandleBackupStateL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::HandleBackupStateL( TBackupState aState )
    {
	iIsBackup = ETrue ;	
    switch( aState )
        {
        case EBackupNotActive: // backup is complete
            {
            // release session lock
            iBackupActive = EFalse;
            if( iStorageManager )
                {
                iStorageManager->ReleaseLockL();
                }
                
			// Sending backup active state change event after unlocking it
			if( iObserverActive )
				{
				iObserverMessage.Complete( KErrNone );
				iObserverActive = EFalse;
				}

            break;
            }
        case EBackupActive: // backup activated
            {

            // Lock session. Branding data is not available until
            // backup is completed.
            iBackupActive = ETrue;

			// Sending backup active state change event before locking it
			if( iObserverActive )
				{
				iObserverMessage.Complete( KErrNone );
				iObserverActive = EFalse;
				}

            if( iStorageManager )
                {
                iStorageManager->LockStorage();
                }
            break;
            }
        default:
            {
            // unknown state
            }
        }

        
    }

// ---------------------------------------------------------
// CBSSession::DispatchMessageL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CBSSession::DispatchMessageL( const RMessage2& aMessage )
    {
    TRACE( T_LIT("CBrandingSession[%d]::DispatchMessageL() %d"),
                  this, aMessage.Function() );
	iMessageCompleted = EFalse;
    TBool msgNeedsToBeCompleted = ETrue;
    
    if( iBackupActive )
        {
        // if backup is active, we don't take requests.
        // We could take some requests which do not require disk
        // operations. This is not a good solution and a better one should be
        // investigated. I'm running out of time.
        TInt msg = aMessage.Function() ; 
        if( msg == EBSObserveGetChange ||
        	msg == EBSObserveGetBackupState ||
        	msg == EBSObserveBrand)
        	{
        	//allow to do the operation	
        	}
        else
    	    {
	       	User::Leave( KErrNotReady );	
        	}
        
        }
    
    switch( aMessage.Function() )
        {
        case EBSInitInstall:
        	{
        	InitUpdateL( aMessage, EBSTxInstall );
        	break;
        	}
        case EBSInitUninstall:
        	{
        	InitUpdateL( aMessage, EBSTxUninstall );
        	break;
        	}
        case EBSInitAppend:
        	{
        	InitUpdateL( aMessage, EBSTxAppend );
        	break;
        	}
        case EBSInitReplace:
        	{
        	InitUpdateL( aMessage, EBSTxReplace );
        	break;
        	}
        case EBSInitAccess:
        	{
        	InitAccessL( aMessage );
        	break;
        	}
        case EBSPrepareText:
            {
			GetTextL( aMessage );
            break;
            }
        case EBSGetText:
            {
            if( iText )
            	{
            	aMessage.WriteL( 2, *iText );	
            	}
			else
				{
				aMessage.WriteL( 2, KNullDesC() );
				}
            break;
            }

		case EBSIsBrandUpdateRequired:
			isBrandUpdateRequiredL (aMessage);
			break;

        case EBSGetInt:
            {
			GetIntL( aMessage );
            break;
            }

        case EBSPrepareBuffer:
            {
			GetBufferL( aMessage );
            break;
            }
            
        case EBSGetBuffer:
            {
            if( iBuffer )
            	{
            	aMessage.WriteL( 2, *iBuffer );	
            	}
			else
				{
				aMessage.WriteL( 2, KNullDesC8() );
				}
            break;
            }
            
        case EBSGetFile:
            {
 			GetFileL( aMessage );
            break;
            }

        case EBSPrepareSeveral:
        	{
			PrepareSeveralL( aMessage );
        	break;
        	}

        case EBSGetSeveral:
        	{
			GetSeveralL( aMessage );
        	break;
        	}

        case EBSPrepareStructure:
        	{
			PrepareStructureL( aMessage );
        	break;
        	}

        case EBSGetStructure:
    		{
			GetStructureL( aMessage );
    		break;
    		}

        case EBSStartTransaction:
        	{
			StartTransactionL( aMessage );
        	break;
        	}
        	
        case EBSStopTransaction:
        	{
        	StopTransactionL( aMessage );
        	break;
        	}
        case EBSCancelTransaction:
        	{
        	CancelTransactionL( aMessage );
        	break;
        	}

        case EBSInstall:
        	{
        	InstallL( aMessage );
        	break;
        	}

        case EBSReplace:
        	{
        	ReplaceL( aMessage );
        	break;
        	}

        case EBSAppend:
        	{
        	AppendL( aMessage );
        	break;
        	}

        case EBSRemoveBrand:
        	{
        	RemoveBrandL( aMessage );
        	break;
        	}
        case EBSRemoveApplication:
        	{
        	RemoveApplicationL( aMessage );
        	break;
        	}
        	
        case EBSObserveBrand:
        	{
        	if( iObserverActive )
        		{
        		// complete the old observer request with KErrCancel
        		iObserverMessage.Complete( KErrCancel );
        		}
        	iObserverMessage = aMessage;
        	iObserverActive = ETrue;
        	msgNeedsToBeCompleted = EFalse;
        	break;
        	}

        case EBSObserveGetNewVersion:
        	{
        	TInt newVersion = GetNewVersionL();
			TPckgC<TInt> pack( newVersion );
			aMessage.WriteL( 0, pack );
        	break;
        	}
        	
		// Get what change happen        	
        case EBSObserveGetChange:
        	{
			TPckgC<TInt> pack( iIsBackup );
			aMessage.WriteL( 0, pack );
        	break ;
        	}
        	
        // Get the backup state
        case EBSObserveGetBackupState:
        	{
			TPckgC<TInt> pack( iBackupActive );
			aMessage.WriteL( 0, pack );
        	break ;	
        	}
        	
        default:
            {
            User::Leave( KErrNotSupported );
            break;
            }
        }

    return msgNeedsToBeCompleted;
    }


// ---------------------------------------------------------
// CBSSession::InitUpdateL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::InitUpdateL( const RMessage2 &aMessage, TTransactionType aType)
	{
    TRACE( T_LIT("CBSSession::InitUpdateL: begin TTransactionType[%d] "),aType );
	
	// get the application id
	TInt bufferSize( aMessage.GetDesLength( 0 ) );

    HBufC8* appId = HBufC8::NewLC( bufferSize );
	TPtr8 appPtr = appId->Des();
	aMessage.ReadL( 0, appPtr );

	HBufC* tmpAppId = CnvUtfConverter::ConvertToUnicodeFromUtf8L( *appId );
	CleanupStack::PopAndDestroy( appId );
	
	delete iApplicationId;
	iApplicationId = tmpAppId;

	// get the brand id
	bufferSize = aMessage.GetDesLength( 1 );
    HBufC8* streamBuf = HBufC8::NewLC( bufferSize );
	TPtr8 streamPtr = streamBuf->Des();
	aMessage.ReadL( 1, streamPtr );

	RDesReadStream stream;
	CleanupClosePushL( stream );
	stream.Open( streamPtr );
	
	TInt descriptorLength = stream.ReadInt16L();
	HBufC8* brandId = HBufC8::NewLC( descriptorLength );
	TPtr8 brandPtr = brandId->Des();
	stream.ReadL( brandPtr, descriptorLength );

	HBufC* tmpBrandId = CnvUtfConverter::ConvertToUnicodeFromUtf8L( *brandId );
	CleanupStack::PopAndDestroy( brandId );
	delete iBrandId;
	iBrandId = tmpBrandId;
	
	descriptorLength = stream.ReadInt16L();
	HBufC8* defaultBrandId = HBufC8::NewLC( descriptorLength );
	TPtr8 defaultPtr = defaultBrandId->Des();
	stream.ReadL( defaultPtr, descriptorLength );
	
	HBufC* tmpDefaultBrandId = CnvUtfConverter::ConvertToUnicodeFromUtf8L( *defaultBrandId );
	CleanupStack::PopAndDestroy( defaultBrandId );
	delete iDefaultBrandId;
	iDefaultBrandId = tmpDefaultBrandId;
	
	CleanupStack::PopAndDestroy( 2, streamBuf ); // stream, streamBuf


	// get the language id
	iLanguageId = (TLanguage)aMessage.Int2();

	// get the version
	iReserved = aMessage.Int3();

	// Check if brand is discarded and client is trying to access/append/replace on that
	// give error message saying brand not found so that client will not use that brand
	if(aType == EBSTxAccess || aType == EBSTxAppend || aType == EBSTxReplace )
	{
		if(!iStorageManager)
		{
			iStorageManager = CBSStorageManager::NewL( this, *iApplicationId );	
		}
		
		TBool brandDiscarded = EFalse ;
		brandDiscarded = iStorageManager->CheckBrandDiscarded(*iApplicationId, *iBrandId) ;
		
		if(brandDiscarded)
		{
			TRACE( T_LIT("CBSSession::InitUpdateL: Brand discarded!->LeaveWith KErrNotFound") );
			///Server()->DisplaySessionInfoL( this, KErrNotFound );
			User::Leave( KErrNotFound );	
		}
	}

	if( aType == EBSTxAppend )
		{
		PrepareAppendL();
		}
	else if( aType == EBSTxReplace )
		{
		PrepareReplaceL();
		}
	else if(aType == EBSTxUninstall)
		{
		if( !iStorageManager )
			{
			iStorageManager = CBSStorageManager::NewL( this, *iApplicationId );
			}
		// if any client is accessing same brand leave with KErrInUse.
		// check if the brand has any active clients
		TBool brandActive = EFalse;
		CBSServer* server = Server();
		if( server )
			{
			brandActive = server->MatchSessionUninstallL( *iApplicationId, *iBrandId,
												 			this);					
			}
			
		if(brandActive)
			{
			TRACE( T_LIT("CBSSession::InitUpdateL: UnInstallation aborted -> LeaveWith KErrInUse") );
			//Server()->DisplaySessionInfoL( this,KErrInUse);
			//when changing this errorcode: please read the note in 
			//UninstallL() ( installer.cpp )
			User::Leave( KErrInUse );
			}
		}
	else if( aType == EBSTxInstall )
		{
		if( !iStorageManager )
			{
			iStorageManager = CBSStorageManager::NewL( this, *iApplicationId );
			}
			
		HBufC* drive = HBufC::NewLC(3);
		TRAPD( err, iStorageManager->GetNewestVersionL( *iApplicationId,
												   	      *iBrandId,
										   		    	  iLanguageId ) );
		if( ( err == KErrNotFound ) || ( err == KErrPathNotFound ) )
			{
			CleanupStack::PopAndDestroy(drive);
			// this brand is not yet there, which is the correct situation here
			// for starting install. We can just ignore this error
			}
			
		else if( !err )
			{
			// there was no error, therefore this brand already exists in the ROM
			// (Z drive).
			// we leave with KErrAlreadyExists
			if( 0 == (drive->Des().Compare(KBSZDrive)) )
				{
				CleanupStack::PopAndDestroy(drive);		
			    User::Leave( KErrAlreadyExists );
				}
			else
				{
				//PopAndDestroy drive as it is not needed any longer.
				CleanupStack::PopAndDestroy(drive);

				// if any client is accessing same brand leave with KErrInUse.
				// check if the brand has any active clients
				TBool brandActive = EFalse;
				CBSServer* server = Server();
				if( server )
					{
					brandActive = server->MatchSessionL( *iApplicationId, *iBrandId,
													 iLanguageId, this, iReserved );					
					}

				if(brandActive == EFalse)
					{	
					TRACE( T_LIT("CBSSession::InitUpdateL: Brand not used -> remove") );
					iStorageManager->RemoveBrandL( *iApplicationId, *iBrandId,
													iLanguageId, iReserved );
					}
				else
					{
					TRACE( T_LIT("CBSSession::InitUpdateL: Installation aborted->LeaveWith KErrInUse") );
					//Server()->DisplaySessionInfoL( this,KErrInUse);
					//when changing this errorcode: please read the note in 
					//UninstallL() ( installer.cpp )
					User::Leave( KErrInUse );
					}
				}
			}
		else
			{
			CleanupStack::PopAndDestroy(drive);	
			// some other error
			TRACE( T_LIT("CBSSession::InitUpdateL: ERROR aType[%d] LeaveWith[%d]"),aType,err );
			User::Leave( err );
			}
		}
		
	
	iInitialized = ETrue;

    TRACE( T_LIT("Server initialized") );
	}



// ---------------------------------------------------------
// CBSSession::PrepareAppendL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::PrepareAppendL()
	{
	if( !iStorageManager )
		{
		iStorageManager = CBSStorageManager::NewL( this, *iApplicationId );
		}
		
	RFile brandHandle;
	iStorageManager->BrandHandleL( *iApplicationId,
								   *iBrandId, iLanguageId, brandHandle, iReserved );
	CleanupClosePushL( brandHandle );
	RFileReadStream oldStream;
	oldStream.Attach( brandHandle );
	CleanupClosePushL( oldStream );
	TInt version = oldStream.ReadInt16L();

	TInt count = oldStream.ReadInt16L();
	
	if( !iStorageManager->Storage() )
		{
		iStorageManager->CreateStorageL();
		}
	for( TInt i = 0; i < count; i++ )
		{
		MBSElement* element = InternalizeElementL( oldStream, ETrue );
		CleanupDeletePushL( element );
		// transfers ownership
		iStorageManager->Storage()->AppendElementsL( element );
		CleanupStack::Pop(); // element
		}

	iStorageManager->Storage()->SetVersion( iReserved );
	iStorageManager->Storage()->SetStorageIdL( *iBrandId );
	iStorageManager->Storage()->SetApplicationIdL( *iApplicationId );
	iStorageManager->Storage()->SetLanguageL( iLanguageId );

	CleanupStack::PopAndDestroy( 2 ); // oldStream, brandHandle
	iAppending = ETrue;
	}
// ---------------------------------------------------------
// CBSSession::PrepareReplaceL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::PrepareReplaceL()
	{
	PrepareAppendL();
	}
	
// ---------------------------------------------------------
// CBSSession::InitAccessL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::InitAccessL( const RMessage2 &aMessage )
	{
	InitUpdateL( aMessage, EBSTxAccess );
	
	iBranding = CBSBrandHandler::NewL( *iApplicationId, *iBrandId, *iDefaultBrandId,
										iLanguageId, this, iReserved );


	iAccessInit = ETrue;
    TRACE( T_LIT("Access initialized") );
	}


// ---------------------------------------------------------
// CBSSession::GetTextL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::GetTextL( const RMessage2 &aMessage )
	{
	if( !iAccessInit )
		{
		User::Leave( KErrNotReady );
		}
	TInt idSize( aMessage.GetDesLength( 0 ) );

    HBufC8* idBuffer = HBufC8::NewLC( idSize );
	TPtr8 ptrId = idBuffer->Des();
	aMessage.ReadL( 0, ptrId );

    HBufC* temp = iBranding->GetTextL( ptrId );
    delete iText;
    iText = temp;

    CleanupStack::PopAndDestroy( idBuffer );
    TPckgC<TInt> pack( iText->Size() );
    aMessage.WriteL( 1, pack );
    TRACE( T_LIT("aMessage.WriteL( 1, %d );"), iText->Size() );
	}

void CBSSession :: isBrandUpdateRequiredL (const RMessage2 &aMessage)
{
	if( !iAccessInit )
		{
		User::Leave( KErrNotReady );
		}
	TInt updateRequired = iBranding->isBrandUpdateRequiredL ();
//	iValue = updateRequired;
	TPckgC <TInt> pack (updateRequired);
	aMessage.WriteL( 0, pack );
}
// ---------------------------------------------------------
// CBSSession::GetIntL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::GetIntL( const RMessage2 &aMessage )
	{
	if( !iInitialized )
		{
		User::Leave( KErrNotReady );
		}
	TInt idSize( aMessage.GetDesLength( 0 ) );

	HBufC8* idBuffer = HBufC8::NewLC( idSize );
	TPtr8 ptrId = idBuffer->Des();
	aMessage.ReadL( 0, ptrId );

	iValue = iBranding->GetIntL( ptrId );
	TPckgC<TInt> pack( iValue );
	aMessage.WriteL( 1, pack );
	CleanupStack::PopAndDestroy( idBuffer );
	}

// ---------------------------------------------------------
// CBSSession::GetBufferL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::GetBufferL( const RMessage2 &aMessage )
	{
	if( !iAccessInit )
		{
		User::Leave( KErrNotReady );
		}
	TInt idSize( aMessage.GetDesLength( 0 ) );

    HBufC8* idBuffer = HBufC8::NewLC( idSize );
	TPtr8 ptrId = idBuffer->Des();
	aMessage.ReadL( 0, ptrId );

    HBufC8* temp = iBranding->GetBufferL( ptrId );
    delete iBuffer;
    iBuffer = temp;

    CleanupStack::PopAndDestroy( idBuffer );
    TPckgC<TInt> pack( iBuffer->Size() );
    aMessage.WriteL( 1, pack );
    TRACE( T_LIT("aMessage.WriteL( 1, %d );"), iBuffer->Size() );
	}


// ---------------------------------------------------------
// CBSSession::GetFileL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::GetFileL( const RMessage2 &aMessage )
	{
    TRACE( T_LIT("CBSSession::GetFileL begin") );
    if( !iInitialized )
		{
		User::Leave( KErrNotReady );
		}
	TInt idSize( aMessage.GetDesLength( 2 ) );
	HBufC8* idBuffer = HBufC8::NewLC( idSize );
	TPtr8 ptrId = idBuffer->Des();
	aMessage.ReadL( 2, ptrId );

	RFile file;
    iBranding->GetFileL( ptrId, file );

	CleanupStack::PopAndDestroy( idBuffer );

	User::LeaveIfError( file.TransferToClient( aMessage, 0 ) );

	iMessageCompleted = ETrue;

	file.Close();
	TRACE( T_LIT("CBSSession::GetFileL end") );
	}


// ---------------------------------------------------------
// CBSSession::InternalizeElementIdsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::InternalizeElementIdsL( RReadStream& aStream, RBSObjOwningPtrArray<HBufC8>& aArray )
	{
	TInt count = aStream.ReadInt16L();
	
	for(TInt i = 0; i < count; i++ )
		{
		TInt length = aStream.ReadInt16L();
		HBufC8* id = HBufC8::NewLC( length );
		TPtr8 ptrId = id->Des();
		aStream.ReadL( ptrId, length );
		aArray.AppendL( id );
		CleanupStack::Pop( id );
		}
	}

// ---------------------------------------------------------
// CBSSession::PrepareTextL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::PrepareSeveralL( const RMessage2 &aMessage )
	{
	if( !iInitialized )
		{
		User::Leave( KErrNotReady );
		}
		
	TInt msgSize( aMessage.GetDesLength( 1 ) );

    HBufC8* idBuffer = HBufC8::NewLC( msgSize );
	TPtr8 ptrId = idBuffer->Des();
	aMessage.ReadL( 1, ptrId );

	RDesReadStream idStream;
	CleanupClosePushL( idStream );
	idStream.Open( ptrId );

	RBSObjOwningPtrArray<HBufC8> idArray;
	CleanupClosePushL( idArray );
	
	InternalizeElementIdsL( idStream, idArray );


	MBSElement* texts = iBranding->GetSeveralL( idArray );

	CleanupStack::PopAndDestroy(); // idArray
	CleanupStack::PopAndDestroy(); // idStream
	CleanupStack::PopAndDestroy();  // idBuffer

	CleanupDeletePushL( texts );

	// FIXME magic number
	CBufFlat* data = CBufFlat::NewL( 2000 );
	CleanupStack::PushL( data );

	RBufWriteStream writeStream;
	CleanupClosePushL( writeStream );
	writeStream.Open( *data );

	texts->ExternalizeL( writeStream );	

	CleanupStack::PopAndDestroy(); // writeStream
	
	delete iSeveralData;
	iSeveralData = data;
	CleanupStack::Pop( data );

	CleanupStack::PopAndDestroy(); // texts

	delete iBuffer;
	iBuffer = NULL;
	iBuffer = iSeveralData->Ptr(0).AllocL();

	delete iSeveralData;
	iSeveralData = NULL;
	TPckgC<TInt> pack( iBuffer->Size() );
	TRACE( T_LIT("DispatchMessageL - EPlatSecPrepareSeveral writing size %d"), iBuffer->Size() );
    aMessage.WriteL( 0, pack );

	}

// ---------------------------------------------------------
// CBSSession::GetSeveralTextL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::GetSeveralL( const RMessage2 &aMessage )
	{
	if( !iInitialized )
		{
		User::Leave( KErrNotReady );
		}
	aMessage.WriteL( 0, *iBuffer );

	delete iBuffer;
	iBuffer = NULL;
	}


// ---------------------------------------------------------
// CBSSession::PrepareStructureL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::PrepareStructureL( const RMessage2 &aMessage )
	{
	if( !iInitialized )
		{
		User::Leave( KErrNotReady );
		}
	TInt idSize( aMessage.GetDesLength( 1 ) );

	RBSObjOwningPtrArray<MBSElement> valueArray;
	CleanupClosePushL( valueArray );

    HBufC8* idBuffer = HBufC8::NewLC( idSize );
	TPtr8 ptrId = idBuffer->Des();
	aMessage.ReadL( 1, ptrId );


	MBSElement* extElement = iBranding->GetStructureL( ptrId );
	CleanupDeletePushL( extElement );
	// FIXME magic number
	CBufFlat* data = CBufFlat::NewL( 2000 );
	CleanupStack::PushL( data );

	RBufWriteStream writeStream;
	CleanupClosePushL( writeStream );
	writeStream.Open( *data );

	extElement->ExternalizeL( writeStream );

	CleanupStack::PopAndDestroy(); // writeStream

	delete iSeveralData;
	iSeveralData = data;
	CleanupStack::Pop( data );

	CleanupStack::PopAndDestroy( extElement );

	delete iBuffer;
	iBuffer = NULL;
	iBuffer = iSeveralData->Ptr(0).AllocL();

	delete iSeveralData;
	iSeveralData = NULL;
	TPckgC<TInt> pack( iBuffer->Size() );
	TRACE( T_LIT("DispatchMessageL - EPlatSecPrepareSeveral writing size %d"), iBuffer->Size() );
    aMessage.WriteL( 0, pack );


	CleanupStack::PopAndDestroy(idBuffer);

	CleanupStack::PopAndDestroy(); // valueArray

	}

// ---------------------------------------------------------
// CBSSession::GetStructureL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::GetStructureL( const RMessage2 &aMessage )
	{
	if( !iInitialized )
		{
		User::Leave( KErrNotReady );
		}
	aMessage.WriteL( 0, *iBuffer );

	delete iBuffer;
	iBuffer = NULL;
	}



// ---------------------------------------------------------
// CBSSession::InstallL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::InstallL( const RMessage2 &aMessage )
	{
	TRACE( T_LIT( "CBSSession::InstallL begin") );
	if( !iInitialized || !iStorageManager )
		{
		User::Leave( KErrNotReady );
		}

	if( !iStorageManager->Storage() )
		{
		iStorageManager->CreateStorageL();
		}
	
	iStorageManager->ConnectTransactionL( *iApplicationId,
										  *iBrandId, iLanguageId, EFalse, iReserved );

	TInt msgSize( aMessage.GetDesLength( 0 ) );

    HBufC8* buffer = HBufC8::NewLC( msgSize );
	TPtr8 ptr = buffer->Des();
	aMessage.ReadL( 0, ptr );

	RDesReadStream stream;
	CleanupClosePushL( stream );
	stream.Open( ptr );

	MBSElement* element = InternalizeElementL( stream );
	CleanupDeletePushL( element );

	iStorageManager->Storage()->SetVersion( iReserved );
	iStorageManager->Storage()->SetStorageIdL( *iBrandId );
	iStorageManager->Storage()->SetApplicationIdL( *iApplicationId );
	iStorageManager->Storage()->SetLanguageL( iLanguageId );
	// transfers ownership
	iStorageManager->Storage()->AppendElementsL( element );

	CleanupStack::Pop(); // element 
	CleanupStack::PopAndDestroy( 2, buffer );
	iWriteNeeded = ETrue;
	TRACE( T_LIT( "CBSSession::InstallL end") );
	}

// ---------------------------------------------------------
// CBSSession::ReplaceL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::ReplaceL( const RMessage2 &aMessage )
	{
	TRACE( T_LIT( "CBSSession::ReplaceL begin") );
	if( !iInitialized || !iStorageManager )
		{
		User::Leave( KErrNotReady );
		}
		
	iStorageManager->ConnectTransactionL( *iApplicationId,
										  *iBrandId, iLanguageId, EFalse, iReserved );

	TInt msgSize( aMessage.GetDesLength( 0 ) );

    HBufC8* buffer = HBufC8::NewLC( msgSize );
	TPtr8 ptr = buffer->Des();
	aMessage.ReadL( 0, ptr );

	RDesReadStream stream;
	CleanupClosePushL( stream );
	stream.Open( ptr );

	MBSElement* element = InternalizeElementL( stream );
	CleanupDeletePushL( element );

	// transfers ownership
	iStorageManager->Storage()->ReplaceElementL( element );

	CleanupStack::Pop(); // element 
	CleanupStack::PopAndDestroy( 2, buffer );
	iWriteNeeded = ETrue;
	TRACE( T_LIT( "CBSSession::ReplaceL end") );
	}


// ---------------------------------------------------------
// CBSSession::AppendL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::AppendL( const RMessage2 &aMessage )
	{
	TRACE( T_LIT( "CBSSession::AppendL begin") );
	if( !iInitialized || !iStorageManager )
		{
		User::Leave( KErrNotReady );
		}
	
	iStorageManager->ConnectTransactionL( *iApplicationId,
										  *iBrandId, iLanguageId, EFalse, iReserved );

	TInt msgSize( aMessage.GetDesLength( 0 ) );

    HBufC8* buffer = HBufC8::NewLC( msgSize );
	TPtr8 ptr = buffer->Des();
	aMessage.ReadL( 0, ptr );

	RDesReadStream stream;
	CleanupClosePushL( stream );
	stream.Open( ptr );

	MBSElement* element = InternalizeElementL( stream );
	CleanupDeletePushL( element );

	iStorageManager->Storage()->SetVersion( iReserved );
	iStorageManager->Storage()->SetStorageIdL( *iBrandId );
	iStorageManager->Storage()->SetApplicationIdL( *iApplicationId );
	iStorageManager->Storage()->SetLanguageL( iLanguageId );
	// transfers ownership
	iStorageManager->Storage()->AppendElementsL( element );

	CleanupStack::Pop(); // element 
	CleanupStack::PopAndDestroy( 2, buffer );
	iWriteNeeded = ETrue;
    TRACE( T_LIT( "CBSSession::AppendL end") );
	}

// ---------------------------------------------------------
// CBSSession::StartTransactionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::StartTransactionL( const RMessage2& /*aMessage*/ )
	{
	if( !iStorageManager )
		{
		iStorageManager = CBSStorageManager::NewL( this, *iApplicationId );
		}
	// if we are appending or replacing, we don't need to 
	// create storage
	if( !iAppending )
		{
		iStorageManager->CreateStorageL();	
		}
	}


// ---------------------------------------------------------
// CBSSession::CancelTransactionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::CancelTransactionL( const RMessage2 &aMessage )
	{
	// get the application id
	TInt bufferSize( aMessage.GetDesLength( 0 ) );

    HBufC8* appId = HBufC8::NewLC( bufferSize );
	TPtr8 appPtr = appId->Des();
	aMessage.ReadL( 0, appPtr );


	// get the brand id
	bufferSize = aMessage.GetDesLength( 1 );
    HBufC8* brandId = HBufC8::NewLC( bufferSize );
	TPtr8 brandPtr = brandId->Des();
	aMessage.ReadL( 1, brandPtr );

	// get the language id
	TLanguage language = (TLanguage)aMessage.Int2();

	// get the version
	TInt version = aMessage.Int3();
	
	
	HBufC* tmpBrandId = CnvUtfConverter::ConvertToUnicodeFromUtf8L( *brandId );
	CleanupStack::PopAndDestroy( brandId );
	CleanupStack::PushL( tmpBrandId );
	HBufC* tmpAppId = CnvUtfConverter::ConvertToUnicodeFromUtf8L( *appId );
	CleanupStack::PushL( tmpAppId );
	
	// check if the transaction is correct	
	iStorageManager->ConnectTransactionL( *tmpAppId,
										  *tmpBrandId, iLanguageId,
										  EFalse, iReserved );

	CleanupStack::PopAndDestroy( 2, tmpBrandId ); // tmpBrandId, tmpAppId
	CleanupStack::PopAndDestroy( appId );
	// cancel the transaction
	iStorageManager->CancelTransactionL();
	iWriteNeeded = EFalse;
	}

// ---------------------------------------------------------
// CBSSession::StopTransactionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::StopTransactionL( const RMessage2 &aMessage )
	{
	if( !iStorageManager )
		{
		// transaction not found
		User::Leave( KErrNotFound );
		}
	if( iAppending )
		{
		// find next available version number
		TInt nextVersion = iStorageManager->NextAvailableVersionL();
		iStorageManager->SetVersion( nextVersion );
		iStorageManager->Storage()->SetVersion( nextVersion );
		}
		
	if( iWriteNeeded )
		{
		iReserved = iStorageManager->WriteStorageFilesL();	
		iWriteNeeded = EFalse;
		}
	
	delete iStorageManager;
	iStorageManager = NULL;
	
	TPckgC<TInt> pack( iReserved );
	aMessage.WriteL( 0, pack );
	
	}


// ---------------------------------------------------------
// CBSSession::InternalizeElementL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
MBSElement* CBSSession::InternalizeElementL( RReadStream& aStream, TBool aAppending /*= EFalse*/ )
	{
	MBSElement* returnValue = NULL;

	TBSElementType type = (TBSElementType)aStream.ReadInt16L();
	TInt idSize = aStream.ReadInt16L();
	HBufC8* elementId = HBufC8::NewLC( idSize );
	TPtr8 elementIdPtr = elementId->Des();
	aStream.ReadL( elementIdPtr, idSize );


	switch( type )
		{
		case EBSInt:
			{
			TInt intData = aStream.ReadInt16L();
			returnValue = BSElementFactory::CreateBSElementL( *elementId,
															   EBSInt,
															   intData );
			break;
			}
		case EBSText:
			{
			TInt textSize = aStream.ReadInt16L();
			HBufC* textData = HBufC::NewLC( textSize );

			TPtr textPtr = textData->Des();
			aStream.ReadL( textPtr, textSize );

			returnValue = BSElementFactory::CreateBSElementL( *elementId,
															   type,
															   *textData );

			CleanupStack::Pop( textData );
			break;
			}

		case EBSFile: 
			{
			TInt nameSize = aStream.ReadInt16L();
			HBufC* fileName = HBufC::NewLC( nameSize );

			TPtr filePtr = fileName->Des();
			aStream.ReadL( filePtr, nameSize );
			
			if( !aAppending )
				{
				// we are installing, so the filename is pointing to a file
				// that has to be installed
				// Install file and get the filename without path
				HBufC* strippedName = iStorageManager->InstallFileLC( *fileName );
				returnValue = BSElementFactory::CreateBSElementL( *elementId,
																   type,
																   *strippedName );
				CleanupStack::Pop( strippedName );
				CleanupStack::PopAndDestroy( fileName );
				}
			else
				{
				// we are appending, so this is really the filename
				returnValue = BSElementFactory::CreateBSElementL( *elementId,
																   type,
																   *fileName );
				CleanupStack::Pop( fileName );
				}
			break;
			}
			
		case EBSList:
			{
			RBSObjOwningPtrArray<MBSElement> listData;
			CleanupClosePushL( listData );
			TInt listCount = aStream.ReadInt16L();
			for( TInt i = 0; i < listCount; i++ )
				{
				MBSElement* listElement = InternalizeElementL( aStream );
				CleanupDeletePushL( listElement );
				listData.AppendL( listElement );
				CleanupStack::Pop(); // listElement
				}

			returnValue = BSElementFactory::CreateBSElementL( *elementId,
															   EBSList,
															   listData );
			CleanupStack::Pop(); //  listData
			break;
			}
		case EBSBuffer:
			{
			TInt bufferSize = aStream.ReadInt16L();
			HBufC8* buffer = HBufC8::NewLC( bufferSize );

			TPtr8 buffPtr = buffer->Des();
			aStream.ReadL( buffPtr, bufferSize );

			returnValue = BSElementFactory::CreateBSElementL( *elementId,
															   EBSBuffer,
															   *buffer );

			CleanupStack::Pop( buffer );
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
													  
			returnValue = BSElementFactory::CreateBSElementL( *elementId, 
															  EBSBitmap,
															  bitmap );
			CleanupStack::Pop( bitmap ); 
			CleanupStack::PopAndDestroy( fileId );
			break;
			}

		default:
			{
			User::Leave( KErrArgument );
			break;
			}
		}

	CleanupStack::PopAndDestroy( elementId );

	return returnValue;
	}


// ---------------------------------------------------------
// CBSSession::MatchSessionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CBSSession::MatchSessionL( const TDesC& aApplicationId,
								 const TDesC& aBrandId,
								 TLanguage aLanguageId,
							 	 TInt aReserved )
	{
	TBool returnValue = EFalse;
	if( iApplicationId && iBrandId )
		{
		if( ( 0 == iApplicationId->Compare( aApplicationId ) ) &&
		   ( 0 == iBrandId->Compare( aBrandId ) ) &&
		   ( iLanguageId == aLanguageId ) && 
		   ( iReserved == aReserved ))
			{
			returnValue = ETrue;
			}
		}
	
	return returnValue;
	}

// ---------------------------------------------------------
// CBSSession::MatchSessionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CBSSession::MatchSessionUninstallL( const TDesC& aApplicationId,
								 const TDesC& aBrandId)
	{
	TBool returnValue = EFalse;
	if( iApplicationId && iBrandId )
		{
		if( ( 0 == iApplicationId->Compare( aApplicationId ) ) &&
		   ( 0 == iBrandId->Compare( aBrandId ) ) )
			{
			returnValue = ETrue;
			}
		}
	
	return returnValue;
	}
// ---------------------------------------------------------
// CBSSession::BrandUpdatedL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::BrandUpdatedL( const TDesC& aApplicationId,
								 const TDesC& aBrandId,
								 TLanguage aLanguageId,
							 	 TInt /*aReserved*/ )
	{
	TRACE( T_LIT( "CBSSession::BrandUpdatedL begin aAppId[%S] aBrandId[%S]"), &aApplicationId, &aBrandId );
	iIsBackup = EFalse ; 
	if( iApplicationId && iBrandId )
		{
		if( ( 0 == iApplicationId->Compare( aApplicationId ) ) &&
		   ( 0 == iBrandId->Compare( aBrandId ) ) &&
		   ( iLanguageId == aLanguageId ) )
			{
			// this event is for us 
			// complete the observer request from client
			if( iObserverActive )
				{
				iObserverMessage.Complete( KErrNone );
				iObserverActive = EFalse;
				}
			}
		}
	TRACE( T_LIT( "CBSSession::BrandUpdatedL end") );
	}


// ---------------------------------------------------------
// CBSSession::RemoveBrandL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::RemoveBrandL( const RMessage2& /*aMessage*/ )
	{
	TRACE( T_LIT( "CBSSession::RemoveBrandL begin") );
	if( !iInitialized || !iStorageManager )
		{
		User::Leave( KErrNotReady );
		}

	iStorageManager->RemoveBrandL( *iApplicationId, *iBrandId,
								   iLanguageId, iReserved );
	
	
	
	TRACE( T_LIT( "CBSSession::RemoveBrandL end") );
	}

// ---------------------------------------------------------
// CBSSession::RemoveApplicationL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CBSSession::RemoveApplicationL( const RMessage2& /*aMessage*/ )
	{
	TRACE( T_LIT( "CBSSession::RemoveApplicationL begin") );
	if( !iInitialized || !iStorageManager )
		{
		User::Leave( KErrNotReady );
		}
	iStorageManager->RemoveApplicationL( *iApplicationId );
	TRACE( T_LIT( "CBSSession::RemoveApplicationL end") );
	}
	
// ---------------------------------------------------------
// CBSSession::GetNewVersionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	
TInt CBSSession::GetNewVersionL()
	{
	if( !iStorageManager )
		{
		iStorageManager = CBSStorageManager::NewL( this, KNullDesC );
		}

	return iStorageManager->GetNewestVersionL( *iApplicationId, *iBrandId, iLanguageId );
	}
	

// CBSSessionInfo
CBSSession::CBSSessionInfo* CBSSession::CBSSessionInfo::NewL( const TDesC& aFileName, 
	    		                                              const TDesC& aCaption,
	    		                                              TThreadId aThreadId, 
	    		                                              TProcessId aProcessId )
	{
	CBSSessionInfo* self = new( ELeave ) CBSSessionInfo( aThreadId, aProcessId );
	CleanupStack::PushL( self );
	self->ConstructL( aFileName, aCaption );
	CleanupStack::Pop( self );
	return self;
	}
void CBSSession::CBSSessionInfo::ConstructL( const TDesC& aFileName, const TDesC& aCaption )
	{	
	iFileName = aFileName.AllocL();
	iCaption = aCaption.AllocL();
	}
CBSSession::CBSSessionInfo::CBSSessionInfo( TThreadId aThreadId, TProcessId aProcessId  )
: iThreadId( aThreadId ),
  iProcessId( aProcessId )
	{
	}
CBSSession::CBSSessionInfo::~CBSSessionInfo()
	{
	delete iFileName;
	delete iCaption;
	}

const TDesC& CBSSession::CBSSessionInfo::FileName()
	{
	return *iFileName;
	}
const TDesC& CBSSession::CBSSessionInfo::Caption()
	{
	return *iCaption;
	}

TThreadId CBSSession::CBSSessionInfo::ThreadId()
	{
	return iThreadId;
	}
TProcessId CBSSession::CBSSessionInfo::ProcessId()
	{
	return iProcessId;
	}


// ---------------------------------------------------------------------------
// CBSSession::ExtractInfoL
// Extracts some information from the specified RMessage2 and saves it.
// ---------------------------------------------------------------------------
//
void CBSSession::ExtractInfoL( const RMessage2& aMessage )
    {
	TRACE( T_LIT( "CBSSession::ExtractInfoL begin") );
	//Collect all necessary data
	RThread thread;
	TInt getProcessErr,getClientThreadErr;
	getClientThreadErr = aMessage.Client( thread );
	CleanupClosePushL( thread );
    
	if ( KErrNone == getClientThreadErr )
	    {
	    TRACE( T_LIT( "CBSSession::ExtractInfoL Client retrieved OK") );
	    TThreadId threadId = thread.Id();
	    RProcess process;
	    getProcessErr = thread.Process( process );
	    CleanupClosePushL( process );
	    if ( getProcessErr == KErrNone )
	        {
	        TRACE( T_LIT( "CBSSession::ExtractInfoL Processfilename retrieved OK") );
	        TFileName fileName = process.FileName();	        
	        TParsePtrC parser( fileName );	        
	        TPtrC processFileNameAndExt( parser.NameAndExt() );
	        
	        RApaLsSession session;
	        TInt connectErr = session.Connect();
	        User :: LeaveIfError (connectErr);
	        session.GetAllApps();	        
	        CleanupClosePushL( session );
	        
	        TApaAppInfo info;	        
	        TPtrC captionPtr( KNullDesC );
	        TPtrC printCaptionPtr( KNullDesC );
	        TPtrC fullName( KNullDesC );
	        while ( KErrNone == session.GetNextApp( info ) )
	            {
	            fullName.Set( info.iFullName );
	            printCaptionPtr.Set( info.iCaption );	            
	            if( KErrNotFound != fullName.Find( processFileNameAndExt ) )
	                {	                
	                captionPtr.Set( info.iCaption );
	                TRACE( T_LIT( "CBSSession::ExtractInfoL caption saved: %S"),&captionPtr );
	                break;
	                }	            
	            }	        
	        
	        CBSSessionInfo* temp = 
	            CBSSessionInfo::NewL( processFileNameAndExt,
	                                  captionPtr,
	                                  threadId, 
	                                  process.Id() );
	        TRACE( T_LIT( "CBSSession::ExtractInfoL SessionInfo object creation OK") );
	        delete iSessionInfo;
	        iSessionInfo = NULL;
	        iSessionInfo = temp;	        
	        CleanupStack::PopAndDestroy( &session );
	        }
	    CleanupStack::PopAndDestroy( &process );
	    }    
    CleanupStack::PopAndDestroy( &thread );    
    TRACE( T_LIT( "CBSSession::ExtractInfoL end") );
    }


// ---------------------------------------------------------------------------
// CBSSession::FileName
// ---------------------------------------------------------------------------
//
const TDesC& CBSSession::FileName()
	{
	TRACE( T_LIT( "CBSSession::FileName begin") );
	if ( iSessionInfo )
	    {
	    TRACE( T_LIT( "CBSSession::FileName SessionInfo exists") );
	    return iSessionInfo->FileName();
	    }
	else
	    {
	    TRACE( T_LIT( "CBSSession::FileName SessionInfo does not exist") );
	    return KNullDesC;
	    }
	}

// ---------------------------------------------------------------------------
// CBSSession::Caption
// ---------------------------------------------------------------------------
//	
const TDesC& CBSSession::Caption()
	{
	TRACE( T_LIT( "CBSSession::Caption begin") );
	if ( iSessionInfo )
	    {
	    TRACE( T_LIT( "CBSSession::Caption SessionInfo exists") );
	    return iSessionInfo->Caption();
	    }
	else
	    {
	    TRACE( T_LIT( "CBSSession::Caption SessionInfo does not exist") );
	    return KNullDesC;
	    }
	}	


// ---------------------------------------------------------------------------
// CBSSession::ThreadId
// ---------------------------------------------------------------------------
//		
TInt CBSSession::ThreadId( TThreadId& aThreadId )
	{
	TRACE( T_LIT( "CBSSession::ThreadId begin") );
	TInt err( KErrNotFound );
	if ( iSessionInfo )
	    {	    
	    TRACE( T_LIT( "CBSSession::ThreadId SessionInfo exists") );
	    aThreadId = iSessionInfo->ThreadId();
	    err = KErrNone;
	    }
	return err;
	}


// ---------------------------------------------------------------------------
// CBSSession::ProcessId
// ---------------------------------------------------------------------------
//	
TInt CBSSession::ProcessId( TProcessId& aProcessId )
	{
	TRACE( T_LIT( "CBSSession::ProcessId begin") );
	TInt err( KErrNotFound );
	if ( iSessionInfo )
	    {	    
	    TRACE( T_LIT( "CBSSession::ProcessId SessionInfo exists") );
	    aProcessId = iSessionInfo->ProcessId();
	    err = KErrNone;
	    }
	return err;
	}


// ---------------------------------------------------------------------------
// CBSSession::InfoAvailable
// ---------------------------------------------------------------------------
//	
TBool CBSSession::InfoAvailable()
    {
    return iSessionInfo ? ETrue : EFalse;
    }

// END OF FILE


