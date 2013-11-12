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


#ifndef __SYSAPMMCOBSERVER_H__
#define __SYSAPMMCOBSERVER_H__

// INCLUDES
#include "SysAp.hrh"

#ifndef RD_MULTIPLE_DRIVE

class MSysApMemoryCardObserver
{
public:
    /**
     * Implement this method to be notified when MMC status
     * changes.
     */
    virtual void MMCStatusChangedL() = 0;
};

class CSysApMMCObserver : public CActive
{
public:
    static CSysApMMCObserver* NewL( MSysApMemoryCardObserver* aObserver, RFs* aFileServerSession, TBool aHotSwapSupported );
    void RunL();
    void DoCancel();
    void StartMountObserver();
    ~CSysApMMCObserver();
public:

    /**
     * Returns the status of the memory card
     */
    TSysApMemoryCardStatus MemoryCardStatus();

private:
    void ConstructL( MSysApMemoryCardObserver* aObserver, RFs* aFileServerSession );	
    CSysApMMCObserver( TBool aHotSwapSupported );
    MSysApMemoryCardObserver* iObserver;
    RFs*                      iFsSession;
    CAsyncCallBack*           iAsyncCallBack;
    TBool                     iHotSwapSupported;

};

#else // RD_MULTIPLE_DRIVE

#include <disknotifyhandler.h>

// FORWARD DECLARATIONS
class CSysApDriveList;

class MSysApMemoryCardObserver
    {
public:
    /**
     * Implement this method to be notified when MMC status
     * changes.
     */
    virtual void MMCStatusChangedL( TInt aDrive  ) = 0;
    };

// CLASS DECLARATION
class CSysApMMCObserver : public CBase,
                          public MDiskNotifyHandlerCallback
    {
public:

    /**
     * Two-phased constructor.
     */
    static CSysApMMCObserver* NewL(
        RFs& aFs,
        CSysApDriveList& aSysApDriveList,
        MSysApMemoryCardObserver& aObserver,
        const TBool aHotSwapSupported );

    /**
     * Destructor
     */
    ~CSysApMMCObserver();

public: // New methods

    /**
     * Starts mount observer
     */
    void StartMountObserver();

private: // From MDiskNotifyHandlerCallback

    void HandleNotifyDisk( TInt aError, const TDiskEvent& aEvent );

private:

    /**
    * Constructors.
    */
    CSysApMMCObserver(
        RFs& aFs,
        CSysApDriveList& aSysApDriveList,
        MSysApMemoryCardObserver& aObserver,
        const TBool aHotSwapSupported );

    void ConstructL();

private: // Data
    // Reference to open file server session
    RFs& iFs;

    // Reference to the drive list
    CSysApDriveList& iSysApDriveList;

    // Reference to the observer
    MSysApMemoryCardObserver& iObserver;

    // Indicates if memory card hotswap is supported
    TBool iHotSwapSupported;

    // Pointer to disk notify handler. Own.
    CDiskNotifyHandler* iDiskNotifyHandler;

    };

#endif // RD_MULTIPLE_DRIVE

#endif // __SYSAPMMCOBSERVER_H__

// End of File
