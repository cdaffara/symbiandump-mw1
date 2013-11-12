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

#ifndef CACLIENTNOTIFIERPROXY_H
#define CACLIENTNOTIFIERPROXY_H

#include <QObject>
#include <QMutex>

#include "caclient_global.h"
#include "cadefs.h"
#include "canotifier_p.h"

class CaEntry;
class CaNotifierFilter;
class CaObserver;




class CACLIENT_EXPORT CaClientNotifierProxy: public QObject
{
    Q_OBJECT

public:

    /**
     * Constructor.
     * @param parent pointer to parent. Defaults to NULL.
     */
    explicit CaClientNotifierProxy(QObject *parent = 0);

    /**
     * Destructor.
     */
    ~CaClientNotifierProxy();

    /**
     * Method for registering notifier
     *
     * @param notifierFilter notifierFilter used to register notifier.
     * @param notifierType type of notifier
     * @return error code
     */
    int registerNotifier(
        const CaNotifierFilter *notifierFilter,
        CaNotifierPrivate::NotifierType notifierType,
        const CaObserver *observer);

    /**
     * Method for unregistering notifier.
     *
     * @param notifierFilter notifierFilter used to unregister notifier.
     * @param notifierType type of notifier
     */
    void unregisterNotifier(
        const CaNotifierFilter &notifierFilter,
        CaNotifierPrivate::NotifierType notifierType);


    /**
     * Reconnect all sessions when server was terminated
     *
     */
    void connectSessions();

private:

    Q_DISABLE_COPY(CaClientNotifierProxy)
};

#endif //_CACLIENTNOTIFIERPROXY_H
