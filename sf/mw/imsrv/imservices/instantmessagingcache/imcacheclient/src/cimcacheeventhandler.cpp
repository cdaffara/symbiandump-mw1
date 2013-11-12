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
* Description:  cache message change observer
*
*/


//  INCLUDE FILES

#include "cimcacheeventhandler.h"
#include "mimcacheclient.h"
#include "mimcacheeventhandler.h"
// logs
#include "imcachedebugtrace.h"

// -----------------------------------------------------------------------------
// CIMCacheEventHandler::NewL
// -----------------------------------------------------------------------------
//
CIMCacheEventHandler* CIMCacheEventHandler::NewL( 
								MIMCacheClient& aRegistrar, 
								MIMCacheEventHandler& aHandler)
    {
    TRACE( T_LIT("CIMCacheEventHandler::NewL() begin") );
	CIMCacheEventHandler* self = new ( ELeave ) CIMCacheEventHandler( aRegistrar ,aHandler ) ;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );  //self
	TRACE( T_LIT("CIMCacheEventHandler::NewL() end") );
    return self;
    }

// -----------------------------------------------------------------------------
// CIMCacheEventHandler::ConstructL
// -----------------------------------------------------------------------------
//
void CIMCacheEventHandler::ConstructL()
    {
    TRACE( T_LIT("CIMCacheEventHandler::ConstructL() begin") );
    iContinueObserving = ETrue ;
   	CActiveScheduler::Add( this );
	iRegistrar.RegisterObserverToServerL( iStatus ,EIMCacheObserveMessageUpdateRegister );
	SetActive();
	TRACE( T_LIT("CIMCacheEventHandler::ConstructL() end") );
    }

// -----------------------------------------------------------------------------
// CIMCacheEventHandler::~CIMCacheEventHandler
// -----------------------------------------------------------------------------
//
CIMCacheEventHandler::~CIMCacheEventHandler()
	{
	TRACE( T_LIT("CIMCacheEventHandler::~CIMCacheEventHandler() begin") );
	Cancel();
	TRACE( T_LIT("CIMCacheEventHandler::~CIMCacheEventHandler() end") );
	}

// -----------------------------------------------------------------------------
// CIMCacheEventHandler::CIMCacheEventHandler(
// -----------------------------------------------------------------------------
//
CIMCacheEventHandler::CIMCacheEventHandler(
								 MIMCacheClient& aRegistrar , 
								 MIMCacheEventHandler& aHandler ) :
	CActive( CActive::EPriorityIdle ),
  	iRegistrar( aRegistrar ),
  	iUpdateHandler( aHandler )
 	{
 	TRACE( T_LIT("CIMCacheEventHandler::CIMCacheEventHandler()") );
	}
// -----------------------------------------------------------------------------
// CIMCacheEventHandler::RunL
// -----------------------------------------------------------------------------
//
void CIMCacheEventHandler::RunL()
	{
	TRACE( T_LIT("CIMCacheEventHandler::RunL() begin") );
	TRACE( T_LIT("CIMCacheEventHandler::RunL() iStatus code = "),iStatus.Int()  );

	switch( iStatus.Int() )
		{
		case EIMOperationUnreadMessage :
		case EIMOperationDataAvailable :
		case EIMOperationMessageAdded :
			{
			// message data available get the data	
            GetBufferChatDataL();
			break;	
			}
	   case EIMOperationCompleted :
	   		{
	   		iUpdateHandler.HandleIMCacheEventL( EIMCacheRequestCompleted, NULL );
			break;	
	   		}
	    case EIMOperationChatStarted :
		    {
		    iUpdateHandler.HandleIMCacheEventL( EIMCacheRequestCompleted, NULL );
		    break;	
		    }
		case EIMOperationUnreadChange:
			{
			iUpdateHandler.HandleIMCacheEventL( EIMCacheRequestCompleted, NULL );
			break;	
			}
		case EIMOperationChatAdded :
			{
			// chat information
		    GetBufferedChatItemL(EIMCacheNewChat);
			break;	
			}
		case EIMOperationChatDeleted :
			{
			// message data available get the data	
            GetBufferedChatItemL(EIMCacheChatClosed );		
			break;	
			}
		 case EIMOperationAllChatDeleted:
		     {
		     iUpdateHandler.HandleIMCacheEventL( EIMCacheAllChatClosed, NULL );
		     break;
		     }
		case EIMOperationUnRegistered:
			{
			TRACE( T_LIT("CIMCacheEventHandler::RunL() EIMOperationUnRegistered") );
			iContinueObserving = EFalse;	
			break;	
			}
		case KErrServerTerminated:
            {
            // This case handles when the server terminated status is set.
            // This is scenario is typically scene when imcache server is uninstalled.

            UnRegisterObserver();
            DoCancel();
            break;
            }

		default :
		    {
		    TRACE( T_LIT("CIMCacheEventHandler::RunL() default") );
		    break;	
		    }
		}	
		
	// Comment: cancel operation is changes as synchronous. iContinueObserving from event
	// can not really be trusted since complete code might be different if leave has occured 
	// for example. Cancel is handled here as generic cancel.
	TRACE( T_LIT("CIMCacheEventHandler::RunL() iContinueObserving iContinueObserving = %d") ,iContinueObserving);
	// check need to continue observing to server
	if( iContinueObserving && KErrCancel != iStatus.Int() )
		{
		iRegistrar.RegisterObserverToServerL( iStatus ,EIMCacheObserveMessageUpdateRegister );
		SetActive();
		}
	TRACE( T_LIT("CIMCacheEventHandler::RunL() end") );
	}
