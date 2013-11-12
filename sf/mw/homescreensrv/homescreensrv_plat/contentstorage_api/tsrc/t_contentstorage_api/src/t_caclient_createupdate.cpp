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
void TestCaClient::Test_instance()
{
    {
        QSharedPointer<CaService> service = CaService::instance();
        QVERIFY(!service.isNull());
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_lastError()
{
    {
        //create item
        CaEntry item;
        item.setText("testName");
        item.setAttribute("attrName1", "attrVal1");
        item.setEntryTypeName("test");
        item.setFlags(RemovableEntryFlag);
        item.setDescription("description");
        CaIconDescription iconDescription;
        iconDescription.setFilename("iconFileName");
        iconDescription.setSkinId("iconSkinId");
        iconDescription.setApplicationId("268458321");

        item.setIconDescription(iconDescription);

        QSharedPointer<CaService> service = CaService::instance();
        QSharedPointer<CaEntry> resultItem = service->createEntry(item);
        QCOMPARE(service->lastError(), NoErrorCode);

        //Cleanup
        service->removeEntry(*resultItem);
        QCOMPARE(service->lastError(), NoErrorCode);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_createEntry()
{
    {
        const CaEntry constItem;
        QCOMPARE(constItem.role(), ItemEntryRole);

        //create item
        //check if proper entry returned
        CaEntry item;
        item.setText("testName");
        item.setAttribute("attrName1", "attrVal1");
        item.setEntryTypeName("test");
        item.setFlags(RemovableEntryFlag);
        item.setDescription("description");
        CaIconDescription iconDescription;
        iconDescription.setFilename("iconFileName");
        iconDescription.setSkinId("iconSkinId");
        iconDescription.setApplicationId("268458321");

        item.setIconDescription(iconDescription);

        QSharedPointer<CaService> service = CaService::instance();
        QSharedPointer<CaEntry> resultItem = service->createEntry(item);
        QCOMPARE(service->lastError(), NoErrorCode);
        QVERIFY(!resultItem.isNull());
        QCOMPARE(resultItem->role(), ItemEntryRole);
        QCOMPARE(item.text(),resultItem->text());
        QCOMPARE(item.description(),resultItem->description());
        QCOMPARE(item.entryTypeName(),resultItem->entryTypeName());
        QCOMPARE(QString("attrVal1"),resultItem->attribute("attrName1"));
        QCOMPARE(item.flags(),resultItem->flags());
        int itId =item.id();
        int resItId =resultItem->id();
        QVERIFY(itId != resItId);
        QCOMPARE(iconDescription.filename(),resultItem->iconDescription().filename());
        QCOMPARE(iconDescription.skinId(),resultItem->iconDescription().skinId());
        QCOMPARE(iconDescription.applicationId(),resultItem->iconDescription().applicationId());

        // entry from storage
        QSharedPointer<CaEntry> storageItem1 = service->getEntry(resultItem->id());
        QVERIFY(storageItem1);
        QCOMPARE(QString("testName"),storageItem1->text());
        QCOMPARE(QString("description"),storageItem1->description());
        QCOMPARE(QString("test"),storageItem1->entryTypeName());
        QCOMPARE(QString("attrVal1"),storageItem1->attribute("attrName1"));
        EntryFlags flags = item.flags();
        EntryFlags storageFlags = storageItem1->flags();
        QCOMPARE(iconDescription.filename(),storageItem1->iconDescription().filename());
        QCOMPARE(iconDescription.skinId(),storageItem1->iconDescription().skinId());
        QCOMPARE(iconDescription.applicationId(),storageItem1->iconDescription().applicationId());
        QVERIFY(storageItem1->iconDescription().id() > 0);

        QCOMPARE(flags ,storageFlags);

        //Cleanup
        service->removeEntry(*resultItem);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_epdateEntry()
{
    {
        CaEntry item1;
        item1.setText("testName");
        item1.setAttribute("attrName1", "attrVal1");
        item1.setEntryTypeName("test");
        item1.setFlags(RemovableEntryFlag);
        CaIconDescription iconDescription1;
        iconDescription1.setFilename("iconFileName");
        iconDescription1.setSkinId("iconSkinId");
        iconDescription1.setApplicationId("268458321"); //Bluetouth app UID
        item1.setIconDescription(iconDescription1);

        CaIconDescription iconDescription2;
        iconDescription2.setFilename("iconFileName2");
        iconDescription2.setSkinId("iconSkinId2");
        iconDescription2.setApplicationId("537001519"); //SW update app UID

        QSharedPointer<CaService> service = CaService::instance();
        QSharedPointer<CaEntry> resultItem1 = service->createEntry(item1);
        QCOMPARE(service->lastError(), NoErrorCode);

        QVERIFY(!resultItem1.isNull());

        int id1 = resultItem1->id();
        resultItem1->setText("testName2");
        resultItem1->setAttribute("attrName2", "attrVal2");
        resultItem1->setAttribute("attrName1", "attrVal2");
        resultItem1->setEntryTypeName("test2");
        resultItem1->setFlags(UninstallEntryFlag);
        resultItem1->setIconDescription(iconDescription2);

        bool updateResult1 = service->updateEntry(item1);
        QVERIFY(service->lastError() != NoErrorCode);

        QVERIFY(!updateResult1);  // no id -> return false

        bool updateResult2 = service->updateEntry(*resultItem1);
        QCOMPARE(service->lastError(), NoErrorCode);
        QVERIFY(updateResult2);  // id present -> true

        QSharedPointer<CaEntry> resultItem2 = service->createEntry(*resultItem1);
        QCOMPARE(service->lastError(), NoErrorCode);
        QVERIFY(!resultItem2.isNull());

        int id2 = resultItem2->id();
        QVERIFY(id1 != id2);   //entry clone created

        QSharedPointer<CaEntry> storageItem1 = service->getEntry(id1);
        QVERIFY(!storageItem1.isNull());

        QSharedPointer<CaEntry> storageItem2 = service->getEntry(id2);
        QVERIFY(!storageItem2.isNull());

        // check if attributes were updated
        QCOMPARE(QString("testName2"),storageItem1->text());
        QCOMPARE(QString("test2"),storageItem1->entryTypeName());

        QCOMPARE(QString("attrVal2"),storageItem1->attribute("attrName1"));
        QCOMPARE(QString("attrVal2"),storageItem1->attribute("attrName2"));
        QCOMPARE(resultItem1->flags(),storageItem1->flags());
        QCOMPARE(iconDescription2.filename(),storageItem1->iconDescription().filename());
        QCOMPARE(iconDescription2.skinId(),storageItem1->iconDescription().skinId());
        QCOMPARE(iconDescription2.applicationId(),storageItem1->iconDescription().applicationId());
        // cloned item should have the same attributes
        QCOMPARE(QString("testName2"),storageItem2->text());
        QCOMPARE(QString("test2"),storageItem2->entryTypeName());
        QCOMPARE(QString("attrVal2"),storageItem2->attribute("attrName1"));
        QCOMPARE(QString("attrVal2"),storageItem2->attribute("attrName2"));
        QCOMPARE(resultItem1->flags(),storageItem2->flags());

        //Cleanup
        service->removeEntry(id1);
        service->removeEntry(id2);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::terminatServer()
{
    TFindProcess processFinder;
    processFinder.Find(_L("camenuserver*"));
    TFullName caName;
    while ( KErrNone == processFinder.Next( caName ) ) {
        RProcess caProcess;
        TInt err = caProcess.Open(caName);
        if( KErrNone ==  err ) {
            caProcess.Terminate(0);
        }
        caProcess.Close();
    }
}

QTEST_MAIN(TestCaClient)
