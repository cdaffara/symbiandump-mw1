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
* Description:  Main test class for hspluginmodel library.
*
*/

#ifndef T_CACLIENTNOTIFIER_H
#define T_CACLIENTNOTIFIER_H

#include <QObject>
#include <QList>
#include "cadefs.h"

class CaEntry;
class CaNotifierFilter;
class CaNotifier;
class CaService;

/**
 * @test Test class for hsmcsserviceprovider library.
 */
class TestCaClientNotifier : public QObject
{
    Q_OBJECT

public:

    TestCaClientNotifier();
    TestCaClientNotifier(const CaService &service, CaNotifierFilter *filter = 0);
    ~TestCaClientNotifier();

    CaNotifier *getNotifier() const;
    CaNotifierFilter *getFilter() const;
    void clean();

public slots:

    void entryChanged(int entryId, ChangeType changeType);
    void entryChanged(const CaEntry &entry, ChangeType changeType);
    void entryTouched(int id);
    void groupContentChanged(int groupId);

public:

    int mEntryId;
    int mGroupId;
    ChangeType mChangeType;
    CaEntry *mEntry;

    int mCount;
    QList<int> *mEntryIds;
    QList<int> *mGroupIds;
    QList<ChangeType> *mChangeTypes;
    QList<const CaEntry *> *mEntries;

    CaNotifierFilter *mFilter;
    CaNotifier *mNotifier;

private:

    Q_DISABLE_COPY(TestCaClientNotifier)

};

#endif
