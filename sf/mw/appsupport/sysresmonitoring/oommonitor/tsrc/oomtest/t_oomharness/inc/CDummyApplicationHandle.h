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
const TUid KUidAbout = {0x10005a22};
const TUid KUidBLID = {0x101f85a0};
const TUid KUidBrowserNG = {0x10008d39};
const TUid KUidCalendar = {0x10005901};
const TUid KUidClock = {0x10005903};
const TUid KUidGallery = {0x101f8599};
const TUid KUidPhonebook2 = {0x101f4cce};
const TUid KUidProfileApp = {0x100058f8};

// CLASS DECLARATION

/**
 *  CCDummyApplicationHandle
 * A helper class for launching dummy apps and checking their status
 */
class CCDummyApplicationHandle : public CBase
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
