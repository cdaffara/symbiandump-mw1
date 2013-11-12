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
* Description:  Application state source context definition.
*
*/



#ifndef APPLICATIONSTATESOURCECONTEXTDEF_H
#define APPLICATIONSTATESOURCECONTEXTDEF_H

#include <cfdefinitions.h>

/**
* Source
*/
_LIT( KApplicationStateSource, "Application.State" );

/**
* Type
*/
_LIT( KApplicationStateForegroundApplicationType,     "Foreground.Application" );
_LIT( KApplicationStateForegroundApplicationViewType, "Foreground.View" );
_LIT( KApplicationStateBackgroundApplicationType,     "Background.Application" );

/**
* Default value
*/
_LIT( KApplicationStateDefaultValue, "Other" );

/**
 * Description:
 *  Describes the foreground application orientation context.
 * 
 * Source:
 *  Application.State
 * 
 * Type:
 *  Foreground.Application.Orientation
 * 
 * Values:
 *  Portait
 *  Landscape
 *  Automatic
 * 
 * Capabilities:
 *  ECapabilitySwEvent
 */

// Type
_LIT( KFgApplicationOrientation, "Foreground.Application.Orientation" );

// Values
_LIT_CONTEXT_VALUES( KFgApplicationOrientationVal ) =
    {
    _S( "Portrait" ),
    _S( "Landscape" ),
    _S( "Automatic" )
    };

enum TFgApplicationOrientationVal
    {
    EFgApplicationOrientationPortrait,
    EFgApplicationOrientationLandscape,
    EFgApplicationOrientationAutomatic
    };

// Security
_LIT_SECURITY_POLICY_C1( KFgApplicationOrientationSec, ECapabilitySwEvent );

#endif // APPLICATIONSTATESOURCECONTEXTDEF_H
