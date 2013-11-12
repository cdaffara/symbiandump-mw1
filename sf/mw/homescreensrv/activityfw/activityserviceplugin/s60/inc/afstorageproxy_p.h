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

#ifndef AFSTORAGEPROXY_P_H
#define AFSTORAGEPROXY_P_H

#include <apgcli.h>

#include <QVariant>
#include <QStringList>

#include "afasyncrequestobserver.h"

class CAfStorageClient;
class CAfEntry;
class AfStorageProxy;

class AfStorageProxyPrivate : public MAfAsyncRequestObserver
{
public:
    AfStorageProxyPrivate(AfStorageProxy *q);
    ~AfStorageProxyPrivate();

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

public: // from MAfAsyncRequestObserver
    virtual void waitActivityRequestCompleted(int result, const TDesC8 &data);
    virtual void getThumbnailRequestCompleted(int result, int bitmapHandle, void *userData);
    virtual void dataChangeNotificationCompleted(int result);    
    
private:
    CAfEntry *createFilterEntry(int applicationId = 0, const QString &activityId = QString());
    CAfEntry *createSaveEntry(int applicationId, const QString &activityId, const QString &customActivityName, const QVariant &activityData, const QVariantHash &metadata);
    CAfEntry *getEntry(int applicationId, const QString &activityId);
    QVariantHash extractMetadata(CAfEntry *entry, bool includePublicData);
    QString activityDisplayText(CAfEntry *entry);
    
    TTime convertQDateTimeToTTime(const QDateTime &timestamp) const;
    QDateTime convertTTimeToQDateTime(const TTime &s60Time) const;
    
private:
    CAfStorageClient *mClient;
    AfStorageProxy *q_ptr;
    RApaLsSession mAppArcSession;

};

#endif //AFSTORAGEPROXY_P_H
