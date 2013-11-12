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
* Description:  Drive unlock handler implementation
 *
*/


#ifndef SYSAPDRIVEUNLOCKHANDLER_H
#define SYSAPDRIVEUNLOCKHANDLER_H

// INCLUDES
#include <e32std.h>
#include <aknmemorycarddialog.h>
#include "hbdeviceinputdialogsymbian.h"

// FORWARD DECLARATIONS
class CSysApDriveList;
class CSysApAppUi;

// CLASS DECLARATION
class CSysApDriveUnlockHandler : public CBase                             
{
public:
    /**
     * Two-phased constructor.
     */
    static CSysApDriveUnlockHandler* NewL(
        CSysApDriveList& aSysApDriveList,
        CSysApAppUi& aSysApAppUi,
        const TBool aMemoryCardLockSupported );

    /**
     * Destructor
     */
    ~CSysApDriveUnlockHandler();

public: // New methods

    /**
     * Starts unlock notifier if not already ongoing
     */
    void StartUnlock();

    /**
     * Stops unlock notifier for given drive if currently shown.
     * @param aDrive Drive to stop unlock notifier if ongoing
     */
    void StopUnlock( TInt aDrive );
    
    void ReleaseMemoryForInputCardDialog();
    TInt CheckMemoryDialogIfNeeded();
    void ConvertCharactersToPwd(TDesC& aWord, TDes8& aConverted);
    
    /**
    * Shows unlock query.
    */
    void ShowUnlockQueryL();
    HBufC8* Convert16to8L(TDesC16& aStr);
    void UnlockComplete( TInt aResult );

private:
    /**
    * Constructor.
    */
    CSysApDriveUnlockHandler(
        CSysApDriveList& aSysApDriveList,
        CSysApAppUi& aSysApAppUi,
        const TBool aMemoryCardLockSupported );
    
    /**
    * Handles query show from CAsyncCallBack.
    */
    static TInt QueryShowCB( TAny* aPtr );

    /**
    * Checks is unlock query ongoing.
    */
    TBool IsQueryOngoing() const;

    /**
    * Start query if drives left to be unlocked.
    */
    void DoStartQueryIfNeededL();

    /**
    * Start query using CAsyncCallBack if drives left to be unlocked.
    */
    void DoStartQueryAsyncIfNeeded();

    /**
    * Stops unlock.
    */
    void DoStopUnlock( TInt aError );

private: // Data
    // Reference to the drive list
    CSysApDriveList& iSysApDriveList;

    // Reference to the app ui
    CSysApAppUi& iSysApAppUi;

    // Indicates if memory card locking is supported
    TBool iMemoryCardLockSupported;
	
	CHbDeviceInputDialogSymbian* iMemCardPwdDialog;
    
	// Current drive to be unlocked
    TInt iDriveToUnlock;

    // Indicates if the query result can be ignored i.e. query was canceled
    TBool iIgnoreQueryResult;
    
    TMediaPassword iPassword;
};

#endif // SYSAPDRIVEUNLOCKHANDLER_H

// End of File
