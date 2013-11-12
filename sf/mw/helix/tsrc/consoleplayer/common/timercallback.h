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
 * Header specifying the timer handling.
 * 
 */

#ifndef __TIMER_CALLBACK_H__
#define __TIMER_CALLBACK_H__

#include <e32base.h>

class ITimerCallbackClient
    {
public:
    
    virtual void TimerCallback() = 0;
    
    };

class CTimerCallback : public CActive
    {
public:
    
    CTimerCallback( ITimerCallbackClient& aClient ); 
    
    ~CTimerCallback();
    
    void Start( TTimeIntervalMicroSeconds32 aInterval );
    
    // inherited from CActive
    void RunL();
    void DoCancel();
    
private:
        
    RTimer                      iTimer;
    ITimerCallbackClient&       iClient;
    TTimeIntervalMicroSeconds32 iInterval;
    
    };

#endif
