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
 * Description: cadefs.h
 *
 */

#ifndef CADEFS_H
#define CADEFS_H

#include <QFlags>
#include <QMetaType>

enum ChangeType
{
    AddChangeType,
    RemoveChangeType,
    UpdateChangeType
};

enum EntryRole
{
    ItemEntryRole = 1,
    GroupEntryRole = 2
};
Q_DECLARE_FLAGS(EntryRoles, EntryRole)
Q_DECLARE_OPERATORS_FOR_FLAGS(EntryRoles)

enum SortAttribute
{
    NameSortAttribute,
    CreatedTimestampSortAttribute,
    MostUsedSortAttribute,
    LastUsedSortAttribute,
    DefaultSortAttribute
// default - return items in custom order
};

enum EntryFlag
{
    UsedEntryFlag = 1,
    UninstallEntryFlag = 2,
    RemovableEntryFlag = 4,
    VisibleEntryFlag = 8,
    ReservedEntryFlag = 16, //flag is unused, stays for compatibility
    NativeEntryFlag = 32,
    MissingEntryFlag = 64
};

enum LocalizationType
{
    NameLocalized,
    DescriptionLocalized
};

Q_DECLARE_FLAGS(EntryFlags, EntryFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(EntryFlags)
Q_DECLARE_METATYPE(EntryFlags)

enum ErrorCode
{
    NoErrorCode = 0,
    NotFoundErrorCode,
    OutOfMemoryErrorCode,
    BadArgumentErrorCode,
    ServerTerminated,
    UnknownErrorCode
};

const char caCmdOpen[] = "open";
const char caCmdRemove[] = "remove";

#ifndef QT_NO_DEBUG_OUTPUT
#   define USE_QDEBUG_IF(predicate) if (!predicate) {} else qDebug()
#else
#   define USE_QDEBUG_IF(predicate) if (1) {} else qDebug()
#endif

#ifndef QT_NO_WARNING_OUTPUT
#   define USE_QWARNING_IF(predicate) if (!predicate) {} else qWarning()
#else
#   define USE_QWARNING_IF(predicate) if (1) {} else qWarning()
#endif

#endif // CADEFS_H
