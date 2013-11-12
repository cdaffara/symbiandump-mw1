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
 * Description:  Default implementation of the home screen runtime.
 *
 */

#include <qservicemanager.h>
#include <qservicefilter.h>
#include <qserviceinterfacedescriptor.h>
#include <XQConversions>
#include <xqappmgr.h>

#include "caentry.h"
#include "catapphandler.h"
#include "caclient_defines.h"

static const QString hsitemLaunchUri("item:launchuri");

QTM_USE_NAMESPACE

/*!
    \class CaTappHandler
    \ingroup
    \brief Implementation of templated application command handler.
*/

/*!
 Constructor.
 \param parent Parent object of this object.
 */
CaTappHandler::CaTappHandler(QObject *parent)
{
    Q_UNUSED(parent);
    mAiwMgr = new XQApplicationManager();
}

/*!
 Destructor.
 */
CaTappHandler::~CaTappHandler()
{
    delete mAiwMgr;
}

/*!
 Executes \a command by default "open" on template application represented by \a entry.
 Uses XQApplicationManager
 \param entry Subject of the \a command.
 \param command Description of the command.
 \param receiver a QObject class with slot to invoke.
 \param member a slot to invoke.
 \retval Error code described in QSERVICEMANAGER.H
 */
int CaTappHandler::execute(const CaEntry& entry, const QString& command, 
        QObject* receiver, const char* member)
{
	Q_UNUSED(receiver);
	Q_UNUSED(member);
    // this returns Error enum from QSERVICEMANAGER.H
    int error = 0;
    if (command == caCmdOpen) {
        QString attribute = entry.attribute(hsitemLaunchUri);
        if (!attribute.isNull()) {
            // e.g. QUrl("appto://101F7AE7?");
            QScopedPointer<XQAiwRequest> request(mAiwMgr->create(QUrl(attribute),
                false));
            if (!request.isNull()) {
                bool res = request->send();
                if (!res) {
                    error = request->lastError();
                }
            }
        }
    } else {
        // TODO: error code cleanning and an appropriate conversion (in other classes too).
        static const int NotSupportedErrorCode = 100;
        error = NotSupportedErrorCode;
    }
    return error;
}
