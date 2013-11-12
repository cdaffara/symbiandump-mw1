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
* Description:  Implementation of COutOfDiskMonitor class.
*
*/


// SYSTEM INCLUDES
#include <driveinfo.h>
#include <centralrepository.h>

// USER INCLUDES
#include "outofdiskmonitor.h"
#include "outofdisknotifyobserver.h"
#include "OodTraces.h"
#include "UiklafInternalCRKeys.h"
#include "outofdiskpsobserver.h"
#include "UiklafInternalCRKeys.h"

// CONSTANTS
// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
COutOfDiskMonitor* COutOfDiskMonitor::NewL(MShutdownEventObserver& aObserver)
    { // static
    TRACES("COutOfDiskMonitor::NewL");
    COutOfDiskMonitor* self = new(ELeave) COutOfDiskMonitor(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
COutOfDiskMonitor::~COutOfDiskMonitor()
    {
    TRACES("COutOfDiskMonitor::~COutOfDiskMonitor");
    delete iOutOfDiskNotifyObserver;
    delete iOutOfDiskGlobalNote;    
    iResourceFile.Close();
    iFs.Close();
    for( TInt i = EDriveA; i <= EDriveZ; i++ )
        {
            if( iDriveStatus[i].iTimer )
            {
            iDriveStatus[i].iTimer->Cancel();
            delete iDriveStatus[i].iTimer;
            }
        }

    TRACES("COutOfDiskMonitor::~COutOfDiskMonitor: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
COutOfDiskMonitor::COutOfDiskMonitor(MShutdownEventObserver& aObserver)
    : CLafShutdownManagerBase(aObserver),
      iGlobalNotesAllowed(EFalse)
    {
    TRACES("COutOfDiskMonitor::COutOfDiskMonitor");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfDiskMonitor::ConstructL()
    {
    TRACES("COutOfDiskMonitor::ConstructL"); 
   
    TRACES("COutOfDiskMonitor::ConstructL: Connect to FS");
    User::LeaveIfError(iFs.Connect());
    
    DriveInfo::GetDefaultDrive( DriveInfo::EDefaultPhoneMemory, iDefaultPhoneMemory );
    DriveInfo::GetDefaultDrive( DriveInfo::EDefaultMassStorage, iDefaultMassStorage );
    TRACES2("COutOfDiskMonitor::ConstructL: Default Phone mem %c, default mass mem %c"
                    ,iDefaultPhoneMemory+'A',iDefaultMassStorage+'A');      
    
    iOutOfDiskGlobalNote = COutOfDiskGlobalNote::NewL( this, iFs );

    for( TInt i = EDriveA; i <= EDriveZ; i++ )
        {
        SaveDriveStatus(i, DISK_SPACE_OK, ETrue);
        }
    
    iOutOfDiskPSObserver = COutOfDiskPSObserver::NewL( this );

    CRepository* repository( NULL );
    TInt warningThreshold(0);
    TInt criticalThreshold(0);
    TInt warningThresholdMassMemory(0);
    TRAPD( err, repository = CRepository::NewL( KCRUidUiklaf ) );
    if ( err == KErrNone )
        {
        err = repository->Get(KUikOODDiskFreeSpaceWarningNoteLevel, warningThreshold);
        err = repository->Get(KUikOODDiskCriticalThreshold, criticalThreshold);
        err = repository->Get(KUikOODDiskFreeSpaceWarningNoteLevelMassMemory, warningThresholdMassMemory);
        }
    delete repository;
    iOODWarningThreshold = warningThreshold;
    iOODCriticalThreshold = criticalThreshold;
    iOODWarningThresholdMassMemory = warningThresholdMassMemory;
    
	TRACES1("COutOfDiskMonitor::ConstructL: Warning threshold Phone Memory: %d percent",iOODWarningThreshold);
    TRACES1("COutOfDiskMonitor::ConstructL: Critical threshold: %ld bytes",iOODCriticalThreshold);
    TRACES1("COutOfDiskMonitor::ConstructL: Warning threshold Mass Memory: %ld bytes",iOODWarningThresholdMassMemory);
    
    iOutOfDiskNotifyObserver = COutOfDiskNotifyObserver::NewL( this, iFs );
    TRACES("COutOfDiskMonitor::ConstructL: End");
    }

// ---------------------------------------------------------
// 
// 
// ---------------------------------------------------------
//
void COutOfDiskMonitor::ShowGlobalQueryIfNeededL()
    {
    TRACES("COutOfDiskMonitor::ShowGlobalQueryIfNeededL");
    TInt drive = GetTopPriorityDrive();
    if (drive != KErrNotFound)
        {
        iOutOfDiskGlobalNote->ShowGlobalQueryL(iDriveStatus[drive].iStatus, drive);
        }
    TRACES("COutOfDiskMonitor::ShowGlobalQueryIfNeededL: End");
    }
    
// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
TInt COutOfDiskMonitor::DoStopTimingL( TAny* aParam )
    {
    TAfterNoteTimer* p = reinterpret_cast<TAfterNoteTimer*>( aParam );
    COutOfDiskMonitor* oodm = STATIC_CAST( COutOfDiskMonitor*, p->iOODM ); // cast
    TBool ret =  oodm->ResetDisplayedStatus(p->iDrive); // call non-static function	
    if ( ret )
        {
        delete p;
        p = NULL;
        }
    return KErrNone;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
TInt COutOfDiskMonitor::GetDriveStatus(TInt aDrive)
    {
    TRACES("COutOfDiskMonitor::GetDriveStatus");
    TInt64 freeSpace(0);
    TInt state(DISK_SPACE_OK);
    TVolumeInfo volInfo;
    TInt ret( iFs.Volume( volInfo, aDrive ) );
    if ( ret == KErrNone )
        {
        freeSpace = volInfo.iFree;
        TRACES1("COutOfDiskMonitor::GetDriveStatus: Free space: %ld",freeSpace);           
        }
    else
        {
        TRACES1("COutOfDiskMonitor::GetDriveStatus: End: Returns %d",state);
        return state;
        }
    
    if (freeSpace < GetThreshold(DISK_SPACE_WARNING, aDrive) && 
        freeSpace > GetThreshold(DISK_SPACE_CRITICAL, aDrive))
        {
        state = DISK_SPACE_WARNING;
        }    
    else if (freeSpace <= iOODCriticalThreshold)
        {
        state = DISK_SPACE_CRITICAL;
        }
    TRACES1("COutOfDiskMonitor::GetDriveStatus: End: Returns %d",state);
    return state;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
TInt64 COutOfDiskMonitor::GetThreshold(TInt aLevel, TInt aDrive)
    {
    TRACES2("COutOfDiskMonitor::GetThreshold(aLevel=%d, aDrive=%d)",aLevel, aDrive);
    TInt64 threshold(KErrNotFound);
    if (aLevel == DISK_SPACE_WARNING)
        {
		TVolumeInfo volInfo;
		TInt ret( iFs.Volume( volInfo, aDrive ) );
        TInt64 volSize(volInfo.iSize);
        TRACES1("COutOfDiskMonitor::GetThreshold: Volume size: %ld",volSize);		
		if ( ret == KErrNone )
			{
			if(aDrive == EDriveC)
			    {
                TRACES1("COutOfDiskMonitor::GetThreshold: Warning threshold Phone Memory: Used disk space %d percent",iOODWarningThreshold);
                threshold = ((volSize*(100-iOODWarningThreshold))/100);
			    }
			else
			    {
			    if(iOODWarningThresholdMassMemory < volSize )
                    {
                    TRACES1("COutOfDiskMonitor::GetThreshold: Warning threshold Mass Memory: %ld bytes",iOODWarningThresholdMassMemory);
                    threshold = iOODWarningThresholdMassMemory;
                    }
                else
                    {
                    TRACES1("COutOfDiskMonitor::GetThreshold: Warning threshold Phone Memory: Used disk space %d percent",iOODWarningThreshold);
                    threshold = ((volSize*(100-iOODWarningThreshold))/100);                                
                    }
			    }
			}
        }
    else if (aLevel == DISK_SPACE_CRITICAL)
        {
        threshold = iOODCriticalThreshold;
        }
    TRACES1("COutOfDiskMonitor::GetThreshold: End: Returns %ld",threshold);
    return threshold;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfDiskMonitor::SaveDriveStatus(TInt aDrive, TInt aStatus, TBool aReset)
    {
    TRACES("COutOfDiskMonitor::SaveDriveStatus");
    if (aDrive == iDefaultPhoneMemory)
        {
        TRACES("COutOfDiskMonitor::SaveDriveStatus: Defult phone mem. Update PS");
        RProperty::Set( KPSUidUikon, KUikFFSFreeLevel, aStatus );
        }
    else if (aDrive == iDefaultMassStorage)
        {
        TRACES("COutOfDiskMonitor::SaveDriveStatus: Defult mass mem. Update PS");
        RProperty::Set( KPSUidUikon, KUikMmcFFSFreeLevel, aStatus );
        }

    iDriveStatus[aDrive].iStatus = aStatus;		
    if (aReset)
        {
        ResetDisplayedStatus(aDrive);
        }
    TRACES3("COutOfDiskMonitor::SaveDriveStatus: End: D: %c, S: %d D: %d",aDrive+'A',aStatus,
            iDriveStatus[aDrive].iDisplayed);
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
TBool COutOfDiskMonitor::ResetDisplayedStatus(TInt aDrive, TBool aResetAll)
    {
    TRACES("COutOfDiskMonitor::ResetDisplayedStatus");
    if (aResetAll)
        {
        TRACES("COutOfDiskMonitor::ResetDisplayedStatus: Reset all drives");
        for( TInt i = EDriveA; i <= EDriveZ; i++ )
            {
            if (AllowedToReset(i))
                {
                iDriveStatus[i].iDisplayed = NULL;
                if (iDriveStatus[i].iTimer)
                    {
                    delete iDriveStatus[i].iTimer;
                    iDriveStatus[i].iTimer = NULL;
                    return ETrue;
                    }
                }
            }
        }
    else
        {
        TRACES1("COutOfDiskMonitor::ResetDisplayedStatus: Reset drive %c",aDrive+'A');
        if (AllowedToReset(aDrive))
            {
            iDriveStatus[aDrive].iDisplayed = NULL;
            if (iDriveStatus[aDrive].iTimer)
                {
                delete iDriveStatus[aDrive].iTimer;
                iDriveStatus[aDrive].iTimer = NULL;
                return ETrue;
                }
            }
        }
    return EFalse;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
TBool COutOfDiskMonitor::AllowedToReset(TInt aDrive)
    {
    TInt retVal(ETrue);
    TNoteInfo noteInfo(iOutOfDiskGlobalNote->GetNoteInfo());
    TRACES1("COutOfDiskMonitor::AllowedToReset: Drive %c",aDrive+'A');	
    if ((iOutOfDiskGlobalNote->NoteOnDisplay())&&
        (noteInfo.iDrive == aDrive))
        {
        if (noteInfo.iStatus == iDriveStatus[aDrive].iStatus)
            {
            TRACES("COutOfDiskMonitor::AllowedToReset: Same drive with same status already on display");
            retVal = EFalse;
            }
        else if ( noteInfo.iStatus == DISK_SPACE_CRITICAL && iDriveStatus[aDrive].iStatus == DISK_SPACE_WARNING )
            {
            TRACES("COutOfDiskMonitor::AllowedToReset: Same drive with worse status already on display");        
            retVal = EFalse;
            }
        }	
    TRACES1("COutOfDiskMonitor::AllowedToReset: %d",retVal);		
    return retVal;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
TInt COutOfDiskMonitor::GetTopPriorityDrive()
    {
    TRACES("COutOfDiskMonitor::GetTopPriorityDrive");
    // 0 - Critical Default Phone Memory
    // 1 - Critical Default Mass Storage
    // 2 - Critical other drives
    // 3 - Warning Default Phone Memory
    // 4 - Warning Default Mass Storage
    // 5 - Warning other drives

    TRACES2("COutOfDiskMonitor::GetTopPriorityDrive: DPM %d, %d",
        iDriveStatus[iDefaultPhoneMemory].iStatus, 
        iDriveStatus[iDefaultPhoneMemory].iDisplayed);
    
    TRACES2("COutOfDiskMonitor::GetTopPriorityDrive: DMS %d, %d",
        iDriveStatus[iDefaultMassStorage].iStatus, 
        iDriveStatus[iDefaultMassStorage].iDisplayed);
    
    if (iDriveStatus[iDefaultPhoneMemory].iStatus == DISK_SPACE_CRITICAL &&
        (iDriveStatus[iDefaultPhoneMemory].iDisplayed == NULL ||
        iDriveStatus[iDefaultPhoneMemory].iDisplayed == DISK_SPACE_WARNING)) 
        {
        TRACES2("COutOfDiskMonitor::GetTopPriorityDrive: Drive %c, status %d"
                        ,iDefaultPhoneMemory+'A',DISK_SPACE_CRITICAL);            
        return iDefaultPhoneMemory;
        }
    if (iDriveStatus[iDefaultMassStorage].iStatus == DISK_SPACE_CRITICAL &&
        iDriveStatus[iDefaultMassStorage].iDisplayed == NULL )
        {
        TRACES2("COutOfDiskMonitor::GetTopPriorityDrive: Drive %c, status %d"
                        ,iDefaultMassStorage+'A',DISK_SPACE_CRITICAL);
        return iDefaultMassStorage;
        }
    for( TInt i = EDriveA; i <= EDriveZ; i++ )
        {
        if (iDriveStatus[i].iStatus == DISK_SPACE_CRITICAL &&
            iDriveStatus[i].iDisplayed == NULL )
            {
            TRACES2("COutOfDiskMonitor::GetTopPriorityDrive: Drive %c, status %d"
                        ,i+'A',iDriveStatus[i].iStatus);                
            return i;
            }
        }
    if (iDriveStatus[iDefaultPhoneMemory].iStatus == DISK_SPACE_WARNING &&
        iDriveStatus[iDefaultPhoneMemory].iDisplayed == NULL )
        {
        TRACES2("COutOfDiskMonitor::GetTopPriorityDrive: Drive %c, status %d"
                        ,iDefaultPhoneMemory+'A',DISK_SPACE_WARNING);
        return iDefaultPhoneMemory;
        }
    if (iDriveStatus[iDefaultMassStorage].iStatus == DISK_SPACE_WARNING &&
        iDriveStatus[iDefaultMassStorage].iDisplayed == NULL )
        {
        TRACES2("COutOfDiskMonitor::GetTopPriorityDrive: Drive %c, status %d"
                        ,iDefaultMassStorage+'A',DISK_SPACE_WARNING);
        return iDefaultMassStorage;
        }
    for( TInt i = EDriveA; i <= EDriveZ; i++ )
        {
        if (iDriveStatus[i].iStatus == DISK_SPACE_WARNING &&
            iDriveStatus[i].iDisplayed == NULL )
            {
            TRACES2("COutOfDiskMonitor::GetTopPriorityDrive: Drive %c, status %d"
                        ,i+'A',iDriveStatus[i].iStatus);
            return i;
            }
        }
	TRACES("COutOfDiskMonitor::GetTopPriorityDrive: return KErrNotFound");
    return KErrNotFound;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfDiskMonitor::SetAsDisplayedL(TInt aDrive, TInt aDriveStatus)
    {
    TRACES2("COutOfDiskMonitor::SetAsDisplayedL: Drive %d: DriveStatus %d",aDrive, aDriveStatus);
    iDriveStatus[aDrive].iDisplayed = aDriveStatus;
    // Don't show notes for same drive on a given time. Show only if state is following cases:
    // 1) State is going worse
    // 2) Autolock off
    // 3) The drive is unmounted and mounted again
    // 4) Phone is restarted
    if ( !iDriveStatus[aDrive].iTimer)
        {
        TAfterNoteTimer* p = 0;
        p = new (ELeave) TAfterNoteTimer;
        p->iOODM = this;
        p->iDrive = aDrive;
        iDriveStatus[aDrive].iTimer = CPeriodic::NewL( EPriorityNormal );
        iDriveStatus[aDrive].iTimer->Start( KDelayAfterNote, KDelayAfterNote, TCallBack( DoStopTimingL, p ) );
        }
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfDiskMonitor::SetGlobalNotesAllowed(TInt aValue)
    {
    TRACES1("COutOfDiskMonitor::SetGlobalNotesAllowed: Value %d",aValue);
    if (aValue)
        iGlobalNotesAllowed = ETrue;
    else
        iGlobalNotesAllowed = EFalse;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
TBool COutOfDiskMonitor::GetGlobalNotesAllowed()
    {
    TRACES1("COutOfDiskMonitor::GetGlobalNotesAllowed: Value %d",iGlobalNotesAllowed);
    return iGlobalNotesAllowed;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
TInt COutOfDiskMonitor::GetDefaultPhoneMemory()
    {
    TRACES("COutOfDiskMonitor::GetDefaultPhoneMemory");
    return iDefaultPhoneMemory;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
TInt COutOfDiskMonitor::GetDefaultMassStorage()
    {
    TRACES("COutOfDiskMonitor::GetDefaultMassStorage");
    return iDefaultMassStorage;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
TInt COutOfDiskMonitor::GetDefaultRomDrive()
    {
    TRACES("COutOfDiskMonitor::GetDefaultRomDrive");
    return iDefaultRomDrive;
    }
// End of file.
