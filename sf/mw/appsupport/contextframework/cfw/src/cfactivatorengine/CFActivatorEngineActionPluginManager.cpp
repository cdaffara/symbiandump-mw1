/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFActivatorEngineActionPluginManager class implementation.
*
*/


#include <ecom/ecom.h>
#include <centralrepository.h>

#include "ContextFrameworkPrivateCRKeys.h"
#include "CFActivatorEngineActionPluginManager.h"
#include "cfactionplugin.h"
#include "CFActionPluginImpl.h"
#include "cfactionindication.h"
#include "cftrace.h"
#include "cfactionpluginthread.h"
#include "CFActivator.h"
#include "cfcommon.h"
#include "cfasynccleanup.h"
#include "cfstartereventhandler.h"

// CONSTANTS

static const TInt KDefaultActionArrayGranularity = 5;

// LOCAL CLASSES
/**
* Loader info
*/
NONSHARABLE_CLASS( TCFActionLoaderInfo )
    {
    public:
    
        TCFActionLoaderInfo( CImplementationInformation& aImplementationInfo,
            CCFActivatorEngineActionPluginManager& aManager ):
            iImplUid( aImplementationInfo.ImplementationUid() ),
            iImplVersion( aImplementationInfo.Version() ),
            iManager( aManager )
            {
            }
            
    public:
    
        TUid iImplUid;
        TInt iImplVersion;
        CCFActivatorEngineActionPluginManager& iManager;
    };

// LOCAL FUNCTIONS

/**
* Clenup operation for RImplInfoPtrArray
*/
LOCAL_C void CleanUpImplInfoArray( TAny* aParams )
    {
    RImplInfoPtrArray* array = static_cast<RImplInfoPtrArray*>( aParams );
    array->ResetAndDestroy();
    }

/**
* Push operation for RImplInfoPtrArray
*/
LOCAL_C void CleanupPushImplInfoArrayL( RImplInfoPtrArray& aArray )
    {
    TCleanupItem item( CleanUpImplInfoArray, &aArray );
    CleanupStack::PushL( item );
    }


// MEMBER FUNCTIONS

CCFActivatorEngineActionPluginManager* CCFActivatorEngineActionPluginManager::NewL(
    RActionCacheArray& aActionCache )
    {
    FUNC_LOG;

    CCFActivatorEngineActionPluginManager* self =
        CCFActivatorEngineActionPluginManager::NewLC( aActionCache );
	CleanupStack::Pop( self );
	return self;
    }

CCFActivatorEngineActionPluginManager* CCFActivatorEngineActionPluginManager::NewLC(
    RActionCacheArray& aActionCache )
    {
    FUNC_LOG;

	CCFActivatorEngineActionPluginManager* self = new( ELeave )
        CCFActivatorEngineActionPluginManager( aActionCache );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
    }


CCFActivatorEngineActionPluginManager::CCFActivatorEngineActionPluginManager(
    RActionCacheArray& aActionCache ):
    iActionCache( aActionCache )
    {
    FUNC_LOG;
    }

CCFActivatorEngineActionPluginManager::~CCFActivatorEngineActionPluginManager()
    {
    FUNC_LOG;

    iCleanupList.ResetAndDestroy();
    iActionPlugInThreads.ResetAndDestroy();
    iLoaders.ResetAndDestroy();
    }

void CCFActivatorEngineActionPluginManager::ConstructL()
    {
    FUNC_LOG;
    }

// METHODS

//----------------------------------------------------------------------------
// CCFActivatorEngineActionPluginManager::TriggerL
//----------------------------------------------------------------------------
//
TBool CCFActivatorEngineActionPluginManager::TriggerL(
    CCFActionIndication* aActionIndication )
    {
    FUNC_LOG;

    TPtrC actionIdentifier( aActionIndication->Identifier() );
    CCFActionCacheElement* element = CCFActivator::ActionCacheElementExists(
        actionIdentifier, iActionCache );
    if( element )
        {
        MCFActionOwner* owner = element->Owner();
        if( owner )
            {
            if( owner->ActionOwner() == MCFActionOwner::ECFInternalAction )
                {
                CCFActionPlugInThread* action =
                    static_cast<CCFActionPlugInThread*>( owner );
                INFO_2( "Action id [%S] triggered from action plug-in [%x]",
                    &actionIdentifier, action->ImplementationUid() );
                action->TriggerL( aActionIndication );
                }
            }
        }

    return element ? ETrue : EFalse;
    }

