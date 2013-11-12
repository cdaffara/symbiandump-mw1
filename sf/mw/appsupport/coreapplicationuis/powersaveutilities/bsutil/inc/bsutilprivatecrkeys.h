/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Battery state util central repository configuration keys.
*
*/


#ifndef BSUTILPRIVATECRKEYS_H
#define BSUTILPRIVATECRKEYS_H

// INCLUDES

// =============================================================================
// BSUtil Configuration API
// =============================================================================
const TUid KCRUidBSUtilConf = { 0x2000B5E3 };

/**
* Following keys configure percentual values for each possible battery level.
* Supported battery levels are defined in power state API. If the configured 
* value is not in supported range, it is converted to a valid value.
*
* Possible values: 0 - 100 percentual battery level. Values must be in ascending order
*
* @see KHWRMBatteryLevel
*/
const TUint32 KBSUtilBatteryLevel0 = 0x00000001;
const TUint32 KBSUtilBatteryLevel1 = 0x00000002;
const TUint32 KBSUtilBatteryLevel2 = 0x00000003;
const TUint32 KBSUtilBatteryLevel3 = 0x00000004;
const TUint32 KBSUtilBatteryLevel4 = 0x00000005;
const TUint32 KBSUtilBatteryLevel5 = 0x00000006;
const TUint32 KBSUtilBatteryLevel6 = 0x00000007;
const TUint32 KBSUtilBatteryLevel7 = 0x00000008;

#endif      // BSUTILPRIVATECRKEYS_H

// End of File