// -----------------------------------------------------------------------------
// CIMCacheEventHandler::DoCancel
// -----------------------------------------------------------------------------
//	
void CIMCacheEventHandler::DoCancel()
	{
	TRACE( T_LIT("CIMCacheEventHandler::DoCancel() ") );
	// Comment: set to EFalse here since we can not trust other changes. See
	// comments in RunL
	iContinueObserving = EFalse;
	if( IsActive() )
        {
        TRAP_IGNORE(iRegistrar.CancelRequestL( iStatus ,EIMCacheCancelRequest ));
        }
	}
// ---------------------------------------------------------
// CIMCacheEventHandler::RunError()
// ---------------------------------------------------------
//
TInt CIMCacheEventHandler::RunError( TInt /* aError */)
    {
    TRACE( T_LIT("CVIMPSTEngineCVListener::RunError() start"));
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CIMCacheEventHandler::UnRegisterObserver
// -----------------------------------------------------------------------------
//		
void CIMCacheEventHandler::UnRegisterObserver()
	{
	TRACE( T_LIT("CIMCacheEventHandler::UnRegisterObserver() begin") );
	if( IsActive() )
		{
		TRACE( T_LIT("CIMCacheEventHandler::UnRegisterObserver() active") );
		TRAP_IGNORE(iRegistrar.UnRegisterObserverToServerL( iStatus ,EIMCacheObserveMessageUpdateUnRegister ));
		}
	TRACE( T_LIT("CIMCacheEventHandler::UnRegisterObserver() end") );	
	}
// -----------------------------------------------------------------------------
// CIMCacheEventHandler::GetBufferChatDataL
// -----------------------------------------------------------------------------
//
void CIMCacheEventHandler::GetBufferChatDataL( ) 
    {
    TRACE( T_LIT("CIMCacheEventHandler::GetBufferChatDataL() begin") );
	TBool more = ETrue;
    while(more)
        {
        TPtr8 bufferPtr = iRegistrar.GetBufferedDataL( EIMCacheGetBufferedPackets );
        InternalizeChatDataL( bufferPtr , more );
        }
    TRACE( T_LIT("CIMCacheEventHandler::GetBufferChatDataL() end") );
    }
// -----------------------------------------------------------------------------
// CIMCacheEventHandler::InternalizeChatDataL
// -----------------------------------------------------------------------------     
void CIMCacheEventHandler::InternalizeChatDataL( TPtr8 aChatDataBuffer , TBool& aMore )
    {
    TRACE( T_LIT("CIMCacheEventHandler::InternalizeChatDataL() Start") );
    if( 0 == aChatDataBuffer.Length())
        {
        TRACE( T_LIT("CIMCacheEventHandler::InternalizeChatDataL()  aChatDataBuffer is zeor, hence returning ") );
        return;
        }
   	RDesReadStream readAllStream ;
	RDesReadStream readStream ;    

	readAllStream.Open( aChatDataBuffer );
	CleanupClosePushL(readAllStream);

	TInt messageCount = readAllStream.ReadInt16L();

	for( TInt i=0; i< messageCount; i++)
		{
		SIMCacheMessageData chatData =
		    {
		    TIMCacheMessageType(0),
		    };
		        
		TInt size = readAllStream.ReadInt16L() ;
		HBufC8* mBuffer  = HBufC8::NewLC( size );
		TPtr8 mBufferPtr = mBuffer->Des();
		        
		readAllStream.ReadL( mBufferPtr , size );

		readStream.Open( *mBuffer );        
		CleanupClosePushL(readStream) ;

		chatData.iMessageType   = static_cast<TIMCacheMessageType>( readStream.ReadInt16L() );
		chatData.iMessagerType  = static_cast<TIMCacheMessagerType>( readStream.ReadInt16L() );
		chatData.iContentType   = static_cast<TIMCacheContentType>( readStream.ReadInt16L() );
		chatData.iTime          = static_cast<TReal64>( readStream.ReadReal64L() );

		TInt buddyLength = readStream.ReadInt16L();
		HBufC* buddy     = HBufC::NewLC( buddyLength );
		TPtr buddyPtr    = buddy->Des();
		readStream.ReadL( buddyPtr , buddyLength );
		chatData.iBuddyId = buddy;

		TInt textSize = readStream.ReadInt16L();
		HBufC* text  = HBufC::NewLC( textSize );
		TPtr textPtr = text->Des(); 
		readStream.ReadL( textPtr , textSize);            
		chatData.iText = text;
		
		iUpdateHandler.HandleIMCacheEventL(EIMCacheNewMessage , &chatData);
		
		CleanupStack::PopAndDestroy(4);//close read stream, delete mBuffer, text, buddy 
		}

	aMore = static_cast<TBool>( readAllStream.ReadInt16L() );

	CleanupStack::PopAndDestroy();//readAllStream.    
	
    TRACE( T_LIT("CIMCacheEventHandler::InternalizeChatDataL() End") );
    }

// -----------------------------------------------------------------------------
// CIMCacheEventHandler::GetBufferedChatItemL
// -----------------------------------------------------------------------------
//
void CIMCacheEventHandler::GetBufferedChatItemL( TIMCacheEventType aEventType )
    {
    TRACE( T_LIT("CIMCacheAccessEventHandler::GetBufferedChatItemL() begin") );
    
    TPtr8 bufferPtr = iRegistrar.GetBufferedDataL( EIMCacheGetBufferedPackets );
    
    InternalizeChatItemL( aEventType, bufferPtr );
    
    TRACE( T_LIT("CIMCacheAccessEventHandler::GetBufferedChatItemL() end") );
    }


// -----------------------------------------------------------------------------
// CIMCacheEventHandler::InternalizeChatItemL
// -----------------------------------------------------------------------------
//
void CIMCacheEventHandler::InternalizeChatItemL( TIMCacheEventType aEventType, 
                                                 TPtr8 aContactBuffer )
    {
    TRACE( T_LIT("CIMCacheClient::InternalizeChatItemL() begin") );   
    if( 0 == aContactBuffer.Length())
          {
          TRACE( T_LIT("CIMCacheEventHandler::InternalizeChatDataL()  aChatDataBuffer is zeor, hence returning ") );
          return;
          }
    RDesReadStream readAllStream ;
    RDesReadStream readStream ;    
    
    readAllStream.Open( aContactBuffer );
    CleanupClosePushL(readAllStream);
    
    TInt messageCount = readAllStream.ReadInt16L();
    
    for( TInt i=0; i< messageCount; i++)
        {
        SIMCacheChatItem chatItem =
          {
          0,
          };
                
        TInt size = readAllStream.ReadInt16L() ;
        HBufC8* mBuffer  = HBufC8::NewLC( size );
        TPtr8 mBufferPtr = mBuffer->Des();
                
        readAllStream.ReadL( mBufferPtr , size );
        
        readStream.Open( *mBuffer );        
        CleanupClosePushL(readStream) ;
        
        chatItem.iServiceId  = static_cast<TInt>( readStream.ReadInt16L() );
        
        TInt textSize = readStream.ReadInt16L();
        HBufC* buddyId = HBufC::NewLC( textSize );
        TPtr buddyIdPtr = buddyId->Des(); 
        readStream.ReadL( buddyIdPtr , textSize);
        
        chatItem.iBuddyId = buddyId;
        
        iUpdateHandler.HandleIMCacheEventL( aEventType , &chatItem );
      
        CleanupStack::PopAndDestroy(3);//buddyId , close read stream, delete mBuffer.      
                  
        }
 
    CleanupStack::PopAndDestroy();//readAllStream.    
    
    TRACE( T_LIT("CIMCacheClient::InternalizeChatItemL() End") );
    }



//  end of file

