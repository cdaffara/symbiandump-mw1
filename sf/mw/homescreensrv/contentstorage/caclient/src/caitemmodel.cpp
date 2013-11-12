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
 * Description: caitemmodel.cpp
 *
 */

#include <HbIcon>
#include <HbParameterLengthLimiter>
#include "caclient_defines.h"
#include "caitemmodel.h"
#include "caitemmodel_p.h"
#include "canotifier.h"
#include "canotifierfilter.h"
#include "caclienttest_global.h"

// Constants
const QSizeF defaultIconSize(30, 30);

// ======== MEMBER FUNCTIONS ========

/*!
 *   \class CaItemModel
 *
 *   \brief This class provides model containing CaEntry class objects.
 *
 *   To create instance of CaItemModel object, you need to pass CaQuery
 *   object in constructor. CaQuery should describe items that you want
 *   to have in a model.
 *
 *   \example
 *   \code
 *
 *   CaQuery query;
 *   query.setFlagsOn(VisibleEntryFlag);
 *   query.setParentId(collectionId);
 *   CaItemModel* model = new CaItemModel(query, this);
 *
 *   \endcode
 */

/*!
 Constructor.
 \param query query describing entries that should be present in a model.
 \param parent parent of a model
 */
CaItemModel::CaItemModel(const CaQuery &query, QObject *parent) :
    QAbstractItemModel(parent), m_d(new CaItemModelPrivate(query, this))
{

}

/*!
 Destructor
 */
CaItemModel::~CaItemModel()
{
    delete m_d;
}

/*!
 Returns number of columns
 \param parent not used
 \retval 1

 \code
 ...
 // to get number of columns in a model
 int columns = model->columnCount();
 ...
 \endcode

 */
int CaItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    //model keeps entries in a column 0 and a parent entry in a column 1
    //parent entry is treated as an invisible root item,
    //so column count is always 1
    return 1;
}

/*!
 Returns number of rows
 \param parent not used
 \retval number of rows

 \code
 ...
 // to get number of rows in a model
 int rows = model->rowCount();
 ...
 \endcode
 */
int CaItemModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_d->rowCount();
}

/*!
 Returns QModelIndex of an item
 \param row row in which an item is placed
 \param column not used
 \param parent not used
 \retval index of item in model

 \code
 ...
 // to get model index of an item
 QModelIndex modelIndex = model->index(5);
 ...
 \endcode

 */
QModelIndex CaItemModel::index(int row, int column,
                               const QModelIndex &parent) const
{
    Q_UNUSED(column);
    Q_UNUSED(parent);
    return m_d->index(row);
}

/*!
 Returns parent item
 \param child index (ignored)
 \retval parent index. in case of CaItemModel it is always QModelIndex()

 \code
 ...
 // to get model index of a parent of an item
 QModelIndex parentModelIndex = model->parent(childModelIndex);
 ...
 \endcode
 */
QModelIndex CaItemModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

/*!
 Returns root item model index
 \retval root item model index

 \code
 ...
 // to get model index of a root item
 QModelIndex rootIndex = model->root();
 ...
 \endcode
 */
QModelIndex CaItemModel::root() const
{
    return m_d->root();
}

/*!
 Returns appropiate model's data
 \param index model index
 \param role which data role to return
 \retval models data

 \code
 ...
 // to get data for model item
 QVariant = model->data(itemModelIndex, Qt::DisplayRole);
 ...
 \endcode

 */
QVariant CaItemModel::data(const QModelIndex &index, int role) const
{
    return m_d->data(index, role);
}

/*!
 Disables or enables auto-update feature of the model
 \param autoUpdate true to enable autoupdate, false to disable

 \code
 ...
 // to enable model auto update
 model->setAutoUpdate(true);
 ...
 \endcode

 */
void CaItemModel::setAutoUpdate(bool autoUpdate)
{
    m_d->setAutoUpdate(autoUpdate);
}

/*!
 Disables or enables secondline feature of the model
 \param secondLine enable or disable second line

 \code
 ...
 // to enable model second line visibility
 model->setSecondLineVisibility(true);
 ...
 \endcode

 */
