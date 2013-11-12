/*
* Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies). 
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


//  CLASS HEADER
#include "MT_CFContextSourceManager.h"

//  EXTERNAL INCLUDES
#include <bautils.h>
#include <ecom/ecom.h>
#include <e32property.h>
#include <EUnitMacros.h>
#include "CFContextSourceManager.h"
#include "cfcontextsourcesettingsmanager.h"
#include "cfcontextsubscription.h"
#include "cfcontextsourcesettingarray.h"

#include "testcontextsourceplugin.h"

// CONSTANTS

_LIT_SECURITY_POLICY_PASS( KPassSec );

const TInt KUIDNameStartPos = 1;
const TInt KUIDNameEndPos = 8;

const TUid KTestContextSourcePluginImplUid = {0x10002001};
const TInt KOneSecond = 1000000;
const TInt KThreeSeconds = 3 * KOneSecond;
const TInt KPluginLoadWaitDelay = KThreeSeconds; // 3 second

// Drive letter hard coded since context source plug-in settings
// can only exist in ROM
_LIT( KSettingsPathRom, "z:\\private\\10282BC4\\Settings\\" );

// Folder extension
_LIT( KFolderExt, "\\" );

// Setting file extension
_LIT( KSettingExtension, "*.xml" );

namespace
    {
    template <class T>
    class CleanupResetAndDestroy
    	{
    public:
    	inline static void PushL(T& aRef);
    private:
    	static void ResetAndDestroy(TAny *aPtr);
    	};
    template <class T>
    inline void CleanupResetAndDestroyPushL(T& aRef);


    template <class T>
    inline void CleanupResetAndDestroy<T>::PushL(T& aRef)
    	{CleanupStack::PushL(TCleanupItem(&ResetAndDestroy,&aRef));}
    template <class T>
    void CleanupResetAndDestroy<T>::ResetAndDestroy(TAny *aPtr)
    	{(STATIC_CAST(T*,aPtr))->ResetAndDestroy();}
    template <class T>
    inline void CleanupResetAndDestroyPushL(T& aRef)
    	{CleanupResetAndDestroy<T>::PushL(aRef);}
    }


// CONSTRUCTION
MT_CFContextSourceManager* MT_CFContextSourceManager::NewL()
    {
    MT_CFContextSourceManager* self = MT_CFContextSourceManager::NewLC();
    CleanupStack::Pop();

    return self;
    }

MT_CFContextSourceManager* MT_CFContextSourceManager::NewLC()
    {
    MT_CFContextSourceManager* self = new( ELeave ) MT_CFContextSourceManager();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
MT_CFContextSourceManager::~MT_CFContextSourceManager()
    {
    Teardown();
    }

// Default constructor
MT_CFContextSourceManager::MT_CFContextSourceManager()
    {
    }

// Second phase construct
void MT_CFContextSourceManager::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void MT_CFContextSourceManager::UnsubscribeContexts(
    MCFContextSubscriptionListener& /*aObserver*/ )
    {

    }

TInt MT_CFContextSourceManager::PublishContext(
    CCFContextObject& /*aContext*/,
    RThread& /*aClientThread */)
    {
    return KErrNone;
    }

TInt MT_CFContextSourceManager::PublishContext(
    CCFContextObject& /*aContext*/,
    CCFContextDataObject& /*aData*/,
    RThread& /*aClientThread */)
    {
    return KErrNone;
    }

TInt MT_CFContextSourceManager::SubscribeContext(
    CCFContextSubscription* aSubscription,
    MCFContextSubscriptionListener* /*aListener*/,
    RThread& /*aClientThread */)
    {
    // Need to delete the subscription since test suite simulates
    // CFW and CFW takes ownership of the subscripion.
    delete aSubscription;

    return KErrNone;
    }

void MT_CFContextSourceManager::UnsubscribeContext(
    CCFContextSubscription& /*aSubscription*/,
    MCFContextSubscriptionListener& /*aListener */)
    {

    }

TInt MT_CFContextSourceManager::RequestContext(
    RContextObjectArray& /*aResultBuffer*/,
    CCFContextQuery& /*aContextQuery*/,
    RThread& /*aClientThread */)
    {
    return KErrNone;
    }

TInt MT_CFContextSourceManager::DefineContext( const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType*/,
    const TSecurityPolicy& /*aSecurityPolicy */)
    {
    return KErrNone;
    }

TInt MT_CFContextSourceManager::DefineContext( const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType*/,
    const TSecurityPolicy& /*aReadWritePolicy*/,
    MCFContextSource& /*aOwner */)
    {
    return KErrNone;
    }

