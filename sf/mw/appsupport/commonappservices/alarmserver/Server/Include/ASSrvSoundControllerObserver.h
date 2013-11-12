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

#ifndef __ASSRVSOUNDCONTROLLEROBSERVER_H__
#define __ASSRVSOUNDCONTROLLEROBSERVER_H__

// System includes
#include <e32std.h>

// User includes
#include <asshddefs.h>

//
// ----> MASSrvSoundControllerObserver (header)
//
/**
 * Handle events from the sound co-ordinator
 */
class MASSrvSoundControllerObserver
	{
//
public:										// FROM MASSrvSoundControllerObserver
//

	/**
	 * The type of event
	 */
	enum TSoundControllerEvent
		{
		/**
		 * The sound controller has requested that an alarm sound be played
		 */
		ESoundControllerEventPlaySound = 0,

		/**
		* The sound controller has request that no sound be played
		*/
		ESoundControllerEventPlayNothing
		};

//
public:										// FROM MASSrvSoundControllerObserver
//

	/**
	 * Handle an event
	 */
	virtual void							MASHandleSoundControllerEvent(TSoundControllerEvent aEvent, TAlarmId aAlarmId) = 0;
	};

#endif
