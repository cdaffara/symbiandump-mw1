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
#include "t_afactivation.h"

#include <QtTest/QtTest>

#include "afactivation.h"
#include "afactivation_p.h"
#include "applicationlauncher_p.h"

TestAfActivation::TestAfActivation()
{
    mDefaultActivityName = QString("appto://EBADC0DE?%1=TestActivity&TestParam=TestValue").arg(
        Af::KActivityUriNameKey);
    qRegisterMetaType<Af::ActivationReason> ("Af::ActivationReason");
}

void TestAfActivation::init()
{
    mActivation = new AfActivation();
    mServiceProvider = AfStorageProxy::instance;
}

void TestAfActivation::cleanup()
{
    delete mActivation;
    mActivation = NULL;
}

void TestAfActivation::invokeActivationPublicInterface()
{
    // AfActivation is only responsible of correct creation of AfActivationPrivate and
    // forwarding requests. Since it's impossible to verify the effects of the member
    // calls, just invoke the methods.
    AfActivation activation;

    activation.parameters();
    activation.reason();
    activation.name();
}

void TestAfActivation::invokeActivationPublicInterfaceCorrupt()
{
    bool errorReceived = false;
    //af storage proxy is mocked so I can set static variable making it leave
    AfStorageProxy::waitFailed = true;
    try
    {
        AfActivation *activation = new AfActivation();
    }
    catch(...)
    {
        errorReceived = true;
    }
    QVERIFY(errorReceived);
    }

    void TestAfActivation::testDefaultValuesAfterCreation()
    {
        QCOMPARE(mActivation->parameters(), QVariantHash());
        QCOMPARE(mActivation->reason(), Af::ActivationReasonNormal);
        QCOMPARE(mActivation->name(), QString());
    }

    void TestAfActivation::testSignalIsEmittedWhenActivityIsRequested()
    {
        QSignalSpy activatedSpy(mActivation,
            SIGNAL(activated(Af::ActivationReason,QString,QVariantHash)));
        QVERIFY(activatedSpy.isValid());

        mServiceProvider->emitActivityRequested(mDefaultActivityName);

        QCOMPARE(activatedSpy.count(), 1);
    }

    void TestAfActivation::testValuesChangeWhenActivityIsRequested()
    {
        QSignalSpy activatedSpy(mActivation,
            SIGNAL(activated(Af::ActivationReason,QString,QVariantHash)));
        QVERIFY(activatedSpy.isValid());

        mServiceProvider->emitActivityRequested(mDefaultActivityName);

        QCOMPARE(activatedSpy.count(), 1);
        QCOMPARE(mActivation->reason(), Af::ActivationReasonActivity);
        QCOMPARE(mActivation->name(), QString("TestActivity"));
        QVERIFY(mActivation->parameters().contains("TestParam"));
        QCOMPARE(mActivation->parameters().value("TestParam").toString(), QString("TestValue"));
    }

    void TestAfActivation::testValuesChangeWhenActivityIsRequestedNoName()
    {
        QString activityname("appto://EBADC0DE?TestParam=TestValue");
        QSignalSpy activatedSpy(mActivation,
            SIGNAL(activated(Af::ActivationReason,QString,QVariantHash)));
        QVERIFY(activatedSpy.isValid());

        mServiceProvider->emitActivityRequested(activityname);
        QCOMPARE(activatedSpy.count(), 1);

        QCOMPARE(mActivation->reason(), Af::ActivationReasonActivity);
        QCOMPARE(mActivation->name(), QString());
        QVERIFY(mActivation->parameters().contains("TestParam"));
        QCOMPARE(mActivation->parameters().value("TestParam").toString(), QString("TestValue"));
    }

    void TestAfActivation::testChangingActivityInBackground()
    {
        // bring to foreground is called by default
        mServiceProvider->emitActivityRequested(mDefaultActivityName);

        ApplicationLauncherPrivate *launcher = ApplicationLauncherPrivate::instance;
        QVERIFY(launcher);
        QCOMPARE(launcher->bringToForegroundCalls.count(), 1);
        launcher->bringToForegroundCalls.clear();

        // bring to foreground is not called if background parameter is present
        mServiceProvider->emitActivityRequested(
            QString("appto://EBADC0DE?%1=TestActivity&%2=").arg(Af::KActivityUriNameKey).arg(
                Af::KActivityUriBackgroundKey));

        launcher = ApplicationLauncherPrivate::instance;
        QVERIFY(launcher);
        QCOMPARE(launcher->bringToForegroundCalls.count(), 0);
    }

    QTEST_MAIN(TestAfActivation)
