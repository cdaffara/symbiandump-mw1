// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CEBOOKMARKRECOGTEST_H__
#define __CEBOOKMARKRECOGTEST_H__

#include <e32std.h>
#include <f32file.h>

#include "cecomrecogtest.h"

class RApaLsSession;
class RFileLogger;

class CEBookmarkRecogTest : public CEComRecogTest
	{
public:
	static CEBookmarkRecogTest* NewL(TAny* aConstructionsParams);
	virtual ~CEBookmarkRecogTest();
	
private:
	CEBookmarkRecogTest(RApaLsSession& aAppArc, RFileLogger& aLogger);
	void ConstructL();
	
	// From CEComRecogTest
	const TDesC& TestPluginTitle() const;
	TInt TestCaseCount() const;
	const TDesC& TestCaseTitle(TInt aIndex);
	void RunTestCaseL(TInt aIndex);
	
	void RunTestL(const TDesC& aTestData);
	void RunTestCase1L();
	void RunTestCase2L();
	void RunTestCase3L();
	void RunTestCase4L();
	void LogConfidence(TInt aConfidence);
	
private:
	TPtrC iTestCaseTitle;
	RApaLsSession& iAppArc;
	RFileLogger& iLogger;
	RFs iFs;
	};
	
#endif // __CEBOOKMARKRECOGTEST_H__
