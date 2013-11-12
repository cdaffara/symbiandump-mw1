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
* Description:  Declaration of COutOfDiskNotifyObserver class.
*
*/


#ifndef __OUTOFDISKNOTIFYOBSERVER_H__
#define __OUTOFDISKNOTIFYOBSERVER_H__

// SYSTEM INCLUDES
#include <disknotifyhandler.h>

// USER INCLUDES
#include "outofdiskmonitor.h"

// CONSTANTS

// FORWARD DECLARATIONS
class COutOfDiskMonitor;

/**
*  
*  
*
*  @lib None
*  @since S60 3.2
*/
NONSHARABLE_CLASS(COutOfDiskNotifyObserver) 
        : public MDiskNotifyHandlerCallback
    {
    public:
        static COutOfDiskNotifyObserver* NewL(COutOfDiskMonitor* aOutOfDiskMonitor, RFs& aFs);
        ~COutOfDiskNotifyObserver();
    public:     // From observer interface
        void HandleNotifyDisk(
            TInt aError, const TDiskEvent& aEvent );

        void HandleNotifyDismount(
            TInt aError, const TDismountEvent& aEvent );

        void HandleNotifyDiskSpace(
            TInt aError, const TDiskSpaceEvent& aEvent );

        void HandleNotifyEntry(
            TInt aError, const TEntryEvent& aEvent );
    public:     // New methods
        void NotifyDiskSpace(TInt64 aWarningThreshold, TInt64 aCriticalThreshold, TInt aDrive);
    private:     // New methods        
        void DiskAddedL(TInt aDrive);
    private:
        COutOfDiskNotifyObserver(COutOfDiskMonitor* aOutOfDiskMonitor, RFs& aFs);
        void ConstructL();       
    private:    // Data
        RFs&                    iFs;    
        COutOfDiskMonitor*      iOutOfDiskMonitor;
        CDiskNotifyHandler*     iDiskNotifyHandler;
        CActiveSchedulerWait*   iWait;
        TBool                   iCritical2OK;
    };

#endif // __OUTOFDISKNOTIFYOBSERVER_H__