//----------------------------------------------------------------------------
// CCFActivatorEngineActionPluginManager::InitializePhaseL
//----------------------------------------------------------------------------
//
void CCFActivatorEngineActionPluginManager::InitializePhaseL(
    CCFPhaseBase::TCFPhaseId aPhaseId )
    {
    FUNC_LOG;

    switch( aPhaseId )
        {
        case CCFPhaseBase::ECFDeviceStarting:
            {
            InitDeviceStartingPhaseL();
            break;
            }
        case CCFPhaseBase::ECFLoadingPlugins:
            {
            InitLoadingPluginsPhaseL();
            break;
            }
        default:
            {
            break;
            }
        }
    }

//----------------------------------------------------------------------------
// CCFActivatorEngineActionPluginManager::SetEventHandler
//----------------------------------------------------------------------------
//
void CCFActivatorEngineActionPluginManager::SetEventHandler( MCFStarterEventHandler& aEventHandler )
    {
    FUNC_LOG;

    iEventHandler = &aEventHandler;
    }

//----------------------------------------------------------------------------
// CCFActivatorEngineActionPluginManager::InitDeviceStartingPhaseL
//----------------------------------------------------------------------------
//
void CCFActivatorEngineActionPluginManager::InitDeviceStartingPhaseL()
    {
    FUNC_LOG;

    // List all plugins
    RImplInfoPtrArray implInfoArray;
    CleanupPushImplInfoArrayL( implInfoArray );
    REComSession::ListImplementationsL( KActionPluginUid, implInfoArray );

    // Load activator engine configuration cenrep
    CRepository* cenRep = CRepository::NewLC( KCRUidCFActionConf );
    TInt count = 0;
    TInt err = cenRep->Get( KActionNumberOfMandatoryPlugIns, count );
    if( err == KErrNone && count )
        {
        INFO_1( "Found %d action plug-in implementations from cenrep", count );
        TUint32 key = KActionNumberOfMandatoryPlugIns + 1;
        for( TInt i = 0; i < count; i++ )
            {
            TInt uid = 0;
            err = cenRep->Get( key + i, uid );
            if( err == KErrNone && uid )
                {
                CImplementationInformation* info = NULL;
                for( TInt ii = 0; ii < implInfoArray.Count(); ii++ )
                    {
                    info = implInfoArray[ii];
                    if( info->ImplementationUid() == TUid::Uid( uid ) )
                        {
                        // Implementation info found
                        break;
                        }
                    info = NULL;
                    }
                if( info )
                    {
                    TRAPD( err, LoadPlugInL( info->ImplementationUid(), info->Version() ) );
                    if( err == KErrNone )
                        {
                        INFO_1( "Loaded action plug-in [%x]", uid );
                        }
                    else
                        {
                        ERROR_1( err, "Failed to load action plug-in [%x]", uid );
                        }
                    }
                }
            else
                {
                INFO_2( "UID 0x%08x skipped, error code %d", uid, err );
                }
            }
        }

    // Clean up
    CleanupStack::PopAndDestroy( cenRep );
    CleanupStack::PopAndDestroy( &implInfoArray );
    }

//----------------------------------------------------------------------------
// CCFActivatorEngineActionPluginManager::InitLoadingPluginsPhaseL
//----------------------------------------------------------------------------
//
void CCFActivatorEngineActionPluginManager::InitLoadingPluginsPhaseL()
    {
    FUNC_LOG;
	iUpdatePluginsAllowed = ETrue;
    UpdatePlugInsL();
    }


