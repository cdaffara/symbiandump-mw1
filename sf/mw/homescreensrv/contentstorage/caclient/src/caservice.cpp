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
 * Description: caservice.cpp
 *
 */

#include <QMetaType>
#include <QCoreApplication>
#include <QString>
#include <QDebug>
#include <QCoreApplication>

#include "caservice.h"
#include "caservice_p.h"
#include "caquery.h"
#include "caentry.h"
#include "canotifier.h"
#include "canotifier_p.h"
#include "cadefs.h"

#include "caclientproxy.h"
#include "caclientnotifierproxy.h"
#include "cahandlerproxy.h"
#include "caqtsfhandlerloader.h"
#include "caobjectadapter.h"
#include "caclienttest_global.h"

// ======== MEMBER FUNCTIONS ========

/*!

 \class CaService.
 \brief This class operates on data, enables creating and inserting new entry
 to DB, removing entry from DB, update entry or get entry from DB, execute
 command on entry and create notifier to notify a client about changes onto DB.

 CaService class uses singleton design pattern, so that it contains static
 method called instance() to get instance of a class.

 References to CaService instance are counted. When the number of references
 drops to zero the instance is deleted to save memory.
 CaNotifier class contains a member referencing to the CaService instance so
 it is not deleted unless all CaNotifier instances are deleted (and there are
 no variables referencing CaService).

 \code
 QSharedPointer<CaService> service = CaService::instance();
 \endcode

 For every operations on data is used always one instantiation of the class.
 Below, there are examples how to create data and work on those ones.

 */

/*!
 \var CaServicePrivate::m_q
 Points to the CaService instance that uses this private implementation.
 */

/*!
 \var CaServicePrivate::mProxy
 Proxy to communicate with Symbian server.
 */

/*!
 \var CaServicePrivate::mErrorCode
 code of error caused by last operation.
 */

/*!
 \var CaServicePrivate::mNotifierProxy
 Proxy to client notifier.
 */

namespace Hs {
    const char packageTypeName[] = "package";
}

// Initialization of a static member variable.
QWeakPointer<CaService> CaService::m_instance = QWeakPointer<CaService>();
/*!
 Constructor.
 \param parent pointer to a parent. It defaults to NULL.
 */
CaService::CaService(QObject *parent) :
    QObject(parent), m_d(new CaServicePrivate(this))
{
}

/*!
 Returns a pointer to the instance of the CaService class.
 \retval A pointer to the instance of the CaService class.
 */
QSharedPointer<CaService> CaService::instance()
{
    QSharedPointer<CaService> service(m_instance);
    if (!service) {
        // Register custom types used as types of signal parameters.
        qRegisterMetaType<ChangeType> ("ChangeType");
        qRegisterMetaType<CaEntry> ("CaEntry");

        service = QSharedPointer<CaService>(new CaService);
        m_instance = service.toWeakRef();
        
        const QString iconEnginesPluginPath("resource/iconengines/plugins");
        QCoreApplication::addLibraryPath(iconEnginesPluginPath);
    }
    return service;
}

/*!
 Destructor.
 */
CaService::~CaService()
{
    delete m_d;
}

/*!
 Search for an entry
 \param entryId id of the entry to find.
 \retval pointer to the found entry.

 \example
 \code
 ...
 CaEntry * entryFromDB = service->getEntry( newItem->id() );

 \endcode
 */
QSharedPointer<CaEntry> CaService::getEntry(int entryId) const
{
    QList< QSharedPointer<CaEntry> > entries = getEntries(QList<int> () << entryId);
    if (entries.count()) {
        // there should be exactly one entry with specified ID if present
        Q_ASSERT(entries.count() == 1);
        return entries[0];
    } else {
        return QSharedPointer<CaEntry>();
    }
}

/*!
 Search for entries.
 \param entryIdList list of ids.
 \return list of found entries.

 \example
 \code
 ...
 QList<CaEntry *> entriesFromDB = service->getEntries(
 QList<int>() << newItem->id() << newGroup->id() );
 QList<int> idsList;
 QStringList textEntry;
 CaEntry *entry(NULL);
 foreach( entry, entriesFromDB ) {
 idsList << entry->id();
 textEntry << entry->text();
 }

 \endcode
 */
QList< QSharedPointer<CaEntry> > CaService::getEntries(const QList<int> &entryIdList) const
{
    return m_d->getEntries(entryIdList);
}

/*!
 Search for entries.
 \param query query
 \retval list of entries

 \example
 \code
 ...
 CaQuery query;
 query.setEntryRoles( ItemEntryRole | GroupEntryRole );
 query.setEntryTypeNames( QStringList() << "TypeNameGroup" << "ItemNameGroup" );
 QList<CaEntry *> entriesFromDB = service->getEntries( query );
 QList<int> idsList;
 QStringList typeNames;
 CaEntry *entry(NULL);
 foreach( entry, entriesFromDB ) {
 idsList << entry->id();
 typeNames << entry->entryTypeName();
 }

 \endcode
 */
QList< QSharedPointer<CaEntry> > CaService::getEntries(const CaQuery &query) const
{
    return m_d->getEntries(query);
}

