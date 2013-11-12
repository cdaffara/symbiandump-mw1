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

#include "afserviceplugin.h"

#include <qserviceinterfacedescriptor.h>
#include <qabstractsecuritysession.h>
#include <qservicecontext.h>

#include "afstorageproxy.h"
#include "afclient.h"
#include "afmanager.h"
#include "afactivation.h"
#include "afactivitystorage.h"

QObject *AfServicePlugin::createInstance(const QServiceInterfaceDescriptor &descriptor, QServiceContext *context, QAbstractSecuritySession *session)
{
    Q_UNUSED(context);
    Q_UNUSED(session);

    if (descriptor.interfaceName() == "com.nokia.qt.activities.ActivityClient") {
        return new AfClient(QSharedPointer<AfActivityStorage>(new AfActivityStorage()), QSharedPointer<AfActivation>(new AfActivation()));
    } else if (descriptor.interfaceName() == "com.nokia.qt.activities.ActivityManager") {
        return new AfManager(QSharedPointer<AfStorageProxy>(new AfStorageProxy()));
    } else if (descriptor.interfaceName() == "com.nokia.qt.activities.Storage") {
        return new AfActivityStorage();
    } else if (descriptor.interfaceName() == "com.nokia.qt.activities.Activation") {
        return new AfActivation();
    } else {
        return NULL;
    }
}

Q_EXPORT_PLUGIN2(activityserviceplugin, AfServicePlugin)
