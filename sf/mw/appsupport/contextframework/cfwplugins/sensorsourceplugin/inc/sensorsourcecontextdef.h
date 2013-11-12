/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Sensor source context definition.
*
*/



#ifndef SENSORSOURCECONTEXTDEF_H
#define SENSORSOURCECONTEXTDEF_H

#include <cfdefinitions.h>

// Generic macro to convert context value in to desc
#define DESC( aList, aIndex ) (const TDesC&)TPtrC( aList[aIndex] )

//-----------------------------------------------------------------------------
// Sensor source command definitions.
//-----------------------------------------------------------------------------
//
// Command name
_LIT( KSensorCommandName, "sensorCommand" );

// Channel id attribute name
_LIT( KSensorChannelAttributeName, "channelId" );

// Command id attribute name
_LIT( KSensorCommandIdName, "id" );

// Feature id attribute name
_LIT( KSensorCommandFeatureIdName, "featureId" );

// Command id activate name
_LIT( KSensorCommandIdActivateName, "activate" );

// Command id deactivate name
_LIT( KSensorCommandIdDeactivateName, "deactivate" );

// General feature id name
_LIT( KSensorFeatureGeneralName, "fea.general.%d" );

//-----------------------------------------------------------------------------
// Source:  'Sensor'
//-----------------------------------------------------------------------------
//
_LIT( KSensorSource, "Sensor" );

//-----------------------------------------------------------------------------
// Source:  'Sensor'
// Type:    'Event'
//
// Describes supported sensor events.
//-----------------------------------------------------------------------------
//
_LIT( KSensorSourceEvent, "Event" );

// Context values
_LIT_CONTEXT_VALUES( KSensorSourceEventValues ) =
    {
    // Activates double tap channel
    _S( "DoubleTap.Activate" ),

    // Deactivates double tap channel
    _S( "DoubleTap.Deactivate" ),

    // Activates orientation channel
    _S( "Orientation.Activate" ),

    // Deactivates orientation channel
    _S( "Orientation.Deactivate" ),

    // Activates proximity channel
    _S( "Proximity.Activate" ),

    // Deactivates proximity channel
    _S( "Proximity.Deactivate" )
    };

// Enumerations for accessing values array
enum TSensorSourceEventValues
    {
    EEventDoubleTapActivate,
    EEventDoubleTapDeactivate,
    EEventOrientationActivate,
    EEventOrientationDeactivate,
    EEventProximityActivate,
    EEventProximityDeactivate,
    EEventCount
    };

// Capabilities
_LIT_SECURITY_POLICY_PASS( KSensorSourceEventSec );

//-----------------------------------------------------------------------------
// Source:  'Sensor'
// Type:    'Event.DoubleTap'
//
// Describes sensor double tap event with direction as a value.
//-----------------------------------------------------------------------------
//
_LIT( KSensorSourceEventDoubleTap, "Event.DoubleTap" );

// Context values
_LIT_CONTEXT_VALUES( KSensorSourceEventDoubleTapValues ) =
    {
    // User has double tapped succesfully the device from X axel
    _S( "X-Axis" ),

    // User has double tapped succesfully the device from Y axel
    _S( "Y-Axis" ),

    // User has double tapped succesfully the device from Z axel
    _S( "Z-Axis" )
    };

// Enumerations for accessing values array
enum TSensorSourceEventDoubleTapValues
    {
    EDoubleTapX,
    EDoubleTapY,
    EDoubleTapZ,
    EDoubleTapCount
    };

// Capabilities
_LIT_SECURITY_POLICY_PASS( KSensorSourceEventDoubleTapSec );

//-----------------------------------------------------------------------------
// Source:  'Sensor'
// Type:    'Event.Orientation'
//
// Describes sensor orientation event.
//-----------------------------------------------------------------------------
//
_LIT( KSensorSourceEventOrientation, "Event.Orientation" );

