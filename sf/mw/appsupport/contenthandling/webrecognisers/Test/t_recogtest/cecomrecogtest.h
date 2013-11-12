// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CECOMRECOGTEST_H__
#define __CECOMRECOGTEST_H__

#include <ecom/ecom.h>

// CEComRecogTest ECOM Interface UID = 101FD8D0
const TUid KUidAppProtRecognisers = {0x101FD8D0};

class RApaLsSession;
class RFileLogger;

class TRecogTestConstructionParams
	{
public:
	inline TRecogTestConstructionParams(RApaLsSession* aAppArcSession, RFileLogger* aLogger);
public:
	RApaLsSession* iAppArcSession;
	RFileLogger* iLogger;
	};

class CEComRecogTest : public CBase
	{
public:
	inline static CEComRecogTest* NewL(TUid aTestImplementation, TRecogTestConstructionParams& aParams);
	inline virtual ~CEComRecogTest();
	
	// Returns a descriptor containing the title of the test plugin
	virtual const TDesC& TestPluginTitle() const = 0;
	
	// Returns the number of test cases contained in this plugin
	virtual TInt TestCaseCount() const = 0;
	
	// Returns a descriptor containing the test case title specified by aIndex
	virtual const TDesC& TestCaseTitle(TInt aIndex) = 0;
	
	// Execute a single test case by index (from 0). Test cases should leave
	// with any errors or failures
	virtual void RunTestCaseL(TInt aIndex) = 0;

protected:
	inline CEComRecogTest();

private:
	TUid iDtor_ID_Key;
	};
	
inline
TRecogTestConstructionParams::TRecogTestConstructionParams(RApaLsSession* aAppArcSession, RFileLogger* aLogger)
: iAppArcSession(aAppArcSession), iLogger(aLogger)
	{
	}

inline
CEComRecogTest* CEComRecogTest::NewL(TUid aTestImplementation, TRecogTestConstructionParams& aParams)
	{
	// Get the instantiation.
	TAny* ptr = REComSession::CreateImplementationL(aTestImplementation, _FOFF(CEComRecogTest, iDtor_ID_Key), &aParams);

	return (reinterpret_cast<CEComRecogTest*>(ptr));
	}
	
inline
CEComRecogTest::~CEComRecogTest()
	{
	REComSession::DestroyedImplementation(iDtor_ID_Key);
	}
	
inline
CEComRecogTest::CEComRecogTest()
: CBase()
	{
	}
	
#endif // __CECOMRECOGTEST_H__
