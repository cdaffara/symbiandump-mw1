// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Contains declaration of CTestCreateEntriesOOMStep class, which is used for testing OOM
// 
//

#ifndef __TEST_CREATE_ENTRIES_OOM_STEP_H__
#define __TEST_CREATE_ENTRIES_OOM_STEP_H__

// User Includes
#include "TestCreateCalEntriesStep.h" 

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestCreateEntriesOOMStep, "TestCreateEntriesOOMStep");
/*@}*/

/**
Test step to test OOM
@internalTechnology
@test
*/
class CTestCreateEntriesOOMStep : public CTestCreateCalEntriesStep
	{
public:
	CTestCreateEntriesOOMStep(CTestMultipleAlarmsServer& aTestServer);

	// TEF pure virtual
	TVerdict 				doTestStepL();
	
private:
	void 					TestOOML();
	};

#endif		// __TEST_CREATE_ENTRIES_OOM_STEP_H__