//-----------------------------------------------------------------------------
// Source:  'Sensor'
// Type:    'Event.Orientation.Previous'
//
// Describes sensor orientation event.
//-----------------------------------------------------------------------------
//
_LIT( KSensorSourceEventOrientationPrevious, "Event.Orientation.Previous" );
// Context values
_LIT_CONTEXT_VALUES( KSensorSourceEventOrientationValues ) =
    {
    // Undefined, this value is used when the sensors are set off
    _S( "Undefined" ),

    // Display facing up
    _S( "DisplayUp" ),

    // Display facing down
    _S( "DisplayDown" ),

    // Display facing left up
    _S( "DisplayLeftUp" ),

    // Display facing right up
    _S( "DisplayRightUp" ),

    // Display upwards
    _S( "DisplayUpwards" ),

    // Display downwards
    _S( "DisplayDownwards" )
    };

// Enumerations for accessing values array
enum TSensorSourceEventOrientationValues
    {
    EUndefined,
    EDisplayUp,
    EDisplayDown,
    EDisplayLeftUp,
    EDisplayRightUp,
    EDisplayUpwards,
    EDisplayDownwards,
    EDisplayCount
    };

// Capabilities
_LIT_SECURITY_POLICY_PASS( KSensorSourceEventOrientationSec );


//-----------------------------------------------------------------------------
// Source:  'Sensor'
// Type:    'Event.Proximity'
//
// Describes sensor (ALS)Proximity event.
//-----------------------------------------------------------------------------
//
_LIT( KSensorSourceEventProximity, "Event.Proximity" );

// Context values
_LIT_CONTEXT_VALUES( KSensorSourceEventProximityValues ) =
    {
    // Proximity Discernible
    _S( "Discernible" ),

    //  Proximity Indiscernible
    _S( "Indiscernible" )
    };

// Enumerations for accessing values array
enum TSensorSourceEventProximityValues
    {
    EDiscernible,
    EIndiscernible
    };

// Capabilities
_LIT_SECURITY_POLICY_PASS( KDeviceActivitySec );

//-----------------------------------------------------------------------------
// Source:
//      Sensor
//
// Type:
//      Event.AutoRotate
//
// Values:
//      Enabled
//      Disabled
//
// Describes if the orientation channel has been activated by
// the autoroateui feature.
//
// NOTE:
// Temporary solution to make sure that automatic UI rotation does not
// keep the orientation channel open.
//-----------------------------------------------------------------------------
//

// Type
_LIT( KSensorSourceEventAutoRotate, "Event.AutoRotate" );

// Values
_LIT_CONTEXT_VALUES( KSensorSourceEventAutoRotateVal ) =
    {
    // Disabled
    _S( "Disabled" ),

    // Enabled
    _S( "Enabled" )
    };

// Enumerations for accessing values array
enum TSensorSourceEventAutoRotateVal
    {
    EAutoRotateDisabled,
    EAutoRotateEnabled
    };

// Capabilities
_LIT_SECURITY_POLICY_S0( KSensorSourceEventAutoRotateSec, 0x10282BC4 );

//-----------------------------------------------------------------------------
// Source:
//      Sensor
//
// Type:
//      Event.AutoRotateAndDoubleTap
//
// Values:
//      Enabled
//      Disabled
//
// Describes if the orientation and doubletap channels have been activated by
// the \s60\app\telephony\phoneapp\silenceactionplugin\data\silence.rul
//
// NOTE:
// Temporary solution to make sure that automatic UI rotation and doubletap
// does not keep the orientation and doubletap channels open.
//-----------------------------------------------------------------------------
//

// Type
_LIT( KSensorSourceEventAutoRotateAndDoubleTap, "Event.AutoRotateAndDoubleTap" );

// Values
_LIT_CONTEXT_VALUES( KSensorSourceEventAutoRotateAndDoubleTapVal ) =
    {
    // Disabled
    _S( "Disabled" ),

    // Enabled
    _S( "Enabled" )
    };

// Enumerations for accessing values array
enum TSensorSourceEventAutoRotateAndDoubleTapVal
    {
    EAutoRotateAndDoubleTapDisabled,
    EAutoRotateAndDoubleTapEnabled
    };

// Capabilities
_LIT_SECURITY_POLICY_S0( KSensorSourceEventAutoRotateAndDoubleTapSec, 0x10282BC4 );


#endif
