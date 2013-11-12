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

#ifndef CANOTIFIERS_H
#define CANOTIFIERS_H

#include <QHash>
#include <QPair>

#include "canotifier_p.h"

class CaNotifierFilter;
class CaObserver;

class CaNotifiers
{
public:
    static int addNotifier(const CaNotifierFilter *notifierFilter,
                           CaNotifierPrivate::NotifierType notifierType,
                           const CaObserver *notifierProxy);

    static void removeNotifier(const CaNotifierFilter *notifierFilter,
                               CaNotifierPrivate::NotifierType notifierType);

    static void Notify(int groupId);

    static void NotifyTouched(int id);

    static void Notify(const CaEntry &targetEntry,
                       ChangeType changeType,
                       QList<int> &parentIds);

private:
    static bool isRegisterdForNotification(
        const CaNotifierFilter &filter,
        const CaEntry &entry,
        const QList<int> &parentIds);

private:
    typedef QPair<const CaNotifierFilter *,
            CaNotifierPrivate::NotifierType> NotifierKey;
    typedef QHash<NotifierKey, const CaObserver *> NotifierHash;

private:
    static NotifierHash mNotifiers;

};

#endif // CANOTIFIERS_H
