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
* Description:  XIMP Framework server test session.
*
*/

#ifndef PRFWSRVTESTCLIENT_H__
#define PRFWSRVTESTCLIENT_H__

#include <ximpbase.h>
#include <e32std.h>
#include "prfwtestserverdefs.h"


/**
 * XIMP Framework server test session.
 *
 * @lib ximpsrvclient.dll
 * @since S60 v3.2
 */
class RXIMPSrvTestClient : public RSessionBase
    {
public:

    /**
     * Default constructor.
     */
    RXIMPSrvTestClient();

    /**
     * Session connection method.
     *
     * @return Error code from creating server connection.
     */
    TInt Connect();
    void ConnectL( TUid aProtocolImpUid );

    /**
     * Closes server connection.
     */
    void Close();
    
    /**
     * Set alloc failure
     */
    void SetAllocFailure( TInt aRate );
    NTestSrv::SErrorData RXIMPSrvTestClient::FetchErrorData();

private:

    /**
     * Helper to get data from server side.
     * @return Externalized object to be passed onward
     */
    NTestSrv::SErrorData GetData( TInt aMsg );
    };


#endif      //  PRFWSRVTESTCLIENT_H__

// END OF FILE
