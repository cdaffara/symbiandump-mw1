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
#include "cainnerentry.h"

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
void CaObserver::entryChanged(TInt entryId,
        TChangeType changeType) const
{
    qDebug() << "CaClientProxy::entryChanged entryId:"
             << entryId << "changeType:" << changeType;

    int entryChangedId(entryId);
    ChangeType entryChangeType(AddChangeType);
    CaObjectAdapter::convert(changeType, entryChangeType);

    emit signalEntryChanged(entryChangedId, entryChangeType);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaObserver::entryChanged(const CCaInnerEntry &entry,
        TChangeType changeType) const
{
    qDebug() << "CaClientProxy::entryChanged changeType:" << changeType;

    QSharedPointer<CaEntry> caEntry(
        new CaEntry(static_cast<EntryRole>(entry.GetRole())));
    ChangeType entryChangeType(AddChangeType);
    CaObjectAdapter::convert(entry, *caEntry);
    CaObjectAdapter::convert(changeType, entryChangeType);
    

    emit signalEntryChanged(*caEntry, entryChangeType);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaObserver::entryTouched(TInt id) const
{
    qDebug() << "CaClientProxy::entryTouched id:" << id;
    emit signalEntryTouched(id);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaObserver::groupContentChanged(TInt groupId) const
{
    qDebug() << "CaClientProxy::groupContentChanged groupId:" << groupId;

    int groupChangedId(groupId);

    emit signalGroupContentChanged(groupChangedId);
}

