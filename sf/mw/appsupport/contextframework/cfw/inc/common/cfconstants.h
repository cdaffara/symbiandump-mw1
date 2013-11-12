/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ?Description
*
*/


#ifndef __CFCONSTANTS_H__
#define __CFCONSTANTS_H__

// INCLUDES
#include <e32base.h>

// CONSTANTS
// The name of the context database
_LIT( KContextDbName, "context.db" );

// The location of the context database
_LIT( KCEpoc32exData, "C:\\epoc32ex\\data\\" );

/**
* Defines the string for carriage return & line feed pair (for ToStringLC()
* method).
*/
_LIT( KCrLf, "\r\n" );

/**
* Defines the tabulator literal for indenting string output (for ToStringLC()
* method).
*/
//_LIT( KTab, "\t" );

// Output format of timestamp.
_LIT( KTimeOutputFormat, "%/0%1%/1%2%/2%3%/3 %-B%:0%J%:1%T%:2%S.%C%+B" );


// The size of the SQL buffer in the Silo
const TInt KCFContextSiloSQLBufferSize = 500;

/**
* Context's confidence change magnitude required for store decision.
* When only the confidence of the context has changed, it must change at least
* the amount of this constant to be stored by the data manager.
* Otherwise, the context is not stored!
*/
const TInt KCFContextSiloRequiredConfidenceChangeMagnitudeOfContext = 20;

/**
* Treshold value defining when the database should be compacted.
* This should be compared against a counter for database operations that change
* the contents of the database, i.e. insert & update. When the treshold value
* is reached, the database should be compacted.
*/
const TInt KCFContextSiloDatabaseCompactTreshold = 100;

/**
* Default value of the confidence
*/
const TInt KCFContextObjectDefaultConfidence = 100;

/**
* Default value of the visibility
*/
const TInt KCFContextObjectDefaultVisibility = 0;

/**
* Default value of the priority
*/
const TInt KCFContextObjectDefaultPriority = 0;


// ScriptEngineSession
_LIT8( KSeparatorChar, "-" );

// SEServer and ScriptEngine
_LIT( KCFScriptEngineDataFolderPath, "\\System\\Apps\\ScriptEngine\\" );

// SEServer
_LIT( KCFSettingsFileName,           "SESettings.dat"                                   );
_LIT( KEmulatorSettingsFilename,     "C:\\System\\Apps\\ScriptEngine\\SESettings.dat"   );
_LIT( KEmulatorServerFilename,       "C:\\System\\Programs\\SEServer.dll"               );



#endif // __CFCONSTANTS_H__

// End of File
