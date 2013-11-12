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
#include "afstorageclient.h"

#include <e32debug.h>

#include "afstorageclient_p.h"
#include "afcmd.h"

// -----------------------------------------------------------------------------
/**
 * Two-phase constructor. Create and initialize instance
 * @return entry instance
 */
EXPORT_C CAfStorageClient *CAfStorageClient::NewL(MAfAsyncRequestObserver &observer)
{
    CAfStorageClient *self = CAfStorageClient::NewLC(observer);
    CleanupStack::Pop(self);
    return self;
}

// -----------------------------------------------------------------------------
/**
 * Two-phase constructor. Create, initialize and push instance into cleanup stack
 * @return entry instance
 */
EXPORT_C CAfStorageClient *CAfStorageClient::NewLC(MAfAsyncRequestObserver &observer)
{
    CAfStorageClient *self = new (ELeave) CAfStorageClient();
    CleanupStack::PushL(self);
    self->ConstructL(observer);
    return self;
}    

// -----------------------------------------------------------------------------
/**
 * Destructor
 */
EXPORT_C CAfStorageClient::~CAfStorageClient()
{
    delete d_ptr;
}
    
// -----------------------------------------------------------------------------
/**
 * Constructor
 */
CAfStorageClient::CAfStorageClient()
{
}

// -----------------------------------------------------------------------------
/**
 * 2nd phase of construction
 */
void CAfStorageClient::ConstructL(MAfAsyncRequestObserver &observer)
{
    d_ptr = CAfStorageClientPrivate::NewL(observer);
}

// -----------------------------------------------------------------------------
/**
 * Function save an activity (add new one or update existing one)
 * @param entry - activity entry data structure
 * @param imageHandle - handle for activity thumbnail
 * @return 0 on success, error code otherwise
 */
EXPORT_C int CAfStorageClient::saveActivity(const CAfEntry &entry, TInt imageHandle)
{    
    return d_ptr->saveActivity(entry, imageHandle);
}

// -----------------------------------------------------------------------------
/**
 * Function remove existing activity
 * @param entry - activity entry template
  * @return 0 on success, error code otherwise
 */
EXPORT_C int CAfStorageClient::removeActivity(const CAfEntry &entry)
{
    return d_ptr->removeActivity(entry);
}

// -----------------------------------------------------------------------------
/**
 * Function remove existing activities for application
 * @param entry - activity entry template
  * @return 0 on success, error code otherwise
 */
EXPORT_C int CAfStorageClient::removeApplicationActivities(const CAfEntry &entry)
{
    return d_ptr->removeApplicationActivities(entry);
}

// -----------------------------------------------------------------------------
/**
 * Function retreive public data for all activities
 * @param dst - list of results
 * @return 0 on success, error code otherwise 
 */
EXPORT_C int CAfStorageClient::activities(RPointerArray<CAfEntry> &dst, TInt limit)
{
    return d_ptr->activities(dst, limit);
}

// -----------------------------------------------------------------------------
/**
 * Function retreive public data for all application activities
 * @param dst - list of results
 * @param entry - activity template
 * @return 0 on success, error code otherwise 
 */
EXPORT_C int CAfStorageClient::applicationActivities(RPointerArray<CAfEntry> &dst, const CAfEntry &entry)
{
    return d_ptr->applicationActivities(dst, entry);
}

// -----------------------------------------------------------------------------
/**
 * Function retreive all data for requested activity
 * @param dst - activity entry
 * @param entry - activity template
 * @return 0 on success, error code otherwise 
 */
EXPORT_C int CAfStorageClient::activityData(CAfEntry *&dst, const CAfEntry &entry)
{
    return d_ptr->activityData(dst, entry);
}

// -----------------------------------------------------------------------------
/**
 * Function subscribe application for notyfication about requested activity changes
 * @return 0 on success, error code otherwise 
 */
EXPORT_C int CAfStorageClient::waitActivity()
{
    return d_ptr->waitActivity();
}

// -----------------------------------------------------------------------------
/**
 * Function request activity change
 * @param entry - activity template 
 * @return 0 on success, error code otherwise 
 */
EXPORT_C int CAfStorageClient::launchActivity(const CAfEntry &entry)
{
    return d_ptr->launchActivity(entry);
}

// -----------------------------------------------------------------------------
EXPORT_C int CAfStorageClient::getThumbnail(const TDesC &imagePath, void *userData)
{
    return d_ptr->getThumbnail(imagePath, userData);
}

// -----------------------------------------------------------------------------
/**
 * Function subscribe launcher for data model changes
 * @return 0 on success, error code otherwise 
 */
EXPORT_C int CAfStorageClient::notifyDataChange()
{
    return d_ptr->notifyDataChange();
}
