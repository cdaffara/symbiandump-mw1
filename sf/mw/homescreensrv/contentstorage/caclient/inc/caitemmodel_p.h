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
 * Description: caitemmodel_p.h
 *
 */

#ifndef CAITEMMODEL_P_H
#define CAITEMMODEL_P_H

// System includes
#include <QObject>
#include <QModelIndex>

// User includes
#include "caservice.h"
#include "caquery.h"
#include "caitemmodel.h"
#include "caitemmodellist.h"
#include "caclient_global.h"

// Forward declarations
class CaItemModel;
CA_CLIENT_TEST_CLASS(TestCaClient)

// Class declaration
class CaItemModelPrivate: public QObject
{

    Q_OBJECT
    CA_CLIENT_TEST_FRIEND_CLASS(TestCaClient)

public:

    // Function declarations
    explicit CaItemModelPrivate(const CaQuery &query,
                                CaItemModel *itemModelPublic);
    ~CaItemModelPrivate();
    int rowCount() const;
    QModelIndex index(int row);
    QVariant data(const QModelIndex &modelIndex, int role) const;
    void setAutoUpdate(bool autoUpdate);
    void setSort(SortAttribute sortAttribute, Qt::SortOrder sortOrder =
                     Qt::AscendingOrder);
    void setIconSize(const QSizeF &size);
    QSizeF getIconSize() const;
    QModelIndex root();
    QSharedPointer<CaEntry> entry(const QModelIndex &modelIndex) const;
    void setSecondLineVisibility(bool secondLineVisibility);
    bool secondLineVisibility() const;
    QVariant displayRole(const CaEntry* entry) const;
    void setParentId(int parentId);
    void setFlagsOn(const EntryFlags &onFlags);
    void setFlagsOff(const EntryFlags &offFlags);
    bool notifierExists() const;
    
    QList<int> getUninstallingEntriesIds(int componentId);
    void updateProgress(int id, int valueOfProgress);
    

public slots:

    // Function declarations
    void updateModel();

private:

    // Function declarations
    void updateParentEntry();
    void updateItemData(const QSharedPointer<CaEntry> &entry);
    void addItem(int id);
    void addItemBlock(const QList<int> &itemsList);
    void handleAddItems(const QList<int> &itemsList);
    int itemRow(int id);
    void removeItem(int id);
    void removeItems(const QList<int> &itemsList);
    void updateLayout();
    void connectSlots();
    void disconnectSlots();
    void reconnectSlots();
    void emitEmpty(int previousCount);
    void emitCountChange(int previousCount);

private slots:

    // Function declarations
    void updateModelItem(const CaEntry &entry, ChangeType changeType);
    void updateModelContent(int id);

private:
    // data

    CaItemModel *const m_q;
    QSharedPointer<CaEntry> mParentEntry;
    CaQuery mQuery;
    QSharedPointer<CaService> mService;
    CaItemModelList mEntries;
    CaNotifier *mNotifier;//own
    QSizeF mSize;
    bool mSecondLineVisibility;

};

#endif // CAITEMMODEL_P_H
