// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef __TZIDINTERNAL_H__
#define __TZIDINTERNAL_H__

#include <e32def.h>

/**
The minimum valid value for a user-defined time zone numeric identifier.

@internalComponent
*/
const TUint KUserTzIdMin = 0x4000;


/**
The maximum valid value for a user-defined time zone numeric identifier.

@internalComponent
*/
const TUint KUserTzIdMax = 0x4FFF;


/**
Represents the maximum allowed value for a numeric identifier.
Some of the special values are beyond this maximum though.

@internalComponent
*/
const TUint KTzIdMax = 0x0000FFFF;

#endif
