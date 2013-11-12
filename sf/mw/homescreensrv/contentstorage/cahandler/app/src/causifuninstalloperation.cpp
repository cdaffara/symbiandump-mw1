/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  ?Description
 *
 */

#include <usif/usifcommon.h>
#include "cautils.h"
#include "cadef.h"
#include "cainnerentry.h"
#include "causifuninstalloperation.h"
#include "cauninstallerobserver.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CCaUsifUninstallOperation::~CCaUsifUninstallOperation
// ---------------------------------------------------------------------------
//
CCaUsifUninstallOperation::~CCaUsifUninstallOperation()
    {
    Cancel();
    delete iArguments;
    delete iResults;
    iUninstaller.Close();
    }

// ---------------------------------------------------------------------------
// CCaUsifUninstallOperation::NewL
// ---------------------------------------------------------------------------
//
CCaUsifUninstallOperation *CCaUsifUninstallOperation::NewL(
        TComponentId aComponentId, TInt aPriority)
    {
    CCaUsifUninstallOperation *self = new (ELeave) CCaUsifUninstallOperation(
            aPriority);
    CleanupStack::PushL(self);
    self->ConstructL(aComponentId);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CCaUsifUninstallOperation::CCaUsifUninstallOperation
// ---------------------------------------------------------------------------
//
CCaUsifUninstallOperation::CCaUsifUninstallOperation( TInt aPriority ) :
    CActive(aPriority), iUninstaller(), iCaUninstallerObserver(NULL)
    {
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// CCaUsifUninstallOperation::ConstructL
// ---------------------------------------------------------------------------
//
void CCaUsifUninstallOperation::ConstructL(TComponentId aComponentId)
    {
    // Silent uninstallation request
    User::LeaveIfError(iUninstaller.Connect());
    iArguments = COpaqueNamedParams::NewL();
    iResults = COpaqueNamedParams::NewL();
    iArguments->AddIntL(KSifInParam_InstallSilently, 1);
    iUninstaller.Uninstall(aComponentId, *iArguments, *iResults, iStatus);
    SetActive();
    }

// ---------------------------------------------------------------------------
// CCaUsifUninstallOperation::RunL
// ---------------------------------------------------------------------------
//
void CCaUsifUninstallOperation::RunL()
    {
    if( iStatus != KErrNone )
        {
        int error = iStatus.Int();
        // notify about error
        if( iCaUninstallerObserver )
            {
            iCaUninstallerObserver->uninstallError( error );
            }
        }
    iUninstaller.Close();
    }

// ---------------------------------------------------------------------------
// CCaUsifUninstallOperation::DoCancel
// ---------------------------------------------------------------------------
//
void CCaUsifUninstallOperation::DoCancel()
    {
    iUninstaller.CancelOperation();
    }

#ifdef COVERAGE_MEASUREMENT
#pragma CTC SKIP
#endif //COVERAGE_MEASUREMENT (error ignored)
// ---------------------------------------------------------------------------
// CCaUsifUninstallOperation::RunError
// ---------------------------------------------------------------------------
//
TInt CCaUsifUninstallOperation::RunError(TInt /* aError */)
    {
    return KErrNone;
    }
#ifdef COVERAGE_MEASUREMENT
#pragma CTC ENDSKIP
#endif //COVERAGE_MEASUREMENT

// ---------------------------------------------------------------------------
// CCaUsifUninstallOperation::AddObserver
// ---------------------------------------------------------------------------
//
void CCaUsifUninstallOperation::AddObserver(CaUninstallerObserver* aCaUninstallerObserver)
    {
    iCaUninstallerObserver = aCaUninstallerObserver;
    }

