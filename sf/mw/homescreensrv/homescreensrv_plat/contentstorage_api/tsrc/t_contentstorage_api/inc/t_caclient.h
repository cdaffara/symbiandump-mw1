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

#ifndef T_CACLIENT_H
#define T_CACLIENT_H

#include <QtTest/QtTest>
#include "cadefs.h"

#ifdef Q_OS_SYMBIAN
#include <e32std.h>
#endif

class CaService;
class CaEntry;
class CaItemModel;

/**
 * @test Test class for hsmcsserviceprovider library.
 */
class TestCaClient : public QObject
{
    Q_OBJECT

private slots:

    /**
     * initialize
     */
    void initTestCase();
    
    // contentsorage_api - caservice
    void Test_instance();
    
    void Test_getEntry();
    void Test_getEntries_ByIds();
    void Test_getEntries_ByQuery();
    
    void Test_createEntry();

    void Test_removeEntry_ById();
    void Test_removeEntry_ByEntry();
    void Test_removeEntries_ByIds();
    void Test_RemoveEntries_ByEntries();
    
    void Test_epdateEntry();
    void Test_touch();
    
    void Test_insertEntryIntoGroup_ById();
    void Test_insertEntryIntoGroup_ByEntry();
    void Test_insertEntriesIntoGroup_ByIds();
    void Test_insertEntriesIntoGroup_ByEntries();
    
    void Test_removeEntryFromGroup_ById();
    void Test_removeEntryFromGroup_ByEntry();
    void Test_removeEntriesFromGroup_ByIds();
    void Test_removeEntriesFromGroup_ByEntries();
    
    void Test_appendEntryToGroup_ById();
    void Test_appendEntryToGroup_ByEntry();
    void Test_appendEntriesToGroup_ByIds();
    void Test_appendEntriesToGroup_ByEntries();
    
    void Test_prependEntryToGroup_ById();
    void Test_prependEntryToGroup_ByEntry();
    void Test_prependEntriesToGroup_ByIds();
    void Test_prependEntriesToGroup_ByEntries();
#ifdef Q_OS_SYMBIAN
    void Test_executeCommand_ById();
    void Test_executeCommand_ByEntry();
#endif //Q_OS_SYMBIAN
    
    void Test_createNotifier();
    void Test_customSort();
    void Test_lastError();

    // contentsorage_api - camenuiconutility
    void Test_getApplicationIcon();
    void Test_getEntryIcon();
    
    /**
     * cleanup
     */
    void cleanupTestCase();

private:
    void startThread();
    void stopThread();
    // utility methods
    QSharedPointer<CaEntry>  createGroup(const QString &groupName, const QString &typeName);
    QSharedPointer<CaEntry> createItem(const QString &itemName, const QString &description,
                        const QString &typeName, EntryFlags flags);
    QSharedPointer<CaEntry> createItem(const QString &itemName, const QString &typeName);
    void addItemToGroup(int groupId, int itemId);
    void removeItemFromGroup(int groupId, int itemId);
    void removeEntry(int entryId);
    bool removeEntry(const QString &entryName);
    QString itemName(const CaItemModel &model, int row);
    void terminatServer();

#ifdef Q_OS_SYMBIAN
    RThread iThread;
#endif //Q_OS_SYMBIAN

private:
    QSharedPointer<CaService> mService;
};


#endif//T_CACLIENT_H
