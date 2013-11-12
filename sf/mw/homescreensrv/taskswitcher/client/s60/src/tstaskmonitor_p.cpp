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
#include "tstaskmonitor_p.h"
#include "tstaskmonitor.h"

#include <eikenv.h>
#include <fbs.h>
#include <s32mem.h>

#include <XQConversions>

#include "tstaskmonitorclient.h"
#include "tstask.h"
#include "tstaskcontent.h"
#include "tstaskchangeinfo.h"
#include "tsutils.h"
#include "tstaskmonitorhistory.h"
using TaskSwitcher::CleanupResetAndDestroyPushL;

TsTaskMonitorPrivate::TsTaskMonitorPrivate(TsTaskMonitor *q) : q_ptr(q), mClient(0), mWsSession(CEikonEnv::Static()->WsSession())
{
    QT_TRAP_THROWING(mClient = CTsTaskMonitorClient::NewL());
    mClient->Subscribe(*this);
}

TsTaskMonitorPrivate::~TsTaskMonitorPrivate()
{
    mClient->CancelSubscribe();
    delete mClient;
}


QList<TsTaskChange> TsTaskMonitorPrivate::changeList(bool fullList)
{
    QList<TsTaskChange> retVal;
     
    QT_TRAP_THROWING (

        HBufC8 *data = mClient->TasksContentLC();
        if (data->Size() == 0) {
            CleanupStack::PopAndDestroy(data);
            return retVal;
        }
        TPtr8 dataPointer(data->Des());
        RDesReadStream dataStream(dataPointer);
        CleanupClosePushL(dataStream);

        int count = dataStream.ReadInt32L();

        QT_TRYCATCH_LEAVING(
            QList< QSharedPointer<TsTaskContent> > taskList;
            QList<TsTaskMonitorHistory> newTaskHistory;
            for (int iter(0); iter < count; iter++) {
                QSharedPointer<TsTaskContent> content(new TsTaskContent);
                TsTaskMonitorHistory newHistoryItem = internalizeContentL(dataStream, content);
                newHistoryItem.setOffset(iter);
                newTaskHistory.append(newHistoryItem);
                taskList.append(content);
            }
            if (fullList) {
                mTaskHistory.clear();
            }
            QList<int> insertsList = findInserts(newTaskHistory);
            QList<int> deletesList = findDeletes(newTaskHistory);
            QList<TsTaskMonitorHistory> newMinusInserts;
            if (insertsList.count() > 0 ) {
                newMinusInserts = substractInsertsFromNew(insertsList, newTaskHistory);
            } else {
                newMinusInserts = newTaskHistory;
            }
            QList<TsTaskMonitorHistory> interimList;
            if (deletesList.count() > 0 ) {
                retVal.append(getDeletesChangeset(deletesList));
                interimList = substractDeletesFromOld(deletesList);
            } else {
                interimList = mTaskHistory;
            }
            retVal.append(findMovesAndUpdates(newMinusInserts, taskList, interimList));
            if (mTaskHistory.isEmpty()) {
                retVal.append(TsTaskChange(TsTaskChangeInfo(), QSharedPointer<TsTask>()));
            }
            retVal.append(getInsertsChangeset(insertsList, taskList));
            mTaskHistory = newTaskHistory;
        )//QT_TRYCATCH_LEAVING
        CleanupStack::PopAndDestroy(&dataStream);
        CleanupStack::PopAndDestroy(data);
    );//QT_TRAP_THROWING
       
    return retVal;
}

TsTaskMonitorHistory TsTaskMonitorPrivate::internalizeContentL(RDesReadStream &  dataStream,
                                    QSharedPointer<TsTaskContent> &content)
{
    // get name
    TInt nameLength(dataStream.ReadInt32L());
    if (0 < nameLength) {
        HBufC* name = HBufC::NewLC(dataStream, nameLength);
        content->mName = XQConversions::s60DescToQString(*name);
        CleanupStack::PopAndDestroy(name);
    }
    TPckgBuf<TTime> updateTime;
    dataStream.ReadL(updateTime);
    QDateTime historyTime =  dateTimeFromS60(updateTime());
    
    // get screenshot
    TInt screenshotHandle = dataStream.ReadInt32L();
    CFbsBitmap *screenshot = new (ELeave) CFbsBitmap;
    CleanupStack::PushL(screenshot);
    if (KErrNone == screenshot->Duplicate(screenshotHandle)) {
        content->mScreenshot = QPixmap::fromSymbianCFbsBitmap(screenshot);
    }
    CleanupStack::PopAndDestroy(screenshot);
    // get key
    TInt keyLength(dataStream.ReadInt32L());
    if (0 < keyLength) {
        HBufC8* key = HBufC8::NewLC(keyLength);
        TPtr8 des(key->Des());
        dataStream.ReadL(des, keyLength);
        content->mKey = XQConversions::s60Desc8ToQByteArray(*key);
        CleanupStack::PopAndDestroy(key);
    }
    // get other values
    content->mActive = dataStream.ReadInt32L();
    content->mClosable = dataStream.ReadInt32L();  
    return TsTaskMonitorHistory(content->mKey, historyTime);
}

