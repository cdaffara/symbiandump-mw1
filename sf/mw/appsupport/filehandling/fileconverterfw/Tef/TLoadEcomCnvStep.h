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
// The following test case is used to test the ecom style converter plugin and test the list and converter APIs.
// 
//

/**
 @file
 @internalComponent - Internal Symbian test code
*/

#if (!defined __TLOADECOMCNV_STEP_H__)
#define __TLOADECOMCNV_STEP_H__
#include "TConArcTestServer.h"
#include <apmstd.h>


//! CTLoadEComCnvStep
/*! 
  This class is used to test ecom style converter plugin and its APIs
*/
class CTLoadEComCnvStep : public CTestStep
	{
public:
	CTLoadEComCnvStep();
	virtual TVerdict doTestStepL();
	void ExecuteL();
	void TestListsL();
private:
	~CTLoadEComCnvStep();
private:
	TDataType mimeEngEpoc32CPlaintext;
	TDataType mimeFrenchxyzzyFrot;
	};


_LIT(KTLoadEComCnvStep,"TLoadEComCnv");
#endif

