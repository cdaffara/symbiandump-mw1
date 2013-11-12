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
* Description:  Declaration of COutOfDiskMonitor class.
*
*/


#ifndef __OUTOFDISKMONITOR_H__
#define __OUTOFDISKMONITOR_H__

// SYSTEM INCLUDES

// USER INCLUDES
#include "outofdiskglobalnote.h"
#include "UiklafInternalCRKeys.h"

// CONSTANTS
const TInt KDelayAfterNote = 120000000; //Microseconds. Max value 2147483647 = 35min 47sec.

// FORWARD DECLARATIONS
class COutOfDiskNotifyObserver;
class COutOfDiskPSObserver;
class COutOfDiskGlobalNote;
class TDriveStatus
    {
public:
    TInt iStatus;
    TInt iDisplayed;
	CPeriodic* iTimer;
    };
class COutOfDiskMonitor;	
struct TAfterNoteTimer
	{
	COutOfDiskMonitor* iOODM;
	TInt iDrive;
	};
	
/**
*  
*  
*
*  @lib None
*  @since S60 3.2
*/
NONSHARABLE_CLASS(COutOfDiskMonitor) 
        : public CLafShutdownManagerBase
    {
    public:
        static COutOfDiskMonitor* NewL(MShutdownEventObserver& aObserver);
        ~COutOfDiskMonitor();
    private:
        COutOfDiskMonitor(MShutdownEventObserver& aObserver);
        void ConstructL();
		static TInt DoStopTimingL( TAny* aParam );
    public:  //New functions
        void ShowGlobalQueryIfNeededL();
        TInt64 GetThreshold(TInt aLevel, TInt aDrive);
        TInt GetDriveStatus(TInt aDrive);        
        void SaveDriveStatus(TInt aDrive, TInt aStatus, TBool aReset = EFalse);
        TBool ResetDisplayedStatus(TInt aDrive, TBool aResetAll = EFalse);
        TBool AllowedToReset(TInt aDrive);
        void SetGlobalNotesAllowed(TBool aValue);
        TBool GetGlobalNotesAllowed();
        TInt GetDefaultPhoneMemory();
        TInt GetDefaultMassStorage();
        TInt GetDefaultRomDrive();
        TInt GetTopPriorityDrive();
        void SetAsDisplayedL(TInt aDrive, TInt aDriveStatus);
    private: //Data
        COutOfDiskNotifyObserver*   iOutOfDiskNotifyObserver; //owns
        COutOfDiskPSObserver*       iOutOfDiskPSObserver; //owns
        COutOfDiskGlobalNote*       iOutOfDiskGlobalNote; // owns
        RFs                         iFs;
        TInt                      	iOODWarningThreshold; //percentages of used disk space
        TInt64                      iOODCriticalThreshold; //bytes free
        TDriveStatus                iDriveStatus[EDriveZ+1];
        TBool                       iGlobalNotesAllowed;
        TInt                        iDefaultPhoneMemory;
        TInt                        iDefaultMassStorage;
        TInt                        iDefaultRomDrive;
        RResourceFile               iResourceFile;
        TInt64                      iOODWarningThresholdMassMemory;
    };

#endif // __OUTOFDISKMONITOR_H__
