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
* Description:  cache session class implementation
*
*/


//  INCLUDE FILES
// local includes
#include "cimcachesession.h"

#include "cimcacheserver.h"
#include "ccacheserveractivehelper.h"
#include "cimcachemessagebase.h"
#include "mimcachemessageheader.h"
// logs
#include "imcachedebugtrace.h"
#include "cimcachebufferarray.h"

// ==============================================================
// ======================== SESSION =============================
// ==============================================================

// ---------------------------------------------------------
// CIMCacheSession::NewL
// Two-phased constructor.
// ---------------------------------------------------------
//
CIMCacheSession* CIMCacheSession::NewL()
    {
    TRACE( T_LIT("CIMCacheSession::NewL begin") );
    CIMCacheSession* self = new( ELeave ) CIMCacheSession(  );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    TRACE( T_LIT("CIMCacheSession::NewL end") );
    return self;
    }

// ---------------------------------------------------------
// CIMCacheSession::NewL
// Symbian OS default constructor can leave.
// ---------------------------------------------------------
//
void CIMCacheSession::ConstructL()
	{
	//This T-class is heap-allocated only because we do not want to 
	//include T-class header in header file of imcachesession.
	}

// ---------------------------------------------------------
// CIMCacheSession::NewL
// destructor
// ---------------------------------------------------------
//
CIMCacheSession::~CIMCacheSession()
    {
    TRACE( T_LIT("CIMCacheSession[%d]::~CIMCacheSession() start"), this );
    if(iObserverActive)
        {
        iObserverMessage.Complete( KErrCancel );
        iObserverActive = EFalse;   
        }
    if( Server() )
        {
        Server()->SessionDied( this );
        }
     iPacketsArray.ResetAndDestroy();
     
     ResetHeaderInoformation();	
     
     TRACE( T_LIT("CIMCacheSession::~CIMCacheSession() end "));
     }


// ---------------------------------------------------------
// CIMCacheSession::NewL
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------
//
CIMCacheSession::CIMCacheSession():
iOpenChatFetchCompleted( EFalse )
	{
 	TRACE( T_LIT("CIMCacheSession::CIMCacheSession"));
    }


// ---------------------------------------------------------
// CIMCacheSession::CreateL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCacheSession::CreateL()
    {
    TRACE( T_LIT("CIMCacheSession[%d]::CreateL()"), this );
    Server()->SessionCreatedL( this );
    TRACE( T_LIT( "CIMCacheSession::CreateL end") );
    }

// ---------------------------------------------------------
// CIMCacheSession::ServiceL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCacheSession::ServiceL( const RMessage2 &aMessage )
    {
	TRACE( T_LIT("CIMCacheSession::ServiceL	, begin"));
    if( DispatchMessageL( aMessage ) )
        {
        TRACE( T_LIT("CIMCacheSession::ServiceL inside  DispatchMessageL"));
        if( !iMessageCompleted  )
        	{
            TRACE( T_LIT("CIMCacheSession::ServiceL inside  !iMessageCompleted"));
        	aMessage.Complete( KErrNone );
        	}
        }
    TRACE( T_LIT("CIMCacheSession::ServiceL	, end"));
    }

// ---------------------------------------------------------
// CIMCacheSession::ServiceError
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCacheSession::ServiceError( const RMessage2& aMessage,
                                      TInt aError )
    {
    TRACE( T_LIT("CIMCacheSession::ServiceError	, begin"));
    aMessage.Complete( aError );
    TRACE( T_LIT("CIMCacheSession::ServiceError	, end"));
    }


