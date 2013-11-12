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

#ifndef AFSTORAGECLIENT_H
#define AFSTORAGECLIENT_H

#include <e32base.h>

class CAfEntry;
class MAfAsyncRequestObserver;

class CAfStorageClient : public CBase
{
public:
    static CAfStorageClient *NewL(MAfAsyncRequestObserver &observer);
    virtual ~CAfStorageClient();

private:
    CAfStorageClient(MAfAsyncRequestObserver &observer);
    
public:    
    int saveActivity(const CAfEntry &entry, TInt imageHandle);
    int removeActivity(const CAfEntry &entry);
    int removeApplicationActivities(const CAfEntry &entry);
    int activities(RPointerArray<CAfEntry> &dst, int limit = 0);
    int applicationActivities(RPointerArray<CAfEntry> &dst, const CAfEntry &entry);
    int activityData(CAfEntry *&dst, const CAfEntry &entry);
    int waitActivity();
    int launchActivity(const CAfEntry &entry);
    int getThumbnail(const TDesC &imagePath, void *userData);
    int notifyDataChange();

private:
    MAfAsyncRequestObserver &mObserver;
    
private:
    void updateLastEntry(const CAfEntry &entry);
    
public:
    void invokeWaitActivityCallback(int result, const TDesC8 &data);
    void invokeGetThumbnailCallback(int result, int bitmapHandle, void *userData);
    void invokeDataChangeCallback(int result);
    
    static void clearLastCallInfo();
    
public:
    enum LastMethodCalled {
        NoCall,
        SaveActivityMethod,
        RemoveActivityMethod,
        RemoveApplicationActivitiesMethod,
        ActivitiesMethod,
        ApplicationActivitiesMethod,
        ActivityDataMethod,
        WaitActivityMethod,
        LaunchActivityMethod,
        GetThumbnailMethod,
        NotifyDataChangeMethod,
    };
    
public:
    static TInt constructorError;
    static TInt expectedReturnCode;

    static CAfEntry *lastCallEntry;
    static TInt lastCallImageHandle;
    static TSize lastCallThumbnailSize;
    static HBufC *lastCallThumbnailPath;
    static void *lastCallUserData;
    
    static LastMethodCalled lastMethodCalled;
};

#endif //AFSTORAGECLIENT_H
