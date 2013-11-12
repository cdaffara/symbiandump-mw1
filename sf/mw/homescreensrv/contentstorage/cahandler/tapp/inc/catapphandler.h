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
 * Description:  Default implementation of the tapp handler
 *
 */

#ifndef CATAPPHANDLER_H
#define CATAPPHANDLER_H

#include <QObject>

#include "cahandler.h"

class CCaInnerEntry;
class XQApplicationManager;

class CaTappHandler: public QObject, public CaHandler
{
    Q_OBJECT
    Q_INTERFACES(CaHandler)

public:
    explicit CaTappHandler(QObject *parent = 0);
    ~CaTappHandler();

    Q_INVOKABLE
    int execute(const CaEntry &entry, const QString &command, 
            QObject* receiver = NULL, const char* member = NULL);

private:
    XQApplicationManager *mAiwMgr;

};

#endif // CATAPPHANDLER_H
