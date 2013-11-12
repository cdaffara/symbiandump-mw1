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
* Description:  active helper for server ,owns the main cache arrray 
*
*/

 // INCLUDE FILES
#include "ccacheserveractivehelper.h"

#include "cimcachesession.h"
#include <imcachedefs.h>
#include "mimcachemessageheader.h"
#include "cimcacheserver.h"
#include "timcachemessagefactory.h"
#include "cimcachemessageheader.h"
#include "cimcachemessagebase.h"
#include <ecom/ecom.h>
 //logs
#include "imcachedebugtrace.h"
// ---------------------------------------------------------------------------
// CCacheServerActiveHelper::NewL()
// ---------------------------------------------------------------------------
//
CCacheServerActiveHelper* CCacheServerActiveHelper::NewL(CIMCacheServer& aServer)
    {
    TRACE( T_LIT( "CCacheServerActiveHelper::NewL begin") );
    CCacheServerActiveHelper* self = new(ELeave) CCacheServerActiveHelper(aServer);
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( self );
    TRACE( T_LIT( "CCacheServerActiveHelper::NewL end") );
    return self;
    }

// ---------------------------------------------------------------------------
// CCacheServerActiveHelper::~CCacheServerActiveHelper()
// ---------------------------------------------------------------------------
//
CCacheServerActiveHelper::~CCacheServerActiveHelper( )
    {
    TRACE( T_LIT( "CCacheServerActiveHelper::~CCacheServerActiveHelper begin") );
      
    iHeaderArray.ResetAndDestroy();  
    //all items are there in iHeaderArray so no need to call ResetAndDestroy
    iPluginInfo.ResetAndDestroy();
    iPluginInfo.Close();
    TRACE( T_LIT( "CCacheServerActiveHelper::~CCacheServerActiveHelper end") );
    }


// ---------------------------------------------------------------------------
// CCacheServerActiveHelper::CCacheServerActiveHelper()
// ---------------------------------------------------------------------------
//
CCacheServerActiveHelper::CCacheServerActiveHelper(CIMCacheServer& aServer ) 
                                                    :iServer(aServer )
                                                      
    {
    TRACE( T_LIT( "CCacheServerActiveHelper::CCacheServerActiveHelper begin") );
	TRACE( T_LIT( "CCacheServerActiveHelper::CCacheServerActiveHelper end") );
    }

// ---------------------------------------------------------------------------
// CCacheServerActiveHelper::ConstructL()
// ---------------------------------------------------------------------------
//
void CCacheServerActiveHelper::ConstructL()
    {
    TRACE( T_LIT( "CCacheServerActiveHelper::ConstructL begin") );
	iMessageFactory = new ( ELeave ) TIMCacheMessageFactory;
	LoadPluginL( );
	TRACE( T_LIT( "CCacheServerActiveHelper::ConstructL end") );
    }
 
// ---------------------------------------------------------------------------
// CCacheServerActiveHelper::ResetHeaderInoformation()
// ---------------------------------------------------------------------------
//
void CCacheServerActiveHelper::ResetHeaderInoformation()
                                                      
    {
    TRACE( T_LIT( "CCacheServerActiveHelper::ResetHeaderInoformation begin") );
    TInt count = iHeaderArray.Count() ;
    for ( TInt i= 0; i< count; i++ )
	    {
	    MIMCacheMessageHeader* header = iHeaderArray[ i ];
	    header->SetChatStarted(EFalse);
	    }
   	TRACE( T_LIT( "CCacheServerActiveHelper::ResetHeaderInoformation end") );
    }
