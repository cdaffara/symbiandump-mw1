/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of COutOfDiskNotifyObserver class.
*
*/


// SYSTEM INCLUDES
#include <driveinfo.h>

// USER INCLUDES
#include "outofdisknotifyobserver.h"
#include "UiklafInternalCRKeys.h"
#include "OodTraces.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
COutOfDiskNotifyObserver* COutOfDiskNotifyObserver::NewL(COutOfDiskMonitor* aOutOfDiskMonitor, 
                                                         RFs& aFs)
    { // static
    TRACES("COutOfDiskNotifyObserver::NewL");
    COutOfDiskNotifyObserver* self = new(ELeave) COutOfDiskNotifyObserver(aOutOfDiskMonitor, aFs);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
COutOfDiskNotifyObserver::~COutOfDiskNotifyObserver()
    {
    TRACES("COutOfDiskNotifyObserver::~COutOfDiskNotifyObserver");
    delete iWait;
    delete iDiskNotifyHandler;
    TRACES("COutOfDiskNotifyObserver::~COutOfDiskNotifyObserver: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
COutOfDiskNotifyObserver::COutOfDiskNotifyObserver(COutOfDiskMonitor* aOutOfDiskMonitor, 
                                                   RFs& aFs) :
    iFs(aFs),
    iOutOfDiskMonitor( aOutOfDiskMonitor ),
    iCritical2OK(EFalse)
    {
    TRACES("COutOfDiskNotifyObserver::COutOfDiskNotifyObserver");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfDiskNotifyObserver::ConstructL()
    {
    TRACES("COutOfDiskNotifyObserver::ConstructL");

    TRACES("COutOfDiskNotifyObserver::ConstructL: Create ActiveScheduler");
    iWait = new ( ELeave )CActiveSchedulerWait;
    TRACES("COutOfDiskNotifyObserver::ConstructL: Create iDiskNotifyHandler");
    iDiskNotifyHandler = CDiskNotifyHandler::NewL( *this, iFs );

    // Start listening disk events first
    TInt status = iDiskNotifyHandler->NotifyDisk();
    TRACES1("COutOfDiskNotifyObserver::ConstructL: NotifyDisk status %d",status);
    
    TRACES("COutOfDiskNotifyObserver::ConstructL: Get list of already mounted drives");
    TDriveList driveListInt;
    TInt driveCountInt(0);
    User::LeaveIfError(DriveInfo::GetUserVisibleDrives(
           iFs, driveListInt, driveCountInt, KDriveAttInternal | KDriveAttRemovable )); // Get already inserted internal and removable drives
    for( TInt i = EDriveA; i <= EDriveZ; i++ )
        {
        if (driveListInt[i])
            {
            TRACES("COutOfDiskNotifyObserver::ConstructL: Add disk +++++++++++++++++++++++++++++");
            DiskAddedL(i);
            TRACES("COutOfDiskNotifyObserver::ConstructL: Disk added ---------------------------");
            }            
        }

    iWait->Start();    
    TRACES("COutOfDiskNotifyObserver::ConstructL: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfDiskNotifyObserver::NotifyDiskSpace(TInt64 aWarningThreshold, TInt64 aCriticalThreshold,
                                               TInt aDrive)
    {
    TRACES("COutOfDiskNotifyObserver::NotifyDiskSpace");
    TRACES1("COutOfDiskNotifyObserver::NotifyDiskSpace: Drive %c",aDrive+'A');

    TRACES1("COutOfDiskNotifyObserver::NotifyDiskSpace: WarningThreshold            %ld",aWarningThreshold);
    TRACES1("COutOfDiskNotifyObserver::NotifyDiskSpace: CriticalThreshold           %ld",aCriticalThreshold);
    
    iDiskNotifyHandler->NotifyDiskSpace(aWarningThreshold, aDrive);
    iDiskNotifyHandler->NotifyDiskSpace(aCriticalThreshold, aDrive);
    TRACES("COutOfDiskNotifyObserver::NotifyDiskSpace: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfDiskNotifyObserver::DiskAddedL(TInt aDrive)
    {
    TRACES1("COutOfDiskNotifyObserver::DiskAddedL: Drive %c",aDrive+'A');

    TUint driveStatus(0);
    if ( DriveInfo::GetDriveStatus( iFs, aDrive, driveStatus ) == KErrNone )
        {
        if (driveStatus & DriveInfo::EDriveRemote)
            {
            TRACES("COutOfDiskNotifyObserver::DiskAddedL: Do not monitor remote drives");
            return;
            }
        }

	TInt64 warningThreshold = iOutOfDiskMonitor->GetThreshold(DISK_SPACE_WARNING, aDrive);
	TInt64 criticalThreshold = iOutOfDiskMonitor->GetThreshold(DISK_SPACE_CRITICAL, aDrive);
	TRACES1("COutOfDiskNotifyObserver::DiskAddedL: Warning Threshold: %ld",warningThreshold);
	TRACES1("COutOfDiskNotifyObserver::DiskAddedL: Critical Threshold: %ld",criticalThreshold);
	
	if (warningThreshold <= 0 || criticalThreshold <= 0)
		{
        TRACES("COutOfDiskNotifyObserver::DiskAddedL: Cannot get threshold of the drive");
	    return;		
		}
	
    TInt status = iOutOfDiskMonitor->GetDriveStatus(aDrive);
    iOutOfDiskMonitor->SaveDriveStatus(aDrive, status, ETrue);
    iOutOfDiskMonitor->ShowGlobalQueryIfNeededL();
    NotifyDiskSpace(warningThreshold, 
                    criticalThreshold, 
                    aDrive);
    TRACES("COutOfDiskNotifyObserver::DiskAddedL: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfDiskNotifyObserver::HandleNotifyDisk(
        TInt /*aError*/, const TDiskEvent& aEvent )
    {
    TRACES("COutOfDiskNotifyObserver::HandleNotifyDisk +++++++++++++++++++++++++++++");
    TRACES1("COutOfDiskNotifyObserver::HandleNotifyDisk: Drive %c:",aEvent.iDrive+'A');

    switch(aEvent.iType)
        {
        case EDiskAdded:
            TRACES("COutOfDiskNotifyObserver::HandleNotifyDisk: EDiskAdded");
            TRAP_IGNORE( DiskAddedL(aEvent.iDrive) );
            break;
        case EDiskError:
        case EDiskRemoved:
            TRACES("COutOfDiskNotifyObserver::HandleNotifyDisk: EDiskRemoved");
            iDiskNotifyHandler->CancelNotifyDiskSpace(aEvent.iDrive);
            break;
        case EDiskStatusChanged:
            TRACES("COutOfDiskNotifyObserver::HandleNotifyDisk: EDiskStatusChanged");
            TVolumeInfo volInfo;
            TInt ret( iFs.Volume( volInfo, aEvent.iDrive ) );
            TRACES1("COutOfDiskMonitor::HandleNotifyDisk: Free space: %d",volInfo.iFree);
            if (volInfo.iFree >= 0)
                {
                TRACES("COutOfDiskNotifyObserver::HandleNotifyDisk: Added");
                TRAP_IGNORE( DiskAddedL(aEvent.iDrive) );
                }
            else
                {
                TRACES("COutOfDiskNotifyObserver::HandleNotifyDisk: Removed");
                iDiskNotifyHandler->CancelNotifyDiskSpace(aEvent.iDrive);
                iOutOfDiskMonitor->SaveDriveStatus(aEvent.iDrive, DISK_SPACE_OK, ETrue);
                TRAP_IGNORE( iOutOfDiskMonitor->ShowGlobalQueryIfNeededL() );
                }
            break;
        }
    TRACES("COutOfDiskNotifyObserver::HandleNotifyDisk: End ------------------------------ ");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfDiskNotifyObserver::HandleNotifyDismount(
        TInt /*aError*/, const TDismountEvent& aEvent )
    {
    TRACES("COutOfDiskNotifyObserver::HandleNotifyDismount +++++++++++++++++++++++++++++");
    iDiskNotifyHandler->CancelNotifyDiskSpace(aEvent.iDrive);
    TRACES("COutOfDiskNotifyObserver::HandleNotifyDismount: End ------------------------------ ");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfDiskNotifyObserver::HandleNotifyDiskSpace(
        TInt /*aError*/, const TDiskSpaceEvent& aEvent )
    {
    TRACES("COutOfDiskNotifyObserver::HandleNotifyDiskSpace +++++++++++++++++++++++++++++");
    TRACES1("COutOfDiskNotifyObserver::HandleNotifyDiskSpace: Drive %c:",aEvent.iDrive+'A');
    TRACES1("COutOfDiskNotifyObserver::HandleNotifyDiskSpace: Threshold: %ld",aEvent.iThreshold);

    iOutOfDiskMonitor->SaveDriveStatus(aEvent.iDrive, 
            iOutOfDiskMonitor->GetDriveStatus(aEvent.iDrive));
    TRAP_IGNORE( iOutOfDiskMonitor->ShowGlobalQueryIfNeededL() );
    
    TRACES("COutOfDiskNotifyObserver::HandleNotifyDismount: End ------------------------------ ");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfDiskNotifyObserver::HandleNotifyEntry(
        TInt /*aError*/, const TEntryEvent& /*aEvent*/ )
    {
    TRACES("COutOfDiskNotifyObserver::HandleNotifyEntry");
    }
// End of file.
