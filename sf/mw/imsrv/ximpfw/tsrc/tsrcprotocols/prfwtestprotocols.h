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
* Description:  XIMP Framework Test Code 
*
*/

#ifndef PRFWTESTPROTOCOLS_H
#define PRFWTESTPROTOCOLS_H

#include <e32std.h>


/**
 * C++ compatible definitions for XIMP test protocols.
 *
 * This must be kept in-sync with definitions in
 * "prfwtestprotocols_resource.hrh"
 */

//XIMP primary test protocol
const TInt K_PRFW_TST_PROTOCOL_1_IMPLEMENTATION_UID  = 0x10282DDF;
const TInt K_PRFW_TST_PROTOCOL_1_VERSION_NO          = 1;
_LIT( K_PRFW_TST_PROTOCOL_1_DISPLAY_NAME,            "XIMP primary test protocol" );
_LIT( K_PRFW_TST_PROTOCOL_1_DEFAULT_DATA,            "" );
_LIT( K_PRFW_TST_PROTOCOL_1_OPAQUE_DATA,             "<p>Nokia<f>feat/pres/pub<f>feat/pres/fetch<f>feat/pres/subs<s>sip" );



//XIMP secondary test protocols
const TInt K_PRFW_TST_PROTOCOL_2_IMPLEMENTATION_UID  = 0x10282DE0;
const TInt K_PRFW_TST_PROTOCOL_3_IMPLEMENTATION_UID  = 0x10282DE1;
const TInt K_PRFW_TST_PROTOCOL_4_IMPLEMENTATION_UID  = 0x10282DE2;
const TInt K_PRFW_TST_PROTOCOL_5_IMPLEMENTATION_UID  = 0x10282DE3;



#endif // PRFWTESTPROTOCOLS_H

