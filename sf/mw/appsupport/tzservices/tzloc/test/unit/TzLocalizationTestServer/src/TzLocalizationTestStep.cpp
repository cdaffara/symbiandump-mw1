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

#include "TzLocalizationTestStep.h"
#include <test/testexecutelog.h>
#include <e32debug.h>
#include <tzupdate.h>
#include <tz.h>
#include "testserver.h"

// Time Zone Tests
_LIT(KBootWithCorruptTimeZoneDbTest,"Detect a corrupt or invalid timezone.");	
_LIT(KGetAllTimeZonesTest,"Get All Time Zones.");

_LIT(KSetAndGetFrequentlyUsedZoneFromTimeZoneId,"Set and get a cached zone and its city using a timezone id");
_LIT(KSetAndGetFrequentlyUsedZoneFromTimeZone,"Set and get a cached zone and its city using a timezone");
_LIT(KSetAndGetFrequentlyUsedZoneFromCity,"Set and get a cached zone and its city using a city");
const TInt KMaxCityBufLength = 26;

	// City Tests
_LIT(KGetDefaultCityFromTimeZoneIdTest,"Get the Default City from a Time Zone ID");
_LIT(KGetDefaultCityFromAnotherCityTest,"Get the Default City from a City");
_LIT(KGetDefaultCityFromTimeZoneTest,"Get the Default City from a Time Zone object");
_LIT(KGetCitiesFromTimeZoneIdTest,"Get Cities in a time zone from a Time Zone ID");
_LIT(KGetCitiesFromTimeZoneTest,"Get Cities in a time zone from a Time Zone object");
_LIT(KGetCitiesFromAnotherCityTest,"Get Cities in a Time Zone, given another City object");
_LIT(KGetAllCitiesTest,"Get all Cities");
_LIT(KAddRemoveCityTest,"Adding and Removing a City Test");

_LIT(KCityInfo,"Testing new city info is correct");
_LIT(KTestCityInAllCitiesArray,"Testing new city is in all cities array");
_LIT(KTestCityInArrayFromTimeZoneId,"Testing new city is in cities array from time zone id");
_LIT(KTestCityInArrayFromDefaultCity,"Testing new city is in cities array from default city");
_LIT(KTestCityInArrayFromTzObject,"Testing new city is in cities array from time zone object");
_LIT(KTestCityInArrayFromNewCity,"Testing new city is in cities array from new city");
_LIT(KTestTimezoneFromNewCity,"Testing time zone retrieved using new city.");
_LIT(KTestDefaultCityFromNewCity,"Testing default city retrieved using new city.");
_LIT(KTestGroupFromNewCity,"Testing group retrieved using new city.");
_LIT(KTestCityArrayFromNewCity,"Testing city array retrieved using new city.");
_LIT(KTestDeleteNewCity,"Testing deletion of new city.");

	// Group Tests
_LIT(KGetCityGroupFromGroupIdTest,"Get a Group from a group id");
_LIT(KGetCityGroupFromCityTest,"Get a group from a city object");
_LIT(KGetCitiesInGroupFromGroupIdTest,"Get cities in a group from a group id");
_LIT(KGetCitiesInGroupFromCityTest,"Get cities in a group from a city object");
_LIT(KGetCitiesInGroupFromGroupTest,"Get cities in a group from a group object");
_LIT(KGetAllGroupsTest,"Getting All Groups");

	//Test get cities with UTC offset
_LIT(KGetCitiesWithUTCOffsetTest,"Get all cities with a matching UTC offset Unsorted");
	//Test get time zones with UTC offset
_LIT(KGetTimeZonesWithUTCOffsetTest,"Get all timezones with a matching UTC offset Unsorted");

	// Sorting Tests
_LIT(KSortTimeZonesTest,"Test that time zones are sorted properly");
_LIT(KSortCitiesTest,"Test that cities are sorted properly");
_LIT(KSortGroupsTest,"Test that group are sorted properly");
_LIT(KSortCitiesInGroupsTest,"Test that cities in group are sorted properly");

	// Finding Tests
_LIT(KFindTimeZoneTest,"Test the finding of a time zone");
_LIT(KFindCityTest,"Test the finding of a city");
_LIT(KFindGroupTest,"Test the finding of a group");


	// Speed tests
_LIT(KGetAllZonesSpeedTest,"Speed testing get all time zones");
_LIT(KGetAllCitiesSpeedTest,"Speed testing get resource-defined cities");
_LIT(KGetCitiesInGroupSpeedTest,"Speed testing get cities in group");
_LIT(KGetAllCitiesIncludingDbCitiesSpeedTest,"Speed Testing get all cities");

// Ini file constants

_LIT(KIniTimeZoneTag,"TestZone");
const TInt KIniTagLength = 20;

// Time Zone Tags
_LIT(KIniTimeZoneIdTag,"time_zone_id");
_LIT(KIniStandardNameTag,"standard_name");
_LIT(KIniDaylightNameTag,"daylight_name");
_LIT(KIniShortStandardNameTag,"short_standard_name");
_LIT(KIniShortDaylightNameTag,"short_daylight_name");
_LIT(KIniNumberOfTimeZonesTag,"number_of_zones");
_LIT(KIniAllZonesTag,"AllZonesTest");

// City Tags
_LIT(KIniCityNameTag,"city_name");
_LIT(KIniCityGroupIdTag,"city_group_id");
_LIT(KIniCityGroupNameTag,"city_group_name");
_LIT(KIniNumberOfCitiesTag,"number_of_cities");

_LIT(KIniRemove,"remove");

//UTC Test Tags
_LIT(KIniUTCTag,"utc_offset");

// group tags
_LIT(KIniGroupTag,"TestGroup");
_LIT(KIniGroupIdTag,"city_group_id");
_LIT(KIniGroupName,"city_group_name");
_LIT(KIniNumberOfGroupsTag,"number_of_groups");

	// sort tags
_LIT(KIniSortStepTag,"sort_test");
_LIT(KIniSortStepTimeZoneOptionTag,"time_zone");
_LIT(KIniSortStepCityOptionTag,"city");
_LIT(KIniSortStepGroupOptionTag,"group");
_LIT(KIniSortStepCitiesInGroupOptionTag,"cities_in_group");

_LIT(KIniSortStepTypeTag,"sort_test_type");
_LIT(KIniSortStepTypeUTCAOptionTag,"utc_ascending");
_LIT(KIniSortStepTypeUTCDOptionTag,"utc_descending");
_LIT(KIniSortStepTypeAlphaAOptionTag,"alpha_name_ascending");
_LIT(KIniSortStepTypeAlphaDOptionTag,"alpha_name_descending");
_LIT(KIniSortStepTypeStandardAOptionTag,"standard_name_ascending");
_LIT(KIniSortStepTypeStandardDOptionTag,"standard_name_descending");
_LIT(KIniSortStepTypeDaylightAOptionTag,"daylight_name_ascending");
_LIT(KIniSortStepTypeDaylightDOptionTag,"daylight_name_descending");
_LIT(KIniSortStepTypeShortStandardAOptionTag,"short_standard_name_ascending");
_LIT(KIniSortStepTypeShortStandardDOptionTag,"short_standard_name_descending");
_LIT(KIniSortStepTypeShortDaylightAOptionTag,"short_daylight_name_ascending");
_LIT(KIniSortStepTypeShortDaylightDOptionTag,"short_daylight_name_descending");

	// speed test tags
_LIT(KIniAllTimeZonesLimit,"all_zones_test_limit");
_LIT(KIniAllCitiesLimit,"all_cities_test_limit");
_LIT(KIniAllCitiesInGroupLimit,"all_cities_in_group_limit");

// Other strings
_LIT(KPreAmble,"Test Step Preamble");
_LIT(KPostAmble,"Test Step Postamble");
_LIT(KInTestStep,"In test step");
_LIT(KPassed,"Passed");
_LIT(KFailed,"Failed");
_LIT(KStartingTimedTest,"Starting Timed Test");
_LIT(KFinishedTimedTest,"Finished Timed Test");
_LIT(KTimedTestTook,"Test took ");
_LIT(KSeconds," seconds");
_LIT(KNumberRead,"Number of items read: ");
_LIT(KTab,"\t");
_LIT(KExpected,"Expected\tActual");
_LIT(KNewCity,"NewCity");
//Sort Order Labels
_LIT(KUnsorted,"Unsorted");
_LIT(KUTCAscending,"UTCAscending");
_LIT(KUTCDescending,"UTCDescending");
_LIT(KAlphaNameAscending,"AlphaNameAscending");
_LIT(KAlphaNameDescending,"AlphaNameDescending");
_LIT(KAlphaStandardNameAscending,"AlphaStandardNameAscending");
_LIT(KAlphaStandardNameDescending,"AlphaStandardNameDescending");
_LIT(KAlphaDaylightNameAscending,"AlphaDaylightNameAscending");
_LIT(KAlphaDaylightNameDescending,"AlphaDaylightNameDescending");
_LIT(KAlphaShortStandardNameAscending,"AlphaShortStandardNameAscending");
_LIT(KAlphaShortStandardNameDescending,"AlphaShortStandardNameDescending");
_LIT(KAlphaShortDaylightNameAscending,"AlphaShortDaylightNameAscending");
_LIT(KAlphaShortDaylightNameDescending,"AlphaShortDaylightNameDescending");

// Other Constants
_LIT(KLocDb,"c:\\private\\100012a5\\DBS_10206A8B_TzLocalization.db");
_LIT(KBadLocDb,"c:\\TzLocalizationTestScripts\\DBS_10206a8b_TZLOCALIZATION_bad.DB");
 
const TInt currentTimeZoneId = 2592; //SX_ re-introducing
const TInt KTime = 100 ;    //used in CTzRaceTestStep

//=============================================================================
// CTzLocalizationTestStep
//=============================================================================
CTzLocalizationTestStep::~CTzLocalizationTestStep()
/**
 * Destructor
 */
	{
	delete iLocalizer;
	delete iScheduler;
	}

CTzLocalizationTestStep::CTzLocalizationTestStep()
/**
 * Constructor
 */
	{
	}

TVerdict CTzLocalizationTestStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	RDebug::Print(TestStepName());
	INFO_PRINTF1(TestStepName());
	INFO_PRINTF1(KPreAmble);

	iScheduler = new CActiveScheduler();
	CActiveScheduler::Install(iScheduler);

	// Set the system time zone to a timezone that is known to be
	// provisioned in the timezone localisation database and the timezone
	// server database.  This will prevent CTzLocalizer leaving when constructed
	// because a previous test may have set the system timezone to a value that
	// was not provisioned in the localisation database.
/*	_LIT8(KSystemTimeZone, "Europe/London");
	CTzId* newCTzId = CTzId::NewL(KSystemTimeZone);
	CleanupStack::PushL(newCTzId);
	RTz tz;
	User::LeaveIfError(tz.Connect());
	CleanupClosePushL(tz);
	tz.SetTimeZoneL(*newCTzId);
	CleanupStack::PopAndDestroy(); //tz
	CleanupStack::PopAndDestroy(newCTzId);
SX_ removing */	
	iLocalizer = CTzLocalizer::NewL();
	
	iLocalizer->SetTimeZoneL(currentTimeZoneId); //SX_ re-introducing
	
	SetTestStepResult(EPass);
	return TestStepResult();
	}

TVerdict CTzLocalizationTestStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	INFO_PRINTF1(KPostAmble);
	delete iLocalizer;
	iLocalizer = NULL;

	delete iScheduler;
	iScheduler = NULL;
	CActiveScheduler::Install(NULL);

	return TestStepResult();
	}

/**
Print passed or failed depending on the argument.

@param aResult The boolean to test.

@internalTechnology
*/
void CTzLocalizationTestStep::PrintResult(const TBool aResult)
	{
	if(aResult)
		{
		INFO_PRINTF1(KPassed);
		}
	else
		{
		INFO_PRINTF1(KFailed);
		}
	}

