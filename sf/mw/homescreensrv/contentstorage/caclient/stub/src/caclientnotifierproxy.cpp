/*
 * Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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

#include <QDebug>
#include <QMutexLocker>

#include "caclientnotifierproxy.h"
#include "caentry.h"
#include "caobjectadapter.h"
#include "canotifiers.h"

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
CaClientNotifierProxy::CaClientNotifierProxy(QObject *parent) :
    QObject(parent)
{
}
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
CaClientNotifierProxy::~CaClientNotifierProxy()
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CaClientNotifierProxy::registerNotifier(
    const CaNotifierFilter *notifierFilter,
    CaNotifierPrivate::NotifierType notifierType,
    const CaObserver *observer)
{
    qDebug() << "CaClientProxy::registerNotifier notifierType:"
             << notifierType;
    return CaNotifiers::addNotifier(notifierFilter, notifierType, observer);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaClientNotifierProxy::unregisterNotifier(
    const CaNotifierFilter &notifierFilter,
    CaNotifierPrivate::NotifierType notifierType)
{
    qDebug() << "CaClientProxy::unregisterNotifier notifierType:"
             << notifierType;
    CaNotifiers::removeNotifier(&notifierFilter, notifierType);
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaClientNotifierProxy::connectSessions()
{
    qDebug() << "Nothing to do on windows";
}


