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

#ifndef __ASSRVSOUNDSETTINGS_H__
#define __ASSRVSOUNDSETTINGS_H__

// System includes
#include <e32base.h>
#include <babitflags.h>

// User includes
#include <asshddefs.h>
#include "ASSrvAlarmSoundDetails.h"
#include "ASSrvSoundSettingsObserver.h"

#include "AlarmServer.hrh"

// Classes referenced
class CASSrvServerWideData;
class MASSrvSoundSettingsObserver;


//
// ----> CASSrvSoundSettings (header)
//
/**
 * Responsible for collecting all the various sound related settings
 * within the alarm server.
 */
class CASSrvSoundSettings : public CBase
	{
//
public:										// INTERNAL - SOUND TIMING RELATED
//

	/**
	 * The position used to calculate sound timing values
	 */
	enum TSoundCyclePosition
		{
		/**
		 * Return a time based upon the start of a sound cycle
		 */
		ESoundCyclePositionAtStart			= 0,

		/**
		 * Return a time based upon the end of a sound cycle
		 */
		ESoundCyclePositionAtEnd
		};

//
public:										// STATIC CONSTRUCT / DESTRUCT
//
	static CASSrvSoundSettings*				NewL(CASSrvServerWideData& aServerWideData);
	~CASSrvSoundSettings();

//
private:									// INTERNAL CONSTRUCT
//
	CASSrvSoundSettings(CASSrvServerWideData& aServerWideData);
	void									ConstructL();

//
public:										// ACCESS
//

	/**
	 * Set the sound intervals.
	 */
	TInt									SetSoundIntervals(RArray<TASSrvAlarmSoundDetails>* aIntervalsToTakeOwnershipOf);

	/**
	 * Return the number of sound intervals that are currently set
	 */
	TInt									SoundIntervalCount() const;

	/**
	 * Return the sound interval at the specified index
	 */
	const TASSrvAlarmSoundDetails&			SoundInterval(TInt aIndex) const;

	/**
	 * Return the sound repeat setting
	 */
	inline TAlarmSoundRepeatSetting				RepeatSetting() const { return iRepeatSetting; }

	/**
	 * Return the start or end time for a given sound cycle based upon
	 * the specified baseline start time.
	 *
	 * @param aStartTime The basline for the time calculations
	 * @param aIndex The index of the sound cycle to use
	 * @param aPosition The sound cycle position to use (start/end of cycle).
	 */
	TTime									SoundIntervalTime(const TTime& aStartTime, TInt aIndex, TSoundCyclePosition aPosition) const;

	/**
	 * Return the global sound state
	 */
	inline TAlarmGlobalSoundState			GlobalSoundState() const { return iGlobalSoundState; }

	/**
	 * Set the global sound state
	 */
	void									SetGlobalSoundState(TAlarmGlobalSoundState aState);

	/**
	 * Inform the specified observer when the sound settings (any) change.
	 */
	void									NotifySoundSettingsChangeL(MASSrvSoundSettingsObserver& aObserver);

	/**
	 * Cancel a request for sound settings change notifications
	 */
	void									NotifySoundSettingsChangeCancel(MASSrvSoundSettingsObserver& aObserver);

	/**
	 * Clear sound pause flag
	 */

	void									ClearSoundPauseFlag();

//
public:										// INTERNALIZE / EXTERNALIZE
//

	/**
	 * Internalize the sound settings from the specified stream
	 * - to a temporary buffer
	 */
	void									InternalizeL(RReadStream& aStream);

	/**
	 * Internalize the sound intervals from the specified stream
	 */
	void									InternalizeSoundIntervalsL(RReadStream& aStream);

	/**
	 * Whether to use the Internalize buffer, or throw it away
	 */
	void									ApplyInternalizedData(TBool aUseNewData);

	/**
	 * Externalize the sound settings to the specified stream
	 */
	void									ExternalizeL(RWriteStream& aStream) const;

	/**
	 * Externalize the sound intervals to the specified stream
	 */
	void									ExternalizeSoundIntervalsL(RWriteStream& aStream) const;

//
private:									// INTERNAL
//

	/**
	 * Initialize the sound intervals to their defaults
	 */
	void									GetSoundIntervalsFromResourceFileL();
	void									SetSoundIntervalsToDefaultValuesL();

	/**
	 * Notify all sound settings change observers about an event
	 */
	void									NotifyEvent(MASSrvSoundSettingsObserver::TSoundSettingsEvent aEvent);

	/**
	 * Access the server-wide data
	 */
	inline CASSrvServerWideData&			ServerData() const { return iServerWideData; }

//
private:									// MEMBER DATA
//

	/**
	 * The sound play intervals
	 */
	RArray<TASSrvAlarmSoundDetails>*		iSoundPlayIntervals;

	/**
	 * The global sound state
	 */
	TAlarmGlobalSoundState					iGlobalSoundState;

	/**
	 * Internal flags
	 */
	TBitFlags								iSoundFlags;

	/**
	 * The server-wide data
	 */
	CASSrvServerWideData&					iServerWideData;

	/**
	 * Sound settings change observers
	 */
	RPointerArray<MASSrvSoundSettingsObserver>* iSettingsObservers;

	/**
	 * The sound play intervals - Internalize buffer
	 */
	RArray<TASSrvAlarmSoundDetails>*		iNewSoundPlayIntervals;

	/**
	 * The Sound Repeat Setting
	 */
	TAlarmSoundRepeatSetting				iRepeatSetting;

	};

#endif
