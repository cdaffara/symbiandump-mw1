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
* Description:  CFContextSourceManager class implementation.
*
*/



#include <ecom/ecom.h>
#include <centralrepository.h>
#include <bautils.h>
#include <cfcontextsourceupgrade.h>
#include <cfcontextsourceinterface.h>
#include <cfcontextsourcecommand.h>
#include <cfcontextsubscriptionlistener.h>

#include "CFContextSourceManager.h"
#include "cfcontextsourcesettingsmanager.h"
#include "ContextFrameworkPrivateCRKeys.h"
#include "cfcontextsourceplugin.h"
#include "cfextendedcontextinterface.h"
#include "cfcontextsourcesettingarray.h"
#include "cftrace.h"

// CONSTANTS

const TInt KUIDNameStartPos = 1;
const TInt KUIDNameEndPos = 8;

// Drive letter hard coded since context source plug-in settings
// can only exist in ROM or RAM
_LIT( KSettingsPathRom, "Z:\\Private\\10282BC4\\Settings\\" );
_LIT( KSettingsPathRam, "C:\\Private\\10282BC4\\Settings\\" );

// Folder extension
_LIT( KFolderExt, "\\" );

// Setting file extension
_LIT( KSettingExtension, "*.xml" );

// Separates SID from filename for client installed settings
_LIT( KUnderscore, "_" );


/**
 * Information about a client acting as a context source.
 * This info is used to keep track of clients acting as context sources to
 * enable sending them context source commands.
 *
 * @lib CFContextSourceManager
 * @since S60 5.0
 */
NONSHARABLE_CLASS( TCFClientContextSourceInfo )
{
public: // Constructors

    TCFClientContextSourceInfo() : iSource( NULL ), iClientSid( KNullUid )
        {
        FUNC_LOG;
        }

    TCFClientContextSourceInfo( MCFContextSource* aSource, const TUid& aSid )
        :   iSource( aSource ), iClientSid( aSid )
        {
        FUNC_LOG;
        }

public: // Data

    /**
    * Context source interface pointer; not owned.
    */
    MCFContextSource* iSource;

    /**
    * Client's secure id (from client thread).
    */
    TUid iClientSid;
};

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
* Orders client countext source infos based on their uids.
* @param aFirst First context source info.
* @param aSecond Second context source info.
* @return Zero if the uid of the first and second are equal. Negative value
*   if the uid of the first is less than the uid of the second. Positive
*   value if the uid of the first is greater than the uid of the second.
*/
LOCAL_C TInt ClientContextSourceInfoUidOrder(
    const TCFClientContextSourceInfo& aFirst,
    const TCFClientContextSourceInfo& aSecond )
    {
    TInt orderValue = 1;

    if ( aFirst.iClientSid == aSecond.iClientSid )
        {
        orderValue = 0;
        }
    else if ( aFirst.iClientSid.iUid < aSecond.iClientSid.iUid )
        {
        orderValue = -1;
        }

    return orderValue;
    }

/**
* Context loader information.
*/
NONSHARABLE_CLASS( TCFLoaderInfo )
    {
    public:
    
    TCFLoaderInfo( CImplementationInformation& aImplementationInfo,
                   CCFContextSourceManager& aManager ):
                   iImplUid( aImplementationInfo.ImplementationUid() ),
                   iImplVersion( aImplementationInfo.Version() ),
                   iManager( aManager )
        {
        }
            
    public:
        TUid iImplUid;
        TInt iImplVersion;
        CCFContextSourceManager& iManager;
    };

/**
* Context source information.
*/
NONSHARABLE_CLASS( TCFContextSourceInfo )
    {
    public:

        TCFContextSourceInfo( CImplementationInformation& aImplInfo,
            CCFContextSourcePlugIn* aPlugIn ):
            iImplUid( aImplInfo.ImplementationUid() ), iImplVersion( aImplInfo.Version() ),
            iPlugIn( aPlugIn ), iMissing( ETrue )
            {
            }
    
    TCFContextSourceInfo( TUid aImplUid,
                          TInt aImplVersion,
                          CCFContextSourcePlugIn* aPlugIn ):
                          iImplUid( aImplUid ),
                          iImplVersion( aImplVersion ),
                          iPlugIn( aPlugIn ),
                          iMissing( ETrue )
        {
        }

    public:

        TUid iImplUid;
        TInt iImplVersion;
        CCFContextSourcePlugIn* iPlugIn;
        TBool iMissing;
    };

