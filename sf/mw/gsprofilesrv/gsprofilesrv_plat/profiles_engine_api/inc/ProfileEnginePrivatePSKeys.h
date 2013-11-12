/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Profiles engine internal Publish & Subscribe keys.
*
*/



#ifndef PROFILEENGINEPRIVATEPSKEYS_H
#define PROFILEENGINEPRIVATEPSKEYS_H

//  INCLUDES
#include <e32std.h>
#include "ProfileEngineInternalPSKeys.h"

// CONSTANTS

/**
 * A setting in the current active profile was modified. This is used
 * internally by Profiles Engine notification mechanism.
 * Default value: 0.
 */
const TUint32 KProEngActiveProfileModified = 0x7D000001;

/**
 * The current active profile has changed (or the active profile has been
 * reactivated). This is used internally by Profiles Engine notification
 * mechanism.
 * Default value: 0.
 */
const TUint32 KProEngActiveProfileChanged = 0x7D000002;

/**
 * Power save mode has changed.
 * This is used internally by Profiles Engine.
 * Default value: 0.
 */
const TUint32 KProEngPsmChanged = 0x7D000003;

#endif      // PROFILEENGINEPRIVATEPSKEYS_H

// End of File
