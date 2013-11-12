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
* Description:  PSC server session adapter
 *
*/


#ifndef XIMPPSCSERVERSESSIONADAPTER_H
#define XIMPPSCSERVERSESSIONADAPTER_H

#include <e32base.h>
#include "ximpsrvsessionadapter.h"

class MXIMPSrv;
class MXIMPSrvSession;
class MXIMPSrvMessage;


/**
 * Session adapter for XIMP Framework PSC server. 
 */
class CXIMPServerSessionAdapter : public CXIMPSrvSessionAdapter
    {
public:     // Constructors and destructors
    static CXIMPServerSessionAdapter* NewL( TUint32 aSessionId,
                                               MXIMPSrv& aServer );
    virtual ~CXIMPServerSessionAdapter();

protected:  // C++ constructors
    CXIMPServerSessionAdapter( TUint32 aSessionId,
                                  MXIMPSrv& aServer );


public:     // From CXIMPSrvSessionAdapter

    /**
     * Template method to instantiate concrete 
     * PSC session objects.
     *
     * @param aMessage The session instantiation message.
     *
     * @return Session implementation object or NULL if the message
     *         isn't valid session instantiation request.
     */
    MXIMPSrvSession* DoInstantiateSessionL( const MXIMPSrvMessage& aMessage );


private:    // Data

    /**
     * Identifier for this session. 
     */
    TUint32 iSessionId;
    };

#endif      //  XIMPPSCSERVERSESSIONADAPTER_H

