/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CFContextSourceManager class implementation.
*
*/



#include <ecom/ecom.h>
#include <centralrepository.h>
#include <bautils.h>

#include "cfoperationpluginmanager.h"
#include "ContextFrameworkPrivateCRKeys.h"
#include "cfoperationplugin.h"
#include "cfoperationservices.h"
#include "cftrace.h"
#include "CFScriptEngineInterface.h"

// ============================= LOCAL FUNCTIONS ==============================

/**
* Clenup operation for RImplInfoPtrArray
*/
LOCAL_C void CleanUpImplInfoArray( TAny* aParams )
    {
    RImplInfoPtrArray* array = static_cast<RImplInfoPtrArray*>( aParams );
    array->ResetAndDestroy();
    }

/**
* Push operations
*/
LOCAL_C void CleanupResetAndDestroyPushL( RImplInfoPtrArray& aArray )
    {
    TCleanupItem item( CleanUpImplInfoArray, &aArray );
    CleanupStack::PushL( item );
    }

/**
* Operation plug-in information.
*/
NONSHARABLE_CLASS( TCFOperationPluginInfo )
    {
    public:

        TCFOperationPluginInfo( CImplementationInformation& aImplInfo,
            CCFOperationPlugIn* aPlugIn ):
            iImplUid( aImplInfo.ImplementationUid() ),
            iImplVersion( aImplInfo.Version() ),
            iMissing( EFalse ),
            iPlugIn( aPlugIn )
            {
            }

    public:

        TUid iImplUid;
        TInt iImplVersion;
        TBool iMissing;
        CCFOperationPlugIn* iPlugIn;
    };

