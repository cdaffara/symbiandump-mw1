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

#include "ASSrvSoundSettings.h"


// User includes
#include "ASSrvStaticUtils.h"
#include "ASSrvServerWideData.h"
#include "ASSrvAnyEventManager.h"

#include <alarmserver.rsg>
#include "AlarmServer.hrh"

// System includes
#include <barsc.h> 
#include <barsread.h>

// Type definitions

// Constants
const TInt KAlarmSoundOffsetsGranularity = 14;
#define KAlarmCycleStartOffsetMinutes {0, 1, 2, 3, 5, 10, 20, 30, 45, 60, 90, 120, 180, 240, KErrNotFound };

// Enumerations

// Classes referenced


//
// ----> CASSrvSoundSettings (source)
//

//*************************************************************************************
CASSrvSoundSettings::CASSrvSoundSettings(CASSrvServerWideData& aServerWideData)
:	iServerWideData(aServerWideData)
	,iRepeatSetting(EAlarmSoundRepeatSettingLoop)
	{
	}


//*************************************************************************************
CASSrvSoundSettings::~CASSrvSoundSettings()
	{
	if	(iSoundPlayIntervals)
		iSoundPlayIntervals->Close();
	delete iSoundPlayIntervals;
	if	(iNewSoundPlayIntervals)
		{
		iNewSoundPlayIntervals->Close();
		delete iNewSoundPlayIntervals;
		}
	if	(iSettingsObservers)
		iSettingsObservers->Close();
	delete iSettingsObservers;
	}


//*************************************************************************************
void CASSrvSoundSettings::ConstructL()
	{
	iSettingsObservers = new(ELeave) RPointerArray<MASSrvSoundSettingsObserver>(2);
	iSoundPlayIntervals = new(ELeave) RArray<TASSrvAlarmSoundDetails>(KAlarmSoundOffsetsGranularity);
	//
	SetSoundIntervalsToDefaultValuesL();
	}


//*************************************************************************************
CASSrvSoundSettings* CASSrvSoundSettings::NewL(CASSrvServerWideData& aServerWideData)
	{
	CASSrvSoundSettings* self = new(ELeave) CASSrvSoundSettings(aServerWideData);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}


//
//
//

/**
This function changes the sound interval sequence. The default sequence is specified by
KAlarmCycleStartOffsetMinutes. alarmserver.rsc takes precedence over the default. 
alarmserver.ini takes precedence over alarmserver.rsc. If this function is invoked, the
new values are persisted in alarmserver.ini.

A zero-length interval list means that CASSrvSoundSettings should be disabled. This function
may not be used to set a zero-length interval array; only alarmserver.rsc may do that.
If CASSrvSoundSettings is disabled due to that alarmserver.rsc configuration, or if a NULL 
pointer or zero-length array is passed to this function, the function will
make no changes and return immediately.

@param aIntervalsToTakeOwnershipOf An RArray of TASSrvAlarmSoundDetails
@return KErrNotSupport If CASSrvSoundSettings is disabled.
*/
TInt CASSrvSoundSettings::SetSoundIntervals(RArray<TASSrvAlarmSoundDetails>* aIntervalsToTakeOwnershipOf)
	{
	// Check to prevent change between enabled & disabled
	if (iSoundPlayIntervals->Count() == 0)
		{
		// Don't cleanup aIntervalsToTakeOwnershipOf,
		// cause it's still on the Session's cleanup stack.
		return KErrNotSupported;
		}
	else if ((EAlarmSoundRepeatSettingRepeatLast == iRepeatSetting) &&
			 (iSoundPlayIntervals->Count() < 2 ))
		{
		return KErrArgument;
		}
	
	iSoundPlayIntervals->Close();
	delete iSoundPlayIntervals;
	iSoundPlayIntervals = aIntervalsToTakeOwnershipOf;

	// Notify observers
	NotifyEvent(MASSrvSoundSettingsObserver::ESoundSettingsEventSoundIntervals);

	// Notify change
	ServerData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventPlayIntervalsChanged, KNullAlarmId);
	return KErrNone;
	}

//*************************************************************************************
TInt CASSrvSoundSettings::SoundIntervalCount() const
	{
	return iSoundPlayIntervals->Count();
	}


