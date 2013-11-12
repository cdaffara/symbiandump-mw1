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
* Description:  client class implementation to connect to server
*
*/



//  INCLUDE FILES
#include "cimcacheclient.h"

#include "imcacheprocessstarter.h"
#include <imcachedefs.h>
#include "mimcacheclient.h"
// logs
#include "imcachedebugtrace.h"
#include "cimcacheeventhandler.h"

#include "cimcacheaccesseventhandler.h"

// ==============================================================
// ======================== CLIENT ==============================
// ==============================================================

// Destructor
CIMCacheClient::~CIMCacheClient()
	{
	TRACE( T_LIT("CIMCacheClient::~CIMCacheClient() begin") );
	if( iUpdateObserver )
		{
		delete iUpdateObserver;
		}
	
	if( iAccessObserver )
		{
		delete iAccessObserver;
		}
    if( iChatBuffer )
        {
        delete iChatBuffer;
        }
    //finally close the session.
    RSessionBase::Close();
	#if _BullseyeCoverage
	cov_write();
	#endif
	TRACE( T_LIT("CIMCacheClient::~CIMCacheClient() end") );
	}
// -----------------------------------------------------------------------------
// CIMCacheClient::CIMCacheClient()
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIMCacheClient::CIMCacheClient( )
	{
    TRACE( T_LIT("CIMCacheClient::CIMCacheClient()") );
    }


// -----------------------------------------------------------------------------
// CIMCacheClient::Connect()
// -----------------------------------------------------------------------------
//
TInt CIMCacheClient::Connect()
    {
    TRACE( T_LIT("CIMCacheClient::Connect() begin") );
    TFileName fullExePath;
    IMCacheProcessStarter::FullExePathForClientLocation( EIMCacheServerExe,
                                                 fullExePath );

	TRACE( T_LIT("CIMCacheClient::Connect() end") );
    return IMCacheProcessStarter::ConnectToServer( fullExePath,
                                            KNullDesC,
                                            *this,
                                            KIMCacheServerName,
                                            TVersion( KIMCacheVersionMajor,
                                                      KIMCacheVersionMinor,
                                                      KIMCacheVersionBuild ),
                                            KIMCacheMsgSlotCount );
    
    }

// -----------------------------------------------------------------------------
// CIMCacheClient::RegisterObserverL()
// -----------------------------------------------------------------------------
//
void CIMCacheClient::RegisterUpdateObserverL( MIMCacheEventHandler& aObserver )
	{
	TRACE( T_LIT("CIMCacheClient::RegisterUpdateObserverL() begin") );

	CIMCacheEventHandler* tempObserver = CIMCacheEventHandler::NewL(  *this , aObserver );

	if( iUpdateObserver )
		{
		delete iUpdateObserver;
		iUpdateObserver = NULL;
		}

	iUpdateObserver = tempObserver;
	TRACE( T_LIT("CIMCacheClient::RegisterUpdateObserverL() end") );
	}

// -----------------------------------------------------------------------------
// CIMCacheClient::UnRegisterUpdateObserver()
// -----------------------------------------------------------------------------
//
void CIMCacheClient::UnRegisterUpdateObserver( MIMCacheEventHandler& /*aObserver*/)
	{
	TRACE( T_LIT("CIMCacheClient::UnRegisterUpdateObserver() begin") );
	
	iUpdateObserver->UnRegisterObserver();

    TRACE( T_LIT("CIMCacheClient::UnRegisterUpdateObserver() end") );
	}

// -----------------------------------------------------------------------------
// CIMCacheClient::RegisterAccessObserverL()
// -----------------------------------------------------------------------------
//
void CIMCacheClient::RegisterAccessObserverL( MIMCacheEventHandler& aObserver )
	{
	TRACE( T_LIT("CIMCacheClient::RegisterUpdateObserverL() begin") );

	CIMCacheAccessEventHandler* tempObserver = CIMCacheAccessEventHandler::NewL(  *this , aObserver );

	if( iAccessObserver )
		{
		delete iAccessObserver;
		iAccessObserver = NULL;
		}

	iAccessObserver = tempObserver;
	TRACE( T_LIT("CIMCacheClient::RegisterUpdateObserverL() end") );
	}