/*!
 Search for entries and return their ids.
 \param query query
 \retval list of ids

 \example
 \code
 ...
 CaQuery query;
 query.setEntryRoles( ItemEntryRole | GroupEntryRole );
 query.setEntryTypeNames( QStringList() << "TypeNameGroup" << "ItemNameGroup" );
 QList<int> entriesIdsFromDB = service->getEntryIds( query );
 QList<int> idsList;
 QStringList typeNames;
 int id( -1 );
 foreach( id, entriesIdsFromDB ) {
 idsList << id
 typeNames << entry->entryTypeName();
 }

 \endcode
 */
QList<int> CaService::getEntryIds(const CaQuery &query) const
{
    return m_d->getEntryIds(query);
}

/*!
 Create copy of entry.
 \param entry entry to copy
 \retval pointer to newely created copy

 \example
 \code
 ...
 // creating new entry and add to DB
 CaEntry group( GroupEntryRole );
 group.setText( "TextGroup" );
 group.setTypeName( "TypeNameGroup" );

 CaEntry item( ItemEntryRole );
 item.setText( "TextItem" );
 item.setTypeName( "TypeNameItem" );

 CaEntry * newGroup = service->createEntry( group );
 CaEntry * newItem = service->createEntry( item );
 ...
 \endcode
 */
QSharedPointer<CaEntry> CaService::createEntry(const CaEntry &entry) const
{
    return m_d->createEntry(entry);
}

/*!
 Remove entry.
 \param entryId id of an entry to remove
 \retval true if operation was successful

 \example
 \code
 ...
 bool result = service->removeEntry( newItem->id() );
 \b Output:
 result == true
 \endcode
 */
bool CaService::removeEntry(int entryId) const
{
    return removeEntries(QList<int> () << entryId);
}

/*!
 Remove entries.
 \param entry entry to remove
 \retval true if operation was successful

 \example
 \code
 ...
 bool result = service->removeEntry( *newItem );
 \b Output:
 result == true
 \endcode
 */
bool CaService::removeEntry(const CaEntry &entry) const
{
    return removeEntry(entry.id());
}

/*!
 Remove entries.
 All others are convenience methods implemented in terms of this one.
 \param entryIdList list of ids of entries to remove
 \retval true if operation was successful

 \example
 \code
 ...
 bool result = service->removeEntries( QList<int>() << newItem->id() );
 \b Output:
 result == true
 \endcode
 */
bool CaService::removeEntries(const QList<int> &entryIdList) const
{
    return m_d->removeEntries(entryIdList);
}

/*!
 Remove entries.
 \param entryList list of entries to remove
 \retval true if operation was successful

 \example
 \code
 ...
 bool result = service->removeEntries( QList< QSharedPointer<CaEntry> >() << newItem );
 \b Output:
 result == true
 \endcode
 */
bool CaService::removeEntries(const QList< QSharedPointer<CaEntry> > &entryList) const
{
    QList<int> idList;
    QSharedPointer<CaEntry> entry;
    foreach(entry, entryList) {
        idList << entry->id();
    }
    return removeEntries(idList);
}

/*!
 Update entry.
 \param entry entry to update
 \retval true if operation was successful

 \example
 \code
 ...
 newItem->setAttribute( "attrName", "attrValue" );
 bool result = service->updateEntry( *newItem );
 \b Output:
 result == true
 \endcode
 */
bool CaService::updateEntry(const CaEntry &entry) const
{
    return m_d->updateEntry(entry);
}

/*!
 Performs touch operation on entry.
 \param entry of entry to be touched
 \retval boolean with result of operation

 \example
 \code
 ...
 bool result = service->touch( newItem );
 \b Output:
 result == true
 \endcode
 */
bool CaService::touch(const CaEntry &entry) const
{
    return m_d->touch(entry);
}

/*!
 Insert entry into a given group.
 \param groupId id of a group.
 \param entryId id of an entry to insert.
 \param beforeEntryId id of an entry before which the given entry is inserted.
 \retval true if operation was successful.

 \example
 \code
 ...
 CaEntry itemToAdd;
 itemToAdd.setText( "TextAdd" );
 itemToAdd.setTypeName( "TypeNameAdd" );
 CaEntry * entryToAdd = service->createEntry( itemToAdd );
 ...
 bool result = service->insertEntryIntoGroup(
 newGroup->id(), entryToAdd->id(), newItem->id());
 \b Output:
 result == true
 \endcode
 */
bool CaService::insertEntryIntoGroup(int groupId, int entryId,
                                     int beforeEntryId) const
{
    return insertEntriesIntoGroup(groupId, QList<int> () << entryId,
                                  beforeEntryId);
}

/*!
 Insert entry into a given group.
 \param group group.
 \param entry entry to insert.
 \param beforeEntryId id of an entry before which the given entry is inserted.
 \retval true if operation was successful.

 \example
 \code
 ...
 CaEntry itemToAdd;
 itemToAdd.setText( "TextAdd" );
 itemToAdd.setTypeName( "TypeNameAdd" );
 CaEntry * entryToAdd = service->createEntry( itemToAdd );
 ...
 bool result = service->insertEntryIntoGroup(
 *newGroup, *entryToAdd, newItem->id());
 \b Output:
 result == true
 \endcode
 */