TInt MT_CFContextSourceManager::DefineContext( const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType*/,
    const TSecurityPolicy& /*aReadPolicy*/,
    const TSecurityPolicy& /*aWritePolicy*/,
    MCFContextSource& /*aOwner */)
    {
    return KErrNone;
    }

TInt MT_CFContextSourceManager::GetSecurityPolicy(
	const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType*/,
	TSecurityPolicy& aPolicy )
	{
	aPolicy = KPassSec;
	return KErrNone;
	}

TInt MT_CFContextSourceManager::GetReadSecurityPolicy( const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType*/,
    TSecurityPolicy& aPolicy )
    {
    aPolicy = KPassSec;
    return KErrNone;
    }

TInt MT_CFContextSourceManager::GetWriteSecurityPolicy( const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType*/,
    TSecurityPolicy& aPolicy )
    {
    aPolicy = KPassSec;
    return KErrNone;
    }

TInt MT_CFContextSourceManager::DefineContext( const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType*/,
    const TSecurityPolicy& /*aReadPolicy*/,
    const TSecurityPolicy& /*aWritePolicy*/,
    MCFContextSource* /*aPublisher*/,
    const TUid& /*aPublisherUid*/ )
    {
    return KErrNone;
    }

void MT_CFContextSourceManager::DeregisterPublisher( MCFContextSource& /*aPublisher*/ )
    {

    }

TInt MT_CFContextSourceManager::InstallContextSourceSetting( RFile& /*aSettingFile*/,
    const TUid& /*aContextSourceUid*/,
    RThread& /*aClientThread */)
    {
    return KErrNone;
    }

TInt MT_CFContextSourceManager::UninstallContextSourceSetting(
    const TDesC& /*aSettingFilename*/,
    const TUid& /*aContextSourceUid*/,
    RThread& /*aClientThread */)
    {
    return KErrNone;
    }

TInt MT_CFContextSourceManager::UninstallContextSourceSettings(
    const TUid& /*aContextSourceUid*/,
    RThread& /*aClientThread */)
    {
    return KErrNone;
    }

TInt MT_CFContextSourceManager::PublishContextFromAction(
    CCFContextObject& /*aContext*/,
    TBool /*aAutoDefine*/ )
    {
    return KErrNone;
    }

void MT_CFContextSourceManager::SetupL(  )
    {
    User::LeaveIfError( iFs.Connect() );

    iContextSourceManager = CCFContextSourceManager::NewL( *this, iFs );
    iSettingsManager = CCFContextSourceSettingsManager::NewL( iFs );
    
    iPeriodic = CPeriodic::NewL( CActive::EPriorityStandard );
    iWait = new( ELeave ) CActiveSchedulerWait;
    }

void MT_CFContextSourceManager::SetupEmptyL(  )
    {

    }

void MT_CFContextSourceManager::SetupPluginsL(  )
    {
    SetupL();

    // Disable checks
    TInt err = RProperty::Set( KContextSourceTestCategory, KDisableChecks, 0 );

    // Need to initialize ECom since otherwise it seems that we leak memory
    // Something to do with ECom initialization.. possibly TLS related issues.

    // This also initializes XML Parses because ParseSettingsL is called.
    // First time use of CMDXMLParser reserves some memory which is not
    // immediately freed.
    TBool fail = EFalse;
    TContextSourceInitParams params( *this );
    RImplInfoPtrArray list;
    CleanupResetAndDestroyPushL( list );
    REComSession::ListImplementationsL( KContextSourcePlugInInterface, list );

    TInt count = list.Count();
    CImplementationInformation* info = NULL;
    for( TInt i = 0; i < count; i++ )
        {
        info = list[i];
        CCFContextSourcePlugIn* plugIn = NULL;
        TRAP_IGNORE( plugIn = CreateAndInitializePlugInL(
            info->ImplementationUid(),
            params ) );
        delete plugIn;
        }

    // Clean up
    CleanupStack::PopAndDestroy( &list );
    }

void MT_CFContextSourceManager::Teardown(  )
    {
    delete iContextSourceManager;
    iContextSourceManager = NULL;

    delete iSettingsManager;
    iSettingsManager = NULL;
    
    delete iPeriodic;
    iPeriodic = NULL;
    
    if( iWait )
        {
        if( iWait->IsStarted() )
            {
            iWait->AsyncStop();
            }
        delete iWait;
        iWait = NULL;
        }
    
    iFs.Close();
    }

