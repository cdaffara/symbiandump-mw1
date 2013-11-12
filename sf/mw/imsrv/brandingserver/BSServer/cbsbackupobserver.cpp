/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Stores element data and writes it to stream
*
*/



#include <connect/sbdefs.h>

#include "cbsbackupobserver.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CBSBackupObserver::NewL
// ---------------------------------------------------------------------------
//
CBSBackupObserver* CBSBackupObserver::NewL()
    {
    CBSBackupObserver* self = NewLC();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// CBSBackupObserver::NewLC
// ---------------------------------------------------------------------------
//
CBSBackupObserver* CBSBackupObserver::NewLC()
    {
    CBSBackupObserver* self = new (ELeave) CBSBackupObserver();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CBSBackupObserver::ConstructL
// ---------------------------------------------------------------------------
//
void CBSBackupObserver::ConstructL()
    {
    CActiveScheduler::Add( this );
    iProperty.Attach( KUidSystemCategory, conn::KUidBackupRestoreKey );
    iLatestState = BackupStateL();
    Subscribe();
    }

// ---------------------------------------------------------------------------
// CBSBackupObserver::CBSBackupObserver
// ---------------------------------------------------------------------------
//
CBSBackupObserver::CBSBackupObserver() :
    CActive( EPriorityStandard )
    {
    }

// ---------------------------------------------------------------------------
// CBSBackupObserver::~CBSBackupObserver
// ---------------------------------------------------------------------------
//
CBSBackupObserver::~CBSBackupObserver()
    {
    Cancel();
    iObservers.Close();
    }

// ---------------------------------------------------------------------------
// CBSBackupObserver::RunL
// ---------------------------------------------------------------------------
//
void CBSBackupObserver::RunL()
    {
    MBSBackupObserver::TBackupState newState = BackupStateL();
    if( newState != iLatestState )
        {
        // state changed
        iLatestState = newState;
        NotifyObserversL( iLatestState );
        }
    Subscribe();
    }

// ---------------------------------------------------------------------------
// CBSBackupObserver::DoCancel
// ---------------------------------------------------------------------------
//
void CBSBackupObserver::DoCancel()
    {
    iProperty.Cancel();
    }

// ---------------------------------------------------------------------------
// CBSBackupObserver::RunError
// ---------------------------------------------------------------------------
//
TInt CBSBackupObserver::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CBSBackupObserver::RegisterObserver
// ---------------------------------------------------------------------------
//
TInt CBSBackupObserver::RegisterObserver( MBSBackupObserver* aObserver )
    {
    TInt err = iObservers.InsertInAddressOrder( aObserver );
    if( err == KErrAlreadyExists )
        {
        // ignore duplicate registration
        err = KErrNone;
        }

    // if backup is already active -> notify
    if( iLatestState == MBSBackupObserver::EBackupActive )
        {
        TRAP_IGNORE( aObserver->HandleBackupStateL( iLatestState ) );
        }

    return err;
    }

// ---------------------------------------------------------------------------
// CBSBackupObserver::UnregisterObserver
// ---------------------------------------------------------------------------
//
void CBSBackupObserver::UnregisterObserver( MBSBackupObserver* aObserver )
    {
    TInt index = iObservers.FindInAddressOrder( aObserver );
    if( index != KErrNotFound )
        {
        iObservers.Remove( index );
        }
    }

// ---------------------------------------------------------------------------
// CBSBackupObserver::Subscribe
// ---------------------------------------------------------------------------
//
void CBSBackupObserver::Subscribe()
    {
    if( IsActive() )
        {
        // already active
        return;
        }
    iStatus = KRequestPending;
    iProperty.Subscribe( iStatus );
    SetActive();    
    }

// ---------------------------------------------------------------------------
// CBSBackupObserver::NotifyObservers
// ---------------------------------------------------------------------------
//
void CBSBackupObserver::NotifyObserversL( MBSBackupObserver::TBackupState aState )
    {
    TInt count = iObservers.Count();
    for( TInt i = 0; i < count; i++ )
        {
        TRAP_IGNORE( iObservers[i]->HandleBackupStateL( aState ) );
        }
    }

// ---------------------------------------------------------------------------
// CBSBackupObserver::BackupState
// ---------------------------------------------------------------------------
//
MBSBackupObserver::TBackupState CBSBackupObserver::BackupStateL()
    {
    TInt value = 0;
    User::LeaveIfError( iProperty.Get( value ) );
    if( value == 0 || 
        value == ( conn::EBURNormal | conn::ENoBackup ) )
        {
        // backup or restore is not active at the moment
        return MBSBackupObserver::EBackupNotActive;
        }
    return MBSBackupObserver::EBackupActive;
    }

// End of file
