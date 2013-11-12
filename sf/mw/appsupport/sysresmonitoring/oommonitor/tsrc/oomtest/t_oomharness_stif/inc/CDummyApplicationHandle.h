/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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




#ifndef CDUMMYAPPLICATIONHANDLE_H
#define CDUMMYAPPLICATIONHANDLE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32msgqueue.h> 

// a few uids...use appuidlister to extract from the app_reg.rsc files
// and mappings to uids not used by tests anymore
const TInt KOomTestAppUid  = 0xE6CFBA01; // 0x101f8599
const TInt KOomTestApp2Uid = 0xE6CFBA02; // 0x10005901
const TInt KOomTestApp3Uid = 0xE6CFBA03; // 0x10005a22
const TInt KOomTestApp4Uid = 0xE6CFBA04; // 0x101f4cd5
const TInt KOomTestApp5Uid = 0xE6CFBA05; // 0x10005234
const TInt KOomTestApp6Uid = 0xE6CFBA06; // 0x10207218
const TInt KOomTestApp7Uid = 0xE6CFBA07; // 0x10008d39
const TInt KOomTestApp8Uid = 0xE6CFBA08; // 0x10005903
const TInt KOomTestApp9Uid = 0xE6CFBA09; // 0x101f4cce
const TInt KOomTestApp10Uid = 0xE6CFBA0A; // 0x101f4cd2

// CLASS DECLARATION

/**
 *  CCDummyApplicationHandle
 * A helper class for launching dummy apps and checking their status
 */
NONSHARABLE_CLASS ( CCDummyApplicationHandle ) : public CBase
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CCDummyApplicationHandle();

	/**
	 * Two-phased constructor.
	 */
	static CCDummyApplicationHandle* NewL(TUid aUid, TInt aExtraMemoryAllocation = 0);

	/**
	 * Two-phased constructor.
	 */
	static CCDummyApplicationHandle* NewLC(TUid aUid, TInt aExtraMemoryAllocation = 0);

	inline RProcess& Process() { return iProcess; }
	inline const TUid& Uid() { return iUid; }
	
	void SendMessage(TInt aMessage);
	
	void BringToForeground();
	
	static TBool CompareTo(const TUid* aKey, const CCDummyApplicationHandle& aValue);
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CCDummyApplicationHandle(TUid aUid);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL(TInt aExtraMemoryAllocation = 0);

	RProcess iProcess;
	//a channel for sending control messages to the dummy app...
	RMsgQueue<TInt> iMsgQueue;
	TUid iUid;
	};

#endif // CDUMMYAPPLICATIONHANDLE_H
