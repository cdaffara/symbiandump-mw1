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
 * Description: canotifier_p.h
 *
 */

#ifndef CANOTIFIERFILTER_PRIVATE_H
#define CANOTIFIERFILTER_PRIVATE_H

#include <QList>
#include <QStringList>

#include "cadefs.h"

class CaNotifierFilter;

class CaNotifierFilterPrivate
{

public:

    explicit
    CaNotifierFilterPrivate(CaNotifierFilter *notifierFilterPublic);
    CaNotifierFilterPrivate &operator=(
        const CaNotifierFilterPrivate &notifierFilterPrivate);
    ~CaNotifierFilterPrivate();

    int getParentId() const;
    void setParentId(int parentId);

    QList<int> getIds() const;
    void setIds(const QList<int> &ids);

    EntryRoles getEntryRole() const;
    void setEntryRole(EntryRoles entryRole);

    QStringList getTypeNames() const;
    void setTypeNames(const QStringList &typeNames);

private:

    int mParentId;
    QList<int> mIds;
    EntryRoles mEntryRoles;
    QStringList mTypeNames;

private:

    CaNotifierFilter *const m_q;

};

#endif // CANOTIFIERFILTER_PRIVATE_H
