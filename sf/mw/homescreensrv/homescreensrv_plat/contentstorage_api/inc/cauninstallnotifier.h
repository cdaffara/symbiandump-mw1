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
 * Description: canotifier.h
 *
 */

#ifndef CAUINSTALLNOTIFIER_H
#define CAUINSTALLNOTIFIER_H

#include <QObject>
#include <QSharedPointer>

#include "caclient_global.h"
class CaUninstallNotifierPrivate;

class CaUninstallNotifier: public QObject
{
Q_OBJECT

public:

    static CaUninstallNotifier* notifier();

private: 
    
    explicit CaUninstallNotifier(CaUninstallNotifierPrivate *const notifierPrivate);

    virtual ~CaUninstallNotifier();

signals:

    /*!
        A signal emited when the entry is changed.
        \param entryId entry id.
        \param changeType the type of the change.
    */
    void progressChange(int componentId, int progressValue);

private:

    CaUninstallNotifierPrivate * const m_d;
    
    static QSharedPointer<CaUninstallNotifier> mInstance;
    
    Q_DISABLE_COPY(CaUninstallNotifier)

    friend class CaUninstallNotifierPrivate;
    
    friend class QtSharedPointer::ExternalRefCount<CaUninstallNotifier>;
};


#endif // CAUINSTALLNOTIFIER_H
