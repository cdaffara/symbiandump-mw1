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
* Description:  CFContextSourceManager class declaration.
 *
*/


#ifndef C_CFCONTEXTSOURCEMANAGER_H
#define C_CFCONTEXTSOURCEMANAGER_H

#include <cfcontextsourcesetting.h>

#include "cfecompluginmanager.h"
#include "cfstarterobserver.h"
#include "cfstateobserver.h"
#include "cfstartereventhandler.h"

class CAsyncCallBack;
class CCFContextSourcePlugIn;
class CCFContextSourceSettingsManager;
class CCFContextSourceSettingArray;
class MCFExtendedContextInterface;
class MCFContextSourceUpgrade;
class RFs;
class TCFContextSourceInfo;
class TContextSourceInitParams;
class CImplementationInformation;
class CCFContextSourceCommand;
class MCFContextSource;
class TCFClientContextSourceInfo;


/**
 *  Context source manager.
 *
 *  Context source manager is responsible for loading all the
 *  context source plug-ins that are currently installed.
 *
 *  @lib CFContextSourceManager.lib
 *  @since S60 4.0
 */
NONSHARABLE_CLASS( CCFContextSourceManager ): public CCFEComPluginManager,
public MCFStarterObserver,
public MCFStateObserver
    {
public:

    // Two phased constrcutors
    IMPORT_C static CCFContextSourceManager* NewL(
        MCFExtendedContextInterface& aCF,
        RFs& aFs );
    IMPORT_C static CCFContextSourceManager* NewLC(
        MCFExtendedContextInterface& aCF,
        RFs& aFs );

    // Destructor
    IMPORT_C virtual ~CCFContextSourceManager();

public: // New functions

    /**
     * Installs a context source setting.
     * Setting is stored for a plugin if install succeeds.
     *
     * @since S60 5.0
     * @param aSettingFile Setting file opened as EFileRead.
     * @param aContextSourceUid Implementation UID of the context source to
     *   receive setting.
     * @param aClientThread Client thread making the request.
     * @return None. Possible leave codes:
     * - KErrBadHandle, context source not found/loaded.
     * - KErrExtensionNotSupported, context source does not support
     *   installing settings.
     * - Otherwise system-wide error code.
     */
    IMPORT_C void InstallSettingL( RFile& aSettingFile,
        const TUid& aContextSourceUid,
        RThread& aClientThread );

    /**
     * Uninstalls a context source setting.
     * Setting is removed if uninstall succeeds.
     *
     * @since S60 5.0
     * @param aSettingFilename Filename (without path) that was used to
     *   install the setting.
     * @param aContextSourceUid Implementation UID of the context source.
     * @param aClientThread Client thread making the request.
     * @return None. Possible leave codes:
     * - KErrNotFound, specified setting file not found.
     * - KErrBadHandle, context source not found/loaded.
     * - KErrExtensionNotSupported, context source does not support
     *   uninstalling settings.
     * - Otherwise system-wide error code.
     */
    IMPORT_C void UninstallSettingL( const TDesC& aSettingFilename,
        const TUid& aContextSourceUid,
        RThread& aClientThread );

    /**
     * Uninstalls context source settings registered by a client.
     * Uninstalls all settings ever registered by the client for the context
     * source. Settings are removed if uninstall succeeds.
     *
     * @since S60 5.0
     * @param aContextSourceUid Implementation UID of the context source.
     * @param aClientThread Client thread making the request.
     * @return None. Possible leave codes:
     * - KErrBadHandle, context source not found/loaded.
     * - KErrExtensionNotSupported, context source does not support
     *   uninstalling settings.
     * - Otherwise system-wide error code.
     */
    IMPORT_C void UninstallSettingsL( const TUid& aContextSourceUid,
        RThread& aClientThread );

    /**
     * Handles context source command.
     *
     * @since S60 5.0
     * @param aCommand Context source command.
     * @return None. Possible leave codes:
     * - KErrBadHandle, context source not found/loaded.
     * - KErrExtensionNotSupported, context source does not support commands.
     * - Otherwise system-wide error code.
     */
    IMPORT_C void HandleContextSourceCommandL(
        const CCFContextSourceCommand& aCommand );

    /**
     * Registers client as a context source.
     * Must be called for enabling client to receive source commands.
     *
     * @since S60 5.0
     * @param aPublisher Context source interface of the publisher client.
     * @param aPublisherUid Uid (secure id) of the publisher client.
     * @return KErrNone if successful, otherwise system-wide error code.
     */
    IMPORT_C TInt RegisterClientContextSource( MCFContextSource* aPublisher,
        const TUid& aPublisherUid );

    /**
     * Deregisters client as a context source.
     * Must be called when a registered client cannot receive source commands
     * anymore, i.e. when client session is deleted.
     *
     * @since S60 5.0
     * @param aPublisher Context source interface of the publisher client.
     * @return None.
     */
    IMPORT_C void DeregisterClientContextSource(
        const MCFContextSource& aPublisher );

private: // From base classes

    // @see MCFInitialize
    void InitializePhaseL( CCFPhaseBase::TCFPhaseId aPhase );

    // from MCFStateObserver
    void UpdatePlugInsL();

    // @see MCFStarterObserver
    void SetEventHandler( MCFStarterEventHandler& aEventHandler );

private: // New methods

    // Releases plugin dependencies before deleting
    void ReleasePlugIn( CCFContextSourcePlugIn*& aPlugIn );
    
    // Call back function for asynchronous plug-in loading   
    static TInt LoaderCallBack( TAny* aLoaderInfo );
        
    // Prepares plug-in loaders
    void PrepareLoaderL( CImplementationInformation* aImplementationInfo );
        
    // Execute function for plug-in load
    void ExecuteLoaders();

private:

    CCFContextSourceManager( MCFExtendedContextInterface& aCF, RFs& aFs );
    void ConstructL();

private: // New methods

    // Loads and initialized plug-in
    CCFContextSourcePlugIn* CreateAndInitializePlugInL(
        const TUid& aImplementationUid,
        TContextSourceInitParams& aParams );

    // Init starting phase
    void InitDeviceStartingPhaseL();

    // Init loading plugins phase
    void InitLoadingPluginsPhaseL();

    // Loads plug-ins and appends it into array
    void LoadPluginL( TUid aUid, TInt aImplVersion );

    // Configures setting folder to rom
    // Returns ETrue if setting file found
    TBool ConfigureRomSettingFolder( TDes& aFolder, const TUid& aUid );

    // Configures setting folder to ram
    // Returns ETrue if setting file found
    TBool ConfigureRamSettingFolder( TDes& aFolder, const TUid& aUid );

    // Searches the plugin setting folder from setting
    // files and parses them
    void ParsePlugInSettingsL( const TDesC& aFolder,
        CCFContextSourceSettingArray& aSettings );

    // Returns parsed plugin settings for a plugin
    CCFContextSourceSettingArray* PlugInSettingsL(
        const TUid& aImplementationUid );

    /**
     * Gets context source plugin supporting MCFContextSourceUpgrade
     * interface.
     * @param aContextSourceUid Implementation UID of the context source.
     * @return Pointer to plugin's MCFContextSourceUpgrade interface.
     * Possible leave codes:
     * - KErrBadHandle, context source not found/loaded.
     * - KErrExtensionNotSupported, context source does not support
     *   MCFContextSourceUpgrade interface.
     */
    MCFContextSourceUpgrade* PlugInL( const TUid& aContextSourceUid ) const;

    /**
     * Copies setting file into RAM setting folder of a context source.
     * Overwrites previous setting file by the same name if it exists.
     * @param aSettingFile Open file handle indicating the file to be copied.
     * @param aContextSourceUid Implementation UID of the context source to
     *   receive setting.
     * @param aClientThread Client thread making the request.
     * @return None.
     */
    void CopySettingFileL( const RFile& aSettingFile,
        const TUid& aContextSourceUid,
        RThread& aClientThread );

    /**
     * Deletes setting file(s) from RAM setting folder of a context source.
     * @param aSettingFile Filename of the setting to be deleted. Use
     *   wildcards to delete more than one file.
     * @param aContextSourceUid Implementation UID of the context source
     *   whose setting file(s) is to be deleted.
     * @param aClientThread Client thread making the request.
     * @return None.
     */
    void DeleteSettingFileL( const TDesC& aSettingFile,
        const TUid& aContextSourceUid,
        RThread& aClientThread );

private: // data

    // Own: Context source settings manager
    CCFContextSourceSettingsManager* iSettingsManager;

    // Own: Context source plugin info array
    RArray<TCFContextSourceInfo> iPlugIns;

    // Ref: Context framework API
    MCFExtendedContextInterface& iCF;

    // Ref: File server session
    RFs& iFs;

    // Own: Array of context sources implemented by clients (no duplicates)
    RArray< TCFClientContextSourceInfo > iClientContextSources;
    
    // Loaders for plug-in loading
    RPointerArray<CAsyncCallBack> iLoaders;
        
    // Counter for loaded plug-ins
    TInt iLoadedCount;

        // Event handler, not own, can be NULL
        MCFStarterEventHandler* iEventHandler;
	//JNIA-849K7G Error
	TBool iUpdatePluginsAllowed;
    };

#endif