// ---------------------------------------------------------------------------
// CCFActivatorEngineActionPluginManager::UpdatePlugInsL
// ---------------------------------------------------------------------------
//
void CCFActivatorEngineActionPluginManager::UpdatePlugInsL()
	{
    FUNC_LOG;
	if (!iUpdatePluginsAllowed){return;}
    // List all plugins
    RImplInfoPtrArray implInfoArray;
    CleanupPushImplInfoArrayL( implInfoArray );
    REComSession::ListImplementationsL( KActionPluginUid, implInfoArray );

    TInt allCount = implInfoArray.Count();
    TInt loadedCount = iActionPlugInThreads.Count();
    INFO_1( "Found %d action plug-in implementations from ecom", allCount );
    INFO_1( "%d action plug-ins currently loaded", loadedCount );

    // Check if there were plugins installed
    if( allCount >= loadedCount )
        {
        for( TInt i = iActionPlugInThreads.Count() - 1; i >= 0; i-- )
            {
            CCFActionPlugInThread* actionPluginThread = iActionPlugInThreads[i];
            for( TInt ii = 0; ii < implInfoArray.Count(); ii++ )
                {
                CImplementationInformation* implInfo = implInfoArray[ii];
                if( implInfo->ImplementationUid() ==
                    actionPluginThread->ImplementationUid() )
                    {
                    TInt oldVer = actionPluginThread->Version();
                    TInt newVer = implInfo->Version();

                    // Check if the version has increased
                    if( newVer > oldVer )
                        {
                        // Cleanup old plug-in, new plug-in will be loaded
                        // asynchronously
                        ReleasePlugInL( actionPluginThread, ETrue );
                        }

                    delete implInfo;
                    implInfoArray.Remove( ii );
                    break;
                    }
                }
            }

        // Check if the installed plugin was not an updgrade but a new plugin
        for( TInt i = 0; i < implInfoArray.Count(); i++ )
           {
           CImplementationInformation* implInfo = implInfoArray[i];
           
           // Prepare loaders
           PrepareLoaderL( *implInfo );
           }
           
           // Execute loaders
           ExecuteLoaders();
        }

    // Check if there were plugins unistalled
    else
        {
        for( TInt i = 0; i < iActionPlugInThreads.Count(); i++ )
            {
            CCFActionPlugInThread* actionPluginThread = iActionPlugInThreads[i];
            actionPluginThread->SetMissing( ETrue );
            }

        // Check that which plugins are missing
        for( TInt i = 0; i < implInfoArray.Count(); i++ )
            {
            CImplementationInformation* implInfo = implInfoArray[i];
            for( TInt i = 0; i < iActionPlugInThreads.Count(); i++ )
                {
                CCFActionPlugInThread* actionPluginThread = iActionPlugInThreads[i];
                if( actionPluginThread->ImplementationUid() ==
                    implInfo->ImplementationUid() )
                    {
                    actionPluginThread->SetMissing( EFalse );
                    break;
                    }
                }
            }

        // Delete missing plugins
        for( TInt i = iActionPlugInThreads.Count() - 1; i >= 0; i-- )
            {
            CCFActionPlugInThread* actionPluginThread = iActionPlugInThreads[i];
            if( actionPluginThread->Missing() )
                {
                INFO_2( "Removed uninstalled action plugin [%x], ver: [%d]",
                    actionPluginThread->ImplementationUid().iUid,
                    actionPluginThread->Version() );

                ReleasePlugInL( actionPluginThread, EFalse );
                }
            }
        }

    // Cleanup
    CleanupStack::PopAndDestroy( &implInfoArray );
	}

// ---------------------------------------------------------------------------
// CCFActivatorEngineActionPluginManager::PrepareLoaderL
// ---------------------------------------------------------------------------
//
void CCFActivatorEngineActionPluginManager::PrepareLoaderL( CImplementationInformation& aImplementationInfo )
    {
    FUNC_LOG;    
    TCFActionLoaderInfo* loaderInfo = new(ELeave)TCFActionLoaderInfo( aImplementationInfo, *this );
    CleanupStack::PushL( loaderInfo );
    TCallBack cb( LoaderCallBack, loaderInfo );
    CAsyncCallBack* loader = new( ELeave ) CAsyncCallBack( cb, CActive::EPriorityStandard );
    CleanupStack::PushL( loader );
    iLoaders.AppendL( loader );
    CleanupStack::Pop( loader );
    CleanupStack::Pop( loaderInfo );
    }

