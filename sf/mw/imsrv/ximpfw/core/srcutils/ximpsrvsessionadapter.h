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
* Description:  Server session adapter base class.
 *
*/


#ifndef XIMPSRVSESSIONADAPTER_H
#define XIMPSRVSESSIONADAPTER_H

#include <e32base.h>

class MXIMPSrv;
class MXIMPSrvSession;
class MXIMPSrvMessage;
class CXIMPSrvMessageImp;


/**
 * Server session adapter base class.
 *
 * @lib ximputils.dll
 * @since S60 v3.2
 */
class CXIMPSrvSessionAdapter : public CSession2
    {
public:     // Constructor and destructor

    IMPORT_C CXIMPSrvSessionAdapter( MXIMPSrv& aServer );
    IMPORT_C ~CXIMPSrvSessionAdapter();


public:     // Methods derived from CSession2
    
    IMPORT_C void ServiceL( const RMessage2& aMessage );
    IMPORT_C void ServiceError( const RMessage2& aMessage,
                                TInt aError );


public:     // New template methods 

    /**
     * Session instantiation template method for
     * derived implementations.
     *
     * @param aMessage The session instantiation message.
     *
     * @return Session implementation object or NULL if the message
     *         isn't valid session instantiation request.
     */
    virtual MXIMPSrvSession* DoInstantiateSessionL( const MXIMPSrvMessage& aMessage ) = 0;    
    
protected:
    
    /**
     * Access to server for derived implementations.
     * @return Access to server object.
     */
    IMPORT_C MXIMPSrv& Server();

private:    // Helpers 

    TBool IsCurrentMessageValid();

private:    // Data

    /**
     * Owning server.
     * Referenced.
     */
    MXIMPSrv& iServer;

    /**
     * Real session implementation.
     * Owned.
     */
    MXIMPSrvSession* iSessionImp;

    /**
     * Currently handled message.
     * Does not own.
     */
    CXIMPSrvMessageImp* iCurrentMsg;
    
    /**
     * Owned.
     */
    CXIMPSrvMessageImp* iMessage;    
    };

#endif      //  XIMPSERVERSESSIONADAPTER_H

