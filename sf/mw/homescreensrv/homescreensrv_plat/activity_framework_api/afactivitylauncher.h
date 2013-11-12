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
#ifndef AFACTIVITYLAUNCHER_H
#define AFACTIVITYLAUNCHER_H

#include <e32base.h>

class RWsSession;
class RApaLsSession;
class CAfActivityLauncherPrivate;

class CAfActivityLauncher : public CBase
{

public:
    IMPORT_C static CAfActivityLauncher *NewL(RApaLsSession &apaLsSession, RWsSession &wsSession);
    IMPORT_C static CAfActivityLauncher *NewLC(RApaLsSession &apaLsSession, RWsSession &wsSession);
    IMPORT_C ~CAfActivityLauncher();
    IMPORT_C void launchActivityL(const TDesC &uriDesc);

private:
    CAfActivityLauncher();
    void ConstructL(RApaLsSession &apaLsSession, RWsSession &wsSession);

private:
    CAfActivityLauncherPrivate *mImplementation;

};

#endif // AFACTIVITYLAUNCHER_H
