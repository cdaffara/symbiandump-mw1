/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:        ?Description
*
*/









// INCLUDE FILES
#include <Stiftestinterface.h>
#include "ProEngWrapAPI.h"
#include <SettingServerClient.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngWrapAPI::CProEngWrapAPI
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CProEngWrapAPI::CProEngWrapAPI( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProEngWrapAPI::ConstructL()
    {
    //Read logger settings to check whether test case name is to be
    //appended to log file name.
    RSettingServer settingServer;
    iSchedulerUtility = CSchedulerUtility::NewL(); 
    User::LeaveIfError( iFs.Connect() );
    TInt ret = settingServer.Connect();
    if(ret != KErrNone)
        {
        User::Leave(ret);
        }
    // Struct to StifLogger settigs.
    TLoggerSettings loggerSettings; 
    // Parse StifLogger defaults from STIF initialization file.
    ret = settingServer.GetLoggerSettings(loggerSettings);
    if(ret != KErrNone)
        {
        User::Leave(ret);
        } 
    // Close Setting server session
    settingServer.Close();

    TFileName logFileName;
    
    if(loggerSettings.iAddTestCaseTitle)
        {
        TName title;
        TestModuleIf().GetTestCaseTitleL(title);
        logFileName.Format(KProEngWrapAPILogFileWithTitle, &title);
        }
    else
        {
        logFileName.Copy(KProEngWrapAPILogFile);
        }

    iLog = CStifLogger::NewL( KProEngWrapAPILogPath, 
                          logFileName,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
    
    
    SendTestClassVersion();
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProEngWrapAPI* CProEngWrapAPI::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CProEngWrapAPI* self = new (ELeave) CProEngWrapAPI( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
   
 //   CTestModuleIf &testMod = TestModuleIf();
 //   testMod.SetBehaviour(ETestLeaksMem);

    return self;

    }

// Destructor
CProEngWrapAPI::~CProEngWrapAPI()
    { 

    // Delete resources allocated from test methods
    Delete();
   //CSchedulerUtility::Release(); 
    if(iSchedulerUtility)
        {
        delete iSchedulerUtility;
        iSchedulerUtility = NULL;
        }

   iFs.Close() ;  
       
    // Delete logger
    delete iLog; 

    }

//-----------------------------------------------------------------------------
// CProEngWrapAPI::SendTestClassVersion
// Method used to send version of test class
//-----------------------------------------------------------------------------
//
void CProEngWrapAPI::SendTestClassVersion()
	{
	TVersion moduleVersion;
	moduleVersion.iMajor = TEST_CLASS_VERSION_MAJOR;
	moduleVersion.iMinor = TEST_CLASS_VERSION_MINOR;
	moduleVersion.iBuild = TEST_CLASS_VERSION_BUILD;
	
	TFileName moduleName;
	moduleName = _L("ProEngWrapAPI.dll");

	TBool newVersionOfMethod = ETrue;
	TestModuleIf().SendTestModuleVersion(moduleVersion, moduleName, newVersionOfMethod);
	
    TestModuleIf().SetBehavior(CTestModuleIf::ETestLeaksHandles);
	}

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point.
// Returns: CScriptBase: New CScriptBase derived object
// -----------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( 
    CTestModuleIf& aTestModuleIf ) // Backpointer to STIF Test Framework
    {

    return ( CScriptBase* ) CProEngWrapAPI::NewL( aTestModuleIf );

    }


//  End of File
