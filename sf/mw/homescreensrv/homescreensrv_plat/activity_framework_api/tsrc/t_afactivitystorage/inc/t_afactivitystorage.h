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
#ifndef TESTAFACTIVITYSTORAGE_H
#define TESTAFACTIVITYSTORAGE_H

#include <QObject>
#include <QSharedPointer>
#include "afstorageproxy.h"
#include "afactivitystorage.h"

class TestAfActivityStorage : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void testSaveActivity();
    void testRemoveActivity();
    void testAllActivities();
    void testActivityData();
    void testActivityMetaData();
    void testActivityMetaDataPublicClient();
    
private:
    QSharedPointer<AfStorageProxy> mServiceProvider;
    AfActivityStoragePrivate *mStorage;

};

#endif // TESTAFACTIVITYSTORAGE_H