bool CaService::insertEntryIntoGroup(const CaEntry &group,
                                     const CaEntry &entry, int beforeEntryId) const
{
    return insertEntryIntoGroup(group.id(), entry.id(), beforeEntryId);
}

/*!
 Insert entries into a given group.
 All others are convenience methods implemented in terms of this one.
 \param groupId id of a group.
 \param entryIdList list of ids of entries to insert.
 \param beforeEntryId id of an entry before which the given entry is inserted.
 \retval true if operation was successful.

 \example
 \code
 ...
 CaEntry itemToAdd;
 itemToAdd.setText( "TextAdd" );
 itemToAdd.setTypeName( "TypeNameAdd" );
 CaEntry * entryToAdd = service->createEntry( itemToAdd );
 ...
 bool result = service->insertEntriesIntoGroup(
 newGroup->id(), QList<int>() << entryToAdd->id(), newItem->id());
 \b Output:
 result == true
 \endcode
 */
bool CaService::insertEntriesIntoGroup(int groupId,
                                       const QList<int> &entryIdList, int beforeEntryId) const
{
    if (beforeEntryId < 0) {
        qWarning(
            "CaService::insertEntriesIntoGroup: beforeEntryId cannot be negative: %d",
            beforeEntryId);
        return false;
    } else {
        return m_d->insertEntriesIntoGroup(groupId, entryIdList,
                                           beforeEntryId);
    }
}

/*!
 Insert entry into a given group.
 \param group group.
 \param entryList list of entries (pointers) to insert.
 \param beforeEntryId id of an entry before which the given entry is inserted.
 \retval true if operation was successful.

 \example
 \code
 ...
 CaEntry itemToAdd;
 itemToAdd.setText( "TextAdd" );
 itemToAdd.setTypeName( "TypeNameAdd" );
 CaEntry * entryToAdd = service->createEntry( itemToAdd );
 ...
 bool result = service->insertEntriesIntoGroup(
 *newGroup, QList<CaEntry *>() << entryToAdd, newItem->id());
 \b Output:
 result == true
 \endcode
 */
bool CaService::insertEntriesIntoGroup(const CaEntry &group,
                                       const QList< QSharedPointer<CaEntry> > &entryList, int beforeEntryId) const
{
    QList<int> idList;
    QSharedPointer<CaEntry> entry;
    foreach(entry, entryList) {
        idList << entry->id();
    }
    return insertEntriesIntoGroup(group.id(), idList, beforeEntryId);
}

/*!
 Remove entry from a given group.
 \param groupId id of a group.
 \param entryId id of entry to remove.
 \retval true if operation was successful.

 \example
 \code
 ...
 CaEntry itemToAddRemove;
 itemToAddRemove.setText( "TextAddRemove" );
 itemToAddRemove.setTypeName( "TypeNameAddRemove" );
 CaEntry * entryToAddRemove = service->createEntry( itemToAddRemove );
 service->prependEntryToGroup( *newGroup, *entryToAddRemove );
 ...
 bool result = service->removeEntryFromGroup(
 newGroup->id(), entryToAddRemove->id());
 \b Output:
 result == true
 \endcode
 */
bool CaService::removeEntryFromGroup(int groupId, int entryId) const
{
    return removeEntriesFromGroup(groupId, QList<int> () << entryId);
}

/*!
 Remove entry from a given group.
 \param group group.
 \param entryId id of entry to remove.
 \retval true if operation was successful.

 \example
 \code
 ...
 CaEntry itemToAddRemove;
 itemToAddRemove.setText( "TextAddRemove" );
 itemToAddRemove.setTypeName( "TypeNameAddRemove" );
 CaEntry * entryToAddRemove = service->createEntry( itemToAddRemove );
 service->prependEntryToGroup( *newGroup, *entryToAddRemove );
 ...
 bool result = service->removeEntryFromGroup(
 *newGroup, *entryToAddRemove );
 \b Output:
 result == true
 \endcode
 */
bool CaService::removeEntryFromGroup(const CaEntry &group,
                                     const CaEntry &entry) const
{
    return removeEntryFromGroup(group.id(), entry.id());
}

/*!
 Remove entries from a given group.
 All others are convenience methods implemented in terms of this one.
 \param groupId id of a group.
 \param entryIdList list of ids of entries to remove.
 \retval true if operation was successful.

 \example
 \code
 ...
 CaEntry itemToAddRemove;
 itemToAddRemove.setText( "TextAddRemove" );
 itemToAddRemove.setTypeName( "TypeNameAddRemove" );
 CaEntry * entryToAddRemove = service->createEntry( itemToAddRemove );
 service->prependEntryToGroup( *newGroup, *entryToAddRemove );
 ...
 bool result = service->removeEntriesFromGroup(
 newGroup->id(), QList<int>() << entryToAddRemove->id() );
 \b Output:
 result == true
 \endcode
 */
bool CaService::removeEntriesFromGroup(int groupId,
                                       const QList<int> &entryIdList) const
{
    return m_d->removeEntriesFromGroup(groupId, entryIdList);
}

