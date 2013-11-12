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
* Description:  Implementation of the MT_MProEngTones test class.
*
*/

////////////////////////////////////////////////////////////////////////
// MT_MProEngTones.cpp
//

//  CLASS HEADER
#include "MT_MProEngTones.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <MProEngTones.h>
#include <MProEngProfile.h>
#include <MProEngEngine.h>
#include "MProfileEngineExtended.h"
#include "MProfile.h"
#include "MProfileTones.h"
#include "MProfileExtraTones.h"
#include "MProfileExtended.h"
#include <ProEngFactory.h>
#include <Profile.hrh>
#include <centralrepository.h>
#include <ProfileEngineInternalCRKeys.h>

//  INTERNAL INCLUDES
#include "T_ProfileTestTools.h"

MT_MProEngTones* MT_MProEngTones::NewL()
    {
    MT_MProEngTones* self = MT_MProEngTones::NewLC();
    CleanupStack::Pop();

    return self;
    }

MT_MProEngTones* MT_MProEngTones::NewLC()
    {
    MT_MProEngTones* self = new( ELeave ) MT_MProEngTones();
    CleanupStack::PushL( self );

	self->ConstructL();

    return self;
    }


MT_MProEngTones::~MT_MProEngTones()
    {
    if( iInternalEngine )
        {
        iInternalEngine->Release();
        iInternalEngine = NULL;
        }
    if( iEngine )
        {
        iEngine->Release();
        iEngine = NULL;
        }
    }


MT_MProEngTones::MT_MProEngTones()
    {
    }

void MT_MProEngTones::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    iInternalEngine = CreateProfileEngineExtendedL();
    iEngine = ProEngFactory::NewEngineL();
    }


void MT_MProEngTones::SetupL()
    {
    iInternalProfile = iInternalEngine->ProfileL( iProfileId );
    iProfile = iEngine->ProfileL( iProfileId );
    }

void MT_MProEngTones::Setup0L()
    {
    iProfileId = 0;
    SetupL();
    }

void MT_MProEngTones::Setup1L()
    {
    iProfileId = 1;
    SetupL();
    }

void MT_MProEngTones::Setup2L()
    {
    iProfileId = 2;
    SetupL();
    }

void MT_MProEngTones::Setup3L()
    {
    iProfileId = 3;
    SetupL();
    }

void MT_MProEngTones::Setup4L()
    {
    iProfileId = 4;
    SetupL();
    }

#ifdef __OFFLINE_MODE
void MT_MProEngTones::Setup5L()
    {
    iProfileId = 5;
    SetupL();
    }

#endif // __OFFLINE_MODE
#ifdef __DRIVE_MODE
void MT_MProEngTones::Setup6L()
    {
    iProfileId = 6;
    SetupL();
    }

#endif // __DRIVE_MODE


void MT_MProEngTones::Teardown()
    {
    if( iInternalProfile )
        {
        iInternalProfile->Release();
        iInternalProfile = NULL;
        }
    if( iProfile )
        {
        iProfile->Release();
        iProfile = NULL;
        }
    }

void MT_MProEngTones::GetRingingTone1TestL()
    {
    MProEngTones& tones( iProfile->ProfileTones() );
    const MProfileTones& tones2( iInternalProfile->ProfileTones() );
    EUNIT_ASSERT( tones.RingingTone1().Compare( tones2.RingingTone1() ) == 0 );
    }

void MT_MProEngTones::GetRingingTone2TestL()
    {
    MProEngTones& tones( iProfile->ProfileTones() );
    const MProfileTones& tones2( iInternalProfile->ProfileTones() );
    EUNIT_ASSERT( tones.RingingTone2().Compare( tones2.RingingTone2() ) == 0 );
    }

void MT_MProEngTones::GetMessageAlertToneTestL()
    {
    MProEngTones& tones( iProfile->ProfileTones() );
    const MProfileTones& tones2( iInternalProfile->ProfileTones() );
    EUNIT_ASSERT( tones.MessageAlertTone().Compare( tones2.MessageAlertTone() ) == 0 );
    }

void MT_MProEngTones::GetEmailAlertToneTestL()
    {
    MProEngTones& tones( iProfile->ProfileTones() );
    const MProfileExtraTones& tones2( iInternalProfile->ProfileExtraTones() );
    EUNIT_ASSERT( tones.EmailAlertTone().Compare( tones2.EmailAlertTone() ) == 0 );
    }

