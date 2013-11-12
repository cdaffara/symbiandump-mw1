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

class CAfEntry;

class CAfStorageClient : public CBase
{
public:
    static CAfStorageClient *NewL(MAfAsyncRequestObserver &observer);
    static CAfStorageClient *NewLC(MAfAsyncRequestObserver &observer);
    virtual ~CAfStorageClient();

private:
    CAfStorageClient();
    
public:    
    int launchActivity(const CAfEntry &entry);
    
public:
    static TInt lastLaunchAppId;
    static HBufC *lastLaunchUri;
    
};

#endif //AFSTORAGECLIENT_H
