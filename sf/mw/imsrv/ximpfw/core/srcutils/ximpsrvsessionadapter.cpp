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
* Description:  Server session adapter base class.
 *
*/


//  INCLUDE FILES
#include "ximpsrvsessionadapter.h"
#include "ximpsrv.h"
#include "ximpsrvsession.h"
#include "ximpsrvmessageimp.h"
#include "ximppanics.h"
#include "ximptrace.h"
#include <e32base.h>


// -----------------------------------------------------------------------------
// CXIMPSrvSessionAdapter::CXIMPSrvSessionAdapter()
// -----------------------------------------------------------------------------
//
EXPORT_C CXIMPSrvSessionAdapter::CXIMPSrvSessionAdapter( MXIMPSrv& aServer )
    : iServer( aServer )
    {
    iServer.SessionCreated();
    TRACE_1( _L("CXIMPSrvSessionAdapter[%d]::CXIMPSrvSessionAdapter()"),
                 this );
    }


// -----------------------------------------------------------------------------
// CXIMPSrvSessionAdapter::~CXIMPSrvSessionAdapter()
// -----------------------------------------------------------------------------
//
EXPORT_C CXIMPSrvSessionAdapter::~CXIMPSrvSessionAdapter()
    {
    delete iMessage;
    
    iServer.SessionDied();
    delete iSessionImp;

    TRACE_1( _L("CXIMPSrvSessionAdapter[%d]::~CXIMPSrvSessionAdapter()"),
                 this );
    #if _BullseyeCoverage
    cov_write();
    #endif
    }


// -----------------------------------------------------------------------------
// CXIMPSrvSessionAdapter::ServiceL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPSrvSessionAdapter::ServiceL( const RMessage2& aMessage )
    {
    __ASSERT_ALWAYS( !iCurrentMsg,
                     User::Panic( NXIMPPrivPanic::KCategory,
                                  NXIMPPrivPanic::ECurrentMsgAlreadyExists ) );

    //Non leaving allocation must be used here to prevent OOM leave
    //to ServiceError() method, without a valid iCurrentMsg
    //==> CurrentMsg absence triggers an assert in ServiceError()
    iCurrentMsg = CXIMPSrvMessageImp::NewOrNull( aMessage,
                                                 ( MXIMPSrvMessage*& )iCurrentMsg, 
                                                 iMessage );
    iMessage = NULL;

    //Handle the message
    MXIMPSrvSession* sessionImp = DoInstantiateSessionL( *iCurrentMsg );
    
    if( sessionImp )
        {
        CleanupDeletePushL( sessionImp );

        __ASSERT_ALWAYS( !iSessionImp,
                         iCurrentMsg->PanicClientAndLeaveL(
                         NXIMPPanic::EOnSrvSessionImpAlreadyExists ) );

        iSessionImp = sessionImp;
        CleanupStack::Pop(); //sessionImp;
        iCurrentMsg->Complete( KErrNone );
        }

    else
        {                
        __ASSERT_ALWAYS( iSessionImp,
                         iCurrentMsg->PanicClientAndLeaveL(
                         NXIMPPanic::EOnSrvSessionImpNotFound ) );


        iSessionImp->TryHandleMessageL( *iCurrentMsg );


        //If current message is still not-completed, inform client
        //that it tried to do something unknown
        __ASSERT_ALWAYS( !IsCurrentMessageValid(),
                         iCurrentMsg->PanicClientAndLeaveL(
                         NXIMPPanic::EOnSrvRequestNotUnderstood) );
        }

    iMessage = iCurrentMsg;
    iCurrentMsg = NULL;
    }
  
// -----------------------------------------------------------------------------
// CXIMPSrvSessionAdapter::ServiceError()
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPSrvSessionAdapter::ServiceError( const RMessage2& /*aMessage*/,
                                                    TInt aError )
    {
    //If assert below is triggered, it means that current message ownersip
    //is taken somewhere (to be completed asynchronously), but there has
    //happened a leave after ownership transfer

    //The place taking message ownership has to be fixed in this case
    //not to cause exeptions after ownership transfer
    __ASSERT_ALWAYS( iCurrentMsg,
                     User::Panic( NXIMPPrivPanic::KCategory,
                                  NXIMPPrivPanic::ECurrentMsgNotFoundInServiceError ) );


    //If assert below is triggered, it means that current message is already
    //completed, but there has happened a leave after completion

    //The place completing message has to be fixed in this case
    //not to cause exeptions after message completion
    __ASSERT_ALWAYS( !iCurrentMsg->IsCompleted(),
                     User::Panic( NXIMPPrivPanic::KCategory,
                                  NXIMPPrivPanic::ECurrentMsgNotValidInServiceError ) );


    TRACE_3( _L("CXIMPSrvSessionAdapter[%d]::ServiceError() in op[%d], code[%d]"),
                 this, iCurrentMsg->Function(), aError );


    iCurrentMsg->Complete( aError );
    delete iCurrentMsg;
    iCurrentMsg = NULL;
    }


// -----------------------------------------------------------------------------
// CXIMPSrvSessionAdapter::Server()
// -----------------------------------------------------------------------------
//
EXPORT_C MXIMPSrv& CXIMPSrvSessionAdapter::Server()
    {
    return iServer;
    }

// -----------------------------------------------------------------------------
// CXIMPSrvSessionAdapter::IsCurrentMessageValid()
// -----------------------------------------------------------------------------
//
TBool CXIMPSrvSessionAdapter::IsCurrentMessageValid()
    {
    if( iCurrentMsg && !iCurrentMsg->IsCompleted() )
        {
        return ETrue;
        }

    return EFalse;
    }



// End of file
