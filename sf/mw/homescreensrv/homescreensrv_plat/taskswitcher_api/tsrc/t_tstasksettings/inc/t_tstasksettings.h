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
#ifndef T_TSTASKSETTINGS_H
#define T_TSTASKSETTINGS_H

#include <QObject>
#include <QSharedPointer>
#include <QPixmap>

#include "tstaskmonitor.h"
#include "tstasksettings.h"

/**
* @test Test class for TsTaskSettings
*/
class T_TsTaskSettings : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testRegisteringScreenshotTriggersTaskListChangeSignal();
    void testUnregisteringScreenshotTriggersTaskListChangeSignal();
    void testIconIsReturnedAfterUnregisteringScreenshot();
    void testRegisteringEmptyPixmapReturnsFalse();
    void testRegisteringTheSameScreenshotTwiceTriggersTaskListChangeSignal();
    void testRegisteringOverwritesOldScreenshot();
    void testRegisteringFailsIfHigherPriorityScreenshotIsRegistered();
    
    void testChangingVisibilityOfApplication();
    
private:
    QSharedPointer<TsTask> unitTestTask();
    QPixmap imageWithCircle();
    bool registerHighPriorityScreenshot(const QPixmap &screenshot);
    
private:
    TsTaskSettings mTaskSettings;    
    TsTaskMonitor mTaskMonitor;   
    
};

#endif //T_TSTASKSETTINGS_H
