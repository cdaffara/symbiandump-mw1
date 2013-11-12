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

#ifndef __ASSRVALARMSOUNDDETAILS_H__
#define __ASSRVALARMSOUNDDETAILS_H__

// System includes
#include <e32std.h>
#include <s32strm.h>

// User includes
#include <asshddefs.h>

// Type definitions

// Constants

// Enumerations


//
// ----> TASSrvAlarmSoundDetails (header)
//
/**
 * Represents a time/duration pair which is used to control
 * sound interval playing within the Alarm Server.
 */
class TASSrvAlarmSoundDetails
	{
//
public:										// CONSTRUCT
//
	/**
	 * Default constructor.
	 */
	inline TASSrvAlarmSoundDetails();

	/**
	 * Constructor
	 */
	inline TASSrvAlarmSoundDetails(TUint aOffset, TUint aDuration);

//
public:										// ACCESS
//

	/**
	 * Return the offset in minutes when the sound should play
	 */
	inline TUint							OffsetInMinutes() const { return iOffsetInMinutes; }

	/**
	 * Return the duration in seconds during which the sound should
	 * be heard.
	 */
	inline TUint							DurationInSeconds() const { return iDurationInSeconds; }

//
public:										// INTERNALIZE / EXTERNALIZE
//

	void									InternalizeL(RReadStream& aStream);

	void									ExternalizeL(RWriteStream& aStream) const;

//
private:									// MEMBER DATA
//

	/**
	 * The offset in minutes
	 */
	TUint									iOffsetInMinutes;

	/**
	 * The duration in seconds
	 */
	TUint									iDurationInSeconds;
	};



//
// ----> TASSrvAlarmSoundDetails (inlines)
//

inline TASSrvAlarmSoundDetails::TASSrvAlarmSoundDetails()
:	iOffsetInMinutes(KDefaultSoundPlayOffsetInSeconds), iDurationInSeconds(KDefaultSoundPlayDurationInSeconds)
	{ }

inline TASSrvAlarmSoundDetails::TASSrvAlarmSoundDetails(TUint aOffset, TUint aDuration)
:	iOffsetInMinutes(aOffset), iDurationInSeconds(aDuration)
	{ }


#endif
