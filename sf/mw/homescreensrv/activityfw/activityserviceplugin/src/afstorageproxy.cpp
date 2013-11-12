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
#include "afstorageproxy_p.h"

AfStorageProxy::AfStorageProxy(QObject *parent) : QObject (parent), d_ptr(new AfStorageProxyPrivate(this))
{    
}

AfStorageProxy::~AfStorageProxy()
{
    delete d_ptr;
}

bool AfStorageProxy::saveActivity(int applicationId, const QString &activityId, const QString &customActivityName, const QVariant &activityData, const QVariantHash &metadata, const QPixmap &screenshot)
{
    return d_ptr->saveActivity(applicationId, activityId, customActivityName, activityData, metadata, screenshot);
}

bool AfStorageProxy::removeActivity(int applicationId, const QString &activityId)
{
    return d_ptr->removeActivity(applicationId, activityId);
}
   
bool AfStorageProxy::removeApplicationActivities(int applicationId)
{
    return d_ptr->removeApplicationActivities(applicationId);
}

bool AfStorageProxy::activities(QList<QVariantHash> &list, int limit)
{
    return d_ptr->activities(list, limit);
}

bool AfStorageProxy::applicationActivities(QStringList &list, int applicationId)
{
    return d_ptr->applicationActivities(list, applicationId);
}

bool AfStorageProxy::activityData(QVariant &data, int applicationId, const QString &activityId)
{
    return d_ptr->activityData(data, applicationId, activityId);
}

bool AfStorageProxy::activityMetaData(QVariantHash &metadata, int applicationId, const QString &activityId)
{
    return d_ptr->activityMetaData(metadata, applicationId, activityId);
}

bool AfStorageProxy::waitActivity()
{
    return d_ptr->waitActivity();
}
    
bool AfStorageProxy::launchActivity(int applicationId, const QString &activityUri)
{
    return d_ptr->launchActivity(applicationId, activityUri);
}

bool AfStorageProxy::getThumbnail(const QString &imagePath, void *userData)
{
    return d_ptr->getThumbnail(imagePath, userData);
}

bool AfStorageProxy::notifyDataChange()
{
    return d_ptr->notifyDataChange();
}
