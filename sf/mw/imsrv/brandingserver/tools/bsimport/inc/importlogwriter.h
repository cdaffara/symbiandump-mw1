/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:   Log writer macros
*
*/




/*
*
*  Description:
*
*     Logging tools. Uniforms style to write debug data to
*     file with RFileLogger.
*
*     Usage:
*
*       following directory must be manually done before logging
*       (no directory, not logs).
*           Epoc32\Winscw\c\logs\bsimport\
*
*
*     2.Printing:
*
*       // normal string                                  output:
*       IMPORT_DP( D_IMPORT_LIT( "Some text." ) );                >> CHAT: Some text.
*       IMPORT_DP( D_PLAIN_LIT( "Some text." ) );               >> Some text.
*       IMPORT_DP_TXT( "Some text." );                          >> CHAT: Some text.
*
*       // string with variables
*       TInt index( 99 );
*       _LIT( KExample, "Example" );
*       IMPORT_DP( D_IMPORT_LIT( "Some text: %d" ), 100 );        >> CHAT: Some text: 100
*       IMPORT_DP( D_IMPORT_LIT( "Some text: %d" ), index );      >> CHAT: Some text: 99
*       IMPORT_DP( D_IMPORT_LIT( "Some text: %S" ), &KExample );  >> CHAT: Some text: Example
*
*       IMPORT_DP( D_PLAIN_LIT( "Some text: %d" ), 100 );       >> Some text: 100
*       IMPORT_DP( D_PLAIN_LIT( "Some text: %d" ), index );     >> Some text: 99
*       IMPORT_DP( D_PLAIN_LIT( "Some text: %S" ), &KExample ); >> Some text: Example
*
*
*     3.Known issues:
*
*       - If you use macros from .mmp file remember to abld makefile <target> to
*         change flags from project.
*       - In statements like IMPORT_DP( D_IMPORT_LIT("Some text: %S"), &KExample );
*         parameters causes still some code to generated in release builds.
*         Propably it is best to #ifdef all debugprint blocks
*         with CHAT_ENABLE_DEBUG_PRINT statement.
*
* ============================================================================
*/


#ifndef __IMPORTLOGWRITER_H__
#define __IMPORTLOGWRITER_H__


// no include files if no debug printing --> faster compile time
// INCLUDES
#include <e32std.h>
#include <e32svr.h>
#include <flogger.h>

/**
 * Depending if the build is UNICODE or not, define the
 * helper macros that insert CHAT prefix.
 */
#ifdef _UNICODE
    //#define IMPORT_TOKEN_PASTING( s ) L##s
    //#define IMPORT_TO_UNICODE( s ) CHAT_TOKEN_PASTING( s )
    #define IMPORT_DEBUG_STR( m ) L##m
#else
    #define IMPORT_DEBUG_STR "CHAT: "
#endif // _UNICODE


/**
 * Helper macro for defining debug strings with plain debug text.
 */
#define D_PLAIN_LIT( s ) _L( s )    // CSI: 78 # Debug print


/**
 * Helper macro for defining debug strings with "CHAT:" prefix.
 */
#define D_IMPORT_LIT( s ) TPtrC( ( const TText * ) L ## s )


/**
 * Method to handle file writing
 */
inline void ImportDebugWriteFormat( TRefByValue<const TDesC> aFmt,... )
    {
    _LIT( KDir,     "BSImport" );
    _LIT( KName,    "Importlog.txt" );

    // take the ellipsis parameters
    VA_LIST args;
    VA_START( args,aFmt );
    RFileLogger::WriteFormat( KDir, KName, EFileLoggingModeAppend,
    	aFmt, args );
    VA_END( args );
    }

/**
 * Actual debug printters.
 * Output to log file.
 */
#define IMPORT_DP ImportDebugWriteFormat
#define IMPORT_DP_TXT( s ) ImportDebugWriteFormat( D_IMPORT_LIT( s ) )

#endif  // __IMPORTLOGWRITER_H__


//  End of File
