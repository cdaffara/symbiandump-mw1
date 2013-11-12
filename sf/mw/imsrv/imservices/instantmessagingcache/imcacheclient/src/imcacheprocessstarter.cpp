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
* Description:  start the server
*
*/


#include "imcacheprocessstarter.h"
// logs
#include "imcachedebugtrace.h"
// system includes
#include <e32std.h>
#include <f32file.h>


// CONSTANTS
_LIT( KEka2ExeDir,"\\sys\\bin\\");
_LIT( KEka2LaunchMutexExt, "[lMtx]" );
const TInt KEka2SrvConnTries = 7;
const TInt KEka2SrvConnInitialRetryWait = 500; //MicroSeconds => 0.0005s


// ==============================================================
// ====================== HELPER CLASS ==========================
// ==============================================================

/**
 * RSessionBase accessor to give to the ProcessStarter
 * access to RSessionBase::CreateSession().
 */
class REka2SessionBaseAccessor : public RSessionBase
    {
    public: // Constructor
        inline REka2SessionBaseAccessor()
            {
            }

    public: // New functions

        /**
         * Public access to RSessionBase::CreateSession().
         */
        inline TInt CreateSession( const TDesC& aServer,
                                   const TVersion& aVersion,
                                   TInt aAsyncMessageSlots )
            {
            return RSessionBase::CreateSession( aServer,
                                                aVersion,
                                                aAsyncMessageSlots );
            }
    };


// ==============================================================
// ====================== PROCESSSTARTER ========================
// ==============================================================

// --------------------------------------------------------------
// IMCacheProcessStarter::FullExePathForClientLocation()
// --------------------------------------------------------------
//
void IMCacheProcessStarter::FullExePathForClientLocation(
    const TDesC& aExeName,
    TFileName& aFullExePath )
    {
	TRACE( T_LIT("IMCacheProcessStarter::FullExePathForClientLocation begin") );
    //Get drive (C:) where this client code is installed
        {
        TFileName tmp;
        Dll::FileName( tmp );
        aFullExePath.Copy( TParsePtrC( tmp ).Drive() );
        }

    //Build the rest from the exe path
    aFullExePath.Append( KEka2ExeDir );
    aFullExePath.Append( aExeName );
    TRACE( T_LIT("IMCacheProcessStarter::FullExePathForClientLocation end") );
    }

// --------------------------------------------------------------
// IMCacheProcessStarter::StartInstance()
// --------------------------------------------------------------
//
TInt IMCacheProcessStarter::StartInstance(
    const TDesC& aFullExePath,
    const TDesC& aCommand )
    {
    TRACE( T_LIT("IMCacheProcessStarter::StartInstance begin") );
    RMutex launchMutex;
    TInt error = KErrNotFound;

        {
        // Dynamic mutex name used to allow code share
        TName launchMutexName( TParsePtrC( aFullExePath ).Name() );
        launchMutexName.Append( KEka2LaunchMutexExt );

        // Open or Create mutex to serialize to access to server startup code.
        // (race condition safe way)
        while( error == KErrNotFound )
            {
            error = launchMutex.CreateGlobal( launchMutexName );
            if( error != KErrAlreadyExists )
                {
                break;
                }
            error = launchMutex.OpenGlobal( launchMutexName );
            }

        if( error != KErrNone )
            {
            TRACE( T_LIT("IMCacheProcessStarter::StartInstance end") );
            return error;
            }
        }


    launchMutex.Wait();

    //Serialized access
    error = IMCacheProcessStarter::DoStartInstance( aFullExePath,
                                             aCommand );

    launchMutex.Signal();
    launchMutex.Close();
	TRACE( T_LIT("IMCacheProcessStarter::StartInstance end") );
    return error;
    }

// --------------------------------------------------------------
// IMCacheProcessStarter::ConnectToServer()
// --------------------------------------------------------------
//
TInt IMCacheProcessStarter::ConnectToServer(
    const TDesC& aFullExePath,
    const TDesC& aCommand,
    RSessionBase& aSessionToConnect,
    const TDesC& aServerName,
    const TVersion& aClientVersion,
    TInt aAsyncMessageSlots )
    {
    TRACE( T_LIT("IMCacheProcessStarter::ConnectToServer begin") );
    if( aSessionToConnect.Handle() != KNullHandle )
        {
        return KErrInUse;
        }

    TInt err = KErrNone;
    TInt startupWait = KEka2SrvConnInitialRetryWait;

    //Server connect and launch loop
    for( TInt trie = 0 ; trie < KEka2SrvConnTries ; trie++ )
        {
        REka2SessionBaseAccessor acc;
        err = acc.CreateSession( aServerName,
                                 aClientVersion,
                                 aAsyncMessageSlots );

        if( err == KErrNone )
            {
            //session ownership is now on client
            aSessionToConnect = acc;
            return KErrNone;
            }

        else if( ( err == KErrNotFound ) ||
                 ( err == KErrServerTerminated ) )
            {
            //Server missing or died when connecting
            //Start a new server
            err = IMCacheProcessStarter::StartInstance( aFullExePath,
                                                 aCommand );

            //If process exist already, then all is fine
            //(some other process started it between the origical connect and launch trie)
            if( err == KErrAlreadyExists )
                {
                err = KErrNone;
                }

            //If server process start failed, bail out.
            if( err != KErrNone )
                {
                return err;
                }

            //If this is 2nd or subsequent try,
            //give some time for server to startup
            if( trie > 0 )
                {
                // Code scanner warning : Use of User::After (id:92)
                // it is required to be used here
                User::After( startupWait ); // CSI: 92 # See above
                startupWait = 2 * startupWait;
                }
            }

        else
            {
            //Server process start failed. Bail out.
            return err;
            }
        }
	TRACE( T_LIT("IMCacheProcessStarter::ConnectToServer end") );
    return err;
    }

// --------------------------------------------------------------
// IMCacheProcessStarter::DoStartServerInstance()
// --------------------------------------------------------------
//
TInt IMCacheProcessStarter::DoStartInstance(
    const TDesC& aFullExePath,
    const TDesC& aCommand )
    {
    TRACE( T_LIT("IMCacheProcessStarter::DoStartInstance begin") );
    TInt error = KErrNone;

    //Create process
    RProcess process;
    error = process.Create( aFullExePath, aCommand );

    if( error == KErrNone )
        {
       
            TRequestStatus rendezvousStatus;
            process.Rendezvous( rendezvousStatus );

            process.Resume();
            // Codescanner warning: user of User::WaitForRequest (Id:94)
            // it is required to use at server startup
            User::WaitForRequest( rendezvousStatus ); // CSI: 94 # See above
            error = rendezvousStatus.Int();

            if( process.ExitType() != EExitPending )
                {
                //Something failed in server startup
                //Force the error code to be always something
                //else than KErrNone
                if( error == KErrNone )
                    {
                    error = KErrServerTerminated;
                    }
                }
           
        }

    process.Close();
	TRACE( T_LIT("IMCacheProcessStarter::DoStartInstance end") );
    return error;
    }

// END OF FILE



