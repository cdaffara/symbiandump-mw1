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
#include "t_afactivitystorage.h"

#include <QtTest/QtTest>

#include "afactivitystorage_p.h"

const char DefaultActivityName [] = "DefaultTestActivityName";
const char DefaultActivityPrivate [] = "DefaultTestActivityPrivateData";
const char DefaultActivityPublic [] = "DefaultTestActivityPublicData";
const char DefaultActivityPublicKey [] = "FakeData";

void TestAfActivityStorage::init()
{
    mServiceProvider = QSharedPointer<AfStorageProxy>(new AfStorageProxy());
    mStorage = new AfActivityStoragePrivate(mServiceProvider);
}

void TestAfActivityStorage::cleanup()
{
    delete mStorage;
    mStorage = NULL;
}

void TestAfActivityStorage::testSaveActivity()
{
    const int applicationId(RProcess().SecureId().iId);
    QString activityId(DefaultActivityName);
    QVariant privateData(DefaultActivityPrivate);
    QVariantHash publicData;
    publicData.insert(DefaultActivityPublicKey, QVariant(DefaultActivityPublic));

    mStorage->saveActivity(activityId, privateData, publicData);
    
    QVERIFY(applicationId == mServiceProvider->mLastCallParams.value("applicationId").toInt());
    QVERIFY(activityId == mServiceProvider->mLastCallParams.value("activityId").toString());
    QVERIFY(privateData == mServiceProvider->mLastCallParams.value("activityData"));
    QVERIFY(publicData.count() <= mServiceProvider->mLastCallParams.value("metadata").toHash().count());
}

void TestAfActivityStorage::testRemoveActivity()
{
    const int applicationId(RProcess().SecureId().iId);
    QString activityId(DefaultActivityName);
    
    mStorage->removeActivity(activityId);
    
    QVERIFY(applicationId == mServiceProvider->mLastCallParams.value("applicationId").toInt());
    QVERIFY(activityId == mServiceProvider->mLastCallParams.value("activityId").toString());
}

void TestAfActivityStorage::testAllActivities()
{
    const int applicationId(RProcess().SecureId().iId);
    
    mStorage->allActivities();
    
    QVERIFY(applicationId == mServiceProvider->mLastCallParams.value("applicationId").toInt());
}

void TestAfActivityStorage::testActivityData()
{
    const int applicationId(RProcess().SecureId().iId);
    QString activityId(DefaultActivityName);
    
    mStorage->activityData(activityId);
    
    QVERIFY(applicationId == mServiceProvider->mLastCallParams.value("applicationId").toInt());
    QVERIFY(activityId == mServiceProvider->mLastCallParams.value("activityId").toString());
}

void TestAfActivityStorage::testActivityMetaData()
{
    const int applicationId(RProcess().SecureId().iId);
    QString activityId(DefaultActivityName);
    
    mStorage->activityMetaData(activityId);
    
    QVERIFY(applicationId == mServiceProvider->mLastCallParams.value("applicationId").toInt());
    QVERIFY(activityId == mServiceProvider->mLastCallParams.value("activityId").toString());
}

//to ensure coverage for AfActivityStorage public implementation
void TestAfActivityStorage::testActivityMetaDataPublicClient()
{
	AfActivityStorage *storage = new AfActivityStorage();
	storage->activityMetaData("testId");
	delete storage;
}

QTEST_MAIN(TestAfActivityStorage)
