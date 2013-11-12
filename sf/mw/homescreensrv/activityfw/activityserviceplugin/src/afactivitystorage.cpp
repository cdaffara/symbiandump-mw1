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

#include "afactivitystorage.h"
#include "afactivitystorage_p.h"

AfActivityStorage::AfActivityStorage(QObject *parent) : QObject(parent), d_ptr(0)
{
    QSharedPointer<AfStorageProxy> connection(new AfStorageProxy());
    d_ptr.reset(new AfActivityStoragePrivate(connection));
}
    
AfActivityStorage::~AfActivityStorage()
{
}
    
bool AfActivityStorage::saveActivity(const QString &activityId, const QVariant &activityData, const QVariantHash &metadata)
{
    return d_ptr->saveActivity(activityId, activityData, metadata);
}

bool AfActivityStorage::removeActivity(const QString &activityId)
{
    return d_ptr->removeActivity(activityId);
}

QStringList AfActivityStorage::allActivities() const
{
    return d_ptr->allActivities();
}

QVariant AfActivityStorage::activityData(const QString &activityId) const
{
    return d_ptr->activityData(activityId);
}

QVariantHash AfActivityStorage::activityMetaData(const QString &activityId) const
{
    return d_ptr->activityMetaData(activityId);
}
