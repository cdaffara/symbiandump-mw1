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

#ifndef TSENTRY_H
#define TSENTRY_H

#include <e32base.h>
#include <e32cmn.h>

#include "tsentrykey.h"
#include "tstaskmonitorglobals.h"
#include "tsthumbnailobserver.h"

class CTsEntry;
class CFbsBitmap;
class QObject;
class MTsDataObserver;
class TsThumbnailProvider;



typedef RPointerArray<CTsEntry> RTsFswArray;

/**
 * An entry in the task list.
 */
NONSHARABLE_CLASS( CTsEntry ) : public CBase,
                                private MTsThumbnailObserver
{
public:
    static CTsEntry *NewL(const TTsEntryKey &key, MTsDataObserver &observer, QObject* obj);
    static CTsEntry *NewLC(const TTsEntryKey &key, MTsDataObserver &observer, QObject* obj);
    ~CTsEntry();

public:
    TUid AppUid() const;
    const TDesC &AppName() const;
    TBool CloseableApp() const;
    Visibility GetVisibility() const;    
    CFbsBitmap *AppIconBitmap() const;
    CFbsBitmap *Screenshot() const;
    const TTsEntryKey &Key() const;
    TTime Timestamp() const;    
    TTime LastUpdateTimestamp() const;
    
    void SetAppUid(const TUid &uid);
    void SetAppNameL(const TDesC &appName);
    void SetCloseableApp(TBool value);   
    void SetVisibility(Visibility visibility);
    void SetAppIcon(CFbsBitmap *aBitmap);
    void SetScreenshotL(const CFbsBitmap &bitmap, UpdatePriority priority, TInt angle);
    void RemoveScreenshotL();
    void SetTimestamp(const TTime &timestamp);
    void RefreshUpdateTimestamp();

private:
    CTsEntry(const TTsEntryKey &aKey, MTsDataObserver &observer);
    void ConstructL(QObject* object);

public://from MTsThumbnailObserver
    void ThumbnailCreated(const CFbsBitmap& aThumbnail);

private:
    TUid mAppUid;
    HBufC* mAppName;
    TBool mCloseableApp;
    Visibility mVisibility;
    CFbsBitmap* mAppIconBitmap;
    CFbsBitmap* mScreenshot;
    TTsEntryKey mKey;
    UpdatePriority mPriority;
    TTime mTimestamp;
    TTime mUpdateTimestamp;
    TsThumbnailProvider* iProvider;
private:
    MTsDataObserver &mObserver;

};

#endif
