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
#include "cpwatchdog.h"
#include <QCoreApplication>
#include <QSettings>
#include <QDebug>
#include "cplogger.h"

//const value definition
const int DefaultMaxPluginBlackListedRuns = 5;
const int DefaultWatchdogActivationLimit  = 1;
const int WatchdogActivationDisabled      = -1;

//Key definition for QSettings
const QString KeyWatchDogStatus                   = "WatchDog";
const QString KeyWatchDogCpRunning                = "cprunning";
const QString KeyWatchDogCrashCounter             = "crashcounter";
const QString KeyWatchDogMaxPluginBlackListedRuns = "maxpluginblacklistedruns";
const QString KeyWatchDogActivationLimit          = "activationlimit";
const QString KeyWatchDogQuarantine               = "Quarantine";
const QString KeyWatchDogQuarantineUid            = "uid";
const QString KeyWatchDogBlackList                = "BlackList";
const QString KeyWatchDogBlackListUid             = "uid";
const QString KeyWatchDogBlackListRunsAfterCrash  = "runsaftercrash";

CpWatchDog *CpWatchDog::self = 0;

CpWatchDog::PluginQuarantine::PluginQuarantine()
: mUid(0), mRunsAfterCrash(0)
{
}

void CpWatchDog::PluginQuarantine::read(QSettings &settings)
{
    mUid = settings.value(KeyWatchDogBlackListUid,0).toInt();
    mRunsAfterCrash = settings.value(KeyWatchDogBlackListRunsAfterCrash,0).toInt();
}

void CpWatchDog::PluginQuarantine::store(QSettings &settings)
{
    settings.setValue(KeyWatchDogBlackListUid,mUid);
    settings.setValue(KeyWatchDogBlackListRunsAfterCrash,mRunsAfterCrash);
}

CpWatchDog *CpWatchDog::instance()
{
    if (!self) {
        self = new CpWatchDog;
    }
    return self;
}

CpWatchDog::CpWatchDog()
    : mActive(true),
      mCrashCounter(0),
      mAppRunning(true),
      mMaxPluginBlackListedRuns(DefaultMaxPluginBlackListedRuns),
      mWatchdogActivationLimit(DefaultWatchdogActivationLimit)
{
    //delete watch dog when application is about to quit.
    connect(qApp,SIGNAL(aboutToQuit()),this,SLOT(destroy()));
    init();
}

CpWatchDog::~CpWatchDog()
{
    reportCleanExit();
    updateBlackListedPluginRunCounters();
    storeState();
}

void CpWatchDog::destroy()
{
    delete this;
    self = 0;
}

void CpWatchDog::quarantine(int uid)
{
    if (isActive()) {
        mQuarantine.append(uid);
        storeQuarantine();
    }
}
void CpWatchDog::removeFromQuarantine(int uid)
{
    if (isActive()) {
        int index = mQuarantine.indexOf(uid);
        if (index >= 0) {
            mQuarantine.remove(index);
            storeQuarantine();
        }
    }
}

bool CpWatchDog::isActive() const
{
    return mActive;
}

bool CpWatchDog::wasCleanExit() const
{
    return !mAppRunning;
}

void CpWatchDog::reportCleanExit()
{
    mAppRunning = false;
    
    QSettings settings;
    settings.beginGroup(KeyWatchDogStatus);
    settings.setValue(KeyWatchDogCpRunning,mAppRunning);
    settings.endGroup();
}

bool CpWatchDog::isInBlackList(int uid)
{
    bool found = false;
    if (isActive()) {
        foreach(const PluginQuarantine &quarantine,mBlackList) {
            if (quarantine.mUid == uid) {
                found = true;
                break;
            }
        }
    }
    
    if (found) {
        CPFW_LOG( QLatin1String("Plugin ") + QString("0x%1").arg(uid,0,16) + QLatin1String(" is in black list."));
    }
    
    return found;
}

