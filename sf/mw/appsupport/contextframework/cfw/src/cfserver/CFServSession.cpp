/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFServSession class implementation.
*
*/


#include "CFServSession.h"

#include <e32svr.h>

#include "CFMessageHandlerBase.h"
#include "CFMessageHandlerContext.h"
#include "CFMessageHandlerAction.h"
#include "CFMessageHandlerScript.h"
#include "cfcommon.h"
#include "cftrace.h"

CCFServSession* CCFServSession::NewL( MCFExtendedContextInterface& aCFContext,
    MCFActionInterface& aCFAction,
    MCFScriptInterface& aScriptInterface )
    {
    FUNC_LOG;
    
    CCFServSession* self
        = CCFServSession::NewLC( aCFContext, aCFAction, aScriptInterface );
    CleanupStack::Pop(self);

    return self;
    }

CCFServSession* CCFServSession::NewLC( MCFExtendedContextInterface& aCFContext,
    MCFActionInterface& aCFAction,
    MCFScriptInterface& aScriptInterface )
    {
    FUNC_LOG;
    
    CCFServSession* self = new ( ELeave ) CCFServSession;
    CleanupStack::PushL( self );
    self->ConstructL( aCFContext, aCFAction, aScriptInterface );

    return self;
    }

CCFServSession::~CCFServSession()
    {
    FUNC_LOG;
    
    iMessageHandlers.ResetAndDestroy();
    }

void CCFServSession::ConstructL( MCFExtendedContextInterface& aCFContext,
    MCFActionInterface& aCFAction,
    MCFScriptInterface& aScriptInterface )
    {
    FUNC_LOG;
    
    // Create message handlers
    iMessageHandlers.InsertL(
        CCFMessageHandlerContext::NewL( aCFContext, aCFAction, aScriptInterface ),
        CCFMessageHandlerBase::EContextMessageHandler );
    iMessageHandlers.InsertL(
        CCFMessageHandlerAction::NewL( aCFContext, aCFAction, aScriptInterface ),
        CCFMessageHandlerBase::EActionMessageHandler );
    iMessageHandlers.InsertL(
        CCFMessageHandlerScript::NewL( aCFContext, aCFAction, aScriptInterface ),
        CCFMessageHandlerBase::EScriptMessageHandler );
    }

CCFServSession::CCFServSession()
    {
    FUNC_LOG;
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFServSession::ServiceL
//-----------------------------------------------------------------------------
//
void CCFServSession::ServiceL( const RMessage2& aMessage )
    {
    FUNC_LOG;
    CLIENT( aMessage );
    
    TRAPD( error, DispatchMessageL( aMessage ) );
    if( error )
        {
        ERROR_1( error, "CCFServSession::ServiceL: Dispatch message failed: %d", error );
        if( !aMessage.IsNull() )
            {
            aMessage.Complete( error );
            }
        }
    }
    
//-----------------------------------------------------------------------------
// CCFServSession::DispatchMessageL
//-----------------------------------------------------------------------------
//
void CCFServSession::DispatchMessageL( const RMessage2 &aMessage )
    {
    FUNC_LOG;
    
    // Forward message to handlers
    TBool handled = EFalse;
    TInt count = iMessageHandlers.Count();
    for( TInt i = 0; i < count; i++ )
        {
        handled = iMessageHandlers[i]->HandleMessageL( aMessage );
        if( handled )
            {
            break;
            }
        }
        
    // Unrecognized message, panic
    if( !handled )
        {
        ERROR_GEN( "Unknown message from client!" );
        PanicClient( aMessage, EBadRequest );
        }
    }
    
//-----------------------------------------------------------------------------
// CCFServSession::PanicClient
//-----------------------------------------------------------------------------
//
void CCFServSession::PanicClient( const RMessage2& aMessage, TInt aPanic )
    {
    FUNC_LOG;
    
    _LIT(KTxtServer,"CFWSRVSESSION");
    aMessage.Panic(KTxtServer,aPanic);
    }

//-----------------------------------------------------------------------------
// CCFServSession::Write
//-----------------------------------------------------------------------------
//
void CCFServSession::Write( const RMessage2& aMessage, TInt aParam,
    const TDesC8& aDes,
    TInt anOffset )
    {
    FUNC_LOG;
    
    TRAPD( ret,aMessage.WriteL( aParam, aDes, anOffset ) )
    if( ret != KErrNone )
        {
        PanicClient( aMessage, EBadDescriptor );
        }
    }
