/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Video thread proxy server 
*
*/


// INCLUDES
#include "CCMRVideoThreadProxyServer.h"
#include "CCMRVideoThreadProxySession.h"

#include <ecom/ecom.h>

// MACROS

// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif







// ====================== CCMRVideoThreadProxyServer MEMBER FUNCTIONS =======================


// -----------------------------------------------------------------------------
// CCMRVideoThreadProxyServer::CCMRVideoThreadProxyServer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCMRVideoThreadProxyServer::CCMRVideoThreadProxyServer(TInt aPriority) :
    CCMRThreadProxyServer(aPriority)
    {
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxyServer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCMRVideoThreadProxyServer* CCMRVideoThreadProxyServer::NewL(RServer2* aServer2)
    {
    CCMRVideoThreadProxyServer* self = new(ELeave) CCMRVideoThreadProxyServer(EPriorityHigh);
    CleanupStack::PushL(self);
    self->ConstructL(aServer2);
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxyServer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCMRVideoThreadProxyServer::ConstructL(RServer2* aServer2)
    {
    //just need to call baseclass's constructL here
    CCMRThreadProxyServer::ConstructL(aServer2);
    }

// destructor
CCMRVideoThreadProxyServer::~CCMRVideoThreadProxyServer()
    {
    }



// -----------------------------------------------------------------------------
// CCMRVideoThreadProxyServer::NewSessionL
// Create new session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CSession2* CCMRVideoThreadProxyServer::NewSessionL(const TVersion& /*aVersion*/, const RMessage2& /*aMessage*/) const
    {
    return CCMRVideoThreadProxySession::NewL();
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxyServer::StartThread
// Start server thread
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRVideoThreadProxyServer::StartThread(TAny* aParam)
    {
    PRINT((_L("CCMRVideoThreadProxyServer::StartThread() in")));
    TInt err = KErrNone;
    //create cleanupstack
    CTrapCleanup* cleanup = CTrapCleanup::New(); // get clean-up stack
    if (!cleanup)
        {
        err = KErrNoMemory;
        }
    if (!err)
        {
        TRAP(err, DoStartThreadL(aParam));
        }
    delete cleanup;
    PRINT((_L("CCMRVideoThreadProxyServer::StartThread() out")));
    return err;
    }

// -----------------------------------------------------------------------------
// CCMRVideoThreadProxyServer::DoStartThreadL
// Start server thread
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoThreadProxyServer::DoStartThreadL(TAny* aParam)
    {
    PRINT((_L("CCMRVideoThreadProxyServer::DoStartThread() in")));
    // create and install the active scheduler we need
    CActiveScheduler* self=new(ELeave) CActiveScheduler;
    CleanupStack::PushL(self);
    CActiveScheduler::Install(self);
    // create the server (leave it on the cleanup stack)
    CleanupStack::PushL(CCMRVideoThreadProxyServer::NewL(static_cast<RServer2*>(aParam)));
    // Initialisation complete, now signal the client
    RThread::Rendezvous(KErrNone);
    // Ready to run
    CActiveScheduler::Start();
    // Cleanup the server and scheduler
    CleanupStack::PopAndDestroy(2);

	REComSession::FinalClose();

    PRINT((_L("CCMRVideoThreadProxyServer::DoStartThread() out")));
    }





