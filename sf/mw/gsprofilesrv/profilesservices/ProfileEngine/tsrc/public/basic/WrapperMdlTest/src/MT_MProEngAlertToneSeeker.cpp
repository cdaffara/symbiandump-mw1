/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the MT_MProEngAlertToneSeeker test class.
*
*/


////////////////////////////////////////////////////////////////////////
// MT_MProEngAlertToneSeeker.cpp
//

//  CLASS HEADER
#include "MT_MProEngAlertToneSeeker.h"

//  EXTERNAL INCLUDES
#include <MProEngAlertToneSeeker.h>
#include <ProEngFactory.h>
#include <EUnitMacros.h>

MT_MProEngAlertToneSeeker* MT_MProEngAlertToneSeeker::NewL()
    {
    MT_MProEngAlertToneSeeker* self = MT_MProEngAlertToneSeeker::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

MT_MProEngAlertToneSeeker* MT_MProEngAlertToneSeeker::NewLC()
    {
    MT_MProEngAlertToneSeeker* self = new( ELeave ) MT_MProEngAlertToneSeeker();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

MT_MProEngAlertToneSeeker::~MT_MProEngAlertToneSeeker()
    {
    delete iObserver;
    }

MT_MProEngAlertToneSeeker::MT_MProEngAlertToneSeeker()
    {
    }

void MT_MProEngAlertToneSeeker::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    iObserver = MT_CProEngAlertToneSeekerObserver::NewL();
    }

void MT_MProEngAlertToneSeeker::SetupL()
    {
    iSeeker = ProEngFactory::NewAlertToneSeekerL();
    }

void MT_MProEngAlertToneSeeker::SetupCancelFetchTestL()
    {
    SetupL();
    iObserver->iNotified = EFalse;
    iSeeker->FetchAlertToneListL( *iObserver );
    }

void MT_MProEngAlertToneSeeker::Teardown()
    {
    delete iObserver->iToneList;
    iObserver->iToneList = NULL;
    delete iSeeker;
    }

void MT_MProEngAlertToneSeeker::FetchAlertToneListTestL()
    {
    iSeeker->FetchAlertToneListL( *iObserver );
    MT_CSchedulerUtility::InstanceL()->Start();
    EUNIT_ASSERT( iObserver->iNotified );
    EUNIT_ASSERT( iObserver->iToneList );
    TInt count( iObserver->iToneList->MdcaCount() );
    EUNIT_PRINT( _L( "*** Tones: ***" ) );
    for( TInt i(0); i<count; ++i )
        {
        TPtrC tone( iObserver->iToneList->MdcaPoint( i ) );
        EUNIT_PRINT( _L( "%S" ), &tone );
        }
    }

void MT_MProEngAlertToneSeeker::CancelFetchTestL()
    {
    EUNIT_PRINT( _L( "CancelFetchTestL begin" ) );
    if( !iObserver->iNotified )
        {
        EUNIT_PRINT( _L( "CancelFetchTestL before seeker->Cancel" ) );
        iSeeker->CancelFetch();
        EUNIT_PRINT( _L( "CancelFetchTestL after seeker->Cancel" ) );
        }
    EUNIT_ASSERT( ETrue );
    EUNIT_PRINT( _L( "CancelFetchTestL end" ) );
    }


EUNIT_BEGIN_TEST_TABLE( 
    MT_MProEngAlertToneSeeker,
    "Wrapper API Alert Tone Seeker Test cases.",
    "MODULE" )

EUNIT_TEST(
    "Fetch Alert Tone List - test",
    "MProEngAlertToneSeeker",
    "FetchAlertToneListL",
    "FUNCTIONALITY",
    SetupL, FetchAlertToneListTestL, Teardown)

EUNIT_TEST(
    "Cancel Fetch Alert Tone List - test",
    "MProEngAlertToneSeeker",
    "CancelFetch",
    "FUNCTIONALITY",
    SetupCancelFetchTestL, CancelFetchTestL, Teardown)

EUNIT_END_TEST_TABLE

// End of File