void MT_MProEngTones::GetVideoCallRingingToneTestL()
    {
    MProEngTones& tones( iProfile->ProfileTones() );
    const MProfileExtraTones& tones2( iInternalProfile->ProfileExtraTones() );
    EUNIT_ASSERT( tones.VideoCallRingingTone().Compare( tones2.VideoCallRingingTone() ) == 0 );
    }


void MT_MProEngTones::SetRingingTone1TestL()
    {
    MProEngTones& tones( iProfile->ProfileTones() );
    const TDesC& origTone( tones.RingingTone1() );
    TInt err( tones.SetRingingTone1L( KProEngMTTestTone ) );
    if( iProfileId != EProfileDriveId )
        {
        EUNIT_ASSERT( err == KErrNone );
        EUNIT_ASSERT( tones.RingingTone1().Compare( KProEngMTTestTone ) == 0 );
        }
    else
        {
        EUNIT_ASSERT( err == KErrAccessDenied );
        EUNIT_ASSERT( tones.RingingTone1().Compare( origTone ) == 0 );
        }
    }

void MT_MProEngTones::SetRingingTone2TestL()
    {
    MProEngTones& tones( iProfile->ProfileTones() );
    const TDesC& origTone( tones.RingingTone2() );
    TInt err( tones.SetRingingTone2L( KProEngMTTestTone ) );
    if( iProfileId != EProfileDriveId )
        {
        EUNIT_ASSERT( err == KErrNone );
        EUNIT_ASSERT( tones.RingingTone2().Compare( KProEngMTTestTone ) == 0 );
        }
    else
        {
        EUNIT_ASSERT( err == KErrAccessDenied );
        EUNIT_ASSERT( tones.RingingTone2().Compare( origTone ) == 0 );
        }
    }

void MT_MProEngTones::SetMessageAlertToneTestL()
    {
    MProEngTones& tones( iProfile->ProfileTones() );
    const TDesC& origTone( tones.MessageAlertTone() );
    TInt err( tones.SetMessageAlertToneL( KProEngMTTestTone ) );
    if( iProfileId != EProfileDriveId )
        {
        EUNIT_ASSERT( err == KErrNone );
        EUNIT_ASSERT( tones.MessageAlertTone().Compare( KProEngMTTestTone ) == 0 );
        }
    else
        {
        EUNIT_ASSERT( err == KErrAccessDenied );
        EUNIT_ASSERT( tones.MessageAlertTone().Compare( origTone ) == 0 );
        }
    }

void MT_MProEngTones::SetEmailAlertToneTestL()
    {
    MProEngTones& tones( iProfile->ProfileTones() );
    const TDesC& origTone( tones.EmailAlertTone() );
    TInt err( tones.SetEmailAlertToneL( KProEngMTTestTone ) );
    if( iProfileId != EProfileDriveId )
        {
        EUNIT_ASSERT( err == KErrNone );
        EUNIT_ASSERT( tones.EmailAlertTone().Compare( KProEngMTTestTone ) == 0 );
        }
    else
        {
        EUNIT_ASSERT( err == KErrAccessDenied );
        EUNIT_ASSERT( tones.EmailAlertTone().Compare( origTone ) == 0 );
        }
    }

void MT_MProEngTones::SetVideoCallRingingToneTestL()
    {
    MProEngTones& tones( iProfile->ProfileTones() );
    const TDesC& origTone( tones.VideoCallRingingTone() );
    TInt err( tones.SetVideoCallRingingToneL( KProEngMTTestTone ) );
    if( ( iProfileId != EProfileOffLineId ) &&
        ( iProfileId != EProfileDriveId ) )
        {
        EUNIT_ASSERT( err == KErrNone );
        EUNIT_ASSERT( tones.VideoCallRingingTone().Compare( KProEngMTTestTone ) == 0 );
        }
    else
        {
        EUNIT_ASSERT( err == KErrAccessDenied );
        EUNIT_ASSERT( tones.VideoCallRingingTone().Compare( origTone ) == 0 );
        }
    }

