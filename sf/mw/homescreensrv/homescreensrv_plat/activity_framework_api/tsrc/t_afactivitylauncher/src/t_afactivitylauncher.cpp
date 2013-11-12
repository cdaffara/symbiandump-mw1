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
* Description:
*
*/
#include "t_afactivitylauncher.h"

#include <QtTest/QtTest>
#include "afapplicationlauncher.h"
#include <afstorageclient.h>
#include "afactivitylauncher.h"
#include "afactivitylauncher_p.h"


void TestAfActivityLauncher::initTestCase()
{
    QCOMPARE(mApaLsSession.Connect(), KErrNone);
    QCOMPARE(mWsSession.Connect(), KErrNone);
}

void TestAfActivityLauncher::cleanupTestCase()
{
    mApaLsSession.Close();
    mWsSession.Close();
}

void TestAfActivityLauncher::init()
{    
    QT_TRAP_THROWING(mLauncher = CAfActivityLauncher::NewL(mApaLsSession, mWsSession));
}

void TestAfActivityLauncher::cleanup()
{
    delete TAfApplicationLauncher::lastStartUri;
    TAfApplicationLauncher::lastStartUri = 0;
    TAfApplicationLauncher::lastStartAppId = 0;
    
    delete CAfStorageClient::lastLaunchUri;
    CAfStorageClient::lastLaunchUri = 0;
    CAfStorageClient::lastLaunchAppId = 0;

    delete mLauncher;
    mLauncher = 0;
}

void TestAfActivityLauncher::testScheme()
{
    TAfApplicationLauncher::expectedIsRunningResult = ETrue;

    TInt error;
    TRAP(error, mLauncher->launchActivityL(_L("appto://DEADBEEF")));
    QCOMPARE(error, KErrNone);
    
    TRAP(error, mLauncher->launchActivityL(_L("bzzzt://DEADBEEF")));
    QCOMPARE(error, KErrArgument);
}

void TestAfActivityLauncher::testUidParsing()
{
    TAfApplicationLauncher::expectedIsRunningResult = ETrue;

    TInt error;
    TRAP(error, mLauncher->launchActivityL(_L("appto://DEADBEEF")));
    QCOMPARE(error, KErrNone);
    
    TRAP(error, mLauncher->launchActivityL(_L("appto://INVALID")));
    QCOMPARE(error, KErrGeneral);
}

void TestAfActivityLauncher::testRunningCase()
{
    TAfApplicationLauncher::expectedIsRunningResult = ETrue;

    _LIT(KActivityUri, "appto://DEADBEEF");
    TRAPD(error, mLauncher->launchActivityL(KActivityUri));
    QCOMPARE(error, KErrNone);
    
    QCOMPARE(CAfStorageClient::lastLaunchAppId, TInt(0xDEADBEEF));
    QVERIFY(CAfStorageClient::lastLaunchUri->Compare(KActivityUri()) == 0);
}

void TestAfActivityLauncher::testStartupCase()
{
    TAfApplicationLauncher::expectedIsRunningResult = EFalse;

    _LIT(KActivityUri, "appto://DEADBEEF");
    TRAPD(error, mLauncher->launchActivityL(KActivityUri));
    QCOMPARE(error, KErrNone);
    
    QCOMPARE(TAfApplicationLauncher::lastStartAppId, TInt(0xDEADBEEF));
    QVERIFY(TAfApplicationLauncher::lastStartUri->Compare(KActivityUri()) == 0);
}

void TestAfActivityLauncher::callUnusedCallbacks()
{
    CAfActivityLauncherPrivate* imp = CAfActivityLauncherPrivate::NewL(mApaLsSession, mWsSession);
    imp->waitActivityRequestCompleted(0, KNullDesC8());
    imp->getThumbnailRequestCompleted(0, 0, 0);
    imp->dataChangeNotificationCompleted(0);
    delete imp;
}

QTEST_MAIN(TestAfActivityLauncher)
