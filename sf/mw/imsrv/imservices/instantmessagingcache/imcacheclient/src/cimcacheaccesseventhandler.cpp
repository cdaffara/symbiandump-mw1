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

#include "cimcacheaccesseventhandler.h"
#include "mimcacheclient.h"
#include "mimcacheeventhandler.h"
// logs
#include "imcachedebugtrace.h"

// -----------------------------------------------------------------------------
// CIMCacheAccessEventHandler::NewL
// -----------------------------------------------------------------------------
//
CIMCacheAccessEventHandler* CIMCacheAccessEventHandler::NewL( 
								MIMCacheClient& aRegistrar, 
								MIMCacheEventHandler& aHandler )
    {
    TRACE( T_LIT("CIMCacheAccessEventHandler::NewL() begin") );
	CIMCacheAccessEventHandler* self = new ( ELeave ) CIMCacheAccessEventHandler( aRegistrar ,aHandler ) ;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );  //self
	TRACE( T_LIT("CIMCacheAccessEventHandler::NewL() end") );
    return self;
    }

// -----------------------------------------------------------------------------
// CIMCacheAccessEventHandler::ConstructL
// -----------------------------------------------------------------------------
//
void CIMCacheAccessEventHandler::ConstructL()
    {
    TRACE( T_LIT("CIMCacheAccessEventHandler::ConstructL() begin") );
    iContinueObserving = ETrue ;
   	CActiveScheduler::Add( this );
	iRegistrar.RegisterObserverToServerL( iStatus ,EIMCacheObserveMessageAccessRegister );
	SetActive();
	TRACE( T_LIT("CIMCacheAccessEventHandler::ConstructL() end") );
    }

// -----------------------------------------------------------------------------
// CIMCacheAccessEventHandler::~CIMCacheAccessEventHandler
// -----------------------------------------------------------------------------
//
CIMCacheAccessEventHandler::~CIMCacheAccessEventHandler()
	{
	TRACE( T_LIT("CIMCacheAccessEventHandler::~CIMCacheAccessEventHandler() begin") );
	Cancel();
	TRACE( T_LIT("CIMCacheAccessEventHandler::~CIMCacheAccessEventHandler() end") );
	}

// -----------------------------------------------------------------------------
// CIMCacheAccessEventHandler::CIMCacheAccessEventHandler(
// -----------------------------------------------------------------------------
//
CIMCacheAccessEventHandler::CIMCacheAccessEventHandler(
								 MIMCacheClient& aRegistrar , 
								 MIMCacheEventHandler& aHandler) :
	CActive( CActive::EPriorityIdle ),
  	iRegistrar( aRegistrar ),
  	iAccessHandler( aHandler )
 	{
 	TRACE( T_LIT("CIMCacheAccessEventHandler::CIMCacheAccessEventHandler()") );
	}
