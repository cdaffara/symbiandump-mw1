/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*		Implements the client interface to the RFS server.
*
*
*/


// INCLUDE FILES

#include "rfsServer.h"
#include "rfsClient.h"
#include "RfsTraces.h"

// CONSTANTS
const TUint KRfsDefaultMessageSlots = 1;
const TInt KRfsConnectionTries = 2;

// ================= MEMBER FUNCTIONS ==========================================

EXPORT_C RRfsClient::RRfsClient()
    {}

// -----------------------------------------------------------------------------
// RRfsClient::Connect()
// Connects to the  server - default number of message slots = 1
// -----------------------------------------------------------------------------

EXPORT_C TInt RRfsClient::Connect()
    {
    TRACES("RRfsClient::Connect()");

    // Connect to server and make sure this can't cause 2 servers launch
    // if 2 clients are trying to connect simultaneously.

    TInt err( KErrNone );

    for ( TInt tries = 0; tries < KRfsConnectionTries ; tries++ )
        { 
        err = CreateSession( KRfsServerName, Version(), KRfsDefaultMessageSlots );

        // Break if connected to existing server or if the problem is
        // other than missing server.

        if ( err == KErrNone || 
           ( err != KErrNotFound && err != KErrServerTerminated ) )
            {
            break;
            }

        err = StartServer();    // try start a new server

        // If server launched ok or someone else got to launch it first,
        // try connect again.

        if ( err != KErrNone && err != KErrAlreadyExists )
            {
            break; // server not launched: don't cycle round again
            }
        }

    return err;
    }

// -----------------------------------------------------------------------------
// RRfsClient::RunScriptInDescriptor()
// -----------------------------------------------------------------------------

EXPORT_C TInt RRfsClient::RunScriptInDescriptor( const TDesC& aScript )
    {
    TRACES("RRfsClient::RunScriptInDescriptor()");

    // 1. message parameter is the script descriptor address
    // 2. message parameter is the script descriptor length

    TRequestStatus status;
    SendReceive(ERfsServRunScriptInDescriptor, TIpcArgs(&aScript, aScript.Length()), status);

    User::WaitForRequest( status );
    return status.Int();
    }

// -----------------------------------------------------------------------------
// RRfsClient::RunScriptInFile()
// -----------------------------------------------------------------------------

EXPORT_C TInt RRfsClient::RunScriptInFile( const TDesC& aFileName )
    {
    TRACES("RRfsClient::RunScriptInFile()");

    // 1. message parameter is the file name descriptor address
    // 2. message parameter is the file name descriptor length

    TRequestStatus status;

    SendReceive(ERfsServRunScriptInFile, TIpcArgs(&aFileName, aFileName.Length()), status);

    User::WaitForRequest( status );
    return status.Int();
    }

// -----------------------------------------------------------------------------
// RRfsClient::PerformRfs()
// -----------------------------------------------------------------------------
     
EXPORT_C TInt RRfsClient::PerformRfs( TRfsType aType, TBool aIncludeNosRfs )
    {
    TRACES("RRfsClient::PerformRfs()");

    // 1. message parameter is the rfs type ( normal or deep level )

    TRequestStatus status;

    SendReceive(ERfsServPerformRfs, TIpcArgs(aType, aIncludeNosRfs), status);
    User::WaitForRequest( status );
    
    // execute init script after deep level RFS (which formats FFS)

    TInt initStatus( KErrNone );

    if ( aType == ERfsDeep )
        {
        TBuf< KMaxFileName > filename;

#ifdef RD_MULTIPLE_DRIVE_ENABLERS
        TInt drive;
        DriveInfo::GetDefaultDrive( DriveInfo::EDefaultRom, drive );
        TChar driveChar;
        RFs fs;
        TInt err = fs.Connect();
        if (err != KErrNone)
        	return err;
        fs.DriveToChar( drive, driveChar );
        fs.Close();
        filename.Append(driveChar);
        filename.Append(KRfsDriveMarker);
        filename.Append(KRfsScriptLocation);
        filename.Append(KRfsScriptFileBody);
#else
        filename.Copy( KRfsScriptFileStartRom );
#endif //RD_MULTIPLE_DRIVE_ENABLERS
        filename.Append( KRfsInitScriptIdentifier );
        filename.Append( KRfsScriptFileExtension );

        initStatus = RunScriptInFile( filename );
        }

    TInt ret( KErrNone );

    // If an error code is returned by one of the scripts, return it.

    if ( status.Int() < 0 )
        {
        ret = status.Int();
        }

    else if ( initStatus < 0 )
        {
        ret = initStatus;
        }

    // Otherwise, sum up the error counts.

    else
        {
        ret = status.Int() + initStatus;
        }
    TRACES1("RRfsClient::PerformRfs(): End: returns %d", ret);
    return ret;
    }

// -----------------------------------------------------------------------------
// RRfsClient::PerformPostEikonRfs()
// -----------------------------------------------------------------------------

EXPORT_C TInt RRfsClient::PerformPostEikonRfs( TRfsType aType )
    {
    TRACES("RRfsClient::PerformPostEikonRfs()");
    // At the moment, deep RFS can be done completely before starting up Eikon.

    if ( aType != ERfsNormal )
        {
        return KErrNone;
        }

    // 1. message parameter is the rfs type ( normal or deep level )

    TRequestStatus status;
    SendReceive(ERfsServPerformPostEikonRfs, TIpcArgs(aType), status);

    User::WaitForRequest( status );
    return status.Int();
    }

// -----------------------------------------------------------------------------
// RRfsClient::Close()
// Closes the session.
// -----------------------------------------------------------------------------

EXPORT_C void RRfsClient::Close()
    {
    TRACES("RRfsClient::Close()");
    RSessionBase::Close();  // deletes session object
    }

// -----------------------------------------------------------------------------
// RRfsClient::Version()
// Returns the client side version number.
// -----------------------------------------------------------------------------

TVersion RRfsClient::Version() const
    {
    TRACES("RRfsClient::Version()");
    // client side version is set same as the server side version here, since
    // both are implemented in the same DLL
    
    return TVersion( KRfsServMajorVersionNumber, KRfsServMinorVersionNumber,
                     KRfsServBuildVersionNumber );
    }

// End of File
