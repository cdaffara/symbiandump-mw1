/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  ?Description
 *
 */

#include <QDesktopServices>
#include <QUrl>

#include <caentry.h>

#include "caurlhandler.h"

static const QString caAttrUrl("url");

/*!
 * Constructor.
 */
CaUrlHandler::CaUrlHandler(QObject *parent)
{
    Q_UNUSED(parent);
}

/*!
 * Destructor.
 */
CaUrlHandler::~CaUrlHandler()
{
}

/*!
 * Execute a given command.
 * \param entry a reference to CaEntry instance.
 * \param command a given command.
 * \param receiver a QObject class with slot to invoke.
 * \param member a slot to invoke.
 * \retval an error code.
 */
int CaUrlHandler::execute(const CaEntry &entry, const QString &command,
        QObject* receiver, const char* member)
{
	Q_UNUSED(receiver);
	Q_UNUSED(member);
	
    int err(KErrGeneral);
    if (command == caCmdOpen) {
        QString attribute = entry.attribute(caAttrUrl);
        if (!attribute.isNull() &&
            QDesktopServices::openUrl(QUrl(attribute))) {
            err = KErrNone;
        }
    }
    return err;
}
