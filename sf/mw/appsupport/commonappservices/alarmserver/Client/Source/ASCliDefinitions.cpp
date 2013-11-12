// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#include <asclidefinitions.h>

// System includes

// User includes
#include "ASShdPragma.h"
#include "ASShdOpCodes.h"

// Type definitions

// Constants

// Enumerations


//
// ----> ASCliDefinitions (source)
//

EXPORT_C const TDesC& ASCliDefinitions::ServerAndThreadName()
/** Returns the alarm server's unique name.

@return Unique name of the alarm server. */
	{
	return KAlarmServerName;
	}


/*
 * Return the name of the server image (Exe/Dll)
 */
const TDesC& ASCliDefinitions::ServerImageName()
	{
	return KAlarmServerImageName;
	}


/*
 * Return the Uid associated with the server image
 */
TUidType ASCliDefinitions::ServerUidType()
	{
	return TUidType(KNullUid, KNullUid, KAlarmServerUid);
	}


/*
 * Return the version number of the server/client
 */
TVersion ASCliDefinitions::Version()
	{
	return TVersion(KAlarmServerMajorVN, KAlarmServerMinorVN, KAlarmServerBuildVN);
	}