// ---------------------------------------------------------
// CIMCacheSession::DispatchMessageL
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CIMCacheSession::DispatchMessageL( const RMessage2& aMessage )
    {
    TRACE( T_LIT("CIMCacheSession[%d]::DispatchMessageL() %d"), this, aMessage.Function() );
    
	iMessageCompleted = EFalse;
    TBool msgNeedsToBeCompleted = ETrue;
    
	  switch( aMessage.Function() )
        {
        // initialization operations
        case EIMCacheInitUpdate :
	        {
	        TRACE( T_LIT("CIMCacheSession::DispatchMessageL EIMCacheInitUpdate") );
	        InitSessionL( aMessage );
	       	break;	
	        }
	     // initialization operations
        case EIMCacheInitAccess :
	        {
	        TRACE( T_LIT("CIMCacheSession::DispatchMessageL EIMCacheInitAccess") );
	        InitSessionL( aMessage );
	        iAccessorInitialized = ETrue;
        	break;	
	        }
	     // initialization operations
        case EIMCacheInitConvesation :
	        {
	        TRACE( T_LIT("CIMCacheSession::DispatchMessageL EIMCacheInitAccess") );
	        InitSessionL( aMessage );
	        iConvesationInitialized = ETrue;
        	break;	
	        }
	     // update right operations
        case EIMCacheStartNewConversation :
        	{
        	TRACE( T_LIT("CIMCacheSession::DispatchMessageL EIMCacheStartNewConversation excuted") );
        	iHelper->StartNewConversationL(aMessage);
        	break;
        	}
      	case EIMCacheAppendSendMessage :
        	{
        	TRACE( T_LIT("CIMCacheSession::DispatchMessageL EIMCacheAppendSendMessage excuted") );
        	iHelper->AppendSendMessageL(aMessage);
        	break;
        	}
        case EIMCacheAppendMessage :
        	{
        	TRACE( T_LIT("CIMCacheSession::DispatchMessageL EIMCacheAppendMessage excuted") );
        	iHelper->AppendMessageL(aMessage);
        	break;
        	}
        case EIMCacheAppendReceiveMessage:
        	{
        	TRACE( T_LIT("CIMCacheSession::DispatchMessageL EIMCacheAppendReceiveMessage excuted") );
        	iHelper->AppendReceiveMessageL(aMessage);
        	break;
        	}
        case EIMCacheCloseConversation :
            {
            TRACE( T_LIT("CIMCacheSession::DispatchMessageL EIMCacheCloseConversation excuted") );
            iHelper->CloseConversationL(aMessage);
            break;
            }
        case EIMCacheCloseAllConversation :
            {
            TRACE( T_LIT("CIMCacheSession::DispatchMessageL EIMCacheCloseAllConversation excuted") );
            iHelper->CloseAllConversationL(iServiceId);
            break;
            }
        case EIMCacheDeactivateConversation:
        	{
        	TRACE( T_LIT("CIMCacheSession::DispatchMessageL EIMCacheDeactivateConversation excuted") );
        	iHelper->DeactivateConversationL();
        
        	break;
        	}
       	 
        case EIMCacheObserveMessageUpdateRegister:
        	{
        	TRACE( T_LIT("CIMCacheSession::DispatchMessageL EIMCacheObserveMessageUpdateRegister excuted") );
        	if( iObserverActive )
        		{
        		TRACE( T_LIT("CIMCacheSession::DispatchMessageL observer already active") );
        		// complete the old observer request with KErrCancel
        		iObserverMessage.Complete( KErrAlreadyExists );
        		}
        	iObserverMessage = aMessage;
        	iObserverActive = ETrue;
           	msgNeedsToBeCompleted = EFalse;
           	// check fetching still needed or not if needed pack and send again 
			if( iPacketsArray.Count() )
				{
				CIMCacheBufferArray* packet = iPacketsArray[ 0 ] ;
				iObserverMessage.Complete( packet->OperationCode() );
				iObserverActive = EFalse;
				}
			
			break;
        	}
        case EIMCacheObserveMessageUpdateUnRegister :
	        {
	        TRACE( T_LIT("CIMCacheSession::DispatchMessageL EIMCacheObserveMessageUpdateUnRegister excuted") );
	        if( !iObserverActive )
        		{
        		TRACE( T_LIT("CIMCacheSession::DispatchMessageL observer not active ") );
        		}
        	else
	        	{
	        	TRACE( T_LIT("CIMCacheSession::DispatchMessageL observer active ") );
				iObserverMessage.Complete( EIMOperationUnRegistered );
				iObserverActive = EFalse;		
	        	}
	        
            break;	
	        }
	     case EIMCacheObserveMessageAccessRegister:
        	{
        	TRACE( T_LIT("CIMCacheSession::DispatchMessageL EIMCacheObserveMessageAccessRegister excuted") );
        	if( iObserverActive )
        		{
        		TRACE( T_LIT("CIMCacheSession::DispatchMessageL observer already active") );
        		// complete the old observer request with KErrCancel
        		iObserverMessage.Complete( KErrAlreadyExists );
        		}
        	iObserverMessage = aMessage;
        	iObserverActive = ETrue;
            msgNeedsToBeCompleted = EFalse;
			 if( iPacketsArray.Count() )
				{
				CIMCacheBufferArray* packet = iPacketsArray[ 0 ] ;
				iObserverMessage.Complete( packet->OperationCode() );
				iObserverActive = EFalse;
				}
	     	break;
        	}
         case EIMCacheObserveMessageAccessUnRegister :
	        {
	        TRACE( T_LIT("CIMCacheSession::DispatchMessageL EIMCacheObserveMessageAccessUnRegister excuted") );
	        if( !iObserverActive )
        		{
        		TRACE( T_LIT("CIMCacheSession::DispatchMessageL observer not active ") );
        		}
        	else
	        	{
	        	TRACE( T_LIT("CIMCacheSession::DispatchMessageL observer active ") );
				iObserverMessage.Complete( EIMOperationUnRegistered );
				iObserverActive = EFalse;		
	        	}
	  
            break;	
	        }
       
        case EIMCacheGetUnreadCount:
        	{
        	iHelper->GetUnreadMessageCountL(aMessage);
        	break;
        	}
        case EIMCacheGetAllUnreadCount:
        	{
        	iHelper->GetAllUnreadMessageCountL(aMessage);
        	break;
        	}
       	   // check for conversation 
        case EIMCacheIsConversationExist :
	        {
	        TRACE( T_LIT("CIMCacheSession::DispatchMessageL EIMCacheIsConversationExist excuted ") );
	       	iHelper->CheckConversationExistL(aMessage);
	    	break;	
	        }
        // Get what change happen        	
        case EIMCacheGetBufferedPackets:
        	{
        	TRACE( T_LIT("CIMCacheSession::DispatchMessageL EIMCacheGetBufferedPackets excuted") );
        	if( iPacketsArray.Count() )
				{
				CIMCacheBufferArray* packet = iPacketsArray[ 0 ] ;
				aMessage.WriteL(0, packet->PacketData() );
				iPacketsArray.Remove(0);
				delete packet;
				packet = NULL;
				iPacketsArray.Compress();
				}
        	break;
        	}       
           	
        case EIMCacheGetChatList:
        	{
        	TRACE( T_LIT("CIMCacheSession::DispatchMessageL EIMCacheGetChatList executed") );
        	//0 serviceId 
        	//1 buffer
        	TInt serviceId = aMessage.Int0();       	
        	
        	ExternalizeChatItemL( EIMOperationFetchAll , serviceId );
        	
        	//aMessage.WriteL( 1, *iChatDataBuffer); 
        	TInt packetsCounts = iPacketsArray.Count();
        
        	if( packetsCounts )
				{
				CIMCacheBufferArray* packet = iPacketsArray[ 0 ] ;
				aMessage.WriteL( 1, packet->PacketData() );	
				iPacketsArray.Remove(0);
				delete packet;
				packet = NULL;
				iPacketsArray.Compress();
				}
            break;	
        	}
        case EIMCacheCancelRequest:
            {
            TRACE( T_LIT("CIMCacheSession::EIMCacheCancelRequest start"));
            if(iObserverActive)
                {
                TRACE( T_LIT("CIMCacheSession::EIMCacheCancelRequest Inside if"));
                iObserverMessage.Complete( KErrCancel );
                TRACE( T_LIT("CIMCacheSession::EIMCacheCancelRequest After iObserverMessage.Complete"));
                iObserverActive = EFalse;   
                }
            TRACE( T_LIT("CIMCacheSession::EIMCacheCancelRequest End"));

          
            break;
            }
        default:
            {
            TRACE( T_LIT("CIMCacheSession::DispatchMessageL default excuted") );
            User::Leave( KErrNotSupported );
            break;
            }
        }
	TRACE( T_LIT("CIMCacheSession::DispatchMessageL	end"));
    return msgNeedsToBeCompleted;
   
    }
 //=======================================================================
