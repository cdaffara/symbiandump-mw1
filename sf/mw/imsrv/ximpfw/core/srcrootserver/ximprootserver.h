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
* Description:  Root server implementation
 *
*/

#ifndef CXIMPROOTSERVER_H
#define CXIMPROOTSERVER_H

#include "ximpsrv.h"
#include <e32base.h>
#include <ximpbase.h>

class CXIMPGlobals;
class MXIMPSrvTestListener;

/**
 * Presence Root server implementation.
 * There exists one root server.
 */
class CXIMPRootServer : public CPolicyServer,
                        public MXIMPSrv
    {
public:     // Execution and destructor

    static void ExecuteL();
    virtual ~CXIMPRootServer();


private:    //Constructors

    static CXIMPRootServer* NewLC();
    CXIMPRootServer();
    void ConstructL();


public:     // From CPolicyServer

    CSession2* NewSessionL( const TVersion& aVersion,
                            const RMessage2& aMessage ) const;


private:     // From MXIMPSrv

    void SessionCreated();
    void SessionDied();    

private:    //Data

    // OWN: Count of sessions
    TInt iSessionCount;

    // OWN: pointer to globals singleton
    CXIMPGlobals* iGlobals;

    // Always increasing session id counter. Stays unique per server for
    // up to 2^32-1 invocations of NewSessionL.
    TUint32 iSessionIdCounter;
    };

#endif      // CXIMPROOTSERVER_H




