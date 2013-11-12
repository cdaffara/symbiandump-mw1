/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  bsprocessstarter.cpp
*
*/

//  INCLUDE FILES
#include "bsprocessstarter.h"
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
// BSProcessStarter::FullExePathForClienLocation()
// --------------------------------------------------------------
//
void BSProcessStarter::FullExePathForClienLocation(
    const TDesC& aExeName,
    TFileName& aFullExePath )
    {
    //Get drive (C:) where this client code is installed
        {
        TFileName tmp;
        Dll::FileName( tmp );
        aFullExePath.Copy( TParsePtrC( tmp ).Drive() );
        }

    //Build the rest from the exe path
    aFullExePath.Append( KEka2ExeDir );
    aFullExePath.Append( aExeName );
    }



// --------------------------------------------------------------
// BSProcessStarter::StartInstance()
// --------------------------------------------------------------
//
TInt BSProcessStarter::StartInstance(
    const TDesC& aFullExePath,
    const TDesC& aCommand,
    const TArray< TProcessStartupParam >* aParams )
    {
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
            return error;
            }
        }


    launchMutex.Wait();

    //Serialized access
    error = BSProcessStarter::DoStartInstance( aFullExePath,
                                             aCommand,
                                             aParams );

    launchMutex.Signal();
    launchMutex.Close();

    return error;
    }



// --------------------------------------------------------------
// BSProcessStarter::ConnectToServer()
// --------------------------------------------------------------
//
TInt BSProcessStarter::ConnectToServer(
    const TDesC& aFullExePath,
    const TDesC& aCommand,
    const TArray< TProcessStartupParam >* aParams,
    RSessionBase& aSessionToConnect,
    const TDesC& aServerName,
    const TVersion& aClientVersion,
    TInt aAsyncMessageSlots )
    {
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
            err = BSProcessStarter::StartInstance( aFullExePath,
                                                 aCommand,
                                                 aParams );

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

    return err;
    }



// --------------------------------------------------------------
// BSProcessStarter::DoStartServerInstance()
// --------------------------------------------------------------
//
TInt BSProcessStarter::DoStartInstance(
    const TDesC& aFullExePath,
    const TDesC& aCommand,
    const TArray< TProcessStartupParam >* aParams )
    {
    TInt error = KErrNone;

    //Create process
    RProcess process;
    error = process.Create( aFullExePath, aCommand );

    if( error == KErrNone )
        {
        //Set process startup parameters
        error = ApplyParameters( aParams, process );

        //And execute the process and wait it's startup
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
        }

    process.Close();

    return error;
    }



// --------------------------------------------------------------
// BSProcessStarter::ApplyParameters()
// --------------------------------------------------------------
//
TInt BSProcessStarter::ApplyParameters(
    const TArray< TProcessStartupParam >* aParams,
    RProcess& aProcess )
    {
    TInt error = KErrNone;
    if( aParams )
        {
        const TInt paramCount = aParams->Count();
        for( TInt ix = 0; ix < paramCount; ix++ )
            {
            error = (*aParams)[ ix ].ApplyParam( aProcess );
            if( error != KErrNone )
                {
                break;
                }
            }
        }

    return error;
    }



// ==============================================================
// ================== TPROCESSSTARTUPPARAM ======================
// ==============================================================

// --------------------------------------------------------------
// TProcessStartupParam::TProcessStartupParam()
// --------------------------------------------------------------
//
TProcessStartupParam::TProcessStartupParam()
    : iType( EUnknown ),
      iSlot( KErrNotFound )
    {
    }


// --------------------------------------------------------------
// TProcessStartupParam::Set()
// --------------------------------------------------------------
//
void TProcessStartupParam::Set( TInt aSlot,
                                const RHandleBase& aHandle )
    {
    iType = EHandle;
    iSlot = aSlot;
    iHandle = &aHandle;
    }


// --------------------------------------------------------------
// TProcessStartupParam::Set()
// --------------------------------------------------------------
//
void TProcessStartupParam::Set( TInt aSlot,
                                const RSubSessionBase& aSubSession )
    {
    iType = ESubSession;
    iSlot = aSlot;
    iSubSession = &aSubSession;
    }


// --------------------------------------------------------------
// TProcessStartupParam::Set()
// --------------------------------------------------------------
//
void TProcessStartupParam::Set( TInt aSlot,
                                const TDesC16& aDes )
    {
    iType = EDes16;
    iSlot = aSlot;
    iDes16.Set( aDes );
    }


// --------------------------------------------------------------
// TProcessStartupParam::Set()
// --------------------------------------------------------------
//
void TProcessStartupParam::Set( TInt aSlot,
                                const TDesC8& aDes )
    {
    iType = EDes8;
    iSlot = aSlot;
    iDes8.Set( aDes );
    }


// --------------------------------------------------------------
// TProcessStartupParam::Set()
// --------------------------------------------------------------
//
void TProcessStartupParam::Set( TInt aSlot,
                                TInt aInt )
    {
    iType = EInt;
    iSlot = aSlot;
    iInt = aInt;
    }


// --------------------------------------------------------------
// TProcessStartupParam::ApplyParam()
// --------------------------------------------------------------
//
TInt TProcessStartupParam::ApplyParam( RProcess& aProcess ) const
    {
    switch( iType )
        {
        case EHandle: return aProcess.SetParameter( iSlot, *iHandle );
        case ESubSession: return aProcess.SetParameter( iSlot, *iSubSession );
        case EDes16: return aProcess.SetParameter( iSlot, iDes16 );
        case EDes8: return aProcess.SetParameter( iSlot, iDes8 );
        case EInt: return aProcess.SetParameter( iSlot, iInt );
        }

    return KErrUnknown;
    }


// END OF FILE