QDateTime TsTaskMonitorPrivate::dateTimeFromS60(const TTime &s60Time)
{
    TTime posixEpoch(_L("19700000:"));
    TTimeIntervalSeconds secondsFrom;
    TTimeIntervalMicroSeconds microSecondsFrom;
    s60Time.SecondsFrom(posixEpoch, secondsFrom);
    microSecondsFrom = s60Time.MicroSecondsFrom(posixEpoch);
    QDateTime retVal = QDateTime::fromTime_t(secondsFrom.Int());
    retVal = retVal.addMSecs((microSecondsFrom.Int64() % TInt64(1000000) ) / TInt64(1000));
    return retVal;    
}

int TsTaskMonitorPrivate::findItemIndex(
                          const QList<TsTaskMonitorHistory> &historyList,
                          const TsTaskMonitorHistory &item )
{
    for (int iter(0); iter < historyList.count(); iter++) {
        if(historyList[iter].isEqual(item)) {
            return iter;
        }
    }
    return -1;
}

QList<int>  TsTaskMonitorPrivate::findInserts(
                                 const QList<TsTaskMonitorHistory> &newHistory)
{
    QList<int> retVal = QList<int>();
    for (int iter(0); iter < newHistory.count(); iter++) {
        if (findItemIndex(mTaskHistory, newHistory[iter]) == -1) {
            retVal.append(iter);
        }
    }
    return retVal;
}

QList<int> TsTaskMonitorPrivate::findDeletes(
                                 const QList<TsTaskMonitorHistory> &newHistory)
{
    QList<int> retVal = QList<int>();
    //iterate backwards to keep order during model operations
    for (int iter(mTaskHistory.count() -1); iter >= 0 ; iter--) {
        if (findItemIndex(newHistory, mTaskHistory[iter]) == -1) {
            retVal.append(iter);
        }
    }
    return retVal;    
}

QList<TsTaskChange> TsTaskMonitorPrivate::getDeletesChangeset(
                                                      const QList<int> &deleteList)
{
    QList<TsTaskChange> retVal;
    foreach (int index, deleteList) {
        TsTaskChangeInfo deleteChange(TsTaskChangeInfo::KInvalidOffset,
                                                                   index);
        retVal.append(TsTaskChange(deleteChange, QSharedPointer<TsTask>()));
    }
    return retVal;
}

QList<TsTaskChange> TsTaskMonitorPrivate::getInsertsChangeset(const QList<int> &insertList,
                                        const QList< QSharedPointer<TsTaskContent> > &taskList)
{
    QList<TsTaskChange> retVal;
    foreach (int index, insertList) {
        TsTaskChangeInfo insertChange(index,
                                 TsTaskChangeInfo::KInvalidOffset);
        retVal.append(TsTaskChange(
                            insertChange,
                            QSharedPointer<TsTask>(new TsTask(taskList[index], *this))));
    }
    return retVal;
}

QList<TsTaskMonitorHistory> TsTaskMonitorPrivate::substractInsertsFromNew(
                                      const QList<int> &insertList,
                                      const QList<TsTaskMonitorHistory> &newHistory )
{
    QList<TsTaskMonitorHistory> retVal(newHistory);
    for ( int iter(insertList.count() - 1); iter >= 0 ; iter--) {
        retVal.removeAt(insertList[iter]);
    }
    return retVal;
}

QList<TsTaskMonitorHistory> TsTaskMonitorPrivate::substractDeletesFromOld(
                                      const QList<int> &deleteList)
{
    QList<TsTaskMonitorHistory> retVal(mTaskHistory);  
    for (int iter(0); iter < deleteList.count() ; iter++) {
        retVal.removeAt(deleteList[iter]);
    }
    return retVal;
}

QList<TsTaskChange> TsTaskMonitorPrivate::findMovesAndUpdates( 
                                     const QList<TsTaskMonitorHistory> &newMinusInserts,
                                     const QList< QSharedPointer<TsTaskContent> > &taskList,
                                     QList<TsTaskMonitorHistory> &workingList)
{
    QList<TsTaskChange> retVal;
    for (int newPos(0); newPos < newMinusInserts.count(); newPos++) {
        //case 1 item has moved
        int previousPos = findItemIndex(workingList, newMinusInserts[newPos]);
        if (newPos != previousPos) {
            TsTaskChangeInfo moveChange(newPos, previousPos);
            retVal.append(
                TsTaskChange(moveChange, QSharedPointer<TsTask>()));
            workingList.move(previousPos, newPos);
        }
        // case 2 item has changed
        if (newMinusInserts[newPos].isUpdated(workingList[newPos])) {
            TsTaskChangeInfo updateChange(newPos, newPos);
            int taskListOffset = newMinusInserts[newPos].offset();
            retVal.append( TsTaskChange(
                              updateChange,
                              QSharedPointer<TsTask>(new TsTask(taskList[taskListOffset], *this)))); 
        }
    }
    return retVal;
}

void TsTaskMonitorPrivate::HandleRunningAppChange()
{
    emit q_ptr->taskListChanged();
}

void TsTaskMonitorPrivate::openTask(const QByteArray &key)
{
    TPtrC8 desC(reinterpret_cast<const TUint8*>(key.constData()), key.length());
    mClient->OpenTask(desC);
    
}

void TsTaskMonitorPrivate::closeTask(const QByteArray &key)
{
    TPtrC8 desC(reinterpret_cast<const TUint8*>(key.constData()), key.length());
    mClient->CloseTask(desC);
}
