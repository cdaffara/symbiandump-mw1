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
* Description:  Audio thread proxy classes
*
*/


//INCLUDES
#include "CCMRAudioThreadProxyServer.h"
#include "CCMRAudioThreadProxySession.h"

#include <ecom/ecom.h>

//MACROS
// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif




// ====================== CCMRAudioThreadProxyServer MEMBER FUNCTIONS =======================


// -----------------------------------------------------------------------------
// CCMRAudioThreadProxyServer::CCMRAudioThreadProxyServer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCMRAudioThreadProxyServer::CCMRAudioThreadProxyServer(TInt aPriority) :
    CCMRThreadProxyServer(aPriority)
    {
    }

// -----------------------------------------------------------------------------
// CCMRAudioThreadProxyServer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCMRAudioThreadProxyServer* CCMRAudioThreadProxyServer::NewL(RServer2* aServer2)
    {
    CCMRAudioThreadProxyServer* self = new(ELeave) CCMRAudioThreadProxyServer(EPriorityHigh);
    CleanupStack::PushL(self);
    self->ConstructL(aServer2);
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CCMRAudioThreadProxyServer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCMRAudioThreadProxyServer::ConstructL(RServer2* aServer2)
    {
    //just need to call baseclass's constructL here
    CCMRThreadProxyServer::ConstructL(aServer2);
    }

// destructor
CCMRAudioThreadProxyServer::~CCMRAudioThreadProxyServer()
    {
    }



// -----------------------------------------------------------------------------
// CCMRAudioThreadProxyServer::NewSessionL
// Create new session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CSession2* CCMRAudioThreadProxyServer::NewSessionL(const TVersion& /*aVersion*/,const RMessage2& /*aMessage*/) const
    {
    return CCMRAudioThreadProxySession::NewL();
    }

// -----------------------------------------------------------------------------
// CCMRAudioThreadProxyServer::StartThread
// Start server thread
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRAudioThreadProxyServer::StartThread(TAny* aParam)
    {
    PRINT((_L("CCMRAudioThreadProxyServer::StartThread() in")));
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
    PRINT((_L("CCMRAudioThreadProxyServer::StartThread() out")));
    return err;
    }

// -----------------------------------------------------------------------------
// CCMRAudioThreadProxyServer::DoStartThreadL
// Start server thread
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRAudioThreadProxyServer::DoStartThreadL(TAny* aParam)
    {
    PRINT((_L("CCMRAudioThreadProxyServer::DoStartThread() in")));
    // create and install the active scheduler we need
    CActiveScheduler* self=new(ELeave) CActiveScheduler;
    CleanupStack::PushL(self);
    CActiveScheduler::Install(self);
    // create the server (leave it on the cleanup stack)
    CleanupStack::PushL(CCMRAudioThreadProxyServer::NewL(static_cast<RServer2*>(aParam)));
    // Initialisation complete, now signal the client
    RThread::Rendezvous(KErrNone);
    // Ready to run
    CActiveScheduler::Start();
    // Cleanup the server and scheduler
    CleanupStack::PopAndDestroy(2);

	REComSession::FinalClose();

    PRINT((_L("CCMRAudioThreadProxyServer::DoStartThread() out")));
    }






