/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  cadebug.h
 *
 */

#ifndef CADEBUG_H
#define CADEBUG_H
// INCLUDES
#include <e32std.h>
#include <flogger.h>
// Maximum formatted size resulting from a single DEBUG* call
#ifndef MAX_DEBUG_STRING_LENGTH
#define MAX_DEBUG_STRING_LENGTH 4096
#endif
#ifdef _KQTI_MENU_TRACE_PERFORMANCE
_LIT( KPerformanceLogDir, "kqti" );
_LIT( KPerformanceLogFile, "performance.txt" );
#endif

#ifdef _DEBUG
#define APPEND_TO_DEBUG_FILE
_LIT(KDebugDirName, "kqti");
_LIT(KDebugFileName, "castorage.txt");

// Load correct mymenudebug.h from module inc directory,
// define your application-specific configuration like this:
// ----------------------------------------------------------
// Debug file - debug output is disabled if the parent dir does not exist


// Replace the current debug file - if not defined appends to the file
#ifndef APPEND_TO_DEBUG_FILE
#define REPLACE_DEBUG_FILE
#endif

/**
 * Prints given descriptor to debug log file.
 * @param aData descriptor with data.
 */
static void DoOutput( TDesC8& aData );

/**
 * Prints given descriptor to debug log file.
 * @param aFmt descriptor with data.
 */
static void DebugStringNarrowL( const char* aFmt, ... )
    {
    VA_LIST args;
    VA_START(args, aFmt);

    TPtrC8 fmt( reinterpret_cast<const unsigned char *> ( aFmt ) );
    HBufC8* buf = HBufC8::NewLC( MAX_DEBUG_STRING_LENGTH );
    buf->Des().FormatList( fmt, args );
    buf->Des().Append( '\n' );
    DoOutput( *buf );
    CleanupStack::PopAndDestroy( buf );

    VA_END(args);
    }

/**
 * Prints given descriptor to debug log file.
 * @param aFmt descriptor with data.
 */
static void DebugStringWideL( const char* aFmt, ... )
    {
    VA_LIST args;
    VA_START(args, aFmt);

    TPtrC8 fmt( reinterpret_cast<const unsigned char *> ( aFmt ) );
    HBufC* fmt16 = HBufC::NewLC( fmt.Length() );
    fmt16->Des().Copy( fmt );
    HBufC* buf = HBufC::NewLC( MAX_DEBUG_STRING_LENGTH );
    buf->Des().FormatList( *fmt16, args );
    buf->Des().Append( '\n' );
    HBufC8* buf8 = HBufC8::NewLC( buf->Length() );
    buf8->Des().Copy( *buf );
    DoOutput( *buf8 );
    CleanupStack::PopAndDestroy( 3 ); // fmt16, buf, buf8

    VA_END(args);
    }

/**
 * Prints given time containinf descriptor to log file.
 * @param aBuf descriptor with time information.
 */
static void DebugBufferL( const TDesC8& aBuf )
    {
    DebugStringNarrowL( "\"%S\"", &aBuf );
    }

/**
 * Prints given time containinf descriptor to log file.
 * @param aBuf descriptor with time information.
 */
static void DebugBufferL( const TDesC& aBuf )
    {
    DebugStringWideL( "\"%S\"", &aBuf );
    }

/**
 * Prints given time to log file.
 * @param aTime time to print.
 */
static void DebugTimeL( const TTime& aTime )
    {
    TBuf<64> dateTimeString;
    _LIT(KDateString, "%E%D%X%N%Y %1 %2 %3");
    aTime.FormatL( dateTimeString, KDateString );
    DebugBufferL( dateTimeString );
    _LIT(KTimeString, "%-B%:0%J%:1%T%:2%S%.%*C4%:3%+B");
    aTime.FormatL( dateTimeString, KTimeString );
    DebugBufferL( dateTimeString );
    }

/**
 * Prints given descriptor to debug log file.
 * @param aData descriptor with data.
 */
static void DoOutput( TDesC8& aData )
    {
    RFileLogger::Write( KDebugDirName, KDebugFileName,
            EFileLoggingModeAppend, aData );
    }
#endif

#ifdef _KQTI_MENU_TRACE_PERFORMANCE

/**
 * Prints given descriptor to preformance log file.
 * @param aData descriptor with data.
 */
static void DoOutputPerf(TDesC8& aData)
    {
    RFileLogger::Write( KPerformanceLogDir,
            KPerformanceLogFile,
            EFileLoggingModeAppend,
            aData );
    }

/**
 * Formats string for performance debug output.
 * @param aFmt string for debug output.
 */
static void DebugStringNarrowPerfL( const char* aFmt, ... )
    {
    VA_LIST args;
    VA_START(args, aFmt);

    TPtrC8 fmt( reinterpret_cast<const unsigned char *>( aFmt ) );
    HBufC8* buf = HBufC8::NewLC( MAX_DEBUG_STRING_LENGTH );
    buf->Des().FormatList( fmt, args );
    buf->Des().Append('\n');

    TTime timestamp;
    timestamp.HomeTime();

    TBuf<64> dateTimeString;
    TBuf8<64> dateTimeString8;

    _LIT( KTimeString, "%:0%J%:1%T%:2%S%.%*C4%:3" );
    timestamp.FormatL( dateTimeString, KTimeString );
    dateTimeString8.Copy( dateTimeString );
    buf->Des().Insert( 0, _L8(" -> ") );
    buf->Des().Insert( 0, dateTimeString8 );

    DoOutputPerf( *buf );
    CleanupStack::PopAndDestroy( buf );

    VA_END(args);
    }
#endif

// MACROS
// If you output one or more narrow descriptors by using '%S',
//    use DEBUGSTRING8
// else if you output one or more unicode descriptors by using '%S',
//    use DEBUGSTRING16
// else
//    use DEBUGSTRING
//
// Narrow and unicode cannot be mixed in a single DEBUGSTRINGx call.

#ifdef _DEBUG
#define DEBUGINIT() DebugInit()
#define DEBUGINITSUSPENDED() DebugInit(ETrue)
#define DEBUGENABLE() SetDebugEnabled(ETrue)
#define DEBUGDISABLE() SetDebugEnabled(EFalse)
#define DEBUGSUSPEND() SetDebugSuspended(ETrue)
#define DEBUGCONTINUE() SetDebugSuspended(EFalse)
#define DEBUG(x) TRAP_IGNORE( DebugStringNarrowL x )
#define DEBUG8(x) TRAP_IGNORE( DebugStringNarrowL x )
#define DEBUG16(x) TRAP_IGNORE( DebugStringWideL x )
#define DEBUGBUFFER(x) TRAP_IGNORE( DebugBufferL x )
#define DEBUGTIME(x) TRAP_IGNORE( DebugTimeL x )
#else
#define DEBUGINIT()
#define DEBUGINITSUSPENDED()
#define DEBUGENABLE()
#define DEBUGDISABLE()
#define DEBUGSUSPEND()
#define DEBUGCONTINUE()
#define DEBUG(x)
#define DEBUG8(x)
#define DEBUG16(x)
#define DEBUGBUFFER(x)
#define DEBUGTIME(x)
#endif

#ifdef _KQTI_MENU_TRACE_PERFORMANCE
#define MMPERF(x) TRAP_IGNORE( DebugStringNarrowPerfL x )
#else
#define MMPERF(x)
#endif

#endif // MENUDEBUG_H