//==========================initialization ==================================
//========================================================================

// ---------------------------------------------------------
// CIMCacheSession::InitSessionL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCacheSession::InitSessionL(const RMessage2& aMessage )
	{
    TRACE( T_LIT("CIMCacheSession::InitSessionL:start") );
    // at zero place service id was appended
    iServiceId = aMessage.Int0();
	if( Server() )
        {
        iHelper = Server()->GetActiveHelper();
        }
    TRACE( T_LIT("CIMCacheSession::InitSessionL end") );
	} 


// ---------------------------------------------------------
// CIMCacheSession::InitSessionL
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CIMCacheSession::ServiceId() const
    {
    TRACE( T_LIT("CIMCacheSession::ServiceId:start") );
    TRACE( T_LIT("CIMCacheSession::ServiceId end") );
    return iServiceId;
    } 
// ---------------------------------------------------------------------------
// CIMCacheSession::ResetHeaderInoformation()
// ---------------------------------------------------------------------------
//
void CIMCacheSession::ResetHeaderInoformation()
                                                      
    {
    TRACE( T_LIT( "CIMCacheSession::ResetHeaderInoformation begin") );
    if( iConvesationInitialized )
	    {
	    iHelper->ResetHeaderInoformation();	
	    }
   	TRACE( T_LIT( "CIMCacheSession::ResetHeaderInoformation end") );
    }
 
 
