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

#include "hbactivityplugin.h"

#include <qservicemanager.h>

QTM_USE_NAMESPACE

HbActivityPlugin::HbActivityPlugin(QObject *parent) : QObject(parent), mActivityClient(0)
{
    QServiceManager serviceManager;

    if (serviceManager.findInterfaces("ActivityService").isEmpty()) {
        // clean old entries
        serviceManager.removeService("ActivityService");
        bool servicesAdded = serviceManager.addService(":/afservice.xml");
        if (!servicesAdded) {
            qWarning("addService for ActivityService returned false, error %d", serviceManager.error());
        }
    }

    mActivityClient = serviceManager.loadInterface("com.nokia.qt.activities.ActivityClient");
    if (!mActivityClient) {
        qWarning("Cannot initialize critical com.nokia.qt.activities.ActivityClient service.");
    }

    connect(mActivityClient, SIGNAL(activityRequested(QString)), this, SIGNAL(activityRequested(QString)));
}

HbActivityPlugin::~HbActivityPlugin()
{
    delete mActivityClient;
}

bool HbActivityPlugin::addActivity(const QString &activityId, const QVariant &data, const QVariantHash &parameters)
{
    bool retVal(false);
    QMetaObject::invokeMethod(mActivityClient,
                              "addActivity",
                              Q_RETURN_ARG(bool, retVal),
                              Q_ARG(QString, activityId),
                              Q_ARG(QVariant, data),
                              Q_ARG(QVariantHash, parameters));
    return retVal;
}

bool HbActivityPlugin::removeActivity(const QString &activityId)
{
    bool retVal(false);
    QMetaObject::invokeMethod(mActivityClient,
                              "removeActivity",
                              Q_RETURN_ARG(bool, retVal),
                              Q_ARG(QString, activityId));
    return retVal;
}

bool HbActivityPlugin::updateActivity(const QString &activityId, const QVariant &data, const QVariantHash &parameters)
{
    bool retVal(false);
    QMetaObject::invokeMethod(mActivityClient,
                              "updateActivity",
                              Q_RETURN_ARG(bool, retVal),
                              Q_ARG(QString, activityId),
                              Q_ARG(QVariant, data),
                              Q_ARG(QVariantHash, parameters));
    return retVal;
}
QList<QVariantHash> HbActivityPlugin::activities()
{
    QList<QVariantHash> data;
    QMetaObject::invokeMethod(mActivityClient,
                              "activities",
                              Q_RETURN_ARG(QList<QVariantHash>, data));
    return data;
}

QVariant HbActivityPlugin::activityData(const QString &activityId)
{
    QVariant data;
    QMetaObject::invokeMethod(mActivityClient,
                              "activityData",
                              Q_RETURN_ARG(QVariant, data),
                              Q_ARG(QString, activityId));
    return data;
}

bool HbActivityPlugin::waitActivity()
{
    bool retVal(false);
    QMetaObject::invokeMethod(mActivityClient,
                              "waitActivity",
                              Q_RETURN_ARG(bool, retVal));
    return retVal;
}

QVariantHash HbActivityPlugin::parseCommandLine(const QStringList &commandLineParams)
{
    QVariantHash retVal;
    QMetaObject::invokeMethod(mActivityClient,
                              "parseCommandLine",
                              Q_RETURN_ARG(QVariantHash, retVal),
                              Q_ARG(QStringList, commandLineParams));
    return retVal;
}

Q_EXPORT_PLUGIN2(hbactivityplugin, HbActivityPlugin)
