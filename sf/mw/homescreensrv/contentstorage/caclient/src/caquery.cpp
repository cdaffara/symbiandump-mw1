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
 * Description: caquery.cpp
 *
 */

#include <QStringList>
#include <QDebug>

#include "caquery.h"
#include "caquery_p.h"

// ======== MEMBER FUNCTIONS ========


/*!
 \class CaQuery
 \brief This class contains data describing a query used by CaService.

 Thanks to CaQuery object, it's possible to create objects easly
 using this one like CaNotifierFilter for example or define criteria during
 geting entries via service. CaQuery anable set usufull data to do that,
 including entry roles, type names, parent id, sort ordering
 and set on/off flags quickly.

 \example
 \code
 CaQuery query;
 query.setEntryRoles( ItemEntryRole | GroupEntryRole );
 query.setSort( MostUsedSortAttribute, Qt::DescendingOrder );
 query.setFlagsOn( UsedEntryFlag | RemovableEntryFlag );
 query.setFlagsOff( SystemEntryFlag | VisibleEntryFlag );
 // using CaQuery to get data from DB
 QList<CaEntry *> entries = CaService::instance()->getEntries( query );
 ...
 // using CaQuery to create notifier
 CaQuery queryNot( query );
 CaNotifierFilter notifierFilter( queryNot );
 CaNotifier * notifier = CaService::instance()->createNotifier( notifierFilter );
 ...

 \endcode
 */

/*!
 \var CaQueryPrivate::m_q
 Points to the CaQuery instance that uses this private implementation.
 */

/*!
 Default constructor.
 */
CaQuery::CaQuery() :
    m_d(new CaQueryPrivate(this))
{

}

/*!
 Copy constructor.
 \param query reference to CaQuery.
 */
CaQuery::CaQuery(const CaQuery &query) :
    m_d(new CaQueryPrivate(this))
{
    *m_d = *(query.m_d);
}

/*!
 Copy assignment operator.
 \param query const reference to CaQuery.
 \retval reference to CaQuery.
 */
CaQuery &CaQuery::operator=(const CaQuery &query)
{
    if (this != &query) {
        *m_d = *(query.m_d);
    }
    return *this;
}

/*!
 Destructor.
 */
CaQuery::~CaQuery()
{
    delete m_d;
}

/*!
 Sets entry roles.
 \param entryRoles role of the entry.
 */
void CaQuery::setEntryRoles(EntryRoles entryRoles)
{
    m_d->setEntryRoles(entryRoles);
}

/*!
 Returns an entry role.
 \retval entry role.
 */
EntryRoles CaQuery::entryRoles() const
{
    return m_d->entryRoles();
}

/*!
 Sets parent id.
 \param id id of the parent.
 */
void CaQuery::setParentId(int id)
{
    m_d->setParentId(id);
}

/*!
 Returns parent id.
 \retval parent id.
 */
int CaQuery::parentId() const
{
    return m_d->parentId();
}

/*!
 Sets child id.
 \param id id of the child.
 */
void CaQuery::setChildId(int id)
{
    m_d->setChildId(id);
}

/*!
 Returns child id.
 \retval child id.
 */
int CaQuery::childId() const
{
    return m_d->childId();
}

/*!
 Sets names of entry types.
 \param entryTypeNames list of entry type names (strings).
 */
void CaQuery::setEntryTypeNames(const QStringList &entryTypeNames)
{
    m_d->setEntryTypeNames(entryTypeNames);
}

/*!
 Returns a list of entry type names.
 \retval list of names of entry types.
 */
QStringList CaQuery::entryTypeNames() const
{
    return m_d->entryTypeNames();
}

/*!
 Adds the next entry type name.
 \param entryTypeName name of an entry type.
 */
void CaQuery::addEntryTypeName(const QString &entryTypeName)
{
    m_d->addEntryTypeName(entryTypeName);
}

/*!
 Sets flags which should be enabled.
 \param onFlags flags.
 */
void CaQuery::setFlagsOn(const EntryFlags &onFlags)
{
    m_d->setFlagsOn(onFlags);
}

/*!
 Returns enabled flags.
 \retval flags which should be enabled.
 */
EntryFlags CaQuery::flagsOn() const
{
    return m_d->flagsOn();
}

