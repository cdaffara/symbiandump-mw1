/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Display source context definition.
*
*/



#ifndef DISPLAYSOURCECONTEXTDEF_H
#define DISPLAYSOURCECONTEXTDEF_H

#include <cfdefinitions.h>

// Context source: 'Device'
_LIT( KDisplaySource, "Device" );

//-----------------------------------------------------------------------------
// Display.Status.Light
//
// Listens P&S key related to display statuses.
//-----------------------------------------------------------------------------
//
// Type definition
_LIT( KDisplayStatusType, "Display.Status.Light" );

// Value definitions
const TInt KDisplayStatusTypeValueCount = 4;
_LIT_CONTEXT_VALUES( KDisplayStatusTypeValues ) =
    {
    _S( "Unknown" ),
    _S( "On" ),
    _S( "Off" ),
    _S( "Blinking" )
    };

// Security definition
_LIT_SECURITY_POLICY_C2( KDisplayStatusTypeSec,
    ECapabilityReadDeviceData,
    ECapabilityWriteDeviceData );

//-----------------------------------------------------------------------------
// Display.UI.Layout
//
// Describes the current UI layout.
//-----------------------------------------------------------------------------
//
// Type definition
_LIT( KDisplayUILayoutType, "Display.UI.Layout" );

// Value definitions
_LIT_CONTEXT_VALUES( KDisplayUILayoutValues ) =
    {
    _S( "Unknown" ),
    _S( "Portrait" ),
    _S( "Landscape" )
    };

enum TDisplayUILayoutValues
    {
    EUILayoutValueUnknown,
    EUILayoutValuePortrait,
    EUILayoutValueLandscape
    };

// Security definition
_LIT_SECURITY_POLICY_C1( KDisplayUILayoutSec,
    ECapabilityReadDeviceData );

//-----------------------------------------------------------------------------
// Display.UI.Rotation
//
// Describes the current UI rotation.
//-----------------------------------------------------------------------------
//
// Type definition
_LIT( KDisplayUIRotationType, "Display.UI.Rotation" );

// Value definitions
// @see CFbsBitGc::TGraphicsOrientation for value enumeration
_LIT_CONTEXT_VALUES( KDisplayUIRotationValues ) =
    {
    _S( "Normal" ),
    _S( "Rotated90" ),
    _S( "Rotated180" ),
    _S( "Rotated270" ),
    };

// Security definition
_LIT_SECURITY_POLICY_C1( KDisplayUIRotationSec,
    ECapabilityReadDeviceData );

#endif // DISPLAYSOURCECONTEXTDEF_H
