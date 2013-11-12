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
* Description:  MBSBrandChangeObserver 
*
*/




#ifndef __MBSBRANDCHANGEOBSERVER_H
#define __MBSBRANDCHANGEOBSERVER_H

#include <e32std.h>


/**
 * An interface through which client gets information
 * about available brand data updates.
 *
 * @since
 */
class MBSBrandChangeObserver
    {
    public:

    	/*
    	* Gets called when brand data update is available.
    	* Client can then decide wether to use the old one
    	* or the updated one.
    	* @return ETrue if observer should continue observing
    	*/
        virtual TBool BrandUpdateAvailable( TInt aNewVersion ) = 0;


protected:
    	/*
    	* Destructor
    	*/
    	virtual ~MBSBrandChangeObserver() {};
    };

#endif //__MBSBRANDCHANGEOBSERVER_H
