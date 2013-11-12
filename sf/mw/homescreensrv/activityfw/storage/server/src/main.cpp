/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/
#include "afserver.h"
#include <e32base.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LOCAL_C void StartActivityServerL()
{
    CActiveScheduler* activeScheduler = new( ELeave ) CActiveScheduler;
    CleanupStack::PushL( activeScheduler );
    CActiveScheduler::Install( activeScheduler );
    CAfServer* serverObject = CAfServer::NewLC();
    RProcess::Rendezvous( KErrNone );
    CActiveScheduler::Start();
    CleanupStack::PopAndDestroy( serverObject );
    CleanupStack::PopAndDestroy( activeScheduler );
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LOCAL_C TInt StartActivityServer()
{
    __UHEAP_MARK;
    TInt errNo(KErrNoMemory);
    CTrapCleanup* cleanupStack = CTrapCleanup::New();
    if (cleanupStack) {
        TRAP(errNo, StartActivityServerL());
        delete cleanupStack;
    }
    __UHEAP_MARKEND;
    if (KErrNone != errNo) {
        RProcess::Rendezvous(errNo);
    }
    return errNo;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt E32Main()
{
    return StartActivityServer();
}
