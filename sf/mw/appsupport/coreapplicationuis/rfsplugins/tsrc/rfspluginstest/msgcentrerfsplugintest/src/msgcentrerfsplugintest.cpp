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
* Description:  Implementation of test class
*
*/


// INCLUDE FILES
#include <e32debug.h>
#include <stifparser.h>
#include <stiftestinterface.h>
#include <rfsplugin.h>
#include "trace.h"
#include "msgcentrerfsplugintest.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ---------------------------------------------------------
// CMsgCentreRfsPluginTest::CMsgCentreRfsPluginTest
// Returns: KErrNone
// ---------------------------------------------------------
CMsgCentreRfsPluginTest::CMsgCentreRfsPluginTest( CTestModuleIf& aTestModuleIf ) :
    CScriptBase( aTestModuleIf )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------
// CMsgCentreRfsPluginTest::ConstructL
// ---------------------------------------------------------
void CMsgCentreRfsPluginTest::ConstructL()
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------
// CMsgCentreRfsPluginTest::NewL
// ---------------------------------------------------------
CMsgCentreRfsPluginTest* CMsgCentreRfsPluginTest::NewL( CTestModuleIf& aTestModuleIf )
    {
    FUNC_LOG;

    CMsgCentreRfsPluginTest* self = new (ELeave) CMsgCentreRfsPluginTest( aTestModuleIf );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------
// CMsgCentreRfsPluginTest::~CMsgCentreRfsPluginTest
// ---------------------------------------------------------
CMsgCentreRfsPluginTest::~CMsgCentreRfsPluginTest()
    {
    FUNC_LOG;

    delete iPlugin;
    REComSession::FinalClose();
    }

// ---------------------------------------------------------
// CMsgCentreRfsPluginTest::LibEntryL
// Returns: Poiter to CMsgCentreRfsPluginTest class
// ---------------------------------------------------------
EXPORT_C CMsgCentreRfsPluginTest* LibEntryL( CTestModuleIf& aTestModuleIf )
    {
    FUNC_LOG;

    CMsgCentreRfsPluginTest* libEntry( CMsgCentreRfsPluginTest::NewL(aTestModuleIf) );
    return libEntry;
    }

// ---------------------------------------------------------
// CMsgCentreRfsPluginTest::RunMethodL
// ---------------------------------------------------------
TInt CMsgCentreRfsPluginTest::RunMethodL( CStifItemParser& aItem )
    {
    FUNC_LOG;

    const TStifFunctionInfo KFunctions[] =
        {
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function.
        ENTRY( "CreatePlugin", CMsgCentreRfsPluginTest::CreatePlugin ),
        ENTRY( "DestroyPlugin", CMsgCentreRfsPluginTest::DestroyPlugin ),
        ENTRY( "RestoreFactorySettings", CMsgCentreRfsPluginTest::RestoreFactorySettings ),
        ENTRY( "GetScript", CMsgCentreRfsPluginTest::GetScript ),
        ENTRY( "ExecuteCustomCommand", CMsgCentreRfsPluginTest::ExecuteCustomCommand )        
        };
    const TInt count( sizeof( KFunctions ) / sizeof( TStifFunctionInfo ) );
    TInt ret( RunInternalL( KFunctions, count, aItem ) );
    ERROR( ret, "Run internal failed" );
    return ret;
    }

// ---------------------------------------------------------
// CMsgCentreRfsPluginTest::CreatePlugin
// ---------------------------------------------------------

TInt CMsgCentreRfsPluginTest::CreatePlugin( CStifItemParser& aItem )
    {
    FUNC_LOG;

    TUint uid(0);
    TInt ret = aItem.GetNextInt( uid );
    if ( ret == KErrNone )
        {
        TRAP(ret, iPlugin = CRFSPlugin::NewL( TUid::Uid( uid ) ) );
        }
    ERROR( ret, "Create plugin failed");
    LOG_1("CreatePlugin %d", ret);
    return ret;
    }

// ---------------------------------------------------------
// CMsgCentreRfsPluginTest::DestroyPlugin
// ---------------------------------------------------------
TInt CMsgCentreRfsPluginTest::DestroyPlugin( CStifItemParser& /*aItem*/ )
    {
    FUNC_LOG;

    delete iPlugin;
    iPlugin = NULL;

    LOG_1("DestroyPlugin %d", KErrNone);
    return KErrNone;
    }

// ---------------------------------------------------------
// CMsgCentreRfsPluginTest::RestoreFactorySettings
// ---------------------------------------------------------
TInt CMsgCentreRfsPluginTest::RestoreFactorySettings( CStifItemParser& aItem )
    {
    FUNC_LOG;
    
    TInt rfsMode(0);
    TInt ret = aItem.GetNextInt( rfsMode );
    if ( ret == KErrNone )
        {
        TRAP( ret, iPlugin->RestoreFactorySettingsL(
                    static_cast<TRfsReason>( rfsMode ) ) );
        }
    ERROR( ret, "RestoreFactorySettings failed");
    LOG_1("RestoreFactorySettings %d", ret);
    return ret;
    }

// ---------------------------------------------------------
// CMsgCentreRfsPluginTest::GetScript
// ---------------------------------------------------------
TInt CMsgCentreRfsPluginTest::GetScript( CStifItemParser& aItem )
    {
    FUNC_LOG;
    
    TInt rfsMode = 0;
    TInt ret = aItem.GetNextInt( rfsMode );
    TPtrC expectedScript( KNullDesC );
    TFileName script;
    if ( ret == KErrNone )
        {
        aItem.GetNextString( expectedScript ); // Ignore error
        TRAP( ret, iPlugin->GetScriptL(
                    static_cast<TRfsReason>( rfsMode ), script ) );
        }
    if ( ret == KErrNone && script.Compare( expectedScript ) )
        {
        ret = KErrGeneral;
        }
    ERROR( ret, "GetScript failed");
    LOG_1("GetScript %d", ret);
    return ret;
    }

// ---------------------------------------------------------
// CMsgCentreRfsPluginTest::ExecuteCustomCommand
// ---------------------------------------------------------
TInt CMsgCentreRfsPluginTest::ExecuteCustomCommand( CStifItemParser& aItem )
    {
    FUNC_LOG;
    
    TInt rfsMode = 0;
    TInt ret = aItem.GetNextInt( rfsMode );
    if ( ret == KErrNone )
        {
        TPtrC cmd( KNullDesC );
        aItem.GetNextString( cmd ); // Ignore error
        TRAP( ret, iPlugin->ExecuteCustomCommandL(
                static_cast<TRfsReason>( rfsMode ), cmd ) );
        }    
    ERROR( ret, "ExecuteCustomCommand failed");
    LOG_1("ExecuteCustomCommand %d", ret);
    return ret;
    }
