/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Main classes for Out of Memory Monitor.
*
*/



#include "oomoutofmemorywatcher.h"
#include "oommemorymonitor.h"
#include "OomTraces.h"
#include <u32hal.h>

// ======================================================================
// class COutOfMemoryWatcher
// - notifies when free memory crosses preset thresholds
// ======================================================================

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
COutOfMemoryWatcher* COutOfMemoryWatcher::NewL(CMemoryMonitor& aMonitor, TInt aLowRamThreshold, TInt aGoodRamThreshold, TBool aSwapUsageMonitored, TInt aLowSwapThreshold, TInt aGoodSwapThreshold)
    {
    FUNC_LOG;

    COutOfMemoryWatcher* self = new (ELeave) COutOfMemoryWatcher(aMonitor, aSwapUsageMonitored);
    CleanupStack::PushL(self);
    self->ConstructL(aLowRamThreshold, aGoodRamThreshold, aLowSwapThreshold, aGoodSwapThreshold);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
COutOfMemoryWatcher::~COutOfMemoryWatcher()
    {
    FUNC_LOG;

    Cancel();
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
COutOfMemoryWatcher::COutOfMemoryWatcher(CMemoryMonitor& aMonitor, TBool aSwapUsageMonitored)
:   CActive(CActive::EPriorityStandard),
    iMemoryMonitor(aMonitor),
    iSwapUsageMonitored(aSwapUsageMonitored)
    {
    FUNC_LOG;

    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfMemoryWatcher::ConstructL(TInt aLowRamThreshold, TInt aGoodRamThreshold, TInt aLowSwapThreshold, TInt aGoodSwapThreshold)
    {
    FUNC_LOG;

    UserSvr::SetMemoryThresholds(aLowRamThreshold,aGoodRamThreshold);
    if (iSwapUsageMonitored)
        {
        SVMSwapThresholds thresholds;
        thresholds.iLowThreshold = aLowSwapThreshold;
        thresholds.iGoodThreshold = aGoodSwapThreshold;
        UserSvr::HalFunction(EHalGroupVM, EVMHalSetSwapThresholds, &thresholds, 0);
        }
    User::LeaveIfError(iChangeNotifier.Create());
    }

void COutOfMemoryWatcher::UpdateThresholds(TInt aLowRamThreshold, TInt aGoodRamThreshold, TInt aLowSwapThreshold, TInt aGoodSwapThreshold)
    {
    FUNC_LOG;

    UserSvr::SetMemoryThresholds(aLowRamThreshold,aGoodRamThreshold);
    if (iSwapUsageMonitored)
        {
        SVMSwapThresholds thresholds;
        thresholds.iLowThreshold = aLowSwapThreshold;
        thresholds.iGoodThreshold = aGoodSwapThreshold;
        UserSvr::HalFunction(EHalGroupVM, EVMHalSetSwapThresholds, &thresholds, 0);
        }
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfMemoryWatcher::Start()
    {
    FUNC_LOG;

    if (!IsActive())
        {
        iChangeNotifier.Logon(iStatus);
        SetActive();
        }
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfMemoryWatcher::DoCancel()
    {
    FUNC_LOG;

    iChangeNotifier.LogonCancel();
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfMemoryWatcher::RunL()
    {
    FUNC_LOG;

    TInt status = iStatus.Int();

    if (status < 0)
        {
        User::Leave(status);
        }

    // Check for memory status change.
    if (status & EChangesFreeMemory)
        {
        iMemoryMonitor.FreeMemThresholdCrossedL();
        }

    // We are not active until FreeMemThresholdCrossedL returns.
    Start();
    }
