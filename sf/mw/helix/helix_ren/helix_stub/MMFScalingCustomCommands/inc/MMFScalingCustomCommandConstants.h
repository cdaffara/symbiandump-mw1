/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __MMFSCALINGCUSTOMCOMMANDCONSTANTS_H
#define __MMFSCALINGCUSTOMCOMMANDCONSTANTS_H
#include <e32std.h>
/**
 * @publishedAll
 *
 * Interface UID and Scaling messages for the Controller API.
 */
const TUid KUidInterfaceMMFControllerScalingMsg = {0x10283355};
/**
 *  Function constants
 */
typedef enum
{
	EMMFGetScalingType=0,
	EMMFSetScalingType
} TMMFScalingControllerMessage;

typedef enum
{
	EMMFDefaultScaling=0,
    EMMFNoScaling,
    EMMFNatural,
    EMMFZoom,
    EMMFStretch
} TMMFScalingType;

#endif __MMFSCALINGCUSTOMCOMMANDCONSTANTS_H