// ============================= MEMBER FUNCTIONS =============================

EXPORT_C CCFContextSourceManager* CCFContextSourceManager::NewL(
    MCFExtendedContextInterface& aCF,
    RFs& aFs )
    {
    FUNC_LOG;

    CCFContextSourceManager* self =
        CCFContextSourceManager::NewLC( aCF, aFs );
    CleanupStack::Pop( self );

    return self;
    }

EXPORT_C CCFContextSourceManager* CCFContextSourceManager::NewLC(
    MCFExtendedContextInterface& aCF,
    RFs& aFs )
    {
    FUNC_LOG;

    CCFContextSourceManager* self =
        new( ELeave ) CCFContextSourceManager( aCF, aFs );
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }

EXPORT_C CCFContextSourceManager::~CCFContextSourceManager()
    {
    FUNC_LOG;

    iClientContextSources.Close();

    // Delete plug-ins
    TInt count = iPlugIns.Count();
    for( TInt i = 0; i < count; i++ )
        {
        CCFContextSourcePlugIn* plugIn = iPlugIns[i].iPlugIn;
        ReleasePlugIn( plugIn );
        }
    iPlugIns.Close();
    delete iSettingsManager;
    
    iLoaders.ResetAndDestroy();
    }

CCFContextSourceManager::CCFContextSourceManager(
    MCFExtendedContextInterface& aCF,
    RFs& aFs ):
    iCF( aCF ),
    iFs( aFs )
    {
    FUNC_LOG;

    // Nothing to do
    }

void CCFContextSourceManager::ConstructL()
    {
    FUNC_LOG;

    iSettingsManager = CCFContextSourceSettingsManager::NewL( iFs );
    }

void CCFContextSourceManager::SetEventHandler( MCFStarterEventHandler& aEventHandler )
    {
    FUNC_LOG;
	  iEventHandler = &aEventHandler;
    }

// ---------------------------------------------------------------------------
// CCFContextSourceManager::InstallSettingL
// ---------------------------------------------------------------------------
//
EXPORT_C void CCFContextSourceManager::InstallSettingL( RFile& aSettingFile,
    const TUid& aContextSourceUid,
    RThread& aClientThread )
    {
    FUNC_LOG;

    // Get plugin.
    MCFContextSourceUpgrade* sourceUpgrade = PlugInL( aContextSourceUid );

    // Parse setting file.
    CCFContextSourceSettingArray* settings
            = CCFContextSourceSettingArray::NewLC();    // CLEANUP<< settings
    RFile parseFile;
    User::LeaveIfError( parseFile.Duplicate( aSettingFile ) );
    // parseFile handle closed by the parser.
    iSettingsManager->ParseSettingsL( parseFile, *settings );

    // Check setting validity from plugin.
    TInt err = sourceUpgrade->CheckValidity( aClientThread, *settings );
    CleanupStack::PopAndDestroy( settings );            // CLEANUP>> settings
    ERROR( err, "CCFContextSourceManager::InstallSettingL - Context source CheckSecurity for new settings failed" );
    User::LeaveIfError( err );

    // Copy the setting file.
    CopySettingFileL( aSettingFile, aContextSourceUid, aClientThread );

    // Read all settings and give them to the plugin.
    CCFContextSourceSettingArray* settingArray
            = PlugInSettingsL( aContextSourceUid );
    // Ownership of settingArray is transferred to plugin.
    sourceUpgrade->UpdateSettingsL( settingArray );
    }

