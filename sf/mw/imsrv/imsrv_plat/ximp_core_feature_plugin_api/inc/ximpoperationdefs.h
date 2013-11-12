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
* Description:  Presence framework operation definitions.
 *
*/

#ifndef XIMPOPERATIONDEFS_H
#define XIMPOPERATIONDEFS_H

#include <e32std.h>

/**
 * PSC operations.
 */
namespace NXIMPOps
    {
    enum TXIMPOpTypes
        {
        EPrNone = 0,     // base class

        // -----------------------------------------------
        // From client
        //
        EXIMPBindContext,                                // 1
        EXIMPUnbindContext,                              // 2
        EXIMPBindContext2,                               // 3

        // Session mgmt internal
        EXIMPForcedTeardown = 100,                // 100
        };
    }

#endif  //XIMPOPERATIONDEFS_H





