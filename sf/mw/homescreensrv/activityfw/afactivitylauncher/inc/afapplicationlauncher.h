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
#ifndef AFAPPLICATIONLAUNCHER_H
#define AFAPPLICATIONLAUNCHER_H

#include <e32base.h>
#include <apgcli.h>
#include <w32std.h>

class TAfApplicationLauncher
{
public:
    TAfApplicationLauncher(RApaLsSession &apaLsSession, RWsSession &wsSession);

public:
    TBool isRunning(TUid applicationId);
    void startApplicationL(TUid applicationId, const TDesC &uri);

private:
    RApaLsSession &mApaLsSession;
    RWsSession &mWsSession;

};

#endif // AFAPPLICATIONLAUNCHER_H