/**
Performs a comparison between a CTzLocalizedTimeZone object and a time zone
specified in the ini file.

@param aTimeZone The CTzLocalizedTimeZone to compare.

@param aSectName The ini file seection specifying a time zone to compare.

@return ETrue if the data are the same, EFalse otherwise

@internalTechnology
*/
TBool CTzLocalizationTestStep::CompareTimeZoneWithIniFileSection(const CTzLocalizedTimeZone& aTimeZone, const TDesC& aSectName)
	{
	TBool result = ETrue;

	// Get Test data from ini file
	TInt iniTimeZoneId;
	GetIntFromConfig(aSectName,KIniTimeZoneIdTag,iniTimeZoneId);
	TPtrC iniStandardName;
	GetStringFromConfig(aSectName,KIniStandardNameTag,iniStandardName);
	TPtrC iniDaylightName;
	GetStringFromConfig(aSectName,KIniDaylightNameTag,iniDaylightName);
	TPtrC iniShortStandardName;
	GetStringFromConfig(aSectName,KIniShortStandardNameTag,iniShortStandardName);
	TPtrC iniShortDaylightName;
	GetStringFromConfig(aSectName,KIniShortDaylightNameTag,iniShortDaylightName);

	// Test the values
	if(aTimeZone.TimeZoneId() != iniTimeZoneId)
		{
		_LIT(KSXint,"Got Tz zone id %d - but in inifile expected = %d ");
     	INFO_PRINTF3(KSXint, aTimeZone.TimeZoneId(), iniTimeZoneId );
     	result = EFalse;
		}
	if(aTimeZone.StandardName() != iniStandardName)
		{
		result = EFalse;
		TPtrC temp(aTimeZone.StandardName() );
		if ( (&temp)->Length()!=0  )
 			{
			_LIT(KSX1,"CTzLocalizationTestStep: got Tz StandardName %S, whilst in ini %S \n");
  			INFO_PRINTF3(KSX1, &temp, &iniStandardName);
			}
		else if ( (&iniStandardName)->Length()!=0 )
			{
				_LIT(KNoMatch,"CTzLocalizationTestStep: Tz StandardName Did not Match, expected: %S");
	    		ERR_PRINTF2(KNoMatch, &iniStandardName);		
			}
			else
			{
				result = EPass;
			} //else if		
		}
	if(aTimeZone.DaylightName() != iniDaylightName)
		{
		result = EFalse;
		TPtrC temp(aTimeZone.DaylightName() );
		if ( (&temp)->Length()!=0  )
 			{
			_LIT(KSX1,"CTzLocalizationTestStep: got Tz DaylightName %S, whilst in ini %S \n");
  			INFO_PRINTF3(KSX1, &temp, &iniDaylightName);
			}
		else if ( (&iniDaylightName)->Length()!=0 )
				{
				_LIT(KNoMatch,"CTzLocalizationTestStep: Tz DaylightName Didnot Match, expected: %S");
	    		ERR_PRINTF2(KNoMatch, &iniDaylightName);		
				}
				else
				{
				result = EPass;
				} //else if
		}
	if(aTimeZone.ShortStandardName() != iniShortStandardName)
		{
		result = EFalse;
		TPtrC temp(aTimeZone.ShortStandardName() );
		if ( (&temp)->Length()!=0  )
 			{
			_LIT(KSX1,"CTzLocalizationTestStep: got Tz ShortStandardName %S, whilst in ini %S \n");
  			INFO_PRINTF3(KSX1, &temp, &iniShortStandardName);
			}
		else if ( (&iniShortStandardName)->Length()!=0 )
				{
				_LIT(KNoMatch,"CTzLocalizationTestStep: Tz ShortStandardName Did not Match, expected: %S");
	    		ERR_PRINTF2(KNoMatch, &iniShortStandardName);		
				}
				else
				{
				result = EPass;
				} //else if
		}
	if(aTimeZone.ShortDaylightName() != iniShortDaylightName)
		{
		result = EFalse;
		TPtrC temp(aTimeZone.ShortDaylightName() );
		if ( (&temp)->Length()!=0  )
 			{
			_LIT(KSX1,"CTzLocalizationTestStep: got Tz ShortDaylightName %S, whilst in ini %S \n");
  			INFO_PRINTF3(KSX1, &temp, &iniShortDaylightName);
			}
		else if ( (&iniShortDaylightName)->Length()!=0 )
				{
				_LIT(KNoMatch,"CTzLocalizationTestStep: Tz ShortDaylightName Didnot Match, expected: %S");
	    		ERR_PRINTF2(KNoMatch, &iniShortDaylightName);		
				}
				else
				{
				result = EPass;
				} //else if
		}

	return result;
	}

/**
Performs a comparison between a CTzLocalizedCity object and a city specified in
the ini file.

@param aCity The city to compare.

@param aSectName The ini file section containing the city

@param aCityNumber The city number - starting from 1.

@return

@internalTechnology
*/
TBool CTzLocalizationTestStep::CompareCityWithIniFileSection(const CTzLocalizedCity& aCity, const TDesC& aSectName, const TInt aCityNumber)
	{
	TBool result = ETrue;

	// Get data from ini file section
	// Create the necessary tags for getting the correct city's info from the ini
	// e.g. city_name1, city_group_id1 <- the "1" needs to be added to the tags

	TBuf<KIniTagLength> iniCityNameTag(KIniCityNameTag);
	iniCityNameTag.AppendNum(aCityNumber);
	TBuf<KIniTagLength> iniCityGroupIdTag(KIniCityGroupIdTag);
	iniCityGroupIdTag.AppendNum(aCityNumber);

	// Get data from the ini file
	TPtrC iniCityName;
	GetStringFromConfig(aSectName,iniCityNameTag,iniCityName);
	TInt iniCityGroupId;
	GetIntFromConfig(aSectName,iniCityGroupIdTag,iniCityGroupId);
	TInt iniTimeZoneId;
	GetIntFromConfig(aSectName,KIniTimeZoneIdTag,iniTimeZoneId);

	if(aCity.Name() != iniCityName)
		{
       	TPtrC temp(aCity.Name() );
		if ( (&temp)->Length()!=0  )
			{
            _LIT(KSX1,"CompareCityWithIniFileSection: aCity.Name() was %S,whilst expecting %S  \n");
 			INFO_PRINTF3(KSX1,  &temp, &iniCityName);
			}
		result = EFalse;
		}
	if(aCity.GroupId() != iniCityGroupId)
		{
		_LIT(KSXint,"Tz cities GroupId= %d whilst expecting %d  ");
	   	INFO_PRINTF3(KSXint, aCity.GroupId() , iniCityGroupId );
		result = EFalse;
		}
	if(aCity.TimeZoneId() != iniTimeZoneId)
		{
		_LIT(KSXint,"Tz cities TimeZoneId= %d whilst expecting %d  ");
	   	INFO_PRINTF3(KSXint, aCity.TimeZoneId(), iniTimeZoneId );
		result = EFalse;
		}

	return result;
	}

/**
Checks that a city array contains a particular city.

@param aCityArray The city array to search.

@param aCity The city to search for.

@return ETrue if the city is found in the array, EFalse if not.

@internalTechnology
*/
TBool CTzLocalizationTestStep::CityArrayContainsCity(CTzLocalizedCityArray* aCityArray, CTzLocalizedCity* aCity)
	{
	TBool result = EFalse;

	TInt count = aCityArray->Count();
	for(TInt i=0; i<count; ++i)
		{
		if(	aCityArray->At(i).Name() == aCity->Name() &&
			aCityArray->At(i).TimeZoneId() == aCity->TimeZoneId() &&
			aCityArray->At(i).GroupId() == aCity->GroupId() )
			{
			// We've found it!!
			result = ETrue;
			break;
			}
		}

	return result;
	}

/**
Checks that the array of cities are all in the specified group.

@param aCityArray The city array to search.

@param aGroupSectName The ini file section containing the group information.

@return ETrue if the city array contains the correct cities, EFalse if not

@internalTechnology
*/
TBool CTzLocalizationTestStep::CheckCitiesinGroup(const CTzLocalizedCityArray& aCityArray, const TDesC& aGroupSectName)
	{
	TBool result = ETrue;

	TInt iniNumberOfCitiesInGroup;
	GetIntFromConfig(aGroupSectName,KIniNumberOfCitiesTag,iniNumberOfCitiesInGroup);

	// check number of cities is correct
	if(aCityArray.Count() != iniNumberOfCitiesInGroup)
		{
		result = EFalse;
		return result;
		}

	TInt iniGroupId;
	GetIntFromConfig(aGroupSectName,KIniCityGroupIdTag,iniGroupId);

	// check all cities have the correct group id
	TInt count = aCityArray.Count();
	for(TInt i=0; i<count; ++i)
		{
		if(aCityArray.At(i).GroupId() != iniGroupId)
			{
			result = EFalse;
			}
		}

	return result;
	}

/**
Checks the specified cached zone contains the correct information.

@param aFrequentlyUsedZone The time zone to check.

@param aCityNumber The index of the city that should be contained in the cached time zone.  Starting from 1.

@param aTimeZoneSectName The ini file section containing the time zone

@return ETrue if the cached zone contains the correct information, EFalse otherwise.

@internalTechnology
*/
TBool CTzLocalizationTestStep::CheckFrequentlyUsedZoneL(
		const CTzLocalizedTimeZone::TTzFrequentlyUsedZone aFrequentlyUsedZone,
		const TDesC& aTimeZoneSectName,
		const TInt aCityNumber)
	{
	CTzLocalizedTimeZone* timeZone = iLocalizer->GetFrequentlyUsedZoneL(aFrequentlyUsedZone);
	CleanupStack::PushL(timeZone);

	// check zone contains correct info
	TBool result = CompareTimeZoneWithIniFileSection(*timeZone,aTimeZoneSectName);

	// check that the cached zone's city was set to the correct city
	CTzLocalizedCity* city = iLocalizer->GetFrequentlyUsedZoneCityL(aFrequentlyUsedZone);
	if(!CompareCityWithIniFileSection(*city,aTimeZoneSectName,aCityNumber))
		{
		result = EFalse;
		}

	delete city;
	CleanupStack::PopAndDestroy(timeZone);

	return result;
	}

void CTzLocalizationTestStep::PrintSortOrder(CTzLocalizer::TTzSortOrder aSortOrder)
	{
	switch(aSortOrder)
		{
	case CTzLocalizer::ETzUnsorted:
		INFO_PRINTF1(KUnsorted);
		break;
	case CTzLocalizer::ETzUTCAscending:
		INFO_PRINTF1(KUTCAscending);
		break;
	case CTzLocalizer::ETzUTCDescending:
		INFO_PRINTF1(KUTCDescending);
		break;
	case CTzLocalizer::ETzAlphaNameAscending:
		INFO_PRINTF1(KAlphaNameAscending);
		break;
	case CTzLocalizer::ETzAlphaNameDescending:
		INFO_PRINTF1(KAlphaNameDescending);
		break;
	case CTzLocalizer::ETzAlphaStandardNameAscending:
		INFO_PRINTF1(KAlphaStandardNameAscending);
		break;
	case CTzLocalizer::ETzAlphaDaylightNameAscending:
		INFO_PRINTF1(KAlphaDaylightNameAscending);
		break;
	case CTzLocalizer::ETzAlphaShortStandardNameAscending:
		INFO_PRINTF1(KAlphaShortStandardNameAscending);
		break;
	case CTzLocalizer::ETzAlphaShortDaylightNameAscending:
		INFO_PRINTF1(KAlphaShortDaylightNameAscending);
		break;
	case CTzLocalizer::ETzAlphaStandardNameDescending:
		INFO_PRINTF1(KAlphaStandardNameDescending);
		break;
	case CTzLocalizer::ETzAlphaDaylightNameDescending:
		INFO_PRINTF1(KAlphaDaylightNameDescending);
		break;
	case CTzLocalizer::ETzAlphaShortStandardNameDescending:
		INFO_PRINTF1(KAlphaShortStandardNameDescending);
		break;
	case CTzLocalizer::ETzAlphaShortDaylightNameDescending:
		INFO_PRINTF1(KAlphaShortDaylightNameDescending);
		break;
		}
	}

/**
Convert an ini file tag to a TTzSortOrder

@param aIniTag The ini file tag to convert

@return The TTzSortOrder corresponding to the aIniTag

@internalTechnology
*/
CTzLocalizer::TTzSortOrder CTzLocalizationTestStep::GetSortOrder(TDesC& aIniTag)
	{
	CTzLocalizer::TTzSortOrder retval(CTzLocalizer::ETzUnsorted);
	if(aIniTag == KIniSortStepTypeUTCAOptionTag)
		{
		retval = CTzLocalizer::ETzUTCAscending;
		}
	else if(aIniTag == KIniSortStepTypeUTCDOptionTag)
		{
		retval = CTzLocalizer::ETzUTCDescending;
		}
	else if(aIniTag == KIniSortStepTypeAlphaAOptionTag)
		{
		retval = CTzLocalizer::ETzAlphaNameAscending;
		}
	else if(aIniTag == KIniSortStepTypeAlphaDOptionTag)
		{
		retval = CTzLocalizer::ETzAlphaNameDescending;
		}
	else if(aIniTag == KIniSortStepTypeStandardAOptionTag)
		{
		retval = CTzLocalizer::ETzAlphaStandardNameAscending;
		}
	else if(aIniTag == KIniSortStepTypeStandardDOptionTag)
		{
		retval = CTzLocalizer::ETzAlphaStandardNameDescending;
		}
	else if(aIniTag == KIniSortStepTypeDaylightAOptionTag)
		{
		retval = CTzLocalizer::ETzAlphaDaylightNameAscending;
		}
	else if(aIniTag == KIniSortStepTypeDaylightDOptionTag)
		{
		retval = CTzLocalizer::ETzAlphaDaylightNameDescending;
		}
	else if(aIniTag == KIniSortStepTypeShortStandardAOptionTag)
		{
		retval = CTzLocalizer::ETzAlphaShortStandardNameAscending;
		}
	else if(aIniTag == KIniSortStepTypeShortStandardDOptionTag)
		{
		retval = CTzLocalizer::ETzAlphaShortStandardNameDescending;
		}
	else if(aIniTag == KIniSortStepTypeShortDaylightAOptionTag)
		{
		retval = CTzLocalizer::ETzAlphaShortDaylightNameAscending;
		}
	else if(aIniTag == KIniSortStepTypeShortDaylightDOptionTag)
		{
		retval = CTzLocalizer::ETzAlphaShortDaylightNameDescending;
		}

	return retval;
	}