// ---------------------------------------------------------
// CCacheServerActiveHelper::GetUnreadMessageCountL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCacheServerActiveHelper::GetUnreadMessageCountL(const RMessage2& aMessage )
	{
	TRACE( T_LIT( "CCacheServerActiveHelper::GetUnreadMessageCountL	start") );
		// at zero place service id was appended
	TInt sericeId = aMessage.Int0();
	
	// at index 1 sender was appended
	TInt bufferSize( aMessage.GetDesLength( 1 ) );
	HBufC* buddyId = HBufC::NewLC( bufferSize );
	TPtr buddyIdPtr = buddyId->Des();
	aMessage.ReadL( 1, buddyIdPtr );

	TInt headerIndex = iMessageFactory->FindHeaderIndexL(iHeaderArray, sericeId, buddyIdPtr );
	TInt unreadCount = 0;
	if( headerIndex != KErrNotFound ) //if match is found
		{
		MIMCacheMessageHeader* newHeader = iHeaderArray[ headerIndex ];
		
		unreadCount = newHeader->UnreadMessageCount();
	
		}
	TPckgC<TInt> pack( unreadCount );
   	aMessage.WriteL( 2, pack );
   		
    CleanupStack::PopAndDestroy(buddyId);
    TRACE( T_LIT( "CCacheServerActiveHelper::GetUnreadMessageCountL	end") );
	}
// ---------------------------------------------------------
// CCacheServerActiveHelper::GetAllUnreadMessageCountL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCacheServerActiveHelper::GetAllUnreadMessageCountL(const RMessage2& aMessage )
	{
	TRACE( T_LIT( "CCacheServerActiveHelper::GetAllUnreadMessageCountL	start") );
		// at zero place service id was appended
	TInt sericeId = aMessage.Int0();
	
	TInt unreadCount = 0;
	
	TInt headerCount = iHeaderArray.Count();
	
	for( TInt i=0; i< headerCount; i++ )
		{
		// note in this case reciepient is own user id 
		MIMCacheMessageHeader* header = iHeaderArray[i];
		if( header->ServiceId() == sericeId ) 	
			{
			unreadCount = unreadCount + header->UnreadMessageCount();	
			}
		}

	TPckgC<TInt> pack( unreadCount );
   	aMessage.WriteL( 1, pack );
   		
      
    TRACE( T_LIT( "CCacheServerActiveHelper::GetAllUnreadMessageCountL	end") );
	}
// ---------------------------------------------------------
// CCacheServerActiveHelper::StartNewConversationL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCacheServerActiveHelper::StartNewConversationL( const RMessage2& aMessage )
	{
	TRACE( T_LIT( "CCacheServerActiveHelper::StartNewConversationL	start") );
	// at zero place service id was appended
	TInt sericeId = aMessage.Int0();
	
	// at index 1 sender was appended
	TInt bufferSize( aMessage.GetDesLength( 1 ) );
	HBufC* buddyId = HBufC::NewLC( bufferSize );
	TPtr buddyIdPtr = buddyId->Des();
	aMessage.ReadL( 1, buddyIdPtr );
	
	TInt messageCount = 0;
	
	MIMCacheMessageHeader* chatHeader = ChatHeaderL( sericeId, buddyIdPtr );
	
	if( chatHeader == NULL ) //if match is not found
		{
		TRACE( T_LIT( "CCacheServerActiveHelper::StartNewConversationL	Fresh conversation") );
		chatHeader = iMessageFactory->CreateIMCacheMessageHeaderL(sericeId, buddyIdPtr );
		// insert always on top
		iHeaderArray.InsertL(chatHeader , 0);
		
		iActiveHeader = chatHeader;
		
		chatHeader->SetChatStarted(ETrue);
		
		PackAndNotifyEventL( EIMOperationChatStarted, sericeId, iActiveHeader , NULL );	
		}
	else  if( !chatHeader->IsChatStarted() ) //if match is found
		{
		TRACE( T_LIT( "CCacheServerActiveHelper::StartNewConversationL	existing conversation") );
		
		chatHeader->SetChatStarted(ETrue);
		
		iActiveHeader = chatHeader ;
		
		iActiveHeader->MarkAllRead();
		
		messageCount = iActiveHeader->MessageCount();

		if( messageCount ) 
			{
			PackAndNotifyEventL(EIMOperationFetchAll, sericeId, iActiveHeader, NULL );
			// still fetching is not completed, until EIMOperationCompleted 
			}
		TRACE( T_LIT( "CCacheServerActiveHelper::StartNewConversationL start fetching") );
		}
	else
		{
		iActiveHeader = chatHeader;
		
		iActiveHeader->MarkAllRead();
		
		PackAndNotifyEventL(EIMOperationUnreadChange, sericeId, iActiveHeader , NULL );
		// once request is completed, mark as fetch completed
		}
	// if there are no unread messages then reset the status pane indicator.
	TInt unreadCount = 0;
    TInt headerCount = iHeaderArray.Count();
    for( TInt i=0; i< headerCount; i++ )
        {
        // note in this case reciepient is own user id 
        MIMCacheMessageHeader* header = iHeaderArray[i];
        if( header->ServiceId() == sericeId )   
            {
            unreadCount = unreadCount + header->UnreadMessageCount();   
            }
        }
    if(!unreadCount)
        {
        PublishMessageInfoL(*buddyId,sericeId);
        }
    
	CleanupStack::PopAndDestroy(buddyId);
	
    TRACE( T_LIT( "CCacheServerActiveHelper::StartNewConversationL	end") );
	}


