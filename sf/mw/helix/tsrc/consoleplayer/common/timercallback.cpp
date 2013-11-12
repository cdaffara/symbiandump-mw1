/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Source code for handling the timer.
 * 
 */

#include "timercallback.h"

CTimerCallback::CTimerCallback( ITimerCallbackClient& aClient ) : 
    CActive( EPriorityStandard ),
    iClient( aClient )
    {    
    CActiveScheduler::Add(this);
    }

CTimerCallback::~CTimerCallback()
    { 
    Cancel();
    }

void CTimerCallback::Start(TTimeIntervalMicroSeconds32 aInterval)
    {
    iInterval = aInterval;
    
    if( iTimer.CreateLocal() == KErrNone )
        {
        SetActive();
        iTimer.After( iStatus, 0 );
        }
    }

// inherited from CActive

void CTimerCallback::RunL()
    {
    SetActive();
    iTimer.After( iStatus, iInterval );

    iClient.TimerCallback();        
    }

void CTimerCallback::DoCancel()
    {
    iTimer.Cancel();
    }
    

