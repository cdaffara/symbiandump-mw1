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
* Description:  GSServer is responsible for initializing the TV
*                driver during the boot and always when the cable is connected.
*
*/


// INCLUDE FILES
#include "GSServerClient.h"
#include <e32svr.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS
const TInt KMaxConnectionAttempts = 2;
const TUid KTVConfigSettingsServerUid3 = { 0x10281F09 };
_LIT( KGSServerName,"gsserver" );
_LIT( KGSServerImg,"gsserver" );		// DLL/EXE name

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS


// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// StartServer Standard server startup code.
// Returns: KErrNone if all is fine, system wide error codes otherwise.
// -----------------------------------------------------------------------------
//
static TInt StartServer()
    {
    const TUidType serverUid( KNullUid, KNullUid, KTVConfigSettingsServerUid3 );

    // We just create a new server process. Simultaneous
    // launching of two such processes should be detected when the second one
    // attempts to create the server object, failing with KErrAlreadyExists.
    RProcess server;
    //TInt r=server.Create( KTVConfigSettingsServerImg,KNullDesC,serverUid );
    TInt r=server.Create( KGSServerImg, KNullDesC );

    if ( r != KErrNone )
      {
      return r;
      }
    TRequestStatus stat;
    server.Rendezvous( stat );
    if ( stat != KRequestPending )
        {
        server.Kill( 0 );    // abort startup
        }
    else
        {
        server.Resume();  // logon OK - start the server
        }
    User::WaitForRequest( stat );    // wait for start or death
    // we can't use the 'exit reason' if the server panicked as this
    // is the panic 'reason' and may be '0' which cannot be distinguished
    // from KErrNone
    r = ( server.ExitType() == EExitPanic ) ? KErrGeneral : stat.Int();
    server.Close();
    return r;
    }


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// RGSServerClient::RGSServerClient
// Empty default constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C RGSServerClient::RGSServerClient()
    {
    }


// -----------------------------------------------------------------------------
// RGSServerClient::Connect
// Connect to the server, standard stuff.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RGSServerClient::Connect()
    {
    //  Try twice, first try to create session, if unsuccesfull, start server
    //  and the try again
    TInt retry = KMaxConnectionAttempts;
    for ( ; ; )
        {
        TInt r = CreateSession( KGSServerName, 
                                TVersion( 1, 0, 0 ), 0 );
        if ( r != KErrNotFound && r != KErrServerTerminated )
            {
            return r;
            }
        if ( --retry == 0 )
            {
            return r;
            }
        r = StartServer();
        if ( r != KErrNone && r != KErrAlreadyExists )
            {
            return r;
            }
        }
    }


// -----------------------------------------------------------------------------
// RGSServerClient::Close
// Close the connection to the server, just call base class.
// -----------------------------------------------------------------------------
//
EXPORT_C void RGSServerClient::Close()
    {
    RSessionBase::Close();
    }


//  End of File
