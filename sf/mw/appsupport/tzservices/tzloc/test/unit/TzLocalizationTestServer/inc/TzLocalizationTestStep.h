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

#ifndef __CTZLOCALIZATIONTEST_STEP_H__
#define __CTZLOCALIZATIONTEST_STEP_H__

#include <test/testexecutestepbase.h>
#include "TzLocalizationTestServer.h"
#include "TzLocalizer.h"
#include <e32property.h>
#include <tzupdate.h>

// Base class for all test steps
class CTzLocalizationTestStep : public CTestStep
	{
public:
	CTzLocalizationTestStep();
	~CTzLocalizationTestStep();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepPostambleL();
protected:
	CTzLocalizer* iLocalizer;
	void PrintResult(const TBool aResult);
	TBool CompareTimeZoneWithIniFileSection(
		const CTzLocalizedTimeZone& aTimeZone,
		const TDesC& aSectName);
	TBool CompareCityWithIniFileSection(
		const CTzLocalizedCity& aCity,
		const TDesC& aSectName,
		const TInt aCityNumber);
	TBool CityArrayContainsCity(
		CTzLocalizedCityArray* aCityArray,
		CTzLocalizedCity* aCity);
	TBool CheckCitiesinGroup(
		const CTzLocalizedCityArray& aCityArray,
		const TDesC& aGroupSectName);
	TBool CheckFrequentlyUsedZoneL(
		const CTzLocalizedTimeZone::TTzFrequentlyUsedZone aFrequentlyUsedZone,
		const TDesC& aTimeZoneSectName,
		const TInt aCityNumber);
	void PrintSortOrder(CTzLocalizer::TTzSortOrder aSortOrder);
	CTzLocalizer::TTzSortOrder GetSortOrder(TDesC& aIniTag);
	TInt GetTimeZoneIdFromTzServerL();
private:
	CActiveScheduler* iScheduler;
	};

class CTzInvalidTimeZoneDbTestStep : public CTzLocalizationTestStep
	{
public:
	CTzInvalidTimeZoneDbTestStep();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepPostambleL();
	~CTzInvalidTimeZoneDbTestStep();
	virtual TVerdict doTestStepL();
private:
	CTzLocalizer* iLocalizer;
	CActiveScheduler* iScheduler;
	TInt timeZoneId;
	TInt cityGroupId;
	TBool DetectCorruptTimeZoneDbTestL();
	void GetAllTimeZonesL();
	void AddCityL();
	void GetCitiesInTimeZoneL();
	void GetCitiesInGroupL();
	void LocalizeTimeZonesL();
	};
	
class CTzPassiveAllTimeZonesTestStep : public CTzLocalizationTestStep
	{
public:
	CTzPassiveAllTimeZonesTestStep();
	~CTzPassiveAllTimeZonesTestStep();
	virtual TVerdict doTestStepL();
private:
	TBool GetAllTimeZonesTestL();
	};

class CTzActiveTimeZoneTestStep : public CTzLocalizationTestStep
	{
public:
	CTzActiveTimeZoneTestStep();
	~CTzActiveTimeZoneTestStep();
	virtual TVerdict doTestStepL();
private:
	TBool SetAndGetFrequentlyUsedZoneFromTimeZoneIdL();
	TBool SetAndGetFrequentlyUsedZoneFromTimeZoneL();
	TBool SetAndGetFrequentlyUsedZoneFromCityL();
	};

class CTzPassiveCityTestStep : public CTzLocalizationTestStep
	{
public:
	CTzPassiveCityTestStep();
	~CTzPassiveCityTestStep();
	virtual TVerdict doTestStepL();
private:
	TBool GetDefaultCityFromTimeZoneIdTestL();
	TBool GetDefaultCityFromAnotherCityTestL();
	TBool GetDefaultCityFromTimeZoneTestL();
	TBool GetCitiesFromTimeZoneIdTestL();
	TBool GetCitiesFromTimeZoneTestL();
	TBool GetCitiesFromAnotherCityTestL();
	};

class CTzPassiveAllCitiesTestStep : public CTzLocalizationTestStep
	{
public:
	CTzPassiveAllCitiesTestStep();
	~CTzPassiveAllCitiesTestStep();
	virtual TVerdict doTestStepL();
private:
	TBool GetAllCitiesTestL();
	};
	
class CTzCitiesWithUTCOffsetTestStep : public CTzLocalizationTestStep
	{
public:
	CTzCitiesWithUTCOffsetTestStep();
	~CTzCitiesWithUTCOffsetTestStep();
	virtual TVerdict doTestStepL();
private:
	TBool GetAllCitiesWithUTCOffsetTestL();
	};

class CTzTimeZonesWithUTCOffsetTestStep : public CTzLocalizationTestStep
	{
public:
	CTzTimeZonesWithUTCOffsetTestStep();
	~CTzTimeZonesWithUTCOffsetTestStep();
	virtual TVerdict doTestStepL();
private:
	TBool GetAllTimeZonesWithUTCOffsetTestL();
	};


class CTzActiveCityTestStep : public CTzLocalizationTestStep
	{
public:
	CTzActiveCityTestStep();
	~CTzActiveCityTestStep();
	virtual TVerdict doTestStepL();
private:
	TBool AddRemoveCityTestL();
	};

class CTzGroupTestStep : public CTzLocalizationTestStep
	{
public:
	CTzGroupTestStep();
	~CTzGroupTestStep();
	virtual TVerdict doTestStepL();
private:
	TBool GetCityGroupFromGroupIdTestL();
	TBool GetCityGroupFromCityTestL();
	TBool GetCitiesInGroupFromGroupIdTestL();
	TBool GetCitiesInGroupFromCityTestL();
	TBool GetCitiesInGroupFromGroupTestL();
	};

