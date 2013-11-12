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
#ifndef TSTASKMONITOR_P_H
#define TSTASKMONITOR_P_H


#include <w32std.h>

#include <QList>
#include <QSharedPointer>
#include <QPair>
#include <QDateTime>

#include "tstaskmonitorobserver.h"
#include "tstasklauncher.h"
#include "tstaskmonitor.h"

class RDesReadStream;
class TsTask;
class TsTaskContent;
class CTsTaskMonitorClient;
class TsTaskMonitorHistory;

class TsTaskMonitorPrivate : public MTsTaskMonitorObserver, public TsTaskLauncher
{
public:
    TsTaskMonitorPrivate(TsTaskMonitor *q);
    virtual ~TsTaskMonitorPrivate();
    
public:
    QList<TsTaskChange> changeList(bool fullList = false);

public: // from MTsTaskMonitorObserver
    virtual void HandleRunningAppChange();

public: // from TsTaskLauncher
    virtual void openTask(const QByteArray &key);
    virtual void closeTask(const QByteArray &key);
    
private: // helper methods
    TsTaskMonitorHistory internalizeContentL(RDesReadStream &dataStream,
                             QSharedPointer<TsTaskContent> &content );
    QDateTime dateTimeFromS60(const TTime &s60Time);
    
    //for comparing algorithm
    int         findItemIndex(const QList<TsTaskMonitorHistory> &historyList,
                              const TsTaskMonitorHistory &item );
    QList<int>  findInserts(const QList<TsTaskMonitorHistory> &newHistory);
    QList<int>  findDeletes(const QList<TsTaskMonitorHistory> &newHistory);
    QList<TsTaskChange> getDeletesChangeset(const QList<int> &deleteList);
    QList<TsTaskChange> getInsertsChangeset(const QList<int> &insertList,
                                   const QList< QSharedPointer<TsTaskContent> > &taskList);
    QList<TsTaskMonitorHistory> substractInsertsFromNew(
                                          const QList<int> &insertList,
                                          const QList<TsTaskMonitorHistory> &newHistory );
    QList<TsTaskMonitorHistory> substractDeletesFromOld(
                                          const QList<int> &deleteList);
    QList<TsTaskChange> findMovesAndUpdates(
                                   const QList<TsTaskMonitorHistory> &newMinusInserts,
                                   const QList< QSharedPointer<TsTaskContent> > &taskList,
                                   QList<TsTaskMonitorHistory> &workingList);
    
private:    
    TsTaskMonitor *q_ptr;
    CTsTaskMonitorClient *mClient;
    RWsSession &mWsSession;
    QList<TsTaskMonitorHistory> mTaskHistory;
};

#endif //TSTASKMONITOR_P_H
