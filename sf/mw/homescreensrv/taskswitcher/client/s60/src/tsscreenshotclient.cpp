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

#include <s32mem.h>

#include "tsscreenshotclient.h"
#include "tsscreenshotclientimpl.h"

EXPORT_C CTsScreenshotClient *CTsScreenshotClient::NewL()
{
    CTsScreenshotClient* self = NewLC();
    CleanupStack::Pop(self);
    return self;
}

EXPORT_C CTsScreenshotClient *CTsScreenshotClient::NewLC()
{
    CTsScreenshotClient* self = new (ELeave) CTsScreenshotClient;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}

void CTsScreenshotClient::ConstructL()
{
    iImpl = CTsScreenshotClientImpl::NewL();
}

CTsScreenshotClient::CTsScreenshotClient()
{
}

EXPORT_C CTsScreenshotClient::~CTsScreenshotClient()
{
    delete iImpl;
}

EXPORT_C TInt CTsScreenshotClient::RegisterScreenshot(CFbsBitmap *screenshot, TInt wgId, UpdatePriority priority)
{
    return iImpl->RegisterScreenshot(screenshot, wgId, priority);
}

EXPORT_C TInt CTsScreenshotClient::UnregisterScreenshot(TInt wgId)
{
    return iImpl->UnregisterScreenshot(wgId);
}

EXPORT_C TInt CTsScreenshotClient::SetVisibility(TInt wgId, Visibility value)
{
    return iImpl->SetVisibility(wgId, value);
}
