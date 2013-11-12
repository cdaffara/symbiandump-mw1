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
* Description:  write right class implemetation
*
*/

//  INCLUDE FILES
#include "cimcacheupdater.h"

#include <imcachedefs.h>
// logs
#include "imcachedebugtrace.h"

#include <e32base.h>
#include <s32mem.h>

//Received message maximum length is 400 character if received message is 
//longer than 400 character truncate it to 400 character
//This must always be in sync with imcvuiapp KReceiveMsgMaxLength
const TInt KReceiveMsgMaxLength = 400; 
_LIT(KPercentage, "%");
// -----------------------------------------------------------------------------
// CIMCacheUpdater::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMCacheUpdater* CIMCacheUpdater::NewL(TInt aServiceId,
											const TDesC& aSenderId ,TBool aRegistrationNeeded )
    {
    TRACE( T_LIT("CIMCacheUpdater::NewL begin") );
    CIMCacheUpdater* self = new ( ELeave ) CIMCacheUpdater( aServiceId ) ;
    CleanupStack::PushL( self );
    self->ConstructL( aSenderId, aRegistrationNeeded );
    CleanupStack::Pop( self );  //self
    TRACE( T_LIT("CIMCacheUpdater::NewL end") );
    return self;
    }
// -----------------------------------------------------------------------------
// CIMCacheUpdater::ConstructL()
// Symbian OS default constructor can leave.
// -----------------------------------------------------------------------------
//
void CIMCacheUpdater::ConstructL( const TDesC& aSenderId ,TBool aRegistrationNeeded )
    {
    TRACE( T_LIT("CIMCacheUpdater::ConstructL begin") );
    iUserId = aSenderId.AllocL();
    User::LeaveIfError( iClient.Connect() );
    EIMCacheOperations opration = EIMCacheInitUpdate;
    if( aRegistrationNeeded )
	    {
	    opration = EIMCacheInitConvesation;	
	    }
    iClient.StartTransactionL(opration, iServiceId, *iUserId );
	TRACE( T_LIT("CIMCacheUpdater::ConstructL end") );
    }
// -----------------------------------------------------------------------------
// CIMCacheUpdater::~CIMCacheUpdater()
// Destructor
// -----------------------------------------------------------------------------
//
CIMCacheUpdater::~CIMCacheUpdater()
    {
    TRACE( T_LIT("CIMCacheUpdater::~CIMCacheUpdater begin") );
    delete iUserId;
    //do not close the client session here. since there is also accessor class which is closing the session
    //moved this part of code which is common to both udpater and Accessor i.e, client ~CIMCacheClient()
    //iClient.Close();
    TRACE( T_LIT("CIMCacheUpdater::~CIMCacheUpdater end") );
    }
// -----------------------------------------------------------------------------
// CIMCacheUpdater::CIMCacheUpdater()
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIMCacheUpdater::CIMCacheUpdater(TInt aServiceId )
	:iServiceId( aServiceId )
	{
	TRACE( T_LIT("CIMCacheUpdater::CIMCacheUpdater ") );
	}


// -----------------------------------------------------------------------------
// CIMCacheUpdater::StartNewConversationL()
// -----------------------------------------------------------------------------
//
void CIMCacheUpdater::StartNewConversationL(const TDesC& aBuddyId )
	{
	TRACE( T_LIT("CIMCacheUpdater::StartNewConversationL begin") );
	if( !aBuddyId.Length() )
		{
		User::Leave( KErrArgument )	;
		}
	iClient.StartTransactionL(EIMCacheStartNewConversation, iServiceId , aBuddyId  );
	
	TRACE( T_LIT("CIMCacheUpdater::StartNewConversationL end") );
	}
	
// -----------------------------------------------------------------------------
// CIMCacheUpdater::AppendReceiveMessageL()
// -----------------------------------------------------------------------------
//
void CIMCacheUpdater::AppendReceiveMessageL( const TDesC& aBuddyId, 
											 const TDesC& aText )
    {
    TRACE( T_LIT("CIMCacheUpdater::AppendReceiveMessageL begin") );	
    if( !aBuddyId.Length() )
        {
        User::Leave( KErrArgument )	;
        }
    TInt len = aText.Length();
    HBufC* bigmsg = NULL;
    bigmsg = HBufC::NewLC(KReceiveMsgMaxLength + 1) ; // 1 for  %
    if( bigmsg )
        {
        TRACE( T_LIT("Inside if( bigmsg )") );
        TPtr bigmsgPtr = bigmsg->Des();
        //If message is more than 400 character take only first 400 character
        //rest of the message will be lost
        bigmsgPtr.Append( aText.Left( KReceiveMsgMaxLength ) ); 
        if( len > KReceiveMsgMaxLength)
            {
            //append % as 401st character to identify on UI that it is a 
            //long message which is truncated        
            bigmsgPtr.Append(KPercentage); 
            }

        iClient.StartTransactionL(EIMCacheAppendReceiveMessage, iServiceId, aBuddyId , bigmsgPtr );

        CleanupStack::PopAndDestroy(bigmsg); // bigmsg
        }
	TRACE( T_LIT("CIMCacheUpdater::AppendReceiveMessageL end") );
	}
// -----------------------------------------------------------------------------
// CIMCacheUpdater::AppendSendMessageL()
// -----------------------------------------------------------------------------
//
void CIMCacheUpdater::AppendSendMessageL(const TDesC& aText )
	{
  	TRACE( T_LIT("CIMCacheUpdater::AppendSendMessageL begin") );
  	
	iClient.StartTransactionL( EIMCacheAppendSendMessage , aText);	
	TRACE( T_LIT("CIMCacheUpdater::AppendSendMessageL end") );
	}

