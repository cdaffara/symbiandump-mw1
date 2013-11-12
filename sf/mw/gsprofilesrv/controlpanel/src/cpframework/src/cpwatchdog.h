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

#ifndef CPWATCHDOG_H
#define CPWATCHDOG_H

#include <QObject>
#include <QVector>

#define gWatchDog CpWatchDog::instance()

#define BEGIN_WATCHDOG_QUARANTINE(PLUGINID) \
        gWatchDog->quarantine(PLUGINID);

#define END_WATCHDOG_QUARANTINE(PLUGINID) \
        gWatchDog->removeFromQuarantine(PLUGINID);

class QSettings;
class CpWatchDog : public QObject
{
    Q_OBJECT
public:
    static CpWatchDog *instance();
    void quarantine(int uid);
    void removeFromQuarantine(int uid);
    bool isActive() const;
    bool wasCleanExit() const;
    void reportCleanExit();
    bool isInBlackList(int uid);
private:
    void init();
    void readState();
    void storeState();
    void readQuarantine();
    void storeQuarantine();
    void readBlackList();
    void storeBlackList();
    void updateBlackListedPluginRunCounters();
public slots:
    void destroy();
private:
    Q_DISABLE_COPY(CpWatchDog)
    CpWatchDog();
    virtual ~CpWatchDog();
private:
    struct PluginQuarantine
    {
        PluginQuarantine();
        void read(QSettings &settings);
        void store(QSettings &settings);
        int mUid;
        int mRunsAfterCrash;
    };
private:
    bool mActive;
    int mCrashCounter;
    bool mAppRunning;
    int mMaxPluginBlackListedRuns;
    int mWatchdogActivationLimit;
    QVector<PluginQuarantine> mBlackList;
    QVector<int> mQuarantine;
    static CpWatchDog *self;
};

#endif /* CPWATCHDOG_H */
