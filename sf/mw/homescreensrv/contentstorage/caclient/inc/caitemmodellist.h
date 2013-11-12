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
 * Description: caitemmodellist_p.h
 *
 */

#ifndef CAITEMMODELLIST_P_H
#define CAITEMMODELLIST_P_H

// System includes
#include <QHash>

// User includes
#include "caservice.h"
#include "caclient_global.h"

// Forward declarations
class CaItemModel;
class CaQuery;
CA_CLIENT_TEST_CLASS(TestCaClient)

// Class declaration
class CaItemModelList
{

public:

    // Function declarations
    CaItemModelList(QSharedPointer<CaService> service);
    ~CaItemModelList();
    void clear();
    int count() const;
    QSharedPointer<CaEntry> at(int row) const;
    void reloadEntries(const CaQuery &query);
    void updateEntry(const QSharedPointer<CaEntry> &entry);
    int updateProgress(int id, int valueOfProgress);
    void updateEntries(const CaQuery &query);
    int indexOf(const int &id) const;
    void insert(int row, int id);
    void remove(int id);
    const int &operator[](int row) const;
    QList<int> orderedIdList();

private:

    // Function declarations
    CaItemModelList();
    CaItemModelList &operator=(const CaItemModelList &otherList);

private: // data

    QList<int> mOrderedList;
    QHash<int, QSharedPointer<CaEntry> > mEntriesHash;
    QSharedPointer<CaService> mService;

};

#endif // CAITEMMODELLIST_P_H
