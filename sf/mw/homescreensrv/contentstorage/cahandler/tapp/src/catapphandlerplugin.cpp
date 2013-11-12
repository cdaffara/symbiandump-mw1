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
 * Description:  Default runtime plugin.
 *
 */

#include "catapphandlerplugin.h"
#include "catapphandler.h"

/*!
    \class CaTappHandlerPlugin
    \ingroup
    \brief Implementation of Qt SF plugin for templated application command handler.
*/

/*!
    \param descriptor service descriptor
    \param context ignored
    \param session ignored
    \retval An instance of the CaTapphandler object when descriptor interface name
    is "com.nokia.homescreen.ICommandHandler", NULL otherwise.
*/
QObject *CaTappHandlerPlugin::createInstance(const QServiceInterfaceDescriptor &descriptor,
    QServiceContext *context, QAbstractSecuritySession *session)
{
    Q_UNUSED(context);
    Q_UNUSED(session);

    if (descriptor.interfaceName() == QLatin1String("com.nokia.homescreen.ICommandHandler")) {
        return new CaTappHandler();
    }
    else {
        return 0;
    }
}

#ifdef COVERAGE_MEASUREMENT
#pragma CTC SKIP
#endif //COVERAGE_MEASUREMENT (QT macro)

Q_EXPORT_PLUGIN2(catapphandlerplugin, CaTappHandlerPlugin)

#ifdef COVERAGE_MEASUREMENT
#pragma CTC ENDSKIP
#endif //COVERAGE_MEASUREMENT
