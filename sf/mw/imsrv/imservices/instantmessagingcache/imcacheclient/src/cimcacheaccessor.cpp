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
* Description:  access right class implemetation
*
*/


//  INCLUDE FILES
#include "cimcacheaccessor.h"
#include <imcachedefs.h>
#include <s32mem.h>
// logs
#include "imcachedebugtrace.h"

// -----------------------------------------------------------------------------
// CIMCacheAccessor::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMCacheAccessor* CIMCacheAccessor::NewL(TInt aServiceId,
											const TDesC& aSenderId  )
    {
    TRACE( T_LIT("CIMCacheAccessor::NewL begin") );
    CIMCacheAccessor* self = new ( ELeave ) CIMCacheAccessor( aServiceId ) ;
    CleanupStack::PushL( self );
    self->ConstructL( aSenderId );
    CleanupStack::Pop( self );  //self
    TRACE( T_LIT("CIMCacheAccessor::NewL end") );
    return self;
    }
// -----------------------------------------------------------------------------
// CIMCacheAccessor::ConstructL()
// Symbian OS default constructor can leave.
// -----------------------------------------------------------------------------
//
void CIMCacheAccessor::ConstructL( const TDesC& aSenderId  )
    {
    TRACE( T_LIT("CIMCacheAccessor::ConstructL begin") );
    iUserId = aSenderId.AllocL();
    User::LeaveIfError( iClient.Connect() );
    iClient.StartTransactionL( EIMCacheInitAccess,
    							iServiceId,
    						    *iUserId
								 );
								 
    TRACE( T_LIT("CIMCacheAccessor::ConstructL end") );
    }
// -----------------------------------------------------------------------------
// CIMCacheAccessor::~CIMCacheAccessor()
// Destructor
// -----------------------------------------------------------------------------
//
CIMCacheAccessor::~CIMCacheAccessor()
    {
    TRACE( T_LIT("CIMCacheAccessor::~CIMCacheAccessor begin") );
    delete iUserId;
    //do not close the client session here. since there is also updater class which is closing the session
    //moved this part of code which is common to both udpater and Accessor i.e, client ~CIMCacheClient()
    //iClient.Close();    
    TRACE( T_LIT("CIMCacheAccessor::~CIMCacheAccessor end") );
    }
// -----------------------------------------------------------------------------
// CIMCacheAccessor::CIMCacheAccessor()
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIMCacheAccessor::CIMCacheAccessor(TInt aServiceId )
	:iServiceId( aServiceId )
	{
	TRACE( T_LIT("CIMCacheAccessor::CIMCacheAccessor ") );
	}

// -----------------------------------------------------------------------------
// CIMCacheAccessor::ServiceId()
// -----------------------------------------------------------------------------
//
TInt CIMCacheAccessor::ServiceId() const 
	{
	return iServiceId;	
	}

// -----------------------------------------------------------------------------
// CIMCacheAccess::RegisterObserverL()
// -----------------------------------------------------------------------------
//
void CIMCacheAccessor::RegisterObserverL( MIMCacheEventHandler& aObserver  )
	{
	TRACE( T_LIT("CIMCacheAccess::RegisterObserverL begin") );
	// update case ETrue says CV is active
	// cch need not to register
	iClient.RegisterAccessObserverL( aObserver  );
	TRACE( T_LIT("CIMCacheAccess::RegisterObserverL end") );
	}

// -----------------------------------------------------------------------------
// CIMCacheAccess::UnRegisterObserverL()
// -----------------------------------------------------------------------------
//
void CIMCacheAccessor::UnRegisterObserver( MIMCacheEventHandler& aObserver )
	{
	TRACE( T_LIT("CIMCacheAccess::UnRegisterObserverL begin") );
	iClient.UnRegisterAccessObserver( aObserver );
	TRACE( T_LIT("CIMCacheAccess::UnRegisterObserverL end") );
	}
// -----------------------------------------------------------------------------
// CIMCacheAccess::GetUnreadMessageCountL()
// -----------------------------------------------------------------------------
//
TInt CIMCacheAccessor::GetUnreadMessageCountL(const TDesC& aBuddyId)
	{
	TRACE( T_LIT("CIMCacheAccess::GetUnreadMessageCount") );
	if( !aBuddyId.Length() )
		{
		User::Leave( KErrArgument )	;
		}
	return iClient.GetInformationL(EIMCacheGetUnreadCount, iServiceId,aBuddyId );
	}
//-----------------------------------------------------------------------------
// CIMCacheAccessor::IsConversationExistL
// ( Other items commented in header )
//-----------------------------------------------------------------------------    
TBool CIMCacheAccessor::IsConversationExistL( const TDesC& aBuddyId ) 
	{
	TRACE( T_LIT("CIMCacheAccess::IsConversationExistL Start ") );
	if( !aBuddyId.Length() )
		{
		User::Leave( KErrArgument )	;
		}
	TBool ret = iClient.GetInformationL(EIMCacheIsConversationExist,iServiceId, aBuddyId );
	TRACE( T_LIT("CIMCacheAccess::IsConversationExistL End ") );
	return ret;  
  	}
  
  // -----------------------------------------------------------------------------
// CIMCacheAccess::GetAllUnreadMessageCountL()
// -----------------------------------------------------------------------------
//
TInt CIMCacheAccessor::GetAllUnreadMessageCountL()
	{
	TRACE( T_LIT("CIMCacheAccess::GetAllUnreadMessageCountL") );
	return iClient.GetInformationL( EIMCacheGetAllUnreadCount,iServiceId );
	}	

	
// -----------------------------------------------------------------------------
// CIMCacheAccessor::CloseConversationL()
// -----------------------------------------------------------------------------
//
void CIMCacheAccessor::CloseConversationL( const TDesC& aBuddyId )
    {
    TRACE( T_LIT("CIMCacheAccessor::CloseConversationL begin") ); 
    if( !aBuddyId.Length() )
		{
		User::Leave( KErrArgument )	;
		}
    iClient.StartTransactionL( EIMCacheCloseConversation,
    						   iServiceId, 
                               aBuddyId                               
                                );
    TRACE( T_LIT("CIMCacheAccessor::CloseConversationL end") );
    }
 
// -----------------------------------------------------------------------------
// CIMCacheAccessor::GetChatListL()
// -----------------------------------------------------------------------------
//
RArray<SIMCacheChatItem> CIMCacheAccessor::GetChatListL( const TInt aServiceId  )
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
// CIMCacheAccessor::InternalizeChatListL
// -----------------------------------------------------------------------------
//
void CIMCacheAccessor::InternalizeChatListL(RArray<SIMCacheChatItem>& aChatArray, TPtr8 aChatListBuffer ,TBool& aMore )
    {
    TRACE( T_LIT("CIMCacheAccessor::InternalizeChatListL() begin") );   
    
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
    TRACE( T_LIT("CIMCacheAccessor::InternalizeChatListL() End") );
    }
 
//  END OF FILE