/*
void MT_MProEngTones::SetDRMRingingTone1TestL()
    {
    MProEngTones& tones( iProfile->ProfileTones() );
    const TDesC& origTone( tones.RingingTone1() );

    // combined delivery tests:
    TInt err( tones.SetRingingTone1L( KProEngMTTestDRM_CD_StartEndTone ) );
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( tones.RingingTone1().Compare( KProEngMTTestDRM_CD_StartEndTone ) == 0 );
    TInt error( KErrNone );
    TRAP( error, iProfile->CommitChangeL() );
    EUNIT_ASSERT( error == KErrNone );

    err = tones.SetRingingTone1L( KProEngMTTestDRM_CD_5TimesTone );
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( tones.RingingTone1().Compare( KProEngMTTestDRM_CD_5TimesTone ) == 0 );
    TRAP( error, iProfile->CommitChangeL() );
    EUNIT_ASSERT( error != KErrNone );

    err = tones.SetRingingTone1L( KProEngMTTestDRM_CD_FutureTone );
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( tones.RingingTone1().Compare( KProEngMTTestDRM_CD_FutureTone ) == 0 );
    TRAP( error, iProfile->CommitChangeL() );
    EUNIT_ASSERT( error != KErrNone );

    err = tones.SetRingingTone1L( KProEngMTTestDRM_CD_1YearTone );
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( tones.RingingTone1().Compare( KProEngMTTestDRM_CD_1YearTone ) == 0 );
    TRAP( error, iProfile->CommitChangeL() );
    EUNIT_ASSERT( error == KErrNone );

    // separate delivery tests:
    err = tones.SetRingingTone1L( KProEngMTTestDRM_SD_StartEndTone );
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( tones.RingingTone1().Compare( KProEngMTTestDRM_SD_StartEndTone ) == 0 );
    TRAP( error, iProfile->CommitChangeL() );
    EUNIT_ASSERT( error == KErrNone );

    err = tones.SetRingingTone1L( KProEngMTTestDRM_SD_5TimesTone );
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( tones.RingingTone1().Compare( KProEngMTTestDRM_SD_5TimesTone ) == 0 );
    TRAP( error, iProfile->CommitChangeL() );
    EUNIT_ASSERT( error != KErrNone );

    err = tones.SetRingingTone1L( KProEngMTTestDRM_SD_1YearTone );
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( tones.RingingTone1().Compare( KProEngMTTestDRM_SD_1YearTone ) == 0 );
    TRAP( error, iProfile->CommitChangeL() );
    EUNIT_ASSERT( error == KErrNone );

    err = tones.SetRingingTone1L( KProEngMTTestDRM_SD_DisplayOnly );
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( tones.RingingTone1().Compare( KProEngMTTestDRM_SD_DisplayOnly ) == 0 );
    TRAP( error, iProfile->CommitChangeL() );
    EUNIT_ASSERT( error != KErrNone );

    }
*/

/*
void MT_MProEngTones::SetRingingToneSizeLimitTestL()
    {
    MProEngTones& tones( iProfile->ProfileTones() );
    const TDesC& origTone( tones.RingingTone1() );
    TInt nToneLimitKB = GetToneSizeLimitL();
    // Is tone file size limit enabled?
    if ( nToneLimitKB )
		{
		// Yes, this should succeed
		TInt err( tones.SetRingingTone1L( KProEngMTTestToneSizeOk ) );
		EUNIT_ASSERT( err == KErrNone );
	    TRAP( err, iProfile->CommitChangeL() );
		EUNIT_ASSERT( err == KErrNone );
		EUNIT_ASSERT( tones.RingingTone1().Compare( KProEngMTTestToneSizeOk ) == 0 );
		
		// This should fail			
		err = tones.SetRingingTone1L( KProEngMTTestToneSizeTooBig );
		EUNIT_ASSERT( err == KErrNone );
	    TRAP( err, iProfile->CommitChangeL() );
		EUNIT_ASSERT( err == KErrTooBig );
		}
	else
		{
		// Because checking feature is OFF, setting both files should succeed
		TInt err( tones.SetRingingTone1L( KProEngMTTestToneSizeOk ) );
		EUNIT_ASSERT( err == KErrNone );
	    TRAP( err, iProfile->CommitChangeL() );
		EUNIT_ASSERT( err == KErrNone );	    
		EUNIT_ASSERT( tones.RingingTone1().Compare( KProEngMTTestToneSizeOk ) == 0 );
		
		err = tones.SetRingingTone1L( KProEngMTTestToneSizeTooBig );
		EUNIT_ASSERT( err == KErrNone );
	    TRAP( err, iProfile->CommitChangeL() );
		EUNIT_ASSERT( err == KErrNone );	    
		EUNIT_ASSERT( tones.RingingTone1().Compare( KProEngMTTestToneSizeTooBig ) == 0 );
		}
    }


// Read tone file size limit from cenrep    
int MT_MProEngTones::GetToneSizeLimitL()
    {
    // Set file size limit if configured ON.
	TInt sizeLimitKB( 0 );
	CRepository* cenrep = CRepository::NewL( KCRUidProfileEngine );
	CleanupStack::PushL( cenrep );
	User::LeaveIfError( cenrep->Get( KProEngRingingToneMaxSize,
                                     sizeLimitKB ) );
	CleanupStack::PopAndDestroy(); // cenrep
	if ( sizeLimitKB < 0 )
	    {
	    sizeLimitKB = 0;
	    }
    return sizeLimitKB;
    }    
*/
    
