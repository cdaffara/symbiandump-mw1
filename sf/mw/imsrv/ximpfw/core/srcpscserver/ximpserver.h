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
* Description:  XIMP Framework PSC server class
 *
*/

#ifndef CXIMPPSCSERVER_H
#define CXIMPPSCSERVER_H

#include "ximpsrv.h"
#include <ximpbase.h>
#include "ximpglobalsobserver.h"

#include <e32base.h>


class CXIMPGlobals;
class MXIMPSrvTestListener;


/**
 * Presence Service Connection (PSC) server implementation.
 * There exists one PSC server per protocol.
 */
class CXIMPServer : public CPolicyServer,
                       public MXIMPSrv,
                       public MXIMPGlobalsObserver
    {
public:     // Execution and destructor
    static void ExecuteL();
    virtual ~CXIMPServer();

public: // from MXIMPGlobalsObserver
    void AllHostsDied();

private:    // Constructors
    static CXIMPServer* NewLC();    
    CXIMPServer();
    void ConstructL();
    
    
private:     // From CPolicyServer
    CSession2* NewSessionL( const TVersion& aVersion,
                            const RMessage2& aMessage ) const;


private:     // From MXIMPSrv
    void SessionCreated();
    void SessionDied();
    MXIMPSrvTestListener* TestListener() const; 
    void RegisterTestListener( MXIMPSrvTestListener* aTestSession );

private: // Helper
    
    void TryToStopScheduling();

private:    //Data

    // OWN: Count of sessions
    TInt iSessionCount;
    TBool iAllHostsDied;


    // OWN: Always increasing session id.
    TUint32 iSessionId;

    // OWN: pointer to globals singleton
    CXIMPGlobals* iGlobals;
    
    // Does not own. Pointer to test listener.
    MXIMPSrvTestListener* iTestListener;
    };

#endif      // CXIMPPSCSERVER_H




