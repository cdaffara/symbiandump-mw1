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

#ifndef AFSTORAGECLIENTPRIVATE_H
#define AFSTORAGECLIENTPRIVATE_H

#include <e32base.h>
#include "afstorageglobals.h"
#include "afstorageclientimp.h"

class CAfStorageClientPrivate : public CBase
{

public:
    static CAfStorageClientPrivate *NewL(MAfAsyncRequestObserver &observer);
    static CAfStorageClientPrivate *NewLC(MAfAsyncRequestObserver &observer);
    virtual ~CAfStorageClientPrivate();

private:
    CAfStorageClientPrivate(MAfAsyncRequestObserver &observer);
    void ConstructL();

public:
    int connect();
    int saveActivity(const CAfEntry &entry, TInt imageHandle);
    int removeActivity(const CAfEntry &templateEntry);
    int removeApplicationActivities(const CAfEntry &templateEntry);
    int activities(RPointerArray<CAfEntry> &dst, TInt limit = 0);
    int applicationActivities(RPointerArray<CAfEntry> &dst, const CAfEntry &entry);
    int activityData(CAfEntry *&resultEntry, const CAfEntry &templateEntry);
    int waitActivity();
    int getThumbnail(const TDesC &imagePath, void *userData);
    int notifyDataChange();
    int launchActivity(const CAfEntry &templateEntry);

private:
    int execute(int function, const CAfEntry &sourceEntry, TInt imageHandle);
    int execute(int function, RPointerArray<CAfEntry> &resultsList,const CAfEntry &templateEntry, int limit = 0);

private:
    RAfStorageClientImplementation mImplementation;
};

#endif // AFSTORAGECLIENTPRIVATE_H
