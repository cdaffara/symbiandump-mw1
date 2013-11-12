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
//

/**
 @file
 @internalComponent - Internal Symbian test code  
*/

#if (!defined __TB64CNV_STEP_H__)
#define __TB64CNV_STEP_H__
#include "TConArcTestServer.h"
#include <concnf.h>
#include <conlist.h>

class CTB64CnvStep : public CTestStep
	{
public:
	CTB64CnvStep();
	~CTB64CnvStep();
//	virtual TVerdict doTestStepPreambleL();
//	virtual TVerdict doTestStepPostambleL();
	virtual TVerdict doTestStepL();
private:
	void DoTestL();
	void DoSpecificL(CCnaConverterList* aList);
	void DoTestQPL(CCnaConverterList* aList);
	void DoTestBase64L(CCnaConverterList* aList);
	void DoTextEtextL(CCnaConverterList* aList);
	void DoTextEtextWithControlCharsL(CCnaConverterList* aList);
	void ConvertFileL(CCnaConverterList& aList, TUint aConverterUid, const TDesC& aInputFile, const TDesC& aOutputFile);
	static TBool FilesAreIdenticalL(RFs& aFs, const TDesC& aFile1, const TDesC& aFile2);
 
private:
	RFs iFs;
	};

_LIT(KTB64CnvStep,"TB64Cnv");
#endif

