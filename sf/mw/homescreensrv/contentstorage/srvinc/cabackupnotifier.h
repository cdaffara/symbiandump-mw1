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

#ifndef CABACKUPNOTIFIER_H
#define CABACKUPNOTIFIER_H

#include <e32property.h>
#include "caclient_global.h"

class CCaStorageProxy;

/**
 *  CA Backup notifier.
 *
 *  @since S60 v5.0
 */


NONSHARABLE_CLASS( CCaBackupNotifier ) : public CActive
    {

enum TCaBackupState
	/** 
	The backup / restore state of a device.

	@released
	@publishedAll
	*/
	    {
	    ECaBackup       = 1,
	    ECaRestore      = 2
	    };

public:

    /**
     * Creates an instance of CCaBackupNotifier.
     * @param aNotifier Reference to notifier interface.
     * @param aNotificationType Notification type.
     */
static CCaBackupNotifier* NewL( CCaStorageProxy* aStorageProxy );

    /**
     * Destructor.
     */
    virtual ~CCaBackupNotifier();

private:

    /**
     * Symbian 2nd phase constructor.
     * @param aNotificationType Notification type.
     */
    void ConstructL( );

    /**
     * From CActive.
     */
    void DoCancel();

    /**
     * From CActive.
     */
    void RunL();

    /**
     * From CActive.
     */
    TInt RunError( TInt aError );

private:

    CCaBackupNotifier( CCaStorageProxy* aStorageProxy );
    
    /**
     * RProperty.
     * Own.
     */
    RProperty iProperty;
       
    /*
     * not owned
     */
    CCaStorageProxy* iStorageProxy;
    
    /*
     * Keeps last state of backup
     */
    TInt iLastState;

    CA_CLIENT_TEST_FRIEND_CLASS(TestCaClient)
    
    };

#endif /* CABACKUPNOTIFIER_H */

// End of File

