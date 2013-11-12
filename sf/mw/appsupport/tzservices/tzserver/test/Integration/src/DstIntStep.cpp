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

#include "DstIntStep.h"

_LIT(KTagCity, "City");

CDstIntStep::CDstIntStep()
	{
	}

void CDstIntStep::GetCityParametersL()
	{
	TPtrC ptr, ptr2;

	// Set London as the default
	iCityName.Copy(KEuropeLondon);
	iCityId = 0;

	// Reads the city name from the cities section
	GetStringFromConfig(ConfigSection(), KTagCity, ptr);
	if(ptr.Length() > 0)
		{
		GetStringFromConfig(KSectionCities, ptr, ptr2);
		if(ptr2.Length() > 0)
			{
			iCityName.Copy(ptr2);
			iCityId = 0;
			}
		}
	}


	
