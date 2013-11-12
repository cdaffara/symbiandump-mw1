/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __DSTINTSTEP_H__
#define __DSTINTSTEP_H__

#include "DstStep.h"
#include "DstIntServer.h"

const TInt KMaxLocationChars = 64;
const TInt KMaxCityNameChars = 64;



_LIT(KSectionCities, "Cities");

// Common Tags for all the scripts
_LIT8(KEuropeLondon, "Europe/London");
_LIT(KTagLocation,			"LocStr");
_LIT(KTagLocationId,		"LocID");


class CDstIntStep : public CDstStep
	{
public:
	CDstIntStep();

	void GetCityParametersL();

	// These store the city name and Id
	TBuf8<KMaxCityNameChars> iCityName;
	TInt iCityId;
	};

#endif



