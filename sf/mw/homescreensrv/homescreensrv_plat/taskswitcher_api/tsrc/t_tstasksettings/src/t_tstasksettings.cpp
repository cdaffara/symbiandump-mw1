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

#include "t_tstasksettings.h"

#include <s32mem.h>

#include <QtTest/QtTest>
#include <QDesktopWidget>
#include <QPainter>

#include <QDebug>
#include <XQConversions>

#if defined (Q_OS_SYMBIAN)
#include <coecntrl.h>
#include "tstaskmonitorclient.h"
#include "tsscreenshotclient.h"
#include "tsutils.h"
using TaskSwitcher::CleanupResetAndDestroyPushL;
#endif

#include "tstask.h"
#include "tstaskchangeinfo.h"
#include "tstestutils.h"

void T_TsTaskSettings::initTestCase()
{
    while (!unitTestTask().data())
        QVERIFY(TsTestUtils::waitForSignal(&mTaskMonitor, SIGNAL(taskListChanged())));
}

void T_TsTaskSettings::testRegisteringScreenshotTriggersTaskListChangeSignal()
{    
    QVERIFY(mTaskSettings.registerScreenshot(imageWithCircle()));
    QVERIFY(TsTestUtils::waitForSignal(&mTaskMonitor, SIGNAL(taskListChanged())));
}

void T_TsTaskSettings::testUnregisteringScreenshotTriggersTaskListChangeSignal()
{    
    QVERIFY(mTaskSettings.registerScreenshot(imageWithCircle()));
    QVERIFY(TsTestUtils::waitForSignal(&mTaskMonitor, SIGNAL(taskListChanged())));
    
    QVERIFY(mTaskSettings.unregisterScreenshot());
    QVERIFY(TsTestUtils::waitForSignal(&mTaskMonitor, SIGNAL(taskListChanged())));
}

void T_TsTaskSettings::testIconIsReturnedAfterUnregisteringScreenshot()
{    
    // get icon
    QSharedPointer<TsTask> taskBefore = unitTestTask();
    QVERIFY(taskBefore.data());
    QPixmap icon = taskBefore->screenshot();
    QVERIFY(!icon.isNull());
    
    // register and unregister screenshot
    QVERIFY(mTaskSettings.registerScreenshot(imageWithCircle()));
    QVERIFY(TsTestUtils::waitForSignal(&mTaskMonitor, SIGNAL(taskListChanged())));    
    QVERIFY(mTaskSettings.unregisterScreenshot());
    QVERIFY(TsTestUtils::waitForSignal(&mTaskMonitor, SIGNAL(taskListChanged())));
    
    // compare pixmap returned after register/unregister with icon
    QSharedPointer<TsTask> taskAfter = unitTestTask();
    QVERIFY(taskAfter.data());
    QVERIFY(taskAfter->screenshot().toImage() == icon.toImage());
}

void T_TsTaskSettings::testRegisteringEmptyPixmapReturnsFalse()
{
    QVERIFY(!mTaskSettings.registerScreenshot(QPixmap()));
}

void T_TsTaskSettings::testRegisteringTheSameScreenshotTwiceTriggersTaskListChangeSignal()
{    
    QPixmap image = imageWithCircle();
    
    QVERIFY(mTaskSettings.registerScreenshot(image));
    QVERIFY(TsTestUtils::waitForSignal(&mTaskMonitor, SIGNAL(taskListChanged())));    
    QVERIFY(mTaskSettings.registerScreenshot(image));
    QVERIFY(TsTestUtils::waitForSignal(&mTaskMonitor, SIGNAL(taskListChanged())));    
}

