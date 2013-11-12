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
#include <afasyncrequestobserver.h>
#include <fbs.h>

TInt CAfStorageClient::constructorError(KErrNone);
TInt CAfStorageClient::expectedReturnCode(KErrNone);

CAfEntry *CAfStorageClient::lastCallEntry(0);
TInt CAfStorageClient::lastCallImageHandle(0);
TSize CAfStorageClient::lastCallThumbnailSize(0, 0);
HBufC *CAfStorageClient::lastCallThumbnailPath(0);
void *CAfStorageClient::lastCallUserData(0);
CAfStorageClient::LastMethodCalled CAfStorageClient::lastMethodCalled = CAfStorageClient::NoCall;
    
CAfStorageClient *CAfStorageClient::NewL(MAfAsyncRequestObserver &observer)
{
    User::LeaveIfError(CAfStorageClient::constructorError);
    return new CAfStorageClient(observer);
}

CAfStorageClient::~CAfStorageClient()
{
    clearLastCallInfo();
}

CAfStorageClient::CAfStorageClient(MAfAsyncRequestObserver &observer) : mObserver(observer)
{
}

int CAfStorageClient::saveActivity(const CAfEntry &entry, TInt imageHandle)
{
    lastMethodCalled = CAfStorageClient::SaveActivityMethod;
    updateLastEntry(entry);
    lastCallImageHandle = imageHandle;
    return CAfStorageClient::expectedReturnCode;
}

int CAfStorageClient::removeActivity(const CAfEntry &entry)
{
    lastMethodCalled = CAfStorageClient::RemoveActivityMethod;
    updateLastEntry(entry);
    return CAfStorageClient::expectedReturnCode;
}

int CAfStorageClient::removeApplicationActivities(const CAfEntry &entry)
{
    lastMethodCalled = CAfStorageClient::RemoveApplicationActivitiesMethod;
    updateLastEntry(entry);
    return CAfStorageClient::expectedReturnCode;
}

int CAfStorageClient::activities(RPointerArray<CAfEntry> &dst, int /*limit*/)
{
    if (lastMethodCalled == CAfStorageClient::SaveActivityMethod && CAfStorageClient::lastCallEntry) {
        dst.Append(CAfStorageClient::lastCallEntry);
        CAfStorageClient::lastCallEntry =0;
    }
    lastMethodCalled = CAfStorageClient::ActivitiesMethod;
    return CAfStorageClient::expectedReturnCode;
}

int CAfStorageClient::applicationActivities(RPointerArray<CAfEntry> &dst, const CAfEntry &entry)
{
    if (lastMethodCalled == CAfStorageClient::SaveActivityMethod && CAfStorageClient::lastCallEntry) {
        dst.Append(CAfStorageClient::lastCallEntry);
        CAfStorageClient::lastCallEntry =0;
    }
    lastMethodCalled = CAfStorageClient::ApplicationActivitiesMethod;
    updateLastEntry(entry);
    return CAfStorageClient::expectedReturnCode;
}

int CAfStorageClient::activityData(CAfEntry *&dst, const CAfEntry &entry)
{
    if (lastMethodCalled == CAfStorageClient::SaveActivityMethod && CAfStorageClient::lastCallEntry) {
        dst =CAfStorageClient::lastCallEntry;
        CAfStorageClient::lastCallEntry =0;
    }
    lastMethodCalled = CAfStorageClient::ActivityDataMethod;
    // @todo return something in dst
    updateLastEntry(entry);
    return CAfStorageClient::expectedReturnCode;
}

int CAfStorageClient::waitActivity()
{
    if(lastMethodCalled == CAfStorageClient::NoCall) {
        lastMethodCalled = CAfStorageClient::WaitActivityMethod;
        mObserver.waitActivityRequestCompleted(CAfStorageClient::expectedReturnCode, KNullDesC8);
    }
    
    lastMethodCalled = CAfStorageClient::WaitActivityMethod;
    return CAfStorageClient::expectedReturnCode;
}

int CAfStorageClient::launchActivity(const CAfEntry &entry)
{
    lastMethodCalled = CAfStorageClient::LaunchActivityMethod;
    updateLastEntry(entry);
    return CAfStorageClient::expectedReturnCode;
}

int CAfStorageClient::getThumbnail(const TDesC &imagePath, void *userData)
{
    lastMethodCalled = CAfStorageClient::GetThumbnailMethod;
    delete lastCallThumbnailPath;
    lastCallThumbnailPath = imagePath.Alloc();
    lastCallUserData = userData;
    CFbsBitmap *bitmap = new (ELeave)CFbsBitmap();
    CleanupStack::PushL(bitmap);
    User::LeaveIfError(bitmap->Create(TSize(128, 128), EColor4K));
    mObserver.getThumbnailRequestCompleted(CAfStorageClient::expectedReturnCode, bitmap->Handle(), userData);
    CleanupStack::PopAndDestroy(bitmap);
    return CAfStorageClient::expectedReturnCode;
}

int CAfStorageClient::notifyDataChange()
{
    if(lastMethodCalled == CAfStorageClient::NoCall) {
        lastMethodCalled = CAfStorageClient::NotifyDataChangeMethod;
        mObserver.dataChangeNotificationCompleted(CAfStorageClient::expectedReturnCode);
    }
    lastMethodCalled = CAfStorageClient::NotifyDataChangeMethod;
    return CAfStorageClient::expectedReturnCode;
}

// ---------------------------------------------------------------------------
//  Invoking callbacks
// ---------------------------------------------------------------------------
// 
void CAfStorageClient::invokeWaitActivityCallback(int result, const TDesC8 &data)
{
    mObserver.waitActivityRequestCompleted(result, data);
}

void CAfStorageClient::invokeGetThumbnailCallback(int result, int bitmapHandle, void *userData)
{
    mObserver.getThumbnailRequestCompleted(result, bitmapHandle, userData);
}

void CAfStorageClient::invokeDataChangeCallback(int result)
{
    mObserver.dataChangeNotificationCompleted(result);
}

// ---------------------------------------------------------------------------
//  Helper methods
// ---------------------------------------------------------------------------
// 
void CAfStorageClient::clearLastCallInfo()
{
    CAfStorageClient::constructorError = EFalse;
    CAfStorageClient::expectedReturnCode = KErrNone;

    delete CAfStorageClient::lastCallEntry;
    CAfStorageClient::lastCallEntry = 0;
    
    CAfStorageClient::lastCallImageHandle = 0;
    CAfStorageClient::lastCallThumbnailSize = TSize(0,0);
    
    delete CAfStorageClient::lastCallThumbnailPath;
    CAfStorageClient::lastCallThumbnailPath = 0;
    
    CAfStorageClient::lastCallUserData = 0;
    
    lastMethodCalled = CAfStorageClient::NoCall;
}

void CAfStorageClient::updateLastEntry(const CAfEntry &entry)
{
    delete CAfStorageClient::lastCallEntry;
    
    CAfStorageClient::lastCallEntry = CAfEntry::NewL(
        entry.Flags(),
        entry.ApplicationId(),
        entry.ActivityId(),
        entry.CustomActivityName(),
        entry.ImageSrc(),
        entry.Data(CAfEntry::Private),
        entry.Data(CAfEntry::Public),
        entry.Timestamp()
    );
}