void CaItemModel::setSecondLineVisibility(bool secondLineVisible)
{
    m_d->setSecondLineVisibility(secondLineVisible);
}

/*!
 Gets second line visibility attribute
 \retrun second line visibility attribute

 \code
 ...
 // to check second line visibility attribute
 bool visibility = model->secondLineVisibility();
 ...
 \endcode

 */
bool CaItemModel::secondLineVisibility() const
{
    return m_d->secondLineVisibility();
}
/*!
 Returns auto update status
 \retval true if autoupdate is on, false if not

 \code
 ...
 // to check auto update attribute
 bool autoUpdate = model->isAutoUpdate();
 ...
 \endcode

 */
bool CaItemModel::isAutoUpdate() const
{
    return m_d->notifierExists();
}

/*!
 Method for setting sorting order on model
 \param sortAttribute sort attribute (by name, timestamp, ect...)
 \param sortOrder sort order (ascending, descending)

 \code
 ...
 // to set sort order in a model
 model->setSort(NameSortAttribute, Qt::Ascending);
 ...
 \endcode

 */
void CaItemModel::setSort(SortAttribute sortAttribute,
                          Qt::SortOrder sortOrder)
{
    m_d->setSort(sortAttribute, sortOrder);
}

/*!
 Method for setting icon size
 \param  size icon size to display

 \code
 ...
 // to set an icon size in a model
 QSize iconSize(50,50);
 model->setIconSize(iconSize);
 ...
 \endcode

 */
void CaItemModel::setIconSize(const QSizeF &size)
{
    m_d->setIconSize(size);
}

/*!
 Method for getting icon size
 \param  size icon size to display
 */
QSizeF CaItemModel::getIconSize() const
{
    return m_d->getIconSize();
}

/*!
 Updates model with fresh entries

 \code
 ...
 // to refresh a model
 model->updateModel();
 ...
 \endcode

 */
void CaItemModel::updateModel()
{
    m_d->updateModel();
}

/*!
 Sets parent and fetch children items from a storage
 \param parentId id of a collection

 \code
 ...
 // to set a new parent id
 int newParentId = 10;
 model->setParentId(newParentId);
 ...
 \endcode

 */
void CaItemModel::setParentId(int parentId)
{
    m_d->setParentId(parentId);
}

/*!
 Sets flags to mQuery which should be enabled.
 This method does not update current model - only mQuery member.
 Now to do this setParentId have to be invoke.
 \param onFlags flags.

 \code
 ...
 // to set a flags enabled
 model->setFlagsOn(RemovableEntryFlag);
 ...
 \endcode

 */
void CaItemModel::setFlagsOn(const EntryFlags &onFlags)
{
    m_d->setFlagsOn(onFlags);
}

/*!
 Sets flags to mQuery which should be disabled.
 This method does not update current model - only mQuery member.
 Now to do this setParentId have to be invoke.
 \param offFlags flags.

 \code
 ...
 // to set a flags disabled
 model->setFlagsOff(RemovableEntryFlag);
 ...
 \endcode

 */
void CaItemModel::setFlagsOff(const EntryFlags &offFlags)
{
    m_d->setFlagsOff(offFlags);
}

/*!
 Returns an entry from model
 \param index of entry in model
 \retval pointer to an entry

 \code
 ...
 // to get an entry from a model
 CaEntry* entry = model->entry(entryModelIndex);
 ...
 delete entry;
 \endcode

 */
QSharedPointer<CaEntry> CaItemModel::entry(const QModelIndex &index) const
{
    return m_d->entry(index);
}

QList<int> CaItemModel::getUninstallingEntriesIds(int componentId)
{
    return m_d->getUninstallingEntriesIds(componentId);
}

void CaItemModel::updateProgress(int id, int valueOfProgress)
{
    m_d->updateProgress(id, valueOfProgress);
}

/*!
 Constructor
 \param query needed to create model
 \param pointer to public implementation object connected
 */
CaItemModelPrivate::CaItemModelPrivate(const CaQuery &query,
                                       CaItemModel *itemModelPublic) :
    QObject(), m_q(itemModelPublic), mParentEntry(), mQuery(query),
    mService(CaService::instance()), mEntries(mService), mNotifier(NULL),
    mSize(defaultIconSize), mSecondLineVisibility(true)
{
    updateModel();
    setAutoUpdate(true);
}