// ---------------------------------------------------------
// CIMCacheSession::PackAndNotifyEventL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCacheSession::PackAndNotifyEventL(TIMCacheOperationsCode aChangeType ,
										  MIMCacheMessageHeader* aChatHeader /*=NULL*/, 
									 	  CIMCacheMessageBase* aMessage /*=NULL*/ ) 

    {	
    TRACE( T_LIT( "CIMCacheSession::PackAndNotifyEventL() begin") );

    switch( aChangeType )
        {
        case EIMOperationFetchAll :
            {
            SendChatListDataL(aChangeType, aChatHeader);
            break;	
            }
        case EIMOperationUnreadChange :
            {
            if(  iObserverActive && iConvesationInitialized )
                {
                iObserverMessage.Complete( aChangeType );
                iObserverActive = EFalse;		
                }
            else if( iObserverActive && iAccessorInitialized  )
                {
                ExternalizeSingleChatItemL(aChangeType, aChatHeader );
                iObserverMessage.Complete( aChangeType );
                iObserverActive = EFalse;
                }
            break;	
            }
        case EIMOperationUnreadMessage :
            {
            SendUnreadChangeNotificationL( aChatHeader, aMessage );
            break;	
            }
        case EIMOperationChatStarted :
            {
            if( iObserverActive && iConvesationInitialized )
                {
                iObserverMessage.Complete( aChangeType );
                iObserverActive = EFalse;		
                }
            else if( iObserverActive && iAccessorInitialized )
                {
                ExternalizeSingleChatItemL(aChangeType, aChatHeader );
                iObserverMessage.Complete( aChangeType );
                iObserverActive = EFalse;   
                }
            break;	
            }
        case EIMOperationChatAdded :
            {
            if(  iConvesationInitialized )
                {
                ExternalizeSingleChatItemL(aChangeType, aChatHeader);
                if( iObserverActive )
                    {
                    iObserverMessage.Complete( aChangeType );
                    iObserverActive = EFalse;	
                    }
                }
            else if( iAccessorInitialized )
                {
                SendUnreadChangeNotificationL(aChatHeader, aMessage );
                }
            break;	
            }
        case EIMOperationMessageAdded :
            {
            if( iConvesationInitialized )
                {
                ExternalizeSingleChatDataL(aChangeType, aChatHeader , aMessage );
                if( iObserverActive )
                    {
                    iObserverMessage.Complete( aChangeType );
                    iObserverActive = EFalse;
                    }
                }
            break;	
            }
        case EIMOperationChatDeleted :
            {
            if( ( iConvesationInitialized || iAccessorInitialized  ) && iObserverActive )
                {
                ExternalizeSingleChatItemL(aChangeType, aChatHeader );
                }
            if( iObserverActive )
                {
                iObserverMessage.Complete( aChangeType );
                iObserverActive = EFalse;	
                }
            break;	
            }
        case EIMOperationAllChatDeleted:
            {
            if( ( iConvesationInitialized || iAccessorInitialized  ) && iObserverActive )
                {
                iObserverMessage.Complete( aChangeType );
                iObserverActive = EFalse;   
                }
            break;
            }
        case EIMOperationCompleted :
            {
            if( iObserverActive && iConvesationInitialized )
                {
                iObserverMessage.Complete( aChangeType );
                iObserverActive = EFalse;		
                }
            break;	
            }

        }

    }
