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
 * Description: canotifierfilter.cpp
 *
 */

#include "canotifierfilter.h"
#include "canotifierfilter_p.h"
#include "caquery.h"

/*!
 \var CaNotifierFilterPrivate::m_q
 Points to the CaNotifierFilter instance that uses
 this private implementation.
 */

/*!
 Constructor.
 \param pointer to CaNotifierFilter - public notifier implementation class.
 */
CaNotifierFilterPrivate::CaNotifierFilterPrivate(
    CaNotifierFilter *notifierfilterPublic) :
    mParentId(0), m_q(notifierfilterPublic)
{

}

/*!
 Copy assignment operator.
 \param notifierFilterPrivate reference to CaNotifierFilterPrivate.
 */

CaNotifierFilterPrivate &CaNotifierFilterPrivate::operator=(
    const CaNotifierFilterPrivate &notifierFilterPrivate)
{
    /*m_q is not changed*/
    mParentId = notifierFilterPrivate.mParentId;
    mIds = notifierFilterPrivate.mIds;
    mEntryRoles = notifierFilterPrivate.mEntryRoles;
    mTypeNames = notifierFilterPrivate.mTypeNames;
    return *this;
}
/*!
 Destructor.
 */
CaNotifierFilterPrivate::~CaNotifierFilterPrivate()
{
}
/*!
 Returns parent id.
 \retval parent id.
 */
int CaNotifierFilterPrivate::getParentId() const
{
    return mParentId;
}

/*!
 Sets parent id.
 \param parentId parent id.
 */
void CaNotifierFilterPrivate::setParentId(int parentId)
{
    mParentId = parentId;
}

/*!
 Returns a list of ids.
 \retval list of ids.
 */
QList<int> CaNotifierFilterPrivate::getIds() const
{
    return mIds;
}

/*!
 Sets a list of ids.
 \param ids reference to a list of ids.
 */
void CaNotifierFilterPrivate::setIds(const QList<int> &ids)
{
    mIds = ids;
}

/*!
 Returns entry role.
 \retval entry role.
 */
EntryRoles CaNotifierFilterPrivate::getEntryRole() const
{
    return mEntryRoles;
}

/*!
 Sets an entry role.
 \param entry role.
 */
void CaNotifierFilterPrivate::setEntryRole(EntryRoles entryRole)
{
    mEntryRoles = entryRole;
}

/*!
 Returns a list of type names.
 \retval list of type names (strings).
 */
QStringList CaNotifierFilterPrivate::getTypeNames() const
{
    return mTypeNames;
}

/*!
 Sets a list of type names.
 \retval list of type names (strings).
 */
void CaNotifierFilterPrivate::setTypeNames(const QStringList &typeNames)
{
    mTypeNames = typeNames;
}

/*!
 \class CaNotifierFilter.
 \brief This class describes notifier filter.

 \example
 It's example how client can be notified about changes on data specified by client's filter

 \code
 // example Client class which wants to be notified about changes
 class Client: public QObject
 {
 Q_OBJECT
 ...
 public slots:

 updateModelForEntryChanged(int,ChangeType);
 ...
 };
 \endcode

 \code
 // example code showed how to create notifier filter
 // to specify interested data

 // Client which will be wait for notifications
 Client * client = new Client();
 ...
 QSharedPointer<CaService> service = CaService::instance();
 // create new entry
 CaEntry entry;
 entry.setText( "Text" );
 entry.setTypeName( "TypeName" );
 CaEntry * newEntry = service->createEntry( entry );

 // create notifier filter and specify entry's id is interested by client
 CaNotifierFilter notifierFilter();
 QList<int> idsList;
 idsList.append( newEntry->Id() );
 notifierFilter.setIds( idsList );
 // create notifier with prepared notifier filter
 CaNotifier * notifier = service->createNotifier( notifierfilter );

 // Connections cause that notifier is registered to server distributed
 // notifications during data changes.
 if( notifier )
 {
 connect( notifier, SIGNAL(entryChanged(int,ChangeType)),
 client, SLOT(updateModelForEntryChanged(int,ChangeType)) );
 ...
 }

 // updating newEntry causes sent notification to the client
 newEntry->setAttribute( "attrName", "attrValue" );
 service->updateEntry( *newEntry );
 ...
 \endcode

 \code
 \b Output:

 // here client is being got notifications newEntry was updated
 Client::updateModelForEntryChanged(int entryId ,ChangeType changeType)
 {
 entryId == newEntry->id();
 changeType == UpdateChangeType;
 ...
 }

 \endcode

 It's possible creating CaNotifierFilter object using CaQuery object.

 \code
 ...
 CaQuery query;
 query.setEntryRoles( ItemEntryRole );
 query.setEntryTypeNames( QStringList() << "Query1" << "TypeName");
 CaNotifierFilter notifierFilter( query );
 notifierFilter.setIds( QList<int>() << newEntry->id() );
 // create notifier with prepared notifier filter
 CaNotifier * notifier = service->createNotifier( notifierfilter );
 ...
 \endcode

 */

