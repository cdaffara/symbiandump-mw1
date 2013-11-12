/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include "caobserver.h"
#include "caobjectadapter.h"
#include "caentry.h"

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
CaObserver::CaObserver(QObject *parent) :
    QObject(parent)
{
}
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
CaObserver::~CaObserver()
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaObserver::entryChanged(int entryId,
                              ChangeType changeType) const
{
    qDebug() << "CaClientProxy::entryChanged entryId:"
             << entryId << "changeType:" << changeType;
    emit signalEntryChanged(entryId, changeType);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaObserver::entryChanged(const CaEntry &entry,
        ChangeType changeType) const
{
    qDebug() << "CaClientProxy::entryChanged changeType:" << changeType;
    QSharedPointer<CaEntry> caEntry(new CaEntry(entry));
    emit signalEntryChanged(*caEntry, changeType);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaObserver::entryTouched(int id) const
{
    qDebug() << "CaClientProxy::entryTouched id:" << id;
    emit signalEntryTouched(id);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaObserver::groupContentChanged(int groupId) const
{
    qDebug() << "CaClientProxy::groupContentChanged groupId:" << groupId;

    emit signalGroupContentChanged(groupId);
}