/**
Retrieves the integer time zone ID of the current time zone from the time zone
server (app-services/Tz).
@return The integer time zone ID according to the time zone server.
*/
TInt CTzLocalizationTestStep::GetTimeZoneIdFromTzServerL()
	{
	//Connect to time zone server
	RTz tz;
	User::LeaveIfError(tz.Connect());
	CleanupClosePushL(tz);

	// Get current time zone using the current CTzId from the time zone server
	CTzId* currentCTzId = tz.GetTimeZoneIdL();
	CleanupStack::PushL(currentCTzId);
	
	TInt timeZoneIdInt = currentCTzId->TimeZoneNumericID();
	
	CleanupStack::PopAndDestroy(currentCTzId);
	CleanupStack::PopAndDestroy(); //tz
	
	return timeZoneIdInt;
	}
	
//=============================================================================
// CTzInvalidTimeZoneDbTestStep
//=============================================================================
CTzInvalidTimeZoneDbTestStep::~CTzInvalidTimeZoneDbTestStep()
/**
 * Destructor
 */
	{
	}

CTzInvalidTimeZoneDbTestStep::CTzInvalidTimeZoneDbTestStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTzInvalidTimeZoneDbTestStep);
	
	}

TVerdict CTzInvalidTimeZoneDbTestStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Demonstrates Detecting a corrupt TZ database and recreate a new one.
 */
	{
	// Use logging macro
	INFO_PRINTF1(KInTestStep);

	if(!DetectCorruptTimeZoneDbTestL())
		{
		SetTestStepResult(EFail);
		}

	return TestStepResult();
	}
TVerdict CTzInvalidTimeZoneDbTestStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	RDebug::Print(TestStepName());
	INFO_PRINTF1(TestStepName());
	INFO_PRINTF1(KPreAmble);

	//Check if the bad database file to be tested has already existed on the disk
	_LIT(KBADDbFileName, "c:\\TzLocalizationTestScripts\\DBS_10206A8B_TzLocalization_bad.db");
	// Connect session
	RFs fsSession;
	CleanupClosePushL(fsSession);
	User::LeaveIfError(fsSession.Connect());
		
	RFile file;
	CleanupClosePushL(file);
	User::LeaveIfError(file.Open(fsSession, KBADDbFileName, EFileRead));
	CleanupStack::PopAndDestroy(2,&fsSession);
 	
	iScheduler = new CActiveScheduler();
	CActiveScheduler::Install(iScheduler);

	
	SetTestStepResult(EPass);
	return TestStepResult();
	}

TVerdict CTzInvalidTimeZoneDbTestStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	INFO_PRINTF1(KPostAmble);

	delete iScheduler;
	iScheduler = NULL;
	CActiveScheduler::Install(NULL);

	return TestStepResult();
	}
		
// Sort and print out all time zones
 void CTzInvalidTimeZoneDbTestStep::GetAllTimeZonesL()
	{
	// Use standard names in alphabetical order
	CTzLocalizedTimeZoneArray* timeZones = iLocalizer->GetAllTimeZonesL(CTzLocalizer::ETzAlphaStandardNameAscending);

	TInt count= timeZones->Count();
	_LIT(KConsoleMessage,"\nAll timezones:");	
	INFO_PRINTF1(KConsoleMessage);	
	for(TInt i=0; i<count; ++i)
		{
		TPtrC temp(timeZones->At(i).StandardName());
		// if the time zone has a standard name, print it and the short name
		if ((&temp)->Length()!=0)
			{
			INFO_PRINTF1(_L("\n"));
			INFO_PRINTF1(temp);
			_LIT(KConsoleTab,"\t");
			INFO_PRINTF1(KConsoleTab);
			INFO_PRINTF1(timeZones->At(i).ShortStandardName());
			}
		} 
		
	delete timeZones;
	}

// Add a city to a specific time zone and city group
 void CTzInvalidTimeZoneDbTestStep::AddCityL()
	{
	// Add the new city to the same time zone and city group as London
	_LIT(KCityName,"London");
	TBufC<KMaxCityBufLength> cityBuf(KCityName);
	TPtrC cityName(cityBuf);
	// First, find the time zone and city group that London is in 
	CTzLocalizedCity* city=iLocalizer->FindCityByNameL(cityName);
	if (!city)
	// city doesn't exist
		User::Leave(KErrNotFound);
	
	// Get the city's time zone ID and city group ID
	timeZoneId = city->TimeZoneId();
	cityGroupId = city->GroupId();
	delete city;

	// Now add the new city to that time zone/city group.
	// First check it is not already present
	CTzLocalizedCityArray* cities=iLocalizer->GetCitiesL(timeZoneId);
	CleanupStack::PushL(cities);
	_LIT(KNewCityName,"Cambridge");
	TBufC<KMaxCityBufLength> newCityBuf(KNewCityName); 
	TPtrC newCityName(newCityBuf);

	CTzLocalizedCity* temp = NULL;
	// (AddCityL() leaves if the city has already been added, so trap the leave)
	TRAPD(err,temp=iLocalizer->AddCityL(timeZoneId,newCityName,cityGroupId));
	// Ignore return value
	if (err==KErrNone)
			delete temp;
	CleanupStack::PopAndDestroy(cities);
	}
	
// print out all the cities in the time zone
void CTzInvalidTimeZoneDbTestStep::GetCitiesInTimeZoneL()
	{
	CTzLocalizedCityArray* tzCities=iLocalizer->GetCitiesL(timeZoneId);
	CleanupStack::PushL(tzCities);
	// get the standard name of the time zone and print it
	CTzLocalizedTimeZone* tz = iLocalizer->GetLocalizedTimeZoneL(timeZoneId);
	_LIT(KConsoleMessage,"\nThe cities in the %S time zone are :");
	TPtrC tzName = tz->StandardName();
	INFO_PRINTF2(KConsoleMessage,&tzName);
	delete tz;
	
	TInt tzCount=tzCities->Count();
	for(TInt i=0; i<tzCount; ++i)
		{
		INFO_PRINTF1(_L("\n"));
		INFO_PRINTF1(tzCities->At(i).Name());
		}

	CleanupStack::PopAndDestroy(tzCities);
	}
	
// print out all the cities in the city group
void CTzInvalidTimeZoneDbTestStep::GetCitiesInGroupL()
	{
	CTzLocalizedCityArray* groupCities = iLocalizer->GetCitiesInGroupL(cityGroupId);
	CleanupStack::PushL(groupCities);
	
	// get the city group name
	CTzLocalizedCityGroup* cityGroup=iLocalizer->GetCityGroupL(cityGroupId);	
	_LIT(KConsoleMessage,"\nThe cities in the %S city group are :");	
	TPtrC groupName = cityGroup->Name();
	INFO_PRINTF2(KConsoleMessage,&groupName);
	delete cityGroup;
	
	TInt groupCount=groupCities->Count();
	for(TInt i=0; i<groupCount; ++i)
			{
			INFO_PRINTF1(_L("\n"));
			INFO_PRINTF1(groupCities->At(i).Name());
			}
	CleanupStack::PopAndDestroy(groupCities); 
	}
	
// Retrieve a list of time zones and get information about a particular time zone using 
// the TimeZone Localization API. It adds a city to a time zone/city group.
void CTzInvalidTimeZoneDbTestStep::LocalizeTimeZonesL()
	{
	// Create an instance of CTzLocalizer
	iLocalizer = CTzLocalizer::NewL();
	CleanupStack::PushL(iLocalizer);
	
	GetAllTimeZonesL(); 
	AddCityL();
	GetCitiesInTimeZoneL();
	GetCitiesInGroupL();
	
	CleanupStack::PopAndDestroy(iLocalizer);
	}	
/**
@return ETrue if test passes, false if it fails

@internalTechnology
* Detect corrupt or invalid timezone database and if so, recreate a new one.
* Test the new created database by retrieving the timezone information from
* the database and adding a new city.
*/	
TBool CTzInvalidTimeZoneDbTestStep::DetectCorruptTimeZoneDbTestL()
	{
    
	INFO_PRINTF1(KBootWithCorruptTimeZoneDbTest);

		
	//Now retrieve the timezone information from the database and add a city.
	TBool result=ETrue;
	TRAPD(err,LocalizeTimeZonesL());
	if (err)
		{
		_LIT(KFailed,"\n\nTzBootWithInvalidTimeZoneTestStep failed: leave code=%d");
		INFO_PRINTF2(KFailed, err);
		result=EFalse;
		}
	
		
	return result;		
	}

//=============================================================================
// CTzPassiveAllTimeZonesTestStep
//=============================================================================

CTzPassiveAllTimeZonesTestStep::~CTzPassiveAllTimeZonesTestStep()
/**
 * Destructor
 */
	{
	}

CTzPassiveAllTimeZonesTestStep::CTzPassiveAllTimeZonesTestStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTzPassiveAllTimeZonesTestStep);
	}

TVerdict CTzPassiveAllTimeZonesTestStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Demonstrates reading configuration parameters fom an ini file section
 */
	{
	// Use logging macro
	INFO_PRINTF1(KInTestStep);

	if(!GetAllTimeZonesTestL())
		{
		SetTestStepResult(EFail);
		}

	return TestStepResult();
	}

/**
Perform test of CTzLocalizer::GetAllTimeZonesL()

@return ETrue if test passes, false if it fails

@internalTechnology
*/
TBool CTzPassiveAllTimeZonesTestStep::GetAllTimeZonesTestL()
	{
	TBool result = ETrue;

	INFO_PRINTF1(KGetAllTimeZonesTest);

	// Testing this call in this step ====================================
	CTzLocalizedTimeZoneArray* timeZones = iLocalizer->GetAllTimeZonesL();
	// ===================================================================
	CleanupStack::PushL(timeZones);

	// Get the number of expected timezones from the ini file
	TInt numberOfTimeZones;
	GetIntFromConfig(ConfigSection(),KIniNumberOfTimeZonesTag,numberOfTimeZones);

	_LIT(KSXint,"Tz timeZones count= %d whilst expecting %d from ini");
	TInt count = timeZones->Count();
	if(count != numberOfTimeZones)
		{
       	INFO_PRINTF3(KSXint, count, numberOfTimeZones );
		result = EFalse;
		PrintResult(result);
		return result;
		}

	// Assign a descriptor to allow us to loop through the ini file sections
	TBuf<KIniTagLength> timeZoneSection(KIniTimeZoneTag);

	for(TInt i=0; i<count; ++i)
		{
		timeZoneSection = KIniTimeZoneTag;
		timeZoneSection.AppendNum(i+1);

		// compare the current element with the curent ini file section
		if(!CompareTimeZoneWithIniFileSection(timeZones->At(i),timeZoneSection))
			{
			// only change if comparison failed, so that result doesn't get set back to ETrue
			result = EFalse;
			}
		}

	CleanupStack::PopAndDestroy(timeZones);

	PrintResult(result);
	return result;
	}

//=============================================================================
// CTzActiveTimeZoneTestStep
//=============================================================================

CTzActiveTimeZoneTestStep::~CTzActiveTimeZoneTestStep()
/**
 * Destructor
 */
	{
	}

CTzActiveTimeZoneTestStep::CTzActiveTimeZoneTestStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTzActiveTimeZoneTestStep);
	}

TVerdict CTzActiveTimeZoneTestStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Demonstrates reading configuration parameters fom an ini file section
 */
	{
	INFO_PRINTF1(KInTestStep);

	if(!SetAndGetFrequentlyUsedZoneFromTimeZoneIdL())
		{
		SetTestStepResult(EFail);
		}
	if(!SetAndGetFrequentlyUsedZoneFromTimeZoneL())
		{
		SetTestStepResult(EFail);
		}
	if(!SetAndGetFrequentlyUsedZoneFromCityL())
		{
		SetTestStepResult(EFail);
		}

	return TestStepResult();
	}

TBool CTzActiveTimeZoneTestStep::SetAndGetFrequentlyUsedZoneFromTimeZoneIdL()
	{
	INFO_PRINTF1(KSetAndGetFrequentlyUsedZoneFromTimeZoneId);

	TBool result = ETrue;

	// get the zone id from the ini file section
	TInt iniTimeZoneId;
	GetIntFromConfig(ConfigSection(),KIniTimeZoneIdTag,iniTimeZoneId);

	// set and get home zone

	// Testing this call in this step ========================================
	iLocalizer->SetFrequentlyUsedZoneL(iniTimeZoneId,CTzLocalizedTimeZone::EHomeZone);
	// =======================================================================

	// check newly-set home zone
	result = CheckFrequentlyUsedZoneL(CTzLocalizedTimeZone::EHomeZone,ConfigSection(),1);

	// set and get zone of interest

	// Testing this call in this step ============================================
	iLocalizer->SetFrequentlyUsedZoneL(iniTimeZoneId,CTzLocalizedTimeZone::EInterestZone);
	// ===========================================================================

	// check zone of interest
	if(!CheckFrequentlyUsedZoneL(CTzLocalizedTimeZone::EInterestZone,ConfigSection(),1))
		{
		result = EFalse;
		}

	// call set on home and zone of interest again, to get the recent zones set
	iLocalizer->SetFrequentlyUsedZoneL(iniTimeZoneId,CTzLocalizedTimeZone::EHomeZone);
	iLocalizer->SetFrequentlyUsedZoneL(iniTimeZoneId,CTzLocalizedTimeZone::EInterestZone);

	// check recent zone 1 - should now be set
	if(!CheckFrequentlyUsedZoneL(CTzLocalizedTimeZone::ERecentZone1,ConfigSection(),1))
		{
		result = EFalse;
		}

	// check recent zone 2 - should now be set
	if(!CheckFrequentlyUsedZoneL(CTzLocalizedTimeZone::ERecentZone2,ConfigSection(),1))
		{
		result = EFalse;
		}
		
  	// set and get current zone
  	
  	// Testing this call in this step ============================================
  	iLocalizer->SetFrequentlyUsedZoneL(iniTimeZoneId,CTzLocalizedTimeZone::ECurrentZone);
  	// ===========================================================================
  
  	// check current zone
  	if(!CheckFrequentlyUsedZoneL(CTzLocalizedTimeZone::ECurrentZone,ConfigSection(),1) 
  		|| GetTimeZoneIdFromTzServerL() != iniTimeZoneId)
  		{
  		result = EFalse;
  		}

	PrintResult(result);
	return result;
	}

