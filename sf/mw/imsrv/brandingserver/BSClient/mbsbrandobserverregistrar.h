/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MBSBrandObserverRegistrar.h
*
*/




#ifndef __MBSBRANDOBSERVERREGISTRAR_H
#define __MBSBRANDOBSERVERREGISTRAR_H

#include <e32std.h>


/**
 * An interface through which client can take into use
 * available brand data updates.
 *
 * @since
 */
class MBSBrandObserverRegistrar
    {
    public:

    	/*
    	* Registeres the brand observer to server
    	*/
        virtual void RegisterObserverToServerL( TRequestStatus& aStatus) = 0;


    	/*
    	* Gets the new updated version number 
    	*/
        virtual TInt GetNewVersionL() = 0;
        
        virtual TInt GetBackupRestoreL() = 0;
        
        virtual TInt GetBackupStateL() = 0;

protected:
    	/*
    	* Destructor
    	*/
    	virtual ~MBSBrandObserverRegistrar() {};
    };

#endif //__MBSBRANDOBSERVERREGISTRAR_H