// -----------------------------------------------------------------------------
// CIMCacheAccessEventHandler::RunL
// -----------------------------------------------------------------------------
//
void CIMCacheAccessEventHandler::RunL()
	{
	TRACE( T_LIT("CIMCacheAccessEventHandler::RunL() begin") );
	TRACE( T_LIT("CIMCacheAccessEventHandler::RunL() iStatus code = "),iStatus.Int()  );
	
	switch( iStatus.Int() )
		{
		case EIMOperationUnreadMessage :
		    {
		    TRACE( T_LIT("CIMCacheAccessEventHandler::RunL() EIMOperationUnreadMessage") );
			GetBufferChatDataL();	            
		    break;
		    }
	    case EIMOperationUnreadChange :
	        {
	        // message data available get the data   
	        GetBufferedChatItemL(EIMCacheUnreadChange);  
	        break;
	        }
	    case EIMOperationChatStarted :
	        {
	        // message data available get the data   
	        GetBufferedChatItemL(EIMCacheChatStarted);  
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
		    iAccessHandler.HandleIMCacheEventL( EIMCacheAllChatClosed, NULL );
		    break;
		    }
       case EIMOperationUnRegistered:
            {
            TRACE(T_LIT(
                    "CIMCacheEventHandler::RunL() EIMOperationUnRegistered"));
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

	TRACE( T_LIT("CIMCacheAccessEventHandler::RunL() iContinueObserving iContinueObserving = %d") ,iContinueObserving);
		// Comment: cancel operation is changes as synchronous. iContinueObserving from event
	// can not really be trusted since complete code might be different if leave has occured 
	// for example. Cancel is handled here as generic cancel.
	// check need to continue observing to server
	if( iContinueObserving && KErrCancel != iStatus.Int() )
		{
		iRegistrar.RegisterObserverToServerL( iStatus ,EIMCacheObserveMessageAccessRegister );
		SetActive();
		}
	TRACE( T_LIT("CIMCacheAccessEventHandler::RunL() end") );
	}
// -----------------------------------------------------------------------------
// CIMCacheAccessEventHandler::DoCancel
// -----------------------------------------------------------------------------
//	
void CIMCacheAccessEventHandler::DoCancel()
	{
	TRACE( T_LIT("CIMCacheAccessEventHandler::DoCancel() ") );
		// Comment: set to EFalse here since we can not trust other changes. See
	// comments in RunL
	iContinueObserving = EFalse;
	if( IsActive() )
        {
        TRAP_IGNORE(iRegistrar.CancelRequestL( iStatus ,EIMCacheCancelRequest ));
        }
	}

// ---------------------------------------------------------
// CIMCacheAccessEventHandler::RunError()
// ---------------------------------------------------------
//
TInt CIMCacheAccessEventHandler::RunError( TInt /* aError */)
    {
    TRACE( T_LIT("CVIMPSTEngineCVListener::RunError() start"));
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CIMCacheAccessEventHandler::UnRegisterObserver
// -----------------------------------------------------------------------------
//		
void CIMCacheAccessEventHandler::UnRegisterObserver()
	{
	TRACE( T_LIT("CIMCacheAccessEventHandler::UnRegisterObserver() begin") );
	if( IsActive() )
		{
		TRACE( T_LIT("CIMCacheAccessEventHandler::UnRegisterObserver() active") );
		TRAP_IGNORE(iRegistrar.UnRegisterObserverToServerL( iStatus ,EIMCacheObserveMessageAccessUnRegister ));
		}
	TRACE( T_LIT("CIMCacheAccessEventHandler::UnRegisterObserver() end") );	
	}

// -----------------------------------------------------------------------------
// CIMCacheAccessEventHandler::GetBufferChatDataL
// -----------------------------------------------------------------------------
//
void CIMCacheAccessEventHandler::GetBufferChatDataL() 
    {
    TRACE( T_LIT("CIMCacheAccessEventHandler::GetBufferChatDataL() begin") );
   
    TBool more = ETrue;
    while(more)
        {
        TPtr8 bufferPtr = iRegistrar.GetBufferedDataL( EIMCacheGetBufferedPackets  );
        InternalizeChatDataL( bufferPtr , more );
        }
    TRACE( T_LIT("CIMCacheAccessEventHandler::GetBufferChatDataL() end") );
    }

// -----------------------------------------------------------------------------
// CIMCacheAccessEventHandler::InternalizeChatDataL
// -----------------------------------------------------------------------------     
void CIMCacheAccessEventHandler::InternalizeChatDataL( TPtr8 aChatDataBuffer , TBool& aMore )
    {
    TRACE( T_LIT("CIMCacheAccessEventHandler::InternalizeChatDataL() Start") );
    if( 0 == aChatDataBuffer.Length())
          {
          TRACE( T_LIT("CIMCacheAccessEventHandler::InternalizeChatItemL() aChatDataBuffer is zeor, hence returning ") );
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
		
		iAccessHandler.HandleIMCacheEventL(EIMCacheUnreadMessage , &chatData);
		
		CleanupStack::PopAndDestroy(4);//close read stream, delete mBuffer, text, buddy 
		}

	aMore = static_cast<TBool>( readAllStream.ReadInt16L() );

	CleanupStack::PopAndDestroy();//readAllStream.     
    
    TRACE( T_LIT("CIMCacheAccessEventHandler::InternalizeChatDataL() End") );
    }
  
// -----------------------------------------------------------------------------
// CIMCacheAccessEventHandler::GetBufferedChatItemL
// -----------------------------------------------------------------------------
//
void CIMCacheAccessEventHandler::GetBufferedChatItemL( TIMCacheEventType aEventType )
    {
    TRACE( T_LIT("CIMCacheAccessEventHandler::GetBufferedChatItemL() begin") );
    
    TPtr8 bufferPtr = iRegistrar.GetBufferedDataL( EIMCacheGetBufferedPackets );
    
    InternalizeChatItemL( aEventType, bufferPtr );
    
    TRACE( T_LIT("CIMCacheAccessEventHandler::GetBufferedChatItemL() end") );
    }
// -----------------------------------------------------------------------------
// CIMCacheAccessEventHandler::InternalizeChatItemL
// -----------------------------------------------------------------------------
//
void CIMCacheAccessEventHandler::InternalizeChatItemL( TIMCacheEventType aEventType, 
                                                 TPtr8 aContactBuffer )
    {
    TRACE( T_LIT("CIMCacheAccessEventHandler::InternalizeChatItemL() begin") );   
    if( 0 == aContactBuffer.Length())
           {
           TRACE( T_LIT("CIMCacheAccessEventHandler::InternalizeChatItemL() aChatDataBuffer is zeor, hence returning ") );
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
        
        iAccessHandler.HandleIMCacheEventL( aEventType , &chatItem );
      
        CleanupStack::PopAndDestroy(3);//buddyId , close read stream, delete mBuffer.      
                  
        }
    CleanupStack::PopAndDestroy();//readAllStream.    
    TRACE( T_LIT("CIMCacheAccessEventHandler::InternalizeChatItemL() End") );
    }

 
//  end of file

