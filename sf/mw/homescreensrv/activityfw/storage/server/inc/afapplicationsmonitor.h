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
#ifndef AFAPPLICATIONMONITOR_H
#define AFAPPLICATIONMONITOR_H

#include <e32base.h>
#include <apgcli.h>

#include "afapplicationsobserver.h"

class CAfApplicationsMonitor: public CActive
{
public:
    static CAfApplicationsMonitor* NewL(RApaLsSession& serviceProvider, 
                                        MAfApplicationsObserver& observer);
    ~CAfApplicationsMonitor();
private:
    CAfApplicationsMonitor(RApaLsSession& serviceProvider, 
                          MAfApplicationsObserver& observer);
    
    void RunL();
    void Subscribe();
    TInt RunError(TInt error);
    void DoCancel();

private:
    RApaLsSession& mServiceProvider;
    MAfApplicationsObserver &mObserver;
};

#endif // AFAPPLICATIONMONITOR_H