/*!
 Sets flags which should be disabled.
 \param offFlags flags.
 */
void CaQuery::setFlagsOff(const EntryFlags &offFlags)
{
    m_d->setFlagsOff(offFlags);
}

/*!
 Returns disabled flags.
 \retval flags which should be disabled.
 */
EntryFlags CaQuery::flagsOff() const
{
    return m_d->flagsOff();
}

/*!
 Sets type of sorting.
 \param sortAttribute enum describing what is taken into account to sort.
 \param sortOrder sort order (ascending, descending).
 */
void CaQuery::setSort(SortAttribute sortAttribute, Qt::SortOrder sortOrder)
{
    m_d->setSort(sortAttribute, sortOrder);
}

/*!
 Gets type of sorting.
 \param[out] sortAttribute enum describing what is taken into account to sort.
 \param[out] sortOrder sort order (ascending, descending).
 */
void CaQuery::getSort(SortAttribute &sortAttribute,
                      Qt::SortOrder &sortOrder) const
{
    m_d->getSort(sortAttribute, sortOrder);
}

/*!
 Returns no of entries.
 \retval no of entries.
 */
unsigned int CaQuery::count() const
{
    return m_d->count();
}

/*!
 Sets no of entries.
 \param count no of entries.
 */
void CaQuery::setCount(unsigned int count)
{
    m_d->setCount(count);
}

/*!
 Returns query attributes.
 \retval map of attributes indexed by their names.
  */
QMap<QString, QString> CaQuery::attributes() const
{
    return m_d->attributes();
}

/*!
 Returns an attribute
 \param name name of an attribute
 \retval value of attribute
 */
QString CaQuery::attribute(const QString &name) const
{
    return m_d->attribute(name);
}

/*!
 Sets attribute.
 \param name name of an attribute.
 \param value value of an attribute.
 */
void CaQuery::setAttribute(const QString &name, const QString &value)
{
    m_d->setAttribute(name, value);
}

/*!
 Removes attribute.
 \param name name of an attribute.
 */
void CaQuery::removeAttribute(const QString &name)
{
    m_d->removeAttribute(name);
}

/*!
 Clears query (restores the initial state).
 */
void CaQuery::clear()
{
    m_d->clear();
}

/*!
 Constructor.
 \param queryPublic reference to CaQuery.
 */
CaQueryPrivate::CaQueryPrivate(CaQuery *queryPublic) :
    m_q(queryPublic), mEntryRoles(ItemEntryRole | GroupEntryRole),
    mParentId(0), mChildId(0), mEntryTypeNames(), mFlagsOn(), mFlagsOff(),
    mSortAttribute(DefaultSortAttribute),
    mSortOrder(Qt::AscendingOrder), mCount(0), mAttributes()
{
}

/*!
 Copy assignment operator.
 \param queryPrivate const reference to CaQueryPrivate.
 \retval reference to CaQueryPrivate.
 */

CaQueryPrivate &CaQueryPrivate::operator=(
    const CaQueryPrivate &queryPrivate)
{
    /*m_q is not changed*/
    mEntryRoles = queryPrivate.mEntryRoles;
    mParentId = queryPrivate.mParentId;
    mChildId = queryPrivate.mChildId;
    mEntryTypeNames = queryPrivate.mEntryTypeNames;
    mFlagsOn = queryPrivate.mFlagsOn;
    mFlagsOff = queryPrivate.mFlagsOff;
    mSortAttribute = queryPrivate.mSortAttribute;
    mSortOrder = queryPrivate.mSortOrder;
    mCount = queryPrivate.mCount;
    mAttributes = queryPrivate.mAttributes;

    return *this;
}

/*!
 Destructor.
 */
CaQueryPrivate::~CaQueryPrivate()
{
}

/*!
 Returns an entry role.
 \retval entry role.
 */
EntryRoles CaQueryPrivate::entryRoles() const
{
    return mEntryRoles;
}

/*!
 Sets entry roles.
 \param entryRoles role of the entry.
 */
void CaQueryPrivate::setEntryRoles(EntryRoles entryRoles)
{
    mEntryRoles = entryRoles;
}

/*!
 Returns parent id.
 \retval parent id.
 */