TInt MT_CFContextSourceManager::CallBack( TAny* aSelf )
    {
    MT_CFContextSourceManager* self = static_cast<MT_CFContextSourceManager*>( aSelf );
    self->iPeriodic->Cancel();
    if( self->iWait->IsStarted() )
        {
        self->iWait->AsyncStop();
        }
    return KErrNone;
    }
    
void MT_CFContextSourceManager::Wait( TTimeIntervalMicroSeconds32& aDelay )
    {
    if( iPeriodic )
        {
        if( !iPeriodic->IsActive() )
            {
            TCallBack cb( CallBack, this );
            iPeriodic->Start( aDelay, aDelay, cb );
            iWait->Start();
            }
        }            
    }

CCFContextSourcePlugIn* MT_CFContextSourceManager::CreateAndInitializePlugInL(
    const TUid& aImplementationUid,
    TContextSourceInitParams& aParams )
    {
    // Create plug-in
    CCFContextSourcePlugIn* plugIn = CCFContextSourcePlugIn::NewL(
        aImplementationUid,
        &aParams );
    CleanupStack::PushL( plugIn );

    // Handle settings
    HBufC* settingFile = HBufC::NewLC( KMaxFileName );
    TPtr settingFilePtr = settingFile->Des();
    if( ConfigureSettingFolder( settingFilePtr, aImplementationUid ) )
        {
        // Setting folder found, parse settings
        settingFilePtr.Append( KSettingExtension );
        CCFContextSourceSettingArray* settings =
            CCFContextSourceSettingArray::NewLC();
        ParsePlugInSettingsL( settingFilePtr, *settings );

        // Forward settings to plug-in
        // Plug-in takes ownership of the array
        CleanupStack::Pop( settings );
        plugIn->HandleSettingL( settings );
        }

    CleanupStack::PopAndDestroy( settingFile );

    // Define context
    plugIn->DefineContextsL();

    // Initialize
    plugIn->InitializeL();

    // Cleanup
    CleanupStack::Pop( plugIn );

    return plugIn;
    }

TBool MT_CFContextSourceManager::ConfigureSettingFolder( TDes& aFile,
    const TUid& aUid )
    {
    // Check rom
    // '[12345678]' -> 'z:\private\10282BC4\Settings\12345678\'
    aFile.Append( KSettingsPathRom );
    aFile.Append( aUid.Name().Mid(
        KUIDNameStartPos,
        KUIDNameEndPos ) );
    aFile.Append( KFolderExt );

    return BaflUtils::PathExists( iFs, aFile );
    }

void MT_CFContextSourceManager::ParsePlugInSettingsL( const TDesC& aFolder,
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
        HBufC* file = HBufC::NewLC( KMaxFileName );
        TPtr filePtr = file->Des();
        for( TInt i = 0; i < count; i++ )
            {
            // Configure full file path
            filePtr.Copy( aFolder.Left(
                aFolder.Length() - KSettingExtension().Length() ) );
            filePtr.Append( (*dirList)[i].iName );

            // Parse settings
            iSettingsManager->ParseSettingsL( filePtr, aSettings );
            }

        CleanupStack::PopAndDestroy( file );
        CleanupStack::PopAndDestroy( dirList );
        }
    }

//-----------------------------------------------------------------------------
// MT_CFContextSourceManager::MT_CCFContextSourceManager_NewLL
//-----------------------------------------------------------------------------
//
void MT_CFContextSourceManager::MT_CCFContextSourceManager_NewLL(  )
    {
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );

    CCFContextSourceManager* instance =
        CCFContextSourceManager::NewL( *this, fs );
    CleanupStack::PushL( instance );

    EUNIT_ASSERT_DESC( instance, "CCFContextSourceManager instance not created!" );

    CleanupStack::PopAndDestroy( instance );
    CleanupStack::PopAndDestroy( &fs );
    }

//-----------------------------------------------------------------------------
// MT_CFContextSourceManager::MT_CCFContextSourceManager_NewLL
//-----------------------------------------------------------------------------
//
void MT_CFContextSourceManager::MT_CCFContextSourceManager_NewLCL(  )
    {
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );

    CCFContextSourceManager* instance =
        CCFContextSourceManager::NewLC( *this, fs );

    EUNIT_ASSERT_DESC( instance, "CCFContextSourceManager instance not created!" );

    CleanupStack::PopAndDestroy( instance );
    CleanupStack::PopAndDestroy( &fs );
    }

