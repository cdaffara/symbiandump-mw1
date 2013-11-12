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

#ifndef __ASSRVENVIRONMENTCHANGEMANAGER_H__
#define __ASSRVENVIRONMENTCHANGEMANAGER_H__

// System includes
#include <bacntf.h>

// Classes referenced
class CASSrvServerWideData;
class MASSrvEnvironmentChangeObserver;

//
// ----> CASSrvEnvironmentChangeManager (header)
//
/**
 * Implements environment (locale) change handling
 */
class CASSrvEnvironmentChangeManager : public CBase
	{
//
public:										// STATIC CONSTRUCT / DESTRUCT
//
	static CASSrvEnvironmentChangeManager*	NewL(CASSrvServerWideData& aServerWideData);
	~CASSrvEnvironmentChangeManager();

//
private:									// INTERNAL CONSTRUCT
//
	CASSrvEnvironmentChangeManager(CASSrvServerWideData& aServerWideData);
	void									ConstructL();

//
public:										// ACCESS
//

	void									RequestEnvironmentChangesL(MASSrvEnvironmentChangeObserver& aObserver);

	void									RequestEnvironmentChangesCancel(MASSrvEnvironmentChangeObserver& aObserver);

	/**
	 * Return the workdays for the current locale
	 */
	inline TUint							CachedWorkDays() const { return iCachedWorkDays; }

#ifdef _DEBUG
	void									SetEnvironmentChangesHandling(TBool aValue) {iEnvChgHandling = aValue;}
	TBool									EnvironmentChangeHandling() {return iEnvChgHandling;}
#endif
	

//
public:										// INTERNAL
//

	static TBool							EnvironmentChangeCallBack(TAny* aSelf);

//
private:									// INTERNAL
//

	/**
	 * Access the server wide data
	 */
	inline CASSrvServerWideData&			ServerData() const { return iServerWideData; }

	/**
	 * Return the cached workdays
	 */
	inline TUint&							CachedWorkDays() { return iCachedWorkDays; }

	void									HandleEnvironmentChange();

//
private:									// MEMBER DATA
//

	/**
	 * The change notifier always reports that everything has changed when it is
	 * first initialized. We deliberately ignore this phantom event because it
	 * causes problems with alarms which expire immedaitely when the device starts
	 * up (they are deleted immediately, when in reality we want them to expire
	 * immedately).
	 */
	TBool									iHaveAbsorbedFirstChange;

	/**
	 * The server wide data 
	 */
	CASSrvServerWideData&					iServerWideData;

	/**
	 * The underlying environment change notifier
	 */
	CEnvironmentChangeNotifier*				iChangeNotifier;

	/**
	 * A cached copy of the System-Wide work days
	 */
	TUint									iCachedWorkDays;

	/**
	 * Observers who are interested in being informed when
	 * an environment change event occurs.
	 */
	RPointerArray<MASSrvEnvironmentChangeObserver> iObservers;
	
#ifdef _DEBUG
	TBool iEnvChgHandling;
#endif
	};

#endif