/*!
 Destructor
 */
CaItemModelPrivate::~CaItemModelPrivate()
{
    mEntries.clear();
    delete mNotifier;
}

/*!
 Returns count of rows in model
 \retval number of rows
 */
int CaItemModelPrivate::rowCount() const
{
    return mEntries.count();
}

/*!
 Returns QModelIndex of an item
 \param row row
 \retval QModelIndex of item in model
 */
QModelIndex CaItemModelPrivate::index(int row)
{
    if ((row >= 0) && (row < mEntries.count())) {
        return m_q->createIndex(row, 0);
    } else {
        return QModelIndex();
    }
}

/*!
 Returns appropiate model's data
 \param modelIndex model index
 \param role which data role to return
 \retval models data as QVariant
 */
QVariant CaItemModelPrivate::data(const QModelIndex &modelIndex,
                                  int role) const
{
    CACLIENTTEST_FUNC_ENTRY("CaItemModelPrivate::data");
    QVariant variant;
    QSharedPointer<CaEntry> pEntry = entry(modelIndex);
    if (!pEntry.isNull()) {
        switch (role) {
        case Qt::DisplayRole:
            variant = displayRole(pEntry.data());
            break;
        case Qt::DecorationRole:
            variant = QVariant(HbIcon(pEntry->makeIcon(mSize)));
            break;
        case CaItemModel::IdRole:
            variant = QVariant(pEntry->id());
            break;
        case CaItemModel::TypeRole:
            variant = QVariant(pEntry->entryTypeName());
            break;
        case CaItemModel::FlagsRole:
            variant = qVariantFromValue(pEntry->flags());
            break;
        case CaItemModel::TextRole:
            variant = QVariant(pEntry->text());
            break;
        case CaItemModel::DescriptionRole:
            variant = QVariant(pEntry->description());
            break;            
        case CaItemModel::FullTextRole:
            variant = QVariant(pEntry->text() + QString(" ") + pEntry->description());
            break;
        case CaItemModel::UninstalRole:
            variant = QVariant(
                pEntry->attribute(UNINSTALL_PROGRESS_APPLICATION_ATTRIBUTE_NAME).toInt());
            break;
        case CaItemModel::CollectionTitleRole:
            if (!pEntry->attribute(COLLECTION_TITLE_NAME).isNull()) {
                variant = QVariant(
                        HbParameterLengthLimiter(pEntry->attribute(COLLECTION_TITLE_NAME))
                        .arg(this->rowCount()).arg(pEntry->text()));
            }
            else {
                variant = QVariant(pEntry->text());
            }
            break;
            
        default:
            variant = QVariant(QVariant::Invalid);
        }
    }
    CACLIENTTEST_FUNC_EXIT("CaItemModelPrivate::data");
    return variant;
}

/*!
 Disables or enables auto-update feature of the model
 \param autoUpdate (HsMenuAutoUpdate)
 */
void CaItemModelPrivate::setAutoUpdate(bool autoUpdate)
{
    CACLIENTTEST_FUNC_ENTRY("CaItemModelPrivate::setAutoUpdate");
    if (autoUpdate) {
        if (!mNotifier) {
            CaNotifierFilter filter(mQuery);
            mNotifier = mService->createNotifier(filter);
            connectSlots();
        }
    } else {
        disconnectSlots();
        delete mNotifier;
        mNotifier = NULL;
    }
    CACLIENTTEST_FUNC_EXIT("CaItemModelPrivate::setAutoUpdate");
}

/*!
 Method for setting sorting order on model
 (probably will be moved to MenuService)
 \param  sortOrder sorting order (SortAttribute)
 */
void CaItemModelPrivate::setSort(SortAttribute sortAttribute,
                                 Qt::SortOrder sortOrder)
{
    CACLIENTTEST_FUNC_ENTRY("CaItemModelPrivate::setSort");
    mQuery.setSort(sortAttribute, sortOrder);
    updateLayout();
    CACLIENTTEST_FUNC_EXIT("CaItemModelPrivate::setSort");
}