/*!
 Remove entries from a given group.
 All others are convenience methods implemented in terms of this one.
 \param group group.
 \param entryList list of entries (pointers) to remove.
 \retval true if operation was successful.

 \example
 \code
 ...
 CaEntry itemToAddRemove;
 itemToAddRemove.setText( "TextAddRemove" );
 itemToAddRemove.setTypeName( "TypeNameAddRemove" );
 CaEntry * entryToAddRemove = service->createEntry( itemToAddRemove );
 service->prependEntryToGroup( *newGroup, *entryToAddRemove );
 ...
 bool result = service->removeEntriesFromGroup(
 *newGroup, QList<CaEntry *>() << entryToAddRemove );
 \b Output:
 result == true
 \endcode
 */
bool CaService::removeEntriesFromGroup(const CaEntry &group,
                                       const QList< QSharedPointer<CaEntry> > &entryList) const
{
    QList<int> idList;
    QSharedPointer<CaEntry> entry;
    foreach(entry, entryList) {
        idList << entry->id();
    }
    return removeEntriesFromGroup(group.id(), idList);
}

/*!
 Place entries in a given group at the end.
 \param groupId id of a group.
 \param entryId id of entry to append.
 \retval true if operation was successful.

 \example
 \code
 ...
 CaEntry itemToAppend;
 itemToAppend.setText( "TextAppend" );
 itemToAppend.setTypeName( "TypeNameAppend" );
 CaEntry * entryToAppend = service->createEntry( itemToAppend );
 bool result = service->appendEntryToGroup(
 newGroup->id(), entryToAppend->id() );
 ...
 \b Output:
 result == true
 \endcode
 */
bool CaService::appendEntryToGroup(int groupId, int entryId) const
{
    return appendEntriesToGroup(groupId, QList<int> () << entryId);
}

/*!
 Place entries in a given group at the end.
 \param group group.
 \param entry entry to append.
 \retval true if operation was successful.

 \example
 \code
 ...
 CaEntry itemToAppend;
 itemToAppend.setText( "TextAppend" );
 itemToAppend.setTypeName( "TypeNameAppend" );
 CaEntry * entryToAppend = service->createEntry( itemToAppend );
 bool result = service->appendEntryToGroup(
 *newGroup, *entryToAppend );
 ...
 \b Output:
 result == true
 \endcode
 */
bool CaService::appendEntryToGroup(const CaEntry &group,
                                   const CaEntry &entry) const
{
    return appendEntryToGroup(group.id(), entry.id());
}

/*!
 Place entries in a given group at the end.
 All others are convenience methods implemented in terms of this one.
 \param groupId group.
 \param entryIdList list of ids of entries to append.
 \retval true if operation was successful.

 \example
 \code
 ...
 CaEntry itemToAppend;
 itemToAppend.setText( "TextAppend" );
 itemToAppend.setTypeName( "TypeNameAppend" );
 CaEntry * entryToAppend = service->createEntry( itemToAppend );
 bool result = service->appendEntriesToGroup(
 newGroup->id(), QList<int>() << entryToAppend->id() );
 ...
 \b Output:
 result == true
 \endcode
 */
bool CaService::appendEntriesToGroup(int groupId,
                                     const QList<int> &entryIdList) const
{
    return m_d->appendEntriesToGroup(groupId, entryIdList);
}

/*!
 Place entries in a given group at the end.
 \param group group.
 \param entryList list of entries to append.
 \retval true if operation was successful.

 \example
 \code
 ...
 CaEntry itemToAppend;
 itemToAppend.setText( "TextAppend" );
 itemToAppend.setTypeName( "TypeNameAppend" );
 CaEntry * entryToAppend = service->createEntry( itemToAppend );
 bool result = service->appendEntriesToGroup(
 *newGroup, QList< QSharedPointer<CaEntry> >() << entryToAppend );
 ...
 \b Output:
 result == true
 \endcode
 */
bool CaService::appendEntriesToGroup(const CaEntry &group,
                                     const QList< QSharedPointer<CaEntry> > &entryList) const
{
    QList<int> idList;
    QSharedPointer<CaEntry> entry;
    foreach(entry, entryList) {
        idList << entry->id();
    }
    return appendEntriesToGroup(group.id(), idList);
}

/*!
 Place entries in a given group at the beginning.
 \param groupId id of a group.
 \param entryId id of entry to prepend.
 \retval true if operation was successful.

 \example
 \code
 ...
 CaEntry itemToPrepend;
 itemToPrepend.setText( "TextPrepend" );
 itemToPrepend.setTypeName( "TypeNamePrepend" );
 CaEntry * entryToPrepend = service->createEntry( itemToPrepend );
 bool result = service->prependEntryToGroup(
 newGroup->id(), entryToPrepend->id() );
 ...
 \b Output:
 result == true
 \endcode
 */
bool CaService::prependEntryToGroup(int groupId, int entryId) const
{
    return prependEntriesToGroup(groupId, QList<int> () << entryId);
}

/*!
 Place entries in a given group at the beginning.
 \param group group.
 \param entry entry to prepend.
 \retval true if operation was successful.

 \example
 \code
 ...
 CaEntry itemToPrepend;
 itemToPrepend.setText( "TextPrepend" );
 itemToPrepend.setTypeName( "TypeNamePrepend" );
 CaEntry * entryToPrepend = service->createEntry( itemToPrepend );
 bool result = service->prependEntryToGroup(
 *newGroup, *entryToPrepend );
 ...
 \b Output:
 result == true
 \endcode
 */
