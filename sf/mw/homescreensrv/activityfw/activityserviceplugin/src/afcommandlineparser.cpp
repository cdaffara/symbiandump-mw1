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

#include "afcommandlineparser.h"

#include <QUrl>

// Startup arguments for services
static const char * StartupArgEmbedded = "embedded=yes";
static const char * StartupArgService = "service=yes";
static const char * StartupArgInterfaceId = "intf=";
static const char * StartupArgOperationId = "oper=";
static const char * StartupArgEmbeddedName = "embedded";
static const char * StartupArgInterfaceName = "intf";
static const char * StartupArgOperationName = "oper";

// for activities
const char KCmdLineMarker[] = "-activity";

void AfCommandLineParser::parseCommandLine(const QStringList &commandLineParams,
            Af::ActivationReason &reason,
            QString &name,
            QVariantHash &parameters)
{
    if (AfCommandLineParser::isServiceCommand(commandLineParams)) {
        parseServiceParameters(commandLineParams,
                reason,
                name,
                parameters);
    }
    else if (AfCommandLineParser::isActivityCommand(commandLineParams)) {
        parseActivityParameters(commandLineParams,
                reason,
                name,
                parameters);
    }  
}

bool AfCommandLineParser::isServiceCommand(const QStringList &commandLineParams)
{
    return commandLineParams.contains(StartupArgService);
}

bool AfCommandLineParser::isActivityCommand(const QStringList &commandLineParams)
{
    return commandLineParams.contains(KCmdLineMarker);
}

void AfCommandLineParser::parseServiceParameters(const QStringList &commandLineParams,
        Af::ActivationReason &reason,
        QString &name,
        QVariantHash &parameters)
{
    reason = Af::ActivationReasonService;
    parameters = QVariantHash();
    int num = commandLineParams.count();
    for ( int i = 0; i < num; i++ ) {
        QString s = commandLineParams[i] ;
        if (s.startsWith(StartupArgInterfaceId)) {
            name = s.remove(StartupArgInterfaceId);
            parameters.insert(StartupArgInterfaceName,name);
        } 
        else if (s.startsWith(StartupArgOperationId)) {
            parameters.insert(StartupArgOperationName,s.remove(StartupArgOperationId));
        }
        else if (s.startsWith(StartupArgEmbedded)) {
            parameters.insert(StartupArgEmbeddedName,true);
        }
    }  
}

void AfCommandLineParser::parseActivityParameters(const QStringList &commandLineParams,
        Af::ActivationReason &reason,
        QString &name,
        QVariantHash &parameters)
{
    QVariantHash startupActivity = AfCommandLineParser::getActivityParameters(commandLineParams);
    if (!startupActivity.isEmpty() && 
            !startupActivity.value(Af::KActivityUriNameKey).toString().isEmpty()) {
        reason = Af::ActivationReasonActivity;
        parameters = startupActivity;
        name = startupActivity.value(Af::KActivityUriNameKey).toString();
    }
}

QVariantHash AfCommandLineParser::getActivityParameters(const QStringList &commandLineParams)
{
    QVariantHash activityParams;
    int activityMarkerIndex = commandLineParams.indexOf(KCmdLineMarker);
    if (activityMarkerIndex != -1 && commandLineParams.count() - 1 > activityMarkerIndex) {
        QUrl activityUri = QUrl::fromEncoded(commandLineParams.at(activityMarkerIndex+1).toAscii());
        if (activityUri.scheme() == Af::KActivityScheme) {
            QList<QPair<QString, QString> > parameters = activityUri.queryItems();
            for (QList<QPair<QString, QString> >::const_iterator i = parameters.constBegin(); i != parameters.constEnd(); ++i) {
                activityParams.insert(i->first, i->second);
            }

            if (activityParams.contains(Af::KActivityUriNameKey) && !activityParams.value(Af::KActivityUriNameKey).toString().isEmpty()) {
                return activityParams;
            }
        }
    }
    return QVariantHash();
}