TBool CTzActiveTimeZoneTestStep::SetAndGetFrequentlyUsedZoneFromTimeZoneL()
	{
	INFO_PRINTF1(KSetAndGetFrequentlyUsedZoneFromTimeZone);

	TBool result = ETrue;

	// get the zone id from the ini file section
	TInt iniTimeZoneId;
	GetIntFromConfig(ConfigSection(),KIniTimeZoneIdTag,iniTimeZoneId);

	// create a time zone object
	CTzLocalizedTimeZone* timeZoneFromId = iLocalizer->GetLocalizedTimeZoneL(iniTimeZoneId);
	CleanupStack::PushL(timeZoneFromId);

	// use time zone object to set the home zone

	// Testing this call in this step ==========================================
	iLocalizer->SetFrequentlyUsedZoneL(*timeZoneFromId,CTzLocalizedTimeZone::EHomeZone);
	// =========================================================================

	// check newly-set zone
	result = CheckFrequentlyUsedZoneL(CTzLocalizedTimeZone::EHomeZone,ConfigSection(),1);

	// use time zone object to set the zone of interest

	// Testing this call in this step ==============================================
	iLocalizer->SetFrequentlyUsedZoneL(*timeZoneFromId,CTzLocalizedTimeZone::EInterestZone);
	// =============================================================================

	// check newly-set zone
	if(!CheckFrequentlyUsedZoneL(CTzLocalizedTimeZone::EInterestZone,ConfigSection(),1))
		{
		result = EFalse;
		}

	// call set on home and zone of interest again, to get the recent zones set
	iLocalizer->SetFrequentlyUsedZoneL(*timeZoneFromId,CTzLocalizedTimeZone::EHomeZone);
	iLocalizer->SetFrequentlyUsedZoneL(*timeZoneFromId,CTzLocalizedTimeZone::EInterestZone);

	// check recent zone 1 - should now be set
	if(!CheckFrequentlyUsedZoneL(CTzLocalizedTimeZone::ERecentZone1,ConfigSection(),1))
		{
		result = EFalse;
		}

	// check recent zone 2 - should now be set
	if(!CheckFrequentlyUsedZoneL(CTzLocalizedTimeZone::ERecentZone2,ConfigSection(),1))
		{
		result = EFalse;
		}

	// set and get current zone
	
	// Testing this call in this step ============================================
	iLocalizer->SetFrequentlyUsedZoneL(*timeZoneFromId,CTzLocalizedTimeZone::ECurrentZone);
	// ===========================================================================

	// check current zone
	if(!CheckFrequentlyUsedZoneL(CTzLocalizedTimeZone::ECurrentZone,ConfigSection(),1) 
		|| GetTimeZoneIdFromTzServerL() != timeZoneFromId->TimeZoneId())
		{
		result = EFalse;
		}

	CleanupStack::PopAndDestroy(timeZoneFromId);

	PrintResult(result);
	return result;
	}

TBool CTzActiveTimeZoneTestStep::SetAndGetFrequentlyUsedZoneFromCityL()
	{
	INFO_PRINTF1(KSetAndGetFrequentlyUsedZoneFromCity);

	TBool result = ETrue;

	// get the zone id from the ini file section
	TInt iniTimeZoneId;
	GetIntFromConfig(ConfigSection(),KIniTimeZoneIdTag,iniTimeZoneId);

	// get all the cities in the time zone
	CTzLocalizedCityArray* citiesInZone = iLocalizer->GetCitiesL(iniTimeZoneId);
	CleanupStack::PushL(citiesInZone);

	// Use the last city in the array to set the home zone
	TInt numberOfCities = citiesInZone->Count();

	// Testing this call in this step =============================================================
	iLocalizer->SetFrequentlyUsedZoneL(citiesInZone->At(numberOfCities-1),CTzLocalizedTimeZone::EHomeZone);
	// ============================================================================================

	// check that it's set correctly
	result = CheckFrequentlyUsedZoneL(CTzLocalizedTimeZone::EHomeZone,ConfigSection(),numberOfCities);

	// Set the zone of interest

	// Testing this call in this step =================================================================
	iLocalizer->SetFrequentlyUsedZoneL(citiesInZone->At(numberOfCities-1),CTzLocalizedTimeZone::EInterestZone);
	// ================================================================================================

	// check it
	if(!CheckFrequentlyUsedZoneL(CTzLocalizedTimeZone::EInterestZone,ConfigSection(),numberOfCities))
		{
		result = EFalse;
		}

	// call set on home and zone of interest again, to get the recent zones set
	iLocalizer->SetFrequentlyUsedZoneL(citiesInZone->At(numberOfCities-1),CTzLocalizedTimeZone::EHomeZone);
	iLocalizer->SetFrequentlyUsedZoneL(citiesInZone->At(numberOfCities-1),CTzLocalizedTimeZone::EInterestZone);

	// check recent zone 1 - should now be set
	if(!CheckFrequentlyUsedZoneL(CTzLocalizedTimeZone::ERecentZone1,ConfigSection(),numberOfCities))
		{
		result = EFalse;
		}

	// check recent zone 2 - should now be set
	if(!CheckFrequentlyUsedZoneL(CTzLocalizedTimeZone::ERecentZone2,ConfigSection(),numberOfCities))
		{
		result = EFalse;
		}

	CleanupStack::PopAndDestroy(citiesInZone);

	PrintResult(result);
	return result;
	}


//=============================================================================
// CTzPassiveCityTestStep
//=============================================================================

CTzPassiveCityTestStep::~CTzPassiveCityTestStep()
/**
 * Destructor
 */
	{
	}

CTzPassiveCityTestStep::CTzPassiveCityTestStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTzPassiveCityTestStep);
	}

TVerdict CTzPassiveCityTestStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Demonstrates reading configuration parameters fom an ini file section
 */
	{
	INFO_PRINTF1(KInTestStep);

	if(!GetDefaultCityFromTimeZoneIdTestL())
		{
		SetTestStepResult(EFail);
		}
	if(!GetDefaultCityFromAnotherCityTestL())
		{
		SetTestStepResult(EFail);
		}
	if(!GetDefaultCityFromTimeZoneTestL())
		{
		SetTestStepResult(EFail);
		}
	if(!GetCitiesFromTimeZoneIdTestL())
		{
		SetTestStepResult(EFail);
		}
	if(!GetCitiesFromTimeZoneTestL())
		{
		SetTestStepResult(EFail);
		}
	if(!GetCitiesFromAnotherCityTestL())
		{
		SetTestStepResult(EFail);
		}

	return TestStepResult();
	}

TBool CTzPassiveCityTestStep::GetDefaultCityFromTimeZoneIdTestL()
	{
	TBool result = ETrue;
	INFO_PRINTF1(KGetDefaultCityFromTimeZoneIdTest);

	TInt iniTimeZoneId;
	GetIntFromConfig(ConfigSection(),KIniTimeZoneIdTag,iniTimeZoneId);

	// Testing this call in this step =========================================
	CTzLocalizedCity* defaultCity = iLocalizer->GetDefaultCityL(iniTimeZoneId);
	// ========================================================================

	CleanupStack::PushL(defaultCity);

	// check the city
	result = CompareCityWithIniFileSection(*defaultCity,ConfigSection(),1);

	CleanupStack::PopAndDestroy(defaultCity);

	PrintResult(result);
	return result;
	}

TBool CTzPassiveCityTestStep::GetDefaultCityFromAnotherCityTestL()
	{
	TBool result = ETrue;
	INFO_PRINTF1(KGetDefaultCityFromAnotherCityTest);

	TInt iniTimeZoneId;
	GetIntFromConfig(ConfigSection(),KIniTimeZoneIdTag,iniTimeZoneId);

	// get the cites from the time zone - tested later
	CTzLocalizedCityArray* cities = iLocalizer->GetCitiesL(iniTimeZoneId);
	CleanupStack::PushL(cities);

	// use the last city in the array to get the default city
	TInt lastCityIndex = cities->Count()-1;

	// Testing this call in this step =====================================================
	CTzLocalizedCity* defaultCity = iLocalizer->GetDefaultCityL(cities->At(lastCityIndex));
	// ====================================================================================

	CleanupStack::PushL(defaultCity);

	result = CompareCityWithIniFileSection(*defaultCity,ConfigSection(),1);

	CleanupStack::PopAndDestroy(defaultCity);
	CleanupStack::PopAndDestroy(cities);

	PrintResult(result);
	return result;
	}

TBool CTzPassiveCityTestStep::GetDefaultCityFromTimeZoneTestL()
	{
	TBool result = ETrue;
	INFO_PRINTF1(KGetDefaultCityFromTimeZoneTest);

	TInt iniTimeZoneId;
	GetIntFromConfig(ConfigSection(),KIniTimeZoneIdTag,iniTimeZoneId);

	CTzLocalizedTimeZone* timeZone = iLocalizer->GetLocalizedTimeZoneL(iniTimeZoneId);
	CleanupStack::PushL(timeZone);

	// Testing this call in this step =====================================
	CTzLocalizedCity* defaultCity = iLocalizer->GetDefaultCityL(*timeZone);
	// ====================================================================

	CleanupStack::PushL(defaultCity);

	result = CompareCityWithIniFileSection(*defaultCity,ConfigSection(),1);

	CleanupStack::PopAndDestroy(defaultCity);
	CleanupStack::PopAndDestroy(timeZone);

	PrintResult(result);
	return result;
	}

TBool CTzPassiveCityTestStep::GetCitiesFromTimeZoneIdTestL()
	{
	TBool result = ETrue;
	INFO_PRINTF1(KGetCitiesFromTimeZoneIdTest);

	TInt iniTimeZoneId;
	GetIntFromConfig(ConfigSection(),KIniTimeZoneIdTag,iniTimeZoneId);

	// get the cites from the time zone, using time zone id

	// Testing this call in this step ====================================
	CTzLocalizedCityArray* cities = iLocalizer->GetCitiesL(iniTimeZoneId);
	// ===================================================================

	CleanupStack::PushL(cities);

	// test number of cities
	TInt iniNumberOfCities;
	GetIntFromConfig(ConfigSection(),KIniNumberOfCitiesTag,iniNumberOfCities);
	_LIT(KSXint,"Tz cities count= %d whilst expecting %d  ");
	TInt count = cities->Count();
	if(cities->Count() != iniNumberOfCities)
		{
       	INFO_PRINTF3(KSXint, count, iniNumberOfCities );
		_LIT(KSXTZiD,"With Tz id = %d ");
		ERR_PRINTF2( KSXTZiD, iniTimeZoneId );
		result = EFalse;
		PrintResult(result);
		return result;
		}

	// test contents of all cities
	for(TInt i=0; i<count; ++i)
		{
		if(!CompareCityWithIniFileSection(cities->At(i),ConfigSection(),i+1))
			{
	    	result = EFalse;
			}
		}

	CleanupStack::PopAndDestroy(cities);

	PrintResult(result);
	return result;
	}


TBool CTzPassiveCityTestStep::GetCitiesFromTimeZoneTestL()
	{
	TBool result = ETrue;
	INFO_PRINTF1(KGetCitiesFromTimeZoneTest);

	TInt iniTimeZoneId;
	GetIntFromConfig(ConfigSection(),KIniTimeZoneIdTag,iniTimeZoneId);

	CTzLocalizedTimeZone* timeZone = iLocalizer->GetLocalizedTimeZoneL(iniTimeZoneId);
	CleanupStack::PushL(timeZone);

	// get the cites from the time zone, using time zone object

	// Testing this call in this step ================================
	CTzLocalizedCityArray* cities = iLocalizer->GetCitiesL(*timeZone);
	// ===============================================================

	CleanupStack::PushL(cities);

	// test number of cities
	TInt iniNumberOfCities;
	GetIntFromConfig(ConfigSection(),KIniNumberOfCitiesTag,iniNumberOfCities);
	TInt count = cities->Count();
	if(count != iniNumberOfCities)
		{
		_LIT(KSXint,"Tz cities count= %d whilst expecting %d  ");
       	INFO_PRINTF3(KSXint, count, iniNumberOfCities );
       	result = EFalse;
		PrintResult(result);
		return result;
		}

	// test contents of all cities
	for(TInt i=0; i<count; ++i)
		{
		if(!CompareCityWithIniFileSection(cities->At(i),ConfigSection(),i+1))
			{
			result = EFalse;
			}
		}

	CleanupStack::PopAndDestroy(cities);
	CleanupStack::PopAndDestroy(timeZone);

	PrintResult(result);
	return result;
	}