/*!
 Default constructor.
 */
CaNotifierFilter::CaNotifierFilter() :
    m_d(new CaNotifierFilterPrivate(this))
{
}
/*!
 Copy constructor.
 \param notifierFilter pointer to CaNotifierFilter.
 */
CaNotifierFilter::CaNotifierFilter(const CaNotifierFilter &notifierFilter) :
    m_d(new CaNotifierFilterPrivate(this))
{
    *m_d = *(notifierFilter.m_d);
}

/*!
 Constructor. It constructs notifier filter of query.
 \param query query.
 */
CaNotifierFilter::CaNotifierFilter(const CaQuery &query) :
    m_d(new CaNotifierFilterPrivate(this))
{
    m_d->setParentId(query.parentId());
    m_d->setEntryRole(query.entryRoles());
    m_d->setTypeNames(query.entryTypeNames());
}

/*!
 Copy assignment operator.
 \param notifierFilter const reference to CaNotifierFilter.
 \retval reference to CaNotifierFilter.
 */
CaNotifierFilter &CaNotifierFilter::operator=(
    const CaNotifierFilter &notifierFilter)
{
    if (this != &notifierFilter) {
        *m_d = *(notifierFilter.m_d);
    }
    return *this;
}

/*!
 Destructor.
 */
CaNotifierFilter::~CaNotifierFilter()
{
    delete m_d;
}

/*!
 Returns parent id.
 \retval parent id.
 */
int CaNotifierFilter::getParentId() const
{
    return m_d->getParentId();
}

/*!
 Sets parent id.
 \param parent id.
 */
void CaNotifierFilter::setParentId(int parentId)
{
    m_d->setParentId(parentId);
}

/*!
 Returns a list of ids.
 \retval list of ids.
 */
QList<int> CaNotifierFilter::getIds() const
{
    return m_d->getIds();
}

/*!
 Sets a list of ids.
 \param list of ids.
 */
void CaNotifierFilter::setIds(const QList<int> &ids)
{
    m_d->setIds(ids);
}

/*!
 Returns entry role.
 \retval entry role.
 */
EntryRoles CaNotifierFilter::getEntryRole() const
{
    return m_d->getEntryRole();
}

/*!
 Sets entry role.
 \param entry role to be set.
 */
void CaNotifierFilter::setEntryRole(EntryRoles entryRole)
{
    m_d->setEntryRole(entryRole);
}

/*!
 Returns a list of type names.
 \retval list of type names (strings).
 */
QStringList CaNotifierFilter::getTypeNames() const
{
    return m_d->getTypeNames();
}

/*!
 Sets a list of type names.
 \param reference to a list of type names.
 */
void CaNotifierFilter::setTypeNames(const QStringList &typeNames)
{
    m_d->setTypeNames(typeNames);
}
