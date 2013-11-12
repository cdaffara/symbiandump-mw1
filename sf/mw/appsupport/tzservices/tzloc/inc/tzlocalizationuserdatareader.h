// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef TZLOCALIZATIONUSERDATAREADER_H_
#define TZLOCALIZATIONUSERDATAREADER_H_

#include <e32base.h> //CBase
#include "TzLocalizationDataTypes.h"

class CTzUserData;
class CTzUserNames;
class RTz;

NONSHARABLE_CLASS(CTzLocalizationUserDataReader) : public CBase
	{
public:
	static CTzLocalizationUserDataReader* NewL(RTz& aTzSession);
	static CTzLocalizationUserDataReader* NewLC(RTz& aTzSession);
	~CTzLocalizationUserDataReader();

	CTzLocalizedTimeZone* ReadTimeZoneL(const TInt aTimeZoneId);
	CTzLocalizedTimeZone* ReadTimeZoneL(const TTzLocalizedId& aTzLocalizedId);
	void ReadAllTimeZonesL(CTzLocalizedTimeZoneArray& aTimeZones);
	void ReadCitiesL(CTzLocalizedCityArray& aCities);
	void ReadCitiesL(CTzLocalizedCityArray& aCities, const TInt aTimeZoneId);
	void ReadCitiesInGroupL(CTzLocalizedCityArray& aCities, const TUint8 aGroupId);
	void ReadAllGroupsL(CTzLocalizedCityGroupArray& aGroups);
	CTzLocalizedCity* ReadDefaultCityL(const TInt aTimeZoneId);

private:
	CTzLocalizationUserDataReader(RTz& aTzSession);
	void ConstructL();
	
	CTzUserNames* GetTzUserNamesLC(TInt aTimeZoneId);
	CTzLocalizedTimeZone* GetLocalizedTimeZoneL(TInt aTimeZoneId);
	CTzLocalizedCity* GetLocalizedCityL(TInt aTimeZoneId);
	CTzLocalizedCityGroup* GetLocalizedCityGroupL(TInt aTimeZoneId);
	
private:
	RTz& iTzSession;
	CTzUserData* iUserData;
	};

#endif /*TZLOCALIZATIONUSERDATAREADER_H_*/
