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



#ifndef __MMFMETADATAEVENTCONFIGCONSTANTS_H
#define __MMFMETADATAEVENTCONFIGCONSTANTS_H
#include <e32std.h>
/**
 * @publishedAll
 *
 * Interface UID of metadata event message for the Controller API.
 */
const TUid KUidInterfaceMMFControllerMetadataEventMsg = {0x2001113A};

/**
 * Event UID to handle metadata event.
 */
const TUid KMMFRefreshMetaData = {0x20011139};

/**
 *  Function constants
 */
typedef enum TMMFMetadataEventConfigControllerMessage
{
	EMMFGetMetadataEventConfig=0,
	EMMFSetMetadataEventConfig
};

#define EMMFDisableMetadataEvent EFalse
#define EMMFEnableMetadataEvent ETrue

#endif __MMFMETADATAEVENTCONFIGCONSTANTS_H