/*!
 Method for setting icon size
 \param size icon size to display
 */
void CaItemModelPrivate::setIconSize(const QSizeF &size)
{
    if (mSize == size)
        return;
    m_q->layoutAboutToBeChanged();
    mSize = size;
    m_q->layoutChanged();
}

/*!
 Method for getting icon size
 \retval icon size to display
 */
QSizeF CaItemModelPrivate::getIconSize() const
{
    return mSize;
}


/*!
 Gets index of the parent item
 \retval QModelIndex representing root item
 */
QModelIndex CaItemModelPrivate::root()
{
    if (mQuery.parentId()) {
        return m_q->createIndex(0, 1);
    } else {
        return QModelIndex();
    }
}

/*!
 Returns an entry from model
 \param modelIndex index of entry in model
 \retval pointer to an entry
 */
QSharedPointer<CaEntry> CaItemModelPrivate::entry(const QModelIndex &modelIndex) const
{
    if (modelIndex.isValid()) {
        if (modelIndex.column() == 1) {
            return mParentEntry;
        }
        else {
            int row = modelIndex.row();
            if (row >= mEntries.count()) {
                return QSharedPointer<CaEntry> ();
            }
            else {
                return mEntries.at(row);
            }
        }
    }
    return QSharedPointer<CaEntry> ();
}

/*!
 Disables or enables secondline feature of the model
 \param secondLine disables or enables second line
 */
void CaItemModelPrivate::setSecondLineVisibility(bool secondLineVisibility)
{
    if (mSecondLineVisibility == secondLineVisibility)
        return;
    m_q->layoutAboutToBeChanged();
    mSecondLineVisibility = secondLineVisibility;
    m_q->layoutChanged();
}

/*!
 Gets second line visibility attribute
 \retrun second line visibility attribute
 */
bool CaItemModelPrivate::secondLineVisibility() const
{
    return mSecondLineVisibility;
}

/*!
 Returns proper display role for item
 \param modelIndex item index
 \retval QVariant containing display role
 */
QVariant CaItemModelPrivate::displayRole(const CaEntry* entry) const
{
    CACLIENTTEST_FUNC_ENTRY("CaItemModelPrivate::displayRole")

    QVariant result;
    if (mSecondLineVisibility) {
        if (entry->description().isEmpty()) {
            result = entry->text();
        } else {
            QList<QVariant> text;
            text << entry->text();
            text << entry->description();
            result = QVariant(text);
        }
    } else {
        result = entry->text();
    }
    CACLIENTTEST_FUNC_EXIT("CaItemModelPrivate::displayRole")
    return result;
}

/*!
 Sets parent
 \param parentId
 */
void CaItemModelPrivate::setParentId(int parentId)
{
    CACLIENTTEST_FUNC_ENTRY("CaItemModelPrivate::setParentId");
    mQuery.setParentId(parentId);
    if (mNotifier) {
        delete mNotifier;
        mNotifier = mService->createNotifier(CaNotifierFilter(mQuery));
        reconnectSlots();
    }
    updateModel();
    CACLIENTTEST_FUNC_EXIT("CaItemModelPrivate::setParentId");

}

/*!
 Sets flags to mQuery which should be enabled.
 \param onFlags flags.

  */
void CaItemModelPrivate::setFlagsOn(const EntryFlags &onFlags)
{
    mQuery.setFlagsOn(onFlags);
}

/*!
 Sets flags to mQuery which should be disabled.
 \param offFlags flags.

 \code
 ...
 // to set a new parent id
 model->setFlagsOff(RemovableEntryFlag);
 ...
 \endcode

 */
void CaItemModelPrivate::setFlagsOff(const EntryFlags &offFlags)
{
    mQuery.setFlagsOff(offFlags);
}

/*!
 Checks if notifier exists
  \retval true if notifier exists otherwise false
 */
bool CaItemModelPrivate::notifierExists() const
{
    if (mNotifier) {
        return true;
    }
    return false;
}


QList<int> CaItemModelPrivate::getUninstallingEntriesIds(int componentId)
{
    CaQuery* query = new CaQuery(mQuery);
    QString compId(QString().setNum(componentId));
    query->setAttribute(QString("component_id"), compId);
    query->setFlagsOn(RemovableEntryFlag | VisibleEntryFlag);
    QList<int> ids = mService->getEntryIds(*query);
    delete query;
    return ids;
}

