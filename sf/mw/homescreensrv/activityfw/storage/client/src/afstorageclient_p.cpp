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
#include "afstorageclient_p.h"

#include <fbs.h>
#include <s32mem.h>

#include "afasyncrequest_p.h"
#include "afstorageglobals.h"
#include "afentry.h"

// -----------------------------------------------------------------------------
/**
 * Two-phase constructor. Create and initialize instance
 * @return entry instance
 */
CAfStorageClientPrivate *CAfStorageClientPrivate::NewL(MAfAsyncRequestObserver &observer)
{
    CAfStorageClientPrivate *self = CAfStorageClientPrivate::NewLC(observer);
    CleanupStack::Pop(self);
    return self;
}

// -----------------------------------------------------------------------------
/**
 * Two-phase constructor. Create, initialize and push instance into cleanup stack
 * @return entry instance
 */
CAfStorageClientPrivate *CAfStorageClientPrivate::NewLC(MAfAsyncRequestObserver &observer)
{
    CAfStorageClientPrivate *self = new (ELeave) CAfStorageClientPrivate(observer);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}    

// -----------------------------------------------------------------------------
/**
 * Destructor
 */
CAfStorageClientPrivate::~CAfStorageClientPrivate()
{
    mImplementation.Close();
}
    
// -----------------------------------------------------------------------------
/**
 * Constructor
 */
CAfStorageClientPrivate::CAfStorageClientPrivate(MAfAsyncRequestObserver &observer) : mImplementation(observer)
{
}

// -----------------------------------------------------------------------------
/**
 * 2nd phase of construction
 */
void CAfStorageClientPrivate::ConstructL()
{
    mImplementation.connectL();
}

// -----------------------------------------------------------------------------
/**
 * Function implementation
 * @see AfStorageClient::saveActivity(const CAfEntry &,const QPixmap&)
 */
int CAfStorageClientPrivate::saveActivity(const CAfEntry &entry, TInt imageHandle)
{
    return execute(SaveActivity, entry, imageHandle);
}

// -----------------------------------------------------------------------------
/**
 * Function implementation
 * @see AfStorageClient::removeActivity(const CAfEntry &)
 */
int CAfStorageClientPrivate::removeActivity(const CAfEntry &entry)
{
    return execute(RemoveActivity, entry, -1);
}

// -----------------------------------------------------------------------------
/**
 * Function implementation
 * @see AfStorageClient::removeApplicationActivities(const CAfEntry &)
 */
int CAfStorageClientPrivate::removeApplicationActivities(const CAfEntry &entry)
{
    return execute(RemoveApplicationActivities, entry, -1);
}

// -----------------------------------------------------------------------------
/**
 * Function implementation
 * @see AfStorageClient::activities(RPointerArray<CAfEntry> &)
 */
int CAfStorageClientPrivate::activities(RPointerArray<CAfEntry> &results, TInt limit)
{
    CAfEntry *entry = CAfEntry::NewL();
    TInt result = execute(Activities, results, *entry, limit);
    delete entry;
    return result;
}

// -----------------------------------------------------------------------------
/**
 * Function implementation
 * @see AfStorageClient::applicationActivities(RPointerArray<CAfEntry> &, const CAfEntry &)
 */
int CAfStorageClientPrivate::applicationActivities(RPointerArray<CAfEntry> &results, const CAfEntry &templateEntry)
{
    return execute(ApplicationActivities, results, templateEntry);
}

// -----------------------------------------------------------------------------
/**
 * Function implementation
 * @see AfStorageClient::activityData(CAfEntry &, const CAfEntry &)
 */
int CAfStorageClientPrivate::activityData(CAfEntry *&resultEntry, const CAfEntry &templateEntry)
{   
    RPointerArray<CAfEntry> results;
    int errNo(execute(ApplicationActivity, results, templateEntry));
    if (KErrNone == errNo && results.Count() > 0) {
        resultEntry = results[0];
        results.Remove(0);
    }
    results.ResetAndDestroy();
    return errNo;
}

// -----------------------------------------------------------------------------
/**
 * Function implementation
 * @see AfStorageClient::waitActivity()
 */
int CAfStorageClientPrivate::waitActivity()
{
    CAfEntry *entry = CAfEntry::NewL();
    TInt result = execute(WaitActivity, *entry, -1);
    delete entry;
    return result;
}

// -----------------------------------------------------------------------------
/**
 * Function implementation
 * @see AfStorageClient::getThumbnail(const TDesC &imagePath, void *userData)
 */
int CAfStorageClientPrivate::getThumbnail(const TDesC &imagePath, void *userData)
{
    TRAPD(errNo, mImplementation.getThumbnailL(imagePath, userData);)
    return errNo;
}

// -----------------------------------------------------------------------------
/**
 * Function implementation
 * @see AfStorageClient::notifyDataChange()
 */
int CAfStorageClientPrivate::notifyDataChange()
{
    CAfEntry *entry = CAfEntry::NewL();
    TInt result = execute(NotifyChange, *entry, -1);
    delete entry;
    return result;
}

// -----------------------------------------------------------------------------
/**
 * Function implementation
 * @see AfStorageClient::launchActivity(const CAfEntry &)
 */
int CAfStorageClientPrivate::launchActivity(const CAfEntry &templateEntry)
{
    return execute(LaunchActivity, templateEntry, -1);
}

// -----------------------------------------------------------------------------
/**
 * Function execute activity framework functinality and return results 
 */
int CAfStorageClientPrivate::execute(int function, const CAfEntry &sourceEntry, TInt imageHandle)
{
    TRAPD(errNo,
        switch (function) {
        case SaveActivity:
        case RemoveActivity:
        case RemoveApplicationActivities:
        case LaunchActivity:
            mImplementation.executeL(function, sourceEntry, imageHandle);
            break;
        
        case WaitActivity:
        case NotifyChange:
            mImplementation.executeL(function);
            break;
        default:
            User::Leave(KErrArgument);
        }
    )
    return errNo;
}

// -----------------------------------------------------------------------------
/**
 * Function execute activity framework functinality and return results 
 */
int CAfStorageClientPrivate::execute(int function, RPointerArray<CAfEntry>& resultsList,const CAfEntry& templateEntry, int limit)
{
    TRAPD(errNo, mImplementation.executeL(function, resultsList, templateEntry, limit);)
    return errNo;
}
