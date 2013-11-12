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
#ifndef TSTASKMONITORCLIENTIMPL_H
#define TSTASKMONITORCLIENTIMPL_H

#include <e32base.h>

#include "tssession.h"

class MTsTaskMonitorObserver;

NONSHARABLE_CLASS( CTsTaskMonitorClientImpl ) : public CActive
{
public:
    static CTsTaskMonitorClientImpl* NewL();
    static CTsTaskMonitorClientImpl* NewLC();
    ~CTsTaskMonitorClientImpl();

public:
    HBufC8* TasksContentLC();
    void Subscribe(MTsTaskMonitorObserver& aObserver);
    void CancelSubscribe();
    void OpenTask(const TDesC8 &key);
    void CloseTask(const TDesC8 &key);

private:
    void RunL();
    void DoCancel();

private:
    CTsTaskMonitorClientImpl();
    void ConstructL();

    RTsSession iSession;
    MTsTaskMonitorObserver* iObserver; // not owned
};

#endif  // TSTASKMONITORCLIENTIMPL_H