void CaItemModelPrivate::updateProgress(int id, int valueOfProgress)
{
    int updateIndex = mEntries.updateProgress(id, valueOfProgress);
    if (updateIndex >= 0) {
        emit m_q->dataChanged(
                index(updateIndex), index(updateIndex));
    }        
}
/*!
 Updates model with fresh entries and resets model
 */
void CaItemModelPrivate::updateModel()
{
    CACLIENTTEST_FUNC_ENTRY("CaItemModelPrivate::updateModel");

    mEntries.reloadEntries(mQuery);
    updateParentEntry();
    m_q->reset();

    CACLIENTTEST_FUNC_EXIT("CaItemModelPrivate::updateModel");
}

/*!
 Updates parent entry
 */
void CaItemModelPrivate::updateParentEntry()
{
    CACLIENTTEST_FUNC_ENTRY("CaItemModelPrivate::updateParentEntry");

    if (mQuery.parentId()) {
        mParentEntry = mService->getEntry(mQuery.parentId());
    }

    CACLIENTTEST_FUNC_EXIT("CaItemModelPrivate::updateParentEntry");

}

/*!
 Updates model item with fresh data
 \param entry item to update
 */
void CaItemModelPrivate::updateItemData(const QSharedPointer<CaEntry> &entry)
{
    CACLIENTTEST_FUNC_ENTRY("CaItemModelPrivate::updateItemData");


    int id = entry->id();
    QList<int> ids = mService->getEntryIds(mQuery);
    if (mEntries.indexOf(id) >= 0
           && ids.indexOf(id) == mEntries.indexOf(id)) {
        mEntries.updateEntry(entry);
        emit m_q->dataChanged(
            index(mEntries.indexOf(id)), index(mEntries.indexOf(id)));
    } else if (mParentEntry && id == mParentEntry->id()) {
        updateParentEntry();
        m_q->reset();
    } else if (ids.indexOf(id) < 0) {
        removeItem(id);
    } else if (mEntries.indexOf(id) < 0) {
        addItem(id);
    } else {
        updateModel();
    }
    CACLIENTTEST_FUNC_EXIT("CaItemModelPrivate::updateItemData");
}

/*!
 Adds new item to model
 \param id id of item to add
 */
void CaItemModelPrivate::addItem(int id)
{
    CACLIENTTEST_FUNC_ENTRY("CaItemModelPrivate::addItem");

    int row = itemRow(id);
    //we use beginInsertRows and endInsertRows to emit proper signal
    //(see Qt documentation of QAbstractItemModel)
    if (mEntries.indexOf(id) < 0 && row >= 0) {
        if (row > mEntries.count()) {
        	m_q->beginInsertRows(QModelIndex(), mEntries.count(), mEntries.count());
        	mEntries.insert(mEntries.count(), id);
        } else {
			m_q->beginInsertRows(QModelIndex(), row , row);
        	mEntries.insert(row, id);
        }
        m_q->endInsertRows();
    } else if (row == -1) {
        //we udpade whole model because we do not know parent collecion for given item
        updateModel();
    }
    CACLIENTTEST_FUNC_EXIT("CaItemModelPrivate::addItem");
}

/*!
 Adds new item block to model
 Use in cases when inserting / appending an adjacent block of items
 \param itemsList list of adjacent items
 */
void CaItemModelPrivate::addItemBlock(const QList<int> &itemsList)
{
    CACLIENTTEST_FUNC_ENTRY("CaItemModelPrivate::addItemBlock");
    if (!itemsList.isEmpty()) {
        int firstRow = itemRow(itemsList.first());
        int lastRow = itemRow(itemsList.last());
        m_q->beginInsertRows(QModelIndex(), firstRow, lastRow);
        for (int i = 0; i < itemsList.count(); ++i) {
            mEntries.insert(firstRow + i, itemsList.at(i));
        }
        m_q->endInsertRows();
        emit m_q->scrollTo(firstRow, QAbstractItemView::PositionAtTop);
    }
    CACLIENTTEST_FUNC_EXIT("CaItemModelPrivate::addItemBlock");
}

