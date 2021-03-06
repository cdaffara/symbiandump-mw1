// Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
 @test
 @internalComponent - Internal Symbian test code  
*/

#ifndef __T_APPLISTFILEUPDATESTEP_H
#define __T_APPLISTFILEUPDATESTEP_H

#include "apparctestserver.h"
#include "appfwk_test_utils.h"


_LIT(KT_AppListFileUpdate,"T_AppListFileUpdate");

class CT_AppListFileUpdateStep : public CTestStep
	{
public:
	CT_AppListFileUpdateStep();
	~CT_AppListFileUpdateStep();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepPostambleL();

private:
	void TestTimeStampL();
	void AppsListModifiedTimeL(TTime &);
	
private:
	void TestDeleteAppListFileL();
	void DeleteAppListFileL(const TDesC& aFileName);
	TBool CheckForFilePresent();

public:
	RFs	iFs;
 	RApaLsSession iSession;
	RSmlTestUtils iUtils;
	};
	

#endif