TBool CTzPassiveCityTestStep::GetCitiesFromAnotherCityTestL()
	{
	TBool result = ETrue;
	INFO_PRINTF1(KGetCitiesFromAnotherCityTest);

	TInt iniTimeZoneId;
	GetIntFromConfig(ConfigSection(),KIniTimeZoneIdTag,iniTimeZoneId);

	// get the default city from a timezone id
	CTzLocalizedCity* defaultCity = iLocalizer->GetDefaultCityL(iniTimeZoneId);
	CleanupStack::PushL(defaultCity);

	// get the cites using the default city

	// Testing this call in this step ===================================
	CTzLocalizedCityArray* cities = iLocalizer->GetCitiesL(*defaultCity);
	// ==================================================================

	CleanupStack::PushL(cities);

	// test number of cities
	TInt iniNumberOfCities;
	GetIntFromConfig(ConfigSection(),KIniNumberOfCitiesTag,iniNumberOfCities);
	if(cities->Count() != iniNumberOfCities)
		{
		result = EFalse;
		PrintResult(result);
		return result;
		}

	// test contents of all cities
	TInt count = cities->Count();
	for(TInt i=0; i<count; ++i)
		{
		if(!CompareCityWithIniFileSection(cities->At(i),ConfigSection(),i+1))
			{
			result = EFalse;
			}
		}

	CleanupStack::PopAndDestroy(cities);
	CleanupStack::PopAndDestroy(defaultCity);

	PrintResult(result);
	return result;
	}

//=============================================================================
// CTzPassiveAllCitiesTestStep
//=============================================================================

CTzPassiveAllCitiesTestStep::~CTzPassiveAllCitiesTestStep()
/**
 * Destructor
 */
	{
	}

CTzPassiveAllCitiesTestStep::CTzPassiveAllCitiesTestStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTzPassiveAllCitiesTestStep);
	}

TVerdict CTzPassiveAllCitiesTestStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Demonstrates reading configuration parameters fom an ini file section
 */
	{
	INFO_PRINTF1(KInTestStep);

	if(!GetAllCitiesTestL())
		{
		SetTestStepResult(EFail);
		}

	return TestStepResult();
	}


TBool CTzPassiveAllCitiesTestStep::GetAllCitiesTestL()
	{
	TBool result = ETrue;
	INFO_PRINTF1(KGetAllCitiesTest);

	// Testing this call in this step ==========================
	CTzLocalizedCityArray* allCities = iLocalizer->GetCitiesL();
	// =========================================================

	CleanupStack::PushL(allCities);

	// Check that the number of cities is correct
	TInt iniNumberOfCities;
	GetIntFromConfig(ConfigSection(),KIniNumberOfCitiesTag,iniNumberOfCities);
	if(allCities->Count() != iniNumberOfCities)
		{
		_LIT(KSXint,"GetAllCitiesTestL Got allCities->Count= %d - but in inifile expected = %d ");
     	INFO_PRINTF3(KSXint, allCities->Count(), iniNumberOfCities );
		result = EFalse;
		PrintResult(result);
		return result;
		}


	// Prepare variables for storing ini file info on a timezone
	TBuf<KIniTagLength> iniTimeZone;
	TInt iniNumberOfCitiesInTimeZone;

	// Prepare an int for looping through the city array, allCities
	TInt k=0;

	// Loop through all time zones
	TInt iniNumberOfTimeZones;
	GetIntFromConfig(KIniAllZonesTag,KIniNumberOfTimeZonesTag,iniNumberOfTimeZones);
	for(TInt i=0; i<iniNumberOfTimeZones; ++i)
		{
		// Form the name of the current ini file section name
		iniTimeZone = KIniTimeZoneTag;
		iniTimeZone.AppendNum(i+1);

		// Find number of cities in this time zone
		GetIntFromConfig(iniTimeZone,KIniNumberOfCitiesTag,iniNumberOfCitiesInTimeZone);
		// Loop through all cities in the zone 
		for(TInt j=0; j>iniNumberOfCitiesInTimeZone; ++j)
			{
			if(!CompareCityWithIniFileSection(allCities->At(k),iniTimeZone,j+1))
				{
				result = EFalse;
				}
			++k;
			}
		}

	CleanupStack::PopAndDestroy(allCities);

	PrintResult(result);
	return result;
	}


//=============================================================================
// CTzCitiesWithUTCOffsetTestStep
//=============================================================================
CTzCitiesWithUTCOffsetTestStep::~CTzCitiesWithUTCOffsetTestStep()
	{
	}

CTzCitiesWithUTCOffsetTestStep::CTzCitiesWithUTCOffsetTestStep()
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTzCitiesWithUTCOffsetTestStep);
	}

TVerdict CTzCitiesWithUTCOffsetTestStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Demonstrates reading configuration parameters fom an ini file section
 */
	{
	INFO_PRINTF1(KInTestStep);

	if(!GetAllCitiesWithUTCOffsetTestL())
		{
		SetTestStepResult(EFail);
		}

	return TestStepResult();
	}


TBool CTzCitiesWithUTCOffsetTestStep::GetAllCitiesWithUTCOffsetTestL()
	{
	TBool result = ETrue;
	INFO_PRINTF1(KGetCitiesWithUTCOffsetTest);

	// Get the offset from the ini file
	TInt utcOffset;
	GetIntFromConfig(ConfigSection(),KIniUTCTag,utcOffset);
	
	//Get all the cities with a matching UTC offset
	CTzLocalizedCityArray* cities = iLocalizer->GetCitiesWithUTCOffsetL(utcOffset,CTzLocalizer::ETzUnsorted);
	CleanupStack::PushL(cities);
	
	
	// buffer for current ini file tag to read
	TBuf<KIniTagLength> iniTag;

	TBuf<50> outputBuffer;
	INFO_PRINTF1(KExpected);

	// check that they are in the correct order
	TInt count = cities->Count();
	for(TInt i=0; i<count; ++i)
		{
		iniTag = KIniCityNameTag;
		iniTag.AppendNum(i+1);

		// get the city name that we are expecting
		TPtrC iniCityName;
		GetStringFromConfig(ConfigSection(),iniTag,iniCityName);

		// check that the current city has the expected name
		if(cities->At(i).Name() != iniCityName)
			{
			result = EFalse;
			}

		// print out expected and actual names
		outputBuffer = iniCityName;
		outputBuffer.Append(KTab);
		if ( (&iniCityName)->Length() <= 7)
			{
			outputBuffer.Append(KTab);
			}
		outputBuffer.Append(cities->At(i).Name());
		INFO_PRINTF1(outputBuffer);
		}

	CleanupStack::PopAndDestroy(cities);
	PrintResult(result);
	return result;
	}
//=============================================================================
// CTzTimeZonesWithUTCOffsetTestStep
//=============================================================================
CTzTimeZonesWithUTCOffsetTestStep::~CTzTimeZonesWithUTCOffsetTestStep()
	{
	}

CTzTimeZonesWithUTCOffsetTestStep::CTzTimeZonesWithUTCOffsetTestStep()
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTzTimeZonesWithUTCOffsetTestStep);
	}

TVerdict CTzTimeZonesWithUTCOffsetTestStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Demonstrates reading configuration parameters fom an ini file section
 */
	{
	INFO_PRINTF1(KInTestStep);

	if(!GetAllTimeZonesWithUTCOffsetTestL())
		{
		SetTestStepResult(EFail);
		}

	return TestStepResult();
	}


TBool CTzTimeZonesWithUTCOffsetTestStep::GetAllTimeZonesWithUTCOffsetTestL()
	{
	TBool result = ETrue;
	INFO_PRINTF1(KGetTimeZonesWithUTCOffsetTest);

	// Get the offset from the ini file
	TInt utcOffset;
	GetIntFromConfig(ConfigSection(),KIniUTCTag,utcOffset);
	
	//Get all the time zones with a matching UTC offset
	CTzLocalizedTimeZoneArray* zones = iLocalizer->GetTimeZonesWithUTCOffsetL(utcOffset,CTzLocalizer::ETzUnsorted);
	CleanupStack::PushL(zones);
	
	// buffer for current ini file tag to read
	TBuf<KIniTagLength> iniTag;

	TBuf<50> outputBuffer;
	INFO_PRINTF1(KExpected);

	// check that they are in the correct order
	TInt count = zones->Count();
	_LIT(KSXint,"Got zones->Count() %d ");
    INFO_PRINTF2(KSXint, count );
	for(TInt i=0; i<count; ++i)
		{
		iniTag = KIniStandardNameTag;
		iniTag.AppendNum(i+1);

		// get the zone name that we are expecting
		TPtrC iniZoneName;
		GetStringFromConfig(ConfigSection(),iniTag,iniZoneName);

		// check that the current city has the expected name
		if(zones->At(i).StandardName() != iniZoneName)
			{
			TPtrC temp( zones->At(i).StandardName() );
			if ( (&temp)->Length()!=0  )
	 			{
				_LIT(KSX1,"CTzTimeZonesWithUTCOffsetTestStep: got Tz StandardName %S, whilst in ini %S \t");
  				INFO_PRINTF3(KSX1, &temp, &iniZoneName);
				}
			result = EFalse;
			}

		// print out expected and actual names
		outputBuffer = iniZoneName;
		outputBuffer.Append(KTab);
		outputBuffer.Append(zones->At(i).StandardName());
		INFO_PRINTF1(outputBuffer);
		}

	CleanupStack::PopAndDestroy(zones);
	PrintResult(result);
	return result;
	}
//=============================================================================
// CTzActiveCityTestStep
//=============================================================================

CTzActiveCityTestStep::CTzActiveCityTestStep()
	{
	SetTestStepName(KTzActiveCityTestStep);
	}

CTzActiveCityTestStep::~CTzActiveCityTestStep()
	{
	}

TVerdict  CTzActiveCityTestStep::doTestStepL()
	{
	INFO_PRINTF1(KInTestStep);


	if(!AddRemoveCityTestL())
		{
		SetTestStepResult(EFail);
		}

	return TestStepResult();
	}

TBool CTzActiveCityTestStep::AddRemoveCityTestL()
	{
	TBool result = ETrue;
	INFO_PRINTF1(KAddRemoveCityTest);

	// Get city data from ini file
	TPtrC iniCityName;
	GetStringFromConfig(ConfigSection(),KIniCityNameTag,iniCityName);
	TInt iniTimeZoneId;
	GetIntFromConfig(ConfigSection(),KIniTimeZoneIdTag,iniTimeZoneId);
	TInt iniGroupId;
	GetIntFromConfig(ConfigSection(),KIniCityGroupIdTag,iniGroupId);

	CTzLocalizedCity* newCity = NULL;
	if(iniGroupId > 0)
		{
		// Testing this call in this step ===================================
		newCity = iLocalizer->AddCityL(iniTimeZoneId,iniCityName,iniGroupId);
		// ==================================================================
		CleanupStack::PushL(newCity);
		}
	else
		{
		// Testing this call in this step ========================
		newCity = iLocalizer->AddCityL(iniTimeZoneId,iniCityName);
		// =======================================================
		CleanupStack::PushL(newCity);
		}

	INFO_PRINTF1(KCityInfo);
	// check new city info
	if(newCity->Name() != iniCityName)
		{
		result = EFalse;
		}
	if(newCity->TimeZoneId() != iniTimeZoneId)
		{
		result = EFalse;
		}
	if(newCity->GroupId() != iniGroupId)
		{
		result = EFalse;
		}
	PrintResult(result);


	// check that the city is retrieved by various getcities overloads
	// All cities
	INFO_PRINTF1(KTestCityInAllCitiesArray);
	CTzLocalizedCityArray* cityArray = iLocalizer->GetCitiesL();
	if(!CityArrayContainsCity(cityArray,newCity))
		{
		result = EFalse;
		}
	delete cityArray;
	PrintResult(result);

	// Cities from time zone id
	INFO_PRINTF1(KTestCityInArrayFromTimeZoneId);
	cityArray = iLocalizer->GetCitiesL(newCity->TimeZoneId());
	if(!CityArrayContainsCity(cityArray,newCity))
		{
		result = EFalse;
		}
	delete cityArray;
	PrintResult(result);

	// Cities from another city - use default city for time zone
	INFO_PRINTF1(KTestCityInArrayFromDefaultCity);
	CTzLocalizedCity* defaultCity = iLocalizer->GetDefaultCityL(newCity->TimeZoneId());
	CleanupStack::PushL(defaultCity);
	cityArray = iLocalizer->GetCitiesL(*defaultCity);
	if(!CityArrayContainsCity(cityArray,newCity))
		{
		result = EFalse;
		}
	delete cityArray;
	CleanupStack::PopAndDestroy(defaultCity);
	PrintResult(result);

	// Cities from a time zone
	INFO_PRINTF1(KTestCityInArrayFromTzObject);
	CTzLocalizedTimeZone* timeZone = iLocalizer->GetLocalizedTimeZoneL(newCity->TimeZoneId());
	CleanupStack::PushL(timeZone);
	cityArray = iLocalizer->GetCitiesL(*timeZone);
	if(!CityArrayContainsCity(cityArray,newCity))
		{
		result = EFalse;
		}
	delete cityArray;
	CleanupStack::PopAndDestroy(timeZone);
	PrintResult(result);

	// Cities from new city
	INFO_PRINTF1(KTestCityInArrayFromNewCity);
	cityArray = iLocalizer->GetCitiesL(*newCity);
	if(!CityArrayContainsCity(cityArray,newCity))
		{
		result = EFalse;
		}
	delete cityArray;
	PrintResult(result);

	// Test remaining methods that take in a city object
	// Get time zone from new city
	INFO_PRINTF1(KTestTimezoneFromNewCity);
	timeZone = iLocalizer->GetLocalizedTimeZoneL(*newCity);
	if(timeZone->TimeZoneId() != newCity->TimeZoneId())
		{
		result = EFalse;
		}
	delete timeZone;
	PrintResult(result);

	// Get default city from new city
	INFO_PRINTF1(KTestDefaultCityFromNewCity);
	defaultCity = iLocalizer->GetDefaultCityL(*newCity);
	if(defaultCity->TimeZoneId() != newCity->TimeZoneId())
		{
		result = EFalse;
		}
	delete defaultCity;
	PrintResult(result);

	// Get Group using new city
	INFO_PRINTF1(KTestGroupFromNewCity);
	CTzLocalizedCityGroup* group;
	if(newCity->GroupId() > 0)
		{
		group = iLocalizer->GetCityGroupL(*newCity);
		if(group->Id() != newCity->GroupId())
			{
			return EFalse;
			}
		delete group;
		}
	PrintResult(result);

	// Get cities in group from new city
	INFO_PRINTF1(KTestCityArrayFromNewCity);
	cityArray = iLocalizer->GetCitiesInGroupL(*newCity);
	if(!CityArrayContainsCity(cityArray,newCity))
		{
		result = EFalse;
		}
	delete cityArray;
	PrintResult(result);

	// Check whether we should delete
	TBool remove;
	GetBoolFromConfig(ConfigSection(),KIniRemove,remove);

	if(remove)
		{
		// Test deleting the city
		INFO_PRINTF1(KTestDeleteNewCity);
		iLocalizer->RemoveCityL(newCity);
		// Check that the city is gone from the list of all cites
		cityArray = iLocalizer->GetCitiesL();
		TInt count = cityArray->Count();
		for(TInt i=0; i<count; ++i)
			{
			if(iniCityName == cityArray->At(i).Name())
				{
				result = EFalse;
				break;
				}
			}
		PrintResult(result);
		}

	CleanupStack::PopAndDestroy(newCity);

	return result;
	}