// ---------------------------------------------------------
// CCacheServerActiveHelper::AppendReceiveMessageL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCacheServerActiveHelper::AppendReceiveMessageL( const RMessage2 &aMessage )
	{
	TRACE( T_LIT( "CCacheServerActiveHelper::AppendReceiveMessageL	start") );
	// at zero pace service id was apended
	TInt sericeId = aMessage.Int0();
	
	// at index 1 sender was appended
	TInt bufferSize( aMessage.GetDesLength( 1 ) );
	HBufC* buddyId = HBufC::NewLC( bufferSize );
	TPtr buddyIdPtr = buddyId->Des();
	aMessage.ReadL( 1, buddyIdPtr );
	
	// at index 2 text was appended
	bufferSize = aMessage.GetDesLength( 2 ) ;
	HBufC* text = HBufC::NewLC( bufferSize );
	TPtr textPtr = text->Des();
	aMessage.ReadL( 2, textPtr );
	
	TRACE( T_LIT( "CCacheServerActiveHelper::AppendReceiveMessageL Msg %S"), &textPtr );
	
	MIMCacheMessageHeader* chatHeader = ChatHeaderL( sericeId, buddyIdPtr );
	
	CIMCacheMessageBase* newmessage = NULL;
	      
	if( chatHeader == NULL ) //if match is not found
		{
		TRACE( T_LIT( "CCacheServerActiveHelper::AppendReceiveMessageL	Fresh conversation") );
		chatHeader = iMessageFactory->CreateIMCacheMessageHeaderL(sericeId, buddyIdPtr);
		// insert always on top
		iHeaderArray.InsertL(chatHeader , 0);
		
		newmessage = iMessageFactory->CreateCacheMessageL( textPtr );
		newmessage->SetMessageType( EIMCMessagePTOP );
		newmessage->SetMessagerType( EIMCMessageReceived );
		
		chatHeader->AppendMessageL(newmessage);
		
		PublishMessageInfoL(*buddyId,sericeId);
		
		// no notification so for this is for servicetab
		// here need to notify for all session
		// fresh conversation started
				
		PackAndNotifyEventL( EIMOperationChatAdded,	sericeId, chatHeader ,newmessage );		
	
		}
	else  // chat found
		{
		TRACE( T_LIT( "CCacheServerActiveHelper::AppendReceiveMessageL	existing conversation") );
		// this is called from network side 
		// here we dont know about ui client wheather this active conversation is active or not. 
			
		newmessage = iMessageFactory->CreateCacheMessageL( textPtr );
		newmessage->SetMessageType( EIMCMessagePTOP );
		newmessage->SetMessagerType( EIMCMessageReceived );
		chatHeader->AppendMessageL(newmessage);
	
		// notify all registeed session about this changed
		// this has happened on active conversation
				
		if( chatHeader == iActiveHeader )
			{
			//cv tab is active notify it.
			PackAndNotifyEventL( EIMOperationMessageAdded, sericeId, chatHeader, newmessage );
			}
		else 
			{
			PublishMessageInfoL(*buddyId,sericeId);
			//cv tab is not active notify it.
			PackAndNotifyEventL( EIMOperationUnreadMessage, sericeId, chatHeader , newmessage );	
			}
		}	
	CleanupStack::PopAndDestroy(2);//text,buddyId
	//	else TODO mark unread messagesinto buffer
	TRACE( T_LIT( "CCacheServerActiveHelper::AppendReceiveMessageL	end") );
	}
