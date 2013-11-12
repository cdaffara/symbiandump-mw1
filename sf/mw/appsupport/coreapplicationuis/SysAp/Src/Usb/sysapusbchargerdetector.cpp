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


#include <e32debug.h>
#include "sysapusbchargerdetector.h"
#include "SysAp.hrh"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// TSysApUsbChargerDetector::TSysApUsbChargerDetector
// ---------------------------------------------------------------------------
//
TSysApUsbChargerDetector::TSysApUsbChargerDetector() :
    iUsbDeviceUsed( EFalse ),
    iChargingUsed( EFalse ),
    iUsbChargingEnabled( EFalse )
    {
    TRACES( RDebug::Print(
        _L( "TSysApUsbChargerDetector::TSysApUsbChargerDetector" ) ) );
    }

// ---------------------------------------------------------------------------
// TSysApUsbChargerDetector::SetUsbDeviceUsed
// ---------------------------------------------------------------------------
//
void TSysApUsbChargerDetector::SetUsbDeviceUsed( TBool aUsbDeviceUsed )
    {
    TRACES( RDebug::Print(
        _L( "TSysApUsbChargerDetector::SetUsbDeviceUsed %d" ),
        aUsbDeviceUsed ) );

    iUsbDeviceUsed = aUsbDeviceUsed;
    }

// ---------------------------------------------------------------------------
// TSysApUsbChargerDetector::SetChargingUsed
// ---------------------------------------------------------------------------
//
void TSysApUsbChargerDetector::SetChargingUsed( TBool aChargingUsed )
    {
    TRACES( RDebug::Print(
        _L( "TSysApUsbChargerDetector::SetChargingUsed %d" ),
        aChargingUsed ) );

    iChargingUsed = aChargingUsed;
    }

// ---------------------------------------------------------------------------
// TSysApUsbChargerDetector::Reset
// ---------------------------------------------------------------------------
//
void TSysApUsbChargerDetector::Reset()
    {
    TRACES( RDebug::Print( _L( "TSysApUsbChargerDetector::Reset" ) ) );

    iUsbDeviceUsed = EFalse;
    iChargingUsed = EFalse;
    }

// ---------------------------------------------------------------------------
// TSysApUsbChargerDetector::HostOnlyUsbChargingUsed
// ---------------------------------------------------------------------------
//
TBool TSysApUsbChargerDetector::HostOnlyUsbChargingUsed() const
    {
    TBool ret( iUsbChargingEnabled && iUsbDeviceUsed && iChargingUsed );

    TRACES( RDebug::Print(
        _L( "TSysApUsbChargerDetector::HostOnlyUsbChargingUsed %d" ),
        ret ) );

    return ret;
    }

// ---------------------------------------------------------------------------
// TSysApUsbChargerDetector::EnableUsbCharging
// ---------------------------------------------------------------------------
//
void TSysApUsbChargerDetector::EnableUsbCharging(
    TBool aUsbChargingEnabled )
    {
    TRACES(
        RDebug::Print( _L( "TSysApUsbChargerDetector::EnableUsbCharging %d" ),
        aUsbChargingEnabled ) );

    iUsbChargingEnabled = aUsbChargingEnabled;
    }
