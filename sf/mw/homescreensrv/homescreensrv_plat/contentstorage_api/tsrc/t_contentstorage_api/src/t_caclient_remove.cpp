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

#include "t_caclient.h"
#include "caservice.h"
#include "caquery.h"

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_removeEntry_ById()
{
    {
        CaEntry item;
        item.setText("testName");
        item.setAttribute("attrName1", "attrVal1");
        item.setEntryTypeName("test");
        item.setFlags(RemovableEntryFlag);

        QSharedPointer<CaService> service = CaService::instance();
        QSharedPointer<CaEntry> resultItem1 = service->createEntry(item);
        QVERIFY(!resultItem1.isNull());
        QVERIFY(!service->removeEntry(9999));
        QVERIFY(service->removeEntry(resultItem1->id()));
        QCOMPARE(service->lastError() , NoErrorCode);

        QSharedPointer<CaEntry> storageItem1 = service->getEntry(resultItem1->id());
        QVERIFY(storageItem1.isNull());  // entry not present in the storage
        QVERIFY(!service->removeEntry(resultItem1->id()));
        QVERIFY(service->lastError() != NoErrorCode);

        QSharedPointer<CaEntry> resultItem2 = service->createEntry(item);
        QVERIFY(!resultItem2.isNull());

        QVERIFY(service->removeEntry(*resultItem2));
        QSharedPointer<CaEntry> storageItem2 = service->getEntry(resultItem2->id());
        QVERIFY(storageItem2.isNull());  // entry not present in the storage
        QVERIFY(!service->removeEntry(*resultItem2));

        //cleanup
        service->removeEntry(*resultItem1);
        service->removeEntry(*resultItem2);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_removeEntry_ByEntry()
{
    {
        CaEntry item;
        item.setText("testName");
        item.setAttribute("attrName1", "attrVal1");
        item.setEntryTypeName("test");
        item.setFlags(RemovableEntryFlag);

        QSharedPointer<CaService> service = CaService::instance();
        QSharedPointer<CaEntry> resultItem1 = service->createEntry(item);
        QVERIFY(!resultItem1.isNull());

        QVERIFY(service->removeEntry(*resultItem1));
        QCOMPARE(service->lastError() , NoErrorCode);

        QSharedPointer<CaEntry> storageItem1 = service->getEntry(resultItem1->id());
        QVERIFY(storageItem1.isNull());  // entry not present in the storage
        
        //cleanup
        service->removeEntry(*resultItem1);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_removeEntries_ByIds()
{
    {
        CaEntry item;
        item.setText("testName");
        item.setAttribute("attrName1", "attrVal1");
        item.setEntryTypeName("test");
        item.setFlags(RemovableEntryFlag);

        QSharedPointer<CaService> service = CaService::instance();
        QSharedPointer<CaEntry> resultItem1 = service->createEntry(item);
        QSharedPointer<CaEntry> resultItem2 = service->createEntry(item);
        QVERIFY(!resultItem1.isNull());
        QVERIFY(!resultItem2.isNull());

        QList<int> ids;
        ids.append(resultItem1->id());
        ids.append(resultItem2->id());
        QVERIFY(service->removeEntries(ids));
        QSharedPointer<CaEntry> storageItem1 = service->getEntry(resultItem1->id());
        QSharedPointer<CaEntry> storageItem2 = service->getEntry(resultItem2->id());
        QVERIFY(storageItem1.isNull());  // entry not present in the storage
        QVERIFY(storageItem2.isNull());  // entry not present in the storage

        //cleanup
        service->removeEntry(*resultItem1);
        service->removeEntry(*resultItem2);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_RemoveEntries_ByEntries()
{
    {
        CaEntry item;
        item.setText("testName");
        item.setAttribute("attrName1", "attrVal1");
        item.setEntryTypeName("test");
        item.setFlags(RemovableEntryFlag);

        QSharedPointer<CaService> service = CaService::instance();
        QSharedPointer<CaEntry> resultItem3 = service->createEntry(item);
        QSharedPointer<CaEntry> resultItem4 = service->createEntry(item);
        QVERIFY(!resultItem3.isNull());
        QVERIFY(!resultItem4.isNull());

        QList<QSharedPointer<CaEntry> > entries;
        entries.append(resultItem3);
        entries.append(resultItem4);

        QVERIFY(service->removeEntries(entries));
        QSharedPointer<CaEntry> storageItem3 = service->getEntry(resultItem3->id());
        QSharedPointer<CaEntry> storageItem4 = service->getEntry(resultItem4->id());
        QVERIFY(storageItem3.isNull());  // entry not present in the storage
        QVERIFY(storageItem4.isNull());  // entry not present in the storage

        //cleanup
        service->removeEntry(*resultItem3);
        service->removeEntry(*resultItem4);
    }
}
