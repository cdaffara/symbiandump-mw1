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
void TestCaClient::Test_appendEntryToGroup_ById()
{
    {
        CaEntry itemEntry;
        itemEntry.setText("item");
        itemEntry.setEntryTypeName("typeok");
        CaEntry groupEntry(GroupEntryRole);
        groupEntry.setText("group");
        groupEntry.setEntryTypeName("typeok");
        QSharedPointer<CaService> service = CaService::instance();

        QSharedPointer<CaEntry> entry1 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> entry2 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> group = service->createEntry(groupEntry);

        CaQuery query;
        query.setParentId(group->id());
        query.setSort(DefaultSortAttribute, Qt::AscendingOrder);

        QVERIFY(!service->appendEntryToGroup(9999, entry1->id()));
        QVERIFY(service->lastError() != NoErrorCode);

        QVERIFY(!service->appendEntryToGroup(group->id(), 9999));
        QVERIFY(service->lastError() != NoErrorCode);

        QVERIFY(service->appendEntryToGroup(group->id(), entry1->id()));
        QCOMPARE(service->lastError() , NoErrorCode);

        QList<int> entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),1);
        QVERIFY(service->appendEntryToGroup(group->id(), entry2->id()));
        entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),2);
        QCOMPARE(entries.at(0),entry1->id());
        QCOMPARE(entries.at(1),entry2->id());

        //Cleanup
        service->removeEntry(*entry1);
        service->removeEntry(*entry2);
        service->removeEntry(*group);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_appendEntryToGroup_ByEntry()
{
    {
        CaEntry itemEntry;
        itemEntry.setText("item");
        itemEntry.setEntryTypeName("typeok");
        CaEntry groupEntry(GroupEntryRole);
        groupEntry.setText("group");
        groupEntry.setEntryTypeName("typeok");
        QSharedPointer<CaService> service = CaService::instance();

        QSharedPointer<CaEntry> entry1 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> entry2 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> group = service->createEntry(groupEntry);

        CaQuery query;
        query.setParentId(group->id());
        query.setSort(DefaultSortAttribute, Qt::AscendingOrder);

        QVERIFY(!service->appendEntryToGroup(groupEntry, *entry1));
        QVERIFY(!service->appendEntryToGroup(*group , itemEntry));
        QVERIFY(service->appendEntryToGroup(*group , *entry1));
        QList<int> entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),1);
        QVERIFY(service->appendEntryToGroup(*group, *entry2));
        entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),2);
        QCOMPARE(entries.at(0),entry1->id());
        QCOMPARE(entries.at(1),entry2->id());

        //Cleanup
        service->removeEntry(*entry1);
        service->removeEntry(*entry2);
        service->removeEntry(*group);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_appendEntriesToGroup_ByIds()
{
    {
        CaEntry itemEntry;
        itemEntry.setText("item");
        itemEntry.setEntryTypeName("typeok");
        CaEntry groupEntry(GroupEntryRole);
        groupEntry.setText("group");
        groupEntry.setEntryTypeName("typeok");
        QSharedPointer<CaService> service = CaService::instance();

        QSharedPointer<CaEntry> entry1 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> entry2 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> entry3 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> entry4 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> group = service->createEntry(groupEntry);

        QList<int> entries1;
        entries1.append(entry1->id());
        entries1.append(entry2->id());
        QList<int> entries2;
        entries2.append(entry3->id());
        entries2.append(entry4->id());
        QList<int> wrongEntries1;
        wrongEntries1.append(entry1->id());
        wrongEntries1.append(9999);


        CaQuery query;
        query.setParentId(group->id());
        query.setSort(DefaultSortAttribute, Qt::AscendingOrder);

        QVERIFY(!service->appendEntriesToGroup(9999, entries1));
        QVERIFY(!service->appendEntriesToGroup(group->id(), wrongEntries1));
        QList<int> entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),0);

        QVERIFY(service->appendEntriesToGroup(group->id(), entries1));
        entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),2);
        QVERIFY(service->appendEntriesToGroup(group->id(), entries2));
        entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),4);

        QCOMPARE(entries.at(0),entry1->id());
        QCOMPARE(entries.at(1),entry2->id());
        QCOMPARE(entries.at(2),entry3->id());
        QCOMPARE(entries.at(3),entry4->id());

        //Cleanup
        service->removeEntry(*entry1);
        service->removeEntry(*entry2);
        service->removeEntry(*entry3);
        service->removeEntry(*entry4);
        service->removeEntry(*group);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_appendEntriesToGroup_ByEntries()
{
    {
        QSharedPointer<CaEntry> itemEntry (new CaEntry());
        itemEntry->setText("item");
        itemEntry->setEntryTypeName("typeok");
        CaEntry groupEntry(GroupEntryRole);
        groupEntry.setText("group");
        groupEntry.setEntryTypeName("typeok");
        QSharedPointer<CaService> service = CaService::instance();

        QSharedPointer<CaEntry> entry1 = service->createEntry(*itemEntry);
        QSharedPointer<CaEntry> entry2 = service->createEntry(*itemEntry);
        QSharedPointer<CaEntry> entry3 = service->createEntry(*itemEntry);
        QSharedPointer<CaEntry> entry4 = service->createEntry(*itemEntry);
        QSharedPointer<CaEntry> group = service->createEntry(groupEntry);

        QList< QSharedPointer<CaEntry> > entries1;
        entries1.append(entry1);
        entries1.append(entry2);
        QList< QSharedPointer<CaEntry> > entries2;
        entries2.append(entry3);
        entries2.append(entry4);
        QList<QSharedPointer<CaEntry> > wrongEntries1;
        wrongEntries1.append(entry1);
        wrongEntries1.append(itemEntry);  //this item has unspecified id

        CaQuery query;
        query.setParentId(group->id());
        query.setSort(DefaultSortAttribute, Qt::AscendingOrder);

        QVERIFY(!service->appendEntriesToGroup(groupEntry, entries1));
        QVERIFY(!service->appendEntriesToGroup(*group, wrongEntries1));
        QList<int> entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),0);
        QVERIFY(service->appendEntriesToGroup(*group, entries1));
        entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),2);
        QVERIFY(service->appendEntriesToGroup(*group, entries2));
        entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),4);

        QCOMPARE(entries.at(0),entry1->id());
        QCOMPARE(entries.at(1),entry2->id());
        QCOMPARE(entries.at(2),entry3->id());
        QCOMPARE(entries.at(3),entry4->id());

        //Cleanup
        service->removeEntry(*entry1);
        service->removeEntry(*entry2);
        service->removeEntry(*entry3);
        service->removeEntry(*entry4);
        service->removeEntry(*group);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_prependEntryToGroup_ById()
{
    {
        CaEntry itemEntry;
        itemEntry.setText("item");
        itemEntry.setEntryTypeName("typeok");
        CaEntry groupEntry(GroupEntryRole);
        groupEntry.setText("group");
        groupEntry.setEntryTypeName("typeok");
        QSharedPointer<CaService> service = CaService::instance();

        QSharedPointer<CaEntry> entry1 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> entry2 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> group = service->createEntry(groupEntry);

        CaQuery query;
        query.setParentId(group->id());
        query.setSort(DefaultSortAttribute, Qt::AscendingOrder);

        QVERIFY(!service->prependEntryToGroup(9999, entry1->id()));
        QVERIFY(service->lastError() != NoErrorCode);
        QVERIFY(!service->prependEntryToGroup(group->id(), 9999));
        QVERIFY(service->lastError() != NoErrorCode);
        QVERIFY(service->prependEntryToGroup(group->id(), entry1->id()));
        QCOMPARE(service->lastError() , NoErrorCode);

        QList<int> entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),1);
        QVERIFY(service->prependEntryToGroup(group->id(), entry2->id()));
        entries = service->getEntryIds(query);

        QCOMPARE(entries.count(),2);
        QCOMPARE(entries.at(0),entry2->id());
        QCOMPARE(entries.at(1),entry1->id());

        //Cleanup
        service->removeEntry(*entry1);
        service->removeEntry(*entry2);
        service->removeEntry(*group);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_prependEntryToGroup_ByEntry()
{
    {
        CaEntry itemEntry;
        itemEntry.setText("item");
        itemEntry.setEntryTypeName("typeok");
        CaEntry groupEntry(GroupEntryRole);
        groupEntry.setText("group");
        groupEntry.setEntryTypeName("typeok");
        QSharedPointer<CaService> service = CaService::instance();

        QSharedPointer<CaEntry> entry1 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> entry2 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> group = service->createEntry(groupEntry);

        CaQuery query;
        query.setParentId(group->id());
        query.setSort(DefaultSortAttribute, Qt::AscendingOrder);

        QVERIFY(!service->prependEntryToGroup(groupEntry, *entry1));
        QVERIFY(!service->prependEntryToGroup(*group , itemEntry));
        QVERIFY(service->prependEntryToGroup(*group , *entry1));
        QList<int> entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),1);
        QVERIFY(service->prependEntryToGroup(*group, *entry2));
        entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),2);
        QCOMPARE(entries.at(0),entry2->id());
        QCOMPARE(entries.at(1),entry1->id());

        //Cleanup
        service->removeEntry(*entry1);
        service->removeEntry(*entry2);
        service->removeEntry(*group);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_prependEntriesToGroup_ByIds()
{
    {
        CaEntry itemEntry;
        itemEntry.setText("item");
        itemEntry.setEntryTypeName("typeok");
        CaEntry groupEntry(GroupEntryRole);
        groupEntry.setText("group");
        groupEntry.setEntryTypeName("typeok");
        QSharedPointer<CaService> service = CaService::instance();

        QSharedPointer<CaEntry> entry1 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> entry2 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> entry3 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> entry4 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> group = service->createEntry(groupEntry);

        QList<int> entries1;
        entries1.append(entry1->id());
        entries1.append(entry2->id());
        QList<int> entries2;
        entries2.append(entry3->id());
        entries2.append(entry4->id());
        QList<int> wrongEntries1;
        wrongEntries1.append(entry1->id());
        wrongEntries1.append(9999);


        CaQuery query;
        query.setParentId(group->id());
        query.setSort(DefaultSortAttribute, Qt::AscendingOrder);

        QVERIFY(!service->prependEntriesToGroup(9999, entries1));
        QVERIFY(!service->prependEntriesToGroup(group->id(), wrongEntries1));
        QList<int> entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),0);

        QVERIFY(service->prependEntriesToGroup(group->id(), entries1));
        entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),2);
        QVERIFY(service->prependEntriesToGroup(group->id(), entries2));
        entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),4);

        QCOMPARE(entries.at(0),entry3->id());
        QCOMPARE(entries.at(1),entry4->id());
        QCOMPARE(entries.at(2),entry1->id());
        QCOMPARE(entries.at(3),entry2->id());

        //Cleanup
        service->removeEntry(*entry1);
        service->removeEntry(*entry2);
        service->removeEntry(*entry3);
        service->removeEntry(*entry4);
        service->removeEntry(*group);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_prependEntriesToGroup_ByEntries()
{
    {
        QSharedPointer<CaEntry> itemEntry (new CaEntry());
        itemEntry->setText("item");
        itemEntry->setEntryTypeName("typeok");
        CaEntry groupEntry(GroupEntryRole);
        groupEntry.setText("group");
        groupEntry.setEntryTypeName("typeok");
        QSharedPointer<CaService> service = CaService::instance();

        QSharedPointer<CaEntry> entry1 = service->createEntry(*itemEntry);
        QSharedPointer<CaEntry> entry2 = service->createEntry(*itemEntry);
        QSharedPointer<CaEntry> entry3 = service->createEntry(*itemEntry);
        QSharedPointer<CaEntry> entry4 = service->createEntry(*itemEntry);
        QSharedPointer<CaEntry> group = service->createEntry(groupEntry);

        QList< QSharedPointer<CaEntry> > entries1;
        entries1.append(entry1);
        entries1.append(entry2);
        QList< QSharedPointer<CaEntry> > entries2;
        entries2.append(entry3);
        entries2.append(entry4);
        QList< QSharedPointer<CaEntry> > wrongEntries1;
        wrongEntries1.append(entry1);
        wrongEntries1.append(itemEntry);  //this item has unspecified id

        CaQuery query;
        query.setParentId(group->id());
        query.setSort(DefaultSortAttribute, Qt::AscendingOrder);

        QVERIFY(!service->prependEntriesToGroup(groupEntry, entries1));
        QVERIFY(!service->prependEntriesToGroup(*group, wrongEntries1));
        QList<int> entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),0);
        QVERIFY(service->prependEntriesToGroup(*group, entries1));
        entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),2);
        QVERIFY(service->prependEntriesToGroup(*group, entries2));
        entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),4);

        QCOMPARE(entries.at(0),entry3->id());
        QCOMPARE(entries.at(1),entry4->id());
        QCOMPARE(entries.at(2),entry1->id());
        QCOMPARE(entries.at(3),entry2->id());

        //Cleanup
        service->removeEntry(*entry1);
        service->removeEntry(*entry2);
        service->removeEntry(*entry3);
        service->removeEntry(*entry4);
        service->removeEntry(*group);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_insertEntryIntoGroup_ById()
{
    {
        CaEntry itemEntry;
        itemEntry.setText("item");
        itemEntry.setEntryTypeName("typeok");
        CaEntry groupEntry(GroupEntryRole);
        groupEntry.setText("group");
        groupEntry.setEntryTypeName("typeok");
        QSharedPointer<CaService> service = CaService::instance();

        QSharedPointer<CaEntry> entry1 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> entry2 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> group = service->createEntry(groupEntry);

        CaQuery query;
        query.setParentId(group->id());
        query.setSort(DefaultSortAttribute, Qt::AscendingOrder);

        QVERIFY(service->prependEntryToGroup(group->id(), entry1->id()));

        QVERIFY(!service->insertEntryIntoGroup(9999, entry2->id(), entry1->id()));
        QVERIFY(service->lastError() != NoErrorCode);

        QVERIFY(!service->insertEntryIntoGroup(group->id(), 9999, entry1->id()));
        QVERIFY(service->lastError() != NoErrorCode);

        QVERIFY(!service->insertEntryIntoGroup(group->id(), entry2->id(), 9999));
        QVERIFY(service->lastError() != NoErrorCode);

        QVERIFY(service->insertEntryIntoGroup(group->id(), entry2->id(), entry1->id()));
        QCOMPARE(service->lastError() , NoErrorCode);

        QList<int> entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),2);
        QCOMPARE(entries.at(0),entry2->id());
        QCOMPARE(entries.at(1),entry1->id());

        //Cleanup
        service->removeEntry(*entry1);
        service->removeEntry(*entry2);
        service->removeEntry(*group);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_insertEntryIntoGroup_ByEntry()
{
    {
        CaEntry itemEntry;
        itemEntry.setText("item");
        itemEntry.setEntryTypeName("typeok");
        CaEntry groupEntry(GroupEntryRole);
        groupEntry.setText("group");
        groupEntry.setEntryTypeName("typeok");
        QSharedPointer<CaService> service = CaService::instance();

        QSharedPointer<CaEntry> entry1 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> entry2 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> group = service->createEntry(groupEntry);

        CaQuery query;
        query.setParentId(group->id());
        query.setSort(DefaultSortAttribute, Qt::AscendingOrder);

        QVERIFY(service->prependEntryToGroup(*group, *entry1));

        QVERIFY(!service->insertEntryIntoGroup(groupEntry, *entry2, entry1->id()));
        QVERIFY(!service->insertEntryIntoGroup(*group, itemEntry, entry1->id()));
        QVERIFY(!service->insertEntryIntoGroup(*group, *entry2, 9999));

        QVERIFY(service->insertEntryIntoGroup(*group, *entry2, entry1->id()));

        QList<int> entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),2);
        QCOMPARE(entries.at(0),entry2->id());
        QCOMPARE(entries.at(1),entry1->id());

        //Cleanup
        service->removeEntry(*entry1);
        service->removeEntry(*entry2);
        service->removeEntry(*group);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_insertEntriesIntoGroup_ByIds()
{
    {
        CaEntry itemEntry;
        itemEntry.setText("item");
        itemEntry.setEntryTypeName("typeok");
        CaEntry groupEntry(GroupEntryRole);
        groupEntry.setText("group");
        groupEntry.setEntryTypeName("typeok");
        QSharedPointer<CaService> service = CaService::instance();

        QSharedPointer<CaEntry> entry1 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> entry3 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> entry4 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> group = service->createEntry(groupEntry);

        QList<int> entries2;
        entries2.append(entry3->id());
        entries2.append(entry4->id());
        QList<int> wrongEntries1;
        wrongEntries1.append(entry1->id());
        wrongEntries1.append(9999);


        CaQuery query;
        query.setParentId(group->id());
        query.setSort(DefaultSortAttribute, Qt::AscendingOrder);

        QVERIFY(service->prependEntryToGroup(group->id(), entry1->id()));

        QVERIFY(!service->insertEntriesIntoGroup(9999, entries2, entry1->id()));
        QVERIFY(!service->insertEntriesIntoGroup(group->id(), wrongEntries1, entry1->id()));
        QVERIFY(!service->insertEntriesIntoGroup(group->id(), entries2, 9999));

        QVERIFY(service->insertEntriesIntoGroup(group->id(), entries2, entry1->id()));
        QList<int> entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),3);

        QCOMPARE(entries.at(0),entry3->id());
        QCOMPARE(entries.at(1),entry4->id());
        QCOMPARE(entries.at(2),entry1->id());

        //Cleanup
        service->removeEntry(*entry1);
        service->removeEntry(*entry3);
        service->removeEntry(*entry4);
        service->removeEntry(*group);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_insertEntriesIntoGroup_ByEntries()
{
    {
        QSharedPointer<CaEntry> itemEntry (new CaEntry());
        itemEntry->setText("item");
        itemEntry->setEntryTypeName("typeok");
        CaEntry groupEntry(GroupEntryRole);
        groupEntry.setText("group");
        groupEntry.setEntryTypeName("typeok");
        QSharedPointer<CaService> service = CaService::instance();

        QSharedPointer<CaEntry> entry1 = service->createEntry(*itemEntry);
        QSharedPointer<CaEntry> entry3 = service->createEntry(*itemEntry);
        QSharedPointer<CaEntry> entry4 = service->createEntry(*itemEntry);
        QSharedPointer<CaEntry> group = service->createEntry(groupEntry);

        QList< QSharedPointer<CaEntry> > entries2;
        entries2.append(entry3);
        entries2.append(entry4);
        QList< QSharedPointer<CaEntry> > wrongEntries1;
        wrongEntries1.append(entry1);
        wrongEntries1.append(itemEntry);  //this item has unspecified id

        CaQuery query;
        query.setParentId(group->id());
        query.setSort(DefaultSortAttribute, Qt::AscendingOrder);

        QVERIFY(service->prependEntryToGroup(group->id(), entry1->id()));

        QVERIFY(!service->insertEntriesIntoGroup(groupEntry, entries2, entry1->id()));
        QVERIFY(!service->insertEntriesIntoGroup(*group, wrongEntries1, entry1->id()));
        QVERIFY(!service->insertEntriesIntoGroup(*group, entries2, 9999));

        QVERIFY(service->insertEntriesIntoGroup(*group, entries2, entry1->id()));

        QList<int> entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),3);

        QCOMPARE(entries.at(0),entry3->id());
        QCOMPARE(entries.at(1),entry4->id());
        QCOMPARE(entries.at(2),entry1->id());

        //Cleanup
        service->removeEntry(*entry1);
        service->removeEntry(*entry3);
        service->removeEntry(*entry4);
        service->removeEntry(*group);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_removeEntryFromGroup_ById()
{
    {
        CaEntry itemEntry;
        itemEntry.setText("item");
        itemEntry.setEntryTypeName("typeok");
        CaEntry groupEntry(GroupEntryRole);
        groupEntry.setText("group");
        groupEntry.setEntryTypeName("typeok");
        QSharedPointer<CaService> service = CaService::instance();

        QSharedPointer<CaEntry> entry1 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> group = service->createEntry(groupEntry);

        CaQuery query;
        query.setParentId(group->id());
        query.setSort(DefaultSortAttribute, Qt::AscendingOrder);

        QVERIFY(!service->removeEntryFromGroup(9999, entry1->id()));
        QVERIFY(service->lastError() != NoErrorCode);

        QVERIFY(!service->removeEntryFromGroup(group->id(), 9999));
        QVERIFY(service->lastError() != NoErrorCode);

        QVERIFY(service->prependEntryToGroup(group->id(), entry1->id()));
        QList<int> entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),1);
        QVERIFY(service->removeEntryFromGroup(group->id(), entry1->id()));
        QCOMPARE(service->lastError() , NoErrorCode);


        entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),0);

        //Cleanup
        service->removeEntry(*entry1);
        service->removeEntry(*group);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_removeEntryFromGroup_ByEntry()
{
    {
        CaEntry itemEntry;
        itemEntry.setText("item");
        itemEntry.setEntryTypeName("typeok");
        CaEntry groupEntry(GroupEntryRole);
        groupEntry.setText("group");
        groupEntry.setEntryTypeName("typeok");
        QSharedPointer<CaService> service = CaService::instance();

        QSharedPointer<CaEntry> entry1 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> group = service->createEntry(groupEntry);

        CaQuery query;
        query.setParentId(group->id());
        query.setSort(DefaultSortAttribute, Qt::AscendingOrder);

        QVERIFY(!service->removeEntryFromGroup(groupEntry, *entry1));
        QVERIFY(!service->removeEntryFromGroup(*group, itemEntry));

        QVERIFY(service->prependEntryToGroup(group->id(), entry1->id()));
        QList<int> entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),1);
        QVERIFY(service->removeEntryFromGroup(*group, *entry1));

        entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),0);

        //Cleanup
        service->removeEntry(*entry1);
        service->removeEntry(*group);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_removeEntriesFromGroup_ByIds()
{
    {
        CaEntry itemEntry;
        itemEntry.setText("item");
        itemEntry.setEntryTypeName("typeok");
        CaEntry groupEntry(GroupEntryRole);
        groupEntry.setText("group");
        groupEntry.setEntryTypeName("typeok");
        QSharedPointer<CaService> service = CaService::instance();

        QSharedPointer<CaEntry> entry1 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> entry2 = service->createEntry(itemEntry);
        QSharedPointer<CaEntry> group = service->createEntry(groupEntry);

        QList<int> entries1;
        entries1.append(entry1->id());
        entries1.append(entry2->id());
        QList<int> wrongEntries1;
        wrongEntries1.append(entry1->id());
        wrongEntries1.append(9999);

        CaQuery query;
        query.setParentId(group->id());
        query.setSort(DefaultSortAttribute, Qt::AscendingOrder);

        QVERIFY(service->prependEntriesToGroup(group->id(), entries1));
        QList<int> entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),2);
        QVERIFY(!service->removeEntriesFromGroup(9999, entries1));
        QVERIFY(!service->removeEntriesFromGroup(group->id(), wrongEntries1));
        entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),2);

        QVERIFY(service->removeEntriesFromGroup(group->id(), entries1));
        entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),0);

        //Cleanup
        service->removeEntry(*entry1);
        service->removeEntry(*entry2);
        service->removeEntry(*group);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_removeEntriesFromGroup_ByEntries()
{
    {
        QSharedPointer<CaEntry> itemEntry (new CaEntry);
        itemEntry->setText("item");
        itemEntry->setEntryTypeName("typeok");
        CaEntry groupEntry(GroupEntryRole);
        groupEntry.setText("group");
        groupEntry.setEntryTypeName("typeok");
        QSharedPointer<CaService> service = CaService::instance();

        QSharedPointer<CaEntry> entry1 = service->createEntry(*itemEntry);
        QSharedPointer<CaEntry> entry2 = service->createEntry(*itemEntry);
        QSharedPointer<CaEntry> group = service->createEntry(groupEntry);

        QList<QSharedPointer<CaEntry> > entries1;
        entries1.append(entry1);
        entries1.append(entry2);
        QList<QSharedPointer<CaEntry> > wrongEntries1;
        wrongEntries1.append(entry1);
        wrongEntries1.append(itemEntry);  //this item has unspecified id

        CaQuery query;
        query.setParentId(group->id());
        query.setSort(DefaultSortAttribute, Qt::AscendingOrder);

        QVERIFY(service->prependEntriesToGroup(*group, entries1));
        QList<int> entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),2);

        QVERIFY(!service->removeEntriesFromGroup(groupEntry, entries1));
        QVERIFY(!service->removeEntriesFromGroup(*group, wrongEntries1));
        entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),2);

        QVERIFY(service->removeEntriesFromGroup(*group, entries1));
        entries = service->getEntryIds(query);
        QCOMPARE(entries.count(),0);

        //Cleanup
        service->removeEntry(*entry1);
        service->removeEntry(*entry2);
        service->removeEntry(*group);

    }
}


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_customSort()
{
    CaQuery collectionQuery;
    collectionQuery.addEntryTypeName("collection");
    QSharedPointer<CaService> service = CaService::instance();
    QList<int> collectionIds = service->getEntryIds(collectionQuery);
    QVERIFY(collectionIds.count());
    
    const int groupId = collectionIds.at(0);

    CaQuery query;
    query.setParentId(groupId);
    query.setSort(DefaultSortAttribute, Qt::AscendingOrder);

    QList<int> idsBefore = service->getEntryIds(query);
    QVERIFY(idsBefore.count());

    int first = idsBefore.takeFirst();
    idsBefore.append(first);

    // sort entries is group in custom order
    QVERIFY(service->customSort(groupId, idsBefore));

    // check results
    QList<int> idsAfter = service->getEntryIds(query);
    QVERIFY(idsAfter.count());

    QCOMPARE(idsAfter.count(),idsBefore.count());
    QCOMPARE(idsAfter, idsBefore);
}