/*!
 Adds new items to model
 \param itemsList current items list
 */
void CaItemModelPrivate::handleAddItems(const QList<int> &itemsList)
{
    CACLIENTTEST_FUNC_ENTRY("CaItemModelPrivate::handleAddItems");
    const int oldItemCount(mEntries.count());
    if (oldItemCount) {
        const int newItemCount(itemsList.count());
        int i = 0;
        QList<int> oldList = mEntries.orderedIdList();
        //we loop through items to find first added
        while (i < oldList.count()) {
            if (oldList[i] != itemsList[i]) {
                oldList.takeAt(i);
            } else {
                ++i;
            }
        }
        if (newItemCount == oldItemCount) {
            // count is the same - check if item order changed
            if (itemsList == oldList) {
                // assume that if the order has not changed
                // it had to be the secondary lines
                emit m_q->dataChanged(index(0), index(m_q->rowCount()-1));
            } else {
                updateLayout();
            }
        } else {
            updateModel();
            //i is the index of first added item
        }
        emit m_q->scrollTo(i, QAbstractItemView::PositionAtTop);
    } else {
        // items added to an empty list - add all as a single block
        addItemBlock(itemsList);
    }
    CACLIENTTEST_FUNC_EXIT("CaItemModelPrivate::handleAddItems");
}

/*!
 Gets index/row for new item
 \param id of item to add
 \retval row in model list for new item to insert
 */
int CaItemModelPrivate::itemRow(int id)
{
    CACLIENTTEST_FUNC_ENTRY("CaItemModelPrivate::itemRow");
    QList<int> ids = mService->getEntryIds(mQuery);
    CACLIENTTEST_FUNC_EXIT("CaItemModelPrivate::itemRow");
    return ids.indexOf(id);
}

/*!
 Removes item from model
 \param id of item to remove
 */
void CaItemModelPrivate::removeItem(int id)
{
    CACLIENTTEST_FUNC_ENTRY("CaItemModelPrivate::removeItem");
    int row = mEntries.indexOf(id);
    if (row >= 0 && rowCount() > 1) {
        m_q->beginRemoveRows(QModelIndex(), mEntries.indexOf(id),
        mEntries.indexOf(id));
        mEntries.remove(id);
        m_q->endRemoveRows();
    } else {
        updateModel();
    }
    CACLIENTTEST_FUNC_EXIT("CaItemModelPrivate::removeItem");
}

/*!
 Removes missing items from model
 \param itemsList current items list
 */
void CaItemModelPrivate::removeItems(const QList<int> &itemsList)
{
    CACLIENTTEST_FUNC_ENTRY("CaItemModelPrivate::removeItems");
    int i = 0;
    for (i = 0; i < itemsList.count(); i++) {
        if (itemsList[i] != mEntries[i]) {
            removeItem(mEntries[i]);
        }
    }
    while (i < mEntries.count()) {
        removeItem(mEntries[i]);
        i++;
    }
    CACLIENTTEST_FUNC_EXIT("CaItemModelPrivate::removeItems");
}

/*!
 Layout update
 NOTE: this method should be called only if the entries get rearranged
 and do not change their contents!
 */
void CaItemModelPrivate::updateLayout()
{
    CACLIENTTEST_FUNC_ENTRY("CaItemModelPrivate::updateLayout");
    m_q->layoutAboutToBeChanged();

    // get the ID list from before the update
    QList<int> oldOrderedIdList = mEntries.orderedIdList();

    // do the update, the entries should only get rearranged
    mEntries.updateEntries(mQuery);

    // get the new ID list after the entries got rearranged
    QList<int> newOrderedIdList = mEntries.orderedIdList();
    // this list will contain the new position indices
    QList<int> newPositionsList;
    int entry;
    foreach (entry, oldOrderedIdList) {
        newPositionsList << newOrderedIdList.indexOf(entry);
    }

    // now check which items in the previous persistent index list changed
    // their positions, make new indices for them and store in the new
    // persistent index list
    QModelIndexList oldPersistentIndexList = m_q->persistentIndexList();
    QModelIndexList newPersistentIndexList;
    QModelIndex index;
    foreach (index, oldPersistentIndexList) {
        newPersistentIndexList <<
            m_q->createIndex(
                newPositionsList.at(index.row()), 0, index.internalPointer());
    }
    m_q->changePersistentIndexList(
        oldPersistentIndexList, newPersistentIndexList);

    m_q->layoutChanged();
    CACLIENTTEST_FUNC_EXIT("CaItemModelPrivate::updateLayout");
}

