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
* Description:  Audio thread proxy classes
*
*/

#ifndef CCMRAUDIOTHREADPROXYSERVER_H
#define CCMRAUDIOTHREADPROXYSERVER_H

// INCLUDES

#include "CCMRAudioThreadProxy.h"



//CLASS DECLARATIONS



/**
*  Audio thread proxy server class
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class CCMRAudioThreadProxyServer : public CCMRThreadProxyServer
    {
    public: // constructor and destructor
        /**
        * Two-phased constructor.
        */
        static CCMRAudioThreadProxyServer* NewL(RServer2* aServer2);
        /**
        * Destructor.
        */
        ~CCMRAudioThreadProxyServer();

    public: // new functions

        /**                     
        * Start thread
        * @param TAny* aAny
        * @return TInt error code
        */
        static TInt StartThread(TAny* aAny);
    
    protected: // new functions
        /**                     
        * Start thread
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
        CCMRAudioThreadProxyServer(TInt aPriority);
        /**
        * Symbian 2nd phase constructor
        */
        void ConstructL(RServer2* aServer2);

    };

#endif      
            
// End of File