//=============================================================================
// CTzGroupTestStep
//=============================================================================
CTzGroupTestStep::CTzGroupTestStep()
	{
	SetTestStepName(KTzGroupTestStep);
	}

CTzGroupTestStep::~CTzGroupTestStep()
	{
	}

TVerdict CTzGroupTestStep::doTestStepL()
	{
	if(!GetCityGroupFromGroupIdTestL())
		{
		SetTestStepResult(EFail);
		}
	if(!GetCityGroupFromCityTestL())
		{
		SetTestStepResult(EFail);
		}
	if(!GetCitiesInGroupFromGroupIdTestL())
		{
		SetTestStepResult(EFail);
		}
	if(!GetCitiesInGroupFromCityTestL())
		{
		SetTestStepResult(EFail);
		}
	if(!GetCitiesInGroupFromGroupTestL())
		{
		SetTestStepResult(EFail);
		}

	return TestStepResult();
	}

TBool CTzGroupTestStep::GetCityGroupFromGroupIdTestL()
	{
	INFO_PRINTF1(KGetCityGroupFromGroupIdTest);
	TBool result = ETrue;

	// Get data from ini file section
	TInt iniGroupId;
	GetIntFromConfig(ConfigSection(),KIniGroupIdTag,iniGroupId);
	TPtrC iniGroupName;
	GetStringFromConfig(ConfigSection(),KIniGroupName,iniGroupName);

	// Testing this call in this step ===================================
	CTzLocalizedCityGroup* group = iLocalizer->GetCityGroupL(iniGroupId);
	// ==================================================================
	CleanupStack::PushL(group);
	if(group->Id() != iniGroupId)
		{
		result = EFalse;
		}
	if(group->Name() != iniGroupName)
		{
		result = EFalse;
		}

	CleanupStack::PopAndDestroy(group);

	PrintResult(result);
	return result;
	}

TBool CTzGroupTestStep::GetCityGroupFromCityTestL()
	{
	TBool result = ETrue;
	INFO_PRINTF1(KGetCityGroupFromCityTest);

	// Get data from ini file section
	TInt iniGroupId;
	GetIntFromConfig(ConfigSection(),KIniGroupIdTag,iniGroupId);
	TPtrC iniGroupName;
	GetStringFromConfig(ConfigSection(),KIniGroupName,iniGroupName);

	CTzLocalizedCityArray* cities = iLocalizer->GetCitiesInGroupL(iniGroupId);
	CleanupStack::PushL(cities);

	// test using the first city in the array

	// Testing this call in this step ======================================
	CTzLocalizedCityGroup* group = iLocalizer->GetCityGroupL(cities->At(0));
	// =====================================================================
	CleanupStack::PushL(group);

	if(group->Id() != iniGroupId)
		{
		result = EFalse;
		}
	if(group->Name() != iniGroupName)
		{
		result = EFalse;
		}

	CleanupStack::PopAndDestroy(group);
	CleanupStack::PopAndDestroy(cities);

	PrintResult(result);
	return result;
	}

TBool CTzGroupTestStep::GetCitiesInGroupFromGroupIdTestL()
	{
	TBool result = ETrue;
	INFO_PRINTF1(KGetCitiesInGroupFromGroupIdTest);

	// Get data from ini file section
	TInt iniGroupId;
	GetIntFromConfig(ConfigSection(),KIniGroupIdTag,iniGroupId);

	// Testing this call in this step ========================================
	CTzLocalizedCityArray* cities = iLocalizer->GetCitiesInGroupL(iniGroupId);
	// =======================================================================
	CleanupStack::PushL(cities);
	result = CheckCitiesinGroup(*cities,ConfigSection());
	CleanupStack::PopAndDestroy(cities);

	PrintResult(result);
	return result;
	}

TBool CTzGroupTestStep::GetCitiesInGroupFromCityTestL()
	{
	TBool result = ETrue;
	INFO_PRINTF1(KGetCitiesInGroupFromCityTest);

	// Get data from ini file section
	TInt iniGroupId;
	GetIntFromConfig(ConfigSection(),KIniGroupIdTag,iniGroupId);

	// Get all cities in group
	CTzLocalizedCityArray* citiesFromId = iLocalizer->GetCitiesInGroupL(iniGroupId);
	CleanupStack::PushL(citiesFromId);

	// Use first city to get the cities in the group

	// Testing this call in this step =========================================================
	CTzLocalizedCityArray* citiesFromCity = iLocalizer->GetCitiesInGroupL(citiesFromId->At(0));
	// ========================================================================================
	CleanupStack::PushL(citiesFromCity);
	result = CheckCitiesinGroup(*citiesFromCity,ConfigSection());

	CleanupStack::PopAndDestroy(citiesFromCity);
	CleanupStack::PopAndDestroy(citiesFromId);

	PrintResult(result);
	return result;
	}

TBool CTzGroupTestStep::GetCitiesInGroupFromGroupTestL()
	{
	TBool result = ETrue;
	INFO_PRINTF1(KGetCitiesInGroupFromGroupTest);

	// Get data from ini file section
	TInt iniGroupId;
	GetIntFromConfig(ConfigSection(),KIniGroupIdTag,iniGroupId);

	// Get group from id
	CTzLocalizedCityGroup* group = iLocalizer->GetCityGroupL(iniGroupId);
	CleanupStack::PushL(group);

	// use group to get cities

	// Testing this call in this step ====================================
	CTzLocalizedCityArray* cities = iLocalizer->GetCitiesInGroupL(*group);
	// ===================================================================
	CleanupStack::PushL(cities);
	result = CheckCitiesinGroup(*cities,ConfigSection());

	CleanupStack::PopAndDestroy(cities);
	CleanupStack::PopAndDestroy(group);

	PrintResult(result);
	return result;
	}

//=============================================================================
// CTzAllGroupsTestStep
//=============================================================================
CTzAllGroupsTestStep::CTzAllGroupsTestStep()
	{
	SetTestStepName(KTzAllGroupsTestStep);
	}

CTzAllGroupsTestStep::~CTzAllGroupsTestStep()
	{
	}

TVerdict CTzAllGroupsTestStep::doTestStepL()
	{
	if(!GetAllGroupsTestL())
		{
		SetTestStepResult(EFail);
		}

	return TestStepResult();
	}
TBool CTzAllGroupsTestStep::GetAllGroupsTestL()
	{
	TBool result = ETrue;
	INFO_PRINTF1(KGetAllGroupsTest);

	TInt iniNumberOfGroups;
	GetIntFromConfig(ConfigSection(),KIniNumberOfGroupsTag,iniNumberOfGroups);

	// get all groups

	// Testing this call in this step ===================================
	CTzLocalizedCityGroupArray* groups = iLocalizer->GetAllCityGroupsL();
	// ==================================================================

	CleanupStack::PushL(groups);

	// check that no. of groups is correct
	if(groups->Count() != iniNumberOfGroups)
		{
		_LIT(KSXint,"Got groups->Count() %d , whilst needed %d");
    	INFO_PRINTF3(KSXint, groups->Count(), iniNumberOfGroups );
		result = EFalse;
		PrintResult(result);
		return result;
		}

	// assign variable descriptor for looping through all group sections
	TBuf<KIniTagLength> groupSection;
	// variable for testing current Group Id
	TInt iniGroupId;
	// variable descriptor for testing group name
	TPtrC iniGroupName;

	for(TInt i=0; i<iniNumberOfGroups; ++i)
		{
		groupSection = KIniGroupTag;
		groupSection.AppendNum(i+1);

		// read data from ini file
		GetIntFromConfig(groupSection,KIniGroupIdTag,iniGroupId);
		GetStringFromConfig(groupSection,KIniGroupName,iniGroupName);

		// test group id
		if(groups->At(i).Id() != iniGroupId)
			{
			_LIT(KSXintG,"Got groups->At(i).Id()= %d , whilst needed iniGroupId= %d");
	    	INFO_PRINTF3(KSXintG, groups->At(i).Id() , iniGroupId );
			result = EFalse;
			}
		// test name
		if(groups->At(i).Name() != iniGroupName)
			{
			TPtrC temp( groups->At(i).Name() );
			if ( (&temp)->Length()!=0  )
	 			{
				_LIT(KSX1,"CTzAllGroupsTestStep: got Tz groupName %S, whilst in ini %S \n");
  				INFO_PRINTF3(KSX1, &temp, &iniGroupName);
				}
			result = EFalse;
			}
		}


	CleanupStack::PopAndDestroy(groups);

	PrintResult(result);
	return result;
	}

//=============================================================================
// CTzSortingTestStep
//=============================================================================
CTzSortingTestStep::CTzSortingTestStep()
	{
	SetTestStepName(KTzSortingTestStep);
	}

CTzSortingTestStep::~CTzSortingTestStep()
	{
	}

TVerdict CTzSortingTestStep::doTestStepL()
	{
	// Find out which test to run
	TPtrC iniSortStep;
	GetStringFromConfig(ConfigSection(),KIniSortStepTag,iniSortStep);
	TPtrC iniSortOrder;
	GetStringFromConfig(ConfigSection(),KIniSortStepTypeTag,iniSortOrder);
	CTzLocalizer::TTzSortOrder sortOrder = GetSortOrder(iniSortOrder);

	if(iniSortStep == KIniSortStepTimeZoneOptionTag)
		{
		if(!SortTimeZonesTestL(sortOrder))
			{
			SetTestStepResult(EFail);
			}
		}
	else if(iniSortStep == KIniSortStepCityOptionTag)
		{
		if(!SortCitiesTestL(sortOrder))
			{
			SetTestStepResult(EFail);
			}
		}
	else if(iniSortStep == KIniSortStepGroupOptionTag)
		{
		if(!SortGroupsTestL(sortOrder))
			{
			SetTestStepResult(EFail);
			}
		}
	else if(iniSortStep == KIniSortStepCitiesInGroupOptionTag)
		{
		TBool citiesInGroup = ETrue;
		if(!SortCitiesTestL(sortOrder,citiesInGroup))
			{
			SetTestStepResult(EFail);
			}
		}

	return TestStepResult();
	}

