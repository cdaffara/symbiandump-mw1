/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Test support file tool
 *
*/


#ifndef CPRFWTESTFILELOGICHELPER_H
#define CPRFWTESTFILELOGICHELPER_H

#include <e32base.h>
#include "prfwtestfileobserver.h"

class MXIMPProtocolConnectionHost;
class CXIMPTestMessenger;

/**
 * @since S60 v4.0
 */
class CXIMPTestFileLogicHelper : public CBase, public MXIMPTestFileObserver
    {
    public:

        IMPORT_C static CXIMPTestFileLogicHelper* NewL( MXIMPProtocolConnectionHost& aHost, CXIMPTestMessenger& aMessenger  );
        virtual ~CXIMPTestFileLogicHelper();

    private:

        CXIMPTestFileLogicHelper( MXIMPProtocolConnectionHost& aHost, CXIMPTestMessenger& aMessenger );
        void ConstructL();

    public:


    private: // From MXIMPTestFileObserver

        /**
         * @see MXIMPTestFileObserver
         */
        virtual void HandleL( TInt aCommandType, TDesC8& aCommand );

    private: // data

        /**
         * Access to host for server messages.
         */
        MXIMPProtocolConnectionHost& iHost;
        
        /**
         * Does not own. Messenger for signaling.
         */
        CXIMPTestMessenger& iMessenger;
    };

#endif // CPRFWTESTFILELOGICHELPER_H
