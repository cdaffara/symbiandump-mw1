/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of Secure Formatter executable
*
*/


#include <f32file.h>

#include "selectiveformatter.h"
#include "trace.h"

_LIT_SECURE_ID( KRfsServerSid, 0x102073EA );
_LIT_SECURE_ID( KTestServerSid, 0x102073E4 );

_LIT( KThreadName, "SecureFormatter" );

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CheckAccess
// ---------------------------------------------------------------------------
//
static TInt CheckAccess()
    {
    FUNC_LOG;

    TSecureId sid = User::CreatorSecureId();
    INFO_1( "CreatorSecureId 0x%08x", sid.iId );
    if ( sid.iId == KRfsServerSid.iId || sid.iId == KTestServerSid.iId )
        {
        return KErrNone;
        }

    return KErrPermissionDenied;
    }


// ---------------------------------------------------------------------------
// DoSecureFormatL
// ---------------------------------------------------------------------------
//
static void DoSecureFormatL()
    {
    FUNC_LOG;

    // Get raw command line
    HBufC* rawCmdLine = HBufC::NewLC( User::CommandLineLength() );
    TPtr rawCmdLinePtr( rawCmdLine->Des() );
    User::CommandLine( rawCmdLinePtr );

    TLex lex( rawCmdLinePtr );
    lex.SkipSpace();
    CSelectiveFormatter* formatter = CSelectiveFormatter::NewLC( lex.NextToken() );
    lex.SkipSpace();
    formatter->FindAndDeleteL( lex.NextToken() );

    CleanupStack::PopAndDestroy( formatter );
    CleanupStack::PopAndDestroy( rawCmdLine );
    }


// ======== GLOBAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    FUNC_LOG;
    __UHEAP_MARK;

    TInt err = CheckAccess();
    ERROR( err, "Access check failed" );
    if ( err == KErrNone )
        {
        User::RenameThread( KThreadName );

        CTrapCleanup* cleanup = CTrapCleanup::New();
        TRAP( err, DoSecureFormatL() );
        delete cleanup;
        }

    __UHEAP_MARKEND;
    return err;
    }