TBool CTzSortingTestStep::SortTimeZonesTestL(CTzLocalizer::TTzSortOrder aSortOrder)
	{
	INFO_PRINTF1(KSortTimeZonesTest);
	TBool result = ETrue;

	PrintSortOrder(aSortOrder);

	// Testing this call in this step ==============================================
	CTzLocalizedTimeZoneArray* timeZones = iLocalizer->GetAllTimeZonesL(aSortOrder);
	// =============================================================================

	CleanupStack::PushL(timeZones);

	// buffer for current ini file tag to read
	TBuf<KIniTagLength> iniTag;

	TBuf<50> outputBuffer;
	INFO_PRINTF1(KExpected);

	// check that they are in the correct order
	TInt count = timeZones->Count();
	_LIT(KSXint,"Got timeZones->Count() %d ");
    INFO_PRINTF2(KSXint, count );

	for(TInt i=0; i<count; ++i)
		{
		iniTag = KIniStandardNameTag;
		iniTag.AppendNum(i+1);

		// get the standard name that we are expecting
		TPtrC iniStandardName;
		GetStringFromConfig(ConfigSection(),iniTag,iniStandardName);

		// check that the current time zone has the correct standard name
		if(timeZones->At(i).StandardName() != iniStandardName)
			{
		    INFO_PRINTF2(KSXint, i+1 );
			result = EFalse;
			TPtrC temp(timeZones->At(i).StandardName() );
			if ( (&temp)->Length()!=0  )
 				{
				_LIT(KSX1,"CTzLocalizationTestStep: got Tz StandardName %S, ini expected %S \t");
				INFO_PRINTF3(KSX1, &temp, &iniStandardName);
				}
				else if ( (&iniStandardName)->Length()!=0 )
					{
					_LIT(KNoMatch,"CTzLocalizationTestStep:CTzSortingTestStep Tz StandardName Didnt Match, expected: %S");
	    			ERR_PRINTF2(KNoMatch, &iniStandardName);		
					}
					else
					{
					result = EPass;
					} //else if		
			}

		// print out expected and actual names
		outputBuffer = iniStandardName;
		outputBuffer.Append(KTab);
		outputBuffer.Append(timeZones->At(i).StandardName());
		INFO_PRINTF1(outputBuffer);
		}

	CleanupStack::PopAndDestroy(timeZones);

	PrintResult(result);
	return result;
	}

TBool CTzSortingTestStep::SortCitiesTestL(CTzLocalizer::TTzSortOrder aSortOrder, TBool aCitiesInGroup)
	{
	if(aCitiesInGroup)
		{
		INFO_PRINTF1(KSortCitiesInGroupsTest);
		}
	else
		{
		INFO_PRINTF1(KSortCitiesTest);
		}

	TBool result = ETrue;

	PrintSortOrder(aSortOrder);

	CTzLocalizedCityArray* cities;
	if(aCitiesInGroup)
		{
		TInt groupId;
		GetIntFromConfig(ConfigSection(),KIniGroupIdTag,groupId);
		// Testing this call in this step =========================
		cities = iLocalizer->GetCitiesInGroupL(groupId,aSortOrder);
		// ========================================================
		}
	else
		{
		// Testing this call in this step ==========
		cities = iLocalizer->GetCitiesL(aSortOrder);
		// =========================================
		}
	CleanupStack::PushL(cities);

	// buffer for current ini file tag to read
	TBuf<KIniTagLength> iniTag;

	TBuf<50> outputBuffer;
	INFO_PRINTF1(KExpected);

	// check that they are in the correct order
	TInt count = cities->Count();
	for(TInt i=0; i<count; ++i)
		{
		iniTag = KIniCityNameTag;
		iniTag.AppendNum(i+1);

		// get the standard name that we are expecting
		TPtrC iniCityName;
		GetStringFromConfig(ConfigSection(),iniTag,iniCityName);

		// check that the current time zone has the correct standard name
		if(cities->At(i).Name() != iniCityName)
			{
			result = EFalse;
			}

		// print out expected and actual names
		outputBuffer = iniCityName;
		outputBuffer.Append(KTab);
		outputBuffer.Append(cities->At(i).Name());
		INFO_PRINTF1(outputBuffer);
		}

	CleanupStack::PopAndDestroy(cities);

	PrintResult(result);
	return result;
	}

TBool CTzSortingTestStep::SortGroupsTestL(CTzLocalizer::TTzSortOrder aSortOrder)
	{
	INFO_PRINTF1(KSortGroupsTest);
	TBool result = ETrue;

	PrintSortOrder(aSortOrder);

	// Testing this call in this step =============================================
	CTzLocalizedCityGroupArray* groups = iLocalizer->GetAllCityGroupsL(aSortOrder);
	// ============================================================================

	CleanupStack::PushL(groups);

	// buffer for current ini file tag to read
	TBuf<KIniTagLength> iniTag;

		TBuf<50> outputBuffer;
	INFO_PRINTF1(KExpected);

	// check that the groups are in the correct order
	TInt count = groups->Count();
	for(TInt i=0; i<count; ++i)
		{
		iniTag = KIniCityGroupNameTag;
		iniTag.AppendNum(i+1);

		// get the standard name that we are expecting
		TPtrC iniGroupName;
		GetStringFromConfig(ConfigSection(),iniTag,iniGroupName);

		// check that the current time zone has the correct standard name
		if(groups->At(i).Name() != iniGroupName)
			{
			result = EFalse;
			}

		// print out expected and actual names
		outputBuffer = iniGroupName;
		outputBuffer.Append(KTab);
		outputBuffer.Append(groups->At(i).Name());
		INFO_PRINTF1(outputBuffer);
		}

	CleanupStack::PopAndDestroy(groups);
	PrintResult(result);
	return result;
	}

//=============================================================================
// CTzFindingTestStep
//=============================================================================
CTzFindingTestStep::CTzFindingTestStep()
	{
	SetTestStepName(KTzFindingTestStep);
	}
CTzFindingTestStep::~CTzFindingTestStep()
	{
	}

TVerdict CTzFindingTestStep::doTestStepL()
	{
	if(!FindTimeZoneTestL())
		{
		SetTestStepResult(EFail);
		}
	if(!FindCityTestL())
		{
		SetTestStepResult(EFail);
		}
	if(!FindGroupTestL())
		{
		SetTestStepResult(EFail);
		}
	return TestStepResult();
	}

TBool CTzFindingTestStep::FindTimeZoneTestL()
	{
	INFO_PRINTF1(KFindTimeZoneTest);
	TBool result = ETrue;

	// get time zone string name strings to find from in file section
	TPtrC iniStandardName;
	GetStringFromConfig(ConfigSection(),KIniStandardNameTag,iniStandardName);
	TPtrC iniDaylightName;
	GetStringFromConfig(ConfigSection(),KIniDaylightNameTag,iniDaylightName);
	TPtrC iniShortStandardName;
	GetStringFromConfig(ConfigSection(),KIniShortStandardNameTag,iniShortStandardName);
	TPtrC iniShortDaylightName;
	GetStringFromConfig(ConfigSection(),KIniShortDaylightNameTag,iniShortDaylightName);

	RArray<TPtrC> iniNames;
	CleanupClosePushL(iniNames);

	iniNames.Append(iniStandardName);
	iniNames.Append(iniDaylightName);
	iniNames.Append(iniShortStandardName);
	iniNames.Append(iniShortDaylightName);

	CTzLocalizedTimeZone* timeZone;

	// test finding by specifying any of the 4 names
	TInt count = iniNames.Count();
	for(TInt i=0; i<count; ++i)
		{
		// Testing this call in this step ======================
		timeZone = iLocalizer->FindTimeZoneByNameL(iniNames[i]);
		// =====================================================

		if(timeZone == NULL)
			{
			result = EFalse;
			continue;
			}

		// check that the time zone ID is correct
		TInt iniTimeZoneID;
		GetIntFromConfig(ConfigSection(),KIniTimeZoneIdTag,iniTimeZoneID);
		if(timeZone->TimeZoneId() != iniTimeZoneID)
			{
			result = EFalse;
			}
		delete timeZone;
		}

	CleanupStack::PopAndDestroy();	// iniNames

	PrintResult(result);
	return result;
	}

TBool CTzFindingTestStep::FindCityTestL()
	{
	INFO_PRINTF1(KFindCityTest);
	TBool result = ETrue;

	// get city to find from ini file section
	TPtrC iniCityName;
	GetStringFromConfig(ConfigSection(),KIniCityNameTag,iniCityName);

	// Testing this call in this step ================================
	CTzLocalizedCity* city = iLocalizer->FindCityByNameL(iniCityName);
	// ===============================================================
	if(city != NULL)
		{
		// check that the correct city was found
		if(city->Name() != iniCityName)
			{
			result = EFalse;

			}
		}
	else
		{
		result = EFalse;
		}
		
	delete city;
		
	PrintResult(result);
	return result;
	}

TBool CTzFindingTestStep::FindGroupTestL()
	{
	INFO_PRINTF1(KFindGroupTest);
	TBool result = ETrue;

	// get group to find from ini file section
	TPtrC iniCityGroupName;
	GetStringFromConfig(ConfigSection(),KIniCityGroupNameTag,iniCityGroupName);

	// Testing this call in this step ================================================
	CTzLocalizedCityGroup* group = iLocalizer->FindCityGroupByNameL(iniCityGroupName);
	// ===============================================================================
	if(group != NULL)
		{
		// check that the correct group was found
		if(group->Name() != iniCityGroupName)
			{
			result = EFalse;
			}
		}
	else
		{
		result = EFalse;
		}
		
	delete group;
	
	PrintResult(result);
	return result;
	}

//=============================================================================
// CTzTimeZonesSpeedTestStep
//=============================================================================
CTzTimeZonesSpeedTestStep::CTzTimeZonesSpeedTestStep()
	{
	SetTestStepName(KTzTimeZonesSpeedTestStep);
	}

CTzTimeZonesSpeedTestStep::~CTzTimeZonesSpeedTestStep()
	{
	}

TVerdict CTzTimeZonesSpeedTestStep::doTestStepL()
	{
	if(!GetAllZonesSpeedTestL())
		{
		SetTestStepResult(EFail);
		}
	return TestStepResult();
	}

TBool CTzTimeZonesSpeedTestStep::GetAllZonesSpeedTestL()
	{
	INFO_PRINTF1(KGetAllZonesSpeedTest);
	TBool result = ETrue;

	// Get the sort order from the current ini file section
	TPtrC iniSortOrder;
	GetStringFromConfig(ConfigSection(),KIniSortStepTypeTag,iniSortOrder);
	CTzLocalizer::TTzSortOrder sortOrder(GetSortOrder(iniSortOrder));
	PrintSortOrder(sortOrder);
	// array to store the result of the test
	CTzLocalizedTimeZoneArray* timeZones;
	// vars for timing the test
	TTime start;
	TTime end;
	INFO_PRINTF1(KStartingTimedTest);
	start.HomeTime();
	// Testing this call in this step ==================
	timeZones = iLocalizer->GetAllTimeZonesL(sortOrder);
	// =================================================
	end.HomeTime();
	INFO_PRINTF1(KFinishedTimedTest);

	// calculate time taken
	TTimeIntervalMicroSeconds timeTaken(end.MicroSecondsFrom(start));

	// Get time limit from ini file
	TInt iniTimeLimit;
	GetIntFromConfig(ConfigSection(),KIniAllTimeZonesLimit,iniTimeLimit);

	if(timeTaken > TTimeIntervalMicroSeconds(iniTimeLimit))
		{
		result = EFalse;
		}

	// buffer for outputting test results
	TBuf<128> outputBuffer;
	outputBuffer = KTimedTestTook;
	
	TReal timeTakenSeconds = I64REAL(timeTaken.Int64());
	
	timeTakenSeconds /= 1000000.0;
	outputBuffer.AppendNum(timeTakenSeconds,TRealFormat(6,4));
	outputBuffer.Append(KSeconds);
	INFO_PRINTF1(outputBuffer);
	outputBuffer = KNumberRead;
	outputBuffer.AppendNum(timeZones->Count());
	INFO_PRINTF1(outputBuffer);

	delete timeZones;
	timeZones = NULL;

	PrintResult(result);
	return result;
	}

//=============================================================================
// CTzCitiesSpeedTestStep
//=============================================================================

CTzCitiesSpeedTestStep::CTzCitiesSpeedTestStep()
	{
	SetTestStepName(KTzCitiesSpeedTestStep);
	}

CTzCitiesSpeedTestStep::~CTzCitiesSpeedTestStep()
	{
	}

TVerdict CTzCitiesSpeedTestStep::doTestStepL()
	{
	if(!GetAllCitiesSpeedTestL())
		{
		SetTestStepResult(EFail);
		}
	return TestStepResult();
	}

