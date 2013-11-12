/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  XIMP framework im operation definitions.
 *
*/

#ifndef IMOPERATIONDEFS_H
#define IMOPERATIONDEFS_H

#include <e32std.h>

// TODO: use ranges for operations types, that will make sure that
// the core, Presence and IM operations won't overlap

/**
 * PSC operations.
 */
namespace NImOps
    {
    enum TImOpTypes
        {
        // -----------------------------------------------
        // From client
        //
        ESendImMessage = 200,                          // 200
        ESendImMessageToGroup,                         // 201
        EHandleSentMessage,       
        EHandleReceiveMessage,      
        ESynthesiseImSubscription = 280,
        EIMCleaner = 290            
        };
    }

#endif  //IMOPERATIONDEFS_H





