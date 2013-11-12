/*
* Copyright (c) 2003-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implements a watcher for MMC disk change events
 *
*/


// INCLUDES
#include <e32base.h>
#include <f32file.h>
#ifdef RD_MULTIPLE_DRIVE
 #include "sysapdrivelist.h"
#endif // RD_MULTIPLE_DRIVE
#include "SysApMMCObserver.h"
#include "SysAp.hrh"

#ifndef RD_MULTIPLE_DRIVE

CSysApMMCObserver* CSysApMMCObserver::NewL( MSysApMemoryCardObserver* aObserver, RFs* aFileServerSession, TBool aHotSwapSupported )
    {
    CSysApMMCObserver* self = new(ELeave) CSysApMMCObserver( aHotSwapSupported );
    CleanupStack::PushL( self );
    self->ConstructL( aObserver, aFileServerSession );
    CleanupStack::Pop( self );
    return self;
    }

void CSysApMMCObserver::ConstructL( MSysApMemoryCardObserver* aObserver, RFs* aFileServerSession )
    {
    iObserver=aObserver;
    iFsSession=aFileServerSession;
    CActiveScheduler::Add( this );
    }

CSysApMMCObserver::CSysApMMCObserver( TBool aHotSwapSupported ) : CActive( EPriorityIdle ), iHotSwapSupported( aHotSwapSupported )
    {
    }

CSysApMMCObserver::~CSysApMMCObserver()
    {
    TRACES(RDebug::Print( _L("CSysApMMCObserver::~CSysApMMCObserver start") ) );
    Cancel();
    TRACES(RDebug::Print( _L("CSysApMMCObserver::~CSysApMMCObserver end") ) );
    }

void CSysApMMCObserver::DoCancel()
    {
    iFsSession->NotifyChangeCancel();
    }

void CSysApMMCObserver::RunL()
    {
    TRACES(RDebug::Print( _L("CSysApMMCObserver::RunL status: %d"), iStatus.Int() ) );
    StartMountObserver();
    iObserver->MMCStatusChangedL();    
    }

void CSysApMMCObserver::StartMountObserver()
    {
    if ( iHotSwapSupported )
        {
        TRACES(RDebug::Print(_L("CSysApMMCObserver::StartMountObserver")));
        if ( IsActive() ) 
            {
            Cancel();
            }
        iFsSession->NotifyChange( ENotifyDisk, iStatus );
        SetActive();
        }
    }

TSysApMemoryCardStatus CSysApMMCObserver::MemoryCardStatus()
    {
    TInt err ( 0 );
    // Get MMC drive information
    TDriveInfo driveInfo;   
    err = iFsSession->Drive( driveInfo, EDriveE );
    TRACES( RDebug::Print( _L( "CSysApMMCObserver::MemoryCardStatus: Getting more info, ret = %d" ), err ) );
    TRACES( RDebug::Print( _L( "CSysApMMCObserver::MemoryCardStatus: DriveAtt: %d"), driveInfo.iDriveAtt ) );
    TRACES( RDebug::Print( _L( "CSysApMMCObserver::MemoryCardStatus: MediaAtt: %d"), driveInfo.iMediaAtt ) );
    TRACES( RDebug::Print( _L( "CSysApMMCObserver::MemoryCardStatus: Media info: %d"), driveInfo.iType ) );

    if ( err ) err = 0; // To supress stupid warnings about "err"

    // Check if the MMC is currently locked.
    // If mmc is locked, then driveInfo.iType is zero, therefore switch clause below does not notice new card
    // Check iMediaAtt instead, if it tells that the media can be locked, and it is currently locked and it has password,
    // then locked MMC is inserted

    TSysApMemoryCardStatus mmcStatus( ESysApMemoryCardNotInserted );

    if ( ( driveInfo.iMediaAtt & ( KMediaAttLockable|KMediaAttLocked|KMediaAttHasPassword ) ) == 
         ( KMediaAttLockable|KMediaAttLocked|KMediaAttHasPassword ) )
        {
        TRACES( RDebug::Print( _L( "CSysApMMCObserver::MemoryCardStatus: Locked MMC inserted" ) ) );
        mmcStatus = ESysApMemoryCardLocked;
        }
    else
        {
        switch( driveInfo.iType )
            {
            case EMediaNotPresent:
                {
                TRACES( RDebug::Print( _L( "CSysApMMCObserver::MemoryCardStatus: MMC not inserted" ) ) );
                mmcStatus = ESysApMemoryCardNotInserted;
                break;
                }
            default:
                {
                TRACES( RDebug::Print( _L( "CSysApMMCObserver::MemoryCardStatus: MMC inserted" ) ) );
                mmcStatus = ESysApMemoryCardInserted;
                break;
                }
            }
        }
    return mmcStatus;
    }

