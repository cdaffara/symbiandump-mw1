// ssyreferencemagnetometer.h

/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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




/**
 @file
 @internalAll Sensor framework Reference SSY header file
 @test
*/


#ifndef SSYREFERENCEMAGNETOMETER_H
#define SSYREFERENCEMAGNETOMETER_H

//  INCLUDES
#include <e32base.h>
#include <sensors/sensrvtypes.h> 


// MAGNETOMETER RELATED CHANNELS 

/**
* - Name:          Magnetometer XYZ-axis data channel type
* - Type:          Rawdata
* - Datatype:      TSensrvMagnetometerAxisData
* - Description:   Magnetometer x-, y-, z-axis data  
*/
const TSensrvChannelTypeId KSensrvChannelTypeIdMagnetometerXYZAxisData = 0x2000BEE0;


// MAGNETOMETER RELATED PROPERTIES

/**
* - Name:        Name of the property
* - Type:        Defines type of the property (TInt/TReal/TBuf)
* - Scope:       Defines a property scope. Property can be defined for a channel, 
*                for a specific item in a channel or for a server related to 
*                channel.
* - Mandatory:   Defines is property mandatory
* - Capability:  Capabilities needed to set this property
* - Description: Description of the property
*/

/**
* - Name:         Auto calibration active
* - Type:         TInt 
* - Scope:        Channel item property
* - Mandatory:    No
* - Capability:   None
* - Description:  Indicates is auto calibration active.
*                 Value is one if calibration is activated, zero otherwise.
*/
const TSensrvPropertyId KSensrvPropAutoCalibrationActive = 0x00001006;

/**
* - Name:         Calibration status
* - Type:         TInt 
* - Scope:        Channel item property
* - Mandatory:    No
* - Capability:   None
* - Description:  Indicates the calibration level.
*                 Calibration level scales between minimum and maximum value.
*                 Maximum indicates that calibration level is at its best
*                 level. Minimum indicates that calibration is undefined.
*/
const TSensrvPropertyId KSensrvPropCalibrationLevel = 0x00001007;

// MAGNETOMETER RELATED DATATYPES

/**
* Magnetometer axis data type
*/
class TSensrvMagnetometerAxisData 
    {
public:
    /**
    * Channel data type Id number
    */      
    static const TSensrvChannelDataTypeId KDataTypeId = 0x2000BEE0;

    /**
    * Channel data type index numbers
    */  
    enum TSensrvMagnetometerAxisDataIndexes
        {
        ETimeStamp = 0,
        EAxisX,
        EAxisY,
        EAxisZ
        };

public:

    /**
    * - Item name:   Sampling time.
    * - Item Index:  0
    * - Conditions:  None
    * - Description: Timestamp for a sample.
    */   
    TTime iTimeStamp;    

    /**
    * - Item name:   Magnetometer x-axis 
    * - Item Index:  1
    * - Conditions:  Single limit and range 
    * - Description: Magnetometer values from x-axis 
    */
    TInt iAxisXRaw;

    /**
    * - Item name:   Magnetometer y-axis 
    * - Item Index:  2
    * - Conditions:  Single limit and range 
    * - Description: Magnetometer values from y-axis 
    */ 
    TInt iAxisYRaw;

    /**
    * - Item name:   Magnetometer z-axis 
    * - Item Index:  3
    * - Conditions:  Single limit and range 
    * - Description: Magnetometer values from z-axis 
    */
    TInt iAxisZRaw;
    
    /**
    * - Item name:   Magnetometer x-axis 
    * - Item Index:  1
    * - Conditions:  Single limit and range 
    * - Description: Magnetometer values from x-axis 
    */
    TInt iAxisXCalibrated;

    /**
    * - Item name:   Magnetometer y-axis 
    * - Item Index:  2
    * - Conditions:  Single limit and range 
    * - Description: Magnetometer values from y-axis 
    */ 
    TInt iAxisYCalibrated;

    /**
    * - Item name:   Magnetometer z-axis 
    * - Item Index:  3
    * - Conditions:  Single limit and range 
    * - Description: Magnetometer values from z-axis 
    */
    TInt iAxisZCalibrated;
    };

#endif //SSYREFERENCEMAGNETOMETER_H

