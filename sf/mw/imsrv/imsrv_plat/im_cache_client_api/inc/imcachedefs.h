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
* Description:  constanst declaration files
*
*/



#ifndef __IMCACHEDEFS_H__
#define __IMCACHEDEFS_H__
#include <e32std.h>

const TInt KMaxSize( 1024 );
/** 
* Different message types
*/
enum TIMCacheMessageType
	{
	EIMCMessagePTOP,
	EIMCMessageSystem,
	EIMCMessageInvalid
	};


/**
* Direction of message. Coming or going or other.
*/
enum TIMCacheMessagerType
	{
	EIMCMessageSent,
	EIMCMessageReceived,
	EIMCMessageOther
	};

/**
* Different message content types
*/
enum TIMCacheContentType
	{
	EIMCContentText,
	EIMCContentOther,
	EIMCContentInvalid
	};	

/**
* struct for send and recieve message details
*/
struct SIMCacheMessageData
	{
	TIMCacheMessageType  	iMessageType;
	TIMCacheMessagerType	iMessagerType;
	TIMCacheContentType		iContentType;
	TTime 	  				iTime;
	HBufC* 					iBuddyId;
	HBufC*	 				iText;
	};

/**
* struct to contain newly added/removed chat details.
*/
struct SIMCacheChatItem
    {
    TInt    iServiceId;
    HBufC*  iBuddyId;
    };

typedef TPckgBuf<SIMCacheMessageData> TIMCacheMessageBuf;
/**
 *
 *cahce versions
 */
const TInt KIMCacheVersionMajor = 1;
const TInt KIMCacheVersionMinor = 1;
const TInt KIMCacheVersionBuild = 1;

/**
 *cache server exe name
 *
 */
_LIT( EIMCacheServerExe, "imcacheserver.exe" );


/**
 *cache server name
 *
 */
_LIT( KIMCacheServerName, "imcacheserver" );


/**
 *
 *
 */
const TUint KIMCacheMsgSlotCount = 4;

/**
 * transaction types
 */
enum TTransactionType
	{
	EIMCacheTxAppend,
	EIMCacheTxReplace,
	EIMCacheTxAccess
	};

/**
 * operation to perform
 * 
 */
enum EIMCacheOperations
    {
    // begining operation boundary for client-server
    EIMCacheOperationFirst = 0,
    
    // access API 
    EIMCacheInitUpdate , 
    EIMCacheInitAccess ,
    EIMCacheInitConvesation,
       // update  
    EIMCacheStartNewConversation,
    EIMCacheAppendReceiveMessage,
    EIMCacheAppendSendMessage,
    EIMCacheAppendMessage,	
    EIMCacheCloseConversation, 
    EIMCacheCloseAllConversation,
    
    EIMCacheDeactivateConversation, 
    // access
    EIMCacheGetUnreadCount,
    EIMCacheGetAllUnreadCount,
    EIMCacheIsConversationExist,
    EIMCacheGetAllConversation,
	// observer operation
	// for already stored messages
	EIMCacheObserveMessageUpdateRegister,
	EIMCacheObserveMessageUpdateUnRegister,
	
	EIMCacheObserveMessageAccessRegister,
	EIMCacheObserveMessageAccessUnRegister,

	//new/deleted chat items
	EIMCacheGetBufferedPackets,
	
	//GetChatItemList
	EIMCacheGetChatList,
	EIMCacheCancelRequest,
	
	// this is last operation boundary supported by client-server ( range has been set )
	// do not add any operation after this
    EIMCacheOperationLast
    
    };

/**
 * operation error code
 * 
 */
enum TIMCacheOperationsCode
    {
   	EIMOPerationBase = -1000,
    // update  
    EIMOperationCompleted,
	EIMOperationAborted,
    // observer operation
	EIMOperationRegistered,
	EIMOperationUnRegistered,
	
	EIMOperationDataAvailable,
	EIMOperationUnreadChange,
	EIMOperationUnreadMessage,
	
	EIMOperationDataUnAvailable,
	EIMOperationSessionDied,
	EIMOperationCacheDataChaned,
	EIMOperationConversationClosed,
	EIMOperationFetchAll,
	EIMOperationChatAdded,
	EIMOperationMessageAdded,
	EIMOperationChatDeleted,
	EIMOperationAllChatDeleted,
	EIMOperationChatStarted
	};

/**
 * operation to perform
 * 
 */
enum TIMCacheEventType
    {
    EIMCacheRequestCompleted = 0,
   	EIMCacheNewMessage ,
   	EIMCacheUnreadMessage,
   	EIMCacheUnreadChange,
   	EIMCacheNewChat,
   	EIMCacheChatStarted,
   	EIMCacheChatClosed,
   	EIMCacheAllChatClosed
    };
/**
 * 
 * 
 */
enum TIMCacheChatListOperation
    {
    EIMChatId,
    EIMChatList
    };

#endif  //__IMCACHEDEFS_H__

// END OF FILE




