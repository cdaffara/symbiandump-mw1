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

#include "ASSrvAlarmSoundDetails.h"

// System includes

// User includes

// Type definitions

// Constants

// Enumerations

// Classes referenced


//
// ----> TASSrvAlarmSoundDetails (source)
//

//*************************************************************************************
/**
 * Internalize this object's representation from a stream
 */
void TASSrvAlarmSoundDetails::InternalizeL(RReadStream& aStream)
	{
	iOffsetInMinutes = aStream.ReadUint32L();
	iDurationInSeconds = aStream.ReadUint32L();
	}


//*************************************************************************************
/**
 * Externalize this object's representation to a stream
 */
void TASSrvAlarmSoundDetails::ExternalizeL(RWriteStream& aStream) const
	{
	aStream.WriteUint32L(iOffsetInMinutes);
	aStream.WriteUint32L(iDurationInSeconds);
	}