// -----------------------------------------------------------------------------
// CIMCacheClient::UnRegisterAccessObserver()
// -----------------------------------------------------------------------------
//
void CIMCacheClient::UnRegisterAccessObserver( MIMCacheEventHandler& /*aObserver*/)
	{
	TRACE( T_LIT("CIMCacheClient::UnRegisterUpdateObserver() begin") );
	
	iAccessObserver->UnRegisterObserver();

    TRACE( T_LIT("CIMCacheClient::UnRegisterUpdateObserver() end") );
	}
// -----------------------------------------------------------------------------
// CIMCacheClient::RegisterObserverToServerL()
// -----------------------------------------------------------------------------
//
void CIMCacheClient::RegisterObserverToServerL( TRequestStatus& aStatus , EIMCacheOperations aRight)
	{
	TRACE( T_LIT("CIMCacheClient::RegisterObserverToServerL() begin") );
	TIpcArgs args;
	SendReceive( aRight, args, aStatus );
	TRACE( T_LIT("CIMCacheClient::RegisterObserverToServerL() end") );
	}

// -----------------------------------------------------------------------------
// CIMCacheClient::UnRegisterObserverToServerL()
// -----------------------------------------------------------------------------
//
void CIMCacheClient::UnRegisterObserverToServerL( TRequestStatus& aStatus, EIMCacheOperations aRight)
	{
	TRACE( T_LIT("CIMCacheClient::UnRegisterObserverToServerL() begin") );
	TIpcArgs args;
	TInt err = SendReceive( aRight, args );
	User::LeaveIfError( err );
	TRACE( T_LIT("CIMCacheClient::UnRegisterObserverToServerL() end") );
	}
// -----------------------------------------------------------------------------
// CIMCacheClient::CancelRequestL()
// -----------------------------------------------------------------------------
//
void CIMCacheClient::CancelRequestL( TRequestStatus& aStatus, EIMCacheOperations aRight)
    {
    TRACE( T_LIT("CIMCacheClient::CancelRequestL() begin") );
    TIpcArgs args;
    TInt err = SendReceive( aRight, args );
    User::LeaveIfError( err );
    TRACE( T_LIT("CIMCacheClient::CancelRequestL() end") );
    }
// -----------------------------------------------------------------------------
// CIMCacheClient::StartTransactionL()
// -----------------------------------------------------------------------------
//
void CIMCacheClient::StartTransactionL( EIMCacheOperations aOperationType,
									   TInt aServiceId ,
									   const TDesC& aText
									   )
	{
	TRACE( T_LIT("CIMCacheClient::StartTransactionL() begin") );
	TIpcArgs args = CreateArgumentsL( aServiceId, aText );
 	TInt err = SendReceive( aOperationType, args );
    User::LeaveIfError( err );
    TRACE( T_LIT("CIMCacheClient::StartTransactionL() end") );
	}


// -----------------------------------------------------------------------------
// CIMCacheClient::StartTransactionL()
// -----------------------------------------------------------------------------
//
void CIMCacheClient::StartTransactionL(EIMCacheOperations aOperationType,
										const TDesC& aText
									   )
	{
	TRACE( T_LIT("CIMCacheClient::StartTransactionL() begin") );
	TIpcArgs args;
    args.Set( 0,  &aText );
   	TInt err = SendReceive( aOperationType, args );
    User::LeaveIfError( err );
    TRACE( T_LIT("CIMCacheClient::StartTransactionL() end") );
	}
// -----------------------------------------------------------------------------
// CIMCacheClient::StartTransactionL()
// -----------------------------------------------------------------------------
//
void CIMCacheClient::StartTransactionL(	EIMCacheOperations aOperationType,
										TInt aServiceId,
										const TDesC& aText1 ,
										const TDesC& aText2 
									   	)
	{
	TRACE( T_LIT("CIMCacheClient::StartTransactionL() begin") );
	TIpcArgs args = CreateArgumentsL(aServiceId, aText1, aText2 );
 	TInt err = SendReceive( aOperationType, args );
    User::LeaveIfError( err );
    TRACE( T_LIT("CIMCacheClient::StartTransactionL() end") );
	}
	
// -----------------------------------------------------------------------------
// CIMCacheClient::StopTransactionL()
// -----------------------------------------------------------------------------
//
TInt CIMCacheClient::StartTransactionL(EIMCacheOperations aOperationType )
	{
	TRACE( T_LIT("CIMCacheClient::StopTransactionL() begin") );
	TRACE( T_LIT(" aOperationType = %d  "),aOperationType );
	TInt err = SendReceive( aOperationType );
    TRACE( T_LIT("CIMCacheClient::StopTransactionL() end") );
	return err;
	}

