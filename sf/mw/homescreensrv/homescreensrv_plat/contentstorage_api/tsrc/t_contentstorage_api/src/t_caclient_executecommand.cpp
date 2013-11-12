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

#ifdef Q_OS_SYMBIAN
//s60
#include <e32base.h>

#include <W32STD.H>
#include <APGTASK.H>

#include "t_cainstaller.h"


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_executeCommand_ById()
{
    {
        QSharedPointer<CaService> service = CaService::instance();
        QSharedPointer<CaEntry> entry;
        CaQuery query;
        query.setEntryRoles(ItemEntryRole);
        QList< QSharedPointer<CaEntry> > items = service->getEntries(query);
        int itemsCount = items.count();
        QVERIFY(itemsCount >0);
        for (int i=0; i< itemsCount; i++) {
            QString tmp = items[i]->text();
            if (items[i]->text() == "Contacts") {
                entry=items[i];
                break;
            }
        }
        QVERIFY(entry);
        bool result = service->executeCommand(entry->id(), "open") == 0;
        QTest::qWait(2500);
        QVERIFY2(result, "Command *open* failed");
        RWsSession session;
        TInt error = session.Connect();
        TApaTaskList taskList(session);
        TApaTask task = taskList.FindApp(_L("Contacts"));
        QTest::qWait(250);
        bool taskExists(false);
        if (task.Exists()) {
            taskExists = true;
            task.EndTask();
        }
        QTest::qWait(250);
        session.Close();
        QTest::qWait(250);
        QVERIFY(taskExists);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_executeCommand_ByEntry()
{
    {

        _LIT(KAppTestFile, "c:\\testing\\data\\t_caclient\\installs\\sapiapp_S60_3_X_v_1_0_0.sisx");
        CaQuery query;
        QSharedPointer<CaService> service = CaService::instance();
        CaEntry *entry;
        CaEntry *entryPhonebook;
        QTest::qWait(250);
        QStringList list("application");
        query.setEntryTypeNames(list);
        query.setFlagsOn(VisibleEntryFlag);
        RFs TheFs;
        User::LeaveIfError(TheFs.Connect());
        
        QScopedPointer<T_CaInstaller> installer;
        QT_TRAP_THROWING(installer.reset(T_CaInstaller::NewL()));
        
        installer->InstallL( KAppTestFile );

        QTest::qWait(30000);
        
        QList< QSharedPointer<CaEntry> > items = service->getEntries(query);
        
        int itemsCountAndOne = items.count();

        for (int i=0; i< items.count(); i++) {
            QString tmp = items[i]->text();
            if (items[i]->text() == "Contacts") {
                entryPhonebook=items[i].data();
                break;
            }
        }

        for (int i=0; i< itemsCountAndOne; i++) {
            if (items[i]->text() == "sapiapp") {
                entry=items[i].data();
                break;
            }
        }
        //entry attribute will be removed by appscanner
        //after "sapiapp" entry is removed
        QVERIFY2(entry, "Sapi app has not installed");
        entryPhonebook->setAttribute("mmc_id","0x12345678");
        service->updateEntry(*entryPhonebook);
        bool result = false;
        if (entry->id() > 0) {
            result = service->executeCommand(*entry, "remove") == 0;
        }
        QVERIFY2(result, "Remove command returns false");
        QTest::qWait(40000);
        
        QList< QSharedPointer<CaEntry> > itemsNew = service->getEntries(query);
        int itemsCount = itemsNew.count();

        QVERIFY2(itemsCountAndOne == itemsCount +1 , "Remove unsuccessful");

    }
}


#endif //Q_OS_SYMBIAN
