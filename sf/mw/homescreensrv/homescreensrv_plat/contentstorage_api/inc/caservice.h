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
 * Description: caservice.h
 *
 */

#ifndef CASERVICE_H
#define CASERVICE_H

#include <QObject>
#include <QSharedPointer>

#include <cadefs.h>

#include "caclient_global.h"
#include "canotifierfilter.h"

class CaQuery;
class CaEntry;
class CaServicePrivate;
class CaNotifier;

class CACLIENT_EXPORT CaService: public QObject
{

Q_OBJECT

    explicit CaService(QObject *parent = 0);

    ~CaService();

public:

    static QSharedPointer<CaService> instance();

    QSharedPointer<CaEntry> getEntry(int entryId) const;
    QList< QSharedPointer<CaEntry> > getEntries(const QList<int> &entryIdList) const;
    QList< QSharedPointer<CaEntry> > getEntries(const CaQuery &query) const;
    QList<int> getEntryIds(const CaQuery &query) const;

    QSharedPointer<CaEntry> createEntry(const CaEntry &entry) const;

    bool removeEntry(int entryId) const;
    bool removeEntry(const CaEntry &entry) const;
    bool removeEntries(const QList<int> &entryIdList) const;
    bool removeEntries(const QList< QSharedPointer<CaEntry> > &entryList) const;

    bool updateEntry(const CaEntry &entry) const;
    bool touch(const CaEntry &entry) const;

    bool insertEntryIntoGroup(int groupId, int entryId,
        int beforeEntryId) const;
    bool insertEntryIntoGroup(const CaEntry &group, const CaEntry &entry,
        int beforeEntryId) const;
    bool insertEntriesIntoGroup(int groupId, const QList<int> &entryIdList,
        int beforeEntryId) const;
    bool insertEntriesIntoGroup(const CaEntry &group,
        const QList< QSharedPointer<CaEntry> > &entryList, int beforeEntryId) const;

    bool removeEntryFromGroup(int groupId, int entryId) const;
    bool removeEntryFromGroup(const CaEntry &group,
        const CaEntry &entry) const;
    bool removeEntriesFromGroup(int groupId,
        const QList<int> &entryIdList) const;
    bool removeEntriesFromGroup(const CaEntry &group,
        const QList< QSharedPointer<CaEntry> > &entryList) const;

    bool appendEntryToGroup(int groupId, int entryId) const;
    bool appendEntryToGroup(const CaEntry &group,
        const CaEntry &entry) const;
    bool appendEntriesToGroup(int groupId,
        const QList<int> &entryIdList) const;
    bool appendEntriesToGroup(const CaEntry &group,
        const QList< QSharedPointer<CaEntry> > &entryList) const;

    bool prependEntryToGroup(int groupId, int entryId) const;
    bool prependEntryToGroup(const CaEntry &group,
        const CaEntry &entry) const;
    bool prependEntriesToGroup(int groupId,
        const QList<int> &entryIdList) const;
    bool prependEntriesToGroup(const CaEntry &group,
        const QList< QSharedPointer<CaEntry> > &entryList) const;

    int executeCommand(int entryId, const QString &command = 
        caCmdOpen, QObject* receiver = NULL, const char* member = NULL) const;
    int executeCommand(const CaEntry &entry, const QString &command =
        caCmdOpen, QObject* receiver = NULL, const char* member = NULL) const;

    CaNotifier * createNotifier(const CaNotifierFilter &filter) const;

    bool customSort(int groupId, QList<int> &entryIdList) const;

    ErrorCode lastError() const;

signals:

    void entryChanged(int entryId, ChangeType changeType);
    void entryChanged(const CaEntry &entry, ChangeType changeType);
    void entryTouched(int id);
    void groupContentChanged(int groupId);

private:

    /**
     * Pointer to a private implementation.
     */
    CaServicePrivate * const m_d;

    /**
     * The static pointer to the instance of CaService.
     */
    static QWeakPointer<CaService> m_instance;

    friend class CaServicePrivate;
    friend class QtSharedPointer::ExternalRefCount<CaService>;

    Q_DISABLE_COPY(CaService)

};


#endif // CASERVICE_H
