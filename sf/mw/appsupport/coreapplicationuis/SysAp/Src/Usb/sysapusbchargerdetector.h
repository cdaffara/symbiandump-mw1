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
* Description:  This class offers USB charger detector service.
*
*/


#ifndef T_SYSAPUSBCHARGERDETECTOR_H
#define T_SYSAPUSBCHARGERDETECTOR_H

#include <e32std.h>

// CLASS DECLARATION
/**
*  This class offers USB charger detector service.
*
*  @since 3.2
*/
class TSysApUsbChargerDetector
    {
public:
    /**
     * Constructor.
     *
     * @since S60 3.2
     */
    TSysApUsbChargerDetector();

    /**
     * Sets USB device used status.
     *
     * @since S60 3.2
     * @param aUsbDeviceUsed Value of USB device used status
     */
    void SetUsbDeviceUsed( TBool aUsbDeviceUsed );

    /**
     * Sets charging used status.
     *
     * @since S60 3.2
     * @param aChargingUsed Value of charging used status
     */
    void SetChargingUsed( TBool aChargingUsed );

    /**
     * Resets the USB charger detector.
     *
     * @since S60 3.2
     */
    void Reset();

    /**
     * Checks is host only USB charging used i.e. charging without
     * decicated USB charger.
     *
     * @since S60 3.2
     * @return ETrue if USB charging is used. Otherwise EFalse.
     */
    TBool HostOnlyUsbChargingUsed() const;

    /**
     * Enables USB charging.
     *
     * @since S60 3.2
     * @param aUsbChargingEnabled Value of charging enabled status
     */
    void EnableUsbCharging( TBool aUsbChargingEnabled );

private:
    /**
     * Indicates if USB device is used.
     */
    TBool iUsbDeviceUsed;

    /**
     * Indicates if charging is used.
     */
    TBool iChargingUsed;

    /**
     * Indicates if USB charging is enabled.
     */
    TBool iUsbChargingEnabled;

    };

#endif // T_SYSAPUSBCHARGERDETECTOR_H