bool CaService::prependEntryToGroup(const CaEntry &group,
                                    const CaEntry &entry) const
{
    return prependEntryToGroup(group.id(), entry.id());
}

/*!
 Place entries in a given group at the beginning.
 All others are convenience methods implemented in terms of this one.
 \param groupId group.
 \param entryIdList list of ids of entries to prepend.
 \retval true if operation was successful.

 \example
 \code
 ...
 CaEntry itemToPrepend;
 itemToPrepend.setText( "TextPrepend" );
 itemToPrepend.setTypeName( "TypeNamePrepend" );
 CaEntry * entryToPrepend = service->createEntry( itemToPrepend );
 bool result = service->prependEntriesToGroup(
 newGroup->id(), QList<int>() << entryToPrepend->id() );
 ...
 \b Output:
 result == true
 \endcode
 */
bool CaService::prependEntriesToGroup(int groupId,
                                      const QList<int> &entryIdList) const
{
    return m_d->prependEntriesToGroup(groupId, entryIdList);
}

/*!
 Place entries in a given group at the begin.
 \param group group.
 \param entryList list of entries to prepend.
 \retval true if operation was successful.

 \example
 \code
 ...
 CaEntry itemToPrepend;
 itemToPrepend.setText( "TextPrepend" );
 itemToPrepend.setTypeName( "TypeNamePrepend" );
 CaEntry * entryToPrepend = service->createEntry( itemToPrepend );
 bool result = service->prependEntriesToGroup(
 *newGroup, QList<CaEntry *>() << entryToPrepend );
 ...
 \b Output:
 result == true
 \endcode
 */
bool CaService::prependEntriesToGroup(const CaEntry &group,
                                      const QList< QSharedPointer<CaEntry> > &entryList) const
{
    QList<int> idList;
    QSharedPointer<CaEntry> entry;
    foreach(entry, entryList) {
        idList << entry->id();
    }
    return prependEntriesToGroup(group.id(), idList);
}

/*!
 Execute command.
 \param entryId id of an entry.
 \param command command.
 \param receiver QObject class with slot.
 \param member Slot from QObject class.
 \retval 0 if operation was successful.

 \example
 \code
 ...
 CaEntry itemExecute;
 itemExecute.setText("Application");
 itemExecute.setTypeName("application");
 itemExecute.setAttribute("application:uid", "0x12345678");
 CaEntry * entryExecute = service->createEntry(itemExecute->id());
 int result = service->executeCommand(entryExecute->id(), "remove");
 ...
 \b Output:
 result == 0
 \endcode
 */
int CaService::executeCommand(int entryId, const QString &command, 
        QObject* receiver, const char* member) const
{
    int result = -19;
    
    const QSharedPointer<CaEntry> temporaryEntry = getEntry(entryId);
    
    if (!temporaryEntry.isNull()) {
        result = executeCommand(*temporaryEntry, command, receiver, member);
    }
    return result;
}

/*!
 Execute command.
 \param entry entry.
 \param command command.
 \param receiver QObject class with slot.
 \param member Slot from QObject class.
 \retval 0 if operation was successful.

 \example
 \code
 ...
 CaEntry itemExecute;
 itemExecute.setText("URL");
 itemExecute.setTypeName("url");
 itemExecute.setAttribute("url", "http://www.nokia.com");
 CaEntry * entryExecute = service->createEntry(itemExecute->id());
 int result = service->executeCommand(*entryExecute, "open");
 ...
 \b Output:
 result == 0
 \endcode
 */
int CaService::executeCommand(const CaEntry &entry, const QString &command, 
        QObject* receiver, const char* member) const
{
    return m_d->executeCommand(entry, command, receiver, member);
}

/*!
 Creates notifier.
 \param const reference to CaNotifierFilter.
 \retval pointer to created CaNotifier instance.

 \code
 ...
 // creating notifier with specified notifier filter
 CaNotifierFilter notifierFilter;
 notifierFilter.setIds( QList<int>() << newGroup->id() << newItem->id() );
 notifierFilter.setEntryRole( ItemEntryRole | GroupEntryRole );
 CaNotifier * notifier = service->createNotifier( notifierFilter );
 ...
 \endcode
 */
CaNotifier *CaService::createNotifier(const CaNotifierFilter &filter) const
{
    return m_d->createNotifier(filter);
}

/*!
    Set new order of collection's items set by user.
    \groupId Group id.
    \param entryIdList consists of new order of items.
    \retval true if new order of collection's items is set correctly,
     otherwise return false.
 */

bool CaService::customSort(int groupId, QList<int> &entryIdList) const
{
    return m_d->customSort(groupId, entryIdList);
}

/*!
 Returns code of an error caused by the last executed operation.
 \retval code of error, zero means no error.

 \example
 \code
 ...
 CaEntry itemExecute;
 itemExecute.setText("URL");
 itemExecute.setTypeName("url");
 itemExecute.setAttribute("url", "http://www.nokia.com");
 CaEntry * entryExecute = service->createEntry(itemExecute->id());
 bool result = service->executeCommand(*entryExecute, "open");

 ErrorCode errorCode = lastError();
 switch (errorCode) {
 case NoErrorCode:
 ...
 case BadArgumentErrorCode:
 ...
 }
 ...

 \endcode
 */
