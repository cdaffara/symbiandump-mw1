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

#ifndef HBACTIVITYPLUGININTERFACE_P_H
#define HBACTIVITYPLUGININTERFACE_P_H

#include <QtPlugin>
#include <QVariant>
#include <QString>
#include <QList>
#include <QVariantHash>

class HbActivityPluginInterface
{
public:
    virtual ~HbActivityPluginInterface() {}

public:
    virtual bool addActivity(const QString &activityId, const QVariant &data, const QVariantHash &parameters) = 0;
    virtual bool removeActivity(const QString &activityId) = 0;
    virtual bool updateActivity(const QString &activityId, const QVariant &data, const QVariantHash &parameters) = 0;
    virtual QList<QVariantHash> activities() = 0;
    virtual QVariant activityData(const QString &activityId) = 0;
    virtual bool waitActivity() = 0;
    virtual QVariantHash parseCommandLine(const QStringList &commandLineParams) = 0;

signals:
    // signal must be re-declared in the plugin implementation
    void activityRequested(const QString &activityId);
};

Q_DECLARE_INTERFACE(HbActivityPluginInterface, "HbActivityPluginInterface/1.0")

#endif // HBACTIVITYPLUGININTERFACE_P_H