// ---------------------------------------------------------
// CIMCacheSession::SendUnreadChangeNotificationL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCacheSession::SendUnreadChangeNotificationL( MIMCacheMessageHeader* aMsgHeader, 
												 	CIMCacheMessageBase* aMessage )	
	{
	// write the message into continuous memory stream
	if( iAccessorInitialized  || 
		iConvesationInitialized && aMsgHeader->IsChatStarted() )
		{
		ExternalizeSingleChatDataL(EIMOperationUnreadMessage, aMsgHeader, aMessage );
		if( iObserverActive )
			{
			iObserverMessage.Complete( EIMOperationUnreadMessage );
			iObserverActive = EFalse;	
			}
		}

	TRACE( T_LIT( "CIMCacheSession::SendUnreadChangeNotificationL() end") );
	}

// ---------------------------------------------------------
// CIMCacheSession::SendChatListDataL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCacheSession::SendChatListDataL( TIMCacheOperationsCode aChangeType ,
									     MIMCacheMessageHeader* aMsgHeader )

	{
	
	TRACE( T_LIT( "CIMCacheSession::SendChatListChangeNotificationL() begin") );
   
	// write the message into continuous memory stream
	if( iConvesationInitialized )
		{
		ExternalizeChatDataL(aChangeType, aMsgHeader );
		if( iObserverActive )
			{
			iObserverMessage.Complete( EIMOperationDataAvailable );
			iObserverActive = EFalse;	
			}
		}
	else if( iObserverActive && iAccessorInitialized  )
		{
        ExternalizeSingleChatItemL(aChangeType, aMsgHeader );
		iObserverMessage.Complete( EIMOperationUnreadChange );
		iObserverActive = EFalse;		
		}
		
	TRACE( T_LIT( "CIMCacheSession::SendChatListChangeNotificationL() end") );
	
	}

