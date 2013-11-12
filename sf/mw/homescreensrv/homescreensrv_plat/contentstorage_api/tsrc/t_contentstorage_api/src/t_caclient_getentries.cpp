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
void TestCaClient::Test_getEntry()
{
    {
        CaEntry item;
        item.setText("testName");
        item.setAttribute("attrName1", "attrVal1");
        item.setEntryTypeName("test");
        item.setFlags(RemovableEntryFlag);

        CaIconDescription iconDescription;
        iconDescription.setFilename("iconFileName");
        iconDescription.setSkinId("iconSkinId");
        iconDescription.setApplicationId("268458321");
        item.setIconDescription(iconDescription);

        QSharedPointer<CaService> service = CaService::instance();
        QSharedPointer<CaEntry> resultItem = service->createEntry(item);

        QVERIFY(!resultItem.isNull());
        // entry from storage
        QSharedPointer<CaEntry> storageItem1 = service->getEntry(resultItem->id());
        QCOMPARE(service->lastError(), NoErrorCode);
        QVERIFY(storageItem1);
        QCOMPARE(storageItem1->role(), ItemEntryRole);
        QCOMPARE(QString("testName"),storageItem1->text());
        QCOMPARE(QString("test"),storageItem1->entryTypeName());
        QCOMPARE(QString("attrVal1"),storageItem1->attribute("attrName1"));
        QCOMPARE(item.flags(),storageItem1->flags());
        QCOMPARE(iconDescription.filename(),storageItem1->iconDescription().filename());
        QCOMPARE(iconDescription.skinId(),storageItem1->iconDescription().skinId());
        QCOMPARE(iconDescription.applicationId(),storageItem1->iconDescription().applicationId());
        QSharedPointer<CaEntry> storageItem2 = service->getEntry(9999);
        QVERIFY(service->lastError() !=  NoErrorCode);
        QVERIFY(!storageItem2);

        //Cleanup
        service->removeEntry(*resultItem);

    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_getEntries_ByIds()
{
    {
        CaEntry item;
        item.setText("testName1");
        item.setAttribute("attrName1", "attrVal1");
        item.setEntryTypeName("test");
        item.setFlags(RemovableEntryFlag);

        QSharedPointer<CaService> service = CaService::instance();
        QSharedPointer<CaEntry> resultItem1 = service->createEntry(item);
        item.setText("testName2");
        QSharedPointer<CaEntry> resultItem2 = service->createEntry(item);
        item.setText("testName3");
        QSharedPointer<CaEntry> resultItem3 = service->createEntry(item);
        QVERIFY(!resultItem1.isNull());
        QVERIFY(!resultItem2.isNull());
        QVERIFY(!resultItem3.isNull());

        QList<int> ids;
        ids.append(resultItem1->id());
        ids.append(resultItem3->id());
        ids.append(resultItem2->id());

        QList< QSharedPointer<CaEntry> > entries = service->getEntries(ids) ;
        QCOMPARE(entries.count(), 3);
        QCOMPARE(service->lastError(), NoErrorCode);

        QVERIFY(entries.at(0));
        QCOMPARE(entries.at(0)->role(), ItemEntryRole);
        QCOMPARE(QString("testName1"),entries.at(0)->text());
        QCOMPARE(QString("test"),entries.at(0)->entryTypeName());
        QCOMPARE(QString("attrVal1"),entries.at(0)->attribute("attrName1"));
        QCOMPARE(item.flags(),entries.at(0)->flags());
        QVERIFY(entries.at(1));
        QCOMPARE(entries.at(1)->role(), ItemEntryRole);
        QCOMPARE(QString("testName3"),entries.at(1)->text());
        QCOMPARE(QString("test"),entries.at(1)->entryTypeName());
        QCOMPARE(QString("attrVal1"),entries.at(1)->attribute("attrName1"));
        QCOMPARE(item.flags(),entries.at(1)->flags());
        QVERIFY(entries.at(2));
        QCOMPARE(entries.at(2)->role(), ItemEntryRole);
        QCOMPARE(QString("testName2"),entries.at(2)->text());
        QCOMPARE(QString("test"),entries.at(2)->entryTypeName());
        QCOMPARE(QString("attrVal1"),entries.at(2)->attribute("attrName1"));
        QCOMPARE(item.flags(),entries.at(2)->flags());

        QCOMPARE(entries.at(0)->id(),resultItem1->id());
        QCOMPARE(entries.at(1)->id(),resultItem3->id());
        QCOMPARE(entries.at(2)->id(),resultItem2->id());

        QList<int> ids2;
        ids2.append(resultItem1->id());
        ids2.append(9999);

        QList< QSharedPointer<CaEntry> > entries2 = service->getEntries(ids2) ;
        QCOMPARE(entries2.count(), 1);
        QVERIFY(service->lastError() != NoErrorCode);

        //Cleanup
        service->removeEntry(*resultItem1);
        service->removeEntry(*resultItem2);
        service->removeEntry(*resultItem3);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_getEntries_ByQuery()
{
    {
        //CHECK INITIAL STATE IN DB
        QSharedPointer<CaService> service = CaService::instance();
        CaQuery query;
        QStringList strList;
        strList << QString("test_item1");
        query.setEntryTypeNames(strList);
        query.addEntryTypeName(QString("test_item2"));
        QList< QSharedPointer<CaEntry> > entries12 = service->getEntries(query);
        int count12Before = entries12.count();

        //ADD TEST DATA TO THE STORAGE
        CaEntry item1;
        item1.setText("testName_item1");
        item1.setAttribute("attrName1_item", "attrVal1_item");
        item1.setEntryTypeName("test_item1");
        CaEntry item2;
        item2.setText("testName_item2");
        item2.setAttribute("attrName2_item", "attrVal2_item");
        item2.setEntryTypeName("test_item2");

        QSharedPointer<CaEntry> resultEntry1 = service->createEntry(item1);
        QSharedPointer<CaEntry> resultEntry2 = service->createEntry(item2);
        QVERIFY(!resultEntry1.isNull());
        QVERIFY(!resultEntry2.isNull());

        //GET STATE FROM THE STORAGE AFTER ADD
        strList.clear();
        strList << QString("test_item1");
        query.setEntryTypeNames(strList);
        query.addEntryTypeName(QString("test_item2"));
        entries12 = service->getEntries(query);
        QCOMPARE(service->lastError(), NoErrorCode);
        int count12After = entries12.count();

        //CHECK RESULTS
        QCOMPARE(count12Before+2,count12After);

        //CLEANUP THE STORAGE AFTER THE TEST
        service->removeEntry(*resultEntry1);
        service->removeEntry(*resultEntry2);
    }
}