// -----------------------------------------------------------------------------
// CIMCacheClient::CreateArgumentsL()
// -----------------------------------------------------------------------------
//
TIpcArgs CIMCacheClient::CreateArgumentsL( TInt aServiceId ,
									   const TDesC& aText  )
	{
	TRACE( T_LIT("CIMCacheClient::CreateArgumentsL() begin") );
	// set the arguments for the message
	TIpcArgs args;
    args.Set( 0,  aServiceId );
    args.Set( 1,  &aText );
    TRACE( T_LIT("CIMCacheClient::CreateArgumentsL() end") );
    return args;
	}

 // -----------------------------------------------------------------------------
// CIMCacheClient::CreateArgumentsL()
// -----------------------------------------------------------------------------
//
TIpcArgs CIMCacheClient::CreateArgumentsL(TInt aServiceId, 
										const TDesC& aText1  ,
									   const TDesC& aText2  )
	{
	TRACE( T_LIT("CIMCacheClient::CreateArgumentsL() begin") );
	// set the arguments for the message
	TIpcArgs args(aServiceId);
    args.Set( 1,  &aText1 );
    args.Set( 2,  &aText2 );
    TRACE( T_LIT("CIMCacheClient::CreateArgumentsL() end") );
    return args;
	}


// -----------------------------------------------------------------------------
// CIMCacheClient::GetInformationL()
// -----------------------------------------------------------------------------
//
TInt CIMCacheClient::GetInformationL(EIMCacheOperations aOperationType, TInt aServiceId,const TDesC& aBuddyId )
	{
	TRACE( T_LIT("CIMCacheClient::GetInformationL start") );
	TInt returnValue = 0;
	TIpcArgs args(aServiceId);
	args.Set( 1,  &aBuddyId );
    TPckg<TInt> pack( returnValue );
    args.Set(2, &pack );
    TInt err = SendReceive( aOperationType, args );
    User::LeaveIfError( err );
    TRACE( T_LIT("CIMCacheClient::GetInformationL() end") );
	return returnValue;
	}

 // -----------------------------------------------------------------------------
// CIMCacheClient::GetCountL()
// -----------------------------------------------------------------------------
//
TInt CIMCacheClient::GetInformationL(EIMCacheOperations aOperationType, TInt aServiceId )
	{
	TRACE( T_LIT("CIMCacheClient::GetInformationL") );
	TInt returnValue = 0;
	TIpcArgs args(aServiceId);
        
    TPckg<TInt> pack( returnValue );
    args.Set(1, &pack );
    TInt err = SendReceive( aOperationType, args );
    User::LeaveIfError( err );
    TRACE( T_LIT("CIMCacheClient::GetInformationL() end") );
	return returnValue;
	}	
// -----------------------------------------------------------------------------
// CIMCacheClient::GetContactBufferL()
// -----------------------------------------------------------------------------
//
TPtr8 CIMCacheClient::GetBufferedDataL(EIMCacheOperations aOperationType )
    {
    TRACE( T_LIT("CIMCacheClient::GetContactBufferL() begin") );
    
    if(!iChatBuffer)//if not allocated allocate.
        {
        iChatBuffer = HBufC8::NewL(KMaxSize);
        }
    TPtr8 chatBufferPtr = iChatBuffer->Des();
    TIpcArgs msgArgs;
    msgArgs.Set( 0, &chatBufferPtr );
    
    TInt err = SendReceive( aOperationType, msgArgs );
    User::LeaveIfError( err );
  
    TRACE( T_LIT("CIMCacheClient::GetContactBufferL() end") );
    
    return iChatBuffer->Des();
    }
    
// -----------------------------------------------------------------------------
// CIMCacheClient::GetChatListL()
// -----------------------------------------------------------------------------    
TPtr8 CIMCacheClient::GetChatListL( const TInt& aServiceId /*= -1*/ )
    {
    
    if(!iChatBuffer)//if not allocated allocate.
        {
        iChatBuffer = HBufC8::NewL(KMaxSize);
        }
    TPtr8 ptr = iChatBuffer->Des();
     
    TIpcArgs msgArgs( aServiceId );//at 0.
    msgArgs.Set( 1, &ptr  );
    
    TInt err = SendReceive( EIMCacheGetChatList, msgArgs );
    User::LeaveIfError( err );
  
  	return iChatBuffer->Des();
    }
    
// end of file

