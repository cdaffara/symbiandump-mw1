/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:
*
*/

#ifndef ACTIONDEF_V1_H_
#define ACTIONDEF_V1_H_

#include <e32std.h>

_LIT( KAction_V1Id, "action_v1" );

const TUid KAction_V1PSUid = {0x0100FFF5};
const TInt KAction_V1PSCategory = 0x00000001;
enum TAction_V1PSValues
    {
    EAction_V1Triggered,
    EAction_V1NotTriggered,
    };

#endif /*ACTIONDEF_V1_H_*/
