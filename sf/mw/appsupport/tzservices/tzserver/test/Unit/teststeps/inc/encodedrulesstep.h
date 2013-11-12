/**
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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


 
#ifndef __ENCODEDRULESSTEP_H__

#include "DstUnitStep.h"

// String for the server create test step code
_LIT(KEncodedRulesStep, "EncodedRules");

class TTzRule;

class CEncodedRulesStep : public CDstUnitStep
	{
public:
	CEncodedRulesStep(CDstUnitServer &aServer);
	~CEncodedRulesStep();

private:
	// from CDstStep
	TInt CreateAndRunTestUnitL();
	void DestroyTestUnit();

	void ReadTestData(TPtrC& aLocation, TInt& aStartYear, TInt& aEndYear, TInt& aNumberOfRules, TInt& aCompareRules);
	void ReadExpectedRulesL(CTzRules& aExpectedRules, TInt aExpectedNumberOfRules);
	TTzRule* ReadExpectedRuleL(const TDesC& aRuleSection);
	TBool CompareRuleCollections(CTzRules& aRules1, CTzRules& aRules2);
	TBool CompareRules(const TTzRule& aRule1, const TTzRule& aRule2);

	};

#endif // __ENCODEDRULESSTEP_H__