// ---------------------------------------------------------
// CCacheServerActiveHelper::AppendSendMessageL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCacheServerActiveHelper::AppendSendMessageL( const RMessage2 &aMessage )
	{
	TRACE( T_LIT( "CCacheServerActiveHelper::AppendSendMessageL	start") );
	
	// at index 0 text was appended
	TInt bufferSize = aMessage.GetDesLength( 0 ) ;
	HBufC* text = HBufC::NewLC( bufferSize );
	TPtr textPtr = text->Des();
	aMessage.ReadL( 0, textPtr );
	
	TRACE( T_LIT( "CCacheServerActiveHelper::AppendSendMessageL Msg %S"), &textPtr );
	
	if( iActiveHeader )
		{
		CIMCacheMessageBase* message = iMessageFactory->CreateCacheMessageL( textPtr );
		message->SetMessageType( EIMCMessagePTOP );
		message->SetMessagerType( EIMCMessageSent );
		message->SetRead();	
		
		iActiveHeader->AppendMessageL(message);
		}
	CleanupStack::PopAndDestroy(text); // text
	
	TRACE( T_LIT( "CCacheServerActiveHelper::AppendSendMessageL	end") );
	}

// ---------------------------------------------------------
// CCacheServerActiveHelper::AppendMessageL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCacheServerActiveHelper::AppendMessageL( const RMessage2 &aMessage )
	{
	TRACE( T_LIT( "CCacheServerActiveHelper::AppendMessageL	start") );
	// this is get called from active conversation only.
	// this append the normal information message , 
	// please note, this is not sent/recieved message
	// at 0 place text was appended
	// this is get called from conversation only system emssage
	// will get addded when buddy presence changes.
	TInt sericeId = aMessage.Int0();
	
	// at index 1 sender was appended
	TInt bufferSize( aMessage.GetDesLength( 1 ) );
	HBufC* buddyId = HBufC::NewLC( bufferSize );
	TPtr buddyIdPtr = buddyId->Des();
	aMessage.ReadL( 1, buddyIdPtr );
	
	// at index 2 text was appended
	bufferSize = aMessage.GetDesLength( 2 ) ;
	HBufC* text = HBufC::NewLC( bufferSize );
	TPtr textPtr = text->Des();
	aMessage.ReadL( 2, textPtr );
	
	TRACE( T_LIT( "CCacheServerActiveHelper::AppendSendMessageL Msg %S"), &textPtr );
	
	MIMCacheMessageHeader* chatHeader = ChatHeaderL( sericeId, buddyIdPtr );
	
	if( chatHeader != NULL )
		{
		CIMCacheMessageBase* message = iMessageFactory->CreateCacheMessageL( textPtr );
		message->SetMessageType( EIMCMessageSystem );
		message->SetMessagerType( EIMCMessageOther );
		message->SetRead();		
		chatHeader->AppendMessageL(message);
		}
	CleanupStack::PopAndDestroy(2); //buddyId, text
	
	TRACE( T_LIT( "CCacheServerActiveHelper::AppendMessageL	end") );
	}
