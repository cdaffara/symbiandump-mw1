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

#include "activitytsplugin.h"

#include <QServiceInterfaceDescriptor>
#include <QAbstractSecuritySession>
#include <QServiceContext>

#include "activitytsmodel.h"

QObject *ActivityTsPlugin::createInstance(const QServiceInterfaceDescriptor &descriptor, QServiceContext *context, QAbstractSecuritySession *session)
{
    Q_UNUSED(context);
    Q_UNUSED(session);

    if (descriptor.interfaceName() == "com.nokia.qt.taskswitcher.dataprovider") {
        return new ActivityTsModel();
    } else {
        return NULL;
    }
}

Q_EXPORT_PLUGIN2(activitytsplugin, ActivityTsPlugin)
