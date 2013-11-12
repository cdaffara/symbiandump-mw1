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

#include "afdatabasecleaner.h"

CAfDatabaseCleaner::~CAfDatabaseCleaner()
{
    Cancel();
}

CAfDatabaseCleaner::CAfDatabaseCleaner(RDbStoreDatabase &database) : CActive(EPriorityIdle), mDatabase(database), mCleanupStep(Initial)
{
    CActiveScheduler::Add(this);
}

void CAfDatabaseCleaner::StartCleanup()
{
    if (!IsActive()) {
        mCleanupStep = Initial;
        SetActive();
        CompleteSelf(KErrNone);
    }
}

void CAfDatabaseCleaner::RunL()
{
    if (KErrNone == iStatus.Int()) {
        switch (mCleanupStep) {
            case Initial:
            {
                User::LeaveIfError(mIncrementalInterface.Compact(mDatabase, mStep));
                mCleanupStep = Continuation;
                SetActive();
                CompleteSelf(KErrNone);
                break;
            }
            case Continuation:
            {
                User::LeaveIfError(mIncrementalInterface.Next(mStep));
                if (mStep != 0) {
                    SetActive();
                    CompleteSelf(KErrNone);
                } else {
                    mIncrementalInterface.Close();
                }
                break;
            }
            default:
                User::Leave(KErrArgument);
        }
    }
}

void CAfDatabaseCleaner::DoCancel()
{
    mIncrementalInterface.Close();
}

void CAfDatabaseCleaner::CompleteSelf(TInt completionCode)
{
    TRequestStatus* status = &iStatus;
    User::RequestComplete(status, completionCode);
}

TInt CAfDatabaseCleaner::RunError(TInt /*aError*/)
{
    mIncrementalInterface.Close();
    return KErrNone;
}