// ---------------------------------------------------------
// CCacheServerActiveHelper::CloseConversationL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCacheServerActiveHelper::CloseConversationL( const RMessage2 &aMessage )
    {
    TRACE( T_LIT( "CCacheServerActiveHelper::CloseConversationL  start") );
    // at zero pace service id was apended
    TInt sericeId = aMessage.Int0();
    
    // at index 1 sender was appended
    TInt bufferSize( aMessage.GetDesLength( 1 ) );
    HBufC* buddyId = HBufC::NewLC( bufferSize );
    TPtr buddyIdPtr = buddyId->Des();
    aMessage.ReadL( 1, buddyIdPtr );

    TInt headerIndex = iMessageFactory->FindHeaderIndexL(iHeaderArray, sericeId, buddyIdPtr ) ;
       
    if( headerIndex != KErrNotFound ) //if match is found
        {
        MIMCacheMessageHeader* msgHeader = iHeaderArray[headerIndex];
        iHeaderArray.Remove(headerIndex);
        if(msgHeader == iActiveHeader)
            {
            iActiveHeader = NULL;
            }
       
		PackAndNotifyEventL( EIMOperationChatDeleted, sericeId, msgHeader, NULL ); 
		 
        delete msgHeader;
        iHeaderArray.Compress();  
       		      
        }  
     CleanupStack::PopAndDestroy(buddyId); // buddyId
     
    TRACE( T_LIT( "CCacheServerActiveHelper::CloseConversationL  end") );
    }
// ---------------------------------------------------------
// CCacheServerActiveHelper::CloseAllConversationL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCacheServerActiveHelper::CloseAllConversationL( TInt aServiceId )
    {
    TRACE( T_LIT( "CCacheServerActiveHelper::CloseAllConversationL  start") );
    TBool needToNotify = EFalse;
    for( TInt index=0; index<iHeaderArray.Count();index++ )
        {
        MIMCacheMessageHeader* msgHeader = iHeaderArray[index];
        if( msgHeader->ServiceId() == aServiceId )
            {
            iHeaderArray.Remove(index);
            if(msgHeader == iActiveHeader)
                {
                iActiveHeader = NULL;
                }
            delete msgHeader;
            msgHeader = NULL;
            iHeaderArray.Compress();
            index = index-1;// start from same index again
            needToNotify = ETrue;
            }
        }
    if( needToNotify )
        {
        PackAndNotifyEventL( EIMOperationAllChatDeleted, aServiceId, NULL, NULL ); 
        }
    TRACE( T_LIT( "CCacheServerActiveHelper::CloseAllConversationL  End") );
    }
// ---------------------------------------------------------
// CCacheServerActiveHelper::DeactivateConversationL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCacheServerActiveHelper::DeactivateConversationL()
	{
	TRACE( T_LIT( "CCacheServerActiveHelper::DeactivateConversationL	start") );
	// this is get called from active conversation only.
	if( iActiveHeader )
		{
		iActiveHeader->MarkAllRead();	
		}
	iActiveHeader = NULL;
	TRACE( T_LIT( "CCacheServerActiveHelper::DeactivateConversationL	end") );
	}

// ---------------------------------------------------------
// CCacheServerActiveHelper::CheckConversationExistL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCacheServerActiveHelper::CheckConversationExistL( const RMessage2& aMessage )
	{
	TRACE( T_LIT( "CCacheServerActiveHelper::CheckConversationExistL	start") );
	// at zero place service id was appended
	TInt sericeId = aMessage.Int0();
	
	// at index 1 sender was appended
	TInt bufferSize( aMessage.GetDesLength( 1 ) );
	HBufC* buddyId = HBufC::NewLC( bufferSize );
	TPtr buddyIdPtr = buddyId->Des();
	aMessage.ReadL( 1, buddyIdPtr );
	
	TInt headerIndex = iMessageFactory->FindHeaderIndexL(iHeaderArray, sericeId, buddyIdPtr ) ;
	
	TBool conversationExist = EFalse;
	
	if( headerIndex != KErrNotFound ) //if match is found
		{
	    conversationExist = ETrue;
		}
	TPckgC<TBool> pack( conversationExist );
   	aMessage.WriteL(2, pack );
    		
	CleanupStack::PopAndDestroy(buddyId); // buddyId
    
    TRACE( T_LIT( "CCacheServerActiveHelper::CheckConversationExistL	end") );
	}

