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

#ifndef CACLIENTPROXY_H
#define CACLIENTPROXY_H

// INCLUDES
#include "canotifier.h"

//FORWARD DECLARATIONS
class CaClientNotifierProxy;
class CaEntry;
class CaQuery;
class HsWidgetComponentDescriptor;

/**
 *  CaClientProxy
 *
 */
class CaClientProxy
{
public:

    /**
     * Insert into group type.
     */
    enum InsertType {
        AfterTheLastEntry = -1,
        BeforeTheFirstEntry = 0
    };

    /**
     * Default constructor.
     */
    CaClientProxy();

    /**
     * Destructor.
     */
    ~CaClientProxy();

    /**
     * Update widgets.
     */
    void updateWidgets();

    /**
     * Connect to server.
     * @return error code.
     */
    ErrorCode connect();

    /**
     * Method for adding entries.
     * @param entryToAdd entry.
     * @param targetEntry entry.
     * @return error code.
     */
    ErrorCode addData(const CaEntry &entryToAdd,
                      CaEntry &targetEntry);

    /**
     * Method for removing entries.
     * @param entryIdList list of ids.
     * @return error code.
     */
    ErrorCode removeData(const QList<int> &entryIdList);

    /**
     * Insert entries into group.
     * @param groupId group id.
     * @param entryIdList list of ids.
     * @param beforeEntryId id of item before which the items are inserted.
     * @return error code.
     */
    ErrorCode insertEntriesIntoGroup(int groupId,
                                     const QList<int> &entryIdList, int beforeEntryId);

    /**
     * Remove entries from group.
     * @param groupId group id.
     * @param entryIdList list of ids.
     * @param calledDirectly true if the function was called directly (not
     * from insertEntriesIntoGroup).
     * @return error code.
     */
    ErrorCode removeEntriesFromGroup(int groupId,
                                     const QList<int> &entryIdList,
                                     bool calledDirectly = true);

    /**
     * Method for fetching entries.
     *
     * @param entryIdList list of input ids.
     * @param sourceList List of results.
     * @return error code
     */
    ErrorCode getData(const QList<int>& entryIdList, QList< QSharedPointer<CaEntry> >& sourceList);

    /**
     * Method for fetching entries.
     *
     * @param query query.
     * @param sourceList List of results.
     * @return error code
     */
    ErrorCode getData(const CaQuery &query, QList< QSharedPointer<CaEntry> >& sourceList);

    /**
     * Method for fetching entries.
     *
     * @param query query.
     * @param sourceIdList List of results (ids).
     * @return error code
     */
    ErrorCode getEntryIds(const CaQuery &query,
                          QList<int>& sourceIdList);

    /**
     * Method for touching entry.
     *
     * @param entry entry to touch
     * @return error code
     */
    ErrorCode touch(const CaEntry &entry);

    /**
     * Method for custom sort.
     * @param entryIdList list of ids.
     * @param groupId Group id.
     * @return error code.
     */
    ErrorCode customSort(const QList<int> &entryIdList, int groupId);

    /**
     * Generate a list of parent ids of the given entries.
     * @param entryIds a list of entries.
     * @param parentIds a list of parent ids.
     * @return true if the operation is successful.
     */
    bool GetParentsIds(const QList<int> &entryIds,
                       QList<int> &parentIds);

private:
    int hsWidgetId(int uid);
    void addWidgetEntry(const HsWidgetComponentDescriptor &widgetToken, int widgetId);

    void modifyQueryForSortOrder(QString &queryString,
                                 const CaQuery &query, bool parent) const;
    bool setIconInDb(CaEntry *entryClone) const;
    bool setEntryInDb(CaEntry *entryClone) const;
    bool setAttributesInDb(CaEntry *entryClone) const;
    void CreateGetParentsIdsQuery(
        const QList<int> &entryIds,
        const QList<int> &parentIds,
        QString &query);
    int getEntryIdByUid(const CaEntry &entry, const int uid);

private:
    QString mWidgetRegistryPath;
};

#endif // CACLIENTPROXY_H
