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
* Description:  Definitions needed for using the testdatacreator component.
*
*/


#ifndef TESTDATACREATORAPI_H
#define TESTDATACREATORAPI_H

// ---------------------------------------------------------------------------
// Command line switches
// ---------------------------------------------------------------------------

/**
* Parameter to give to the executable to make it create directory structure
* and files.
*
* Usage:
* RProcess process;
* process.Create( _L( "testdatacreator.exe" ), KSetupFilesParam );
* process.SetParameter( KFileNameIndex, "c:\\testdata\\testdata1.txt" );
* process.SetParameter( KRootDirNameIndex, "e:\\" );
*/
_LIT( KSetupFilesParam, "-f" );

/**
* Parameter to give to the executable to create the exclude list.
*
* Usage:
* RProcess process;
* process.Create( _L( "testdatacreator.exe" ), KSetupExcludeListParam );
* process.SetParameter( KFileNameIndex, "e:\\testdata\\testexcludelist1.txt" );
* process.SetParameter( KTargetFileNameIndex, "e:\\temp\\excludelist.txt" );
*
*/
_LIT( KSetupExcludeListParam, "-e" );

/**
* Parameter to give to the executable to create the exclude list.
*
* Usage:
* RProcess process;
* process.Create( _L( "testdatacreator.exe" ), KSetupSystemDriveFileParam );
* process.SetParameter( KRootDirNameIndex, "e:\\" );
*
*/
_LIT( KSetupSystemDriveFileParam, "-s" );

/**
* Parameter to give to the executable to make it verify directory structure
* and files.
*
* Usage:
* RProcess process;
* process.Create( _L( "testdatacreator.exe" ), KVerifyFilesParam );
* process.SetParameter( KFileNameIndex, "c:\\testdata\\testdata1.txt" );
* process.SetParameter( KRootDirNameIndex, "e:\\" );
*/
_LIT( KVerifyFilesParam, "-v" );

/**
* Parameter to give to the executable to clean up the exclude list.
* Usage:
* RProcess process;
* process.Create( KCleanupExcludeListParam, _L( "-c" ) );
* process.SetParameter( KFileNameIndex, "e:\\temp\\excludelist.txt" );
*
*/
_LIT( KCleanupExcludeListParam, "-c" );

// ---------------------------------------------------------------------------
// Environment slot indexes
// ---------------------------------------------------------------------------

/**
* Index of the environment slot containing a file name.
*/
const TInt KFileNameIndex = 1;
/**
* Index of the environment slot containing a file name.
*/
const TInt KTargetFileNameIndex = 2;
/**
* Index of the environment slot to use to identify the root directory for
* test data.
*/
const TInt KRootDirNameIndex = 3;


#endif // TESTDATACREATORAPI_H
