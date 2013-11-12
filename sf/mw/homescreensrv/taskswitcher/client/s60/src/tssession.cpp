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
#include <s32mem.h>
#include <fbs.h>

#include "tssession.h"
#include "tsscreenshotmsg.h"
#include "tsunregscreenshotmsg.h"
#include "tsvisibilitymsg.h"

TInt RTsSession::Connect()
{
    TInt retry = 4;
    TInt error(KErrNone);
    do {
        error = CreateSession(KRunningAppServerName, TVersion(0, 0, 0));
        if (KErrNotFound == error || KErrServerTerminated == error) {
            error = StartServer();
            if (KErrNone == error || KErrAlreadyExists == error) {
                retry = 0;
                error = CreateSession(KRunningAppServerName, TVersion(0, 0, 0));
            }
        } else {
            retry = 0;
        }
    } while (--retry > 0);
    return error;
}

TInt RTsSession::StartServer()
    {
    RSemaphore semaphore;
    CleanupClosePushL(semaphore);
    TInt error = semaphore.OpenGlobal(KTsServerReadySemaphore);
    if (KErrNone != error) {
        RProcess server;
        CleanupClosePushL(server);
        error = server.Create(KRunningAppServerName, KNullDesC, TUidType(KNullUid, KNullUid, KRunningAppServerUid));
        if (KErrNone == error) 
            {
            TRequestStatus status;
            server.Rendezvous(status);
            if (status != KRequestPending) 
                {
                server.Kill(0);
                User::WaitForRequest(status);
                }
            else
                {
                server.Resume();
                User::WaitForRequest(status);
                }
            
            if(KErrCancel == status.Int())
                {
                error = KErrNone;
                }
            else
                {
                error = (EExitPanic == server.ExitType()) ? KErrGeneral : status.Int();
                }
            }
        CleanupStack::PopAndDestroy(&server);
        }
    CleanupStack::PopAndDestroy(&semaphore);
    return error;
    }


HBufC8* RTsSession::TasksContentLC()
{
    for (;;) {
        TPckgBuf<TInt> dataSize;
        User::LeaveIfError(SendReceive(GetRunningAppInfo, TIpcArgs(&dataSize)));        
        HBufC8* data = HBufC8::NewLC(dataSize());
        TPtr8 dataPointer(data->Des());
        TInt err = SendReceive(FlushData, TIpcArgs(&dataPointer, dataSize()));
        if (err == KErrNone) {
            return data;
        } else if (err != KErrCorrupt) {
            User::Leave(err);
        }
        CleanupStack::PopAndDestroy(data);
        // If result was KErrCorrupt then the size received from GetRunningAppInfo is
        // not valid anymore so restart the whole procedure.
    }
}

void RTsSession::Subscribe(TRequestStatus& aStatus)
{
    SendReceive(SubscribeRunningAppChanges, aStatus);
}

void RTsSession::CancelSubscribe()
{
    SendReceive(CancelSubscribeRunningAppChanges);
}

void RTsSession::OpenTask(const TDesC8 &key)
{
    SendReceive(OpenTaskMessage, TIpcArgs(&key));
}

void RTsSession::CloseTask(const TDesC8 &key)
{
    SendReceive(CloseTaskMessage, TIpcArgs(&key));
}

TInt RTsSession::RegisterScreenshot(CFbsBitmap *screenshot, TInt wgId, UpdatePriority priority)
{
   TRAPD(errNo, RegisterScreenshotL(screenshot, wgId, priority));
   return errNo;
}

TInt RTsSession::UnregisterScreenshot(TInt wgId)
{
    TRAPD(errNo, UnregisterScreenshotL(wgId));
   return errNo;
}

TInt RTsSession::SetVisibility(TInt wgId, Visibility value)
{
    TRAPD(errNo, SetVisibilityL(wgId, value));
    return errNo;
}

void RTsSession::RegisterScreenshotL(CFbsBitmap *screenshot, TInt wgId, UpdatePriority priority)
{
    CTsScreenshotMsg *message = CTsScreenshotMsg::NewLC(wgId, *screenshot, priority, 0);
    HBufC8* serializedData = message->ExternalizeLC();
    TIpcArgs arguments(serializedData);
    User::LeaveIfError(SendReceive(RegisterScreenshotMessage, arguments));
    CleanupStack::PopAndDestroy(serializedData);
    CleanupStack::PopAndDestroy(message);
}

void RTsSession::UnregisterScreenshotL(TInt wgId)
{
    CTsUnregisterScreenshotMsg *message = CTsUnregisterScreenshotMsg::NewLC(wgId);
    HBufC8* serializedData = message->ExternalizeLC();
    TIpcArgs arguments(serializedData);
    User::LeaveIfError(SendReceive(UnregisterScreenshotMessage, arguments));
    CleanupStack::PopAndDestroy(serializedData);
    CleanupStack::PopAndDestroy(message);
}

void RTsSession::SetVisibilityL(TInt wgId, Visibility value)
{
    CTsVisibilitMsg *message = CTsVisibilitMsg::NewLC(wgId, value);
    HBufC8* serializedData = message->ExternalizeLC();
    TIpcArgs arguments(serializedData);
    User::LeaveIfError(SendReceive(VisibilityChange, arguments));
    CleanupStack::PopAndDestroy(serializedData);
    CleanupStack::PopAndDestroy(message);
}
