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
 * Description: caquery_p.h
 *
 */

#ifndef CAQUERY_PRIVATE_H
#define CAQUERY_PRIVATE_H

#include <QString>
#include <QStringList>

#include "cadefs.h"

class CaQuery;

class CaQueryPrivate
{
public:

    explicit CaQueryPrivate(CaQuery *queryPublic);
    CaQueryPrivate &operator=(const CaQueryPrivate &queryPrivate);
    ~CaQueryPrivate();

    EntryRoles entryRoles() const;
    void setEntryRoles(EntryRoles entryRoles);

    int parentId() const;
    void setParentId(int id);

    int childId() const;
    void setChildId(int id);
    
    QStringList entryTypeNames() const;
    void setEntryTypeNames(const QStringList &entryTypeNames);

    void addEntryTypeName(const QString &entryTypeName);

    void setFlagsOn(const EntryFlags &onFlags);
    EntryFlags flagsOn() const;
    void setFlagsOff(const EntryFlags &offFlags);
    EntryFlags flagsOff() const;

    void getSort(SortAttribute &sortAttribute,
                 Qt::SortOrder &sortOrder) const;
    void setSort(SortAttribute sortAttribute,
                 Qt::SortOrder = Qt::AscendingOrder);

    unsigned int count() const;
    void setCount(unsigned int count);

    QMap<QString, QString> attributes() const;
    QString attribute(const QString &name) const;
    void setAttribute(const QString &name, const QString &value);
    void removeAttribute(const QString &name);
    
    void clear();

private:

    CaQuery *const m_q;

    EntryRoles mEntryRoles;

    int mParentId;
    
    int mChildId;

    QStringList mEntryTypeNames;

    EntryFlags mFlagsOn;

    EntryFlags mFlagsOff;

    SortAttribute mSortAttribute;

    Qt::SortOrder mSortOrder;

    unsigned int mCount;

    QMap<QString, QString> mAttributes;
};

#endif //CAQUERY_PRIVATE_H
