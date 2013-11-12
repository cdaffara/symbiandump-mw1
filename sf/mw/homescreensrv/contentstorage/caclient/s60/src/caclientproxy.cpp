/*
 * Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32std.h>
#include <QList>
#include <QDebug>
#include <utf.h>  // charconv.lib
#include <QMutexLocker>

#include "caclientproxy.h"
#include "cainnerentry.h"
#include "cainnerquery.h"
#include "caobjectadapter.h"
#include "caentriesarray.h"
#include "caidsarray.h"
#include "caarraycleanup.inl"

#include "caentry.h"
#include "caquery.h"
#include "cadefs.h"
#include "cahandlerproxy.h"
#include "caqtsfhandlerloader.h"

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
CaClientProxy::CaClientProxy()
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
CaClientProxy::~CaClientProxy()
{
    mSession.Close();

}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ErrorCode CaClientProxy::connect()
{
    int error = mSession.StartServer();
    if (!error) {
        error = mSession.Connect();
    }
    if (error) {
        mSession.Close();
        qWarning("CaClientProxy::connect: error (%d)", error);
    }
    return CaObjectAdapter::convertErrorCode(error);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ErrorCode CaClientProxy::addData(const CaEntry &entryToAdd,
                                 CaEntry &targetEntry)
{
    TRAPD(error, addDataL(entryToAdd, targetEntry));

    USE_QDEBUG_IF(error) << "CaClientProxy::addData - Error ("
                         <<  error << ")";

    return CaObjectAdapter::convertErrorCode(error);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ErrorCode CaClientProxy::removeData(const QList<int> &entryIdList)
{
    TRAPD(error, removeDataL(entryIdList));

    USE_QDEBUG_IF(error) << "CaClientProxy::removeData - Error ("
                         << error << ")";

    return CaObjectAdapter::convertErrorCode(error);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ErrorCode CaClientProxy::insertEntriesIntoGroup(int groupId,
        const QList<int> &entryIdList, int beforeEntryId)
{
    TRAPD(error,
          insertEntriesIntoGroupL(groupId, entryIdList, beforeEntryId));

    USE_QDEBUG_IF(error)<< "CaClientProxy::insertEntriesIntoGroup - Error ("
                        << error << ")";

    return CaObjectAdapter::convertErrorCode(error);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ErrorCode CaClientProxy::removeEntriesFromGroup(int groupId,
        const QList<int> &entryIdList)
{
    TRAPD(error, removeEntriesFromGroupL(groupId, entryIdList));

    USE_QDEBUG_IF(error)<< "CaClientProxy::removeEntriesFromGroup - Error ("
                        << error << ")";

    return CaObjectAdapter::convertErrorCode(error);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ErrorCode CaClientProxy::getData(const QList<int> &entryIdList,
        QList< QSharedPointer<CaEntry> > &sourceList)
{
    TRAPD(error, getDataL(entryIdList, sourceList));

    USE_QDEBUG_IF(error) << "CaClientProxy::getData - Error ("
                         << error << ")";

    return CaObjectAdapter::convertErrorCode(error);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ErrorCode CaClientProxy::getData(const CaQuery &query,
        QList< QSharedPointer<CaEntry> > &sourceList)
{
    TRAPD(error, getDataL(query, sourceList));

    USE_QDEBUG_IF(error) << "CaClientProxy::getData - Error ("
                         << error << ")";

    return CaObjectAdapter::convertErrorCode(error);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ErrorCode CaClientProxy::getEntryIds(const CaQuery &query,
                                     QList<int> &sourceIdList)
{
    TRAPD(error, getEntryIdsL(query, sourceIdList));

    USE_QDEBUG_IF(error) << "CaClientProxy::getEntryIds - Error ("
                         << error << ")";

    return CaObjectAdapter::convertErrorCode(error);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ErrorCode CaClientProxy::touch(const CaEntry &aEntry)
{
    TRAPD(error, touchL(aEntry));

    USE_QDEBUG_IF(error) << "CaClientProxy::touch - Error ("
                         << error << ")";

    return CaObjectAdapter::convertErrorCode(error);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ErrorCode CaClientProxy::customSort(const QList<int> &entryIdList,
                                    int groupId)
{
    TRAPD(error, customSortL(entryIdList, groupId));

    USE_QDEBUG_IF(error)<< "CaClientProxy::customSort - Error ("
                        << error << ")";

    return CaObjectAdapter::convertErrorCode(error);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaClientProxy::addDataL(const CaEntry &entryToAdd,
                             CaEntry &targetEntry)
{
    CCaInnerEntry *newEntryToAdd = CCaInnerEntry::NewLC();

    CaObjectAdapter::convertL(entryToAdd, *newEntryToAdd);
    mSession.AddL(*newEntryToAdd);
    CaObjectAdapter::convert(*newEntryToAdd, targetEntry);

    CleanupStack::PopAndDestroy(newEntryToAdd);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaClientProxy::removeDataL(const QList<int> &entryIdList)
{
    RCaIdsArray array;
    CleanupClosePushL(array);
    CaObjectAdapter::convertL(entryIdList, array);
    mSession.RemoveL(array);
    CleanupStack::PopAndDestroy(&array);
}

void CaClientProxy::insertEntriesIntoGroupL(int groupId,
        const QList<int> &entryIdList, int beforeEntryId)
{
    RCaIdsArray array;
    CleanupClosePushL(array);
    CaObjectAdapter::convertL(entryIdList, array);
    TCaOperationParams::OperationType operationType;
    switch (beforeEntryId) {
    case AfterTheLastEntry:
        operationType = TCaOperationParams::EAppend;
        break;
    case BeforeTheFirstEntry:
        operationType = TCaOperationParams::EPrepend;
        break;
    default:
        if (beforeEntryId < 1) {
            qWarning("CaClientProxy::insertEntriesIntoGroupL"
                     "- wrong beforeEntryId: (%d)", beforeEntryId);
        }
        operationType = TCaOperationParams::EInsert;
        break;
    }
    TCaOperationParams params = { operationType, groupId, beforeEntryId };
    mSession.OrganizeL(array, params);
    CleanupStack::PopAndDestroy(&array);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaClientProxy::removeEntriesFromGroupL(int groupId,
        const QList<int> &entryIdList)
{
    RCaIdsArray array;
    CleanupClosePushL(array);
    CaObjectAdapter::convertL(entryIdList, array);
    TCaOperationParams params = { TCaOperationParams::ERemove,
                                  groupId, 0 /*not used*/
                                };
    mSession.OrganizeL(array, params);
    CleanupStack::PopAndDestroy(&array);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaClientProxy::getDataL(const QList<int> &entryIdList,
        QList< QSharedPointer<CaEntry> > &sourceList)
{
    CCaInnerQuery *query = CCaInnerQuery::NewLC();
    RArray<TInt> array;
    CleanupClosePushL(array);
    CaObjectAdapter::convertL(entryIdList, array);
    query->SetIdsL(array);
    RCaEntriesArray results;
    CleanupResetAndDestroyPushL(results);
    mSession.GetListL(*query, results);
    CaObjectAdapter::convertL(results, sourceList);
    CleanupStack::PopAndDestroy(&results);
    CleanupStack::PopAndDestroy(&array);
    CleanupStack::PopAndDestroy(query);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaClientProxy::getDataL(const CaQuery &query,
        QList< QSharedPointer<CaEntry> > &sourceList)
{
    CCaInnerQuery *innerQuery = CCaInnerQuery::NewLC();
    CaObjectAdapter::convertL(query, *innerQuery);
    RCaEntriesArray results;
    CleanupResetAndDestroyPushL(results);
    mSession.GetListL(*innerQuery, results);
    CaObjectAdapter::convertL(results, sourceList);
    CleanupStack::PopAndDestroy(&results);
    CleanupStack::PopAndDestroy(innerQuery);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaClientProxy::getEntryIdsL(const CaQuery &query,
                                 QList<int> &sourceIdList)
{
    CCaInnerQuery *innerQuery = CCaInnerQuery::NewLC();
    CaObjectAdapter::convertL(query, *innerQuery);
    RCaIdsArray results;
    CleanupClosePushL(results);
    mSession.GetEntryIdsL(*innerQuery, results);
    CaObjectAdapter::convertL(results, sourceIdList);
    CleanupStack::PopAndDestroy(&results);
    CleanupStack::PopAndDestroy(innerQuery);
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaClientProxy::touchL(const CaEntry &entry)
{
    CCaInnerEntry *innerEntry = CCaInnerEntry::NewLC();

    CaObjectAdapter::convertL(entry, *innerEntry);

    mSession.TouchL(*innerEntry);

    CleanupStack::PopAndDestroy(innerEntry);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaClientProxy::customSortL(const QList<int> &entryIdList, int groupId)
{
    RCaIdsArray array;
    CleanupClosePushL(array);
    CaObjectAdapter::convertL(entryIdList, array);

    mSession.CustomSortL(array, groupId);
    CleanupStack::PopAndDestroy(&array);
}

