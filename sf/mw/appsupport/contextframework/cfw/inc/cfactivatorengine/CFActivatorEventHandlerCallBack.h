/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MCFActivatorEventHandlerCallBack interface declaration.
*
*/


#ifndef M_ACTIVATOREVENTHANDLERCALLBACK_H
#define M_ACTIVATOREVENTHANDLERCALLBACK_H


// Deprecation warning
// This header file has been deprecated. Remove at next BC break.

/**
 *  Event handler call back.
 *  CCFActivatorEventHandler uses this interface to asynchronously
 *  trigger actions.
 *
 *  @lib -
 *  @since S60 5.0
 */
 class MCFActivatorEventHandlerCallBack
    {
    protected:

        // Deny destruction through this interface.
        virtual ~MCFActivatorEventHandlerCallBack(){};
    };

#endif
