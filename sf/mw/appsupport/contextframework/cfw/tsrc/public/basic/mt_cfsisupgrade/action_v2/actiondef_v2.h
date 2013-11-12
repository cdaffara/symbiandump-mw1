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

#ifndef ACTIONDEF_V2_H_
#define ACTIONDEF_V2_H_

#include <e32std.h>

_LIT( KAction_V2Id, "action_V2" );

const TUid KAction_V2PSUid = {0x0100FFF6};
const TInt KAction_V2PSCategory = 0x00000001;
enum TAction_V2PSValues
    {
    EAction_V2Triggered,
    EAction_V2NotTriggered,
    };

#endif /*ACTIONDEF_V2_H_*/
