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
#include "afactivation_p.h"

#include <QCoreApplication>

#include "afstorageproxy.h"

#include "aflauncher.h"
#include "afcommandlineparser.h"
    
AfActivationPrivate::AfActivationPrivate(const QSharedPointer<AfStorageProxy> &connection, QObject *parent) : QObject(parent), mReason(Af::ActivationReasonNormal), mConnection(connection)
{
    AfCommandLineParser::parseCommandLine(qApp->arguments(),
            mReason,
            mName,
            mParameters);
    connect(mConnection.data(), SIGNAL(activityRequested(QString)), this, SLOT(handleActivityRequest(QString)));
    connect(mConnection.data(), SIGNAL(activityRequested(QString)), this, SLOT(bringToForeground()));
}
   
QVariantHash AfActivationPrivate::parameters() const
{
    return mParameters;
}

Af::ActivationReason AfActivationPrivate::reason() const
{
    return mReason;
}

QString AfActivationPrivate::name() const
{
    return mName;
}

void AfActivationPrivate::handleActivityRequest(const QString &activityUri)
{
    mReason = Af::ActivationReasonActivity;
    mName = QString();
    mParameters = QVariantHash();
    
    QUrl uri(activityUri);
    QVariantHash parameters;
    QList<QPair<QString, QString> > uriParams = uri.queryItems();
    for (QList<QPair<QString, QString> >::const_iterator i = uriParams.constBegin(); i != uriParams.constEnd(); ++i) {
        parameters.insert(i->first, i->second);
    }
    if (parameters.contains(Af::KActivityUriNameKey)) {
        mName = parameters.value(Af::KActivityUriNameKey).toString();
        parameters.remove(Af::KActivityUriNameKey);
    }
    mParameters = parameters;
    
    emit activated(mReason, mName, mParameters);
}

void AfActivationPrivate::bringToForeground()
{
    if (!mParameters.contains(Af::KActivityUriBackgroundKey)) {
        // process all update events from widgets to prevent flickering
        QCoreApplication::processEvents();
        AfLauncher().bringToForeground(RProcess().SecureId().iId);
    }
}