// ---------------------------------------------------------------------------
// CCFActivatorEngineActionPluginManager::ExecuteLoaders
// ---------------------------------------------------------------------------
//
void CCFActivatorEngineActionPluginManager::ExecuteLoaders()
    {
    FUNC_LOG;
    
    for( TInt i = 0; i < iLoaders.Count(); i++ )
        {
        CAsyncCallBack* loader = iLoaders[i];
        loader->CallBack();
        }
    }

// ---------------------------------------------------------------------------
// CCFActivatorEngineActionPluginManager::LoaderCallBack
// ---------------------------------------------------------------------------
//
TInt CCFActivatorEngineActionPluginManager::LoaderCallBack( TAny* aLoaderInfo )
    {
    FUNC_LOG;
    
    TCFActionLoaderInfo* loaderInfo = static_cast<TCFActionLoaderInfo*>( aLoaderInfo );
    CCFActivatorEngineActionPluginManager& manager = loaderInfo->iManager;    
    
    TRAPD( err, manager.LoadPlugInL( loaderInfo->iImplUid,
                                     loaderInfo->iImplVersion ) );
    if( err == KErrNone )
        {
        INFO_2( "Loaded action plugin [%x], ver: [%d]",
                loaderInfo->iImplUid.iUid, loaderInfo->iImplVersion );
        }
    else
        {
        ERROR_2( err, "Failed to load action plugin [%x], ver: [%d]",
                 loaderInfo->iImplUid.iUid, loaderInfo->iImplVersion );
        }
    
    manager.iLoadedCount++;
    
    if( manager.iLoadedCount == manager.iLoaders.Count() )
        {
        manager.iLoaders.ResetAndDestroy();
        manager.iLoadedCount = 0;
        
        if( manager.iEventHandler )
            {
        	  manager.iEventHandler->HandleEvent( MCFStarterEventHandler::EActionPluginsLoaded );
            }
        }
    delete loaderInfo;    
    return KErrNone;
    }

//----------------------------------------------------------------------------
// CCFActivatorEngineActionPluginManager::LoadPlugInL
//----------------------------------------------------------------------------
//
void CCFActivatorEngineActionPluginManager::LoadPlugInL( TUid aUid,
                                                         TInt aImplVersion )
	{
    FUNC_LOG;

    CCFActionPlugInThread* actionPluginThread
    	= CCFActionPlugInThread::NewLC( *this, aUid, aImplVersion );

    AddNewActionsToCacheL( *actionPluginThread );

    // Finally add the action plugin
    iActionPlugInThreads.AppendL( actionPluginThread );
    CleanupStack::Pop( actionPluginThread );
	}


//----------------------------------------------------------------------------
// CCFActivatorEngineActionPluginManager::SetImplPointer
//----------------------------------------------------------------------------
//
void CCFActivatorEngineActionPluginManager::SetImplPointer(
		CCFActionPlugIn& aActionPlugIn,
		CCFActionPlugInImpl* aImpl)
	{
    FUNC_LOG;

    // utilizing friendship to set a private member variable
	aActionPlugIn.iImpl = aImpl;
	}

