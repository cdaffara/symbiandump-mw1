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
 * Description: canotifiers.cpp
 *
 */

#include <canotifierfilter.h>
#include <QStringList>

#include "canotifiers.h"
#include "caobserver.h"
#include "caclientnotifierproxy.h"

CaNotifiers::NotifierHash CaNotifiers::mNotifiers;

int CaNotifiers::addNotifier(const CaNotifierFilter *notifierFilter,
                             CaNotifierPrivate::NotifierType notifierType,
                             const CaObserver *notifierProxy)
{
    NotifierKey key = NotifierKey(notifierFilter, notifierType);
    if (!mNotifiers.contains(key)) {
        mNotifiers.insert(key, notifierProxy);
    }
    return 0;
}

void CaNotifiers::removeNotifier(const CaNotifierFilter *notifierFilter,
                                 CaNotifierPrivate::NotifierType notifierType)
{
    mNotifiers.remove(NotifierKey(notifierFilter, notifierType));
}

void CaNotifiers::Notify(int groupId)
{
    for (NotifierHash::const_iterator i = mNotifiers.constBegin();
            i != mNotifiers.constEnd();
            ++i) {
        const CaNotifierFilter *filter = i.key().first;
        if (filter->getIds().isEmpty() || filter->getIds().contains(groupId)) {
            if (i.key().second
                    == CaNotifierPrivate::GroupContentChangedNotifierType) {
                i.value()->groupContentChanged(groupId);
            }
        }
    }
}

void CaNotifiers::NotifyTouched(int id)
{
    for (NotifierHash::const_iterator i = mNotifiers.constBegin();
            i != mNotifiers.constEnd();
            ++i) {
        const CaNotifierFilter *filter = i.key().first;
        if (filter->getIds().isEmpty() || filter->getIds().contains(id)) {
            if (i.key().second == CaNotifierPrivate::EntryTouchedNotifierType) {
                i.value()->entryTouched(id);
            }
        }
    }
}

void CaNotifiers::Notify(const CaEntry &targetEntry,
                         ChangeType changeType,
                         QList<int> &parentIds)
{
    for (NotifierHash::const_iterator i = mNotifiers.constBegin();
            i != mNotifiers.constEnd();
            ++i) {
        if (isRegisterdForNotification(
                    *i.key().first, targetEntry, parentIds)) {
            if (i.key().second
                    == CaNotifierPrivate::EntryChangedWithIdNotifierType) {
                i.value()->entryChanged(targetEntry.id(), changeType);
            } else if (i.key().second
                       == CaNotifierPrivate::EntryChangedWithEntryNotifierType) {
                i.value()->entryChanged(targetEntry, changeType);
            } else if (i.key().second
                       == CaNotifierPrivate::EntryTouchedNotifierType) {
                i.value()->entryChanged(targetEntry.id(), changeType);
            }
        }
    }
}

bool CaNotifiers::isRegisterdForNotification(
    const CaNotifierFilter &filter,
    const CaEntry &entry,
    const QList<int> &parentIds)
{
    bool ret = true;
    if (!filter.getIds().isEmpty()) {
        ret = ret && filter.getIds().contains(entry.id());
    }
    if (filter.getParentId() > 0) {
        if (entry.id() == filter.getParentId()) {
            ret = true;
        } else {
            ret = ret && parentIds.contains(filter.getParentId());
        }
    }
    if (!filter.getTypeNames().isEmpty()) {
        ret = ret && filter.getTypeNames().contains(entry.entryTypeName());
    }
    if (filter.getEntryRole()) {
        ret = ret && (filter.getEntryRole().testFlag(entry.role()));
    }
    return ret;
}