//-----------------------------------------------------------------------------
// MT_CFContextSourceManager::MT_CCFContextSourceManager_InitializePhaseL1L
//-----------------------------------------------------------------------------
//
void MT_CFContextSourceManager::MT_CCFContextSourceManager_InitializePhaseL1L(  )
    {
    // Activate checks
    TInt err = RProperty::Set( KContextSourceTestCategory, KDisableChecks, 1 );

    // Load only first phase plug-ins (TestContextSourcePlugin)
    MCFStarterObserver* instance =
        static_cast<MCFStarterObserver*>( iContextSourceManager );
    instance->InitializePhaseL( CCFPhaseBase::ECFDeviceStarting );
    TTimeIntervalMicroSeconds32 interval( KPluginLoadWaitDelay );
    Wait( interval );
    }

//-----------------------------------------------------------------------------
// MT_CFContextSourceManager::MT_CCFContextSourceManager_InitializePhaseL2L
//-----------------------------------------------------------------------------
//
void MT_CFContextSourceManager::MT_CCFContextSourceManager_InitializePhaseL2L(  )
    {
    // Activate checks
    TInt err = RProperty::Set( KContextSourceTestCategory, KDisableChecks, 1 );

    // Load all phases
    MCFStarterObserver* instance =
        static_cast<MCFStarterObserver*>( iContextSourceManager );
    instance->InitializePhaseL( CCFPhaseBase::ECFDeviceStarting );
    TTimeIntervalMicroSeconds32 interval( KPluginLoadWaitDelay );
    Wait( interval );    
    instance->InitializePhaseL( CCFPhaseBase::ECFDeviceStarted );
    Wait( interval );
    instance->InitializePhaseL( CCFPhaseBase::ECFLoadingPlugins );
    Wait( interval );        
    instance->InitializePhaseL( CCFPhaseBase::ECFLoadingRules );
    Wait( interval );
    instance->InitializePhaseL( CCFPhaseBase::ECFCfwReady );
    Wait( interval );
    }

//-----------------------------------------------------------------------------
// MT_CFContextSourceManager::MT_CCFContextSourceManager_InstallUninstallSettingL
//-----------------------------------------------------------------------------
//
void MT_CFContextSourceManager::MT_CCFContextSourceManager_InstallUninstallSettingL(  )
    {
    MT_CCFContextSourceManager_InitializePhaseL2L();

    // Open the file.
    RFile settingFile;
    TInt err = settingFile.Open( iFs,
        _L("z:\\data\\cftestdata\\testsettings\\testcontextsourcesetting3.xml"),
        EFileRead );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when opening settings3!" );
    CleanupClosePushL( settingFile );               // CLEANUP<< settingFile

    RThread client;
    CleanupClosePushL( client );                    // CLEANUP<< client

    // 1. Install setting succeeds
    iContextSourceManager->InstallSettingL( settingFile,
        KTestContextSourcePluginImplUid,
        client );

    TBool fileExists = BaflUtils::FileExists( iFs,
        _L( "C:\\Private\\10282BC4\\Settings\\10002001\\20000fb1_testcontextsourcesetting3.xml" ) ) ;
    EUNIT_ASSERT_DESC( fileExists, "Error, cannot find settings3 file after install!" );

    // 2. Uninstall setting succeeds
    iContextSourceManager->UninstallSettingL(
        _L("testcontextsourcesetting3.xml"),
        KTestContextSourcePluginImplUid,
        client );

    fileExists = BaflUtils::FileExists( iFs,
        _L( "C:\\Private\\10282BC4\\Settings\\10002001\\20000fb1_testcontextsourcesetting3.xml" ) ) ;
    EUNIT_ASSERT_DESC( !fileExists, "Error, settings3 file exists after uninstall!" );

    // 3. Install setting leaves during UpdateSettingsL
    RFile settingFile2;
    err = settingFile2.Open( iFs,
        _L("z:\\data\\cftestdata\\testsettings\\testcontextsourcesetting7.xml"),
        EFileRead );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when opening settings7!" );
    CleanupClosePushL( settingFile2 );              // CLEANUP<< settingFile2

    EUNIT_ASSERT_SPECIFIC_LEAVE(
        iContextSourceManager->InstallSettingL( settingFile2,
            KTestContextSourcePluginImplUid,
            client ),
        KErrGeneral );

    // 4. Uninstall setting succeeds
    iContextSourceManager->UninstallSettingL(
        _L("testcontextsourcesetting7.xml"),
        KTestContextSourcePluginImplUid,
        client );

    fileExists = BaflUtils::FileExists( iFs,
        _L( "C:\\Private\\10282BC4\\Settings\\10002001\\20000fb1_testcontextsourcesetting7.xml" ) ) ;
    EUNIT_ASSERT_DESC( !fileExists, "Error, settings3 file exists after uninstall!" );

    CleanupStack::PopAndDestroy( &settingFile2 );   // CLEANUP>> settingFile2
    CleanupStack::PopAndDestroy( &client );         // CLEANUP>> client
    CleanupStack::PopAndDestroy( &settingFile );    // CLEANUP>> settingFile
    }