//------------------------------------------------------------------------------
// CCFActivatorEngineActionPluginManager::ReleasePlugIn
//------------------------------------------------------------------------------
//
void CCFActivatorEngineActionPluginManager::ReleasePlugInL(
    CCFActionPlugInThread* aActionPlugInThread,
    TBool aUpgrade )
    {
    FUNC_LOG;

    // Go through the action cache and set action as "inactive" if the
    // release is not followed by an upgrade
    if( !aUpgrade )
        {
        MCFActionOwner* owner = static_cast<MCFActionOwner*>( aActionPlugInThread );
        for( TInt i = 0; i < iActionCache.Count(); i++ )
            {
            CCFActionCacheElement* element = iActionCache[i];
            if( element->Owner() == owner )
                {
                // Action cache element found - set the owner inactive
                element->SetOwner( NULL );
                }
            }
        }
    // Run pending actions and start plug-in cleanup
    aActionPlugInThread->RunPendingActions();
    CCFAsyncCleanup* cleanup = CCFAsyncCleanup::NewLC( aUpgrade,
        *aActionPlugInThread,
        *this );
    TInt err = iCleanupList.Append( cleanup );
    CleanupStack::Pop( cleanup );

    // Check if there were an error while appending cleanup item
    if( err != KErrNone )
        {
        delete cleanup;
        }
    }

//------------------------------------------------------------------------------
// CCFActivatorEngineActionPluginManager::CleanupCompleted
//------------------------------------------------------------------------------
//
void CCFActivatorEngineActionPluginManager::CleanupCompleted(
    CCFAsyncCleanup* aCleanup )
    {
    FUNC_LOG;

    // Search for the cleanup item and delete it
    for( TInt i = 0; i < iCleanupList.Count(); i++ )
        {
        CCFAsyncCleanup* cleanupItem = iCleanupList[i];
        if( cleanupItem == aCleanup )
            {
            if( !cleanupItem->Upgrade() )
                {
                // Cleanup was uninstall based, delete action thread
                CCFActionPlugInThread* obsoleteThread = &cleanupItem->Thread();
                for( TInt ii = 0; ii < iActionPlugInThreads.Count(); ii++ )
                    {
                    CCFActionPlugInThread* thread = iActionPlugInThreads[ii];
                    if( thread == obsoleteThread )
                        {
                        iActionPlugInThreads.Remove( ii );
                        delete thread;
                        break;
                        }
                    }
                }
            iCleanupList.Remove( i );
            delete cleanupItem;
            break;
            }
        }
    }


//------------------------------------------------------------------------------
// CCFActivatorEngineActionPluginManager::AddNewActionsToCacheL
//------------------------------------------------------------------------------
//
void CCFActivatorEngineActionPluginManager::AddNewActionsToCacheL(
    CCFActionPlugInThread& aActionPluginThread )
    {
    FUNC_LOG;

    // Add new actions in the action cache
    CDesCArrayFlat* actions = new( ELeave ) CDesCArrayFlat(
        KDefaultActionArrayGranularity );
    CleanupStack::PushL( actions );
    aActionPluginThread.GetActionsL( *actions );

    // Do not create double action ids
    for( TInt i = 0 ; i < actions->Count(); i++ )
        {
        TPtrC actionId( (*actions)[i] );
        CCFActionCacheElement* element =
            CCFActivator::ActionCacheElementExists( actionId, iActionCache );
        if( element )
            {
            // Check that owner uid is CFServer SID and the owner has not been set
            if( element->OwnerUid() == KCFServerSid && !element->Owner() )
                {
                // Assuming that plug-in has been upgraded
                element->SetOwner( &aActionPluginThread );
                INFO_2( "Action id [%S] from action plug-in [%x] re-activated",
                    &actionId, aActionPluginThread.ImplementationUid() );
                }
            else
                {
                // Action already defined
                ERROR_2( KErrAlreadyExists, "Action id [%S] from action plug-in [%x] already defined",
                    &actionId, aActionPluginThread.ImplementationUid() );
                }
            }
        else
            {
            // new action id
            element = CCFActionCacheElement::NewLC( actionId,
                aActionPluginThread.SecurityPolicy( actionId ),
                KCFServerSid, &aActionPluginThread );
            iActionCache.AppendL( element );
            CleanupStack::Pop( element );
            INFO_2( "Action id [%S] from action plug-in [%x] defined",
                &actionId, aActionPluginThread.ImplementationUid() );
            }
        }

    CleanupStack::PopAndDestroy( actions );
    }

// End of file
