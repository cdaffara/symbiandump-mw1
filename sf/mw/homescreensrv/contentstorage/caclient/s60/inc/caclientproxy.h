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

#include <QMutex>
#include <QSharedPointer>

// INCLUDES
#include "cadef.h"
#include "caclientsession.h"
#include "canotifier.h"
#include "cahandlerproxy.h"

//FORWARD DECLARATIONS

class CaClientNotifierProxy;
class CaEntry;
class CaQuery;
class CaNotifierFilter;

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
     * @return error code.
     */
    ErrorCode removeEntriesFromGroup(int groupId,
                                     const QList<int> &entryIdList);

    /**
     * Method for fetching entries.
     *
     * @param entryIdList list of input ids.
     * @param sourceList List of results.
     * @return error code
     */
    ErrorCode getData(const QList<int> &entryIdList, QList< QSharedPointer<CaEntry> > &sourceList);

    /**
     * Method for fetching entries.
     *
     * @param query query.
     * @param sourceList List of results.
     * @return error code
     */
    ErrorCode getData(const CaQuery &query, QList< QSharedPointer<CaEntry> > &sourceList);

    /**
     * Method for fetching entries.
     *
     * @param query query.
     * @param sourceIdList List of results (ids).
     * @return error code
     */
    ErrorCode getEntryIds(const CaQuery &query,
                          QList<int> &sourceIdList);

    /**
     * Method for touching entry.
     *
     * @param aEntry entry to touch
     * @return error code
     */
    ErrorCode touch(const CaEntry &aEntry);

    /**
     * Method for custom sort.
     * @param entryIdList list of ids.
     * @param groupId Group id.
     * @return error code.
     */
    ErrorCode customSort(const QList<int> &entryIdList, int groupId);

private:

    /**
     * Method for adding entries. Symbian specific.
     * @param entryToAdd
     * @param targetEntry
     *
     */
    void addDataL(const CaEntry &entryToAdd,
                  CaEntry &targetEntry);

    /**
     * Method for removing entries. Symbian specific.
     * @param entryIdList list of ids of entries to remove
     *
     */
    void removeDataL(const QList<int> &entryIdList);

    /**
     * Method for inserting entries
     * @param groupId id of a group where entries will be inserted
     * @param entryIdList list of entries ids to insert
     * @param beforeEntryId id of entry in the group that points to place
     * where entries will be inserted
     *
     */
    void insertEntriesIntoGroupL(int groupId,
                                 const QList<int> &entryIdList, int beforeEntryId);

    /**
     * Method for removing entries from a group
     * @param groupId id of a group where entries will be removed
     * @param entryIdList list of entries ids to remove
     *
     */
    void removeEntriesFromGroupL(int groupId,
                                 const QList<int> &entryIdList);

    /**
     * Method for fetching entries. Symbian specific.
     * @param entryIdList list of ids
     * @param sourceList list of entries.
     */
    void getDataL(const QList<int> &entryIdList,
            QList< QSharedPointer<CaEntry> > &sourceList);

    /**
     * Method for fetching entries. Symbian specific.
     * @param entryIdList list of ids
     * @param sourceList list of entries.
     */
    void getDataL(const CaQuery &query,
            QList< QSharedPointer<CaEntry> > &sourceList);

    /**
     * Method for fetching entries. Symbian specific.
     * @param query query.
     * @param sourceList list of ids.
     */
    void getEntryIdsL(const CaQuery &query,
                      QList<int> &sourceIdList);


    /**
     * Touch.
     * @param aEntry entry to be touched.
     */
    void touchL(const CaEntry &aEntry);

    /**
     * Custom Sort.
     * @param aEntry entry to be touched.
     */
    void customSortL(const QList<int> &entryIdList, int groupId);

private:
    // Session to server.
    // Own.
    RCaClientSession mSession;

    // Mutex to serialize access to mSessions.
    QMutex mMutex;
};

#endif // CACLIENTPROXY_H
