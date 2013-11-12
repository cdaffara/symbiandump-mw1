/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Proxy class to manage context client session.
 *
*/


#include "ximpcontextclientproxy.h"
#include "ximppanics.h"


const TInt KXIMPWaitListGranurality = 10;

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CXIMPContextClientProxy::CXIMPContextClientProxy()
// ---------------------------------------------------------------------------
//
CXIMPContextClientProxy::CXIMPContextClientProxy()
    : iCtxConnectionState( ENotConnected ),
      iReqWaitList( KXIMPWaitListGranurality )
    {
    }


// ---------------------------------------------------------------------------
// CXIMPContextClientProxy::~CXIMPContextClientProxy()
// ---------------------------------------------------------------------------
//
CXIMPContextClientProxy::~CXIMPContextClientProxy()
    {
    iReqWaitList.Reset();
    Close();
    }


// ---------------------------------------------------------------------------
// CXIMPContextClientProxy::InitServerSessionLC()
// ---------------------------------------------------------------------------
//
void CXIMPContextClientProxy::InitServerSessionLC( TUid aProtocolUid )
    {
    __ASSERT_ALWAYS( iCtxConnectionState == ENotConnected, 
                     User::Leave( KErrAlreadyExists ) );
    
    iCtxConnection.ConnectL( aProtocolUid );
    iCtxConnectionState = EInTransition;
    CleanupClosePushL( *this );
    }


// ---------------------------------------------------------------------------
// CXIMPContextClientProxy::QueueAsyncConnectionOpenL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CXIMPContextClientProxy::QueueAsyncConnectionOpenL(
    TInt aOperationType, 
    const TDesC8& aOperationParams )
    {
    //Async open requests are accepted only when client context is 
    //transition state
    __ASSERT_ALWAYS( iCtxConnectionState == EInTransition, 
                     User::Leave( KErrNotReady ) );
                     
    return DoQueueAsyncOpL( aOperationType, aOperationParams, EXIMPAsyncOpenReq );
    }

// ---------------------------------------------------------------------------
// CXIMPContextClientProxy::QueueAsyncConnectionOpen2L()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CXIMPContextClientProxy::QueueAsyncConnectionOpen2L(
    TInt aOperationType, 
    const TDesC8& aOperationParams )
    {
    //Async open requests are accepted only when client context is 
    //transition state
    __ASSERT_ALWAYS( iCtxConnectionState == EInTransition, 
                     User::Leave( KErrNotReady ) );

    return DoQueueAsyncOp2L( aOperationType, aOperationParams, EXIMPAsyncOpenReq );
    }

// ---------------------------------------------------------------------------
// CXIMPContextClientProxy::QueueAsyncConnectionCloseL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CXIMPContextClientProxy::QueueAsyncConnectionCloseL(
    TInt aOperationType, 
    const TDesC8& aOperationParams )
    {
    //Async close requests are accepted when client context is 
    //fully binded or in transition state
    __ASSERT_ALWAYS( ( iCtxConnectionState == EConnected ) ||
                     ( iCtxConnectionState == EInTransition ), 
                     User::Leave( KErrNotReady ) );
                     
    return DoQueueAsyncOpL( aOperationType, aOperationParams, EXIMPAsyncCloseReq );
    }
    


// ---------------------------------------------------------------------------
// CXIMPContextClientProxy::QueueOperationL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CXIMPContextClientProxy::QueueOperationL(
    TInt aOperationType, 
    const TDesC8& aOperationParams )
    {
    //New operation requests are accepted only when client context is 
    //fully binded
    __ASSERT_ALWAYS( iCtxConnectionState == EConnected, 
                     User::Leave( KErrNotReady ) );

    return DoQueueAsyncOpL( aOperationType, aOperationParams, EXIMPAsyncGeneralReq );
    }



// ---------------------------------------------------------------------------
// CXIMPContextClientProxy::DirectSessionL()
// ---------------------------------------------------------------------------
//
const RXIMPSrvContextClient& CXIMPContextClientProxy::DirectSessionL() const 
    {
    //Other direct requests are accepted only when client context is fully binded 

    __ASSERT_ALWAYS( iCtxConnectionState == EConnected, 
                     User::Leave( KErrNotReady ) );
    return iCtxConnection;
    }


// ---------------------------------------------------------------------------
// CXIMPContextClientProxy::EventPumpSessionL()
// ---------------------------------------------------------------------------
//
RXIMPSrvContextClient& CXIMPContextClientProxy::EventPumpSessionL()
    {
    //Event pump requests are accepted when client context is either fully binded 
    //or in transition state
    __ASSERT_ALWAYS( iCtxConnectionState != ENotConnected, 
                     User::Leave( KErrNotReady ) );

    return iCtxConnection;
    }

    

