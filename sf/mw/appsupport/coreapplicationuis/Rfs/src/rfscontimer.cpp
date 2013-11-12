/*
* Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
*		Implements CRFsScript class which controls the parsing of the Rfs scripts
*
*/

#include <e32debug.h>

#include "rfscontimer.h"
#include "RfsTraces.h"

const TInt KRfsConnectionTimeOut = 60000000;

CRfsConTimer :: CRfsConTimer(CRfsConnectionObserver * aRfsConnectionObserver):CActive( EPriorityStandard )
    {
    iRfsConnectionObserver = aRfsConnectionObserver;
    iTimer.CreateLocal();
	CActiveScheduler::Add( this );
    }

CRfsConTimer :: ~CRfsConTimer()
    {
    TRACES("CRfsConTimer :: ~CRfsConTimer()");
    
    iRfsConnectionObserver = NULL;
    Cancel();        
    iTimer.Close();        
    }

CRfsConTimer*  CRfsConTimer::NewL(CRfsConnectionObserver * aRfsConnectionObserver)
    {
    TRACES("CRfsConTimer :: NewL()");
    
    CRfsConTimer* self = new (ELeave) CRfsConTimer(aRfsConnectionObserver );
    return self;
    }

void CRfsConTimer :: IssueTimerRequest()
    {
    TRACES("CRfsConTimer :: IssueTimerRequest()");
    
    if(!IsActive())
        {
        iTimer.After(iStatus, KRfsConnectionTimeOut);
        SetActive();
        }
    }
void CRfsConTimer :: RunL()
    {
    TRACES("CRfsConTimer :: RunL()");
	
    if(iRfsConnectionObserver->iState == ESipConnectionClose  && iRfsConnectionObserver->iIsPDPFeatureEnabled)
        {
        // If SIP doesn't respond with in the set time, RunL will be called,
        // Rfs cancels the outstanding SIP request and If PDP connection exists,
        // Rfs will inform the PDP and start the timer.
    
        iRfsConnectionObserver->Cancel();            
        iRfsConnectionObserver->iState = EPdpConnectionClose;
        iRfsConnectionObserver->Subscribe();
        iTimer.After(iStatus, KRfsConnectionTimeOut);    
        SetActive();
        }
    else
        {
        iRfsConnectionObserver->Cancel();
        }        
    }

void CRfsConTimer :: DoCancel()
    {
    TRACES("CRfsConTimer :: DoCancel()");
    
    if(IsActive())
        {
        iTimer.Cancel();
        }
    }
