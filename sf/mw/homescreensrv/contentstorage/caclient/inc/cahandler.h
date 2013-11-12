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
#ifndef CAHANDLER_H
#define CAHANDLER_H

#include <QObject>
#include <cadefs.h>
#include <caclient_global.h>

class QString;
class CaEntry;

class CaHandler
{
public:
    virtual ~CaHandler() {}
    virtual int execute(const CaEntry &entry,
                        const QString &commandName,
                        QObject* receiver = NULL,
                        const char* member = NULL) = 0;
};

Q_DECLARE_INTERFACE(CaHandler, "com.nokia.homescreen.ICommandHandler")

#endif // CAHANDLER_H