// ---------------------------------------------------------------------------
// CXIMPContextClientProxy::EventPumpSession()
// ---------------------------------------------------------------------------
//
RXIMPSrvContextClient* CXIMPContextClientProxy::EventPumpSession()
    {
    //Event pump requests are accepted when client context is either fully binded 
    //or in transition state

    if( iCtxConnectionState != ENotConnected )
        {
        return &iCtxConnection;    
        }
    else
        {
        return NULL;
        }        
    }

    
    
// ---------------------------------------------------------------------------
// CXIMPContextClientProxy::Close()
// ---------------------------------------------------------------------------
//
void CXIMPContextClientProxy::Close()
    {
    iCtxConnection.Close();
    iCtxConnectionState = ENotConnected;
    }


// ---------------------------------------------------------------------------
// CXIMPContextClientProxy::DoQueueAsyncOpL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CXIMPContextClientProxy::DoQueueAsyncOpL(
    TInt aOperationType, 
    const TDesC8& aOperationParams,
    TXIMPAsyncReqType aReqType )
    {
    iReqWaitList.ExtendL();
    TXIMPRequestId regId;

    TInt err = iCtxConnection.DoQueueOperation( aOperationType, 
                                                aOperationParams, 
                                                regId );
                                            
    if( err == KErrNone )                                            
        {
        TXIMPAsyncReqEntry reqEntry;
        reqEntry.iReqId = regId;
        reqEntry.iReqType = aReqType;
        
        iReqWaitList[ iReqWaitList.Count() - 1 ] = reqEntry;        
        }
    else        
        {
        iReqWaitList.Delete( iReqWaitList.Count() - 1 );
        User::Leave( err );
        }
    
    return regId;
    }

// ---------------------------------------------------------------------------
// CXIMPContextClientProxy::DoQueueAsyncOp2L()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CXIMPContextClientProxy::DoQueueAsyncOp2L(
    TInt aOperationType, 
    const TDesC8& aOperationParams,
    TXIMPAsyncReqType aReqType )
    {
    iReqWaitList.ExtendL();
    TXIMPRequestId regId;

    TInt err = iCtxConnection.DoQueueOperation2( aOperationType, 
                                                aOperationParams, 
                                                regId );
                                            
    if( err == KErrNone )                                            
        {
        TXIMPAsyncReqEntry reqEntry;
        reqEntry.iReqId = regId;
        reqEntry.iReqType = aReqType;
        
        iReqWaitList[ iReqWaitList.Count() - 1 ] = reqEntry;        
        }
    else        
        {
        iReqWaitList.Delete( iReqWaitList.Count() - 1 );
        User::Leave( err );
        }
    
    return regId;
    }


// ---------------------------------------------------------------------------
// CXIMPContextClientProxy::HandleReqCompleted()
// ---------------------------------------------------------------------------
//
void CXIMPContextClientProxy::HandleReqCompleted( TXIMPRequestId aReqId,
                                                  TInt aCompletionCode )
    {
    const TInt count = iReqWaitList.Count();                                                                        
    for( TInt ix = 0; ix < count; ix++ )                                                                
        {
        TXIMPAsyncReqEntry entry = iReqWaitList[ ix ];
        if( entry.iReqId == aReqId )
            {
            iReqWaitList.Delete( ix );
            UpdateCtxConnectionStatus( entry.iReqType, aCompletionCode );
            return;
            }
        }
        
    NXIMPPrivPanic::Panic( NXIMPPrivPanic::EMatchingClientSideReqNotFound );
    }
    
    
// ---------------------------------------------------------------------------
// CXIMPContextClientProxy::GetTopPendingReq()
// ---------------------------------------------------------------------------
//
TBool CXIMPContextClientProxy::GetTopPendingReq( TXIMPRequestId& aReqId ) const
    {
    if( iReqWaitList.Count() > 0 )
        {
        aReqId = iReqWaitList[ 0 ].iReqId;
        return ETrue;
        }

    aReqId = TXIMPRequestId::Null();
    return EFalse;
    }


// ---------------------------------------------------------------------------
// CXIMPContextClientProxy::UpdateCtxConnectionStatus()
// ---------------------------------------------------------------------------
//
void CXIMPContextClientProxy::UpdateCtxConnectionStatus( 
    TXIMPAsyncReqType aReqType, 
    TInt aCompletionCode )
    {
    if( aReqType == EXIMPAsyncOpenReq )  
        {
        if( aCompletionCode == KErrNone )
            {
            iCtxConnectionState = EConnected;            
            }
        else
            {
            Close();  
            }
        }
        
        
    if( aReqType == EXIMPAsyncCloseReq )
        {
        Close();  
        }
    }