ErrorCode CaService::lastError() const
{
    return m_d->lastError();
}


/*!
 Constructor
 \param servicePublic pointer to public service
 */
CaServicePrivate::CaServicePrivate(CaService *servicePublic) :
    m_q(servicePublic),
    mCommandHandler(new CaHandlerProxy(QSharedPointer<CaHandlerLoader>
        (new CaQtSfHandlerLoader()))),
    mProxy(new CaClientProxy()),
    mNotifierProxy(NULL)
{
    const ErrorCode connectionResult = mProxy->connect();

    USE_QWARNING_IF(connectionResult)
            << "CaServicePrivate::CaServicePrivate - unable to connect proxy";

    mErrorCode = connectionResult;
}

/*!
 destructor
 */
CaServicePrivate::~CaServicePrivate()
{
    delete mProxy;
    delete mNotifierProxy;
}

/*!
 Search for entries.
 \param entryIdList list of entry ids
 \retval list of entries (pointers)
 */
QList< QSharedPointer<CaEntry> > CaServicePrivate::getEntries(
    const QList<int> &entryIdList) const
{
    qDebug() << "CaServicePrivate::getEntries"
             << "entryIdList:" << entryIdList;

    CACLIENTTEST_FUNC_ENTRY("CaServicePrivate::getEntries");

    QList< QSharedPointer<CaEntry> > resultList;

    mErrorCode = mProxy->getData(entryIdList, resultList);

    // figure out whether all entries have been retrieved and
    // store the operation status
    if (mErrorCode == NoErrorCode
            && entryIdList.count() != resultList.count()) {
        mErrorCode = NotFoundErrorCode;
    }//one line with else if
    else if (mErrorCode == ServerTerminated) {
        if (!mProxy->connect()) {
            if (mNotifierProxy) {
                mNotifierProxy->connectSessions();
            }
            resultList.clear();
            mErrorCode = mProxy->getData(entryIdList, resultList);
        }
    }
    CACLIENTTEST_FUNC_EXIT("CaServicePrivate::getEntries");

    qDebug() << "CaServicePrivate::getEntries mErrorCode:"
             << mErrorCode;

    return resultList;
}

/*!
 Fetches data from database.
 \param query the query information to select specific entries.
 \param placeholder list of entries for a specific select.
 */
QList< QSharedPointer<CaEntry> > CaServicePrivate::getEntries(const CaQuery &query) const
{
    QList< QSharedPointer<CaEntry> > resultList;

    mErrorCode = mProxy->getData(query, resultList);
    if (mErrorCode == ServerTerminated) {
        if (!mProxy->connect()) {
            if (mNotifierProxy) {
                mNotifierProxy->connectSessions();
            }
            resultList.clear();
            mErrorCode = mProxy->getData(query, resultList);
        }
    }
    qDebug() << "CaServicePrivate::getEntries mErrorCode:" << mErrorCode;

    return resultList;
}

/*!
 Search for entries and return their ids.
 \param query query
 \retval list of ids
 */
QList<int> CaServicePrivate::getEntryIds(const CaQuery &query) const
{
    CACLIENTTEST_FUNC_ENTRY("CaItemModelList::getEntryIds");

    QList<int> resultList;
    mErrorCode = mProxy->getEntryIds(query, resultList);
    if (mErrorCode == ServerTerminated) {
        if (!mProxy->connect()) {
            if (mNotifierProxy) {
                mNotifierProxy->connectSessions();
            }
            resultList.clear();
            mErrorCode = mProxy->getEntryIds(query, resultList);
        }
    }
    CACLIENTTEST_FUNC_EXIT("CaItemModelList::getEntryIds");
    qDebug() << "CaServicePrivate::getEntryIds mErrorCode:" << mErrorCode;
    return resultList;
}

/*!
 Create copy of entry.
 \param query const reference to entry to copy
 \retval pointer to newely created copy
 */
QSharedPointer<CaEntry> CaServicePrivate::createEntry(const CaEntry &entry)
{
    qDebug() << "CaServicePrivate::createEntry"
             << "entry id:" << entry.id();

    CACLIENTTEST_FUNC_ENTRY("CaServicePrivate::createEntry");

    ErrorCode addDataResult = NoErrorCode;

    QSharedPointer<CaEntry> newEntry (new CaEntry(entry.role())); 

    QScopedPointer<CaEntry> entryClone(new CaEntry(entry));
    const int nonExistingObjectId = 0;
    CaObjectAdapter::setId(*entryClone, nonExistingObjectId);

    addDataResult =
        mProxy->addData(*entryClone, *newEntry);
    if (addDataResult == ServerTerminated) {
        if (!mProxy->connect()) {
            if (mNotifierProxy) {
                mNotifierProxy->connectSessions();
            }
            addDataResult =
                mProxy->addData(*entryClone, *newEntry);
        }
    }    

    // return empty pointer if nothing was added
    if (addDataResult != NoErrorCode) {
        newEntry.clear();
    }
    
    mErrorCode = addDataResult;

    qDebug() << "CaServicePrivate::createEntry mErrorCode:" << mErrorCode;

    CACLIENTTEST_FUNC_EXIT("CaServicePrivate::createEntry");

    return newEntry;
}

