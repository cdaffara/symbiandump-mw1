/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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


//INCLUDE
#include <e32def.h>
#include <e32cmn.h>

#include <mediakeysinternalpskeys.h>
#include <UsbWatcherInternalPSKeys.h>
#include "MMKeyBearerObserverPS.h"
#include "trace.h"

// ================= MEMBER FUNCTIONS =======================
//
// ----------------------------------------------------------
// CMMKeyBearerObserver::NewL()
// Constructs a new entry with given values.
// ----------------------------------------------------------
//
CMMKeyBearerObserver* CMMKeyBearerObserver::NewL(MCallBackReceiver& aCallback,
                                             TInt aKeyType)
    {
    FUNC_LOG;

    CMMKeyBearerObserver* self = new (ELeave) CMMKeyBearerObserver(aCallback);
    CleanupStack::PushL(self);
    self->ConstructL(aKeyType);
    CleanupStack::Pop();
    return self;
    }
// ----------------------------------------------------------
// CMMKeyBearerObserver::CMMKeyBearerObserver()
// C++ constructor
// ----------------------------------------------------------
//
CMMKeyBearerObserver::CMMKeyBearerObserver(MCallBackReceiver& aCallback)
    :  CActive(EPriorityStandard) ,iCallback(aCallback)
    {
    FUNC_LOG;

    //Pass
    }
// ----------------------------------------------------------
// CMMKeyBearerObserver::CMMKeyBearerObserver()
// Destructor
// ----------------------------------------------------------
//
CMMKeyBearerObserver::~CMMKeyBearerObserver()
    {
    FUNC_LOG;

    Cancel();
    }
// ----------------------------------------------------------
// CMMKeyBearerObserver::ConstructL()
// Symbian OS default constructor
// ----------------------------------------------------------
//
void CMMKeyBearerObserver::ConstructL(TInt aKeyType)
    {
    FUNC_LOG;

    iKeyType = aKeyType;

    // Add this active object to the scheduler.
    CActiveScheduler::Add(this);
    Start();
    }
// ----------------------------------------------------------
// CMMKeyBearerObserver::Start()
// Starts listening KUidCurrentCall event
// ----------------------------------------------------------
//
TInt CMMKeyBearerObserver::Start()
    {
    FUNC_LOG;

    if (IsActive())
        {
        ERROR( KErrInUse, "Start observing" );
        return KErrInUse;
        }

    iProperty.Close();
    iStatus = KRequestPending;
    if (iKeyType == ESideVolumeKeys)
        {
        iProperty.Attach(KPSUidMediaKeysEventNotifier,
                         KMediaKeysVolumeKeyEvent);
        }
    else if (iKeyType == EMediaKeys)
        {
        iProperty.Attach(KPSUidMediaKeysEventNotifier,
                         KMediaKeysControlKeyEvent);
        }
    else if (iKeyType == EAccessoryVolumeKeys)
        {
        iProperty.Attach(KPSUidMediaKeysEventNotifier,
                         KMediaKeysAccessoryVolumeEvent);
        }
    else if (iKeyType == EFileTransferStatus)
        {
        iProperty.Attach(KPSUidUsbWatcher,
                         KUsbWatcherSelectedPersonality);
        }
    iProperty.Subscribe(iStatus);

    SetActive();

    INFO_1( "Start observing: iKeyType = %d", iKeyType );

    return KErrNone;
    }
// ----------------------------------------------------------
// CMMKeyBearerObserver::Stop()
// Stops listening KUidCurrentCall event
// ----------------------------------------------------------
//
void CMMKeyBearerObserver::Stop()
    {
    FUNC_LOG;

    Cancel();
    }
// ----------------------------------------------------------
// CMMKeyBearerObserver::RunL()
//
// ----------------------------------------------------------
//
void CMMKeyBearerObserver::RunL()
    {
    FUNC_LOG;

    TInt scanCode;
    iProperty.Get( scanCode);

    ERROR_1( iStatus.Int(), "Observed key failed: keyType = %d", iKeyType );
    INFO_2( "Observed key: keyType = %d, scanCode = %d", iKeyType, scanCode );

    // If this Active Object is for receiving the USB MTP status, the
    // scanCode varibale contains the status whether transfer is
    // happening now(active) or not(not active).

    iCallback.ReceivedKeyEvent(scanCode, iKeyType);
    }
// ----------------------------------------------------------
// CMMKeyBearerObserver::DoCancel()
// Cancels event listening
// ----------------------------------------------------------
//
void CMMKeyBearerObserver::DoCancel()
    {
    FUNC_LOG;

    iProperty.Cancel();
    iProperty.Close();
    }

// ----------------------------------------------------------
// CMMKeyBearerObserver::GetValue()
//
// ----------------------------------------------------------
//
TInt CMMKeyBearerObserver::GetValue()
    {
    FUNC_LOG;

    TInt ret = 0;
    TInt err = iProperty.Get( ret );
    if ( err != KErrNone )
        {
        ERROR_1( err, "Observer value read failed: keyType = %d", iKeyType );
        }
    return ret;
    }

// End of file
