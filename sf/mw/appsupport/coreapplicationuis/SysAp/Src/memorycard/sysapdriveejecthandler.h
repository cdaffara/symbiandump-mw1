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


#ifndef SYSAPDRIVEEJECTHANDLER_H
#define SYSAPDRIVEEJECTHANDLER_H

// INCLUDES
#include <e32std.h>
#include <disknotifyhandler.h>

// FORWARD DECLARATIONS
class CSysApDriveList;
class CSysApAppUi;

// CLASS DECLARATION
class CSysApDriveEjectHandler : public CBase,
                                public MDiskNotifyHandlerCallback
{
public:
    /**
     * Two-phased constructor.
     */
    static CSysApDriveEjectHandler* NewL(
        CSysApDriveList& aSysApDriveList,
        CSysApAppUi& aSysApAppUi,
		RFs& aFs );

    /**
     * Destructor
     */
    ~CSysApDriveEjectHandler();

public: // New methods

    /**
     * Starts eject if not already ongoing
     */
    void StartEject();

    /**
     * Completes drive's dismount.
     * Note that this must be called when the dialog launched from 
     * CSysApAppUi::ShowMMCDismountedDialogL has been closed.
     *
     * @param aDrive Drive to be dismounted
     * @return ETrue if drives left waiting to be dismounted, otherwise EFalse.
     */
    TBool CompleteDismount( TInt aDrive );

private:
    /**
    * Constructors.
    */
    CSysApDriveEjectHandler(
        CSysApDriveList& aSysApDriveList,
        CSysApAppUi& aSysApAppUi );

    void ConstructL( RFs& aFs );

    /**
    * Starts eject.
    */
    TBool DoStartEjectL();

    /**
    *  Stops eject.
    */
    void StopEject( TInt aError );

private: // From MDiskNotifyHandlerCallback
    void HandleNotifyDismountFinished(
        TInt aError,
        const TDismountFinishedEvent& aEvent );

private: // Data
    // Reference to the drive list
    CSysApDriveList& iSysApDriveList;

    // Reference to the app ui
    CSysApAppUi& iSysApAppUi;

    // Current drive to be ejected
    TInt iDriveToEject;

    // For controlled dismount with timeout. Owned.
    CDiskNotifyHandler* iDiskNotifyHandler;

};

#endif // SYSAPDRIVEEJECTHANDLER_H

// End of File
