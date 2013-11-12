/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  Task list entry
 *
 */

#define __E32SVR_H__

#include <s32strm.h>
#include <fbs.h>

#include "tsentry.h"
#include "tsdataobserver.h"
#include "tsthumbnailprovider.h"

// --------------------------------------------------------------------------
// CTsFswEntry::NewL
// --------------------------------------------------------------------------
//
CTsEntry* CTsEntry::NewL(const TTsEntryKey &key, MTsDataObserver &observer, QObject* obj)
{
    CTsEntry* self = NewLC(key, observer, obj);
    CleanupStack::Pop(self);
    return self;
}

// --------------------------------------------------------------------------
// CTsFswEntry::NewLC
// --------------------------------------------------------------------------
//
CTsEntry* CTsEntry::NewLC(const TTsEntryKey &key, MTsDataObserver &observer, QObject* obj)
{
    CTsEntry* self = new (ELeave) CTsEntry(key, observer);
    CleanupStack::PushL(self);
    self->ConstructL(obj);
    return self;
}

// --------------------------------------------------------------------------
// CTsFswEntry::~CTsFswEntry
// --------------------------------------------------------------------------
//
CTsEntry::~CTsEntry()
{
    delete mAppName;
    delete mAppIconBitmap;
    delete mScreenshot;
    delete iProvider;
}

// --------------------------------------------------------------------------
// CTsFswEntry::CTsFswEntry
// --------------------------------------------------------------------------
//
CTsEntry::CTsEntry(const TTsEntryKey &key, MTsDataObserver &observer) 
:
    mVisibility(Visible),
    mKey(key), 
    mPriority(Low), 
    mObserver(observer)
{
    RefreshUpdateTimestamp();
}

// --------------------------------------------------------------------------
void CTsEntry::ConstructL(QObject* object)
{
    QT_TRYCATCH_LEAVING(
       iProvider = new TsThumbnailProvider(*this, object);
    )
    
}

// --------------------------------------------------------------------------
// CTsFswEntry::SetAppUid
// --------------------------------------------------------------------------
//
void CTsEntry::SetAppUid(const TUid &uid)
{
    mAppUid = uid;
}

// --------------------------------------------------------------------------
// CTsFswEntry::SetAppNameL
// --------------------------------------------------------------------------
//
void CTsEntry::SetAppNameL(const TDesC &appName)
{
    delete mAppName;
    mAppName = 0;
    mAppName = appName.AllocL();
}

// --------------------------------------------------------------------------
// CTsFswEntry::SetSystemApp
// --------------------------------------------------------------------------
//
void CTsEntry::SetCloseableApp(TBool value)
{
    mCloseableApp = value;
}

// --------------------------------------------------------------------------
// CTsFswEntry::SetAppIconHandles
// --------------------------------------------------------------------------
//
void CTsEntry::SetAppIcon(CFbsBitmap * bitmap)
{
    mAppIconBitmap = bitmap;
}

// --------------------------------------------------------------------------
/**
 * Application uid.
 */
TUid CTsEntry::AppUid() const
{
    return mAppUid;
}

// --------------------------------------------------------------------------
/**
 * Application name.
 */
const TDesC& CTsEntry::AppName() const
{
    return mAppName ? *mAppName : KNullDesC();
}

// --------------------------------------------------------------------------
/**
 * Retrieve entry visibilit status
 */
Visibility CTsEntry::GetVisibility() const
{
    return mVisibility;
}

// --------------------------------------------------------------------------
/**
 * Change entry visibility status
 * @param visibility - new visibility status
 */
void CTsEntry::SetVisibility(Visibility visibility)
{
    mVisibility = visibility;
}

// --------------------------------------------------------------------------
/**
 * ETrue if the application is closeable
 */
TBool CTsEntry::CloseableApp() const
{
    return mCloseableApp;
}

// --------------------------------------------------------------------------
/**
 * Application icon bitmap
 */
CFbsBitmap *CTsEntry::AppIconBitmap() const
{
    return mAppIconBitmap;
}

// --------------------------------------------------------------------------
/**
 * Entry's key
 */
const TTsEntryKey &CTsEntry::Key() const
{
    return mKey;
}

TTime CTsEntry::Timestamp() const
{
    return mTimestamp;
}
    
// --------------------------------------------------------------------------
/**
 * Timestamp of last entry change 
 */
TTime CTsEntry::LastUpdateTimestamp() const
{
    return mUpdateTimestamp;
}

void CTsEntry::SetTimestamp(const TTime &timestamp)
{
    mTimestamp = timestamp;
    RefreshUpdateTimestamp();
}

// --------------------------------------------------------------------------
/**
 * Set new value of updates timestamp
 */
void CTsEntry::RefreshUpdateTimestamp()
{
    mUpdateTimestamp.UniversalTime();
}

// --------------------------------------------------------------------------
// CTsFswEntry::SetScreenshot
// --------------------------------------------------------------------------
//
void CTsEntry::SetScreenshotL(const CFbsBitmap &bitmapArg, UpdatePriority priority, TInt angle)
{
    TInt currentPriority = static_cast<TInt> (mPriority);
    TInt newPriority = static_cast<TInt> (priority);
    if(newPriority <currentPriority) {
        User::Leave(KErrAccessDenied);
    }
    
    CFbsBitmap* bitmap = new (ELeave) CFbsBitmap;
    CleanupStack::PushL(bitmap);
    User::LeaveIfError(bitmap->Duplicate(bitmapArg.Handle()));
    CleanupStack::Pop(bitmap);

    mPriority = priority;
    delete mScreenshot;
    mScreenshot = bitmap;
    RefreshUpdateTimestamp();

    iProvider->createThumbnail( *mScreenshot, angle);
}

// --------------------------------------------------------------------------
// CTsFswEntry::RemoveScreenshot
// --------------------------------------------------------------------------
//
void CTsEntry::RemoveScreenshotL()
{
    if (!mScreenshot) {
        User::Leave(KErrNotFound);
    }
    delete mScreenshot;
    mScreenshot = NULL;
    mPriority = Low;
    
    mObserver.DataChanged();
    RefreshUpdateTimestamp();
}

// --------------------------------------------------------------------------
/**
 * Application screenshot.
 */
CFbsBitmap* CTsEntry::Screenshot() const
{
    return mScreenshot;
}

// --------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MTsThumbnailObserver::ThumbnailCreated( const CFbsBitmap& )
 */
void CTsEntry::ThumbnailCreated(const CFbsBitmap& aThumbnail)
    {
    mScreenshot->Reset();
    mScreenshot->Duplicate(aThumbnail.Handle());
    RefreshUpdateTimestamp();
    mObserver.DataChanged();
    }

// end of file
