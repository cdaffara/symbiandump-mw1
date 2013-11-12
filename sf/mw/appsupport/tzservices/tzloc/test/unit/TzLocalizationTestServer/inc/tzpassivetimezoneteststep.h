// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __TZPASSIVETIMEZONETESTSTEP_H__
#define __TZPASSIVETIMEZONETESTSTEP_H__

#include "TzLocalizationTestStep.h"

_LIT(KTzPassiveTimeZoneTestStep,"TzPassiveTimeZoneTestStep");

class CTzPassiveTimeZoneTestStep : public CTzLocalizationTestStep
	{
public:
	CTzPassiveTimeZoneTestStep();
	~CTzPassiveTimeZoneTestStep();
	virtual TVerdict doTestStepL();

private:
	TBool GetTimeZoneFromTimeZoneIdTest();
	TBool GetTimeZoneFromCityTest();
	};

#endif
