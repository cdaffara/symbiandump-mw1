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

#include "afactivitylauncher.h"
#include "afactivitylauncher_p.h"

// -----------------------------------------------------------------------------
//  Public part of construction, destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CAfActivityLauncher *CAfActivityLauncher::NewL(RApaLsSession &apaLsSession, RWsSession &wsSession)
{
    CAfActivityLauncher *self = CAfActivityLauncher::NewLC(apaLsSession, wsSession);
    CleanupStack::Pop(self);
    return self;
}

EXPORT_C CAfActivityLauncher *CAfActivityLauncher::NewLC(RApaLsSession &apaLsSession, RWsSession &wsSession)
{
    CAfActivityLauncher *self = new (ELeave) CAfActivityLauncher();
    CleanupStack::PushL(self);
    self->ConstructL(apaLsSession, wsSession);
    return self;
}

EXPORT_C CAfActivityLauncher::~CAfActivityLauncher()
{
    delete mImplementation;
}

// -----------------------------------------------------------------------------
//  Private part of construction
// -----------------------------------------------------------------------------
//
CAfActivityLauncher::CAfActivityLauncher()
{
}

void CAfActivityLauncher::ConstructL(RApaLsSession &apaLsSession, RWsSession &wsSession)
{
    mImplementation = CAfActivityLauncherPrivate::NewL(apaLsSession, wsSession);
}

// -----------------------------------------------------------------------------
//  Actual activity launching
// -----------------------------------------------------------------------------
//

EXPORT_C void CAfActivityLauncher::launchActivityL(const TDesC &uriDesc)
{
    mImplementation->launchActivityL(uriDesc);
}
