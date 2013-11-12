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


#ifndef OOMMEMORYMONITORSESSION_H
#define OOMMEMORYMONITORSESSION_H

#include <e32base.h>

class CMemoryMonitorServer;
class CMemoryMonitor;
class COomClientRequestQueue;

NONSHARABLE_CLASS(CMemoryMonitorSession) : public CSession2
    {
public:
    CMemoryMonitorSession();
#ifndef CLIENT_REQUEST_QUEUE
    void CloseAppsFinished(TInt aBytesFree, TBool aMemoryGood);
    TBool IsDataPaged(const RMessage2& aMessage);
#endif
    
private:
    ~CMemoryMonitorSession();
    CMemoryMonitorServer& Server();
#ifdef CLIENT_REQUEST_QUEUE
    COomClientRequestQueue& ClientRequestQueue();
#endif
    CMemoryMonitor& Monitor();
    void ServiceL(const RMessage2& aMessage);
private:
    RMessagePtr2 iRequestFreeRam;
#ifndef CLIENT_REQUEST_QUEUE
    TInt iFunction;
#endif    
    TInt iMinimumMemoryRequested;
    };

#endif /*OOMMEMORYMONITORSESSION_H*/
