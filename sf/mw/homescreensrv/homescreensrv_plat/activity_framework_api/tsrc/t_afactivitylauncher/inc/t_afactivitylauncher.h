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
#ifndef T_AFACTIVITYLAUNCHER_H
#define T_AFACTIVITYLAUNCHER_H

#include <QObject>

#include <apgcli.h>
#include <w32std.h>

class CAfActivityLauncher;

class TestAfActivityLauncher : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    
    void init();
    void cleanup();

    void testScheme();
    void testUidParsing();
    void testRunningCase();
    void testStartupCase();
    
    void callUnusedCallbacks();

private:
    CAfActivityLauncher *mLauncher;
    
    RApaLsSession mApaLsSession;
    RWsSession mWsSession;
};

#endif // T_AFACTIVITYLAUNCHER_H