// ---------------------------------------------------------
// CIMCacheSession::ExternalizeSingleChatItemL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCacheSession::ExternalizeSingleChatItemL( TIMCacheOperationsCode aChangeType, MIMCacheMessageHeader* aMsgHeader )
	{

	 TRACE( T_LIT( "CIMCacheSession::ExternalizeSingleChatItemL() Start") );

    RPointerArray<HBufC8> bufferedHeaderArray;
    bufferedHeaderArray.Reset();
    

    TInt availableSize = KMaxSize - 2*sizeof(TInt);//messageCount + more.
    TRACE( T_LIT( "CIMCacheSession::ExternalizeSingleChatItemL() availableSize ---- %d"),availableSize );

    TBool more = EFalse;

    if( aMsgHeader != NULL )
        {
        TRACE( T_LIT( "CIMCacheSession::ExternalizeSingleChatItemL() inside if aMsgHeader") );
        TInt dataSize = sizeof( TInt )+ //for service id.               
                    sizeof( TInt )+ aMsgHeader->BuddyId().Size() ; // for buddyId length and buddyId.

        availableSize = availableSize - dataSize - sizeof( dataSize );
        TRACE( T_LIT( "CIMCacheSession::ExternalizeSingleChatItemL() inside if aMsgHeader availableSize --- %d"), availableSize );

        if( availableSize > 0 )
            {
            HBufC8* tempBuf = GetPackedChatItemBufferL( aMsgHeader , dataSize );
            bufferedHeaderArray.AppendL( tempBuf );
            }
        }
	    
	    ExternalizeBufferedDataL(aChangeType, bufferedHeaderArray , KMaxSize , more );
	    TRACE( T_LIT( "CIMCacheSession::ExternalizeSingleChatItemL() after ExternalizeBufferedDataL") );
	    bufferedHeaderArray.ResetAndDestroy(); // delete all buffered contacts.
	    TRACE( T_LIT( "CIMCacheSession::ExternalizeSingleChatItemL() after bufferedHeaderArray.resetanddestroy") );
	    TRACE( T_LIT( "CIMCacheSession::ExternalizeSingleChatItemL() Start") );
	}
// ---------------------------------------------------------
// CIMCacheSession::ExternalizeChatItemL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCacheSession::ExternalizeChatItemL( TIMCacheOperationsCode aChangeType, 
										    TInt aServiceId /*= KErrNotFound*/ )
	{
	
    TRACE( T_LIT( "CIMCacheSession::ExternalizeL() begin") );
    
   	TInt availableSize = KMaxSize - 2*sizeof(TInt);
   
   	RPointerArray<HBufC8> bufferedHeaderArray;
   	
	TBool more = EFalse;
	TInt i = 0;
    if( aChangeType == EIMOperationFetchAll )
        {
        RPointerArray<MIMCacheMessageHeader> tempHeaderArray;
    	iHelper->GetServiceChatHeaderArrayL( aServiceId , tempHeaderArray );
  
        TInt chatCount = tempHeaderArray.Count();
         
        for( i=0; i < chatCount; i++ )
			{
			MIMCacheMessageHeader* chatHeader = tempHeaderArray[ i ];

			TInt dataSize = sizeof( TInt )+ //for service id.               
			sizeof( TInt )+ chatHeader->BuddyId().Size() ; // for buddyId length and buddyId.

			availableSize = availableSize - dataSize - sizeof( dataSize );

			if( availableSize > 0 )
				{
				HBufC8* tempBuf = GetPackedChatItemBufferL( chatHeader , dataSize );
				bufferedHeaderArray.AppendL( tempBuf );
				}
			else
				{
				more = ETrue;
				ExternalizeBufferedDataL(aChangeType, bufferedHeaderArray , KMaxSize , more );
				bufferedHeaderArray.ResetAndDestroy(); // delete all buffered contacts.
				availableSize = KMaxSize - 2*sizeof(TInt);
				//preserve information for next trial.
				}
			}
        if( i == chatCount )
            {
            more = EFalse;
            ExternalizeBufferedDataL(aChangeType, bufferedHeaderArray , KMaxSize , more );
            }
        }
 
    bufferedHeaderArray.ResetAndDestroy(); // delete all buffered contacts.
    
    TRACE( T_LIT( "CIMCacheSession::ExternalizeL() end") );
    }