// ---------------------------------------------------------------------------
// CCFContextSourceManager::UninstallSettingL
// ---------------------------------------------------------------------------
//
EXPORT_C void CCFContextSourceManager::UninstallSettingL(
    const TDesC& aSettingFilename,
    const TUid& aContextSourceUid,
    RThread& aClientThread )
    {
    FUNC_LOG;

    // Get plugin.
    MCFContextSourceUpgrade* sourceUpgrade = PlugInL( aContextSourceUid );

    // Delete the setting file.
    DeleteSettingFileL( aSettingFilename, aContextSourceUid, aClientThread );

    // Read all settings and give them to the plugin.
    CCFContextSourceSettingArray* settingArray
            = PlugInSettingsL( aContextSourceUid );
    // Ownership of settingArray is transferred to plugin.
    sourceUpgrade->UpdateSettingsL( settingArray );
    }

// ---------------------------------------------------------------------------
// CCFContextSourceManager::UninstallSettingsL
// ---------------------------------------------------------------------------
//
EXPORT_C void CCFContextSourceManager::UninstallSettingsL(
    const TUid& aContextSourceUid,
    RThread& aClientThread )
    {
    FUNC_LOG;

    // Get plugin.
    MCFContextSourceUpgrade* sourceUpgrade = PlugInL( aContextSourceUid );

    // Delete all setting files installed by the client.
    DeleteSettingFileL( KSettingExtension, aContextSourceUid, aClientThread );

    // Read all settings and give them to the plugin.
    CCFContextSourceSettingArray* settingArray
            = PlugInSettingsL( aContextSourceUid );
    // Ownership of settingArray is transferred to plugin.
    sourceUpgrade->UpdateSettingsL( settingArray );
    }

// ---------------------------------------------------------------------------
// CCFContextSourceManager::HandleContextSourceCommandL
// ---------------------------------------------------------------------------
//
EXPORT_C void CCFContextSourceManager::HandleContextSourceCommandL(
    const CCFContextSourceCommand& aCommand )
    {
    FUNC_LOG;

    const TUid sourceUid = aCommand.SourceUid();
    // Find plugin.
    CCFContextSourcePlugIn* plugIn = NULL;
    for( TInt i = 0; i < iPlugIns.Count(); ++i )
        {
        if( iPlugIns[ i ].iImplUid == sourceUid )
            {
            plugIn = iPlugIns[ i ].iPlugIn;
            break;
            }
        }

    MCFContextSource* contextSource = NULL;
    if ( plugIn )
        {
        // Check plugin's support for MCFContextSource extension.
        contextSource = reinterpret_cast< MCFContextSource* >(
                plugIn->Extension( KCFContextSourceInterfaceUid ) );
        if ( !contextSource )
            {
            User::Leave( KErrExtensionNotSupported );
            }
        }
    else
        {
        TLinearOrder< TCFClientContextSourceInfo > uidOrder(
                ClientContextSourceInfoUidOrder );
        TCFClientContextSourceInfo clientInfo;
        clientInfo.iClientSid = sourceUid;
        TInt pos = iClientContextSources.FindInOrder( clientInfo, uidOrder );
        if ( pos == KErrNotFound )
            {
            User::Leave( KErrBadHandle );
            }
        contextSource = iClientContextSources[ pos ].iSource;
        }

    // Process source command
    if ( contextSource )
        {
        contextSource->HandleCommand( aCommand );
        }
    }

// ---------------------------------------------------------------------------
// CCFContextSourceManager::RegisterClientContextSource
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCFContextSourceManager::RegisterClientContextSource(
    MCFContextSource* aPublisher,
    const TUid& aPublisherUid )
    {
    FUNC_LOG;

    TLinearOrder< TCFClientContextSourceInfo > uidOrder(
            ClientContextSourceInfoUidOrder );
    TCFClientContextSourceInfo clientInfo( aPublisher, aPublisherUid );

    TInt err = iClientContextSources.InsertInOrder( clientInfo, uidOrder );
    if ( err == KErrAlreadyExists )
        {
        err = KErrNone;
        }

    ERROR( err, "CCFContextSourceManager::RegisterContextSourceClient - Failed" );

    return err;
    }

// ---------------------------------------------------------------------------
// CCFContextSourceManager::DeregisterClientContextSource
// ---------------------------------------------------------------------------
//
EXPORT_C void CCFContextSourceManager::DeregisterClientContextSource(
    const MCFContextSource& aPublisher )
    {
    FUNC_LOG;

    for ( TInt i = 0; i < iClientContextSources.Count(); ++i )
        {
        if ( &aPublisher == iClientContextSources[ i ].iSource )
            {
            iClientContextSources.Remove( i );
            break; // Stop, no duplicate entries are allowed.
            }
        }
    }