#else // RD_MULTIPLE_DRIVE

// ============================ MEMBER FUNCTIONS =============================
// ---------------------------------------------------------------------------
// CSysApMMCObserver::NewL
// ---------------------------------------------------------------------------
//
CSysApMMCObserver* CSysApMMCObserver::NewL(
        RFs& aFs,
        CSysApDriveList& aSysApDriveList,
        MSysApMemoryCardObserver& aObserver,
        const TBool aHotSwapSupported )
    {
    CSysApMMCObserver* self = new ( ELeave ) CSysApMMCObserver(
        aFs, aSysApDriveList, aObserver, aHotSwapSupported );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CSysApMMCObserver::ConstructL
// ---------------------------------------------------------------------------
//
void CSysApMMCObserver::ConstructL()
    {
    iDiskNotifyHandler = CDiskNotifyHandler::NewL( *this, iFs );
    }

// ---------------------------------------------------------------------------
// CSysApMMCObserver::CSysApMMCObserver
// ---------------------------------------------------------------------------
//
CSysApMMCObserver::CSysApMMCObserver(
        RFs& aFs,
        CSysApDriveList& aSysApDriveList,
        MSysApMemoryCardObserver& aObserver,
        const TBool aHotSwapSupported ) :
    iFs( aFs ),
    iSysApDriveList( aSysApDriveList ),
    iObserver( aObserver ),
    iHotSwapSupported( aHotSwapSupported )
    {
    }

// ---------------------------------------------------------------------------
// CSysApMMCObserver::~CSysApMMCObserver
// ---------------------------------------------------------------------------
//
CSysApMMCObserver::~CSysApMMCObserver()
    {
    TRACES( RDebug::Print(
        _L( "CSysApMMCObserver::~CSysApMMCObserver" ) ) );

    delete iDiskNotifyHandler;
    }

// ---------------------------------------------------------------------------
// CSysApMMCObserver::StartMountObserver
// ---------------------------------------------------------------------------
//
void CSysApMMCObserver::StartMountObserver()
    {
    TInt err( iDiskNotifyHandler->NotifyDisk() );
    if ( err != KErrNone )
        {
        TRACES( RDebug::Print(
            _L( "CSysApMMCObserver::StartMountObserver, err: %d" ),
            err ) );
        }
    }

// ---------------------------------------------------------------------------
// CSysApMMCObserver::HandleNotifyDisk
// ---------------------------------------------------------------------------
//
void CSysApMMCObserver::HandleNotifyDisk( TInt aError, const TDiskEvent& aEvent )
    {
    TRACES( RDebug::Print(
        _L( "CSysApMMCObserver::HandleNotifyDisk: error: %d, type: %d, drive: %d, iHotSwapSupported: %d" ),
        aError, aEvent.iType, aEvent.iDrive, iHotSwapSupported ) );

    if ( aError != KErrNone )
        {
        return;
        }

    iSysApDriveList.UpdateDrive( aEvent.iDrive, ETrue );

    if ( iHotSwapSupported )
        {
        TRAPD( err, iObserver.MMCStatusChangedL( aEvent.iDrive ) );
        if ( err != KErrNone )
            {
            TRACES( RDebug::Print(
                _L( "CSysApMMCObserver::HandleNotifyDisk: MMCStatusChangedL err: %d" ),
                err ) );
            }
        }
    }

#endif // RD_MULTIPLE_DRIVE

// End of File