// ---------------------------------------------------------
// CIMCacheSession::ExternalizeBufferedDataL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCacheSession::ExternalizeBufferedDataL(TIMCacheOperationsCode aOperationCode,
											   RPointerArray<HBufC8>& aArray , 
											   TInt aTotalSize , 
											   TBool aMore /*= EFalse*/)
	{
	TRACE( T_LIT( "CIMCacheSession::ExternalizeBufferedDataL() begin") );
	TInt appendedMsgCount = aArray.Count(); 
	CBufFlat* flatBuf = CBufFlat::NewL(aTotalSize);//( aTotalSize + ( sizeof(TInt) * appendedMsgCount ) + sizeof(TInt) );
	CleanupStack::PushL( flatBuf );
	RBufWriteStream writeAllStream ;
	CleanupClosePushL( writeAllStream );

	writeAllStream.Open( *flatBuf );
	writeAllStream.WriteInt16L( appendedMsgCount );
	
	for(TInt i = 0; i< appendedMsgCount ; i++ )
		{
		HBufC8* buf =  aArray[i];
		writeAllStream.WriteInt16L(buf->Length());
		writeAllStream.WriteL(*buf);			
		}
	
	writeAllStream.WriteInt16L( aMore );//write more information at end.
	
	writeAllStream.CommitL(); 

	CIMCacheBufferArray* packet = CIMCacheBufferArray::NewL( flatBuf->Ptr(0) , aOperationCode );
	iPacketsArray.Append(packet); // ownership to array
	
	CleanupStack::PopAndDestroy(); // writeAllStream
	CleanupStack::PopAndDestroy(); // flatBuf
	
	TRACE( T_LIT( "CIMCacheSession::ExternalizeBufferedDataL() end") );
	
	}  	
 
// ---------------------------------------------------------
// CIMCacheSession::ExternalizeSingleChatDataL
// (other items were commented in a header).
// ---------------------------------------------------------
void CIMCacheSession::ExternalizeSingleChatDataL(TIMCacheOperationsCode aChangeType, MIMCacheMessageHeader* aChatHeader, CIMCacheMessageBase* aMessage )
	{
	
	TInt availableSize = KMaxSize - 2*sizeof(TInt);//messageCount + more.

	TBool more = EFalse;

	if( aMessage && aChatHeader )
		{
		if( aChangeType == EIMOperationMessageAdded )
			{
			aMessage->SetRead()	;
			}
		RPointerArray<HBufC8> bufferedChatArray;
			
		TInt dataSize = sizeof( aMessage->MessageType() )+
		                sizeof( aMessage->MessagerType() )+
		                sizeof( aMessage->ContentType() )+
		                sizeof( aMessage->TimeStamp().Int64() )+  
		                sizeof( TInt )+ aChatHeader->BuddyId().Size() + // for buddy  length and buddy name
		                sizeof( TInt )+ aMessage->Text().Size() ; // for message length and message
		             
		availableSize = availableSize - dataSize - sizeof( dataSize );
		if( availableSize > 0 )
		    {
		     HBufC8* messageBuffer = GetPackedChatDataBufferL( aMessage , aChatHeader , dataSize );
		     bufferedChatArray.AppendL( messageBuffer );
		    }
		    
		ExternalizeBufferedDataL(aChangeType, bufferedChatArray , KMaxSize , more );
		bufferedChatArray.ResetAndDestroy();
		}
	}

// ---------------------------------------------------------
// CIMCacheSession::ExternalizeChatDataL
// (other items were commented in a header).
// ---------------------------------------------------------
void CIMCacheSession::ExternalizeChatDataL(TIMCacheOperationsCode aChangeType ,MIMCacheMessageHeader* aChatHeader )
    {    
    RPointerArray<HBufC8> bufferedChatArray;
    bufferedChatArray.Reset();
    
    TInt availableSize = KMaxSize - 2*sizeof(TInt);//messageCount + more.
   
    TBool more = EFalse;
     
    if( aChangeType == EIMOperationFetchAll &&  aChatHeader )
        { 
        TInt messageCount = aChatHeader->MessageCount();
    	
   		TInt index = 0;
        for( index = 0 ; index < messageCount  ; index++ )
            {
            CIMCacheMessageBase* message = aChatHeader->GetMessage(index);
            TInt dataSize = sizeof( message->MessageType() )+
                            sizeof( message->MessagerType() )+
                            sizeof( message->ContentType() )+
                            sizeof( message->TimeStamp().Int64() )+  
                            sizeof( TInt )+ aChatHeader->BuddyId().Size() + // for buddy  length and buddy name
                            sizeof( TInt )+ message->Text().Size() ; // for message length and message
                         
            availableSize = availableSize - dataSize - sizeof( dataSize );
            if( availableSize > 0 )
                {
                // mark message read
    			 message->SetRead();
                 HBufC8* messageBuffer = GetPackedChatDataBufferL( message , aChatHeader , dataSize );
                 bufferedChatArray.AppendL( messageBuffer );
                 }
            else
                {
                //else set information for more and message nest to last sent message i.e index
                more = ETrue;
                ExternalizeBufferedDataL(aChangeType, bufferedChatArray , KMaxSize , more );
    			bufferedChatArray.ResetAndDestroy();  
    			availableSize = KMaxSize - 2*sizeof(TInt); //messageCount + more.  
                index--;
                }
            }
        if( index == messageCount )
			{
			more = EFalse;
			ExternalizeBufferedDataL(aChangeType, bufferedChatArray , KMaxSize , more );
			}
        }
     bufferedChatArray.ResetAndDestroy();    
    }
