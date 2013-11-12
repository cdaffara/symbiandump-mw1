/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Internal Publish & Subscribe definitions of Startup app.
*
*/


#ifndef STARTUPAPPINTERNALPSKEYS_H
#define STARTUPAPPINTERNALPSKEYS_H

// INCLUDES
#include <startupdomainpskeys.h>

// =============================================================================
// Touch Screen Calibration API
// =============================================================================

// Use TUid KPSUidStartup = { 0x101F8766 };

/**
* Wait TouchScreen calibration
*/
const TUint32 KPSTouchScreenCalibration = 0x00000051;
enum TPSTouchScreenCalibration
    {
    ETouchScreenCalibrationWait,
    ETouchScreenCalibrationOk    
    };

#endif      // STARTUPAPPINTERNALPSKEYS_H

// End of File

