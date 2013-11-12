/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Orientation action definitions.
 *
*/


#ifndef ORIENTATIONACTIONDEF_H
#define ORIENTATIONACTIONDEF_H

// SYSTEM INCLUDE FILES
#include <e32std.h>

//------------------------------------------------------------------------------
// Action ID:
//      Orientation
//
// Paramaters:
//      Mode='Normal' (set UI layout to primary mode)
//      Mode='Alternate' (set UI layout to alternate mode)
//      Mode='Portrait' (set UI layout to primary/portrait mode)
//      Mode='Landscape' (set UI layout to alternate/landscape mode)
//
// Capabilities:
//      ECapability_None
//
// Example:
//      <actions>
//          <Orientation Mode='Landscape'/>
//      </actions>
//------------------------------------------------------------------------------
//

// Action ID
_LIT( KOrientationAction,       "Orientation" );

// Parameters
_LIT( KOrientationParamMode,    "Mode" );

// Values
_LIT( KOrientationParamModeValNormal,       "Normal" );
_LIT( KOrientationParamModeValAlternate,    "Alternate" );
_LIT( KOrientationParamModeValPortrait,     "Portrait" );
_LIT( KOrientationParamModeValLandscape,    "Landscape" );

// Security policy
_LIT_SECURITY_POLICY_PASS( KOrientationActionSec );

#endif // ORIENTATIONACTIONDEF_H