// ---------------------------------------------------------------------------
// CCFContextSourceManager::InitializePhaseL
// ---------------------------------------------------------------------------
//
void CCFContextSourceManager::InitializePhaseL(
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
// CCFContextSourceManager::UpdatePlugInsL
// ---------------------------------------------------------------------------
//
void CCFContextSourceManager::UpdatePlugInsL()
	{
    FUNC_LOG;
	if (!iUpdatePluginsAllowed){return;}
    // List all plugins
    RImplInfoPtrArray implInfoArray;
    CleanupResetAndDestroyPushL( implInfoArray );
    REComSession::ListImplementationsL( KContextSourcePlugInInterface,
        implInfoArray );
    RemoveBlackListed( implInfoArray );

    // Check if the update is install or uninstall based
    TInt allCount = implInfoArray.Count();
    TInt loadedCount = iPlugIns.Count();
    INFO_1( "Found %d context source plug-in implementations from ecom", allCount );
    INFO_1( "%d context source plug-ins currently loaded", loadedCount );

    // Check if there were plugins installed
    if( allCount >= loadedCount )
        {
        for( TInt i = iPlugIns.Count() - 1; i >= 0; i-- )
            {
            TCFContextSourceInfo& pluginInfo = iPlugIns[i];
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
                        // Delete old plugin
                        ReleasePlugIn( pluginInfo.iPlugIn );
                        iPlugIns.Remove( i );

                        // Clean up garbage
                        REComSession::FinalClose();

                        // Load the new version
                        TRAPD( err, LoadPluginL( implInfo->ImplementationUid(), implInfo->Version() ) );
                        ERROR_3( err, "Upgrade of new version of context source plugin [%x], oldVer: [%d], newVer: [%d] failed",
                            implInfo->ImplementationUid().iUid, oldVer, newVer );
                        if( err == KErrNone )
                            {
                            INFO_3( "Upgraded new version of context source plugin [%x], oldVer: [%d], newVer: [%d]",
                                implInfo->ImplementationUid().iUid, oldVer, newVer );
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
            
            // Prepare loaders
            PrepareLoaderL( implInfo );
            }
            // Execute loaders
           ExecuteLoaders();
        }

    // Check if there were plugins unistalled
    else
        {
        for( TInt i = 0; i < iPlugIns.Count(); i++ )
            {
            TCFContextSourceInfo& pluginInfo = iPlugIns[i];
            pluginInfo.iMissing = ETrue;
            }

        // Check that which plugins are missing
        for( TInt i = 0; i < implInfoArray.Count(); i++ )
            {
            CImplementationInformation* implInfo = implInfoArray[i];
            for( TInt i = 0; i < iPlugIns.Count(); i++ )
                {
                TCFContextSourceInfo& pluginInfo = iPlugIns[i];
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
            TCFContextSourceInfo& pluginInfo = iPlugIns[i];
            if( pluginInfo.iMissing )
                {
                INFO_2( "Removed uninstalled context source plugin [%x], ver: [%d]",
                    pluginInfo.iImplUid.iUid, pluginInfo.iImplVersion );

                ReleasePlugIn( pluginInfo.iPlugIn );
                iPlugIns.Remove( i );

                // Clean up garbage
                REComSession::FinalClose();
                }
            }
        }

    // Cleanup
    CleanupStack::PopAndDestroy( &implInfoArray );
	}

// ---------------------------------------------------------------------------
// CCFContextSourceManager::PrepareLoaderL
// ---------------------------------------------------------------------------
//
void CCFContextSourceManager::PrepareLoaderL( CImplementationInformation* aImplementationInfo )
    {
    FUNC_LOG;

    TCFLoaderInfo* loaderInfo = new(ELeave)TCFLoaderInfo( *aImplementationInfo, *this );
    CleanupStack::PushL( loaderInfo );
    TCallBack cb( LoaderCallBack, loaderInfo );
    CAsyncCallBack* loader = new( ELeave ) CAsyncCallBack( cb, CActive::EPriorityStandard );
    CleanupStack::PushL( loader );
    iLoaders.AppendL( loader );
    CleanupStack::Pop( loader );
    CleanupStack::Pop( loaderInfo );    
    }

// ---------------------------------------------------------------------------
// CCFContextSourceManager::ExecuteLoaders
// ---------------------------------------------------------------------------
//
void CCFContextSourceManager::ExecuteLoaders()
    {
    FUNC_LOG;
    
    for( TInt i = 0; i < iLoaders.Count(); i++ )
        {
        CAsyncCallBack* loader = iLoaders[i];
        loader->CallBack();
        }
    }

// ---------------------------------------------------------------------------
// CCFContextSourceManager::LoaderCallBack
// ---------------------------------------------------------------------------
//
TInt CCFContextSourceManager::LoaderCallBack( TAny* aLoaderInfo )
    {
    FUNC_LOG;
    
    TCFLoaderInfo* loaderInfo = static_cast<TCFLoaderInfo*>( aLoaderInfo );
    CCFContextSourceManager& manager = loaderInfo->iManager;    
    
    TRAPD( err, manager.LoadPluginL( loaderInfo->iImplUid,
                                     loaderInfo->iImplVersion ));                

    if( err == KErrNone )
        {
        INFO_2( "Loaded context source plugin [%x], ver: [%d]",
                 loaderInfo->iImplUid.iUid, loaderInfo->iImplVersion );
        }
    else
        {
        ERROR_2( err, "Failed to load context source plugin [%x], ver: [%d]",
                 loaderInfo->iImplUid.iUid, loaderInfo->iImplVersion );
        }
    
    manager.iLoadedCount++;
    
    if( manager.iLoadedCount == manager.iLoaders.Count() )
        {
        manager.iLoaders.ResetAndDestroy();
        manager.iLoadedCount = 0;

        if( manager.iEventHandler )
            {
        	manager.iEventHandler->HandleEvent( MCFStarterEventHandler::EContextSourcePluginsLoaded );
            }
        }
    delete loaderInfo;    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CCFContextSourceManager::CreateAndInitializePlugInL
// ---------------------------------------------------------------------------
//
CCFContextSourcePlugIn* CCFContextSourceManager::CreateAndInitializePlugInL(
    const TUid& aImplementationUid,
    TContextSourceInitParams& aParams )
    {
    FUNC_LOG;

    // Create plug-in
    CCFContextSourcePlugIn* plugIn = CCFContextSourcePlugIn::NewL(
        aImplementationUid,
        &aParams );
    CleanupStack::PushL( plugIn );

    // Handle settings
    CCFContextSourceSettingArray* settings
            = PlugInSettingsL( aImplementationUid );

    if ( settings->Count() )
        {
        // Forward settings to plug-in
        // Plug-in takes ownership of the array.
        plugIn->HandleSettingL( settings );
        }
    else
        {
        // Manager owns the array, cleanup now.
        delete settings;
        settings = NULL;
        }

    // Define context
    plugIn->DefineContextsL();

    // Initialize
    plugIn->InitializeL();

    // Cleanup
    CleanupStack::Pop( plugIn );

    return plugIn;
    }

// ---------------------------------------------------------------------------
// CCFContextSourceManager::InitDeviceStartingPhaseL
// ---------------------------------------------------------------------------
//
void CCFContextSourceManager::InitDeviceStartingPhaseL()
    {
    FUNC_LOG;

    // List all plugins
    RImplInfoPtrArray implInfoArray;
    CleanupResetAndDestroyPushL( implInfoArray );
    REComSession::ListImplementationsL( KContextSourcePlugInInterface,
        implInfoArray );

    // Load context source manager configuration cenrep
    CRepository* cenRep = CRepository::NewLC( KCRUidCFContextSourceConf );
    TInt count = 0;
    TInt err = cenRep->Get( KContextSourceNumberOfMandatoryPlugIns, count );
    if( err == KErrNone && count )
        {
        INFO_1( "Found %d context source plug-in implementations from cenrep", count );
        TUint32 key = KContextSourceNumberOfMandatoryPlugIns + 1;
        for( TInt i = 0; i < count; i++ )
            {
            TInt uid = 0;

            // Ignore first key
            err = cenRep->Get( key + i, uid );
            if( err == KErrNone && uid )
                {
                CImplementationInformation* info = NULL;
                for( TInt ii = 0; ii < implInfoArray.Count(); ii++ )
                    {
                    info = implInfoArray[ii];
                    if( info->ImplementationUid() == TUid::Uid( uid ) )
                        {
                        break;
                        }
                    info = NULL;
                    }
                if( info )
                    {
                    TRAPD( err, LoadPluginL( info->ImplementationUid(), info->Version() ) );
                    if( err == KErrNone )
                        {
                        INFO_2( "Loaded context source plugin [%x], ver: [%d]",
                            info->ImplementationUid().iUid, info->Version() );
                        }
                    else
                        {
                        ERROR_2( err, "Failed to load context source plugin [%x], ver: [%d]",
                            info->ImplementationUid().iUid, info->Version() );
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

// ---------------------------------------------------------------------------
// CCFContextSourceManager::InitLoadingPluginsPhaseL
// ---------------------------------------------------------------------------
//
void CCFContextSourceManager::InitLoadingPluginsPhaseL()
    {
    FUNC_LOG;
	iUpdatePluginsAllowed = ETrue;
    UpdatePlugInsL();
    }

// ---------------------------------------------------------------------------
// CCFContextSourceManager::LoadPluginL
// ---------------------------------------------------------------------------
//
void CCFContextSourceManager::LoadPluginL( TUid aUid,
                                           TInt aImplVersion )
    {
    FUNC_LOG;

    INFO_2( "Loading context source plug-in with UID [%x], version [%d]",
    		aUid.iUid,
    		aImplVersion );
    HEAP_2( "Before loading context source plug-in with UID [%x], version [%d]",
            aUid.iUid,
            aImplVersion );
    TContextSourceInitParams initParams( static_cast<MCFContextInterface&>( iCF ) );
    CCFContextSourcePlugIn* plugIn = NULL;
    TRAPD( err, plugIn = CreateAndInitializePlugInL( aUid,
    		                                         initParams ) );
    if( err == KErrNone )
        {
        // Plug-in loaded succesfully, store it
        TCFContextSourceInfo info( aUid, aImplVersion, plugIn );
        TInt err = iPlugIns.Append( info );
        if( err != KErrNone )
            {
            ERROR_1( err, "Context source plug-in: %x could not be appended in array",
            		aUid.iUid );
            ReleasePlugIn( plugIn );
            }
        else
            {
            INFO_1( "Context source plug-in: %x succesfully loaded",
            		aUid.iUid );
            }
        }
    else
        {
        ERROR_2( err, "Context source plug-in %x load error: %d",
        		aUid.iUid,
    	    	err );

        err = AddToBlackList( aUid );
        ERROR( err, "Failed to add UID to blacklist" );
        }

    HEAP_2( "After loading context source plug-in with UID [%x], version [%d]",
            aUid.iUid,
            aImplVersion );
    }

// ---------------------------------------------------------------------------
// CCFContextSourceManager::ConfigureRomSettingFolder
// ---------------------------------------------------------------------------
//
TBool CCFContextSourceManager::ConfigureRomSettingFolder( TDes& aFile,
    const TUid& aUid )
    {
    FUNC_LOG;

    // Check rom
    // '[12345678]' -> 'z:\private\10282BC4\Settings\12345678\'
    aFile.Copy( KSettingsPathRom );
    aFile.Append( aUid.Name().Mid(
        KUIDNameStartPos,
        KUIDNameEndPos ) );
    aFile.Append( KFolderExt );

    return BaflUtils::PathExists( iFs, aFile );
    }

// ---------------------------------------------------------------------------
// CCFContextSourceManager::ConfigureRamSettingFolder
// ---------------------------------------------------------------------------
//
TBool CCFContextSourceManager::ConfigureRamSettingFolder( TDes& aFile,
    const TUid& aUid )
    {
    FUNC_LOG;

    // Check rom
    // '[12345678]' -> 'c:\private\10282BC4\Settings\12345678\'
    aFile.Copy( KSettingsPathRam );
    aFile.Append( aUid.Name().Mid(
        KUIDNameStartPos,
        KUIDNameEndPos ) );
    aFile.Append( KFolderExt );

    return BaflUtils::PathExists( iFs, aFile );
    }

// ---------------------------------------------------------------------------
// CCFContextSourceManager::ParsePlugInSettingsL
// ---------------------------------------------------------------------------
//
void CCFContextSourceManager::ParsePlugInSettingsL( const TDesC& aFolder,
    CCFContextSourceSettingArray& aSettings )
    {
    CDir* dirList = NULL;
    TInt err = iFs.GetDir( aFolder,
        KEntryAttMaskSupported,
        ESortByName,
        dirList );
    if( err == KErrNone )
        {
        CleanupStack::PushL( dirList );
        TInt count = dirList->Count();
        INFO_1( "Found %d setting files", count );

        HBufC* file = HBufC::NewLC( KMaxFileName );
        TPtr filePtr = file->Des();
        for( TInt i = 0; i < count; i++ )
            {
            // Configure full file path
            filePtr.Copy( aFolder.Left(
                aFolder.Length() - KSettingExtension().Length() ) );
            filePtr.Append( (*dirList)[i].iName );
            INFO_1( "Parsing setting file '%S'", &(*dirList)[i].iName );

            // Parse settings
            iSettingsManager->ParseSettingsL( filePtr, aSettings );
            }

        CleanupStack::PopAndDestroy( file );
        CleanupStack::PopAndDestroy( dirList );
        }
    }

// ---------------------------------------------------------------------------
// CCFContextSourceManager::PlugInSettingsL
// ---------------------------------------------------------------------------
//
CCFContextSourceSettingArray* CCFContextSourceManager::PlugInSettingsL(
    const TUid& aImplementationUid )
    {
    // Get settings for the plugin.
    CCFContextSourceSettingArray* settings
            = CCFContextSourceSettingArray::NewLC();    // CLEANUP<< settings
    HBufC* settingFile = HBufC::NewLC( KMaxFileName );  // CLEANUP<< settingFile
    TPtr settingFilePtr = settingFile->Des();
    if( ConfigureRomSettingFolder( settingFilePtr, aImplementationUid ) )
        {
        INFO_1( "Found context source ROM setting folder '%S'", &settingFilePtr );
        // Setting folder found, parse settings.
        settingFilePtr.Append( KSettingExtension );
        ParsePlugInSettingsL( settingFilePtr, *settings );
        }

    if( ConfigureRamSettingFolder( settingFilePtr, aImplementationUid ) )
        {
        INFO_1( "Found context source RAM setting folder '%S'", &settingFilePtr );
        // Setting folder found, parse settings.
        settingFilePtr.Append( KSettingExtension );
        ParsePlugInSettingsL( settingFilePtr, *settings );
        }

    CleanupStack::PopAndDestroy( settingFile );         // CLEANUP>> settingFile
    CleanupStack::Pop( settings );                      // CLEANUP>> settings
    return settings;
    }

// ---------------------------------------------------------------------------
// CCFContextSourceManager::PlugInL
// ---------------------------------------------------------------------------
//
MCFContextSourceUpgrade* CCFContextSourceManager::PlugInL(
    const TUid& aContextSourceUid ) const
    {
    FUNC_LOG;

    // Find plugin.
    CCFContextSourcePlugIn* plugIn = NULL;
    for( TInt i = 0; i < iPlugIns.Count(); ++i )
        {
        if( iPlugIns[ i ].iImplUid == aContextSourceUid )
            {
            plugIn = iPlugIns[ i ].iPlugIn;
            break;
            }
        }
    if ( !plugIn )
        {
        User::Leave( KErrBadHandle );
        }

    // Check plugin's support for MCFContextSourceUpgrade extension.
    MCFContextSourceUpgrade* contextSourceUpgrade =
            ( MCFContextSourceUpgrade* )plugIn->Extension(
                    KCFContextSourceUpgradeUid );
    if ( !contextSourceUpgrade )
        {
        User::Leave( KErrExtensionNotSupported );
        }

    return contextSourceUpgrade;
    }

// ---------------------------------------------------------------------------
// CCFContextSourceManager::CopySettingFileL
// ---------------------------------------------------------------------------
//
void CCFContextSourceManager::CopySettingFileL( const RFile& aSettingFile,
    const TUid& aContextSourceUid,
    RThread& aClientThread )
    {
    FUNC_LOG;

    HBufC* settingFile = HBufC::NewLC( KMaxFileName );  // CLEANUP<< settingFile
    TPtr settingFilePtr = settingFile->Des();

    ConfigureRamSettingFolder( settingFilePtr, aContextSourceUid );

    HBufC* filename = HBufC::NewLC( KMaxFileName );     // CLEANUP<< filename
    TPtr filenamePtr = filename->Des();

    User::LeaveIfError( aSettingFile.Name( filenamePtr ) );

    TUid clientSid( aClientThread.SecureId() );
    settingFilePtr.Append( clientSid.Name().Mid( KUIDNameStartPos,
            KUIDNameEndPos ) );
    settingFilePtr.Append( KUnderscore );
    settingFilePtr.Append( filenamePtr );
    BaflUtils::EnsurePathExistsL( iFs, settingFilePtr );

    CleanupStack::PopAndDestroy( filename );            // CLEANUP>> filename

    CFileMan* fileMan = CFileMan::NewL( iFs );
    CleanupStack::PushL( fileMan );                     // CLEANUP<< fileMan

    TInt err = fileMan->Copy( aSettingFile, settingFilePtr );
    ERROR( err, "CCFContextSourceManager::CopySettingFileL - Copying setting file failed" );
    User::LeaveIfError( err );
    TInt clearMask = KEntryAttReadOnly | KEntryAttHidden | KEntryAttSystem;
    err = fileMan->Attribs( settingFilePtr, 0, clearMask, 0 );
    User::LeaveIfError( err );

    CleanupStack::PopAndDestroy( fileMan );             // CLEANUP>> fileMan
    CleanupStack::PopAndDestroy( settingFile );         // CLEANUP>> settingFile
    }

// ---------------------------------------------------------------------------
// CCFContextSourceManager::DeleteSettingFileL
// ---------------------------------------------------------------------------
//
void CCFContextSourceManager::DeleteSettingFileL( const TDesC& aSettingFile,
    const TUid& aContextSourceUid,
    RThread& aClientThread )
    {
    FUNC_LOG;

    HBufC* settingFile = HBufC::NewLC( KMaxFileName );  // CLEANUP<< settingFile
    TPtr settingFilePtr = settingFile->Des();

    ConfigureRamSettingFolder( settingFilePtr, aContextSourceUid );

    TUid clientSid( aClientThread.SecureId() );
    settingFilePtr.Append( clientSid.Name().Mid( KUIDNameStartPos,
            KUIDNameEndPos ) );
    settingFilePtr.Append( KUnderscore );
    settingFilePtr.Append( aSettingFile );

    TInt err = BaflUtils::DeleteFile( iFs, settingFilePtr );
    ERROR( err, "CCFContextSourceManager::DeleteSettingFileL - Deleting setting file(s) failed" );
    User::LeaveIfError( err );

    CleanupStack::PopAndDestroy( settingFile );         // CLEANUP>> settingFile
    }

//------------------------------------------------------------------------------
// CCFContextSourceManager::ReleasePlugIn
//------------------------------------------------------------------------------
//
void CCFContextSourceManager::ReleasePlugIn( CCFContextSourcePlugIn*& aPlugIn )
    {
    FUNC_LOG;

    // Deregister publisher
    TAny* extension = aPlugIn->Extension( KCFContextSourceInterfaceUid );
    if( extension )
        {
        MCFContextSource* contextSource =
            reinterpret_cast<MCFContextSource*>( extension );
        iCF.DeregisterPublisher( *contextSource );
        }
    // Unsubscribe all contexts if the plug-in is uninstalled
    extension = aPlugIn->Extension( KCFSubscriptionListenerInterfaceUid );
    if( extension )
        {
        MCFContextSubscriptionListener* interface =
            static_cast<MCFContextSubscriptionListener*>( extension );
        iCF.UnsubscribeContexts( *interface );
        }
    delete aPlugIn;
    aPlugIn = NULL;
    }