// ============================= MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CCFOperationPluginManager::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CCFOperationPluginManager* CCFOperationPluginManager::NewL(
    MCFScriptEngineInterface& aScriptEngine )
    {
    FUNC_LOG;

    CCFOperationPluginManager* self =
        CCFOperationPluginManager::NewLC( aScriptEngine );
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------------------------
// CCFOperationPluginManager::NewLC
// ---------------------------------------------------------------------------
//
EXPORT_C CCFOperationPluginManager* CCFOperationPluginManager::NewLC(
    MCFScriptEngineInterface& aScriptEngine )
    {
    FUNC_LOG;

    CCFOperationPluginManager* self =
        new( ELeave ) CCFOperationPluginManager( aScriptEngine );
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }

// ---------------------------------------------------------------------------
// CCFOperationPluginManager::~CCFOperationPluginManager
// ---------------------------------------------------------------------------
//
EXPORT_C CCFOperationPluginManager::~CCFOperationPluginManager()
    {
    FUNC_LOG;

    // Delete plug-ins
    TInt count = iPlugIns.Count();
    for( TInt i = 0; i < count; i++ )
        {
        delete iPlugIns[i].iPlugIn;
        }
    iPlugIns.Close();
    }

// ---------------------------------------------------------------------------
// CCFOperationPluginManager::CCFOperationPluginManager
// ---------------------------------------------------------------------------
//
CCFOperationPluginManager::CCFOperationPluginManager(
    MCFScriptEngineInterface& aScriptEngine ):
    iScriptEngine( aScriptEngine )
    {
    FUNC_LOG;

    // Nothing to do
    }

// ---------------------------------------------------------------------------
// CCFOperationPluginManager::ConstructL
// ---------------------------------------------------------------------------
//
void CCFOperationPluginManager::ConstructL()
    {
    FUNC_LOG;

    }

// ---------------------------------------------------------------------------
// CCFOperationPluginManager::InitializePhaseL
// ---------------------------------------------------------------------------
//
void CCFOperationPluginManager::InitializePhaseL(
    CCFPhaseBase::TCFPhaseId aPhase )
    {
    FUNC_LOG;

    switch( aPhase )
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

// ---------------------------------------------------------------------------
// CCFOperationPluginManager::SetEventHandler
// ---------------------------------------------------------------------------
//
void CCFOperationPluginManager::SetEventHandler( MCFStarterEventHandler& /*aEventHandler*/ )
    {
    FUNC_LOG;	  
    }

// ---------------------------------------------------------------------------
// CCFOperationPluginManager::CreateAndInitializePlugInL
// ---------------------------------------------------------------------------
//
CCFOperationPlugIn* CCFOperationPluginManager::CreateAndInitializePlugInL(
    const TUid& aImplementationUid, TOperationPluginInitParams& aParams )
    {
    FUNC_LOG;

    // Create plug-in
    CCFOperationPlugIn* plugIn = CCFOperationPlugIn::NewL(
        aImplementationUid,
        &aParams );
    CleanupStack::PushL( plugIn );

    // Cleanup
    CleanupStack::Pop( plugIn );

    return plugIn;
    }

// ---------------------------------------------------------------------------
// CCFOperationPluginManager::InitDeviceStartingPhaseL
// ---------------------------------------------------------------------------
//
void CCFOperationPluginManager::InitDeviceStartingPhaseL()
    {
    FUNC_LOG;

    INFO( "Checking plug-ins in [Init Device Starting] phase." );
    UpdatePlugInsL();
    }

// ---------------------------------------------------------------------------
// CCFOperationPluginManager::InitLoadingPluginsPhaseL
// ---------------------------------------------------------------------------
//
void CCFOperationPluginManager::InitLoadingPluginsPhaseL()
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// CCFOperationPluginManager::LoadPluginL
// ---------------------------------------------------------------------------
//
void CCFOperationPluginManager::LoadPluginL(
	CImplementationInformation& aImplInfo )
    {
    FUNC_LOG;

    INFO_2( "Loading operation plug-in with UID [%x], version [%d]",
    		aImplInfo.ImplementationUid().iUid,	aImplInfo.Version() );

    HEAP_2( "Before loading operation plug-in with UID [%x], version [%d]",
            aImplInfo.ImplementationUid().iUid, aImplInfo.Version() );

    TOperationPluginInitParams initParams;
    CCFOperationPlugIn* plugIn = NULL;
    TRAPD( err, plugIn = CreateAndInitializePlugInL(
    		aImplInfo.ImplementationUid(),
    		initParams ) );
    if( err == KErrNone )
        {
        // Plug-in loaded succesfully, store it
        TCFOperationPluginInfo info( aImplInfo, plugIn );
        TInt err = iPlugIns.Append( info );
        if( err != KErrNone )
            {
            ERROR_1( err, "Operation plug-in: [%x] could not be appended in array",
            		aImplInfo.ImplementationUid().iUid );
            delete plugIn;
            plugIn = NULL;
            }
        else
            {
            INFO_1( "Operation plug-in: [%x] succesfully loaded",
            		aImplInfo.ImplementationUid().iUid );
            }
        }
    else
        {
        ERROR_2( err, "Operation plug-in [%x] load error: [%d]",
        		aImplInfo.ImplementationUid().iUid, err );

        err = AddToBlackList( aImplInfo.ImplementationUid() );
        ERROR( err, "Failed to add UID to blacklist" );
        }

    HEAP_2( "After loading operation plug-in with UID [%x], version [%d]",
            aImplInfo.ImplementationUid().iUid, aImplInfo.Version() );
    }


// ---------------------------------------------------------------------------
// CCFOperationPluginManager::UpdatePlugInsL
// ---------------------------------------------------------------------------
//
void CCFOperationPluginManager::UpdatePlugInsL()
	{
    FUNC_LOG;

    // List all plugins
    RImplInfoPtrArray implInfoArray;
    CleanupResetAndDestroyPushL( implInfoArray );
    REComSession::ListImplementationsL( KOperationPlugInInterface,
        implInfoArray );
    RemoveBlackListed( implInfoArray );

    // Check if the update is install or uninstall based
    TInt allCount = implInfoArray.Count();
    TInt loadedCount = iPlugIns.Count();
    INFO_1( "Found %d operation plug-in implementations from ecom", allCount );
    INFO_1( "%d operation plug-ins currently loaded", loadedCount );

    // Check if there were plugins installed
    if( allCount >= loadedCount )
        {
        for( TInt i = iPlugIns.Count() - 1; i >= 0; i-- )
            {
            TCFOperationPluginInfo& pluginInfo = iPlugIns[i];
            for( TInt ii = 0; ii < implInfoArray.Count(); ii++ )
                {
                CImplementationInformation* implInfo = implInfoArray[ii];
                if( implInfo->ImplementationUid() == pluginInfo.iImplUid )
                    {
                    TInt oldVer = pluginInfo.iImplVersion;
                    TInt newVer = implInfo->Version();

                    // Check if the version has increased
                    if( newVer > oldVer )
                        {
                        // Remove scripts which have a dependency to the provider
                        // which is being deleted
                        TInt err = iScriptEngine.RemoveScriptByProviderUid(
                            pluginInfo.iImplUid, ETrue );
                        INFO_2( "Remove script by provider [%x] completed with code [%d]",
                            pluginInfo.iImplUid.iUid, err );

                        // Delete old plugin
                        ReleasePlugIn( pluginInfo.iPlugIn );
                        iPlugIns.Remove( i );

                        // Clean up old implementation
                        REComSession::FinalClose();

                        // Load the new version
                        TRAP( err, LoadPluginL( *implInfo ) );
                        ERROR_3( err, "Upgrade of new version of operation plugin [%x], oldVer: [%d], newVer: [%d] failed",
                            implInfo->ImplementationUid().iUid, oldVer, newVer );
                        if( err == KErrNone )
                            {
                            INFO_3( "Upgraded new version of operation plugin [%x], oldVer: [%d], newVer: [%d]",
                                implInfo->ImplementationUid().iUid, oldVer, newVer );

                            // Rollback removed scripts
                            iScriptEngine.RollbackScripts();
                            }
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
            TRAPD( err, LoadPluginL( *implInfo ) );
            if( err == KErrNone )
                {
                INFO_2( "Loaded operation plugin [%x], ver: [%d]",
                    implInfo->ImplementationUid().iUid, implInfo->Version() );
                }
            else
                {
                ERROR_2( err, "Failed to load operation plugin [%x], ver: [%d]",
                    implInfo->ImplementationUid().iUid, implInfo->Version() );
                }
            }
        }

    // Check if there were plugins unistalled
    else
        {
        for( TInt i = 0; i < iPlugIns.Count(); i++ )
            {
            TCFOperationPluginInfo& pluginInfo = iPlugIns[i];
            pluginInfo.iMissing = ETrue;
            }

        // Check that which plugins are missing
        for( TInt i = 0; i < implInfoArray.Count(); i++ )
            {
            CImplementationInformation* implInfo = implInfoArray[i];
            for( TInt i = 0; i < iPlugIns.Count(); i++ )
                {
                TCFOperationPluginInfo& pluginInfo = iPlugIns[i];
                if( pluginInfo.iImplUid == implInfo->ImplementationUid() )
                    {
                    pluginInfo.iMissing = EFalse;
                    break;
                    }
                }
            }

        // Delete missing plugins
        for( TInt i = iPlugIns.Count() - 1; i >= 0; i-- )
            {
            TCFOperationPluginInfo& pluginInfo = iPlugIns[i];
            if( pluginInfo.iMissing )
                {
                // Remove scripts which have a dependency to the provider
                // which is being deleted
                TInt err = iScriptEngine.RemoveScriptByProviderUid(
                    pluginInfo.iImplUid, ETrue );
                INFO_2( "Remove script by provider [%x] completed with code [%d]",
                    pluginInfo.iImplUid.iUid, err );

                // Release plug-in
                ReleasePlugIn( pluginInfo.iPlugIn );
                INFO_2( "Removed uninstalled operation plugin [%x], ver: [%d]",
                    pluginInfo.iImplUid.iUid, pluginInfo.iImplVersion );
                iPlugIns.Remove( i );

                // Clean up old implementation
                REComSession::FinalClose();

                // Rollback removed scripts since we might have uninstalled
                // plug-in which original version is in rom
                iScriptEngine.RollbackScripts();
                }
            }
        }

    // Cleanup
    CleanupStack::PopAndDestroy( &implInfoArray );
	}

// ---------------------------------------------------------------------------
// CCFOperationPluginManager::ParseScriptRootL
// ---------------------------------------------------------------------------
//
CCFScriptRoot* CCFOperationPluginManager::ParseScriptRootL(
    CCFOperationNode* aParent,
    CMDXMLNode& aNode,
    MCFOperationServices& aOperationServices,
    TUid& aUid )
    {
    CCFScriptRoot* root = DoParseScriptRootL(
        aParent, aNode, aOperationServices, aUid );
    if ( root )
        {
        return root;
        }

    // Operation not found, check new plugins.
    UpdatePlugInsL();

    root = DoParseScriptRootL(
        aParent, aNode, aOperationServices, aUid );
    if ( root )
        {
        return root;
        }

    // Generate error info to log.

    return NULL;
    }

// ---------------------------------------------------------------------------
// CCFOperationPluginManager::ParseL
// ---------------------------------------------------------------------------
//
CCFOperationNode* CCFOperationPluginManager::ParseL( CCFOperationNode* aParent,
    CMDXMLNode& aDOMNode,
    MCFOperationServices& aOperationServices,
    TUid& aUid )
    {
    CCFOperationNode* node = DoParseL(
        aParent, aDOMNode, aOperationServices, aUid );
	if ( node )
		{
		return node;
		}

    // Operation not found, check new plugins.
    UpdatePlugInsL();

    node = DoParseL(
        aParent, aDOMNode, aOperationServices, aUid );
    if ( node )
        {
        return node;
        }

    // Generate error info to log.

    return NULL;
    }

//------------------------------------------------------------------------------
// CCFOperationPluginManager::ReleasePlugIn
//------------------------------------------------------------------------------
//
void CCFOperationPluginManager::ReleasePlugIn( CCFOperationPlugIn*& aPlugIn )
    {
    FUNC_LOG;

    delete aPlugIn;
    aPlugIn = NULL;
    }

//------------------------------------------------------------------------------
// CCFOperationPluginManager::DoParseL
//------------------------------------------------------------------------------
//
CCFOperationNode* CCFOperationPluginManager::DoParseL( CCFOperationNode* aParent,
    CMDXMLNode& aDOMNode,
    MCFOperationServices& aOperationServices,
    TUid& aUid )
    {
    FUNC_LOG;

    CCFOperationNode* node = NULL;
    for ( TInt i = 0; i < iPlugIns.Count(); i++ )
        {
        CCFOperationPlugIn* plugin = iPlugIns[ i ].iPlugIn;
        node = plugin->ParseL(
            aParent, aDOMNode, aOperationServices );
        if( node )
            {
            // Node found - break
            aUid = iPlugIns[i].iImplUid;
            break;
            }
        }

    return node;
    }

//------------------------------------------------------------------------------
// CCFOperationPluginManager::DoParseScriptRootL
//------------------------------------------------------------------------------
//
CCFScriptRoot* CCFOperationPluginManager::DoParseScriptRootL(
    CCFOperationNode* aParent,
    CMDXMLNode& aDOMNode,
    MCFOperationServices& aOperationServices,
    TUid& aUid )
    {
    FUNC_LOG;

    CCFScriptRoot* root = NULL;
    for ( TInt i = 0; i < iPlugIns.Count(); i++ )
        {
        CCFOperationPlugIn* plugin = iPlugIns[ i ].iPlugIn;
        root = plugin->ParseScriptRootL(
            aParent, aDOMNode, aOperationServices );
        if( root )
            {
            // Root found - break
            aUid = iPlugIns[i].iImplUid;
            break;
            }
        }

    return root;
    }

// End of file
