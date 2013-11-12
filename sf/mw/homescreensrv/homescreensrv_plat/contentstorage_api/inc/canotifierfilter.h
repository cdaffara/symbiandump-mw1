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
 * Description: canotifierfilter.h
 *
 */

#ifndef CANOTIFIERFILTER_H
#define CANOTIFIERFILTER_H

#include <QList>

#include "cadefs.h"
#include "caclient_global.h"
#include "caentry.h"

class CaNotifierFilterPrivate;
class CaQuery;

class CACLIENT_EXPORT CaNotifierFilter
{
    friend class CaNotifierFilterPrivate;

public:

    CaNotifierFilter();
    CaNotifierFilter(const CaNotifierFilter &notifierFilter);
    explicit CaNotifierFilter(const CaQuery &query);
    CaNotifierFilter &operator=(const CaNotifierFilter &notifierFilter);
    virtual ~CaNotifierFilter();

    int getParentId() const;
    void setParentId(int parentId);

    QList<int> getIds() const;
    void setIds(const QList<int> &ids);

    EntryRoles getEntryRole() const;
    void setEntryRole(EntryRoles entryRole);

    QStringList getTypeNames() const;
    void setTypeNames(const QStringList &typeNames);

private:

    CaNotifierFilterPrivate * const m_d;

};

#endif // CANOTIFIERFILTER_H
