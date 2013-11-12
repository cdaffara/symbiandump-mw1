/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Drive eject handler implementation
 *
*/



// INCLUDE FILES
#include <e32std.h>
#include "sysapdriveejecthandler.h"
#include "sysapdrivelist.h"
#include "SysApAppUi.h"
#include "SysAp.hrh"

// CONSTANTS
const TInt KSecondsToMicro = 1000000; // seconds to microseconds multiplier

// ============================ MEMBER FUNCTIONS =============================
// ---------------------------------------------------------------------------
// CSysApDriveEjectHandler::CSysApDriveEjectHandler
// ---------------------------------------------------------------------------
//
CSysApDriveEjectHandler* CSysApDriveEjectHandler::NewL(
        CSysApDriveList& aSysApDriveList,
        CSysApAppUi& aSysApAppUi,
        RFs& aFs )
    {
    CSysApDriveEjectHandler* self = new ( ELeave ) CSysApDriveEjectHandler(
        aSysApDriveList, aSysApAppUi );
    CleanupStack::PushL( self );
    self->ConstructL( aFs );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CSysApDriveEjectHandler::CSysApDriveEjectHandler
// ---------------------------------------------------------------------------
//
CSysApDriveEjectHandler::CSysApDriveEjectHandler(
        CSysApDriveList& aSysApDriveList,
        CSysApAppUi& aSysApAppUi ) :
    iSysApDriveList( aSysApDriveList ),
    iSysApAppUi( aSysApAppUi ),
    iDriveToEject( KErrNotFound )
    {
    }

// ---------------------------------------------------------------------------
// CSysApDriveEjectHandler::ConstructL
// ---------------------------------------------------------------------------
//
void CSysApDriveEjectHandler::ConstructL( RFs& aFs )
    {
    iDiskNotifyHandler = CDiskNotifyHandler::NewL( *this, aFs );
    }

// ---------------------------------------------------------------------------
// CSysApDriveEjectHandler::~CSysApDriveEjectHandler
// ---------------------------------------------------------------------------
//
CSysApDriveEjectHandler::~CSysApDriveEjectHandler()
    {
    TRACES( RDebug::Print(
        _L( "CSysApDriveEjectHandler::~CSysApDriveEjectHandler" ) ) );

    delete iDiskNotifyHandler;
    }

// ---------------------------------------------------------------------------
// CSysApDriveEjectHandler::StartEject
// ---------------------------------------------------------------------------
//
void CSysApDriveEjectHandler::StartEject()
    {
    TBool isOngoing( iDriveToEject != KErrNotFound );

    TRACES( RDebug::Print(
        _L( "CSysApDriveEjectHandler::StartEject: ongoing: %d" ),
        isOngoing ) );

    if ( isOngoing )
        {
        return;
        }
    TRAPD( err, DoStartEjectL() );
    if ( err != KErrNone )
        {
        StopEject( err );
        }
    }

// ---------------------------------------------------------------------------
// CSysApDriveEjectHandler::DoStartEjectL()
// ---------------------------------------------------------------------------
//  
TBool CSysApDriveEjectHandler::DoStartEjectL()
    {
    CSysApDriveList::TDriveEjectType ejectType;
    iDriveToEject = iSysApDriveList.DriveToEject( ejectType );

    TRACES( RDebug::Print(
        _L( "CSysApDriveEjectHandler::DoStartEjectL: drive: %d" ),
        iDriveToEject ) );

    if ( iDriveToEject == KErrNotFound )
        {
        return EFalse;
        }

    iSysApAppUi.ShowEjectWaitNoteL( iDriveToEject );
    User::LeaveIfError( iDiskNotifyHandler->StartDismount( 
        iDriveToEject, KMaxExitTimeInHotSwap * KSecondsToMicro ) );
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CSysApDriveEjectHandler::CompleteDismount
// ---------------------------------------------------------------------------
//
TBool CSysApDriveEjectHandler::CompleteDismount( TInt aDrive )
    {
    TRACES( RDebug::Print(
        _L( "CSysApDriveEjectHandler::CompleteDismount: aDrive: %d" ),
        aDrive ) );

    if ( aDrive != KErrNotFound )
        {
        iSysApDriveList.ResetDriveToEject( aDrive );
        }
    
    TBool ret( EFalse );
    TRAPD( err, ret = DoStartEjectL() );
    if ( err != KErrNone )
        {
        StopEject( err );
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// CSysApDriveEjectHandler::StopEject
// ---------------------------------------------------------------------------
//
void CSysApDriveEjectHandler::StopEject( TInt aError )
    {
    if ( aError != KErrNone )
        {
        TRACES( RDebug::Print(
            _L( "CSysApDriveEjectHandler::StopEject: error: %d" ),
            aError ) );
        }
    // Ensure that the wait note without softkeys gets closed
    iSysApDriveList.ResetDrivesToEject();
	iDriveToEject = KErrNotFound;
    iDiskNotifyHandler->CancelStartedDismount();
    }

// ---------------------------------------------------------------------------
// CSysApDriveEjectHandler::HandleNotifyDismountFinished
// ---------------------------------------------------------------------------
//
void CSysApDriveEjectHandler::HandleNotifyDismountFinished(
        TInt aError,
        const TDismountFinishedEvent& aEvent )
    {
    (void)aError;
    (void)aEvent;

    TRACES( RDebug::Print(
        _L( "CSysApDriveEjectHandler::HandleNotifyDismountFinished: error: %d drive: %d forced: %d" ),
        aError, aEvent.iDrive, aEvent.iForcedDismount ) );

    iDriveToEject = KErrNotFound;

    // Show dismount ready note
    TBool dialogShown( EFalse );
    TRAPD( err, dialogShown = iSysApAppUi.ShowMMCDismountedDialogL(
            aEvent.iDrive, CSysApDriveList::EEjectFromMenu ) );
    if ( err != KErrNone || !dialogShown )
        {
        StopEject( err );
        }
    }

// End of File
