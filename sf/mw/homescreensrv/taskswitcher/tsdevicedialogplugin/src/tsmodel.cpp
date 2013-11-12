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
* Description: tsmodel.cpp
*
*/
#include "tsmodel.h"

#include <QVariant>
#include <QList>

#include <HbIcon>

#include <afstorageglobals.h>

#include "tsmodelitem.h"
#include "tsdataroles.h"
#include "tstaskchangeinfo.h"

/*!
    \class TsModel
    \ingroup group_tsdevicedialogplugin
    \brief Model storing running tasks.
*/

/*!
    Constructor
    \param query used to create model
    \param pointer to parent object
*/
TsModel::TsModel(TsTaskMonitor &applicationSrv, QObject *parent) :
    QAbstractListModel(parent),
    mEntries(),
    mApplicationService(applicationSrv)
{
    connect(&applicationSrv,
            SIGNAL(taskListChanged()),
            this,
            SLOT(updateApplications()));

    fullUpdate();
}

/*!
    Destructor
*/
TsModel::~TsModel()
{
    qDeleteAll(mEntries);
}

/*!
    Returns count of rows in model
    \retval number of rows
*/
int TsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mEntries.count();
}

bool TsModel::insertRows(int row, int count, TsModelItem *item, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row+count-1);
    mEntries.insert(row, item);
    endInsertRows();
    return true;
}

bool TsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    TsModelItem *oldItem = mEntries.at(row);
    beginRemoveRows(parent, row, row + count - 1);
    mEntries.removeAt(row);
    delete oldItem;
    endRemoveRows();
    return true;
}


bool TsModel::moveRows(int oldPosition, int newPosition, const QModelIndex &parent)
{
    beginMoveRows(parent, oldPosition, oldPosition, parent, newPosition);
    mEntries.move(oldPosition, newPosition);
    endMoveRows();
    return true;
}


bool TsModel::updateRows(int row, TsModelItem *item)
{
    TsModelItem *oldItem = mEntries.at(row);
    mEntries[row] = item;
    delete oldItem;

    emit dataChanged(index(row),index(row));
    return true;
}

/*!
    Returns appropiate model's data
    \param index model index
    \param role which data role to return
    \retval models data
*/
QVariant TsModel::data(const QModelIndex &index, int role) const
{
    return index.isValid() ? entry(index)->data(role) : QVariant();
}

/*!
    Activate one of model entries
*/
void TsModel::openApplication(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    entry(index)->open();
}

/*!
    Close one of moder entries
*/
void TsModel::closeApplication(const QModelIndex &index)
{
    if (!index.isValid() ||
            !entry(index)->data(TsDataRoles::Closable).toBool()) {
        return;
    }
    entry(index)->close();
}

/*!
    Updates model with fresh entries
*/
void TsModel::updateApplications()
{
    QList<TsTaskChange> changes(mApplicationService.changeList());

    if (changes.count() == 0) {
        return;
    }
    //check 1st item whether we have cancel change - if so reset model
    if (changes[0].first.changeType() == TsTaskChangeInfo::EChangeCancel) {
        fullUpdate();
        return;
    }
    for (int iter(0); iter < changes.count(); iter++) {
        switch (changes[iter].first.changeType()) {
            case TsTaskChangeInfo::EChangeDelete :
                removeRows(changes[iter].first.oldOffset(), 1);
                break;
            case TsTaskChangeInfo::EChangeInsert :
                insertRows(changes[iter].first.newOffset(), 1,
                           new TsModelItem(changes[iter].second));
                break;
            case TsTaskChangeInfo::EChangeMove :
                moveRows(changes[iter].first.oldOffset(), changes[iter].first.newOffset());
                break;
            case TsTaskChangeInfo::EChangeUpdate :
                updateRows(changes[iter].first.oldOffset(),
                           new TsModelItem(changes[iter].second));
                break;
            default:
                break;
        }
    }
}

/*!
    reset model
*/
void TsModel::fullUpdate()
{
    beginResetModel();
    qDeleteAll(mEntries);
    mEntries.clear();
    getApplications();
    endResetModel();

}

/*!
    Read list of running applications
*/
void TsModel::getApplications()
{
    //get all running applications and append to entries list
    QList<TsTaskChange> tasks(mApplicationService.changeList(true));
    foreach(TsTaskChange taskData, tasks) {
        if (!taskData.second.isNull()) {
            mEntries.append(new TsModelItem(taskData.second));
        }
    }
}

/*!
    Returns an entry from model
    \param index of entry in model
    \retval pointer to an entry
*/
TsModelItem *TsModel::entry(const QModelIndex &index) const
{
    return mEntries.at(index.row());
}
