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
#ifndef AFMANAGER_H
#define AFMANAGER_H

#include <QObject>
#include <QVariant>
#include <QList>
#include <QPixmap>
#include <QSize>

#include "afstorageproxy.h"

class AfManager : public QObject
{

    Q_OBJECT

public:
    AfManager(const QSharedPointer<AfStorageProxy> &serviceProvider, QObject *parent = 0);
    ~AfManager();

public slots:
    QList<QVariantHash> activitiesList(int limit = 0);
    void launchActivity(const QString &uri);
    void launchActivity(const QUrl &uri);
    void launchActivity(int applicationId, const QString &activityId, const QVariantHash& parameters = QVariantHash());
    void getThumbnail(const QString &thumbnailPath, void *data =0);

signals:
    void thumbnailReady(QPixmap, void *);
    void dataChanged();

private:
    void launchActivity(const QVariantHash& activity);
    QUrl activityToUri(const QVariantHash& activity) const;

private:
    QSharedPointer<AfStorageProxy> mServiceProvider;
};

#endif // AFMANAGER_H
