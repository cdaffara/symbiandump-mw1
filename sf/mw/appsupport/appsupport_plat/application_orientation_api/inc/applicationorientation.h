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
* Description:  Default Application Orientation API
*
*/


#ifndef APPLICATIONORIENTATION_H
#define APPLICATIONORIENTATION_H

#include <e32std.h>

// =============================================================================
// Application Default Orientation API
// =============================================================================

const TUid KCRUidDefaultAppOrientation = {0x200159ED};

/**
* The KCRUidAppOrientation repository contains keys which are mapped directly
* to the application SID. This key will define the application default
* orientation when the application is launched.
* 
* The key can have one of the following values:
* - Not set   = 0x00000000 (0)
* - Portrait  = 0x00010000 (65536)
* - Landscape = 0x00030000 (196608)
* - Automatic = 0x00050000 (327680)
*
* For more information,
* @see Application Framework API.
*/

#endif