/*!
 Performs touch operation on entry.
 \param entry to be touched
 \retval boolean with result of operation
 */
bool CaServicePrivate::touch(const CaEntry &entry)
{
    qDebug() << "CaServicePrivate::touch" << "entryId: " << entry.id();
    
    mErrorCode = NoErrorCode;
    if (entry.flags() & RemovableEntryFlag &&
        (entry.flags() & UsedEntryFlag) == 0 &&
        entry.role() == ItemEntryRole &&
        entry.entryTypeName() != QString(Hs::packageTypeName)) {
           
        mErrorCode = mProxy->touch(entry);
        if (mErrorCode == ServerTerminated) {
            if (!mProxy->connect()) {
                if (mNotifierProxy) {
                    mNotifierProxy->connectSessions();
                }
                mErrorCode = mProxy->touch(entry);
            }
        }        
    }

    qDebug() << "CaServicePrivate::touch mErrorCode:" << mErrorCode;

    return (mErrorCode == NoErrorCode);
}

/*!
 Update entry.
 \param entry entry const reference of entry to update
 \retval true if operation was successful
 */
bool CaServicePrivate::updateEntry(const CaEntry &entry)
{
    qDebug() << "CaServicePrivate::updateEntry"
             << "entry id:" << entry.id();

    CACLIENTTEST_FUNC_ENTRY("CaServicePrivate::updateEntry");

    ErrorCode updateEntryResult = UnknownErrorCode;

    if (entry.id() != 0) {
        try {
            QScopedPointer<CaEntry> updatedEntry(new CaEntry(entry.role()));

            updateEntryResult = mProxy->addData(entry, *updatedEntry);
            if (updateEntryResult == ServerTerminated) {
                if (!mProxy->connect()) {
                    if (mNotifierProxy) {
                        mNotifierProxy->connectSessions();
                    }
                    updateEntryResult = mProxy->addData(entry, *updatedEntry);
                }
            }

        } catch (const std::bad_alloc &) {
            updateEntryResult = OutOfMemoryErrorCode;
        }
    }

    mErrorCode = updateEntryResult;

    qDebug() << "CaServicePrivate::updateEntry mErrorCode on return:"
             << mErrorCode;
    CACLIENTTEST_FUNC_EXIT("CaServicePrivate::updateEntry");

    return (updateEntryResult == NoErrorCode);
}

/*!
 Remove entries.
 \param entryIdList list of entries ids to remove
 \retval true if operation was successful
 */
bool CaServicePrivate::removeEntries(const QList<int> &entryIdList)
{
    qDebug() << "CaServicePrivate::removeEntries"
             << "entryIdList: " << entryIdList;

    CACLIENTTEST_FUNC_ENTRY("CaServicePrivate::removeEntries");

    mErrorCode = mProxy->removeData(entryIdList);
    if (mErrorCode == ServerTerminated) {
        if (!mProxy->connect()) {
            if (mNotifierProxy) {
                mNotifierProxy->connectSessions();
            }
            mErrorCode = mProxy->removeData(entryIdList);
        }
    }

    qDebug() << "CaServicePrivate::removeEntries mErrorCode:" << mErrorCode;

    CACLIENTTEST_FUNC_EXIT("CaServicePrivate::removeEntries");

    return (mErrorCode == NoErrorCode);
}

/*!
 Insert entry into a given group.
 \param groupId groupId.
 \param entryIdList list of entries ids to insert.
 \param beforeEntryId id of an entry before which the given entry is inserted.
 \retval true if operation was successful.
 */
bool CaServicePrivate::insertEntriesIntoGroup(
    int groupId,
    const QList<int> &entryIdList,
    int beforeEntryId) // all others are convenience methods implemented in terms of this one
{
    qDebug() << "CaServicePrivate::insertEntriesIntoGroup"
             << "groupId: " << groupId << " beforeEntryId: " << beforeEntryId
             << "entryIdList: " << entryIdList;

    CACLIENTTEST_FUNC_ENTRY("CaServicePrivate::insertEntriesIntoGroup");

    mErrorCode = mProxy->insertEntriesIntoGroup(groupId, 
            entryIdList, beforeEntryId);
    if (mErrorCode == ServerTerminated) {
        if (!mProxy->connect()) {
            if (mNotifierProxy) {
                mNotifierProxy->connectSessions();
            }
            mErrorCode = mProxy->insertEntriesIntoGroup(groupId, 
                    entryIdList, beforeEntryId);
        }
    }
    qDebug() << "CaServicePrivate::insertEntriesIntoGroup mErrorCode:"
             << mErrorCode;

    CACLIENTTEST_FUNC_EXIT("CaServicePrivate::insertEntriesIntoGroup");

    return (mErrorCode == NoErrorCode);
}

/*!
 Remove entries from a given group.

 \param groupId groupId.
 \param entryIDList list of entries ids to remove.
 \retval true if operation was successful.
 */
