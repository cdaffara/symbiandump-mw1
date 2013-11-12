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
* Description:  Server Message interface.
 *
*/

#ifndef MXIMPSRV_H
#define MXIMPSRV_H

#include <e32base.h>

/**
 * Server interface for generic server message handling.
 */
class MXIMPSrv
    {
public:

    /**
     * Notifies server from instantiation of new session.
     */
    virtual void SessionCreated() = 0;

    /**
     * Notifies server from cleanup of existing session.
     */
    virtual void SessionDied() = 0;

protected:

    /**
     * Protected destructor.
     * Objects can't be deleted through this interface.
     */
    virtual ~MXIMPSrv() {};

    };


#endif      //  MXIMPSRV_H


