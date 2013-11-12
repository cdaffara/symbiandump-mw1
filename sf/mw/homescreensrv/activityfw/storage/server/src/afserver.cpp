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
#include <fbs.h>

#include "afserver.h"
#include "afsession.h"
#include "afstorage.h"
#include "aftask.h"
#include "afapplicationsengine.h"
#include "afcmd.h"

_LIT( KActivityServerName, "hsactivitydbserver" );
_LIT(KObserverAlreadyExists, "Observer task exists");



const TInt KSecureServerRanges[] =
    {
    SaveActivity,
    Activities,
    ApplicationActivities, 
    CancelNotify + 1
    };

const TUint8 KSecureServerElementsIndex[] =
    {
    0,
    1,
    CPolicyServer::EAlwaysPass,
    CPolicyServer::ENotSupported
    };

const TInt KSecureServerRangeCount(sizeof(KSecureServerRanges) / sizeof(TInt));

const CPolicyServer::TPolicyElement KSecureServerElements[] =
    {
    {_INIT_SECURITY_POLICY_C1(ECapabilityWriteDeviceData), CPolicyServer::EFailClient},
    {_INIT_SECURITY_POLICY_C1(ECapabilityReadDeviceData ), CPolicyServer::EFailClient}
    };

const CPolicyServer::TPolicy KSecureServerPolicy =
    {
    CPolicyServer::EAlwaysPass,
    KSecureServerRangeCount,
    KSecureServerRanges,
    KSecureServerElementsIndex,
    KSecureServerElements
   };
// -----------------------------------------------------------------------------
/**
 * Constructor for performing 1st stage construction
 */
CAfServer::CAfServer()
:
CPolicyServer( EPriorityStandard, KSecureServerPolicy )
{
    // No implementation required
}

// -----------------------------------------------------------------------------
/**
 * Destructor.
 */
CAfServer::~CAfServer()
{
    delete mAppEngine;
    delete mStorage;
    mFsSession.Close();
    mApaSession.Close();
    mObservers.ResetAndDestroy();
    RFbsSession::Disconnect();
}

// -----------------------------------------------------------------------------
/**
 * Two-phased constructor.
 */
CAfServer* CAfServer::NewLC()
{
    CAfServer* self = new (ELeave) CAfServer();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}

// -----------------------------------------------------------------------------
/**
 * Default constructor for performing 2nd stage construction
 */
void CAfServer::ConstructL()
{
    StartL(KActivityServerName);
    User::LeaveIfError(mApaSession.Connect());
    User::LeaveIfError(mFsSession.Connect());
    User::LeaveIfError(RFbsSession::Connect(mFsSession));
    mStorage = CAfStorage::NewL(mFsSession);
    mAppEngine = CAfApplicationsEngine::NewL(mApaSession, *mStorage, *this);
}

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see CServer2::NewSessionL(const TVersion&, const RMessage2&)
 */
CSession2* CAfServer::NewSessionL(const TVersion &, const RMessage2&) const
{
    return CAfSession::NewL(const_cast<CAfServer*>(this)->mFsSession, 
                            *const_cast<CAfServer*>(this), 
                            *mStorage);
}

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MAfTaskStorage::PushL(CAfTask *)
 */
void CAfServer::PushL(CAfTask * task)
{
    (KErrNotFound == mObservers.Find(task)) ?
     mObservers.AppendL(task) :
     User::Panic(KObserverAlreadyExists, KErrAlreadyExists);
}

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MAfTaskStorage::Pop(CActivityTask *)
 */
void CAfServer::Pop(CAfTask *task)
{
    const TInt offset(mObservers.Find(task));
    if (KErrNotFound != offset) {
        mObservers.Remove(offset);
    }
}

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MAfTaskStorage::StorageData()
 */
const RPointerArray<CAfTask>& CAfServer::StorageData() const
{
    return mObservers;
}

// -----------------------------------------------------------------------------
/**
 * Removes not valid task
 */
void CAfServer::RemoveNotValidTasks(const CSession2* session)
{
    for (TInt i=mObservers.Count()-1; i>=0; --i) {
        if( mObservers[i]->IsSessionTask(session) ) {
            delete mObservers[i];
            mObservers.Remove(i);
        }
    }
}
