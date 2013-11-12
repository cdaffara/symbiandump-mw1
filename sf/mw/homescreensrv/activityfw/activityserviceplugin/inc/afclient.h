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
#ifndef AFCLIENT_H
#define AFCLIENT_H

#include <QObject>
#include <QVariant>
#include <QString>
#include <QSharedPointer>

#include "afactivitystorage.h"
#include "afactivation.h"

class AfClient : public QObject
{
    Q_OBJECT

public:
    AfClient(const QSharedPointer<AfActivityStorage> &storage, const QSharedPointer<AfActivation> &activation, QObject *parent = 0);             
    ~AfClient();

public slots:
    bool addActivity(const QString &activityId, const QVariant &data, const QVariantHash &parameters);
    bool removeActivity(const QString &activityId);
    bool updateActivity(const QString &activityId, const QVariant &data, const QVariantHash &parameters);
    QList<QVariantHash> activities() const;
    QVariant activityData(const QString &activityId) const;
    QVariantHash parseCommandLine(const QStringList &commandLineParams) const;

private slots:
    void handleActivityRequest(Af::ActivationReason reason, const QString &name, const QVariantHash &parameters);
    
signals:
    void activityRequested(const QString &activityId);

private:    
    QSharedPointer<AfActivityStorage> mStorage;
    QSharedPointer<AfActivation> mActivation;
    
};

#endif // AFCLIENT_H
