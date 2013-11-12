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

#ifndef ACTIVITYTSMODEL_H
#define ACTIVITYTSMODEL_H

#include <QObject>
#include <QVariantHash>

#include "activitytsentry.h"
#include "tswindowgroupsobserver.h"

class MTsWindowGroupsMonitor;

class ActivityTsModel : public QObject,
                        public MTsWindowGroupsObserver
{
    Q_OBJECT

public:
    ActivityTsModel(QObject *parent = 0);
    virtual ~ActivityTsModel();

public: //from MTsWindowGroupsObserver
    void HandleWindowGroupChanged(MTsResourceManager& resources, 
                                  const MTsRunningApplicationStorage& storage);

private:
    bool filterActivity();
    ActivityTsEntry *findEntryWithScreenshot(const QList<ActivityTsEntry*> &entryList, const QVariantHash &activityEntry);
    
public slots:
    QList<QVariantHash> taskList() const;
    QList<QVariantHash> taskList(int limit) const;
    
    bool openTask(const QVariant &id);
    bool closeTask(const QVariant &id);
    
signals:
    void dataChanged();
    
private slots:
    void getActivities();
    
public slots:
    void convertScreenshotToThumbnail(const QPixmap &thumbnail, void *userData);
    void thumbnailCreated(const QPixmap &thumbnail, const void *userData);
    void setResources(MTsResourceManager& resources);

signals:
    void createThumbnail(const QPixmap &source, int angle, const void *userData);
    
private:
    MTsWindowGroupsMonitor* mMonitor;
    QObject *mAfManager;
    int mMaxItems;
    QList<ActivityTsEntry*> mData;
    QList<ActivityTsEntry*> mPublishedData;
    
    QList<int> mRunningAppsUid;
};

#endif // ACTIVITYTSMODEL_H