//-------------------------------------------------------------    

EUNIT_BEGIN_TEST_TABLE(
    MT_MProEngTones,
    "Profile Tones Wrapper Test cases.",
    "MODULE" )

EUNIT_TEST(
    "Get Line1 Ringing Tone 0 - test",
    "MProEngTones",
    "RingingTone1",
    "FUNCTIONALITY",
    Setup0L,
    GetRingingTone1TestL,
    Teardown)

EUNIT_TEST(
    "Get Line2 Ringing Tone 0 - test",
    "MProEngTones",
    "RingingTone2",
    "FUNCTIONALITY",
    Setup0L,
    GetRingingTone2TestL,
    Teardown)

EUNIT_TEST(
    "Get Message Alert Tone 0 - test",
    "MProEngTones",
    "MessageAlertTone",
    "FUNCTIONALITY",
    Setup0L,
    GetMessageAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Get Email Alert Tone 0 - test",
    "MProEngTones",
    "EmailAlertTone",
    "FUNCTIONALITY",
    Setup0L,
    GetEmailAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Get Video Call Ringing Tone 0 - test",
    "MProEngTones",
    "VideoCallRingingTone",
    "FUNCTIONALITY",
    Setup0L,
    GetVideoCallRingingToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Line1 Ringing Tone 0 - test",
    "MProEngTones",
    "SetRingingTone1L",
    "FUNCTIONALITY",
    Setup0L,
    SetRingingTone1TestL,
    Teardown)

EUNIT_TEST(
    "Set Line2 Ringing Tone 0 - test",
    "MProEngTones",
    "SetRingingTone2",
    "FUNCTIONALITY",
    Setup0L,
    SetRingingTone2TestL,
    Teardown)

EUNIT_TEST(
    "Set Message Alert Tone 0 - test",
    "MProEngTones",
    "SetMessageAlertToneL",
    "FUNCTIONALITY",
    Setup0L,
    SetMessageAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Email Alert Tone 0 - test",
    "MProEngTones",
    "SetEmailAlertToneL",
    "FUNCTIONALITY",
    Setup0L,
    SetEmailAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Video Call Ringing Tone 0 - test",
    "MProEngTones",
    "SetVideoCallRingingToneL",
    "FUNCTIONALITY",
    Setup0L,
    SetVideoCallRingingToneTestL,
    Teardown)

EUNIT_TEST(
    "Get Line1 Ringing Tone 1 - test",
    "MProEngTones",
    "RingingTone1",
    "FUNCTIONALITY",
    Setup1L,
    GetRingingTone1TestL,
    Teardown)

EUNIT_TEST(
    "Get Line2 Ringing Tone 1 - test",
    "MProEngTones",
    "RingingTone2",
    "FUNCTIONALITY",
    Setup1L,
    GetRingingTone2TestL,
    Teardown)

