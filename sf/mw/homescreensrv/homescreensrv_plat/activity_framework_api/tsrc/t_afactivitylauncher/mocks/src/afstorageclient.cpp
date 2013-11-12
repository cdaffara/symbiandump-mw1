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

#include "afstorageclient.h"

#include <afentry.h>

TInt CAfStorageClient::lastLaunchAppId = 0;
HBufC* CAfStorageClient::lastLaunchUri = 0;

CAfStorageClient *CAfStorageClient::NewL(MAfAsyncRequestObserver &observer)
{
    CAfStorageClient *self = CAfStorageClient::NewLC(observer);
    CleanupStack::Pop(self);
    return self;
}

CAfStorageClient *CAfStorageClient::NewLC(MAfAsyncRequestObserver &/*observer*/)
{
    CAfStorageClient *self = new (ELeave) CAfStorageClient();
    CleanupStack::PushL(self);
    return self;
}

CAfStorageClient::~CAfStorageClient()
{
    delete lastLaunchUri;
    lastLaunchUri = 0;
}

CAfStorageClient::CAfStorageClient()
{
}

int CAfStorageClient::launchActivity(const CAfEntry &entry)
{
    lastLaunchAppId = entry.ApplicationId();
    lastLaunchUri = entry.ActivityId().Alloc();
    
    return KErrNone;
}
