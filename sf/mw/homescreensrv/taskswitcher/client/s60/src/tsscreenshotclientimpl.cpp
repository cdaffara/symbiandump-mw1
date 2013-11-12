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

#include <e32base.h>

#include "tsscreenshotclientimpl.h"

CTsScreenshotClientImpl* CTsScreenshotClientImpl::NewL()
{
    CTsScreenshotClientImpl* self = NewLC();
    CleanupStack::Pop(self);
    return self;
}

CTsScreenshotClientImpl* CTsScreenshotClientImpl::NewLC()
{
    CTsScreenshotClientImpl* self = new (ELeave) CTsScreenshotClientImpl;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}

CTsScreenshotClientImpl::CTsScreenshotClientImpl()
{
}

CTsScreenshotClientImpl::~CTsScreenshotClientImpl()
{
    iSession.Close();
}

void CTsScreenshotClientImpl::ConstructL()
{
    User::LeaveIfError(iSession.Connect());
}

TInt CTsScreenshotClientImpl::RegisterScreenshot(CFbsBitmap *screenshot, TInt wgId, UpdatePriority priority)
{
    return iSession.RegisterScreenshot(screenshot, wgId, priority);
}

TInt CTsScreenshotClientImpl::UnregisterScreenshot(TInt wgId)
{
    return iSession.UnregisterScreenshot(wgId);
}

TInt CTsScreenshotClientImpl::SetVisibility(TInt wgId, Visibility value)
{
    return iSession.SetVisibility(wgId, value);
}