bool CaServicePrivate::removeEntriesFromGroup(
    int groupId,
    const QList<int> &entryIdList)
{
    qDebug() << "CaServicePrivate::removeEntriesFromGroup"
             << "groupId: " << groupId << "entryIdList" << entryIdList;

    CACLIENTTEST_FUNC_ENTRY("CaServicePrivate::removeEntriesFromGroup");

    mErrorCode = mProxy->removeEntriesFromGroup(groupId, entryIdList);
    if (mErrorCode == ServerTerminated) {
        if (!mProxy->connect()) {
            if (mNotifierProxy) {
                mNotifierProxy->connectSessions();
            }
            mErrorCode = mProxy->removeEntriesFromGroup(groupId, entryIdList);
        }
    }

    qDebug() << "CaServicePrivate::removeEntriesFromGroup mErrorCode:"
             << mErrorCode;

    CACLIENTTEST_FUNC_EXIT("CaServicePrivate::removeEntriesFromGroup");

    return (mErrorCode == NoErrorCode);
}

/*!
 Place entries in a given group at the end.
 \param groupId groupId.
 \param entryIdList list of entries ids to prepend.
 \retval true if operation was successful.
 */
bool CaServicePrivate::appendEntriesToGroup(
    int groupId,
    const QList<int> &entryIdList)
{
    qDebug() << "CaServicePrivate::appendEntriesToGroup"
             << "groupId: " << groupId << "entryIdList: " << entryIdList;

    CACLIENTTEST_FUNC_ENTRY("CaServicePrivate::appendEntriesToGroup");

    const bool result = insertEntriesIntoGroup(
                            groupId,
                            entryIdList,
                            CaClientProxy::AfterTheLastEntry);

    qDebug() << "CaServicePrivate::appendEntriesToGroup result:"
             << QString(result ? "true" : "false");

    CACLIENTTEST_FUNC_EXIT("CaServicePrivate::appendEntriesToGroup");

    return result;
}

/*!
 Place entries in a given group at the begin.
 \param groupId groupId.
 \param entryIdList list of entries ids to prepend.
 \retval true if operation was successful.
 */
bool CaServicePrivate::prependEntriesToGroup(int groupId,
        const QList<int> &entryIdList)
{
    qDebug() << "CaServicePrivate::prependEntriesToGroup"
             << "groupId: " << groupId << "entryIdList: " << entryIdList;

    CACLIENTTEST_FUNC_ENTRY("CaServicePrivate::prependEntriesToGroup");

    const bool result =
        insertEntriesIntoGroup(
            groupId,
            entryIdList,
            CaClientProxy::BeforeTheFirstEntry);

    qDebug() << "CaServicePrivate::prependEntriesToGroup result:"
             << QString(result ? "PASS" : "FAIL");

    CACLIENTTEST_FUNC_EXIT("CaServicePrivate::prependEntriesToGroup");

    return result;
}

/*!
 Executes command on entry (fe. "open", "remove")
 \param const reference to an entry on which command will be issued
 \param string containing a command
 \param receiver QObject with slot
 \param member slot from QObject
 \retval int which is used as an error code return value, 0 means no errors
 */
int CaServicePrivate::executeCommand(const CaEntry &entry,
                                      const QString &command,
                                      QObject* receiver, 
                                      const char* member)
{
    qDebug() << "CaServicePrivate::executeCommand"
             << "entry id:" << entry.id() << "command:" << command;

    CACLIENTTEST_FUNC_ENTRY("CaServicePrivate::executeCommand");

    if (entry.flags() & UninstallEntryFlag) {
        return 0;
    }    
    
    int errorCode = mCommandHandler->execute(entry, 
            command, receiver, member);
    mErrorCode = CaObjectAdapter::convertErrorCode(errorCode);
    
    if (command == caCmdOpen) {
        touch(entry);
    }

    qDebug() << "CaServicePrivate::executeCommand mErrorCode on return:"
             << mErrorCode;

    CACLIENTTEST_FUNC_EXIT("CaServicePrivate::executeCommand");

    return errorCode;
}

/*!
 Creates new notifier. Factory class
 \param CaNotifierFilter which is used be new notifier
 \retval pointer to new Notifier
 */
CaNotifier *CaServicePrivate::createNotifier(const CaNotifierFilter &filter)
{
    if (!mNotifierProxy) {
        mNotifierProxy = new CaClientNotifierProxy();
    }
    return new CaNotifier(new CaNotifierPrivate(filter, mNotifierProxy));
}

/*!
    Set new order of collection's items set by user.
    \param groupId Group id.
    \param entryIdList consists of new order of items.
    \retval true if new order of collection's items is set correctly,
     otherwise return false.
 */
bool CaServicePrivate::customSort(int groupId, QList<int> &entryIdList)
{
    CACLIENTTEST_FUNC_ENTRY("CaServicePrivate::customSort");

    mErrorCode = mProxy->customSort(entryIdList, groupId);
    if (mErrorCode == ServerTerminated) {
        if (!mProxy->connect()) {
            if (mNotifierProxy) {
                mNotifierProxy->connectSessions();
            }
            mErrorCode = mProxy->customSort(entryIdList, groupId);
        }
    }
    CACLIENTTEST_FUNC_EXIT("CaServicePrivate::customSort");

    return (mErrorCode == NoErrorCode);
}

/*!
 Returns code of an error caused by the last executed operation.
 \retval code of error, zero means no error.
 */
ErrorCode CaServicePrivate::lastError() const
{
    return mErrorCode;
}
