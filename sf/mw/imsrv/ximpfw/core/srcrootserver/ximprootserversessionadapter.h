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
* Description:  XIMP Framework Root server session adapter
 *
*/


#ifndef XIMPROOTSESSIONADAPTER_H
#define XIMPROOTSESSIONADAPTER_H

#include <e32base.h>
#include "ximpsrvsessionadapter.h"

class MXIMPSrv;
class MXIMPSrvSession;
class MXIMPSrvMessage;


/**
 * Session adapter for XIMP Framework Root server. 
 */
class CXIMPRootServerSessionAdapter : public CXIMPSrvSessionAdapter
    {
public:     // Constructors and destructors
    static CXIMPRootServerSessionAdapter* NewL( MXIMPSrv& aServer );
    virtual ~CXIMPRootServerSessionAdapter();

protected:  // C++ constructors
    CXIMPRootServerSessionAdapter( MXIMPSrv& aServer );


public:     // From CXIMPSrvSessionAdapter

    /**
     * Template method to instantiate concrete 
     * Root session objects.
     *
     * @param aMessage The session instantiation message.
     *
     * @return Session implementation object or NULL if the message
     *         isn't valid session instantiation request.
     */
    MXIMPSrvSession* DoInstantiateSessionL( const MXIMPSrvMessage& aMessage );

    };

#endif      //  XIMPROOTSESSIONADAPTER_H