//-----------------------------------------------------------------------------
// MT_CFContextSourceManager::MT_CCFContextSourceManager_TestContextSourcePluginsMemoryL
//-----------------------------------------------------------------------------
//
void MT_CFContextSourceManager::MT_CCFContextSourceManager_TestContextSourcePluginsMemoryL(  )
    {
    // Disable checks
    TInt err = RProperty::Set( KContextSourceTestCategory, KDisableChecks, 0 );

    TBool fail = EFalse;
    TContextSourceInitParams params( *this );
    RImplInfoPtrArray list;
    CleanupResetAndDestroyPushL( list );
    REComSession::ListImplementationsL( KContextSourcePlugInInterface, list );

    EUNIT_PRINT( _L("Found %d plugins."), list.Count() );

    TInt count = list.Count();
    CImplementationInformation* info = NULL;
    for( TInt i = 0; i < count; i++ )
        {
        info = list[i];
        // Force ECom to clean up garbage so it doesn't affect memory consumption.
        REComSession::FinalClose();

        TInt before = 0;
        TInt after = 0;
        before = User::CountAllocCells();

        CCFContextSourcePlugIn* plugIn = NULL;
        TRAPD( err, plugIn = CreateAndInitializePlugInL(
            info->ImplementationUid(),
            params ) );
        if( err == KErrNone )
            {
            TAny* ptr = plugIn->Extension( KNullUid );
            EUNIT_ASSERT_DESC( !ptr, "Invalid extension interface returned" );
            delete plugIn;
            plugIn = NULL;

            // Force ECom to clean up garbage
            REComSession::FinalClose();

            after = User::CountAllocCells();
            if( before != after )
                {
                fail = ETrue;
                EUNIT_PRINT( _L("Plug-in [Name: %S], [Uid: 0x%x] leaked %d cells."),
                    &info->DisplayName(),
                    info->ImplementationUid().iUid,
                    after - before );
                }
            else
                {
                EUNIT_PRINT( _L("Plug-in [Name: %S], [Uid: 0x%x] did not leak memory."),
                    &info->DisplayName(),
                    info->ImplementationUid().iUid );
                }
            }
        else
            {
            EUNIT_PRINT( _L("Failed to load plug-in [Name: %S], [Uid: 0x%x]."),
                &info->DisplayName(),
                info->ImplementationUid().iUid );
            }
        }

    if( fail )
        {
        EUNIT_PRINT( _L("Memory leaks detected!") );
        EUNIT_ASSERT( EFalse );
        }
    else
        {
        EUNIT_PRINT( _L("No memory leaks detected in plug-ins.") );
        }

    // Clean up
    CleanupStack::PopAndDestroy( &list );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    MT_CFContextSourceManager,
    "CFContextSourceManager module test",
    "MODULE" )

EUNIT_TEST(
    "NewL - test ",
    "CCFContextSourceManager",
    "NewL",
    "FUNCTIONALITY",
    SetupEmptyL, MT_CCFContextSourceManager_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFContextSourceManager",
    "NewLC",
    "FUNCTIONALITY",
    SetupEmptyL, MT_CCFContextSourceManager_NewLCL, Teardown)

EUNIT_TEST(
    "InitializePhaseL 1 - test ",
    "CCFContextSourceManager",
    "InitializePhaseL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextSourceManager_InitializePhaseL1L, Teardown)

EUNIT_TEST(
    "InitializePhaseL 2 - test ",
    "CCFContextSourceManager",
    "InitializePhaseL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextSourceManager_InitializePhaseL2L, Teardown)

EUNIT_TEST(
    "Install/Uninstall Setting - test ",
    "CCFContextSourceManager",
    "Install/UninstallSettingL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextSourceManager_InstallUninstallSettingL, Teardown)

EUNIT_TEST(
    "Check plug-ins for memory leaks - test ",
    "CCFContextSourceManager",
    "Construction",
    "FUNCTIONALITY",
    SetupPluginsL, MT_CCFContextSourceManager_TestContextSourcePluginsMemoryL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
