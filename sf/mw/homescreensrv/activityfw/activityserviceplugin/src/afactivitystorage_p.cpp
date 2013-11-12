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

#include "afactivitystorage_p.h"

#include <QPixmap>
#include <QDateTime>

#include <afstorageglobals.h>

AfActivityStoragePrivate::AfActivityStoragePrivate(const QSharedPointer<AfStorageProxy> &connection) : mConnection(connection)
{
}

bool AfActivityStoragePrivate::saveActivity(const QString &activityId, const QVariant &activityData, const QVariantHash &metadata)
{
    QVariantHash publicData(metadata);
        
    QPixmap screenshot(publicData[ActivityScreenshotKeyword].value<QPixmap>());
    publicData.remove(ActivityScreenshotKeyword);
    
    publicData.insert(ActivityApplicationKeyword, applicationId());
    publicData.insert(ActivityActivityKeyword, activityId);
    publicData.insert(ActivityTimestamp, QDateTime::currentDateTime());

    return mConnection->saveActivity(applicationId(), activityId, publicData[ActivityApplicationName].toString(), activityData, publicData, screenshot);
}

bool AfActivityStoragePrivate::removeActivity(const QString &activityId)
{
    return mConnection->removeActivity(applicationId(), activityId);
}

QStringList AfActivityStoragePrivate::allActivities() const
{    
    QStringList activities;
    mConnection->applicationActivities(activities, applicationId());        
    return activities;
}

QVariant AfActivityStoragePrivate::activityData(const QString &activityId) const
{
    QVariant data;
    mConnection->activityData(data, applicationId(), activityId);
    return data;
}

QVariantHash AfActivityStoragePrivate::activityMetaData(const QString &activityId) const
{
    QVariantHash metadata;
    mConnection->activityMetaData(metadata, applicationId(), activityId);
    return metadata;
}

int AfActivityStoragePrivate::applicationId()
{
    return RProcess().SecureId().iId;
}