// ---------------------------------------------------------
// CCacheServerActiveHelper::PackAndNotifyEventL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCacheServerActiveHelper::PackAndNotifyEventL( TIMCacheOperationsCode aNotification, 
                                                    TInt aServiceId,
												    MIMCacheMessageHeader* aChatHeader /*=NULL*/, 
												    CIMCacheMessageBase* aMessage /*=NULL*/)	
	{
	TRACE( T_LIT( "CCacheServerActiveHelper::PackAndNotifyEventL	start") );
	
	iServer.PackAndNotifyEventL( aNotification, aServiceId, aChatHeader, aMessage );
	 
	TRACE( T_LIT( "CCacheServerActiveHelper::PackAndNotifyEventL	end") );
	}
	
   
// ---------------------------------------------------------
// CCacheServerActiveHelper::GetServiceChatHeaderArrayL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCacheServerActiveHelper::GetServiceChatHeaderArrayL(TInt aServiceId , RPointerArray<MIMCacheMessageHeader>& aServiceHeaderArray)
	{
	TRACE( T_LIT( "CCacheServerActiveHelper::GetServiceChatHeaderArrayL	start") );		
	
   
    if( -1 == aServiceId )
        {
        aServiceHeaderArray = iHeaderArray;        
        }
    else
        {
        for( TInt i= 0 ; i < iHeaderArray.Count() ; i++ )
            {
            MIMCacheMessageHeader* header = iHeaderArray[i];
            if( header->ServiceId() == aServiceId )
                {
                aServiceHeaderArray.AppendL(header);
                }
            }        
        }
    TRACE( T_LIT( "CCacheServerActiveHelper::GetServiceChatHeaderArrayL    end") );
	}

// ---------------------------------------------------------
// CCacheServerActiveHelper::ChatHeaderL
// (other items were commented in a header).
// ---------------------------------------------------------
MIMCacheMessageHeader* CCacheServerActiveHelper::ChatHeaderL(const TInt& aServiceId, const TDesC& aBuddyId )
    {
    MIMCacheMessageHeader* header = NULL;
    TInt headerCount = iHeaderArray.Count();
    
    for( TInt index = 0 ; index < headerCount ; index++ )
        {
        header = iHeaderArray[index];
        if( header )
            {
            if( ( aServiceId == header->ServiceId() ) && ( aBuddyId.CompareC( header->BuddyId() ) == 0 ) )  
                {
                break;   
                }                
            }
        header = NULL;
        }
    return header;
    }
// -----------------------------------------------------------------------------
// CCacheServerActiveHelper::ChatHeaderAt
// name of history data
// -----------------------------------------------------------------------------
//
MIMCacheMessageHeader* CCacheServerActiveHelper::ChatHeaderAt( TInt aIndex ) 
	{
	if( aIndex >= ChatHeadersCount() )
		{
		return NULL;	
		}
	return iHeaderArray[aIndex];
	}
	
// -----------------------------------------------------------------------------
// CCacheServerActiveHelper::ChatHeadersCount
// name of history data
// -----------------------------------------------------------------------------
//
TInt CCacheServerActiveHelper::ChatHeadersCount() 
	{
	return iHeaderArray.Count();
	}


