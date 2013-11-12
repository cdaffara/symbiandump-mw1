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

#ifndef AFSTORAGEPROXY_H
#define AFSTORAGEPROXY_H

#include <QObject>
#include <QVariant>
#include <QStringList>

class AfStorageProxyPrivate;

class AfStorageProxy : public QObject
{
    Q_OBJECT

public:
    AfStorageProxy(QObject *parent = 0);
    virtual ~AfStorageProxy();

public:    
    bool saveActivity(int applicationId, const QString &activityId, const QString &customActivityName, const QVariant &activityData, const QVariantHash &metadata, const QPixmap &screenshot);
    bool removeActivity(int applicationId, const QString &activityId);   
    bool removeApplicationActivities(int applicationId);
    bool activities(QList<QVariantHash> &list, int limit = 0);
    bool applicationActivities(QStringList &list, int applicationId);
    bool activityData(QVariant &data, int applicationId, const QString &activityId);
    bool activityMetaData(QVariantHash &metadata, int applicationId, const QString &activityId);
    bool waitActivity();    
    bool launchActivity(int applicationId, const QString &activityUri);
    bool getThumbnail(const QString &imagePath, void *userData);
    bool notifyDataChange();

signals:
    void activityRequested(const QString &activityUri);
    void thumbnailRequested(const QPixmap &thumbnailPixmap, void *userData);
    void dataChanged();
    
private:
    AfStorageProxyPrivate *d_ptr;
    
    friend class AfStorageProxyPrivate;
    
};

#endif //AFSTORAGEPROXY_H
