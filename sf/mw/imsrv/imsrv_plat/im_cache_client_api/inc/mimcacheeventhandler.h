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
* Description:  client side obssrver class message change observer
*
*/



#ifndef __MIMCACHEEVENTHANDLER_H
#define __MIMCACHEEVENTHANDLER_H

#include <e32std.h>
#include "imcachedefs.h"


/**
 * An interface through which client gets information
 * about available  data updates.
 *
 * @since
 */
class MIMCacheEventHandler
    {
    public:
           
    	/**
    	* Gets called when data update is available.
    	* Client can then decide wether to use the old one
    	* or the updated one.
    	* @param aNewMessage, new message details
       	*/
        virtual void HandleIMCacheEventL(TIMCacheEventType aEvent, TAny* aChatMessage = NULL ) = 0;
     
  	
protected:
    	/**
    	* Destructor
    	*/
    	virtual ~MIMCacheEventHandler() {};
    };
   
 
    
#endif //__MIMCACHEEVENTHANDLER_H