void CpWatchDog::init()
{
    readState();
    
    if (!wasCleanExit() 
        && mWatchdogActivationLimit != WatchdogActivationDisabled) {
        mCrashCounter++;
        mActive = true;
        
        for (int i(0); i < mQuarantine.size(); i++) {
            if (!isInBlackList(mQuarantine[i])) {
                PluginQuarantine quarantine;
                quarantine.mUid = mQuarantine[i];
                quarantine.mRunsAfterCrash = 0;
                mBlackList.append(quarantine);
                mQuarantine.remove(i);
                i--;
            }
        }
    }
    else {
        mActive = false;
        mCrashCounter = 0;
        mQuarantine.clear();
    }
    
    mAppRunning = true;
    storeState();
}

void CpWatchDog::readState()
{
    QSettings settings;
    settings.beginGroup(KeyWatchDogStatus);
    int appRunning = settings.value(KeyWatchDogCpRunning,0).toInt();
    mAppRunning = (appRunning != 0);
    mCrashCounter = settings.value(KeyWatchDogCrashCounter,0).toInt();
    mMaxPluginBlackListedRuns = settings.value(
            KeyWatchDogMaxPluginBlackListedRuns,DefaultMaxPluginBlackListedRuns).toInt();
    mWatchdogActivationLimit = settings.value(
            KeyWatchDogActivationLimit,DefaultWatchdogActivationLimit).toInt();
    settings.endGroup();
    
    readQuarantine();
    readBlackList();
}

void CpWatchDog::storeState()
{
    {
        QSettings settings;
        settings.beginGroup(KeyWatchDogStatus);
        settings.setValue(KeyWatchDogCpRunning, mAppRunning ? 1 : 0);  
        settings.setValue(KeyWatchDogCrashCounter,mCrashCounter);   
        settings.setValue(KeyWatchDogMaxPluginBlackListedRuns,mMaxPluginBlackListedRuns);
        settings.setValue(KeyWatchDogActivationLimit,mWatchdogActivationLimit);
        settings.endGroup();
    } //Destructor of QSettings: writes any unsaved changes to permanent storage
    
    storeQuarantine();
    storeBlackList();
}

void CpWatchDog::readQuarantine()
{
    QSettings settings;
    int size = settings.beginReadArray(KeyWatchDogQuarantine);
    for (int i(0); i < size; i++) {
        settings.setArrayIndex(i);
        int uid = settings.value(KeyWatchDogQuarantineUid,0).toInt();
        if (uid != 0) {
            mQuarantine.append(uid);
        }
    }
    settings.endArray();
}

void CpWatchDog::storeQuarantine()
{
    QSettings settings;
    
    //remove old array data before updating
    settings.beginGroup(KeyWatchDogQuarantine);
    settings.remove(QString());
    settings.endGroup();
    
    settings.beginWriteArray(KeyWatchDogQuarantine);
    int size = mQuarantine.size();
    for (int i(0); i < size; i++) {
        settings.setArrayIndex(i);
        settings.setValue(KeyWatchDogQuarantineUid,mQuarantine[i]);
    }
    settings.endArray();
}

void CpWatchDog::readBlackList()
{
    QSettings settings;
    int size = settings.beginReadArray(KeyWatchDogBlackList);
    for (int i(0); i < size; i++) {
        settings.setArrayIndex(i);
        PluginQuarantine quarantine;
        quarantine.read(settings);
        if (quarantine.mUid != 0) {
            mBlackList.append(quarantine);
        }
    }
    settings.endArray();
}

void CpWatchDog::storeBlackList()
{
    QSettings settings;

    //remove old array data before updating
    settings.beginGroup(KeyWatchDogBlackList);
    settings.remove(QString());
    settings.endGroup();
    
    settings.beginWriteArray(KeyWatchDogBlackList);    
    int size = mBlackList.size();
    for (int i(0); i < size; i++) {
        settings.setArrayIndex(i);
        mBlackList[i].store(settings);
    }
    settings.endArray();
}

void CpWatchDog::updateBlackListedPluginRunCounters()
{
    for (int i(0); i < mBlackList.size(); i++) {
        mBlackList[i].mRunsAfterCrash++;
        if (mBlackList[i].mRunsAfterCrash > mMaxPluginBlackListedRuns) {
            mBlackList.remove(i);
            i--;
        }
    }
}