TBool CTzCitiesSpeedTestStep::GetAllCitiesSpeedTestL()
	{
	INFO_PRINTF1(KGetAllCitiesSpeedTest);
	TBool result = ETrue;

	// Get the sort order from the current ini file section
	TPtrC iniSortOrder;
	GetStringFromConfig(ConfigSection(),KIniSortStepTypeTag,iniSortOrder);
	CTzLocalizer::TTzSortOrder sortOrder(GetSortOrder(iniSortOrder));
	PrintSortOrder(sortOrder);
	INFO_PRINTF1(KStartingTimedTest);
	// array to store the result of the test
	CTzLocalizedCityArray* cities;
	// vars for timing the test
	TTime start;
	TTime end;
	start.HomeTime();
	// Testing this call in this step =========
	cities = iLocalizer->GetCitiesL(sortOrder);
	// ========================================
	end.HomeTime();
	INFO_PRINTF1(KFinishedTimedTest);

	// calculate time taken
	TTimeIntervalMicroSeconds timeTaken;
	timeTaken = end.MicroSecondsFrom(start);

	// Get time limit from ini file
	TInt iniTimeLimit;
	GetIntFromConfig(ConfigSection(),KIniAllCitiesLimit,iniTimeLimit);

	if(timeTaken > TTimeIntervalMicroSeconds(iniTimeLimit))
		{
		result = EFalse;
		}

	// buffer for outputting test results
	TBuf<128> outputBuffer;
	outputBuffer = KTimedTestTook;
	TReal timeTakenSeconds;
	timeTakenSeconds = I64REAL(timeTaken.Int64());
	timeTakenSeconds /= 1000000.0;
	outputBuffer.AppendNum(timeTakenSeconds,TRealFormat(6,4));
	outputBuffer.Append(KSeconds);
	INFO_PRINTF1(outputBuffer);
	outputBuffer = KNumberRead;
	outputBuffer.AppendNum(cities->Count());
	INFO_PRINTF1(outputBuffer);
	TInt i;
	delete cities;
	cities = NULL;

	INFO_PRINTF1(KGetAllCitiesIncludingDbCitiesSpeedTest);


	// Add 100 cities, named "NewCityX"
	// Get the time zone to add the cities to - from the ini file
	TInt iniTimeZone;
	GetIntFromConfig(ConfigSection(),KIniTimeZoneIdTag,iniTimeZone);
	// keep an array of the added cities, so that they can be removed later
	CTzLocalizedCity* newCity;
	RPointerArray<CTzLocalizedCity> newCities;

	// buffer for storing new city names that are added to the DB
	TBuf<11> newCityName;
	for(i=0; i<100; ++i)
		{
		newCityName = KNewCity;
		newCityName.AppendNum(i+1);
		newCity = iLocalizer->AddCityL(iniTimeZone,newCityName);
		newCities.Append(newCity);
		}

	// time how long it takes to get the cities now
	INFO_PRINTF1(KStartingTimedTest);
	start.HomeTime();
	// Testing this call in this step =========
	cities = iLocalizer->GetCitiesL(sortOrder);
	// ========================================
	end.HomeTime();
	INFO_PRINTF1(KFinishedTimedTest);

	timeTaken = end.MicroSecondsFrom(start);

	// check that it doesn't take too much time
	if(timeTaken > TTimeIntervalMicroSeconds(iniTimeLimit))
		{
		result = EFalse;
		}

	outputBuffer = KTimedTestTook;
	timeTakenSeconds = I64REAL(timeTaken.Int64());
	timeTakenSeconds /= 1000000.0;
	outputBuffer.AppendNum(timeTakenSeconds,TRealFormat(6,4));
	outputBuffer.Append(KSeconds);
	INFO_PRINTF1(outputBuffer);
	outputBuffer = KNumberRead;
	outputBuffer.AppendNum(cities->Count());
	INFO_PRINTF1(outputBuffer);

	delete cities;
	cities = NULL;
	
	// remove the cities that were added to the DB
	for(i=newCities.Count()-1; i>=0; --i)
		{
		iLocalizer->RemoveCityL(newCities[i]);
		newCities.Remove(i);
		}
	newCities.ResetAndDestroy();
	
	PrintResult(result);
	return result;
	}

//=============================================================================
// CTzGroupsSpeedTestStep
//=============================================================================

CTzGroupsSpeedTestStep::CTzGroupsSpeedTestStep()
	{
	SetTestStepName(KTzGroupsSpeedTestStep);
	}

CTzGroupsSpeedTestStep::~CTzGroupsSpeedTestStep()
	{
	}

TVerdict CTzGroupsSpeedTestStep::doTestStepL()
	{
	if(!GetCitiesInGroupSpeedTestL())
		{
		SetTestStepResult(EFail);
		}
	return TestStepResult();
	}

TBool CTzGroupsSpeedTestStep::GetCitiesInGroupSpeedTestL()
	{
	INFO_PRINTF1(KGetCitiesInGroupSpeedTest);
	TBool result = ETrue;

	// Get the sort order from the current ini file section
	TPtrC iniSortOrder;
	GetStringFromConfig(ConfigSection(),KIniSortStepTypeTag,iniSortOrder);
	CTzLocalizer::TTzSortOrder sortOrder(GetSortOrder(iniSortOrder));
	PrintSortOrder(sortOrder);
	// Get the ID of the group containing the cities
	TInt iniGroupId;
	GetIntFromConfig(ConfigSection(),KIniGroupIdTag,iniGroupId);
	// array to store the result of the test
	CTzLocalizedCityArray* citiesInGroup;
	// vars for timing the test
	TTime start;
	TTime end;
	INFO_PRINTF1(KStartingTimedTest);
	start.HomeTime();
	// Testing this call in this step ==================================
	citiesInGroup = iLocalizer->GetCitiesInGroupL(iniGroupId,sortOrder);
	// =================================================================
	end.HomeTime();

	INFO_PRINTF1(KFinishedTimedTest);

	// calculate time taken
	TTimeIntervalMicroSeconds timeTaken(end.MicroSecondsFrom(start));

	// Get time limit from ini file
	TInt iniTimeLimit;
	GetIntFromConfig(ConfigSection(),KIniAllCitiesInGroupLimit,iniTimeLimit);

	if(timeTaken > TTimeIntervalMicroSeconds(iniTimeLimit))
		{
		result = EFalse;
		}

		// buffer for outputting test results
	TBuf<128> outputBuffer;
	outputBuffer = KTimedTestTook;
	TReal timeTakenSeconds;
	timeTakenSeconds = I64REAL(timeTaken.Int64());
	timeTakenSeconds /= 1000000.0;
	outputBuffer.AppendNum(timeTakenSeconds,TRealFormat(6,4));
	outputBuffer.Append(KSeconds);
	INFO_PRINTF1(outputBuffer);
	outputBuffer = KNumberRead;
	outputBuffer.AppendNum(citiesInGroup->Count());
	INFO_PRINTF1(outputBuffer);

	delete citiesInGroup;

	PrintResult(result);
	return result;
	}





CPropertyWatcher* CPropertyWatcher::NewL()
	{
	CPropertyWatcher* self = new (ELeave) CPropertyWatcher;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CPropertyWatcher::CPropertyWatcher() : CActive(EPriorityNormal)
	{
	CActiveScheduler::Add(this);
	}
	
CPropertyWatcher::~CPropertyWatcher()
	{
	Cancel();
	}
	
void CPropertyWatcher::ConstructL()
	{
	User::LeaveIfError(iProperty.Attach(NTzUpdate::KPropertyCategory, NTzUpdate::EHomeTimeZoneId));
	}
	
void CPropertyWatcher::Start()
	{
	if (!IsActive())
		{
		SetActive();
		iProperty.Subscribe(iStatus);
		iPubSubChange.iNewTimeZoneId = KErrNotFound;
		iPubSubChange.iOldTimeZoneId = KErrNotFound;
		}
	}

TInt CPropertyWatcher::PubSubTzId() const
	{
	return iPubSubChange.iNewTimeZoneId;
	}

TInt CPropertyWatcher::OldPubSubTzId() const
	{
	return iPubSubChange.iOldTimeZoneId;
	}

void CPropertyWatcher::RunL()
	{
	TPckgBuf<NTzUpdate::TTimeZoneChange> changeBuf;
	iProperty.Get(changeBuf);
	iPubSubChange = changeBuf();
	CActiveScheduler::Stop();
	}
	
void CPropertyWatcher::DoCancel()
	{
	iProperty.Cancel();
	}

CTzPubSubTestStep::CTzPubSubTestStep()
	{
	}

CTzPubSubTestStep::~CTzPubSubTestStep()
	{
	delete iPropertyWatcher;
	}

void CTzPubSubTestStep::TestTimeZoneIdL(TInt aTzId)
	{
	_LIT(KTestTimeZoneId, "Testing home time pub-sub notification, setting id to: ");
	TBuf<128> outputBuffer;
	outputBuffer.Copy(KTestTimeZoneId);
	outputBuffer.AppendNum(aTzId);
	INFO_PRINTF1(outputBuffer);
	
	iPropertyWatcher->Start();
	iLocalizer->SetFrequentlyUsedZoneL(aTzId, CTzLocalizedTimeZone::EHomeZone);
	CActiveScheduler::Start();
	if (iPropertyWatcher->PubSubTzId() != aTzId)
		{
		ERR_PRINTF1(_L("New home time zone ID different from time zone RProperty"));
		SetTestStepResult(EFail);
		}
	if (iOldTzId != KErrNotFound && iOldTzId != iPropertyWatcher->OldPubSubTzId())
		{
		ERR_PRINTF1(_L("Old home time zone ID different from time zone RProperty"));
		SetTestStepResult(EFail);
		}
	iOldTzId = aTzId;
	}

TVerdict CTzPubSubTestStep::doTestStepL()
	{
	SetTestStepResult(EPass);
	
	iPropertyWatcher = CPropertyWatcher::NewL();
	
	iOldTzId = KErrNotFound;
	TestTimeZoneIdL(2592);
	TestTimeZoneIdL(920);
	TestTimeZoneIdL(2656);
		
	return TestStepResult();
	}

   
CTzRaceTestStep::CTzRaceTestStep()
	{   
  
	}

CTzRaceTestStep::~CTzRaceTestStep()
	{   
   
	}

TVerdict CTzRaceTestStep::doTestStepL()
	{
	TInt ret1, ret2 ;

	ret1 = ret2 = -1 ;

	iThread1.Create(_L("Thread1"), TThreadFunction1, 4096, 4096, 64000, &ret1);
	INFO_PRINTF1(_L("Thread1 created\n")) ;   

	iThread2.Create(_L("Thread2"), TThreadFunction2, 4096, 4096, 64000, &ret2);
	INFO_PRINTF1(_L("Thread2 created\n")) ;
	           
	iThread1.Resume() ;
	iThread2.Resume() ;

	TRequestStatus stat ;
	   
	iThread1.Logon(stat) ;
	iThread2.Logon(stat) ;   
	   
	TInt i ;
	   
	for(i = 0; i < 2; i++)
		{    /* doing the thread join */
		INFO_PRINTF1(_L("Waiting for the thread to die\n"));
		User::WaitForRequest(stat) ;       
		}   

	__ASSERT_ALWAYS((ret1 == KErrNone), SetTestStepResult(EFail)) ;
	__ASSERT_ALWAYS((ret2 == KErrNone), SetTestStepResult(EFail)) ;   

	iThread2.Close() ;
	iThread1.Close() ;

	return TestStepResult();
	   
	}
       
TInt CTzRaceTestStep::TThreadFunction1(TAny* arg)
	{
	TInt err, i ;
	CTzLocalizer* localizer1 = NULL;
	CTrapCleanup* cleanup=CTrapCleanup::New();
	CActiveScheduler* scheduler = new CActiveScheduler();
	CActiveScheduler::Install(scheduler);

	for(i = 0; i < KTime; i++)
		{
	    TRAP(err, localizer1 = CTzLocalizer::NewL());
	    if (err != KErrNone)
		    {
	        *(static_cast<TInt *> (arg)) = err ;   
	        User::Exit(err) ;
		    }           
	    delete localizer1 ;
		}

	delete cleanup ;
	delete scheduler ;

	*(static_cast<TInt *> (arg)) = err ;   
	return err ;

	}


TInt CTzRaceTestStep::TThreadFunction2(TAny* arg)
	{
	TInt err, i ;
	CTzLocalizer* localizer2 = NULL ;

	CTrapCleanup* cleanup=CTrapCleanup::New();   
	CActiveScheduler* scheduler = new CActiveScheduler();
	CActiveScheduler::Install(scheduler);

	for(i = 0; i < KTime; i++)
		{
		TRAP(err, localizer2 = CTzLocalizer::NewL());
		if (err != KErrNone)
			{
			*(static_cast<TInt *> (arg)) = err ;   
			User::Exit(err) ;
			}           
		delete localizer2 ;
		}

	delete cleanup ;
	delete scheduler ;   

	*(static_cast<TInt *> (arg)) = err ;   
	return err ;

	}

//=============================================================================
// CPrepEnvDelFileStep
//=============================================================================

CPrepEnvDelFileStep::CPrepEnvDelFileStep()
	{
	SetTestStepName(KTzPrepEnvDelFileTestStep);
	}

CPrepEnvDelFileStep::~CPrepEnvDelFileStep()
	{
	}

TVerdict CPrepEnvDelFileStep::doTestStepL()
	{
	RPIMTestServer serv;
	User::LeaveIfError(serv.Connect());
	CleanupClosePushL(serv);
	serv.CloseTzSession();
	INFO_PRINTF1(_L("Killing agenda server\n")) ;
	_LIT(KAgendaServer,"agsvexe*");
	TRAP_IGNORE(serv.KillProcessL(KAgendaServer));
	INFO_PRINTF1(_L("Killing Timezone server\n")) ;
	_LIT(KTzServer,"Tzserver*");
	TRAP_IGNORE(serv.KillProcessL(KTzServer));
	INFO_PRINTF1(_L("Deleting localization database\n")) ;
	serv.DeleteFileL(KLocDb);
	CleanupStack::PopAndDestroy();
	return TestStepResult();
	}

//=============================================================================
// CPrepEnvCopyFileStep
//=============================================================================

CPrepEnvCopyFileStep::CPrepEnvCopyFileStep()
	{
	SetTestStepName(KTzPrepEnvCopyFileTestStep);
	}

CPrepEnvCopyFileStep::~CPrepEnvCopyFileStep()
	{
	}

TVerdict CPrepEnvCopyFileStep::doTestStepL()
	{
	RPIMTestServer serv;
	User::LeaveIfError(serv.Connect());
	CleanupClosePushL(serv);
	serv.CloseTzSession();
	serv.CopyFileL(KBadLocDb, KLocDb);
	CleanupStack::PopAndDestroy();
	return TestStepResult();
	}
