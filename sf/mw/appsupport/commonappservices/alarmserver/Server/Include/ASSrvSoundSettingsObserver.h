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

#ifndef __ASSRVSOUNDSETTINGSOBSERVER_H__
#define __ASSRVSOUNDSETTINGSOBSERVER_H__


//
// ----> MASSrvSoundSettingsObserver (header)
//
/**
 * Observe when the sound settings change
 */
class MASSrvSoundSettingsObserver
	{
//
public:										// FROM MASSrvSoundSettingsObserver
//

	/**
	 * The event enumeration for anything that occurs within the
	 * sound settings object
	 */
	enum TSoundSettingsEvent
		{
		/**
		 * Global sound state has changed
		 */
		ESoundSettingsEventGlobalSoundState	= 0,

		/**
		 * Sound intervals have changed
		 */
		ESoundSettingsEventSoundIntervals,

		/**
		 * Need to clear pause flag
		 */
		ESoundSettingsClearPauseFlag
		};

//
public:										// FROM MASSrvSoundSettingsObserver
//

	/**
	 * Handle sound setting event changes
	 */
	virtual void							MASSoundSettingsHandleChangeEvent(TSoundSettingsEvent aEvent) = 0;

	};

#endif