EUNIT_TEST(
    "Get Message Alert Tone 1 - test",
    "MProEngTones",
    "MessageAlertTone",
    "FUNCTIONALITY",
    Setup1L,
    GetMessageAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Get Email Alert Tone 1 - test",
    "MProEngTones",
    "EmailAlertTone",
    "FUNCTIONALITY",
    Setup1L,
    GetEmailAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Get Video Call Ringing Tone 1 - test",
    "MProEngTones",
    "VideoCallRingingTone",
    "FUNCTIONALITY",
    Setup1L,
    GetVideoCallRingingToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Line1 Ringing Tone 1 - test",
    "MProEngTones",
    "SetRingingTone1L",
    "FUNCTIONALITY",
    Setup1L,
    SetRingingTone1TestL,
    Teardown)

EUNIT_TEST(
    "Set Line2 Ringing Tone 1 - test",
    "MProEngTones",
    "SetRingingTone2",
    "FUNCTIONALITY",
    Setup1L,
    SetRingingTone2TestL,
    Teardown)

EUNIT_TEST(
    "Set Message Alert Tone 1 - test",
    "MProEngTones",
    "SetMessageAlertToneL",
    "FUNCTIONALITY",
    Setup1L,
    SetMessageAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Email Alert Tone 1 - test",
    "MProEngTones",
    "SetEmailAlertToneL",
    "FUNCTIONALITY",
    Setup1L,
    SetEmailAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Video Call Ringing Tone 1 - test",
    "MProEngTones",
    "SetVideoCallRingingToneL",
    "FUNCTIONALITY",
    Setup1L,
    SetVideoCallRingingToneTestL,
    Teardown)

EUNIT_TEST(
    "Get Line1 Ringing Tone 2 - test",
    "MProEngTones",
    "RingingTone1",
    "FUNCTIONALITY",
    Setup2L,
    GetRingingTone1TestL,
    Teardown)

EUNIT_TEST(
    "Get Line2 Ringing Tone 2 - test",
    "MProEngTones",
    "RingingTone2",
    "FUNCTIONALITY",
    Setup2L,
    GetRingingTone2TestL,
    Teardown)

EUNIT_TEST(
    "Get Message Alert Tone 2 - test",
    "MProEngTones",
    "MessageAlertTone",
    "FUNCTIONALITY",
    Setup2L,
    GetMessageAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Get Email Alert Tone 2 - test",
    "MProEngTones",
    "EmailAlertTone",
    "FUNCTIONALITY",
    Setup2L,
    GetEmailAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Get Video Call Ringing Tone 2 - test",
    "MProEngTones",
    "VideoCallRingingTone",
    "FUNCTIONALITY",
    Setup2L,
    GetVideoCallRingingToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Line1 Ringing Tone 2 - test",
    "MProEngTones",
    "SetRingingTone1L",
    "FUNCTIONALITY",
    Setup2L,
    SetRingingTone1TestL,
    Teardown)

EUNIT_TEST(
    "Set Line2 Ringing Tone 2 - test",
    "MProEngTones",
    "SetRingingTone2",
    "FUNCTIONALITY",
    Setup2L,
    SetRingingTone2TestL,
    Teardown)

EUNIT_TEST(
    "Set Message Alert Tone 2 - test",
    "MProEngTones",
    "SetMessageAlertToneL",
    "FUNCTIONALITY",
    Setup2L,
    SetMessageAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Email Alert Tone 2 - test",
    "MProEngTones",
    "SetEmailAlertToneL",
    "FUNCTIONALITY",
    Setup2L,
    SetEmailAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Video Call Ringing Tone 2 - test",
    "MProEngTones",
    "SetVideoCallRingingToneL",
    "FUNCTIONALITY",
    Setup2L,
    SetVideoCallRingingToneTestL,
    Teardown)

EUNIT_TEST(
    "Get Line1 Ringing Tone 3 - test",
    "MProEngTones",
    "RingingTone1",
    "FUNCTIONALITY",
    Setup3L,
    GetRingingTone1TestL,
    Teardown)

EUNIT_TEST(
    "Get Line2 Ringing Tone 3 - test",
    "MProEngTones",
    "RingingTone2",
    "FUNCTIONALITY",
    Setup3L,
    GetRingingTone2TestL,
    Teardown)

