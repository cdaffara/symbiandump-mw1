// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "IsDaylightSavingsOn.h"
#include "DstStep.h"
#include "DstUnitStep.h"
#include "DstUnitServer.h"
#include <tz.h>
#include <vtzrules.h>

TInt CDaylightSavingsTest::CreateAndRunTestUnitL()
	{
	TestDaylightSavingsOnL();

    return KErrNone;	
	}
	
CDaylightSavingsTest::CDaylightSavingsTest(CDstUnitServer& aServer) : 
    CDstUnitStep( aServer )
    {
    SetTestStepName( KDaylightSavingsStep );
    }

CDaylightSavingsTest::~CDaylightSavingsTest()
	{
	// nothing to destroy
	}
	
void CDaylightSavingsTest::DestroyTestUnit()
	{
	// nothing to destroy
	}

void CDaylightSavingsTest::TestDaylightSavingsOnL()
    {
    RTz tz;
   	User::LeaveIfError( tz.Connect() );
   	CleanupClosePushL( tz );
    // Set time zone to London.
    _LIT8( KTzEuropeLondon, "Europe/London" );
	CTzId* tzEuropeLondonId = CTzId::NewL( KTzEuropeLondon );
	CleanupStack::PushL( tzEuropeLondonId );
	tz.SetTimeZoneL( *tzEuropeLondonId );
	
	_LIT( KTimeOff, "20060101:000000.000000" );
	TBool dstSettingOn = ETrue;
    TTime timeOff( KTimeOff );
    TBool dstSetting = tz.IsDaylightSavingOnL( *tzEuropeLondonId, timeOff );
	if(dstSetting == dstSettingOn)
		{
		User::Leave( KErrGeneral );
		} 	
   	
   	_LIT( KTimeOn, "20060601:000000.000000" );
    TTime timeOn( KTimeOn );
    dstSetting = tz.IsDaylightSavingOnL( *tzEuropeLondonId, timeOn );
	if(dstSetting != dstSettingOn)
		{
		User::Leave( KErrGeneral );
		}
		
	CleanupStack::PopAndDestroy( tzEuropeLondonId ); 
   	
    // Set time zone to Vancouver.
    _LIT8( KTzAmericaVancouver, "America/Vancouver" );
	CTzId* tzId = CTzId::NewL( KTzAmericaVancouver );
	CleanupStack::PushL( tzId );
	tz.SetTimeZoneL( *tzId );

    // Get rules from the server for 1 year before and after the period 
    // of interest.
    TInt startYear = 2006;
    _LIT( KTime1, "20050000:000000.000000" );
    TTime startTime( KTime1 );
    _LIT( KTime2, "20070000:000000.000000" );
    TTime endTime( KTime2 );
    CTzRules* rules = tz.GetTimeZoneRulesL( *tzId, startTime, endTime, 
        ETzWallTimeReference);
   	CleanupStack::PopAndDestroy( 2 );  // tz, tzId
    CleanupStack::PushL( rules );

    // A time that falls in DST.
    _LIT( KTime3, "20060601:000000.000000" );
    TTime dstTime( KTime3 );

    // A time that falls in standard time.
    _LIT( KTime4, "20060101:000000.000000" );
    TTime stdTime( KTime4 );

    // Get actualized rules for 1 year before/after 2006. Any year will do.
    CVTzActualisedRules* actRules = CVTzActualisedRules::NewL(
        startYear - 1, startYear + 1 );
    CleanupStack::PushL( actRules );
    rules->GetActualisedRulesL( *actRules );

    // DST should be on.
    TInt dstOn = actRules->IsDaylightSavingOn( dstTime );
    if ( !dstOn )
        {
        User::Leave( KErrGeneral );
        }
    
    // DST should be off.
    dstOn = actRules->IsDaylightSavingOn( stdTime );
    if (dstOn)
        {
        User::Leave( KErrGeneral );
        }

    // Check the std offset for dst and std times.
    TInt dstOffset = actRules->GetOffsetFromRuleL( dstTime, 
        ETzWallTimeReference );
    TInt expectedOffset = -420; // -7 hrs
    if ( dstOffset != expectedOffset)
        {
        User::Leave( KErrGeneral );
        }

    TInt stdOffset = actRules->GetOffsetFromRuleL( stdTime, 
        ETzWallTimeReference );
    expectedOffset = -480; // -8 hrs
    if ( stdOffset != expectedOffset)
        {
        User::Leave( KErrGeneral );
        }

    CleanupStack::PopAndDestroy( 2 );
    }
