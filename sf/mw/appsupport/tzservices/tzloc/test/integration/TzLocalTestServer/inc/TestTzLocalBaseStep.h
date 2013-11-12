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



#ifndef __TESTTZLOCALBASESTEP_H__
#define __TESTTZLOCALBASESTEP_H__

#include <test/testexecutestepbase.h>
#include "TzLocalTestServer.h"
#include "tzlocalizer.h"

namespace tzconstants
    {
    _LIT(KTzIdTag, "TzId");
    _LIT(KStandardNameTag,"TimeZoneStandardName");
    _LIT(KDaylightNameTag, "TimeZoneDaylightSavingName");
    _LIT(KShortStandardName, "TimeZoneShortStandardname");
    _LIT(KShortDaylightName, "TimeZoneShortDaylightSavingName");
    _LIT(KInputCityTag, "InputCity");
    _LIT(KUserDefinedCityTag, "UserDefinedCity");
    _LIT(KCachedTzTag, "CachedTzType");
    _LIT(KCityGroupIdIdTag, "CityGroupId");
    _LIT(KSortKeyTag, "SortKey");
    _LIT(KCityTag, "City");
    _LIT(KDefaultCityTag, "DefaultCity");
    _LIT(KCityGroupNameTag, "CityGroupName");
    _LIT(KGroupNameTag, "GroupName");
    _LIT(KLanguageTag, "Language");
    _LIT(KUtcOffsetTag, "UtcOffset");
    _LIT(KSetTzInTzLocalTag,"SetTzInTzLocal");
    _LIT(KDataSourceTag,"DataSource");
    const TInt KDefaultTzId = 2592; 
    const TInt KDefaultCityGroupId = 1;
    const TInt KTagLength = 25;
    const TInt KDefaultUtcOffset = 0;
    }
class CTzUserData;
class CTestTzLocalBaseStep : public CTestStep
	{
public:
      CTestTzLocalBaseStep();
      ~CTestTzLocalBaseStep(); 
      
protected: 
        TBool CheckReturnedTzInfo (const CTzLocalizedTimeZone& aTz);
        void CreateUserRuleLC(CTzUserData& aUserData);
        void ResetTzUserDataL();
	};


#endif
