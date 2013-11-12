/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Video thread proxy server class
*
*/

#ifndef CCMRVIDEOTHREADPROXYSERVER_H
#define CCMRVIDEOTHREADPROXYSERVER_H

// INCLUDES
#include "CCMRVideoThreadProxy.h"



// DATA TYPES



//CLASS DECLARATIONS



/**
*  Video thread proxy server class
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class CCMRVideoThreadProxyServer : public CCMRThreadProxyServer
    {
    public: // constructor and destructor
        /**
        * Two-phased constructor.
        */
        static CCMRVideoThreadProxyServer* NewL(RServer2* aServer2);
        /**
        * Destructor.
        */
        ~CCMRVideoThreadProxyServer();

    public: // new functions

        /**                     
        * Start thread
        * @since 2.1
        * @param TAny* aAny
        * @return TInt error code
        */
        static TInt StartThread(TAny* aAny);
    
    protected: // new functions
        /**                     
        * Start thread
        * @since 2.1
        * @return void
        */
        static void DoStartThreadL(TAny* aAny);

    protected: // from base classes
        /**                     
        * From CCMRThreadProxyServer/CServer create a new session
        */
		CSession2* NewSessionL(const TVersion& aVersion, const RMessage2& aMessage) const;

    private: // constructors
        /**
        * C++ default constructor.
        */
        CCMRVideoThreadProxyServer(TInt aPriority);
        /**
        * Symbian 2nd phase constructor
        */
        void ConstructL(RServer2* aServer2);

    };


#endif      
            
// End of File



