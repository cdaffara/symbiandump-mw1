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
* Description:  Implementation of the MT_MProEngNotifyHandler test class.
*
*/

////////////////////////////////////////////////////////////////////////
// MT_MProEngNotifyHandler.cpp
//

//  CLASS HEADER
#include "MT_MProEngNotifyHandler.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <MProEngNotifyHandler.h>
#include <MProEngProfile.h>
#include <MProEngEngine.h>
#include <MProEngTones.h>
#include <MProEngProfileName.h>
#include <MProEngToneSettings.h>
#include <ProEngFactory.h>
#include <Profile.hrh>
#include <CPbkContactEngine.h>
#include <centralrepository.h>
#include <ProfileEngineSDKCRKeys.h>


//  INTERNAL INCLUDES
#include "T_ProfileTestTools.h"

// CONSTANTS
namespace
    {
    const TInt KProEngUndefinedError = -999;
    }

// CONSTRUCTION
MT_MProEngNotifyHandler* MT_MProEngNotifyHandler::NewL()
    {
    MT_MProEngNotifyHandler* self = MT_MProEngNotifyHandler::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

MT_MProEngNotifyHandler* MT_MProEngNotifyHandler::NewLC()
    {
    MT_MProEngNotifyHandler* self = new( ELeave ) MT_MProEngNotifyHandler();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
MT_MProEngNotifyHandler::~MT_MProEngNotifyHandler()
    {
    if( iProfile )
        {
        iProfile->Release();
        }
    if( iEngine )
        {
        iEngine->Release();
        }
    delete iActiveIdObserver;
    iActiveIdObserver = NULL;
    delete iActiveProfileObserver;
    iActiveProfileObserver = NULL;
    delete iProfileObserver;
    iProfileObserver = NULL;
    delete iNameArrayObserver;
    iNameArrayObserver = NULL;
    MT_CSchedulerUtility::Release();
    iFs.Close();
    delete iDebugObs;
    }

// Default constructor
MT_MProEngNotifyHandler::MT_MProEngNotifyHandler()
    {
    }

// Second phase construct
void MT_MProEngNotifyHandler::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    User::LeaveIfError( iFs.Connect() );
    iEngine = ProEngFactory::NewEngineL();
    MT_CSchedulerUtility::InstanceL();
    iDebugObs = CMyDebugObserver::NewL(); 
    }

//  METHODS


// Setup methods:
//

void MT_MProEngNotifyHandler::SetupCancelAllTestL()
    {
    iHandler = ProEngFactory::NewNotifyHandlerL();
    iActiveIdObserver = MT_CProEngActiveIdObserver::NewL();
    iActiveProfileObserver = MT_CProEngActiveProfileObserver::NewL();
    iNameArrayObserver = MT_CProEngNameArrayObserver::NewL();
    iProfileObserver = MT_CProEngProfileObserver::NewL();
    iProfileId = 0;
    }

void MT_MProEngNotifyHandler::SetupRequestProfileActivationNotificationsTestL()
    {
    iHandler = ProEngFactory::NewNotifyHandlerL();
    iActiveIdObserver = MT_CProEngActiveIdObserver::NewL();
    } 

void MT_MProEngNotifyHandler::SetupRequestActiveProfileNotificationsTestL(  )
    {
    iHandler = ProEngFactory::NewNotifyHandlerL();
    iActiveProfileObserver = MT_CProEngActiveProfileObserver::NewL();
    iProfileId = iEngine->ActiveProfileId();
    iProfile = iEngine->ActiveProfileL();
    } 

void MT_MProEngNotifyHandler::SetupRequestProfileNotificationsTestL()
    {
    iHandler = ProEngFactory::NewNotifyHandlerL();
    iProfileObserver = MT_CProEngProfileObserver::NewL();
    iProfile = iEngine->ProfileL( iProfileId );
    iContactEngine = CPbkContactEngine::NewL( &iFs );
    T_ProfileTestTools::CreateContactGroupsL( 5, iContactGroups, *iContactEngine );
    }

void MT_MProEngNotifyHandler::SetupRequestProfileNotificationsTest0L()
    {
    iProfileId = 0;
    SetupRequestProfileNotificationsTestL();
    } 

void MT_MProEngNotifyHandler::SetupRequestProfileNotificationsTest1L()
    {
    iProfileId = 1;
    SetupRequestProfileNotificationsTestL();
    } 

void MT_MProEngNotifyHandler::SetupRequestProfileNotificationsTest2L()
    {
    iProfileId = 2;
    SetupRequestProfileNotificationsTestL();
    } 

void MT_MProEngNotifyHandler::SetupRequestProfileNotificationsTest3L()
    {
    iProfileId = 3;
    SetupRequestProfileNotificationsTestL();
    } 

void MT_MProEngNotifyHandler::SetupRequestProfileNotificationsTest4L()
    {
    iProfileId = 4;
    SetupRequestProfileNotificationsTestL();
    } 

void MT_MProEngNotifyHandler::SetupRequestProfileNotificationsTest5L()
    {
    iProfileId = 5;
    SetupRequestProfileNotificationsTestL();
    } 

void MT_MProEngNotifyHandler::SetupRequestProfileNameArrayNotificationsTestL(  )
    {
    iDebugObs->NotifyReqL();
    iHandler = ProEngFactory::NewNotifyHandlerL();
    iNameArrayObserver = MT_CProEngNameArrayObserver::NewL();
    iProfile = iEngine->ProfileL( iProfileId );
    } 

void MT_MProEngNotifyHandler::SetupRequestProfileNameArrayNotificationsTest0L(  )
    {
    iProfileId = 0;
    SetupRequestProfileNameArrayNotificationsTestL();
    } 

void MT_MProEngNotifyHandler::SetupRequestProfileNameArrayNotificationsTest1L(  )
    {
    iProfileId = 1;
    SetupRequestProfileNameArrayNotificationsTestL();
    } 

void MT_MProEngNotifyHandler::SetupRequestProfileNameArrayNotificationsTest2L(  )
    {
    iProfileId = 2;
    SetupRequestProfileNameArrayNotificationsTestL();
    } 

void MT_MProEngNotifyHandler::SetupRequestProfileNameArrayNotificationsTest3L(  )
    {
    iProfileId = 3;
    SetupRequestProfileNameArrayNotificationsTestL();
    } 

void MT_MProEngNotifyHandler::SetupRequestProfileNameArrayNotificationsTest4L(  )
    {
    iProfileId = 4;
    SetupRequestProfileNameArrayNotificationsTestL();
    } 

void MT_MProEngNotifyHandler::SetupRequestProfileNameArrayNotificationsTest5L(  )
    {
    iProfileId = 5;
    SetupRequestProfileNameArrayNotificationsTestL();
    } 

void MT_MProEngNotifyHandler::SetupL(  )
    {
    }

/// Teardown functions:
//

void MT_MProEngNotifyHandler::TeardownCommon(  )
    {
    delete iHandler;
    iHandler = NULL;

    if( iProfile )
        {
        iProfile->Release();
        iProfile = NULL;
        }
    T_ProfileTestTools::ResetRepository();
    }

void MT_MProEngNotifyHandler::TeardownRequestProfileActivationNotificationsTest(  )
    {
    iHandler->CancelProfileActivationNotifications();
    delete iActiveIdObserver;
    iActiveIdObserver = NULL;

    delete iHandler;
    iHandler = NULL;

    T_ProfileTestTools::ResetRepository();
    }

void MT_MProEngNotifyHandler::TeardownRequestActiveProfileNotificationsTest(  )
    {
    iHandler->CancelActiveProfileNotifications();
    delete iActiveProfileObserver;
    iActiveProfileObserver = NULL;
    TeardownCommon();
    T_ProfileTestTools::ResetRepository();
    }

void MT_MProEngNotifyHandler::TeardownRequestProfileNotificationsTest(  )
    {
    iHandler->CancelProfileNotifications( iProfileId );
    delete iProfileObserver;
    iProfileObserver = NULL;
    TeardownCommon();
    TRAP_IGNORE( T_ProfileTestTools::DeleteContactGroups(
                iContactGroups, *iContactEngine ) );
    delete iContactEngine;
    iContactGroups.Close();
    }

void MT_MProEngNotifyHandler::TeardownRequestProfileNameArrayNotificationsTest(  )
    {
    iDebugObs->Cancel();
    iHandler->CancelProfileNameArrayNotifications();
    delete iNameArrayObserver;
    iNameArrayObserver = NULL;
    TeardownCommon();
    }

void MT_MProEngNotifyHandler::TeardownCancelAllTest(  )
    {
    delete iActiveIdObserver;
    iActiveIdObserver = NULL;
    delete iActiveProfileObserver;
    iActiveProfileObserver = NULL;
    delete iProfileObserver;
    iProfileObserver = NULL;
    delete iNameArrayObserver;
    iNameArrayObserver = NULL;

    delete iHandler;
    iHandler = NULL;
    }

/// Test functions
//

void MT_MProEngNotifyHandler::RequestProfileActivationNotificationsTestL(  )
    {
    TInt result( iHandler->RequestProfileActivationNotificationsL(
                 *iActiveIdObserver ) );
    EUNIT_ASSERT( result == KErrNone );
    iActiveIdObserver->iProfileId = 0;
    iActiveIdObserver->iError = KProEngUndefinedError;
    iEngine->SetActiveProfileL( 2 );
    MT_CSchedulerUtility::InstanceL()->Start();
    EUNIT_ASSERT( iActiveIdObserver->iProfileId == 2 );
    EUNIT_ASSERT( iActiveIdObserver->iError == KErrNone );
    iHandler->CancelProfileActivationNotifications();
    }

void MT_MProEngNotifyHandler::RequestActiveProfileNotificationsTestL(  )
    {
    TInt result( iHandler->RequestActiveProfileNotificationsL(
                 *iActiveProfileObserver ) );
    EUNIT_ASSERT( result == KErrNone );
    iActiveProfileObserver->iNotified = EFalse;
    iActiveProfileObserver->iError = KProEngUndefinedError;
    MProEngToneSettings& ts = iProfile->ToneSettings();
    ts.SetRingingVolume( EProfileRingingVolumeLevel5 );
    iProfile->CommitChangeL();
    MT_CSchedulerUtility::InstanceL()->Start();
    EUNIT_ASSERT( iActiveProfileObserver->iNotified );
    EUNIT_ASSERT( iActiveProfileObserver->iError == KErrNone );
    iHandler->CancelActiveProfileNotifications();
    }

void MT_MProEngNotifyHandler::RequestProfileNotificationsTestL(  )
    {
    EUNIT_PRINT( _L( "RequestProfileNotificationsTestL before req" ) );
    TInt result( iHandler->RequestProfileNotificationsL(
                 *iProfileObserver, iProfileId ) );
    EUNIT_PRINT( _L( "RequestProfileNotificationsTestL result=%d" ), result );
    EUNIT_PRINT( _L( "RequestProfileNotificationsTestL after req" ) );
#ifndef __OFFLINE_MODE
    if( iProfileId == EProfileOffLineId )
        {
        EUNIT_ASSERT( result == KErrNotFound );
        return;
        }
#endif // __OFFLINE_MODE
    EUNIT_ASSERT( result == KErrNone );
    MProEngTones& tones = iProfile->ProfileTones();
    MProEngToneSettings& ts = iProfile->ToneSettings();

    if( ( iProfileId != EProfileOffLineId ) )
        {
        iProfileObserver->iProfileId = -1;
        iProfileObserver->iError = KProEngUndefinedError;

        EUNIT_PRINT( _L( "ReqNotificationsTestL rtype before set:%d" ), ts.RingingType() );
        User::LeaveIfError( ts.SetRingingType( EProfileRingingTypeAscending ) );
        EUNIT_PRINT( _L( "ReqNotificationsTestL rtype after set:%d" ), ts.RingingType() );
        iProfile->CommitChangeL();
        MT_CSchedulerUtility::InstanceL()->Start();
        EUNIT_PRINT( _L( "ReqNotificationsTestL observer->iProfileId:%d" ), iProfileObserver->iProfileId );
        EUNIT_PRINT( _L( "ReqNotificationsTestL iProfileId:%d" ), iProfileId );
        EUNIT_ASSERT( iProfileObserver->iProfileId == iProfileId );
        EUNIT_ASSERT( iProfileObserver->iError == KErrNone );
        }

    iProfileObserver->iProfileId = -1;
    iProfileObserver->iError = KProEngUndefinedError;

    EUNIT_PRINT( _L( "ReqNotificationsTestL keyvol before set:%d" ), ts.KeypadVolume() );
    if( ( iProfileId == 4 ) || ( iProfileId == 5 ) )
        {
        User::LeaveIfError( ts.SetKeypadVolume( EProfileKeypadVolumeLevel1 ) );
        }
    else
        {
        User::LeaveIfError( ts.SetKeypadVolume( EProfileKeypadVolumeLevel2 ) );
        }
    EUNIT_PRINT( _L( "ReqNotificationsTestL keyvol after set:%d" ), ts.KeypadVolume() );
    iProfile->CommitChangeL();
    MT_CSchedulerUtility::InstanceL()->Start();
    EUNIT_ASSERT( iProfileObserver->iProfileId == iProfileId );
    EUNIT_ASSERT( iProfileObserver->iError == KErrNone );

    iProfileObserver->iProfileId = -1;
    iProfileObserver->iError = KProEngUndefinedError;

    EUNIT_PRINT( _L( "ReqNotificationsTestL rvol before set:%d" ), ts.RingingVolume() );
    User::LeaveIfError( ts.SetRingingVolume( EProfileRingingVolumeLevel5 ) );
    EUNIT_PRINT( _L( "ReqNotificationsTestL rvol after set:%d" ), ts.RingingVolume() );
    iProfile->CommitChangeL();
    EUNIT_PRINT( _L( "ReqNotificationsTestL rvol after CommitChangeL" ) );
    MT_CSchedulerUtility::InstanceL()->Start();
    EUNIT_ASSERT( iProfileObserver->iProfileId == iProfileId );
    EUNIT_ASSERT( iProfileObserver->iError == KErrNone );

    iProfileObserver->iProfileId = -1;
    iProfileObserver->iError = KProEngUndefinedError;

    EUNIT_PRINT( _L( "ReqNotificationsTestL before vibra" ) );
    if( ( iProfileId == EProfileOutdoorId ) || ( iProfileId == EProfilePagerId ) )
        {
        User::LeaveIfError( ts.SetVibratingAlert( EFalse ) );
        }
    else
        {
        User::LeaveIfError( ts.SetVibratingAlert( ETrue ) );
        }
    EUNIT_PRINT( _L( "ReqNotificationsTestL after vibra" ) );
    iProfile->CommitChangeL();
    EUNIT_PRINT( _L( "ReqNotificationsTestL after vibra commit" ) );
    MT_CSchedulerUtility::InstanceL()->Start();
    EUNIT_ASSERT( iProfileObserver->iProfileId == iProfileId );
    EUNIT_ASSERT( iProfileObserver->iError == KErrNone );

    iProfileObserver->iProfileId = -1;
    iProfileObserver->iError = KProEngUndefinedError;

    EUNIT_PRINT( _L( "ReqNotificationsTestL before warn&games" ) );
    if( ( iProfileId == EProfileSilentId ) || ( iProfileId == EProfileMeetingId ) )
        {
        User::LeaveIfError( ts.SetWarningAndGameTones( ETrue ) );
        }
    else
        {
        User::LeaveIfError( ts.SetWarningAndGameTones( EFalse ) );
        }
    EUNIT_PRINT( _L( "ReqNotificationsTestL after warn&games" ) );
    iProfile->CommitChangeL();
    EUNIT_PRINT( _L( "ReqNotificationsTestL after warn&games commit" ) );
    MT_CSchedulerUtility::InstanceL()->Start();
    EUNIT_ASSERT( iProfileObserver->iProfileId == iProfileId );
    EUNIT_ASSERT( iProfileObserver->iError == KErrNone );

    if( ( iProfileId != EProfileOffLineId ) )
        {
        iProfileObserver->iProfileId = -1;
        iProfileObserver->iError = KProEngUndefinedError;

        EUNIT_PRINT( _L( "ReqNotificationsTestL before tts" ) );
        User::LeaveIfError( ts.SetTextToSpeech( ETrue ) );
        EUNIT_PRINT( _L( "ReqNotificationsTestL after tts" ) );
        iProfile->CommitChangeL();
        EUNIT_PRINT( _L( "ReqNotificationsTestL after tts commit" ) );
        MT_CSchedulerUtility::InstanceL()->Start();
        EUNIT_ASSERT( iProfileObserver->iProfileId == iProfileId );
        EUNIT_ASSERT( iProfileObserver->iError == KErrNone );

        iProfileObserver->iProfileId = -1;
        iProfileObserver->iError = KProEngUndefinedError;

        EUNIT_PRINT( _L( "ReqNotificationsTestL before rtone1" ) );
        User::LeaveIfError( tones.SetRingingTone1L( KProEngMTTestTone ) );
        EUNIT_PRINT( _L( "ReqNotificationsTestL after rtone1" ) );
        iProfile->CommitChangeL();
        EUNIT_PRINT( _L( "ReqNotificationsTestL after rtone1 commit" ) );
        MT_CSchedulerUtility::InstanceL()->Start();
        EUNIT_ASSERT( iProfileObserver->iProfileId == iProfileId );
        EUNIT_ASSERT( iProfileObserver->iError == KErrNone );

        iProfileObserver->iProfileId = -1;
        iProfileObserver->iError = KProEngUndefinedError;

        EUNIT_PRINT( _L( "ReqNotificationsTestL before rtone2" ) );
        User::LeaveIfError( tones.SetRingingTone2L( KProEngMTTestTone ) );
        EUNIT_PRINT( _L( "ReqNotificationsTestL after rtone2" ) );
        iProfile->CommitChangeL();
        EUNIT_PRINT( _L( "ReqNotificationsTestL after rtone2 commit" ) );
        iProfile->CommitChangeL();
        MT_CSchedulerUtility::InstanceL()->Start();
        EUNIT_ASSERT( iProfileObserver->iProfileId == iProfileId );
        EUNIT_ASSERT( iProfileObserver->iError == KErrNone );
        }

    iProfileObserver->iProfileId = -1;
    iProfileObserver->iError = KProEngUndefinedError;

    EUNIT_PRINT( _L( "ReqNotificationsTestL before msgtone" ) );
    User::LeaveIfError( tones.SetMessageAlertToneL( KProEngMTTestTone ) );
    EUNIT_PRINT( _L( "ReqNotificationsTestL after msgtone" ) );
    iProfile->CommitChangeL();
    EUNIT_PRINT( _L( "ReqNotificationsTestL after msgtone commit" ) );
    MT_CSchedulerUtility::InstanceL()->Start();
    EUNIT_ASSERT( iProfileObserver->iProfileId == iProfileId );
    EUNIT_ASSERT( iProfileObserver->iError == KErrNone );

    if( ( iProfileId != EProfileOffLineId ) )
        {
        iProfileObserver->iProfileId = -1;
        iProfileObserver->iError = KProEngUndefinedError;

        EUNIT_PRINT( _L( "ReqNotificationsTestL before email" ) );
        User::LeaveIfError( tones.SetEmailAlertToneL( KProEngMTTestTone ) );
        EUNIT_PRINT( _L( "ReqNotificationsTestL after email" ) );
        iProfile->CommitChangeL();
        EUNIT_PRINT( _L( "ReqNotificationsTestL after email commit" ) );
        MT_CSchedulerUtility::InstanceL()->Start();
        EUNIT_ASSERT( iProfileObserver->iProfileId == iProfileId );
        EUNIT_ASSERT( iProfileObserver->iError == KErrNone );

        iProfileObserver->iProfileId = -1;
        iProfileObserver->iError = KProEngUndefinedError;

        EUNIT_PRINT( _L( "ReqNotificationsTestL before video" ) );
        User::LeaveIfError( tones.SetVideoCallRingingToneL( KProEngMTTestTone ) );
        EUNIT_PRINT( _L( "ReqNotificationsTestL after video" ) );
        iProfile->CommitChangeL();
        EUNIT_PRINT( _L( "ReqNotificationsTestL after video commit" ) );
        MT_CSchedulerUtility::InstanceL()->Start();
        EUNIT_ASSERT( iProfileObserver->iProfileId == iProfileId );
        EUNIT_ASSERT( iProfileObserver->iError == KErrNone );
        }

    iHandler->CancelProfileNotifications( iProfileId );
    }

void MT_MProEngNotifyHandler::RequestProfileNameArrayNotificationsTestL(  )
    {
    EUNIT_PRINT( _L( "CMyDebugObserver::ReqNameArrayNot begin" ) );
    TInt result( iHandler->RequestProfileNameArrayNotificationsL(
                 *iNameArrayObserver ) );
    EUNIT_PRINT( _L( "RequestNameArrayNotif result=%d" ), result );
    EUNIT_ASSERT( result == KErrNone );
    iNameArrayObserver->iNotified = EFalse;
    iNameArrayObserver->iError = KProEngUndefinedError;
    MProEngProfileName& name = iProfile->ProfileName();
    const TDesC& n( name.Name() );
    const TDesC& t( KProEngMTTestName() );
    EUNIT_PRINT( _L( "RequestNameArrayNotif KProEngMTTestName=%S" ), &t );
    EUNIT_PRINT( _L( "RequestNameArrayNotif name before set:%S" ), &n );
    User::LeaveIfError( name.SetNameL( KProEngMTTestName ) );
    const TDesC& n2( name.Name() );
    EUNIT_PRINT( _L( "RequestNameArrayNotif name after set:%S" ), &n2 );
    iProfile->CommitChangeL();
    MT_CSchedulerUtility::InstanceL()->Start();
    EUNIT_ASSERT( iNameArrayObserver->iNotified );
    EUNIT_ASSERT( iNameArrayObserver->iError == KErrNone );
    iHandler->CancelProfileNameArrayNotifications();
    }

void MT_MProEngNotifyHandler::CancelProfileActivationNotificationsTestL(  )
    {
    __UHEAP_MARK;
    TInt result( iHandler->RequestProfileActivationNotificationsL(
                 *iActiveIdObserver ) );
    EUNIT_ASSERT( result == KErrNone );
    iHandler->CancelProfileActivationNotifications();
    EUNIT_ASSERT( ETrue );
    __UHEAP_MARKEND;
    }

void MT_MProEngNotifyHandler::CancelActiveProfileNotificationsTestL(  )
    {
    __UHEAP_MARK;
    TInt result( iHandler->RequestActiveProfileNotificationsL(
                 *iActiveProfileObserver ) );
    EUNIT_ASSERT( result == KErrNone );
    iHandler->CancelActiveProfileNotifications();
    EUNIT_ASSERT( ETrue );
    __UHEAP_MARKEND;
    }

void MT_MProEngNotifyHandler::CancelProfileNotificationsTestL(  )
    {
    TInt result( iHandler->RequestProfileNotificationsL(
                 *iProfileObserver, iProfileId ) );
    EUNIT_ASSERT( result == KErrNone );
    iHandler->CancelProfileNotifications( iProfileId );
    EUNIT_ASSERT( ETrue );
    }

void MT_MProEngNotifyHandler::CancelProfileNameArrayNotificationsTestL(  )
    {
    __UHEAP_MARK;
    TInt result( iHandler->RequestProfileNameArrayNotificationsL(
                 *iNameArrayObserver ) );
    EUNIT_ASSERT( result == KErrNone );
    iHandler->CancelProfileNameArrayNotifications();
    EUNIT_ASSERT( ETrue );
    __UHEAP_MARKEND;
    }

void MT_MProEngNotifyHandler::CancelAllTestL(  )
    {
    TInt result( iHandler->RequestProfileActivationNotificationsL(
                 *iActiveIdObserver ) );
    EUNIT_ASSERT( result == KErrNone );
    result = iHandler->RequestActiveProfileNotificationsL(
                 *iActiveProfileObserver );
    EUNIT_ASSERT( result == KErrNone );
    result = iHandler->RequestProfileNotificationsL(
                 *iProfileObserver, iProfileId );
    EUNIT_ASSERT( result == KErrNone );
    result = iHandler->RequestProfileNameArrayNotificationsL(
                 *iNameArrayObserver );
    EUNIT_ASSERT( result == KErrNone );
    iHandler->CancelAll();
    EUNIT_ASSERT( ETrue );
    }

CMyDebugObserver::CMyDebugObserver()
    : CActive( EPriorityStandard )
    {
    }

CMyDebugObserver::~CMyDebugObserver()
    {
    delete iRep;
    }

CMyDebugObserver* CMyDebugObserver::NewL()
    {
    CMyDebugObserver* self = new ( ELeave ) CMyDebugObserver();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

void CMyDebugObserver::RunL()
    {
    NotifyReqL();
    EUNIT_PRINT( _L( "CMyDebugObserver::RunL: iStatus=%u" ), iStatus.Int() );
    }

TInt CMyDebugObserver::RunError( TInt aError )
    {
    EUNIT_PRINT( _L( "CMyDebugObserver::RunError: aError=%d" ), aError );
    return KErrNone;
    }

void CMyDebugObserver::DoCancel()
    {
    EUNIT_PRINT( _L( "Docancel: errcode=%d" ), iRep->NotifyCancel( 0x00000002, 0x40FFFFFF ) );
    }

void CMyDebugObserver::NotifyReqL()
    {
    EUNIT_PRINT( _L( "CMyDebugObserver::NotifyReqL" ) );
    User::LeaveIfError( iRep->NotifyRequest( 0x00000002, 0x40FFFFFF, iStatus ) );
    SetActive();
    }

void CMyDebugObserver::ConstructL()
    {
    EUNIT_PRINT( _L( "CMyDebugObserver::ConstructL" ) );
    iRep = CRepository::NewL( KCRUidProfileEngine );
    CActiveScheduler::Add( this );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    MT_MProEngNotifyHandler,
    "Wrapper API Notification Handler Test cases.",
    "MODULE" )

EUNIT_TEST(
    "Request profile activation notifications - test",
    "MProEngNotifyHandler",
    "RequestProfileActivationNotificationsL",
    "FUNCTIONALITY",
    SetupRequestProfileActivationNotificationsTestL,
    RequestProfileActivationNotificationsTestL,
    TeardownRequestProfileActivationNotificationsTest)

EUNIT_TEST(
    "Request active profile notifications - test",
    "MProEngNotifyHandler",
    "RequestActiveProfileNotificationsL",
    "FUNCTIONALITY",
    SetupRequestActiveProfileNotificationsTestL,
    RequestActiveProfileNotificationsTestL,
    TeardownRequestActiveProfileNotificationsTest)

EUNIT_TEST(
    "Request profile(0) notifications - test",
    "MProEngNotifyHandler",
    "RequestProfileNotificationsL",
    "FUNCTIONALITY",
    SetupRequestProfileNotificationsTest0L,
    RequestProfileNotificationsTestL,
    TeardownRequestProfileNotificationsTest)

EUNIT_TEST(
    "Request profile(1) notifications - test",
    "MProEngNotifyHandler",
    "RequestProfileNotificationsL",
    "FUNCTIONALITY",
    SetupRequestProfileNotificationsTest1L,
    RequestProfileNotificationsTestL,
    TeardownRequestProfileNotificationsTest)

EUNIT_TEST(
    "Request name array notifications 1 - test",
    "MProEngNotifyHandler",
    "RequestProfileNameArrayNotificationsL",
    "FUNCTIONALITY",
    SetupRequestProfileNameArrayNotificationsTest1L,
    RequestProfileNameArrayNotificationsTestL,
    TeardownRequestProfileNameArrayNotificationsTest)

EUNIT_TEST(
    "Request profile(2) notifications - test",
    "MProEngNotifyHandler",
    "RequestProfileNotificationsL",
    "FUNCTIONALITY",
    SetupRequestProfileNotificationsTest2L,
    RequestProfileNotificationsTestL,
    TeardownRequestProfileNotificationsTest)

EUNIT_TEST(
    "Request name array notifications 2 - test",
    "MProEngNotifyHandler",
    "RequestProfileNameArrayNotificationsL",
    "FUNCTIONALITY",
    SetupRequestProfileNameArrayNotificationsTest2L,
    RequestProfileNameArrayNotificationsTestL,
    TeardownRequestProfileNameArrayNotificationsTest)

EUNIT_TEST(
    "Request profile(3) notifications - test",
    "MProEngNotifyHandler",
    "RequestProfileNotificationsL",
    "FUNCTIONALITY",
    SetupRequestProfileNotificationsTest3L,
    RequestProfileNotificationsTestL,
    TeardownRequestProfileNotificationsTest)

EUNIT_TEST(
    "Request name array notifications 3 - test",
    "MProEngNotifyHandler",
    "RequestProfileNameArrayNotificationsL",
    "FUNCTIONALITY",
    SetupRequestProfileNameArrayNotificationsTest3L,
    RequestProfileNameArrayNotificationsTestL,
    TeardownRequestProfileNameArrayNotificationsTest)

EUNIT_TEST(
    "Request profile(4) notifications - test",
    "MProEngNotifyHandler",
    "RequestProfileNotificationsL",
    "FUNCTIONALITY",
    SetupRequestProfileNotificationsTest4L,
    RequestProfileNotificationsTestL,
    TeardownRequestProfileNotificationsTest)

EUNIT_TEST(
    "Request name array notifications 4 - test",
    "MProEngNotifyHandler",
    "RequestProfileNameArrayNotificationsL",
    "FUNCTIONALITY",
    SetupRequestProfileNameArrayNotificationsTest4L,
    RequestProfileNameArrayNotificationsTestL,
    TeardownRequestProfileNameArrayNotificationsTest)
/*
EUNIT_TEST(
    "Request profile(5) notifications - test",
    "MProEngNotifyHandler",
    "RequestProfileNotificationsL",
    "FUNCTIONALITY",
    SetupRequestProfileNotificationsTest5L,
    RequestProfileNotificationsTestL,
    TeardownRequestProfileNotificationsTest)
*/
EUNIT_TEST(
    "Cancel profile activation notifications - test",
    "MProEngNotifyHandler",
    "CancelProfileActivationNotifications",
    "FUNCTIONALITY",
    SetupRequestProfileActivationNotificationsTestL,
    CancelProfileActivationNotificationsTestL,
    TeardownRequestProfileActivationNotificationsTest)

EUNIT_TEST(
    "Cancel active profile notifications - test",
    "MProEngNotifyHandler",
    "CancelActiveProfileNotifications",
    "FUNCTIONALITY",
    SetupRequestActiveProfileNotificationsTestL,
    CancelActiveProfileNotificationsTestL,
    TeardownRequestActiveProfileNotificationsTest)

EUNIT_TEST(
    "Cancel profile notifications -  test",
    "MProEngNotifyHandler",
    "CancelProfileNotifications",
    "FUNCTIONALITY",
    SetupRequestProfileNotificationsTest0L,
    CancelProfileNotificationsTestL,
    TeardownRequestProfileNotificationsTest)

EUNIT_TEST(
    "Cancen profile name array notifications - test",
    "MProEngNotifyHandler",
    "CancelProfileNameArrayNotifications",
    "FUNCTIONALITY",
    SetupRequestProfileNameArrayNotificationsTest1L,
    CancelProfileNameArrayNotificationsTestL,
    TeardownRequestProfileNameArrayNotificationsTest)

EUNIT_TEST(
    "Cancel all notifications - test",
    "MProEngNotifyHandler",
    "CancelAll",
    "FUNCTIONALITY",
    SetupCancelAllTestL,
    CancelAllTestL, TeardownCancelAllTest)


EUNIT_END_TEST_TABLE

//  END OF FILE