// -----------------------------------------------------------------------------
// CCacheServerActiveHelper::PublishMessageInfoL()
// name of history data
// -----------------------------------------------------------------------------
//
void  CCacheServerActiveHelper::PublishMessageInfoL(const TDesC& aSenderId,TInt aServiceId )
    {
    // set/reset  the status pane indicator here
    // based on the unread count.
    // as we need to show the indicator only for the received messages
    TInt unreadCount = 0;
    TInt multipleSender = 0;
    TInt headerCount = iHeaderArray.Count();
    TRACE(T_LIT( "CCacheServerActiveHelper::PublishMessageInfoL headerCount -- %d"), headerCount);
    for( TInt i=0; i< headerCount; i++ )
        {
        // note in this case reciepient is own user id 
        MIMCacheMessageHeader* header = iHeaderArray[i];
        TRACE(T_LIT( "CCacheServerActiveHelper::PublishMessageInfoL UnreadMessageCount -- %d"), header->UnreadMessageCount());
        if( header->ServiceId() == aServiceId  && header->UnreadMessageCount() )   
            {
            unreadCount = unreadCount + header->UnreadMessageCount();  
	        multipleSender ++; 
            }
        }
    TRACE(T_LIT( "CCacheServerActiveHelper::PublishMessageInfoL   multipleSender -- %d"), multipleSender);
    // Get the number of  plugins
    const TInt count = iPluginInfo.Count();
    // Loop though all the command handlers, If DynInitMenuPaneL leaves for one plugin, a plugin
    // error message will be displayed and the loop will continue with
    // the next command handler. If none of the plugins leave, there will
    // be only one TRAP used.
    for ( TInt index = 0; index < count; ++index )
        {
        //trap is required if one plugin leaves then it should continue with other plugins.
        TRAP_IGNORE(iPluginInfo[index]->Plugin().MessageInfoL(unreadCount,aServiceId,aSenderId,multipleSender));
        }
    }
 // -----------------------------------------------------------------------------
 // CCacheServerActiveHelper::LoadPluginL
 // -----------------------------------------------------------------------------
 //
 void CCacheServerActiveHelper::LoadPluginL( )
     {
     // plugininfo array, Owned
     RImplInfoPtrArray pluginInfo;
     iPluginInfo.ResetAndDestroy();
     REComSession::ListImplementationsL( KINSTANTMSGINTERFACEUID,pluginInfo );
     TInt pluginCount = pluginInfo.Count();
     for ( TInt pluginIndex(0); pluginIndex < pluginCount; ++pluginIndex )
         {
         TUid pluginUid = pluginInfo[pluginIndex]->ImplementationUid();
         // Creates the plugin and transfers ownership of the services
         // object to the plugin.
         CInstantMsgIndicator* plugin = NULL;
         TRAPD(error, plugin = CInstantMsgIndicator::CreateImplementationL( pluginUid ));
         
         if(KErrNone != error)
             {
             // handle the error here.
             // error loading the plugin
             }
         else
             {
             // plugin loaded sucessfully 
             //if its here its sure that plugin is not null;
             //add the plugin to the array
             CPluginInfo* newPlugin = new ( ELeave ) CCacheServerActiveHelper::CPluginInfo( plugin, pluginUid);    
             CleanupStack::PushL( newPlugin );
             iPluginInfo.AppendL( newPlugin );
             CleanupStack::Pop( newPlugin );
            
            }
         
         }
     pluginInfo.ResetAndDestroy();
     pluginInfo.Close();
     }
 // ----------------------------------------------------------------------------
 // CCacheServerActiveHelper::CPluginInfo::CPluginInfo(
 // C++ constructor
 // (other items were commented in a header).
 // ----------------------------------------------------------------------------
 //
 CCacheServerActiveHelper::CPluginInfo::CPluginInfo( 
         CInstantMsgIndicator* aPlugin,
                                     TUid aPluginUid )
     : iPlugin( aPlugin ),
       iPluginUid( aPluginUid )
     {

     }
 // ----------------------------------------------------------------------------
 // CCacheServerActiveHelper::CPluginInfo::~CPluginInfo(
 // C++ destructor
 // (other items were commented in a header).
 // ----------------------------------------------------------------------------
 //
 CCacheServerActiveHelper::CPluginInfo::~CPluginInfo() 
     {
      delete iPlugin;
     }
 // -----------------------------------------------------------------------------
 // CCacheServerActiveHelper::LoadPluginL
 // -----------------------------------------------------------------------------
 //
 CInstantMsgIndicator& CCacheServerActiveHelper::CPluginInfo::Plugin()
     {
      return *iPlugin;
     }   
/// end of file
