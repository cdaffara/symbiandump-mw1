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

#ifndef OBJECTADAPTER_H
#define OBJECTADAPTER_H

// INCLUDES

// CLASS DECLARATION
#include <QList>
#include <QSharedPointer>

#include <cadefs.h>

#include "cainnernotifierfilter.h"
#include "canotifier.h"
#include "canotifier_p.h"
#include "cainnerquery.h"

class CaEntry;
class CaQuery;
class CaNotifierFilter;
class CCaInnerEntry;
class CCaInnerNotifierFilter;
class HbIcon;
class CFbsBitmap;


/**
 *  CaObjectAdapter
 *
 */
class CaObjectAdapter
{
public:

    //Static convertion methods between Symbian and Qt objects

    /**
     * Converts entry to inner entry.
     * @param fromEntry entry to be converted
     * @param toEntry inner entry resulting from conversion
     */
    static void convertL(
        const CaEntry &fromEntry,
        CCaInnerEntry &toEntry);

    /**
     * Converts inner entry to entry.
     * @param fromEntry inner entry to be converted
     * @param toEntry entry resulting from conversino
     */
    static void convert(const CCaInnerEntry &fromEntry,
                        CaEntry &toEntry);

    /**
     * Converts query to inner query.
     * @param fromQuery query to be converted
     * @param toQuery inner query resutling from conversion
     */
    static void convertL(
        const CaQuery &fromQuery,
        CCaInnerQuery &toQuery);

    /**
     * Converts inner entry table to QList of pointers to entries.
     * @param fromEntriesArray an array containing inner entries
     * for conversion.
     * @param toEntriesArray list of converted entries.
     */
    static void convertL(
        const RPointerArray<CCaInnerEntry>& fromEntriesArray,
        QList< QSharedPointer<CaEntry> > & toEntriesArray);

    /**
     * Converts QList of ids to RArray of TInts.
     * @param fromEntryIdList list of entry id
     * @param toEntryIdArray array of entry ids
     */
    static void convertL(const QList<int> &fromEntryIdList,
                         RArray<TInt>& toEntryIdArray);

    /**
     * Converts RArray of TInts to QList of ids.
     * @param fromEntryIdList an array of entry ids.
     * @param toEntryIdList resulgin list of entry ids.
     */
    static void convertL(const RArray<TInt>& fromEntryIdList,
                         QList<int>& toEntryIdList);

    /**
     * Converts symbian error code to error code (used in QT code).
     */
    static ErrorCode convertErrorCode(
        TInt internalErrorCode);

    /**
     * Converts notifier type to inner notifier type.
     * @param from notifier type.
     * @param to resulting inner notifier type.
     */
    static void convert(
        CaNotifierPrivate::NotifierType from,
        CCaInnerNotifierFilter::TNotifierType &to);

    /**
     * Converts inner change type to change type.
     * @param from inner change type.
     * @param to resulting change type.
     */
    static void convert(
        TChangeType src, ChangeType &dest);

    /**
     * Converts filter to inner filter.
     * @param from notifier filter.
     * @param to resulting notifier filter.
     */
    static void convertL(
        const CaNotifierFilter &from,
        CCaInnerNotifierFilter &to);

    /**
     * Set entry id.
     * @param entry entry.
     * @param id entry id.
     */
    static void setId(CaEntry &entry,
                      int id);

private:

    /**
     * Maps a given sort subject and the order to be applied
     * into a sort code used
     * @param sortAttribute - what is to be sorted
     * @param sortOrder - how it is to be sorted
     * @return sort code corresponding to input parameters
     */
    static CCaInnerQuery::TSortAttribute getSortCode(
        SortAttribute sortAttribute,
        Qt::SortOrder sortOrder);

};

#endif // OBJECTADAPTER_H
