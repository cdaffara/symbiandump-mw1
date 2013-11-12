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

#ifndef CAAPPHANDLER_H
#define CAAPPHANDLER_H

#include <QObject>

#include "cahandler.h"
#include "cauninstallerobserver.h"

class CEikonEnv;
class CCaUsifUninstallOperation;
class CaEntry;

class CaAppHandler: public QObject, public CaHandler, public CaUninstallerObserver
{
    Q_OBJECT
    Q_INTERFACES(CaHandler)

public:

    explicit CaAppHandler(QObject *parent = 0);

    virtual ~CaAppHandler();

public:

    int execute(const CaEntry &entry, const QString &command, 
            QObject* receiver = NULL, const char* member = NULL);
    
    void uninstallError(int error); // from CaUninstallerObserver
    
signals:

    void uninstallFailed(int errorCode);

private:

    void launchApplicationL(const TUid uid, TInt viewId);

    int handleRemove(const EntryFlags &flags,
        const QString &typeName,
        const QString &componentId);
    
    void startUsifUninstallL(TInt componentId);
    
private:
    CEikonEnv *iEikEnv;
    CCaUsifUninstallOperation *iUsifUninstallOperation;
};

#endif // CAAPPHANDLER_H