int CaQueryPrivate::parentId() const
{
    return mParentId;
}

/*!
 Sets parent id.
 \param id id of the parent.
 */
void CaQueryPrivate::setParentId(int id)
{
    mParentId = id;
}

/*!
 Returns child id.
 \retval child id.
 */
int CaQueryPrivate::childId() const
{
    return mChildId;
}

/*!
 Sets child id.
 \param id id of the child.
 */
void CaQueryPrivate::setChildId(int id)
{
    mChildId = id;
}

/*!
 Returns a list of entry type names.
 \retval list of names of entry types.
 */
QStringList CaQueryPrivate::entryTypeNames() const
{
    return mEntryTypeNames;
}

/*!
 Sets names of entry types.
 \param entryTypeNames list of entry type names (strings).
 */
void CaQueryPrivate::setEntryTypeNames(const QStringList &entryTypeNames)
{
    mEntryTypeNames = entryTypeNames;
}

/*!
 Adds the next entry type name.
 \param entryTypeName name of an entry type.
 */
void CaQueryPrivate::addEntryTypeName(const QString &entryTypeName)
{
    mEntryTypeNames << entryTypeName;
}

/*!
 Sets flags which should be enabled.
 \param onFlags flags.
 */
void CaQueryPrivate::setFlagsOn(const EntryFlags &onFlags)
{
    mFlagsOn = onFlags;
}

/*!
 Returns enabled flags.
 \retval flags which should be enabled.
 */
EntryFlags CaQueryPrivate::flagsOn() const
{
    return mFlagsOn;
}

/*!
 Sets flags which should be disabled.
 \param offFlags flags.
 */
void CaQueryPrivate::setFlagsOff(const EntryFlags &offFlags)
{
    mFlagsOff = offFlags;
}

/*!
 Returns disabled flags.
 \retval flags which should be disabled.
 */
EntryFlags CaQueryPrivate::flagsOff() const
{
    return mFlagsOff;
}

/*!
 Gets type of sorting.
 \param[out] sortAttribute enum describing what is taken into account to sort.
 \param[out] sortOrder sort order (ascending, descending).
 */
void CaQueryPrivate::getSort(SortAttribute &sortAttribute,
                             Qt::SortOrder &sortOrder) const
{
    sortAttribute = mSortAttribute;
    sortOrder = mSortOrder;
}

/*!
 Sets type of sorting.
 \param sortAttribute enum describing what is taken into account to sort.
 \param sortOrder sort order (ascending, descending).
 */
void CaQueryPrivate::setSort(SortAttribute sortAttribute,
                             Qt::SortOrder sortOrder)
{
    mSortAttribute = sortAttribute;
    mSortOrder = sortOrder;
}

/*!
 Returns no of entries.
 \retval no of entries.
 */
unsigned int CaQueryPrivate::count() const
{
    return mCount;
}

/*!
 Sets no of entries.
 \param count no of entries.
 */
void CaQueryPrivate::setCount(unsigned int count)
{
    mCount = count;
}

/*!
 \retval map of attributes indexed by their names
 */
QMap<QString, QString> CaQueryPrivate::attributes() const
{
    return mAttributes;
}

/*!
 \param name name of an attribute
 \retval value of attribute
 */
QString CaQueryPrivate::attribute(const QString &name) const
{
    return mAttributes.value(name);
}

/*!
 Sets attribute.
 \param name name of an attribute.
 \param value value of an attribute.
 */
void CaQueryPrivate::setAttribute(const QString &name, const QString &value)
{
    mAttributes.insert(name, value);
}

/*!
 Removes an attribute.
 \param name name of an attribute.
 */
void CaQueryPrivate::removeAttribute(const QString &name)
{
    mAttributes.remove(name);
}

/*!
 Clears query (restores the initial state).
 */
void CaQueryPrivate::clear()
{
    mEntryRoles = ItemEntryRole | GroupEntryRole;
    mParentId = 0;
    mChildId = 0;
    mEntryTypeNames = QStringList();
    mFlagsOn = EntryFlags();
    mFlagsOff = EntryFlags();
    mSortAttribute = DefaultSortAttribute;
    mSortOrder = Qt::AscendingOrder;
    mCount = 0;
}
