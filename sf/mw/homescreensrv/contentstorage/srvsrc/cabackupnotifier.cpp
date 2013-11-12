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
 * Description:  ?Description
 *
 */

#include <e32base.h>
#include <sacls.h>
#include <connect/sbdefs.h>

#include "cabackupnotifier.h"
#include "castorageproxy.h"

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CCaBackupNotifier* CCaBackupNotifier::NewL( CCaStorageProxy* aStorageProxy )
    {
    CCaBackupNotifier* self = new ( ELeave ) CCaBackupNotifier( aStorageProxy );
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CCaBackupNotifier::~CCaBackupNotifier()
    {
    Cancel();
    iProperty.Close();
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CCaBackupNotifier::CCaBackupNotifier( CCaStorageProxy* aStorageProxy ) :
    CActive( EPriorityNormal ),
    iStorageProxy( aStorageProxy ),
    iLastState(0)
    {
    iProperty.Attach( KUidSystemCategory, KUidBackupRestoreKey );
    CActiveScheduler::Add( this );
    iStatus = KRequestPending;
    iProperty.Subscribe( iStatus );
    SetActive();
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCaBackupNotifier::ConstructL(  )
    {

    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
#ifdef COVERAGE_MEASUREMENT
#pragma CTC SKIP
#endif //COVERAGE_MEASUREMENT (calls another method)
void CCaBackupNotifier::DoCancel()
    {
    iProperty.Cancel();
    }
#ifdef COVERAGE_MEASUREMENT
#pragma CTC ENDSKIP
#endif //COVERAGE_MEASUREMENT

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCaBackupNotifier::RunL()
    {
	TInt backupStateValue = 0;
	iProperty.Get(backupStateValue);
	// Resubscribe before dealing with the current notification 	
    iProperty.Subscribe(iStatus);
    SetActive();

    conn::TBURPartType type = static_cast< conn::TBURPartType >
        ( backupStateValue & conn::KBURPartTypeMask );
    
    if( type == conn::EBURRestoreFull || type == conn::EBURRestorePartial  )
    	{
        // restore starting
        iLastState = ECaRestore;
    	}
    else if( type == conn::EBURBackupFull || type == conn::EBURBackupPartial )
    	{
        // backup starting
        iStorageProxy->SaveDatabaseL();
        iLastState = ECaBackup;
    	}
    else if( ( type == conn::EBURNormal || type == conn::EBURUnset ) && iLastState == ECaRestore )
    	{ // restore ends
        iStorageProxy->RestoreDatabaseL();
    	}
    else if( ( type == conn::EBURNormal || type == conn::EBURUnset ) && iLastState == ECaBackup )
    	{ // backup ends
         // do nothing
    	}
    }


#ifdef COVERAGE_MEASUREMENT
#pragma CTC SKIP
#endif //COVERAGE_MEASUREMENT (calls another method)
TInt CCaBackupNotifier::RunError( TInt /*aError*/)
    {
    // No need to do anything
    return KErrNone;
    }
#ifdef COVERAGE_MEASUREMENT
#pragma CTC ENDSKIP
#endif //COVERAGE_MEASUREMENT
