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

#include <cadefs.h>
#include <qserviceinterfacedescriptor.h>
#include <qabstractsecuritysession.h>
#include <qservicecontext.h>

#include "cahandlerloader.h"
#include "caapphandler.h"
#include "caapphandlerplugin.h"

/*!
    \class CaAppHandlerPlugin
    \ingroup
    \brief Implementation of Qt SF plugin for application command handler.
*/

/*!
    \param descriptor Service descriptor.
    \param context Ignored.
    \param session Ignored.
    \retval an instance of the CaAppHandler when descriptor interface name
    is "com.nokia.homescreen.ICommandHandler", NULL otherwise.
*/
QObject *CaAppHandlerPlugin::createInstance(const QServiceInterfaceDescriptor &descriptor,
        QServiceContext *context,
        QAbstractSecuritySession *session)
{
    Q_UNUSED(context);
    Q_UNUSED(session);

    if (descriptor.interfaceName() ==
            "com.nokia.homescreen.ICommandHandler") {
        return new CaAppHandler();
    } else {
        return 0;
    }
}

#ifdef COVERAGE_MEASUREMENT
#pragma CTC SKIP
#endif //COVERAGE_MEASUREMENT (QT macro)

Q_EXPORT_PLUGIN2(caapphandlerplugin, CaAppHandlerPlugin)

#ifdef COVERAGE_MEASUREMENT
#pragma CTC ENDSKIP
#endif //COVERAGE_MEASUREMENT

