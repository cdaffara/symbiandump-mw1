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
* Description:  start the server
*
*/



#ifndef __IMCACHEPROCESSSTARTER_H__
#define __IMCACHEPROCESSSTARTER_H__

#include <e32std.h>

/**
 * Generic process starter.
 */
class IMCacheProcessStarter
    {
    public:     // New methods

		/**
    	* read full path for client 
    	* @param aExeName the , name of server exe
    	* @param aFullExePath path of server exe
    	*/
        static void FullExePathForClientLocation(
                        const TDesC& aExeName,
                        TFileName& aFullExePath );

		/**
    	* StartInstance the instance of server
    	* @param aFullExePath path of server exe
    	* @param aCommand the , to excute 
    	* @param aParams, starter parameter
    	*/
        static TInt StartInstance(
                        const TDesC& aFullExePath,
                        const TDesC& aCommand
                        );
		
		/**
    	* connect to the server
    	* @param aFullExePath path of server exe
    	* @param aCommand the , to excute 
       	* @param aSessionToConnect, sesion to be used
		* @param aServerName, server  name 
		* @param aClientVersion, client version
		* @param aAsyncMessageSlots, slots number
		*/
        static TInt ConnectToServer(
                        const TDesC& aFullExePath,
                        const TDesC& aCommand,
                        RSessionBase& aSessionToConnect,
                        const TDesC& aServerName,
                        const TVersion& aClientVersion,
                        TInt aAsyncMessageSlots );



    private:    // Helpers
    
    	/**
    	* DoStartInstance
        * @param aFullExePath path of server exe
        * @param aCommand the , to excute 
    	* @param aParams, starter parameter
    	*/
        static TInt DoStartInstance(
                        const TDesC& aFullExePath,
                        const TDesC& aCommand );
                        
        

    };

#endif      //  __IMCACHEPROCESSSTARTER_H__


//END OF FILE