// ---------------------------------------------------------
// CIMCacheSession::GetPackedChatDataBufferL
// (other items were commented in a header).
// ---------------------------------------------------------
HBufC8* CIMCacheSession::GetPackedChatDataBufferL(CIMCacheMessageBase* aMsg , MIMCacheMessageHeader* aActiveHeader , const TInt aSize)
    {
    TRACE( T_LIT( "CIMCacheSession::GetPackedChatDataBufferL() begin") );
    

    CBufFlat*   flatBuf = CBufFlat::NewL( aSize );
    CleanupStack::PushL( flatBuf );

    RBufWriteStream writeStream ;
    CleanupClosePushL( writeStream );

    writeStream.Open( *flatBuf );

    writeStream.WriteInt16L( aMsg->MessageType() );  
    writeStream.WriteInt16L( aMsg->MessagerType() );
    writeStream.WriteInt16L( aMsg->ContentType() );   
    writeStream.WriteReal64L( aMsg->TimeStamp().Int64() );
    writeStream.WriteInt16L( aActiveHeader->BuddyId().Length() );    
    writeStream.WriteL( aActiveHeader->BuddyId() );
    writeStream.WriteInt16L( aMsg->Text().Length() );    
    writeStream.WriteL( aMsg->Text() );
    
    writeStream.CommitL();

    HBufC8* tempBuffer = flatBuf->Ptr(0).AllocL();
    
    CleanupStack::PopAndDestroy(); // flatBuf
    CleanupStack::PopAndDestroy(); // writeStream
    
    TRACE( T_LIT( "CIMCacheSession::GetPackedChatDataBufferL() end") ); 
    return tempBuffer;
    }

// ---------------------------------------------------------
// CIMCacheSession::GetPackedChatItemBufferL
// (other items were commented in a header).
// ---------------------------------------------------------
//
HBufC8* CIMCacheSession::GetPackedChatItemBufferL(MIMCacheMessageHeader* aChatHeader, TInt& aSize)
    {
    TRACE( T_LIT( "CIMCacheSession::GetPackedChatItemBufferL() end") );
    
    TInt  serviceId   = aChatHeader->ServiceId();
    const TDesC& buddyId = aChatHeader->BuddyId();
             
     CBufFlat*   flatBuf = CBufFlat::NewL( aSize );
     CleanupStack::PushL( flatBuf );
     
     RBufWriteStream writeStream ;
     CleanupClosePushL( writeStream );
     writeStream.Open( *flatBuf );
        
     writeStream.WriteInt16L( serviceId );
     writeStream.WriteInt16L( buddyId.Length() );
     writeStream.WriteL( buddyId );       
     writeStream.CommitL();
     
     HBufC8* chatHeaderBuffer = flatBuf->Ptr(0).AllocL();
    
     CleanupStack::PopAndDestroy(2); // writeAllStream,flatBuf
     
     TRACE( T_LIT( "CIMCacheSession::GetPackedChatItemBufferL() end") );
     
     return chatHeaderBuffer;
    }


// end of file