//*************************************************************************************
const TASSrvAlarmSoundDetails& CASSrvSoundSettings::SoundInterval(TInt aIndex) const
	{
	__ASSERT_ALWAYS(aIndex >=0 && aIndex < SoundIntervalCount(), ASSrvStaticUtils::Fault(ASSrvStaticUtils::EASSrvFaultBadSoundIntervalIndex));
	return (*iSoundPlayIntervals)[aIndex];
	}


//*************************************************************************************
TTime CASSrvSoundSettings::SoundIntervalTime(const TTime& aStartTime, TInt aIndex, TSoundCyclePosition aPosition) const
	{
	TTime finishTime(aStartTime);
	//
	const TASSrvAlarmSoundDetails& interval = SoundInterval(aIndex);

	// Add on the offset from the start of the cycle. This gets us the starting time
	// for a sound playback interval.
	finishTime += TTimeIntervalMinutes(interval.OffsetInMinutes());
	if	(aPosition == ESoundCyclePositionAtEnd)
		{
		// If the end time is required, we need to add on the duration
		finishTime += TTimeIntervalSeconds(interval.DurationInSeconds());
		}

	return finishTime;
	}


//*************************************************************************************
void CASSrvSoundSettings::SetGlobalSoundState(TAlarmGlobalSoundState aState)
	{
	if	(aState == iGlobalSoundState)
		return;

	iGlobalSoundState = aState;

	// Notify observers
	NotifyEvent(MASSrvSoundSettingsObserver::ESoundSettingsEventGlobalSoundState);

	// Notify change
	ServerData().AnyEventManager().MASAnyEventManagerObserverNotifyChanges(EAlarmChangeEventGlobalSoundStateChanged, KNullAlarmId);
	}


//*************************************************************************************
void CASSrvSoundSettings::NotifySoundSettingsChangeL(MASSrvSoundSettingsObserver& aObserver)
	{
	User::LeaveIfError(iSettingsObservers->InsertInAddressOrder(&aObserver));
	}


//*************************************************************************************
void CASSrvSoundSettings::NotifySoundSettingsChangeCancel(MASSrvSoundSettingsObserver& aObserver)
	{
	TInt index = KErrNotFound;
	const TInt error = iSettingsObservers->FindInAddressOrder(&aObserver, index);
	if	(error != KErrNotFound)
		iSettingsObservers->Remove(index);
	}


//
//
//


//*************************************************************************************
void CASSrvSoundSettings::InternalizeL(RReadStream& aStream)
	{
	InternalizeSoundIntervalsL(aStream);
	}


//*************************************************************************************
void CASSrvSoundSettings::InternalizeSoundIntervalsL(RReadStream& aStream)
	{
	RArray<TASSrvAlarmSoundDetails>* newIntervals = ASSrvStaticUtils::InternalizeSoundPlayIntervalsLC(aStream);
	iNewSoundPlayIntervals = newIntervals;
	CleanupStack::Pop(); // newIntervals
	}


//*************************************************************************************
void CASSrvSoundSettings::ApplyInternalizedData(TBool aUseNewData)
	{
	if (aUseNewData)
		{
		SetSoundIntervals(iNewSoundPlayIntervals);
		}
	else if (iNewSoundPlayIntervals)
		{
		// throw away any buffered data
		iNewSoundPlayIntervals->Close();
		delete iNewSoundPlayIntervals;
		}
	// the buffer is no longer valid
	iNewSoundPlayIntervals = NULL;
	}


//*************************************************************************************
void CASSrvSoundSettings::ExternalizeL(RWriteStream& aStream) const
	{
	ExternalizeSoundIntervalsL(aStream);
	}


//*************************************************************************************
void CASSrvSoundSettings::ExternalizeSoundIntervalsL(RWriteStream& aStream) const
	{
	const TInt count = iSoundPlayIntervals->Count();
	aStream.WriteInt32L(count);
	//
	for(TInt i=0; i<count; i++)
		{
		const TASSrvAlarmSoundDetails interval = (*iSoundPlayIntervals)[i];
		aStream << interval;
		}
	}


//
//
//


