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
#ifndef TESTAFACTIVATION_H
#define TESTAFACTIVATION_H

#include <QObject>
#include <QSharedPointer>

#include "afstorageproxy.h"
class AfActivation;

class TestAfActivation : public QObject
{
    Q_OBJECT

public:
    TestAfActivation();
    
private slots:
    //QTest specific slots
    void init();
    void cleanup();

    //test functions
    void invokeActivationPublicInterface();
    void invokeActivationPublicInterfaceCorrupt();
    void testDefaultValuesAfterCreation();
    void testSignalIsEmittedWhenActivityIsRequested();
    void testValuesChangeWhenActivityIsRequested();
    void testValuesChangeWhenActivityIsRequestedNoName();
    void testChangingActivityInBackground();

private:
    AfStorageProxy* mServiceProvider;
    AfActivation *mActivation;
    QString mDefaultActivityName;
};

#endif // TESTAFACTIVATION_H
