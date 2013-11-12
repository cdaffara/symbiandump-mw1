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
#include <tswindowgroupsobserver.h>
#include "tstaskmonitorglobals.h"
#include "tsrunningappserver.h"
#include "tsrunningappsession.h"
#include "tsresourcemanagerimp.h"
#include "tsbacksteppingactivation.h"
#include "tswindowgroupsmonitorimp.h"
#include "tsmodel.h"
#include "tsstorage.h"
#include "tsservicesprovider.h"
#include "tsserializeddataprovider.h"
#include "tsrunningappmodel.h"

// -----------------------------------------------------------------------------
/**
 * Constructor for performing 1st stage construction
 */
CTsRunningAppServer::CTsRunningAppServer()
:
CServer2(EPriorityStandard)
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
/**
 * Destructor.
 */
CTsRunningAppServer::~CTsRunningAppServer()
    {
    delete iBacksteppingEngine;
    delete iStorage;
    delete iAppsModel;
    delete iServiceProvider;
    delete iSerializer;
    }

// -----------------------------------------------------------------------------
/**
 * Two-phased constructor.
 */
CTsRunningAppServer* CTsRunningAppServer::NewLC()
    {
    CTsRunningAppServer* self = new (ELeave) CTsRunningAppServer();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
/**
 * Default constructor for performing 2nd stage construction
 */
void CTsRunningAppServer::ConstructL()
    {
    StartL(KRunningAppServerName);
    iResources = CTsResourceManager::NewL();
    
    iSerializer = CTsSerializedDataProvider::NewL(*this);
    
    RPointerArray<MTsModel> providers;
    CleanupClosePushL(providers);
    
    iAppsModel = CTsRunningAppModel::NewL(*iResources, iEnv);
    providers.AppendL(iAppsModel);
    
    CTsServiceProviderConfig *cfg = CTsServiceProviderConfig::NewLC(&iEnv);
    iServiceProvider = CTsServiceProvider::NewL( *iResources, *cfg);
    CleanupStack::PopAndDestroy(cfg);
    addProviders(providers, *iServiceProvider);

    iStorage = CTsStorage::NewL(providers.Array());
    iStorage->SetObserver(iSerializer);
    CleanupStack::PopAndDestroy(&providers);

    // load initial data
    iStorage->DataChanged();

    TRAP_IGNORE(iBacksteppingEngine = CTsBacksteppingActivation::NewL(iResources->WsMonitor());)
    }

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see CServer2::NewSessionL(const TVersion&, const RMessage2&)
 */
CSession2* CTsRunningAppServer::NewSessionL( const TVersion & /*aVersion*/, 
                                             const RMessage2& /*aMsg*/) const
    {
    RPointerArray<MTsDataStorage> dataStorages;
    CleanupClosePushL(dataStorages);
    dataStorages.AppendL(const_cast<CTsRunningAppServer *>(this)->iAppsModel);
    dataStorages.AppendL(const_cast<CTsRunningAppServer *>(this)->iStorage);
    CSession2* retVal = 
        CTsRunningAppSession::NewL( *const_cast<CTsRunningAppServer *>(this)->iSerializer,
                                    dataStorages.Array());
    CleanupStack::PopAndDestroy(&dataStorages);
    return retVal;
    }

// -----------------------------------------------------------------------------
void CTsRunningAppServer::DataChanged()
    {
    TAny* currentObserver(0);
    TDblQueIter<CSession2> currentItem(iSessionIter);
    currentItem.SetToFirst();
    do
        {
        currentObserver = currentItem++;
        if(currentObserver)
            {
            static_cast<CTsRunningAppSession*>(currentObserver)->DataChanged();
            }
        }
    while(0 != currentObserver);
    }

// -----------------------------------------------------------------------------
void CTsRunningAppServer::addProviders( RPointerArray<MTsModel> &dst, 
                                        const CTsServiceProvider& serviceProvider)
    {
    for ( TInt offset(0); offset < serviceProvider.Count(); ++offset )
        {
        dst.Append(&serviceProvider[offset]);
        }
    }
