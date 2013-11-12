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


#ifndef OOMMEMORYMONITORSERVER_H
#define OOMMEMORYMONITORSERVER_H

#include <e32base.h>

class CMemoryMonitor;
class COomClientRequestQueue;

NONSHARABLE_CLASS(CMemoryMonitorServer) : public CServer2
    {
public:
#ifdef CLIENT_REQUEST_QUEUE
    static CMemoryMonitorServer* NewL(COomClientRequestQueue& aQueue);
#else
    static CMemoryMonitorServer* NewL(CMemoryMonitor& aMonitor);
#endif    
    ~CMemoryMonitorServer();

    CMemoryMonitor& Monitor();
#ifndef CLIENT_REQUEST_QUEUE
    void CloseAppsFinished(TInt aBytesFree, TBool aMemoryGood);
#endif
#ifdef CLIENT_REQUEST_QUEUE
    COomClientRequestQueue& ClientRequestQueue();
#endif
    
private:
#ifdef CLIENT_REQUEST_QUEUE
    CMemoryMonitorServer(COomClientRequestQueue& aQueue);
#else
    CMemoryMonitorServer(CMemoryMonitor& aMonitor);
#endif
    void ConstructL();
    CSession2* NewSessionL(const TVersion& aVersion,const RMessage2& aMessage) const;
    TInt RunError(TInt aError);

private:
#ifdef CLIENT_REQUEST_QUEUE
    COomClientRequestQueue& iQueue;
#else
    CMemoryMonitor& iMonitor;
#endif
    };

#endif /*OOMMEMORYMONITORSERVER_H*/
