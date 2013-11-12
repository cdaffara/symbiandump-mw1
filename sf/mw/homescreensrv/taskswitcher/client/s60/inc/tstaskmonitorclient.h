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
#ifndef TSTASKMONITORCLIENT_H
#define TSTASKMONITORCLIENT_H

#include <e32base.h>

class MTsTaskMonitorObserver;
class CTsTaskMonitorClientImpl;

NONSHARABLE_CLASS( CTsTaskMonitorClient ) : public CBase
{
public:
    static CTsTaskMonitorClient* NewL();
    static CTsTaskMonitorClient* NewLC();
    ~CTsTaskMonitorClient();

public:
    HBufC8* TasksContentLC();
    void Subscribe(MTsTaskMonitorObserver& aObserver);
    void CancelSubscribe();
    void OpenTask(const TDesC8 &key);
    void CloseTask(const TDesC8 &key);

private:
    CTsTaskMonitorClient();
    void ConstructL();

private:
    CTsTaskMonitorClientImpl* iImpl;
};

#endif  // TSTASKMONITORCLIENT_H
