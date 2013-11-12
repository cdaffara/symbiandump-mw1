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
 * Description: caitemmodel.h
 *
 */

#ifndef CASTANDARDITEMMODEL_H
#define CASTANDARDITEMMODEL_H

// System includes
#include <QAbstractItemModel>
#include <QAbstractItemView>

// User includes
#include "caclient_global.h"
#include "caquery.h"

// Forward declarations
class CaEntry;
class CaItemModelPrivate;
CA_CLIENT_TEST_CLASS(TestCaClient)

// Class declaration
class CACLIENT_EXPORT CaItemModel: public QAbstractItemModel
{

Q_OBJECT
    CA_CLIENT_TEST_FRIEND_CLASS(TestCaClient)

public:

    // Data types
    enum CaItemRole
    {
        IdRole = Qt::UserRole + 1,
        ParentIdRole,
        TypeRole,
        FlagsRole,
        TextRole,//contains title represented as QString,
        DescriptionRole, // contains item description
        FullTextRole, //'title description' convinient for operations such as search
        UninstalRole,  //contains progress status as decimal number in QString
        CollectionTitleRole //contains title represented as QString,

    };

    // Function declarations
    CaItemModel(const CaQuery &query, QObject *parent = 0);
    ~CaItemModel();

    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex index(int row, int column = 0,
        const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    QModelIndex root() const;
    virtual QVariant data(const QModelIndex &index, int role =
        Qt::DisplayRole) const;
    void setAutoUpdate(bool autoUpdate);
    void setSecondLineVisibility(bool secondLineVisible);
    bool secondLineVisibility() const;
    bool isAutoUpdate() const;
    void setSort(SortAttribute sortAttribute, Qt::SortOrder sortOrder =
        Qt::AscendingOrder);
    void setIconSize(const QSizeF &size);
    QSizeF getIconSize() const;
    void updateModel();
    void setParentId(int parentId);
    void setFlagsOn(const EntryFlags &onFlags);
    void setFlagsOff(const EntryFlags &offFlags);
    QSharedPointer<CaEntry> entry(const QModelIndex &index) const;

    QList<int> getUninstallingEntriesIds(int componentId);
    void updateProgress(int id, int valueOfProgress);

    
signals:

    void scrollTo(int row, QAbstractItemView::ScrollHint hint =
        QAbstractItemView::EnsureVisible);
    void empty(bool value);
    void countChange();
    
protected:
    // data

    CaItemModelPrivate * const m_d;
    friend class CaItemModelPrivate;

};

#endif // CASTANDARDITEMMODEL_H
