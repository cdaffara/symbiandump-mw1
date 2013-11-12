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
#ifndef RUNNINGAPPSERVER_H
#define RUNNINGAPPSERVER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <w32std.h>
#include "tsdataobserver.h"
#include "tsenv.h"

class CTsResourceManager;
class MTsModel;
class CTsRunningAppModel;
class CTsStorage;
class CTsSerializedDataProvider;
class CTsServiceProvider;

/**
 *  CRunningAppServer
 * 
 */
class CTsRunningAppServer : public CServer2,
                            public MTsDataObserver
{
public:
    ~CTsRunningAppServer();

    static CTsRunningAppServer* NewLC();
    
public: //MHsDataObserver
    void DataChanged();

private:
    CTsRunningAppServer();
    void ConstructL();
    CSession2* NewSessionL(const TVersion& aVersion, const RMessage2& aMsg) const;
    void addProviders( RPointerArray<MTsModel> &aDst, 
                       const CTsServiceProvider& aServiceProvider);

private:
    TsEnv iEnv;
    CTsResourceManager* iResources;
    CTsSerializedDataProvider* iSerializer;
    CTsStorage* iStorage; 
    CTsServiceProvider* iServiceProvider;
    CTsRunningAppModel* iAppsModel;
    CBase *iBacksteppingEngine;
};

#endif // RUNNINGAPPSERVER_H