/*!
 Connects slots
 */
void CaItemModelPrivate::connectSlots()
{
    CACLIENTTEST_FUNC_ENTRY("CaItemModelPrivate::connectSlots");
    connect(mNotifier, SIGNAL(entryChanged(CaEntry ,ChangeType)),
            this, SLOT(updateModelItem(CaEntry, ChangeType)));

    if (mQuery.parentId() > 0) {
        connect(mNotifier, SIGNAL(groupContentChanged(int)),
                this, SLOT(updateModelContent(int)));
    }
    CACLIENTTEST_FUNC_EXIT("CaItemModelPrivate::connectSlots");
}

/*!
 Disconnects slots
 */
void CaItemModelPrivate::disconnectSlots()
{
    CACLIENTTEST_FUNC_ENTRY("CaItemModelPrivate::disconnectSlots");
    disconnect(mNotifier, SIGNAL(entryChanged(CaEntry ,ChangeType)),
            this, SLOT(updateModelItem(CaEntry, ChangeType)));
    if (mQuery.parentId() > 0) {
        disconnect(mNotifier, SIGNAL(groupContentChanged(int)),
                   this, SLOT(updateModelContent(int)));
    }
    CACLIENTTEST_FUNC_EXIT("CaItemModelPrivate::disconnectSlots");
}

/*!
 Reconnects slots
 */
void CaItemModelPrivate::reconnectSlots()
{
    CACLIENTTEST_FUNC_ENTRY("CaItemModelPrivate::reconnectSlots");
    disconnectSlots();
    connectSlots();
    CACLIENTTEST_FUNC_EXIT("CaItemModelPrivate::reconnectSlots");
}

/*!
 Updates model with fresh entries
 \param id of item to handle
 \param changeType change type
 */
void CaItemModelPrivate::updateModelItem(
    const CaEntry &entry, ChangeType changeType)
{
    CACLIENTTEST_FUNC_ENTRY("CaItemModelPrivate::updateModelItem");
    QSharedPointer<CaEntry> sharedEntry(new CaEntry(entry));
    int previousCount = rowCount();
    switch (changeType) {
        case AddChangeType:
            addItem(sharedEntry->id());
            break;
        case RemoveChangeType:
            removeItem(sharedEntry->id());
            break;
        default:
            updateItemData(sharedEntry);
            break;
    }
    emitEmpty(previousCount);
    emitCountChange(previousCount);
    CACLIENTTEST_FUNC_EXIT("CaItemModelPrivate::updateModelItem");
}

/*!
 Updates model with fresh entries
 \param id of parent
 */
void CaItemModelPrivate::updateModelContent(int id)
{
    Q_UNUSED(id);
    int previousCount = rowCount();

    CACLIENTTEST_FUNC_ENTRY("CaItemModelPrivate::updateModelContent");
    QList<int> ids = mService->getEntryIds(mQuery);

    if (ids.count() >= mEntries.count()) {
        handleAddItems(ids);
    } else {
        removeItems(ids);
    }
    emitEmpty(previousCount);
    emitCountChange(previousCount);
    CACLIENTTEST_FUNC_EXIT("CaItemModelPrivate::updateModelContent");
}

/*!
 Emits empty signal if model state was changed
 \param id of parent
 */
void CaItemModelPrivate::emitEmpty(int previousCount)
{
    if ( previousCount && !rowCount()) {
        emit m_q->empty(true);
    }
    if ( !previousCount && rowCount()) {
        emit m_q->empty(false);
    }
}

/*!
 Emits empty signal if count of item in model was change
 \param previousCount 
 */
void CaItemModelPrivate::emitCountChange(int previousCount)
{
    if (previousCount != rowCount()) {
        emit m_q->countChange();
    }
}