// -----------------------------------------------------------------------------
// CIMCacheUpdater::AppendMessageL()
// -----------------------------------------------------------------------------
//
void CIMCacheUpdater::AppendMessageL(const TDesC& aBuddyId, const TDesC& aText )
	{
  	TRACE( T_LIT("CIMCacheUpdater::AppendMessageL begin") );
	if( !aBuddyId.Length() )
		{
		User::Leave( KErrArgument )	;
		}
	iClient.StartTransactionL(EIMCacheAppendMessage, iServiceId, aBuddyId, aText);	
	TRACE( T_LIT("CIMCacheUpdater::AppendMessageL end") );
	}

// -----------------------------------------------------------------------------
// CIMCacheUpdater::CloseConversation()
// -----------------------------------------------------------------------------
//
void CIMCacheUpdater::CloseConversationL( const TDesC& aBuddyId )
    {
    TRACE( T_LIT("CIMCacheUpdater::CloseConversationL begin") ); 
    
	if( !aBuddyId.Length() )
		{
		User::Leave( KErrArgument )	;
		}
    iClient.StartTransactionL( EIMCacheCloseConversation,
								iServiceId, 
								aBuddyId                                
								);
    TRACE( T_LIT("CIMCacheUpdater::CloseConversationL end") );
   }

// -----------------------------------------------------------------------------
// CIMCacheUpdater::CloseConversation()
// -----------------------------------------------------------------------------
//
void CIMCacheUpdater::CloseAllConversationL()
    {
    TRACE( T_LIT("CIMCacheUpdater::CloseAllConversationL begin") ); 
    iClient.StartTransactionL( EIMCacheCloseAllConversation );
    TRACE( T_LIT("CIMCacheUpdater::CloseAllConversationL end") );
    }
// -----------------------------------------------------------------------------
// CIMCacheUpdater::DeactivateConversationL()
// -----------------------------------------------------------------------------
//
TInt CIMCacheUpdater::DeactivateConversationL( )
	{
	TRACE( T_LIT("CIMCacheUpdater::DeactivateConversationL begin") );
	
	TInt error = iClient.StartTransactionL( EIMCacheDeactivateConversation );
								 
	TRACE( T_LIT("CIMCacheUpdater::DeactivateAllConversationsL end") );
	return error;
	}
// -----------------------------------------------------------------------------
// CIMCacheUpdater::ServiceId()
// -----------------------------------------------------------------------------
//
TInt CIMCacheUpdater::ServiceId() const 
	{
	return iServiceId;	
	}

// -----------------------------------------------------------------------------
// CIMCacheAccess::RegisterObserverL()
// -----------------------------------------------------------------------------
//
void CIMCacheUpdater::RegisterObserverL( MIMCacheEventHandler& aObserver  )
	{
	TRACE( T_LIT("CIMCacheAccess::RegisterObserverL begin") );
	// update case ETrue says CV is active
	// cch need not to register
	iClient.RegisterUpdateObserverL( aObserver  );
	TRACE( T_LIT("CIMCacheAccess::RegisterObserverL end") );
	}

// -----------------------------------------------------------------------------
// CIMCacheAccess::UnRegisterObserverL()
// -----------------------------------------------------------------------------
//
void CIMCacheUpdater::UnRegisterObserver( MIMCacheEventHandler& aObserver )
	{
	TRACE( T_LIT("CIMCacheAccess::UnRegisterObserverL begin") );
	iClient.UnRegisterUpdateObserver( aObserver );
	TRACE( T_LIT("CIMCacheAccess::UnRegisterObserverL end") );
	}	
  
// -----------------------------------------------------------------------------
// CIMCacheAccess::GetChatListL()
// -----------------------------------------------------------------------------
//
RArray<SIMCacheChatItem> CIMCacheUpdater::GetChatListL( const TInt aServiceId  )
	{
	TRACE( T_LIT("CIMCacheUpdater::GetChatListL") );	
	RArray<SIMCacheChatItem> chatListArray;//array to hold chat item
	TBool more = EFalse;
	TPtr8 bufferPtr =  iClient.GetChatListL( aServiceId );
	InternalizeChatListL( chatListArray, bufferPtr, more );
	
	while( more)
		{
		bufferPtr = iClient.GetBufferedDataL( EIMCacheGetBufferedPackets );
		InternalizeChatListL( chatListArray, bufferPtr, more );
		}
	return chatListArray;
	}
    
// -----------------------------------------------------------------------------
// CIMCacheUpdater::InternalizeChatListL
// -----------------------------------------------------------------------------
//
void CIMCacheUpdater::InternalizeChatListL(RArray<SIMCacheChatItem>& aChatArray, TPtr8 aChatListBuffer ,TBool& aMore )
    {
    TRACE( T_LIT("CIMCacheUpdater::InternalizeChatListL() begin") );   
    
    RDesReadStream readAllStream ;
    RDesReadStream readStream ;    
    
    readAllStream.Open( aChatListBuffer );
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
        CleanupStack::Pop(buddyId); // ownership to structure variable
        aChatArray.Insert(chatItem, 0);
        CleanupStack::PopAndDestroy(2);//close read stream, delete mBuffer.                
        }
    aMore = static_cast<TBool>( readAllStream.ReadInt16L() );
    
    CleanupStack::PopAndDestroy();//readAllStream.    
    
    TRACE( T_LIT("CIMCacheUpdater::InternalizeChatListL() End") );
    }

//  END OF FILE
