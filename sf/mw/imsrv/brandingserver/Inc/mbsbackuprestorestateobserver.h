/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MBS Backup restore observer definition.
*
*/




#ifndef __MBSBACKUPRESTORESTATEOBSERVER_H
#define __MBSBACKUPRESTORESTATEOBSERVER_H

#include <e32std.h>


/**
 * An interface through which client gets information
 * about available brand data updates.
 *
 * @since
 */
class MBSBackupRestoreStateObserver
    {
    public:

    	/*
    	* Gets called when backup restore operation begins or ends
    	* Client can then decide wether to store accessing date or not
    	* @return ETrue if observer should continue observing
    	*/
		virtual TBool BackupRestoreActivated(TInt activeState) = 0 ;

protected:
    	/*
    	* Destructor
    	*/
    	virtual ~MBSBackupRestoreStateObserver() {};
    };

#endif //__MBSBACKUPRESTORESTATEOBSERVER_H
