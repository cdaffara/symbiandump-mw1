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
#ifndef AFACTIVITYLAUNCHERPRIVATE_H
#define AFACTIVITYLAUNCHERPRIVATE_H

#include <e32base.h>

#include <afasyncrequestobserver.h>
#include "afapplicationlauncher.h"

class CAfStorageClient;

class CAfActivityLauncherPrivate : public CBase, public MAfAsyncRequestObserver
{

public:
    static CAfActivityLauncherPrivate *NewL(RApaLsSession &apaLsSession, RWsSession &wsSession);
    ~CAfActivityLauncherPrivate();

private:
    CAfActivityLauncherPrivate(RApaLsSession &apaLsSession, RWsSession &wsSession);
    void ConstructL();
    
public: // from MAfAsyncRequestObserver
    virtual void waitActivityRequestCompleted(int result, const TDesC8 &data);
    virtual void getThumbnailRequestCompleted(int result, int bitmapHandle, void *userData);
    virtual void dataChangeNotificationCompleted(int result);    
    
public:
    void launchActivityL(const TDesC &uriDesc);
    
private:
    CAfStorageClient *mActivitiesClient;
    TAfApplicationLauncher mApplicationLauncher;
    
};

#endif // AFACTIVITYLAUNCHERPRIVATE_H
