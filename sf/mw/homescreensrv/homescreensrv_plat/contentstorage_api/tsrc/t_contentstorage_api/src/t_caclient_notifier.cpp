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

#include <QScopedPointer>
#include <qservicemanager.h>

#include "t_caclient.h"
#include "caservice.h"
#include "canotifier.h"
#include "canotifierfilter.h"
#include "caquery.h"
#include "t_caclientNotifier.h"

const int entryIdFake(-1);
const int waitTime500ms(500);
const int waitTime1sec(1000);

QTM_USE_NAMESPACE

QStringList pluginPaths;
QString xmlName("caapphandlerplugin.xml");

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::initTestCase()
{
#ifdef Q_OS_SYMBIAN
    startThread();
#endif //Q_OS_SYMBIAN
    mService = CaService::instance();
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::cleanupTestCase()
{
#ifdef Q_OS_SYMBIAN
    stopThread();
#endif //Q_OS_SYMBIAN
    QSharedPointer<CaService> serviceNull(NULL);
    mService = serviceNull;
    QTest::qWait(5000);
}


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
int threadFunction(void */*params*/)
{
#ifdef Q_OS_SYMBIAN
    while (ETrue) {
        User::ResetInactivityTime();//it should help for Viewserver11 panic
        User::After(5000000);
    }
#endif //Q_OS_SYMBIAN
    return 0;
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::startThread()
{
#ifdef Q_OS_SYMBIAN
    User::LeaveIfError(iThread.Create(
                           _L("thread_kill_viewsrv11"),
                           threadFunction,
                           16384,
                           4000,
                           4000,
                           NULL));
    iThread.Resume();
#endif //Q_OS_SYMBIAN
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::stopThread()
{
#ifdef Q_OS_SYMBIAN
    iThread.Close();
#endif //Q_OS_SYMBIAN
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_createNotifier()
{
#ifdef Q_OS_SYMBIAN
#ifdef UT_MEMORY_CHECK
// __UHEAP_MARK;
#endif//UT_MEMORY_CHECK
#endif//Q_OS_SYMBIAN
    {

        CaNotifierFilter *filter = new CaNotifierFilter();
        filter->setEntryRole(GroupEntryRole);
        filter->setTypeNames(QStringList() << "TypeName");
        TestCaClientNotifier *clientNotifier = new TestCaClientNotifier(*mService, filter);
        CaNotifier *notifier = clientNotifier->getNotifier();

        connect(notifier, SIGNAL(entryChanged(int,ChangeType)),
        clientNotifier, SLOT(entryChanged(int,ChangeType)));
        QTest::qWait(waitTime500ms);
        CaEntry group(GroupEntryRole);
        group.setText("Text");
        group.setEntryTypeName("TypeName");

        QSharedPointer<CaEntry> entry = mService->createEntry(group);
        QTest::qWait(waitTime1sec);

        QCOMPARE(clientNotifier->mChangeType, AddChangeType);
        QVERIFY(clientNotifier->mEntryId > 0);
        QCOMPARE(clientNotifier->mEntryId, entry->id());

        // cleanup
        disconnect(notifier, SIGNAL(entryChanged(int,ChangeType)),
        clientNotifier, SLOT(entryChanged(int,ChangeType)));
        QTest::qWait(waitTime500ms);
        delete clientNotifier;
        mService->removeEntry(entry->id());
    }
#ifdef Q_OS_SYMBIAN
#ifdef UT_MEMORY_CHECK
// __UHEAP_MARKEND;
#endif//UT_MEMORY_CHECK
#endif//Q_OS_SYMBIAN
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_touch()
{
#ifdef Q_OS_SYMBIAN
#ifdef UT_MEMORY_CHECK
// __UHEAP_MARK;
#endif//UT_MEMORY_CHECK
#endif//Q_OS_SYMBIAN

    CaEntry item;
    item.setText("Text");
    item.setEntryTypeName("TypeName");
    QSharedPointer<CaEntry> entry = mService->createEntry(item);
    QTest::qWait(waitTime1sec);

    CaNotifierFilter *filter = new CaNotifierFilter();
    filter->setEntryRole(ItemEntryRole);
    filter->setIds(QList<int>() << entry->id());
    TestCaClientNotifier *clientNotifier = new TestCaClientNotifier(*mService, filter);
    CaNotifier *notifier = clientNotifier->getNotifier();

    connect(notifier, SIGNAL(entryTouched(int)),
            clientNotifier, SLOT(entryTouched(int)));
    QTest::qWait(waitTime500ms);

    bool touched = mService->touch(*entry);
    QTest::qWait(waitTime1sec);

    QVERIFY(clientNotifier->mEntryId > 0);
    QCOMPARE(clientNotifier->mEntryId, entry->id());
    QCOMPARE(touched, TRUE);

    // cleanup
    disconnect(notifier, SIGNAL(entryTouched(int)),
               clientNotifier, SLOT(entryTouched(int)));
    QTest::qWait(waitTime500ms);
    delete clientNotifier;
    delete filter;
    mService->removeEntry(entry->id());
    QTest::qWait(waitTime1sec);

#ifdef Q_OS_SYMBIAN
#ifdef UT_MEMORY_CHECK
// __UHEAP_MARKEND;
#endif//UT_MEMORY_CHECK
#endif//Q_OS_SYMBIAN
}

//QTEST_MAIN(homescreen::test::TestCaClient)
