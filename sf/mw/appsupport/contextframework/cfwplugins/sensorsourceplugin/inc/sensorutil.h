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
* Description:  SensorUtil class declaration.
 *
*/


#ifndef SENSORUTIL_H
#define SENSORUTIL_H

// SYSTEM INCLUDE FILES
#include <e32std.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS

// DATA TYPES

// CLASS DECLARATION

/**
 * Sensor Source Plug-in utility class.
 *
 * @lib None.
 * @since S60 5.0
 */
class SensorUtil
    {
public:

    /**
     * Converts descriptor in to hex.
     * @param aString Descriptor to convert.
     * @param aValue Converted unsigned integer.
     * @return KErrNone if no errors.
     */
    static TInt ConvertToHex( const TDesC& aString, TUint32& aValue );

private:

    /**
     * C++ constructor.
     */
    SensorUtil( );
    };

#endif // SENSORUTIL_H

