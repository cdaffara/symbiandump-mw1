/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  XIMP Framework process starter implementation.
*
*/


//  INCLUDE FILES
#include "ximpprocessstarter.h"
#include <e32std.h>
#include <f32file.h>


// CONSTANTS
_LIT( KEka2ExeDir,"\\sys\\bin\\");
_LIT( KEka2LaunchMutexExt, "[lMtx]" );
const TInt KEka2SrvConnTries = 7;               // Try to start 7 times
                                                // before giving up
const TInt KEka2SrvConnInitialRetryWait = 500;  // MicroSeconds => 0.0005s



// ==============================================================
// ====================== HELPER CLASS ==========================
// ==============================================================

/**
 * RSessionBase accessor to give to the XIMPProcessStarter
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
// XIMPProcessStarter::FullExePathForClienLocation()
// --------------------------------------------------------------
//
EXPORT_C void XIMPProcessStarter::FullExePathForClienLocation(
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
// XIMPProcessStarter::StartInstance()
// --------------------------------------------------------------
//
EXPORT_C TInt XIMPProcessStarter::StartInstance(
    const TDesC& aFullExePath,
    const TDesC& aCommand,
    const TArray< TXIMPProcessStartupParam >* aParams )
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
    error = XIMPProcessStarter::DoStartInstance( aFullExePath,
                                             aCommand,
                                             aParams );

    launchMutex.Signal();
    launchMutex.Close();

    return error;
    }


// --------------------------------------------------------------
// XIMPProcessStarter::ConnectToServer()
// --------------------------------------------------------------
//
EXPORT_C TInt XIMPProcessStarter::ConnectToServer(
    const TDesC& aFullExePath,
    const TDesC& aCommand,
    const TArray< TXIMPProcessStartupParam >* aParams,
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
            err = XIMPProcessStarter::StartInstance( aFullExePath,
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
                User::After( startupWait ); // CSI: 92 #
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
// XIMPProcessStarter::DoStartServerInstance()
// --------------------------------------------------------------
//
TInt XIMPProcessStarter::DoStartInstance(
    const TDesC& aFullExePath,
    const TDesC& aCommand,
    const TArray< TXIMPProcessStartupParam >* aParams )
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
            User::WaitForRequest( rendezvousStatus ); // CSI: 94 #
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
// XIMPProcessStarter::ApplyParameters()
// --------------------------------------------------------------
//
TInt XIMPProcessStarter::ApplyParameters(
    const TArray< TXIMPProcessStartupParam >* aParams,
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
// TXIMPProcessStartupParam::TXIMPProcessStartupParam()
// --------------------------------------------------------------
//
EXPORT_C TXIMPProcessStartupParam::TXIMPProcessStartupParam()
    : iType( EUnknown ),
      iSlot( KErrNotFound )
    {
    }


// --------------------------------------------------------------
// TXIMPProcessStartupParam::Set()
// --------------------------------------------------------------
//
void TXIMPProcessStartupParam::Set( TInt aSlot,
                                const RHandleBase& aHandle )
    {
    iType = EHandle;
    iSlot = aSlot;
    iHandle = &aHandle;
    }


// --------------------------------------------------------------
// TXIMPProcessStartupParam::Set()
// --------------------------------------------------------------
//
void TXIMPProcessStartupParam::Set( TInt aSlot,
                                const RSubSessionBase& aSubSession )
    {
    iType = ESubSession;
    iSlot = aSlot;
    iSubSession = &aSubSession;
    }


// --------------------------------------------------------------
// TXIMPProcessStartupParam::Set()
// --------------------------------------------------------------
//
void TXIMPProcessStartupParam::Set( TInt aSlot,
                                const TDesC16& aDes )
    {
    iType = EDes16;
    iSlot = aSlot;
    iDes16.Set( aDes );
    }


// --------------------------------------------------------------
// TXIMPProcessStartupParam::Set()
// --------------------------------------------------------------
//
void TXIMPProcessStartupParam::Set( TInt aSlot,
                                const TDesC8& aDes )
    {
    iType = EDes8;
    iSlot = aSlot;
    iDes8.Set( aDes );
    }


// --------------------------------------------------------------
// TXIMPProcessStartupParam::Set()
// --------------------------------------------------------------
//
EXPORT_C void TXIMPProcessStartupParam::Set( TInt aSlot,
                                TInt aInt )
    {
    iType = EInt;
    iSlot = aSlot;
    iInt = aInt;
    }


// --------------------------------------------------------------
// TXIMPProcessStartupParam::ApplyParam()
// --------------------------------------------------------------
//
TInt TXIMPProcessStartupParam::ApplyParam( RProcess& aProcess ) const
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



