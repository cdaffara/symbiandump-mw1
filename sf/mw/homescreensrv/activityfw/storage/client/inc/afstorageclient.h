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

#ifndef AFSTORAGECLIENT_H
#define AFSTORAGECLIENT_H

#include "afasyncrequestobserver.h"

class CAfStorageClientPrivate;
class CAfEntry;

NONSHARABLE_CLASS(CAfStorageClient) : public CBase
{
public:
    IMPORT_C static CAfStorageClient *NewL(MAfAsyncRequestObserver &observer);
    IMPORT_C static CAfStorageClient *NewLC(MAfAsyncRequestObserver &observer);
    IMPORT_C virtual ~CAfStorageClient();

private:
    CAfStorageClient();
    void ConstructL(MAfAsyncRequestObserver &observer);
    
public:    
    IMPORT_C int saveActivity(const CAfEntry &entry, TInt imageHandle);
    IMPORT_C int removeActivity(const CAfEntry &entry);
    IMPORT_C int removeApplicationActivities(const CAfEntry &entry);
    IMPORT_C int activities(RPointerArray<CAfEntry> &dst, TInt limit = 0);
    IMPORT_C int applicationActivities(RPointerArray<CAfEntry> &dst, const CAfEntry &entry);
    IMPORT_C int activityData(CAfEntry *&dst, const CAfEntry &entry);
    IMPORT_C int waitActivity();
    IMPORT_C int launchActivity(const CAfEntry &entry);
    IMPORT_C int getThumbnail(const TDesC &imagePath, void *userData);
    IMPORT_C int notifyDataChange();

private: 
    CAfStorageClientPrivate *d_ptr;
    
};

#endif //AFSTORAGECLIENT_H