EUNIT_TEST(
    "Get Message Alert Tone 3 - test",
    "MProEngTones",
    "MessageAlertTone",
    "FUNCTIONALITY",
    Setup3L,
    GetMessageAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Get Email Alert Tone 3 - test",
    "MProEngTones",
    "EmailAlertTone",
    "FUNCTIONALITY",
    Setup3L,
    GetEmailAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Get Video Call Ringing Tone 3 - test",
    "MProEngTones",
    "VideoCallRingingTone",
    "FUNCTIONALITY",
    Setup3L,
    GetVideoCallRingingToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Line1 Ringing Tone 3 - test",
    "MProEngTones",
    "SetRingingTone1L",
    "FUNCTIONALITY",
    Setup3L,
    SetRingingTone1TestL,
    Teardown)

EUNIT_TEST(
    "Set Line2 Ringing Tone 3 - test",
    "MProEngTones",
    "SetRingingTone2",
    "FUNCTIONALITY",
    Setup3L,
    SetRingingTone2TestL,
    Teardown)

EUNIT_TEST(
    "Set Message Alert Tone 3 - test",
    "MProEngTones",
    "SetMessageAlertToneL",
    "FUNCTIONALITY",
    Setup3L,
    SetMessageAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Email Alert Tone 3 - test",
    "MProEngTones",
    "SetEmailAlertToneL",
    "FUNCTIONALITY",
    Setup3L,
    SetEmailAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Video Call Ringing Tone 3 - test",
    "MProEngTones",
    "SetVideoCallRingingToneL",
    "FUNCTIONALITY",
    Setup3L,
    SetVideoCallRingingToneTestL,
    Teardown)

EUNIT_TEST(
    "Get Line1 Ringing Tone 4 - test",
    "MProEngTones",
    "RingingTone1",
    "FUNCTIONALITY",
    Setup4L,
    GetRingingTone1TestL,
    Teardown)

EUNIT_TEST(
    "Get Line2 Ringing Tone 4 - test",
    "MProEngTones",
    "RingingTone2",
    "FUNCTIONALITY",
    Setup4L,
    GetRingingTone2TestL,
    Teardown)

EUNIT_TEST(
    "Get Message Alert Tone 4 - test",
    "MProEngTones",
    "MessageAlertTone",
    "FUNCTIONALITY",
    Setup4L,
    GetMessageAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Get Email Alert Tone 4 - test",
    "MProEngTones",
    "EmailAlertTone",
    "FUNCTIONALITY",
    Setup4L,
    GetEmailAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Get Video Call Ringing Tone 4 - test",
    "MProEngTones",
    "VideoCallRingingTone",
    "FUNCTIONALITY",
    Setup4L,
    GetVideoCallRingingToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Line1 Ringing Tone 4 - test",
    "MProEngTones",
    "SetRingingTone1L",
    "FUNCTIONALITY",
    Setup4L,
    SetRingingTone1TestL,
    Teardown)

EUNIT_TEST(
    "Set Line2 Ringing Tone 4 - test",
    "MProEngTones",
    "SetRingingTone2",
    "FUNCTIONALITY",
    Setup4L,
    SetRingingTone2TestL,
    Teardown)

EUNIT_TEST(
    "Set Message Alert Tone 4 - test",
    "MProEngTones",
    "SetMessageAlertToneL",
    "FUNCTIONALITY",
    Setup4L,
    SetMessageAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Email Alert Tone 4 - test",
    "MProEngTones",
    "SetEmailAlertToneL",
    "FUNCTIONALITY",
    Setup4L,
    SetEmailAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Video Call Ringing Tone 4 - test",
    "MProEngTones",
    "SetVideoCallRingingToneL",
    "FUNCTIONALITY",
    Setup4L,
    SetVideoCallRingingToneTestL,
    Teardown)

#ifdef __OFFLINE_MODE
EUNIT_TEST(
    "Get Line1 Ringing Tone 5 - test",
    "MProEngTones",
    "RingingTone1",
    "FUNCTIONALITY",
    Setup5L,
    GetRingingTone1TestL,
    Teardown)

EUNIT_TEST(
    "Get Line2 Ringing Tone 5 - test",
    "MProEngTones",
    "RingingTone2",
    "FUNCTIONALITY",
    Setup5L,
    GetRingingTone2TestL,
    Teardown)

