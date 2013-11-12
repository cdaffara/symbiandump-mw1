/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
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
#include "ut_profileengwrapper.h"
#include "cpprofileengine.h"
#include "cpprofile.h"
#include "cpprofiledef.h"
#include <QDebug>
#include <qstringlist>
#include <qttest/qttest>


void TestProfileEngWrapper::initTestCase()
{
    mProfileEng = new CpProfileEngine();
    QVERIFY(mProfileEng != 0);
}

void TestProfileEngWrapper::testProfileList()
{
    QStringList profileList = mProfileEng->profileList();
    
    QVERIFY(profileList.size() > 0);
    
    qDebug() << "read all profiles:\r\n";
    foreach(const QString &name,profileList) {
        qDebug() << name << "\t";
    }
    qDebug() << "\r\n";
}

void TestProfileEngWrapper::testModifyProfile()
{
    CpProfile *generalProfile = mProfileEng->profile(CpProfileGeneralId);
   
    int prevolume = generalProfile->ringingVolume(CpProfileKnownCaller);
    qDebug() << "before modifying volume:" << prevolume << "\r\n";
    
    int newvolume = prevolume + 1;
    generalProfile->setRingingVolume(CpProfileKnownCaller, newvolume);
    mProfileEng->saveProfile(generalProfile);
    
    qDebug() << "after modifying volume:" << generalProfile->ringingVolume(CpProfileKnownCaller) << "\r\n";

    generalProfile->setRingingVolume(CpProfileKnownCaller, prevolume);
    mProfileEng->saveProfile(generalProfile);
    
    QVERIFY(prevolume == generalProfile->ringingVolume(CpProfileKnownCaller));
}

void TestProfileEngWrapper::testCreateAndDeleteProfile()
{
    QStringList profileList = mProfileEng->profileList();
    qDebug() << "origion size:" << profileList.size() << "\r\n";
    
    //create a profile
    CpProfile *newProfile = mProfileEng->createProfile();
    mProfileEng->saveProfile(newProfile);
    
    QStringList newProfileList = mProfileEng->profileList();
    
    qDebug() << "size after creating new:" << newProfileList.size() << "\r\n";
    QVERIFY(profileList.size() + 1 == newProfileList.size());
    
    mProfileEng->deleteProfile(newProfile->id());
    newProfileList = mProfileEng->profileList();
    qDebug() << "size after deleting new:" << newProfileList.size() << "\r\n";
    QVERIFY(profileList.size() == newProfileList.size());
    
}

void TestProfileEngWrapper::cleanupTestCase()
{
    delete mProfileEng;
    mProfileEng = 0;
}

QTEST_MAIN(TestProfileEngWrapper)