class CTzAllGroupsTestStep : public CTzLocalizationTestStep
	{
public:
	CTzAllGroupsTestStep();
	~CTzAllGroupsTestStep();
	virtual TVerdict doTestStepL();
private:
	TBool GetAllGroupsTestL();
	};

class CTzSortingTestStep : public CTzLocalizationTestStep
	{
public:
	CTzSortingTestStep();
	~CTzSortingTestStep();
	virtual TVerdict doTestStepL();
private:
	TBool SortTimeZonesTestL(CTzLocalizer::TTzSortOrder aSortOrder);
	TBool SortCitiesTestL(CTzLocalizer::TTzSortOrder aSortOrder, TBool aCitiesInGroup=EFalse);
	TBool SortGroupsTestL(CTzLocalizer::TTzSortOrder aSortOrder);
	};

class CTzFindingTestStep : public CTzLocalizationTestStep
	{
public:
	CTzFindingTestStep();
	~CTzFindingTestStep();
	virtual TVerdict doTestStepL();
private:
	TBool FindTimeZoneTestL();
	TBool FindCityTestL();
	TBool FindGroupTestL();
	};

class CTzTimeZonesSpeedTestStep : public CTzLocalizationTestStep
	{
public:
	CTzTimeZonesSpeedTestStep();
	~CTzTimeZonesSpeedTestStep();
	virtual TVerdict doTestStepL();
private:
	TBool GetAllZonesSpeedTestL();
	};

class CTzCitiesSpeedTestStep : public CTzLocalizationTestStep
	{
public:
	CTzCitiesSpeedTestStep();
	~CTzCitiesSpeedTestStep();
	virtual TVerdict doTestStepL();
private:
	TBool GetAllCitiesSpeedTestL();
	};

class CTzGroupsSpeedTestStep : public CTzLocalizationTestStep
	{
public:
	CTzGroupsSpeedTestStep();
	~CTzGroupsSpeedTestStep();
	virtual TVerdict doTestStepL();
private:
	TBool GetCitiesInGroupSpeedTestL();
	};

class CPropertyWatcher : public CActive
	{
public:
	static CPropertyWatcher* NewL();
	~CPropertyWatcher();
	
	void Start();
	TInt PubSubTzId() const;
	TInt OldPubSubTzId() const;

	// from CActive
	void RunL();
	void DoCancel();

private:
	CPropertyWatcher();
	void ConstructL();

	RProperty iProperty;
	NTzUpdate::TTimeZoneChange iPubSubChange;
	};

class CTzPubSubTestStep : public CTzLocalizationTestStep
	{
public:
	CTzPubSubTestStep();
	~CTzPubSubTestStep();
	virtual TVerdict doTestStepL();
private:
	void TestTimeZoneIdL(TInt aTzId);
	
	CPropertyWatcher* iPropertyWatcher;
	TInt iOldTzId;
	};

class CTzRaceTestStep : public CTzLocalizationTestStep
	{
public:
	CTzRaceTestStep() ;
	~CTzRaceTestStep() ;
	virtual TVerdict doTestStepL();
       
	static TInt TThreadFunction1(TAny* arg) ;
	static TInt TThreadFunction2(TAny* arg) ;

private:
	RThread    iThread1 ;
	RThread    iThread2 ;   
 
	};

class CPrepEnvDelFileStep : public CTestStep
	{
public:
	CPrepEnvDelFileStep();
	~CPrepEnvDelFileStep();
	virtual TVerdict doTestStepL();
	};

class CPrepEnvCopyFileStep : public CTestStep
	{
public:
	CPrepEnvCopyFileStep();
	~CPrepEnvCopyFileStep();
	virtual TVerdict doTestStepL();
	};

_LIT(KTzInvalidTimeZoneDbTestStep,"TzBootWithInvalidTimeZoneDbTestStep");
_LIT(KTzPassiveAllTimeZonesTestStep,"TzPassiveAllTimeZonesTestStep");
_LIT(KTzActiveTimeZoneTestStep,"TzActiveTimeZoneTestStep");
_LIT(KTzPassiveCityTestStep,"TzPassiveCityTestStep");
_LIT(KTzPassiveAllCitiesTestStep,"TzPassiveAllCitiesTestStep");
_LIT(KTzCitiesWithUTCOffsetTestStep,"TzCitiesWithUTCOffsetTestStep");
_LIT(KTzTimeZonesWithUTCOffsetTestStep,"TzTimeZonesWithUTCOffsetTestStep");
_LIT(KTzActiveCityTestStep,"TzActiveCityTestStep");
_LIT(KTzGroupTestStep,"TzGroupTestStep");
_LIT(KTzAllGroupsTestStep,"TzAllGroupsTestStep");
_LIT(KTzSortingTestStep,"TzSortingTestStep");
_LIT(KTzFindingTestStep,"TzFindingTestStep");
_LIT(KTzTimeZonesSpeedTestStep,"TzTimeZonesSpeedTestStep");
_LIT(KTzCitiesSpeedTestStep,"TzCitiesSpeedTestStep");
_LIT(KTzGroupsSpeedTestStep,"TzGroupsSpeedTestStep");
_LIT(KTzPubSubTestStep, "TzPubSubTestStep");
_LIT(KTzRaceTestStep, "TzRaceTestStep");
_LIT(KTzPrepEnvDelFileTestStep, "TzPrepEnvDelFileTestStep");
_LIT(KTzPrepEnvCopyFileTestStep, "TzPrepEnvCopyFileTestStep");

#endif
