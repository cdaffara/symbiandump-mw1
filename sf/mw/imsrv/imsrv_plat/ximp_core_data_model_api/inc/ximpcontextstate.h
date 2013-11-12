/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface for XIMP context state object.
*
*/

#ifndef MXIMPFWCONTEXTSTATE_H
#define MXIMPFWCONTEXTSTATE_H

#include <e32std.h>
#include <ximpbase.h>
#include <ximpdatamodelifids.hrh>


/**
 * Interface for XIMP context state object.
 *
 * XIMP context state object presents state of the
 * XIMP context.
 *
 * @ingroup ximpdatamodelapi
 * @since S60 v3.2
 */
class MXIMPContextState : public MXIMPBase

    {
public:

    /** Interface ID for the MXIMPContextState. */
    enum { KInterfaceId = XIMP_IF_ID_CONTEXT_STATE };


    /** XIMP context states. */
    enum TState
        {
        /** Context is inactive */
        EInactive = 0,

        /** Context is binding to service */
        EBinding,

        /** Context is active and binded to service */
        EActive,

        /** Context is unbinding from service */
        EUnbinding
        };


public:

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MXIMPContextState() {}


public:

    /**
     * Gets the XIMP Context state value.
     *
     * @return The XIMP Context state value.
     */
    virtual TState ContextState() const = 0;

    };


#endif // MXIMPFWCONTEXTSTATE_H