void T_TsTaskSettings::testRegisteringOverwritesOldScreenshot()
{
    QPixmap previousScreenshot;
    {
        QSharedPointer<TsTask> myTask = unitTestTask();
        QVERIFY(myTask.data());    
        previousScreenshot = myTask->screenshot();
    }
        
    QVERIFY(mTaskSettings.registerScreenshot(imageWithCircle()));
    QVERIFY(TsTestUtils::waitForSignal(&mTaskMonitor, SIGNAL(taskListChanged())));
    
    QPixmap screenshotAfterFirstRegistration;
    {
        QSharedPointer<TsTask> myTask = unitTestTask();
        QVERIFY(myTask.data());
        screenshotAfterFirstRegistration = myTask->screenshot();
    }
    
    QVERIFY(previousScreenshot.toImage() != screenshotAfterFirstRegistration.toImage());
    
    QVERIFY(mTaskSettings.registerScreenshot(imageWithCircle()));
    QVERIFY(TsTestUtils::waitForSignal(&mTaskMonitor, SIGNAL(taskListChanged())));
    
    QPixmap screenshotAfterSecondRegistration;
    {
        QSharedPointer<TsTask> myTask = unitTestTask();
        QVERIFY(myTask.data());
        screenshotAfterSecondRegistration = myTask->screenshot();
    }
    
    QVERIFY(screenshotAfterSecondRegistration.toImage() != screenshotAfterFirstRegistration.toImage());
}

void T_TsTaskSettings::testRegisteringFailsIfHigherPriorityScreenshotIsRegistered()
{
    QPixmap highPriorityScreenshot = imageWithCircle();
    registerHighPriorityScreenshot(highPriorityScreenshot);
    QVERIFY(TsTestUtils::waitForSignal(&mTaskMonitor, SIGNAL(taskListChanged())));
    
    QVERIFY(!mTaskSettings.registerScreenshot(imageWithCircle()));
    
    // clean up and verify the screenshots can be registered
    QVERIFY(mTaskSettings.unregisterScreenshot());
    QVERIFY(TsTestUtils::waitForSignal(&mTaskMonitor, SIGNAL(taskListChanged())));
    QVERIFY(mTaskSettings.registerScreenshot(imageWithCircle()));
    QVERIFY(TsTestUtils::waitForSignal(&mTaskMonitor, SIGNAL(taskListChanged())));
}

void T_TsTaskSettings::testChangingVisibilityOfApplication()
{
    {
        QSharedPointer<TsTask> myTask = unitTestTask();
        QVERIFY(myTask.data());
    }
    
    // hide unit test task
    mTaskSettings.setVisibility(false);
    QVERIFY(TsTestUtils::waitForSignal(&mTaskMonitor, SIGNAL(taskListChanged())));
    {
        QSharedPointer<TsTask> myTask = unitTestTask();
        QVERIFY(!myTask.data());
    }

    // show unit test task
    mTaskSettings.setVisibility(true);
    QVERIFY(TsTestUtils::waitForSignal(&mTaskMonitor, SIGNAL(taskListChanged())));
    {
        QSharedPointer<TsTask> myTask = unitTestTask();
        QVERIFY(myTask.data());
    }    
}

QSharedPointer<TsTask> T_TsTaskSettings::unitTestTask()
{
    // find unit test task
    QSharedPointer<TsTask> myTask;
    foreach(TsTaskChange taskChange, mTaskMonitor.changeList(true)) {
        if (!taskChange.second.isNull()) {
            if (taskChange.second->name() == "t_tstasksettings") {
                myTask = taskChange.second;
                break;
            }
        }
    }
    return myTask;
}

bool T_TsTaskSettings::registerHighPriorityScreenshot(const QPixmap &screenshot)
{
#if defined (Q_OS_SYMBIAN)
    int wgId = QApplication::desktop()->winId()->ControlEnv()->RootWin().WindowGroupId();
    
    QScopedPointer<CFbsBitmap> bitmap(screenshot.toSymbianCFbsBitmap());
    if (bitmap.isNull()) 
        return false;
        
    int error(KErrGeneral);
    
    QT_TRAP_THROWING (    
        CTsScreenshotClient* screenshotClient = CTsScreenshotClient::NewLC();            
        error = screenshotClient->RegisterScreenshot(bitmap.data(), wgId, High);
        CleanupStack::PopAndDestroy(screenshotClient);
    );
    
    return error == KErrNone;
#else
    return false;
#endif
}

QPixmap T_TsTaskSettings::imageWithCircle()
{
    static int hue = 0;
    QPixmap screenshot(10, 10);
    
    {
        QPainter painter(&screenshot);
        painter.setBrush(Qt::black);
        painter.drawRect(screenshot.rect());
        painter.setBrush(QColor::fromHsv(hue, 255, 255));
        painter.drawEllipse(screenshot.rect());
    }

    hue += 17;
    return screenshot;
}

QTEST_MAIN(T_TsTaskSettings)
