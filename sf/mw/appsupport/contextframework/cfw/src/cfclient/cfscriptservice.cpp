/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFScriptService class implementation.
 *
*/


// SYSTEM INCLUDES
#include <f32file.h>
#include <bautils.h>

// USER INCLUDES
#include "cfscriptservice.h"
#include "cftrace.h"
#include "cfcommon.h"
#include "CFClientSession.h"
#include "cfscriptmessagelistener.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCFScriptService::CCFScriptService( RCFClientSession& aSession,
    MCFListener& aListener ):
    CCFServiceBase( CCFServiceBase::ECFScriptService, aSession, aListener )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CCFScriptService::ConstructL( )
    {
    FUNC_LOG;
    
    iMessageListener = CCFScriptMessageListener::NewL( iSession, iListener );
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CCFScriptService* CCFScriptService::NewL( RCFClientSession& aSession,
    MCFListener& aListener )
    {
    FUNC_LOG;

    CCFScriptService* self = CCFScriptService::NewLC( aSession, aListener );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CCFScriptService* CCFScriptService::NewLC( RCFClientSession& aSession,
    MCFListener& aListener )
    {
    FUNC_LOG;

    CCFScriptService* self = new( ELeave ) CCFScriptService(
        aSession,
        aListener );
    CleanupStack::PushL( self );
    self->ConstructL( );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CCFScriptService::~CCFScriptService( )
    {
    FUNC_LOG;
    
    delete iMessageListener;
    }

//----------------------------------------------------------------------------
// CCFScriptService::RegisterScript
//----------------------------------------------------------------------------
//
TInt CCFScriptService::RegisterScript( const TDesC& aScriptFileName )
    {
    FUNC_LOG;

    TInt scriptId = KErrNotFound;
    TRAPD( err, scriptId = ShareFileAndSendSyncL(
        aScriptFileName, ERegisterScriptByName ) );

    if( err == KErrNone )
        {
        if( !iMessageListener->IsActive() )
            {
            // Activate listening for the first time
            iMessageListener->RequestNotification();
            }
        err = scriptId;
        }
    
    return err;
    }

//----------------------------------------------------------------------------
// CCFScriptService::RegisterScript
//----------------------------------------------------------------------------
//
TInt CCFScriptService::RegisterScript( const TDesC8& aScript,
    const TDesC& aScriptFileName )
    {
    FUNC_LOG;
    
    TInt err = KErrNone;
    
    // Check arguments
    if( aScriptFileName.Length() > 0 && aScript.Length() > 0 )
        {
        TIpcArgs args( &aScript, &aScriptFileName );
        err = iSession.SendSync( ERegisterScriptByNameAndDesc, args );
        }
    else
        {
        err = KErrArgument;
        }

    return err;
    }

//----------------------------------------------------------------------------
// CCFScriptService::DeregisterScript
//----------------------------------------------------------------------------
//
TInt CCFScriptService::DeregisterScript( TInt aScriptId )
    {
    FUNC_LOG;
    
    TIpcArgs args( aScriptId );
    TInt err = iSession.SendSync( EDeregisterScriptById, args );
    
    return err;
    }

//----------------------------------------------------------------------------
// CCFScriptService::SaveScript
//----------------------------------------------------------------------------
//
TInt CCFScriptService::SaveScript( const TDesC& aScriptFileName )
    {
    FUNC_LOG;
    
    TInt scriptId = KErrNotFound;
    TRAPD( err, scriptId = ShareFileAndSendSyncL(
        aScriptFileName, ESaveScriptByName ) );
    
    if( err == KErrNone )
        {
        err = scriptId;
        }
    
    return err;
    }

//----------------------------------------------------------------------------
// CCFScriptService::SaveScript
//----------------------------------------------------------------------------
//
TInt CCFScriptService::SaveScript( const TDesC8& aScript,
    const TDesC& aScriptFileName )
    {
    FUNC_LOG;
    
    TInt err = KErrNone;
    
    // Check arguments
    if( aScriptFileName.Length() > 0 && aScript.Length() > 0 )
        {
        TIpcArgs args( &aScript, &aScriptFileName );
        err = iSession.SendSync( ESaveScriptByNameAndDesc, args );
        }
    else
        {
        err = KErrArgument;
        }
    
    return err;
    }

//----------------------------------------------------------------------------
// CCFScriptService::DeleteScript
//----------------------------------------------------------------------------
//
TInt CCFScriptService::DeleteScript(
    const TDesC& aScriptFileName )
    {
    FUNC_LOG;

    TInt err = KErrNone;
    
    // Check arguments
    if( aScriptFileName.Length() > 0 )
        {
        TIpcArgs args( &aScriptFileName );
        err = iSession.SendSync( EDeleteScriptByName, args );
        }
    else
        {
        err = KErrArgument;
        }
    
    return err;
    }

//----------------------------------------------------------------------------
// CCFScriptService::DeleteScripts
//----------------------------------------------------------------------------
//
TInt CCFScriptService::DeleteScripts()
    {
    FUNC_LOG;

    return iSession.SendSync( EDeleteScriptByUid );
    }

//----------------------------------------------------------------------------
// CCFScriptService::UpgradeRomScriptL
//----------------------------------------------------------------------------
//
TInt CCFScriptService::UpgradeRomScript( const TDesC& aScriptFileName )
    {
    FUNC_LOG;

    TRAPD( err, ShareFileAndSendSyncL( aScriptFileName, EUpgradeRomScriptByName ) );
    return err;
    }

//----------------------------------------------------------------------------
// CCFScriptService::UpgradeRomScriptDes
//----------------------------------------------------------------------------
//
TInt CCFScriptService::UpgradeRomScriptDes( const TDesC& aScriptFileName,
    const TDesC8& aScript )
    {
    FUNC_LOG;
    
    TInt err = KErrNone;
    
    // Check arguments
    if( aScriptFileName.Length() > 0 && aScript.Length() > 0 )
        {
        TIpcArgs args( &aScript, &aScriptFileName );
        err = iSession.SendSync( EUpgradeRomScriptByNameAndDesc, args );
        }
    else
        {
        err = KErrArgument;
        }

    return err;
    }

//----------------------------------------------------------------------------
// CCFScriptService::RestoreRomScript
//----------------------------------------------------------------------------
//
TInt CCFScriptService::RestoreRomScript( const TDesC& aScriptFileName )
    {
    FUNC_LOG;
    
    TInt err = KErrNone;
    
    // Check argument
    if( aScriptFileName.Length() > 0 )
        {
        TIpcArgs args( &aScriptFileName );
        err = iSession.SendSync( ERestoreRomScript, args );
        }
    else
        {
        err = KErrArgument;
        }
    
    return err;
    }

//----------------------------------------------------------------------------
// CCFScriptService::ShareFileAndSendSyncL
//----------------------------------------------------------------------------
//
TInt CCFScriptService::ShareFileAndSendSyncL( const TDesC& aScriptFileName,
    TInt aOpCode )
    {
    FUNC_LOG;
    
    RFs fs;
    CleanupClosePushL( fs );
    User::LeaveIfError( fs.Connect() );
    
    // make possible to share the file server session with the CF server
    User::LeaveIfError( fs.ShareProtected() );
    
    // open the file
    RFile scriptFile;
    CleanupClosePushL( scriptFile );
    User::LeaveIfError( scriptFile.Open( fs, aScriptFileName, EFileRead ) );
    
    // deliver the file handle to the server
    TIpcArgs args;
    User::LeaveIfError( scriptFile.TransferToServer( args, 0, 1 ) );
    
    // Register and install script to disk
    TInt err = iSession.SendSync( aOpCode, args );
    User::LeaveIfError( err );

    // Cleanup
    CleanupStack::PopAndDestroy( &scriptFile );
    CleanupStack::PopAndDestroy( &fs );

    return err;
    }

// End of file