//*************************************************************************************
void CASSrvSoundSettings::SetSoundIntervalsToDefaultValuesL()
	{
	// We first try to get the Sound Intervals from the resource file.
	// If, for some reason, it fails, we fall back to use the default,
	// hard-coded values as before.
	TRAPD(err, GetSoundIntervalsFromResourceFileL() );
	if( err != KErrNone )
		{
		// use hardcoded defaults
		const TInt cycles[] = KAlarmCycleStartOffsetMinutes;
		iSoundPlayIntervals->Reset();
		
		TInt i = 0;
		FOREVER
			{
			const TInt offset = cycles[i++];
			if	(offset == KErrNotFound)
				break;

			TASSrvAlarmSoundDetails soundInterval(offset, KDefaultSoundPlayDurationInSeconds);
			User::LeaveIfError(iSoundPlayIntervals->Append(soundInterval));
			}
		}
	}
//*************************************************************************************
void CASSrvSoundSettings::GetSoundIntervalsFromResourceFileL()
	{
	iSoundPlayIntervals->Reset();
	// Reading the intervals and durations from the resource file is 
	// usually a waste of time, since the .ini backup file normally
	// exists after the first run. However, if other information is 
	// stored in the resource file as well we have to open it anyway.
	// To check for the .ini file, use ASSrvStaticUtils::GetServerPathL.
	// See also CASSrvAlarmStore::OpenStoreL.

	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);

	// Read resource file if it exists
	RResourceFile resource;

	#ifdef __WINS__ 
		_LIT(KAlarmSvrResourceFile, "c:\\Private\\101F5027\\AlarmServer.rsc");
	#else
		_LIT(KAlarmSvrResourceFile, "z:\\Private\\101F5027\\AlarmServer.rsc");
	#endif // __WINS__

	resource.OpenL(fs, KAlarmSvrResourceFile);
	CleanupClosePushL(resource);
	resource.ConfirmSignatureL(0);


	HBufC8* intervalBuf = resource.AllocReadL(SOUND_CONTROLLER);
	CleanupStack::PushL(intervalBuf);
		
	TResourceReader reader;
	reader.SetBuffer(intervalBuf);

	// Read the Sound Settings
	TUint16 repeatSetting = reader.ReadInt16();
	if ((repeatSetting != EAlarmSoundRepeatSettingLoop) &&
		(repeatSetting != EAlarmSoundRepeatSettingRepeatLast) &&
		(repeatSetting != EAlarmSoundRepeatSettingStop) )
		{
		User::Leave(KErrArgument);
		}
	iRepeatSetting = static_cast<TAlarmSoundRepeatSetting>(repeatSetting);

	// Read the Sound Intervals
	TInt16 numEntries = reader.ReadInt16();
	for(TInt i=0; i<numEntries; i++)
		{
		TInt16 offset = reader.ReadInt16();
		TInt16 duration = reader.ReadInt16();

		TASSrvAlarmSoundDetails soundInterval(offset, duration);
		User::LeaveIfError(iSoundPlayIntervals->Append(soundInterval));
		}

	CleanupStack::PopAndDestroy(intervalBuf);
	CleanupStack::PopAndDestroy(&resource);
	CleanupStack::PopAndDestroy(&fs);

	// Check the sound play intervals are valid
	if (iSoundPlayIntervals->Count() > 0)
		{
		const TInt error = ASSrvStaticUtils::ValidateSoundPlayIntervals(*iSoundPlayIntervals);
		User::LeaveIfError(error);
		}
	else if ((EAlarmSoundRepeatSettingRepeatLast == iRepeatSetting) &&
			 (iSoundPlayIntervals->Count() < 2 ))
		{
		// We need at least 2 intervals to determine the length of the last
		// interval, especially since the first interval's Offset = 0.
		User::Leave(KErrArgument);
		}
	}


//*************************************************************************************
void CASSrvSoundSettings::NotifyEvent(MASSrvSoundSettingsObserver::TSoundSettingsEvent aEvent)
	{
	// Cascade changes to observers
	const TInt count = iSettingsObservers->Count();
	for(TInt i=0; i<count; i++)
		{
		(*iSettingsObservers)[i]->MASSoundSettingsHandleChangeEvent(aEvent);
		}
	}

//*************************************************************************************
void CASSrvSoundSettings::ClearSoundPauseFlag()
	{
	NotifyEvent(MASSrvSoundSettingsObserver::ESoundSettingsClearPauseFlag);
	}

