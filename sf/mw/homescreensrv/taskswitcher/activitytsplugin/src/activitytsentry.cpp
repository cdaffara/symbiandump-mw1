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

#include "activitytsentry.h"

#include <fbs.h>

#include <QDateTime>
#include <QUrl>

#include "afactivities_global.h"
#include "afstorageglobals.h"

ActivityTsEntry::ActivityTsEntry(const QVariantHash &activityData) : mBitmap(0)
{
    mData.insert("TaskTimestamp", activityData.value(ActivityTimestamp));
    mData.insert("TaskUpdateTimestamp", QDateTime::currentDateTime());
    mData.insert("TaskIsRunning", false);
    mData.insert("TaskCanBeClosed", false);
    mData.insert("TaskIsMandatory", 0);
    
    {
        QUrl uri;
        uri.setScheme(Af::KActivityScheme);
        uri.setHost(QString("%1").arg(activityData.value(ActivityApplicationKeyword).toUInt(), 8, 16, QChar('0')));
        
        if (activityData.contains(ActivityActivityKeyword))
            uri.addQueryItem(Af::KActivityUriNameKey, activityData.value(ActivityActivityKeyword).toString());
        mData.insert("Uri", uri);
        mData.insert("TaskId", qHash(uri.toString()));
    }

    mData.insert("TaskName", activityData.value(ActivityApplicationName));
    mData.insert("TaskScreenshot", -1);
    
    mData.insert(ActivityApplicationKeyword, activityData.value(ActivityApplicationKeyword));
    mData.insert(ActivityActivityKeyword, activityData.value(ActivityActivityKeyword));
}

ActivityTsEntry::~ActivityTsEntry()
{
    delete mBitmap;
}

void ActivityTsEntry::setThumbnail(const CFbsBitmap *bitmap)
{
    mBitmap = bitmap;
    mData.insert("TaskScreenshot", mBitmap->Handle());
    mData.insert("TaskUpdateTimestamp", QDateTime::currentDateTime());
}

QVariantHash ActivityTsEntry::data() const
{
    return mData;
}    
