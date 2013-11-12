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
#include "afclient.h"

#include "aflauncher.h"
#include "afcommandlineparser.h"

AfClient::AfClient(const QSharedPointer<AfActivityStorage> &storage, const QSharedPointer<AfActivation> &activation, QObject *parent)
: 
    QObject(parent),
    mStorage(storage),
    mActivation(activation)
{
    connect(mActivation.data(), SIGNAL(activated(Af::ActivationReason,QString,QVariantHash)), this, SLOT(handleActivityRequest(Af::ActivationReason,QString,QVariantHash)));
}

AfClient::~AfClient()
{
}

bool AfClient::addActivity(const QString &activityId, const QVariant &data, const QVariantHash &parameters)
{
    return mStorage->saveActivity(activityId, data, parameters);
}

bool AfClient::removeActivity(const QString &activityId)
{
    return mStorage->removeActivity(activityId);
}

bool AfClient::updateActivity(const QString &activityId, const QVariant &data, const QVariantHash &parameters)
{
    return mStorage->saveActivity(activityId, data, parameters);
}

QList<QVariantHash> AfClient::activities() const
{
    QList<QVariantHash> result;
    
    QStringList activitiesList = mStorage->allActivities();
    foreach (const QString &activityId, activitiesList) {
        result.append(mStorage->activityMetaData(activityId));
    }
    return result;
}

QVariant AfClient::activityData(const QString &activityId) const
{
    return mStorage->activityData(activityId);
}

QVariantHash AfClient::parseCommandLine(const QStringList &commandLineParams) const
{
    return AfCommandLineParser::getActivityParameters(commandLineParams);
}

void AfClient::handleActivityRequest(Af::ActivationReason reason, const QString &name, const QVariantHash &parameters)
{
    Q_UNUSED(parameters);
    if (Af::ActivationReasonActivity == reason) {
        emit activityRequested(name);
    }
}