EUNIT_TEST(
    "Get Message Alert Tone 5 - test",
    "MProEngTones",
    "MessageAlertTone",
    "FUNCTIONALITY",
    Setup5L,
    GetMessageAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Get Email Alert Tone 5 - test",
    "MProEngTones",
    "EmailAlertTone",
    "FUNCTIONALITY",
    Setup5L,
    GetEmailAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Get Video Call Ringing Tone 5 - test",
    "MProEngTones",
    "VideoCallRingingTone",
    "FUNCTIONALITY",
    Setup5L,
    GetVideoCallRingingToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Line1 Ringing Tone 5 - test",
    "MProEngTones",
    "SetRingingTone1L",
    "FUNCTIONALITY",
    Setup5L,
    SetRingingTone1TestL,
    Teardown)

EUNIT_TEST(
    "Set Line2 Ringing Tone 5 - test",
    "MProEngTones",
    "SetRingingTone2",
    "FUNCTIONALITY",
    Setup5L,
    SetRingingTone2TestL,
    Teardown)

EUNIT_TEST(
    "Set Message Alert Tone 5 - test",
    "MProEngTones",
    "SetMessageAlertToneL",
    "FUNCTIONALITY",
    Setup5L,
    SetMessageAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Email Alert Tone 5 - test",
    "MProEngTones",
    "SetEmailAlertToneL",
    "FUNCTIONALITY",
    Setup5L,
    SetEmailAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Video Call Ringing Tone 5 - test",
    "MProEngTones",
    "SetVideoCallRingingToneL",
    "FUNCTIONALITY",
    Setup5L,
    SetVideoCallRingingToneTestL,
    Teardown)
#endif // __OFFLINE_MODE

#ifdef __DRIVE_MODE
EUNIT_TEST(
    "Get Line1 Ringing Tone 6 - test",
    "MProEngTones",
    "RingingTone1",
    "FUNCTIONALITY",
    Setup6L,
    GetRingingTone1TestL,
    Teardown)

EUNIT_TEST(
    "Get Line2 Ringing Tone 6 - test",
    "MProEngTones",
    "RingingTone2",
    "FUNCTIONALITY",
    Setup6L,
    GetRingingTone2TestL,
    Teardown)

EUNIT_TEST(
    "Get Message Alert Tone 6 - test",
    "MProEngTones",
    "MessageAlertTone",
    "FUNCTIONALITY",
    Setup6L,
    GetMessageAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Get Email Alert Tone 6 - test",
    "MProEngTones",
    "EmailAlertTone",
    "FUNCTIONALITY",
    Setup6L,
    GetEmailAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Get Video Call Ringing Tone 6 - test",
    "MProEngTones",
    "VideoCallRingingTone",
    "FUNCTIONALITY",
    Setup6L,
    GetVideoCallRingingToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Line1 Ringing Tone 6 - test",
    "MProEngTones",
    "SetRingingTone1L",
    "FUNCTIONALITY",
    Setup6L,
    SetRingingTone1TestL,
    Teardown)

EUNIT_TEST(
    "Set Line2 Ringing Tone 6 - test",
    "MProEngTones",
    "SetRingingTone2",
    "FUNCTIONALITY",
    Setup6L,
    SetRingingTone2TestL,
    Teardown)

EUNIT_TEST(
    "Set Message Alert Tone 6 - test",
    "MProEngTones",
    "SetMessageAlertToneL",
    "FUNCTIONALITY",
    Setup6L,
    SetMessageAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Email Alert Tone 6 - test",
    "MProEngTones",
    "SetEmailAlertToneL",
    "FUNCTIONALITY",
    Setup6L,
    SetEmailAlertToneTestL,
    Teardown)

EUNIT_TEST(
    "Set Video Call Ringing Tone 6 - test",
    "MProEngTones",
    "SetVideoCallRingingToneL",
    "FUNCTIONALITY",
    Setup6L,
    SetVideoCallRingingToneTestL,
    Teardown)
#endif // __DRIVE_MODE

/*
EUNIT_TEST(
    "DRM Set Line1 Ringing Tone 0 - test",
    "MProEngTones",
    "SetRingingTone1L",
    "FUNCTIONALITY",
    Setup0L,
    SetDRMRingingTone1TestL,
    Teardown)
*/
/*
EUNIT_TEST(
    "FileSizeLimit, Line1 Set Ringing Tone 0 - test",
    "MProEngTones",
    "SetRingingToneSizeLimitL",
    "FUNCTIONALITY",
    Setup0L,
    SetRingingToneSizeLimitTestL,
    Teardown)
*/
EUNIT_END_TEST_TABLE


