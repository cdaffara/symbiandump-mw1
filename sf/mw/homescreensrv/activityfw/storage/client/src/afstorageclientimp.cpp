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
#include "afstorageclientimp.h"

#include <fbs.h>
#include <s32mem.h>

#include "afstorageglobals.h"
#include "afglobals.h"
#include "afentry.h"
#include "afasyncrequest_p.h"
#include "afthumbnailrequest_p.h"

// -----------------------------------------------------------------------------
/**
 * Furst phase constructor 
 */
RAfStorageClientImplementation::RAfStorageClientImplementation(MAfAsyncRequestObserver &observer)
:
mObserver(observer)
{}

// -----------------------------------------------------------------------------
/**
 * Destructor
 */
RAfStorageClientImplementation::~RAfStorageClientImplementation()
{
    Close();
}

// -----------------------------------------------------------------------------
void RAfStorageClientImplementation::Close()
{
    mAsyncTasks.ResetAndDestroy();
    RSessionBase::Close();
}

// -----------------------------------------------------------------------------
/**
 * Function establish connection to activity server
  */
void RAfStorageClientImplementation::connectL()
{
    const int asyncMessageSlots(12);
    const int maxRetry(4);
    TInt retry = maxRetry;
    TInt errNo(KErrNone);
    do {
        errNo = CreateSession(KActivityServerName, 
                              TVersion(0, 0, 0), 
                              asyncMessageSlots);
        if (KErrNotFound != errNo && KErrServerTerminated != errNo) {
            retry =0;
        } else {
            TRAP(errNo, startServerL());
            if (KErrNone != errNo && KErrAlreadyExists != errNo) {
                retry =0;
                errNo = CreateSession(KActivityServerName, 
                                      TVersion(0, 0, 0), 
                                      asyncMessageSlots);
            }
        }
    } while (--retry > 0);
    User::LeaveIfError(errNo);
}

// -----------------------------------------------------------------------------
void RAfStorageClientImplementation::startServerL()
{
    RProcess server;
    const TUidType uid(KNullUid, KNullUid, KActivityServerUid);
    User::LeaveIfError(server.Create(KActivityServerName, KNullDesC, uid));
    TRequestStatus stat;
    server.Rendezvous(stat);
    if (stat != KRequestPending) {
        server.Kill(0);
    } else {
        server.Resume();
    }
    User::WaitForRequest(stat);
    int errNo = (EExitPanic == server.ExitType()) ?
                KErrGeneral : stat.Int();
    if (KErrCancel == errNo) {
        errNo = KErrNone;
    }
    server.Close();
    User::LeaveIfError(errNo);
}

// -----------------------------------------------------------------------------
void RAfStorageClientImplementation::executeL(int function)
{
    switch (function) {
    case WaitActivity:
    case NotifyChange:
        CAfAsyncRequestPrivate::NewLD(mObserver, *this, static_cast<ActivityCmd>(function));
        break;
    default:
        User::LeaveIfError(SendReceive(function, TIpcArgs()));
    }
}

// -----------------------------------------------------------------------------
void RAfStorageClientImplementation::executeL(int function, 
                                              const CAfEntry &entry,
                                              int userData)
{
    TPckgBuf<TInt> data(userData);
    RBuf8 serializedEntry;
    CleanupClosePushL(serializedEntry);
    serializedEntry.CreateL(entry.Size());
    RDesWriteStream stream(serializedEntry);
    CleanupClosePushL(stream);
    stream << entry;
    CleanupStack::PopAndDestroy(&stream);
    User::LeaveIfError(SendReceive(function, TIpcArgs(&serializedEntry, &data)));
    CleanupStack::PopAndDestroy(&serializedEntry);
}

// -----------------------------------------------------------------------------
void RAfStorageClientImplementation::executeL(int function, RPointerArray<CAfEntry> &resultsList, const CAfEntry& templateEntry, TInt limit)
{
    resultsList.ResetAndDestroy();
    RBuf8 buffer;
    CleanupClosePushL(buffer);
    CAfEntry::ReallocL(buffer, templateEntry.Size());
    RDesWriteStream writer(buffer);
    CleanupClosePushL(writer);
    writer << templateEntry;
    CleanupStack::PopAndDestroy(&writer);
    TPckgBuf<int> length(0), taskId(0);
    User::LeaveIfError(SendReceive(function,
                                   TIpcArgs(&buffer, &length, &taskId, limit)));
    CAfEntry::ReallocL(buffer, length());
    User::LeaveIfError(SendReceive(GetData, TIpcArgs(&taskId, &buffer)));
    resultsList << buffer;
    CleanupStack::PopAndDestroy(&buffer);
}

// -----------------------------------------------------------------------------
void RAfStorageClientImplementation::getThumbnailL(const TDesC& source, 
                                                   TAny *userData )
{
    CAfThumbnailRequestPrivate::NewLD(mObserver, *this, source, userData);
}

// -----------------------------------------------------------------------------
void RAfStorageClientImplementation::PushL(CBase* asyncTask)
{
    const TInt offset(mAsyncTasks.Find(asyncTask));
    (KErrNotFound == offset) ? mAsyncTasks.AppendL(asyncTask) : User::Leave(KErrAlreadyExists);
}

// -----------------------------------------------------------------------------
void RAfStorageClientImplementation::Pop(CBase* asyncTask)
{
    const TInt offset(mAsyncTasks.Find(asyncTask));
    if (KErrNotFound != offset) {
        mAsyncTasks.Remove(offset);
    }
}

// -----------------------------------------------------------------------------
void RAfStorageClientImplementation::sendAsync(int function, 
                                              const TIpcArgs& args, 
                                              TRequestStatus& status)
{
    SendReceive(function, args, status);
}

// -----------------------------------------------------------------------------
void RAfStorageClientImplementation::getDataL(int id, TDes8& buffer)
{
    TPckgBuf<int> requestId(id);
    SendReceive(GetData, TIpcArgs(&requestId, &buffer));
}
