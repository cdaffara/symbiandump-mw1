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
* Description:   Gets and notifies backup and Restore events from system
*
*/



#ifndef CBSBACKUPOBSERVER_H
#define CBSBACKUPOBSERVER_H

#include <e32base.h>
#include <e32property.h>

#include "mbsbackupobserver.h"

/**
 *  Gets and notifies backup and Restore events from system
 *
 *  @lib bsserver.exe
 *  @since S60 v3.2
 */
class CBSBackupObserver : public CActive
{
public:

    static CBSBackupObserver* NewL();

    static CBSBackupObserver* NewLC();

    ~CBSBackupObserver();


private: // From CActive

    /**
     * @see CActive
     */
    void RunL();
    
    /**
     * @see CActive
     */
    void DoCancel();
    
    /**
     * @see CActive
     */
    TInt RunError( TInt aError );


private: // Private constructors

    CBSBackupObserver();

    void ConstructL();


public: // New Methods

    /**
     * Register observer to observe backup state changes
     * @param aObserver observer to register
     * @return KErrNone or some other system wide errorcode
     */
    TInt RegisterObserver( MBSBackupObserver* aObserver );
    
    /**
     * Unregister observer
     * @param aObserver observer to remove
     */
    void UnregisterObserver( MBSBackupObserver* aObserver );   


private: // New methods

    /**
     * Subscribes to backup and restore P&S value.
     */
    void Subscribe();
    
    /**
     * Notify registered observers of new backup state
     */
    void NotifyObserversL( MBSBackupObserver::TBackupState aState );
    
    /**
     * Read current backup state
     * @return current backup state
     */
    MBSBackupObserver::TBackupState BackupStateL();


private: // Data

    // backup state property
    RProperty iProperty;
    
    // Own (array only). Observers
    RPointerArray<MBSBackupObserver> iObservers;
    
    // latest state of backup and restore;
    MBSBackupObserver::TBackupState iLatestState;
};

#endif //CBSBACKUPOBSERVER_H
