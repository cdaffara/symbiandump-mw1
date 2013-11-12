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

#include "afstorageproxy.h"

#include <QPixmap>

bool AfStorageProxy::waitFailed = false;
AfStorageProxy *AfStorageProxy::instance = 0;

AfStorageProxy::AfStorageProxy(QObject *parent) :
    QObject(parent)
{
    instance = this;
}

AfStorageProxy::~AfStorageProxy()
{
    instance = 0;
}

bool AfStorageProxy::saveActivity(int applicationId, const QString &activityId,
    const QString &customActivityName, const QVariant &activityData, const QVariantHash &metadata,
    const QPixmap &screenshot)
{
    mLastCallParams.clear();
    mLastCallParams.insert("applicationId", applicationId);
    mLastCallParams.insert("activityId", activityId);
    mLastCallParams.insert("customActivityName", customActivityName);
    mLastCallParams.insert("activityData", activityData);
    mLastCallParams.insert("metadata", metadata);
    mLastCallParams.insert("screenshot", QVariant::fromValue<QPixmap>(screenshot));
    return true;
}

bool AfStorageProxy::removeActivity(int applicationId, const QString &activityId)
{
    mLastCallParams.clear();
    mLastCallParams.insert("applicationId", applicationId);
    mLastCallParams.insert("activityId", activityId);
    return true;
}

bool AfStorageProxy::removeApplicationActivities(int applicationId)
{
    mLastCallParams.clear();
    mLastCallParams.insert("applicationId", applicationId);
    return true;
}

bool AfStorageProxy::activities(QList<QVariantHash> &list, int limit)
{
    Q_UNUSED(list);
    mLastCallParams.clear();
    mLastCallParams.insert("limit", limit);
    return true;
}

bool AfStorageProxy::applicationActivities(QStringList &list, int applicationId)
{
    Q_UNUSED(list);
    mLastCallParams.clear();
    mLastCallParams.insert("applicationId", applicationId);
    return true;
}

bool AfStorageProxy::activityData(QVariant &data, int applicationId, const QString &activityId)
{
    Q_UNUSED(data);
    mLastCallParams.clear();
    mLastCallParams.insert("applicationId", applicationId);
    mLastCallParams.insert("activityId", activityId);
    return true;
}

bool AfStorageProxy::activityMetaData(QVariantHash &metadata, int applicationId,
    const QString &activityId)
{
    Q_UNUSED(metadata);
    mLastCallParams.clear();
    mLastCallParams.insert("applicationId", applicationId);
    mLastCallParams.insert("activityId", activityId);
    return true;
}

bool AfStorageProxy::waitActivity()
{
    if (waitFailed) {
        waitFailed = false;
        return false;
    }
    mLastCallParams.clear();
    return true;
}

bool AfStorageProxy::launchActivity(int applicationId, const QString &activityUri)
{
    mLastCallParams.clear();
    mLastCallParams.insert("applicationId", applicationId);
    mLastCallParams.insert("activityUri", activityUri);
    return true;
}

bool AfStorageProxy::getThumbnail(const QString &imagePath, void *userData)
{
    mLastCallParams.clear();
    mLastCallParams.insert("imagePath", imagePath);
    mLastCallParams.insert("userData", (uint) (userData));
    emit
    thumbnailRequested(QPixmap(), userData);
    return true;
}

bool AfStorageProxy::notifyDataChange()
{
    mLastCallParams.clear();
    return true;
}

// ---------------------------------------------------------------------------
//  Mock implementation
// ---------------------------------------------------------------------------
//
void AfStorageProxy::emitActivityRequested(const QString &activityId)
{
    emit activityRequested(activityId);
}
