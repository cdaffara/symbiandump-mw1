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
* Description:   Backup and Restore observer interface
*
*/



#ifndef __MBSBACKUPOBSERVER_H
#define __MBSBACKUPOBSERVER_H

#include <e32base.h>

/**
 * Observer interface for backup and restore events
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( MBSBackupObserver )
    {
    public:
    
        // Backup and restore states
        enum TBackupState
        	{
        	EBackupNotActive,
        	EBackupActive
        	};

    	/*
    	* Hanlder method to receive observer events
    	* @param aEvent new state of backup and restore
    	*/
		virtual void HandleBackupStateL( TBackupState aState ) = 0;
		
		/*
		* Destructor
		*/
	    virtual ~MBSBackupObserver() {};
    };

#endif //__MBSBACKUPOBSERVER_H
