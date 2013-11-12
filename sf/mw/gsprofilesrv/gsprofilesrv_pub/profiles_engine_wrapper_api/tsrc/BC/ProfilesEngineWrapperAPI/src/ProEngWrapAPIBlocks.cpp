/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:        ?Description
*
*/









// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "ProEngWrapAPI.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngWrapAPI::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CProEngWrapAPI::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "NewEngineL", CProEngWrapAPI::NewEngineL ),
        ENTRY( "NewEngineLC", CProEngWrapAPI::NewEngineLC ),
        ENTRY( "NewEngineLCWithRFsL", CProEngWrapAPI::NewEngineLCWithRFsL ),
        ENTRY( "NewNotifyHandlerL", CProEngWrapAPI::NewNotifyHandlerL ),
        ENTRY( "ReqProfActNotificationsL", CProEngWrapAPI::RequestProfileActivationNotificationsL ),
        ENTRY( "CanProfActNotificationsL", CProEngWrapAPI::CancelProfileActivationNotificationsL ),
        ENTRY( "ReqActProfNotificationsL", CProEngWrapAPI::RequestActiveProfileNotificationsL ),
        ENTRY( "CanActProfNotificationsL", CProEngWrapAPI::CancelProfileActivationNotificationsL ),
        ENTRY( "ReqProfNotificationsL", CProEngWrapAPI::RequestProfileNotificationsL ),
        ENTRY( "CanProfNotificationsL", CProEngWrapAPI::CancelProfileNotificationsL ),
        ENTRY( "ReqProfNameArrayNotL", CProEngWrapAPI::RequestProfileNameArrayNotificationsL ),
        ENTRY( "CanProfNameArrayNotL", CProEngWrapAPI::CancelProfileNameArrayNotificationsL ),
        ENTRY( "CancelAllL", CProEngWrapAPI::CancelAllL ),
        ENTRY( "ActiveProfileLC", CProEngWrapAPI::ActiveProfileLC ),
        ENTRY( "ActiveProfileL", CProEngWrapAPI::ActiveProfileLC ),
        ENTRY( "ActiveProfileIdL", CProEngWrapAPI::ActiveProfileIdL ),
        ENTRY( "ProfileNameArrayLC", CProEngWrapAPI::ProfileNameArrayLC ),
        ENTRY( "SetActiveProfileL", CProEngWrapAPI::SetActiveProfileL ),
        ENTRY( "ProfileLC", CProEngWrapAPI::ProfileLC ),
        ENTRY( "ProfileL", CProEngWrapAPI::ProfileL ),
        ENTRY( "MdcaCountL", CProEngWrapAPI::MdcaCountL ),
        ENTRY( "MdcaPointL", CProEngWrapAPI::MdcaPointL ),
        ENTRY( "ProfileIdL", CProEngWrapAPI::ProfileIdL ),
        ENTRY( "FindByIdL", CProEngWrapAPI::FindByIdL ),
        ENTRY( "FindByNameL", CProEngWrapAPI::FindByNameL ),
        ENTRY( "ProfileNameL", CProEngWrapAPI::ProfileNameL ),
        ENTRY( "ProfileTonesL", CProEngWrapAPI::ProfileTonesL ),
        ENTRY( "ToneSettingsL", CProEngWrapAPI::ToneSettingsL ),
        ENTRY( "AlertForL", CProEngWrapAPI::AlertForL ),
        ENTRY( "SetAlertForL", CProEngWrapAPI::SetAlertForL ),
        ENTRY( "IsSilentL", CProEngWrapAPI::IsSilentL ),
        ENTRY( "CommitChangeL", CProEngWrapAPI::CommitChangeL ),
        ENTRY( "NameL", CProEngWrapAPI::NameL ),
        ENTRY( "IdL", CProEngWrapAPI::IdL ),
        ENTRY( "SetNameL", CProEngWrapAPI::SetNameL ),
        ENTRY( "RingingTone1L", CProEngWrapAPI::RingingTone1L ),
        ENTRY( "RingingTone2L", CProEngWrapAPI::RingingTone2L ),
        ENTRY( "MessageAlertToneL", CProEngWrapAPI::MessageAlertToneL ),
        ENTRY( "EmailAlertToneL", CProEngWrapAPI::EmailAlertToneL ),
        ENTRY( "VideoCallRingingToneL", CProEngWrapAPI::VideoCallRingingToneL ),
        ENTRY( "SetRingingTone1L", CProEngWrapAPI::SetRingingTone1L ),
        ENTRY( "SetRingingTone2L", CProEngWrapAPI::SetRingingTone2L ),
        ENTRY( "SetMessageAlertToneL", CProEngWrapAPI::SetMessageAlertToneL ),
        ENTRY( "SetEmailAlertToneL", CProEngWrapAPI::SetEmailAlertToneL ),
        ENTRY( "SetVideoCallRingingToneL", CProEngWrapAPI::SetVideoCallRingingToneL ),
        ENTRY( "RingingTypeL", CProEngWrapAPI::RingingTypeL ),
        ENTRY( "KeypadVolumeL", CProEngWrapAPI::KeypadVolumeL ),
        ENTRY( "RingingVolumeL", CProEngWrapAPI::RingingVolumeL ),
        ENTRY( "VibratingAlertL", CProEngWrapAPI::VibratingAlertL ),
        ENTRY( "WarningAndGameTonesL", CProEngWrapAPI::WarningAndGameTonesL ),
        ENTRY( "TextToSpeechL", CProEngWrapAPI::TextToSpeechL ),
        ENTRY( "SetRingingTypeL", CProEngWrapAPI::SetRingingTypeL ),
        ENTRY( "SetKeypadVolumeL", CProEngWrapAPI::SetKeypadVolumeL ),
        ENTRY( "SetRingingVolumeL", CProEngWrapAPI::SetRingingVolumeL ),
        ENTRY( "SetVibratingAlertL", CProEngWrapAPI::SetVibratingAlertL ),
        ENTRY( "SetWarningAndGameTonesL", CProEngWrapAPI::SetWarningAndGameTonesL ),
        ENTRY( "SetTextToSpeechL", CProEngWrapAPI::SetTextToSpeechL ),
        ENTRY( "NewAlertToneSeekerL", CProEngWrapAPI::NewAlertToneSeekerL ),
        ENTRY( "FetchAlertToneListL", CProEngWrapAPI::FetchAlertToneListL ),
        ENTRY( "CancelFetchL", CProEngWrapAPI::CancelFetchL ),
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::NewEngineL
// Test:    ProEngFactory::NewEngineL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::NewEngineL()
    {
    RDebug::Print( _L( "ProEng Validation test NewEngineL" ) );
    MProEngEngine* engine( NULL );
    TRAP_IGNORE( engine = ProEngFactory::NewEngineL() );
      if(engine)
        {
        engine->Release();
        engine = NULL;
        }
        return KErrNone;
    }
// -----------------------------------------------------------------------------
// CProEngWrapAPI::NewEngineLC
// Test:    ProEngFactory::NewEngineLC()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::NewEngineLC()
    {
    RDebug::Print( _L( "ProEng Validation test NewEngineLC" ) );
    MProEngEngine* engine(NULL);
    engine = ProEngFactory::NewEngineLC();
    CleanupStack::PopAndDestroy(); // engine
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CProEngWrapAPI::NewEngineLCWithRFsL
// Test:    ProEngFactory::NewEngineLC(RFs)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::NewEngineLCWithRFsL()
    {
     
    RDebug::Print( _L( "ProEng Validation test NewEngineLCWithRFsL" ) );
    MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
    CleanupStack::PopAndDestroy(); // engine
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CProEngWrapAPI::NewEngineLCWithRFsL
// Test:    ProEngFactory::NewNotifyHandlerL()
// Test:    MProEngNotifyHandler::RequestProfileActivationNotificationsL(MProEngProfileActivationObserver&)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

TInt CProEngWrapAPI::NewNotifyHandlerL()
    {
    RDebug::Print( _L( "ProEng Validation test NewNotifyHandlerL" ) );
    MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
    MProEngNotifyHandler* nh( NULL );
    CProEngVTNotifyHandlerWrapper* wrapper = new ( ELeave )CProEngVTNotifyHandlerWrapper();
    CleanupStack::PushL( wrapper );
    TRAP_IGNORE( nh = ProEngFactory::NewNotifyHandlerL() );
    if (nh)
        {
        wrapper->SetObject(nh);
        }
    CleanupStack::PopAndDestroy( wrapper );
    CleanupStack::PopAndDestroy(); // engine
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CProEngWrapAPI::RequestProfileActivationNotificationsL
// Test:    MProEngNotifyHandler::RequestProfileActivationNotificationsL(MProEngProfileActivationObserver&)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::RequestProfileActivationNotificationsL()
    {
    RDebug::Print( _L( "ProEng Validation test RequestProfileActivationNotificationsL begin" ) );
    RDebug::Print( _L( "ProEng Validation test NewNotifyHandlerL" ) );
    MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
    MProEngNotifyHandler* nh( NULL );
    CProEngVTNotifyHandlerWrapper* wrapper = new ( ELeave )CProEngVTNotifyHandlerWrapper();
    CleanupStack::PushL( wrapper );
    TRAP_IGNORE( nh = ProEngFactory::NewNotifyHandlerL() );
    if( nh )
        {
        wrapper->SetObject( nh ); //  takes ownership
        }
    CProEngVTObserver* observer = new ( ELeave ) CProEngVTObserver(*iSchedulerUtility);
    CleanupStack::PushL( observer );

    TRAP_IGNORE(nh->RequestProfileActivationNotificationsL ( *observer ) );
    engine->SetActiveProfileL( EProfileMeetingId );
    iSchedulerUtility->Start();
    CleanupStack::PopAndDestroy( observer );
    CleanupStack::PopAndDestroy( wrapper );
    CleanupStack::PopAndDestroy(); // engine
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::CancelProfileActivationNotificationsL
// Test:    MProEngNotifyHandler::CancelProfileActivationNotifications()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::CancelProfileActivationNotificationsL()
    {
    RDebug::Print( _L( "ProEng Validation test CancelProfileActivationNotificationsL begin" ) );
    RDebug::Print( _L( "ProEng Validation test NewNotifyHandlerL" ) );
    MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
    MProEngNotifyHandler* nh( NULL );
    CProEngVTNotifyHandlerWrapper* wrapper = new ( ELeave )CProEngVTNotifyHandlerWrapper();
    CleanupStack::PushL( wrapper );
    TRAP_IGNORE( nh = ProEngFactory::NewNotifyHandlerL() );
    if( nh )
        {
        wrapper->SetObject( nh ); //  takes ownership
        }
    CProEngVTObserver* observer = new ( ELeave ) CProEngVTObserver(*iSchedulerUtility);
    CleanupStack::PushL( observer );

    TRAP_IGNORE(nh->RequestProfileActivationNotificationsL ( *observer ) );
    engine->SetActiveProfileL( EProfileMeetingId );
    iSchedulerUtility->Start();
    nh->CancelProfileActivationNotifications();
    CleanupStack::PopAndDestroy( observer );
    CleanupStack::PopAndDestroy( wrapper );
    CleanupStack::PopAndDestroy(); // engine
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::RequestActiveProfileNotificationsL
// Test:    MProEngNotifyHandler::RequestActiveProfileNotificationsL(MProEngActiveProfileObserver&)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::RequestActiveProfileNotificationsL()
    {
    RDebug::Print( _L( "ProEng Validation test RequestProfileActivationNotificationsL begin" ) );
    RDebug::Print( _L( "ProEng Validation test NewNotifyHandlerL" ) );
    MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
    MProEngNotifyHandler* nh( NULL );
    CProEngVTNotifyHandlerWrapper* wrapper = new ( ELeave )CProEngVTNotifyHandlerWrapper();
    CleanupStack::PushL( wrapper );
    TRAP_IGNORE( nh = ProEngFactory::NewNotifyHandlerL() );
    if( nh )
        {
        wrapper->SetObject( nh ); //  takes ownership
        }
    CProEngVTObserver* observer = new ( ELeave ) CProEngVTObserver(*iSchedulerUtility);
    CleanupStack::PushL( observer );

    MProEngProfile* activeProfile = engine->ActiveProfileLC();
    MProEngToneSettings& ts( activeProfile->ToneSettings() );
    TRAP_IGNORE( 
            nh->RequestActiveProfileNotificationsL ( *observer ) );
    ts.SetVibratingAlert( !ts.VibratingAlert() );
    activeProfile->CommitChangeL();
    iSchedulerUtility->Start();
    CleanupStack::PopAndDestroy(); // activeProfile
    CleanupStack::PopAndDestroy( observer );
    CleanupStack::PopAndDestroy( wrapper );
    CleanupStack::PopAndDestroy(); // engine
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::CancelActiveProfileNotificationsL
// Test:    MProEngNotifyHandler::CancelActiveProfileNotifications()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::CancelActiveProfileNotificationsL()
    {
    RDebug::Print( _L( "ProEng Validation test CancelActiveProfileNotifications begin" ) );
    RDebug::Print( _L( "ProEng Validation test NewNotifyHandlerL" ) );
    MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
    MProEngNotifyHandler* nh( NULL );
    CProEngVTNotifyHandlerWrapper* wrapper = new ( ELeave )CProEngVTNotifyHandlerWrapper();
    CleanupStack::PushL( wrapper );
    TRAP_IGNORE( nh = ProEngFactory::NewNotifyHandlerL() );
    if( nh )
        {
        wrapper->SetObject( nh ); //  takes ownership
        }
    CProEngVTObserver* observer = new ( ELeave ) CProEngVTObserver(*iSchedulerUtility);
    CleanupStack::PushL( observer );
    MProEngProfile* activeProfile = engine->ActiveProfileLC();
    MProEngToneSettings& ts( activeProfile->ToneSettings() );
    RDebug::Print( _L( "ProEng Validation test TestNotifyHandler 5" ) );
    TRAP_IGNORE( 
            nh->RequestActiveProfileNotificationsL ( *observer ) );
    ts.SetVibratingAlert( !ts.VibratingAlert() );
    activeProfile->CommitChangeL();
    iSchedulerUtility->Start();
    nh->CancelActiveProfileNotifications();
    CleanupStack::PopAndDestroy(); // activeProfile
    CleanupStack::PopAndDestroy( observer );
    CleanupStack::PopAndDestroy( wrapper );
    CleanupStack::PopAndDestroy(); // engine
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::RequestProfileNotificationsL
// Test:    MProEngNotifyHandler::RequestProfileNotificationsL(MProEngActiveProfileObserver&)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::RequestProfileNotificationsL()
    {
    RDebug::Print( _L( "ProEng Validation test RequestProfileNotificationsL begin" ) );
    RDebug::Print( _L( "ProEng Validation test NewNotifyHandlerL" ) );
    MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
    MProEngNotifyHandler* nh( NULL );
    CProEngVTNotifyHandlerWrapper* wrapper = new ( ELeave )CProEngVTNotifyHandlerWrapper();
    CleanupStack::PushL( wrapper );
    TRAP_IGNORE( nh = ProEngFactory::NewNotifyHandlerL() );
    if( nh )
        {
        wrapper->SetObject( nh ); //  takes ownership
        }
    CProEngVTObserver* observer = new ( ELeave ) CProEngVTObserver(*iSchedulerUtility);
    CleanupStack::PushL( observer );

    MProEngProfile* generalProfile = engine->ProfileLC( EProfileGeneralId );
//    MProEngToneSettings& ts2( generalProfile->ToneSettings() );
    TRAP_IGNORE( 
            nh->RequestProfileNotificationsL( *observer,
                                                   EProfileGeneralId ) );
    //ts2.SetVibratingAlert( !ts2.VibratingAlert() );
    MProEngTones& pt = generalProfile->ProfileTones();
    const TDesC& emailAlertTone = pt.EmailAlertTone();
    _LIT( KFile1, "z:\\data\\sounds\\digital\\alarm.aac" );
    _LIT( KFile2, "z:\\data\\sounds\\digital\\Buzzer.aac" );

    TBuf<100> buf1(KFile1);
    TBuf<100> buf2(KFile2);
    
    if ( emailAlertTone.Compare( buf1 ) )
        {
        TRAP_IGNORE( pt.SetEmailAlertToneL(buf1));
        }
    else 
        {
        TRAP_IGNORE(pt.SetEmailAlertToneL(buf2));
        }
    generalProfile->CommitChangeL();
    iSchedulerUtility->Start();
    CleanupStack::PopAndDestroy(); // generalProfile
    CleanupStack::PopAndDestroy( observer );
    CleanupStack::PopAndDestroy( wrapper );
    CleanupStack::PopAndDestroy(); // engine
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::CancelProfileNotificationsL
// Test:    MProEngNotifyHandler::CancelProfileNotifications()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::CancelProfileNotificationsL()
    {
    RDebug::Print( _L( "ProEng Validation test CancelProfileNotifications begin" ) );
    RDebug::Print( _L( "ProEng Validation test NewNotifyHandlerL" ) );
    MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
    MProEngNotifyHandler* nh( NULL );
    CProEngVTNotifyHandlerWrapper* wrapper = new ( ELeave )CProEngVTNotifyHandlerWrapper();
    CleanupStack::PushL( wrapper );
    TRAP_IGNORE( nh = ProEngFactory::NewNotifyHandlerL() );
    if( nh )
        {
        wrapper->SetObject( nh ); //  takes ownership
        }
    CProEngVTObserver* observer = new ( ELeave ) CProEngVTObserver(*iSchedulerUtility);
    CleanupStack::PushL( observer );
    MProEngProfile* generalProfile = engine->ProfileLC( EProfileGeneralId );
//    MProEngToneSettings& ts2( generalProfile->ToneSettings() );
    TRAP_IGNORE( 
            nh->RequestProfileNotificationsL( *observer,
                                                   EProfileGeneralId ) );
    MProEngTones& pt = generalProfile->ProfileTones();
    const TDesC& emailAlertTone = pt.EmailAlertTone();
    _LIT( KFile1, "z:\\data\\sounds\\digital\\alarm.aac" );
    _LIT( KFile2, "z:\\data\\sounds\\digital\\Buzzer.aac" );

    TBuf<100> buf1(KFile1);
    TBuf<100> buf2(KFile2);
    
    if ( emailAlertTone.Compare( buf1 ) )
        {
        TRAP_IGNORE( pt.SetEmailAlertToneL(buf1));
        }
    else 
        {
        TRAP_IGNORE(pt.SetEmailAlertToneL(buf2));
        }
    generalProfile->CommitChangeL();
    iSchedulerUtility->Start();
    nh->CancelProfileNotifications( EProfileGeneralId );
    CleanupStack::PopAndDestroy(); // generalProfile
    CleanupStack::PopAndDestroy( observer );
    CleanupStack::PopAndDestroy( wrapper );
    CleanupStack::PopAndDestroy(); // engine
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::RequestProfileNameArrayNotificationsL
// Test:    MProEngNotifyHandler::RequestProfileNameArrayNotificationsL(MProEngActiveProfileObserver&)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::RequestProfileNameArrayNotificationsL()
    {
    RDebug::Print( _L( "ProEng Validation test RequestProfileNameArrayNotificationsL begin" ) );
    RDebug::Print( _L( "ProEng Validation test NewNotifyHandlerL" ) );
    MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
    MProEngNotifyHandler* nh( NULL );
    CProEngVTNotifyHandlerWrapper* wrapper = new ( ELeave )CProEngVTNotifyHandlerWrapper();
    CleanupStack::PushL( wrapper );
    TRAP_IGNORE( nh = ProEngFactory::NewNotifyHandlerL() );
    if( nh )
        {
        wrapper->SetObject( nh ); //  takes ownership
        }
    CProEngVTObserver* observer = new ( ELeave ) CProEngVTObserver(*iSchedulerUtility);
    CleanupStack::PushL( observer );
    MProEngProfile* silentProfile = engine->ProfileLC( EProfileSilentId );
    MProEngProfileName& silentName( silentProfile->ProfileName() );
    TRAP_IGNORE( 
              nh->RequestProfileNameArrayNotificationsL ( *observer ) );
      // All this hassle here is just for modifying the name in every run of this
      // test so that we really get notification from Cenrep:
    HBufC* nameBuf = silentName.Name().AllocLC();
    TPtr ptr( nameBuf->Des() );
    TInt lastCharPos( nameBuf->Length() - 1 );
    const TUint KWrapperVTAllOnes( 0xFFFF );
    ptr[lastCharPos] = ptr[lastCharPos] ^ KWrapperVTAllOnes;
    silentName.SetNameL( *nameBuf );
    CleanupStack::PopAndDestroy( nameBuf );
    silentProfile->CommitChangeL();
    iSchedulerUtility->Start();
    CleanupStack::PopAndDestroy(); // silentProfile
    CleanupStack::PopAndDestroy( observer );
    CleanupStack::PopAndDestroy( wrapper );
    CleanupStack::PopAndDestroy(); // engine
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::CancelProfileNameArrayNotificationsL
// Test:    MProEngNotifyHandler::CancelProfileNameArrayNotifications()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::CancelProfileNameArrayNotificationsL()
    {
    RDebug::Print( _L( "ProEng Validation test CancelProfileNameArrayNotificationsL begin" ) );
    RDebug::Print( _L( "ProEng Validation test NewNotifyHandlerL" ) );
    MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
    MProEngNotifyHandler* nh( NULL );
    CProEngVTNotifyHandlerWrapper* wrapper = new ( ELeave )CProEngVTNotifyHandlerWrapper();
    CleanupStack::PushL( wrapper );
    TRAP_IGNORE( nh = ProEngFactory::NewNotifyHandlerL() );
    if( nh )
        {
        wrapper->SetObject( nh ); //  takes ownership
        }
    CProEngVTObserver* observer = new ( ELeave ) CProEngVTObserver(*iSchedulerUtility);
    CleanupStack::PushL( observer );
    MProEngProfile* silentProfile = engine->ProfileLC( EProfileSilentId );
    MProEngProfileName& silentName( silentProfile->ProfileName() );
    TRAP_IGNORE( 
              nh->RequestProfileNameArrayNotificationsL ( *observer ) );
      // All this hassle here is just for modifying the name in every run of this
      // test so that we really get notification from Cenrep:
    HBufC* nameBuf = silentName.Name().AllocLC();
    TPtr ptr( nameBuf->Des() );
    TInt lastCharPos( nameBuf->Length() - 1 );
    const TUint KWrapperVTAllOnes( 0xFFFF );
    ptr[lastCharPos] = ptr[lastCharPos] ^ KWrapperVTAllOnes;
    silentName.SetNameL( *nameBuf );
    CleanupStack::PopAndDestroy( nameBuf );
    silentProfile->CommitChangeL();
    iSchedulerUtility->Start();
    //CSchedulerUtility::InstanceL()->Start(); // observer stops this
    nh->CancelProfileNameArrayNotifications();
    CleanupStack::PopAndDestroy(); // silentProfile
    CleanupStack::PopAndDestroy( observer );
    CleanupStack::PopAndDestroy( wrapper );
    CleanupStack::PopAndDestroy(); // engine
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::CancelAllL
// Test:    MProEngEngine::CancelAll()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::CancelAllL()
    {
    RDebug::Print( _L( "ProEng Validation test CancelAllL begin" ) );
    RDebug::Print( _L( "ProEng Validation test NewNotifyHandlerL" ) );
    MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
    MProEngNotifyHandler* nh( NULL );
    CProEngVTNotifyHandlerWrapper* wrapper = new ( ELeave )CProEngVTNotifyHandlerWrapper();
    CleanupStack::PushL( wrapper );
    TRAP_IGNORE( nh = ProEngFactory::NewNotifyHandlerL() );
    if( nh )
        {
        wrapper->SetObject( nh ); //  takes ownership
        }
    nh->CancelAll();
    CleanupStack::PopAndDestroy( wrapper );
    CleanupStack::PopAndDestroy(); // engine
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CProEngWrapAPI::ActiveProfileLC
// Test:    MProEngEngine::ActiveProfileLC()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::ActiveProfileLC()
    {
   RDebug::Print( _L( "ProEng Validation test ActiveProfileLC" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = engine->ActiveProfileLC();
   CleanupStack::PopAndDestroy(); // prof
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }
// -----------------------------------------------------------------------------
// CProEngWrapAPI::ActiveProfileL
// Test:    MProEngEngine::ActiveProfileL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::ActiveProfileL()
    {
   RDebug::Print( _L( "ProEng Validation test ActiveProfileL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = engine->ActiveProfileL();
   if(prof)
       {
       prof->Release();
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::ActiveProfileId
// Test:    MProEngEngine::ActiveProfileId()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::ActiveProfileIdL()
    {
   RDebug::Print( _L( "ProEng Validation test ActiveProfileId" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   //RDebug::Print( _L( "ProEng Validation test TestEngineL 3" ) );
   TInt id( engine->ActiveProfileId() );
   RDebug::Print( _L( "Active id:%d" ), id ); 
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::ProfileNameArrayLC
// Test:    MProEngEngine::ProfileNameArrayLC()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::ProfileNameArrayLC()
    {
   RDebug::Print( _L( "ProEng Validation test ProfileNameArrayLC" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   TRAP_IGNORE(
           MProEngProfileNameArray* names( engine->ProfileNameArrayLC() );
           CleanupStack::PopAndDestroy(); // names
           );
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::SetActiveProfileL
// Test:    MProEngEngine::SetActiveProfileL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::SetActiveProfileL()
    {
   RDebug::Print( _L( "ProEng Validation test SetActiveProfileL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   TRAP_IGNORE( engine->SetActiveProfileL( EProfileSilentId ) ); 
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::ProfileLC
// Test:    MProEngEngine::ProfileLC()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::ProfileLC()
    {
   RDebug::Print( _L( "ProEng Validation test ProfileLC" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( 
               prof = engine->ProfileLC( EProfileSilentId );
               CleanupStack::PopAndDestroy(); // prof
               prof = NULL;
               );
 
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::ProfileL
// Test:    MProEngEngine::ProfileLC()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::ProfileL()
    {
   RDebug::Print( _L( "ProEng Validation test ProfileL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       //TRAP_IGNORE( TestProfileSettingsL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::MdcaCountL
// Test:    MProEngProfileNameArray::MdcaCount()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::MdcaCountL()
    {
   RDebug::Print( _L( "ProEng Validation test MdcaCount" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   TRAP_IGNORE(
           MProEngProfileNameArray* names( engine->ProfileNameArrayLC() );
           TestProfileNameArrayMdcaCountL( *names );
           CleanupStack::PopAndDestroy(); // names
            );
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }
void CProEngWrapAPI::TestProfileNameArrayMdcaCountL(MProEngProfileNameArray& aNameArray)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileNameArrayMdcaCountL begin" ) );
    TInt count( aNameArray.MdcaCount() );
    RDebug::Print( _L( "count = %d" ), count );
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::MdcaPointL
// Test:    MProEngProfileNameArray::MdcaPoint(TInt)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::MdcaPointL()
    {
   RDebug::Print( _L( "ProEng Validation test MdcaPoint" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   TRAP_IGNORE(
           MProEngProfileNameArray* names( engine->ProfileNameArrayLC() );
           TestProfileNameArrayMdcaPointL( *names );
           CleanupStack::PopAndDestroy(); // names
            );
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }
void CProEngWrapAPI::TestProfileNameArrayMdcaPointL(MProEngProfileNameArray& aNameArray)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileNameArrayMdcaPointL begin" ) );
    TInt count( aNameArray.MdcaCount() );
    RDebug::Print( _L( "count = %d" ), count );
    for( TInt i( 0 ); i<count; ++i )
        {
        TPtrC name( aNameArray.MdcaPoint( i ) );
        RDebug::Print( _L( "ProEng Validation test nameArray[%d].MdcaPoint=%S" ), i, &name );
        }
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::ProfileIdL
// Test:    MProEngProfileNameArray::ProfileId(TInt)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::ProfileIdL()
    {
   RDebug::Print( _L( "ProEng Validation test ProfileId" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   TRAP_IGNORE(
           MProEngProfileNameArray* names( engine->ProfileNameArrayLC() );
           TestProfileNameArrayProfileIdL( *names );
           CleanupStack::PopAndDestroy(); // names
            );
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }
void CProEngWrapAPI::TestProfileNameArrayProfileIdL(MProEngProfileNameArray& aNameArray)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileNameArrayProfileIdL begin" ) );
    TInt count( aNameArray.MdcaCount() );
    RDebug::Print( _L( "count = %d" ), count );
    for( TInt i( 0 ); i<count; ++i )
        {
        TPtrC name( aNameArray.MdcaPoint( i ) );
        TInt id( aNameArray.ProfileId( i ) );
        RDebug::Print( _L( "ProEng Validation test nameArray[%d].MdcaPoint=%S" ), i, &name );
        RDebug::Print( _L( "ProEng Validation test nameArray[%d].ProfileId=%d" ), i, id );
        
        }
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::FindByIdL
// Test:    MProEngProfileNameArray::FindById(TInt)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::FindByIdL()
    {
   RDebug::Print( _L( "ProEng Validation test FindByIdL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   TRAP_IGNORE(
           MProEngProfileNameArray* names( engine->ProfileNameArrayLC() );
           TestProfileNameArrayFindByIdL( *names );
           CleanupStack::PopAndDestroy(); // names
            );
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }


void CProEngWrapAPI::TestProfileNameArrayFindByIdL(MProEngProfileNameArray& aNameArray)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileNameArrayFindByIdL begin" ) );
    TInt count( aNameArray.MdcaCount() );
    RDebug::Print( _L( "count = %d" ), count );
    for( TInt i( 0 ); i<count; ++i )
        {
        TPtrC name( aNameArray.MdcaPoint( i ) );
        TInt id( aNameArray.ProfileId( i ) );
        RDebug::Print( _L( "ProEng Validation test nameArray[%d].MdcaPoint=%S" ), i, &name );
        RDebug::Print( _L( "ProEng Validation test nameArray[%d].ProfileId=%d" ), i, id );
        
        }
    TInt idx( aNameArray.FindById( EProfileMeetingId ) );
    RDebug::Print( _L( "ProEng Validation test Index of Meeting profile = %d" ), idx );
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::FindByNameL
// Test:    MProEngProfileNameArray::FindByName(const TDesC&)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::FindByNameL()
    {
   RDebug::Print( _L( "ProEng Validation test FindByNameL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   TRAP_IGNORE(
           MProEngProfileNameArray* names( engine->ProfileNameArrayLC() );
           TestProfileNameArrayFindByNameL( *names );
           CleanupStack::PopAndDestroy(); // names
            );
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileNameArrayFindByNameL(MProEngProfileNameArray& aNameArray)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileNameArrayFindByNameL begin" ) );
    TInt count( aNameArray.MdcaCount() );
    RDebug::Print( _L( "count = %d" ), count );
    for( TInt i( 0 ); i<count; ++i )
        {
        TPtrC name( aNameArray.MdcaPoint( i ) );
        TInt id( aNameArray.ProfileId( i ) );
        RDebug::Print( _L( "ProEng Validation test nameArray[%d].MdcaPoint=%S" ), i, &name );
        RDebug::Print( _L( "ProEng Validation test nameArray[%d].ProfileId=%d" ), i, id );
        
        }
    TInt idx;
    _LIT( KProEngVTSilent, "Silent" );
    idx = aNameArray.FindByName( KProEngVTSilent );
    RDebug::Print( _L( "ProEng Validation test Index of Silent profile = %d" ), idx );
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::ProfileNameL
// Test:    MProEngProfile::ProfileName()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::ProfileNameL()
    {
   RDebug::Print( _L( "ProEng Validation test ProfileNameL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsProfileNameL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsProfileNameL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsProfileNameL begin" ) );
    MProEngProfileName& name( aProfile.ProfileName() );
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::ProfileTonesL
// Test:    MProEngProfile::ProfileTones()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::ProfileTonesL()
    {
   RDebug::Print( _L( "ProEng Validation test ProfileTonesL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsProfileTonesL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsProfileTonesL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsProfileTonesL begin" ) );
    MProEngTones& tones( aProfile.ProfileTones() );
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::ToneSettingsL
// Test:    MProEngProfile::ToneSettings()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::ToneSettingsL()
    {
     
   RDebug::Print( _L( "ProEng Validation test ToneSettingsL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsToneSettingsL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
 
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsToneSettingsL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsRingingTypeL begin" ) );
    MProEngToneSettings& toneSettings( aProfile.ToneSettings() );
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::AlertForL
// Test:    MProEngProfile::AlertForL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::AlertForL()
    {
   RDebug::Print( _L( "ProEng Validation test AlertForL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsAlertForL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsAlertForL(MProEngProfile& aProfile)
    {
    // Alert for getter/setter:
    TRAP_IGNORE(const TArray<TContactItemId> alertFor = aProfile.AlertForL(););    
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::SetAlertForL
// Test:    MProEngProfile::SetAlertForL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::SetAlertForL()
    {
   RDebug::Print( _L( "ProEng Validation test setAlertForL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsSetAlertForL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsSetAlertForL(MProEngProfile& aProfile)
    {
    // Alert for getter/setter:
     TRAP_IGNORE(
            const TArray<TContactItemId> alertFor = aProfile.AlertForL();
            aProfile.SetAlertForL( alertFor );
            );    
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::IsSilentL
// Test:    MProEngProfile::IsSilentL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::IsSilentL()
    {
   RDebug::Print( _L( "ProEng Validation test IsSilentL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsIsSilentL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsIsSilentL(MProEngProfile& aProfile)
    {
    TBool silent( aProfile.IsSilent() );
    RDebug::Print( _L( "ProEng Validation test profile silent=%d" ), silent );
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::CommitChangeL
// Test:    MProEngProfile::CommitChangeL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::CommitChangeL()
    {
   RDebug::Print( _L( "ProEng Validation test CommitChangeL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsCommitChangeL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsCommitChangeL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test before CommitChangeL" ) );
    TRAP_IGNORE( aProfile.CommitChangeL() );
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::NameL
// Test:    MProEngProfileName::Name()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::NameL()
    {
   RDebug::Print( _L( "ProEng Validation test NameL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsNameL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsNameL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsNameL begin" ) );
    MProEngProfileName& name( aProfile.ProfileName() );
    const TDesC& daName( name.Name() );
    RDebug::Print( _L( "ProEng Validation test profile name=%S" ), &daName );
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::IdL
// Test:    MProEngProfileName::Id()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::IdL()
    {
   RDebug::Print( _L( "ProEng Validation test IdL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsIdL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsIdL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsIdL begin" ) );
    MProEngProfileName& name( aProfile.ProfileName() );
    TInt id( name.Id() );
    RDebug::Print( _L( "ProEng Validation test profile id=%d" ), id );
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::SetNameL
// Test:    MProEngProfileName::SetNameL(const TDesC&)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::SetNameL()
    {
   RDebug::Print( _L( "ProEng Validation test SetNameL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsSetNameL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsSetNameL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsSetNameL begin" ) );
    MProEngProfileName& name( aProfile.ProfileName() );
    const TDesC& daName( name.Name() );
    HBufC* nameToBeSet = daName.AllocLC();
    RDebug::Print( _L( "ProEng Validation test before SetNameL" ) );
    TRAP_IGNORE( name.SetNameL( *nameToBeSet ) );
    CleanupStack::PopAndDestroy(nameToBeSet);
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::RingingTone1L
// Test:    MProEngTones::RingingTone1()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::RingingTone1L()
    {
   RDebug::Print( _L( "ProEng Validation test RingingTone1L" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsRingingTone1L( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsRingingTone1L(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsRingingTone1L begin" ) );
    MProEngTones& tones( aProfile.ProfileTones() );
    const TDesC& rtone( tones.RingingTone1() );
    RDebug::Print( _L( "ProEng Validation test rtone=%S" ), &rtone );
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::RingingTone2L
// Test:    MProEngTones::RingingTone1()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::RingingTone2L()
    {
   RDebug::Print( _L( "ProEng Validation test RingingTone2L" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsRingingTone2L( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsRingingTone2L(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsRingingTone2L begin" ) );
    MProEngTones& tones( aProfile.ProfileTones() );
    const TDesC& rtone2( tones.RingingTone2() );
    RDebug::Print( _L( "ProEng Validation test rtone2 =%S" ), &rtone2 );
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::MessageAlertToneL
// Test:    MProEngTones::MessageAlertTone()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::MessageAlertToneL()
    {
   RDebug::Print( _L( "ProEng Validation test MessageAlertToneL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsMessageAlertToneL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsMessageAlertToneL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsMessageAlertToneL begin" ) );
    MProEngTones& tones( aProfile.ProfileTones() );
    const TDesC& mtone( tones.MessageAlertTone() );
    RDebug::Print( _L( "ProEng Validation test mtone =%S" ), &mtone );
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::EmailAlertToneL
// Test:    MProEngTones::EmailAlertTone()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::EmailAlertToneL()
    {
   RDebug::Print( _L( "ProEng Validation test EmailAlertToneL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsEmailAlertToneL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsEmailAlertToneL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsEmailAlertToneL begin" ) );
    MProEngTones& tones( aProfile.ProfileTones() );
    const TDesC& etone( tones.EmailAlertTone() );
    RDebug::Print( _L( "ProEng Validation test etone =%S" ), &etone );
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::VideoCallRingingToneL
// Test:    MProEngTones::VideoCallRingingToneL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::VideoCallRingingToneL()
    {
   RDebug::Print( _L( "ProEng Validation test VideoCallRingingToneL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsVideoCallRingingToneL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsVideoCallRingingToneL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsVideoCallRingingToneL begin" ) );
    MProEngTones& tones( aProfile.ProfileTones() );
    const TDesC& vcallTone( tones.VideoCallRingingTone() );
    RDebug::Print( _L( "ProEng Validation test vcallTone =%S" ), &vcallTone );
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::SetRingingTone1L
// Test:    MProEngTones::SetRingingTone1L(const TDesC&)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::SetRingingTone1L()
    {
   RDebug::Print( _L( "ProEng Validation test SetRingingTone1L" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsSetRingingTone1L( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsSetRingingTone1L(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsSetRingingTone1L begin" ) );
    MProEngTones& tones( aProfile.ProfileTones() );
    const TDesC& rtone( tones.RingingTone1() );
    HBufC* toneToBeSet = rtone.AllocLC();
    RDebug::Print( _L( "ProEng Validation test before SetRingingTone1L rtone=%S" ), toneToBeSet );
    TRAP_IGNORE( tones.SetRingingTone1L( *toneToBeSet ) );
    CleanupStack::PopAndDestroy( toneToBeSet );
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::SetRingingTone2L
// Test:    MProEngTones::SetRingingTone2L(const TDesC&)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::SetRingingTone2L()
    {
   RDebug::Print( _L( "ProEng Validation test SetRingingTone2L" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsSetRingingTone2L( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsSetRingingTone2L(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsSetRingingTone2L begin" ) );
    MProEngTones& tones( aProfile.ProfileTones() );
    const TDesC& rtone2( tones.RingingTone2() );
    HBufC* toneToBeSet = rtone2.AllocLC();
    RDebug::Print( _L( "ProEng Validation test before SetRingingTone1L rtone2=%S" ), toneToBeSet );
    TRAP_IGNORE( tones.SetRingingTone2L( *toneToBeSet ) );
    CleanupStack::PopAndDestroy( toneToBeSet );
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::SetMessageAlertToneL
// Test:    MProEngTones::SetMessageAlertTone()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::SetMessageAlertToneL()
    {
   RDebug::Print( _L( "ProEng Validation test SetMessageAlertToneL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsSetMessageAlertToneL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsSetMessageAlertToneL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsSetMessageAlertToneL begin" ) );
    MProEngTones& tones( aProfile.ProfileTones() );
    const TDesC& mtone( tones.MessageAlertTone() );
    RDebug::Print( _L( "ProEng Validation test mtone =%S" ), &mtone );
    RDebug::Print( _L( "ProEng Validation test before SetMessageAlertToneL" ) );
    TRAP_IGNORE( tones.SetMessageAlertToneL( mtone ) );        
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::SetEmailAlertToneL
// Test:    MProEngTones::SetEmailAlertTone()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::SetEmailAlertToneL()
    {
   RDebug::Print( _L( "ProEng Validation test SetEmailAlertToneL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsSetEmailAlertToneL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsSetEmailAlertToneL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsSetEmailAlertToneL begin" ) );
    MProEngTones& tones( aProfile.ProfileTones() );
    const TDesC& etone( tones.EmailAlertTone() );
    RDebug::Print( _L( "ProEng Validation test etone =%S" ), &etone );
    RDebug::Print( _L( "ProEng Validation test before SetEmailAlertToneL" ) );
    TRAP_IGNORE( tones.SetEmailAlertToneL( etone ) );        
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::SetVideoCallRingingToneL
// Test:    MProEngTones::SetVideoCallRingingToneL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::SetVideoCallRingingToneL()
    {
     
   RDebug::Print( _L( "ProEng Validation test SetVideoCallRingingToneL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsSetVideoCallRingingToneL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
 
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsSetVideoCallRingingToneL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsSetVideoCallRingingToneL begin" ) );
    MProEngTones& tones( aProfile.ProfileTones() );
    const TDesC& vcallTone( tones.VideoCallRingingTone() );
    RDebug::Print( _L( "ProEng Validation test vcallTone =%S" ), &vcallTone );
    RDebug::Print( _L( "ProEng Validation test before SetVideoCallAlertToneL" ) );
    TRAP_IGNORE( tones.SetVideoCallRingingToneL( vcallTone ) );
        
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::RingingTypeL
// Test:    MProEngToneSettings::RingingType()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::RingingTypeL()
    {
     
   RDebug::Print( _L( "ProEng Validation test RingingTypeL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsRingingTypeL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
 
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsRingingTypeL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsRingingTypeL begin" ) );
    MProEngToneSettings& toneSettings( aProfile.ToneSettings() );
    TProfileRingingType rtype( toneSettings.RingingType() );
    RDebug::Print( _L( "ProEng Validation test ringing type=%d" ), toneSettings.RingingType() );    
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::KeypadVolumeL
// Test:    MProEngToneSettings::KeypadVolumeL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::KeypadVolumeL()
    {
     
   RDebug::Print( _L( "ProEng Validation test KeypadVolumeL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsKeypadVolumeL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
 
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsKeypadVolumeL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsKeypadVolumeL begin" ) );
    MProEngToneSettings& toneSettings( aProfile.ToneSettings() );
    TProfileKeypadVolume keyvol( toneSettings.KeypadVolume() );
    RDebug::Print( _L( "ProEng Validation test ringing keypad=%d" ), toneSettings.KeypadVolume() );    
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::RingingVolumeL
// Test:    MProEngToneSettings::RingingVolumeL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::RingingVolumeL()
    {
     
   RDebug::Print( _L( "ProEng Validation test RingingVolumeL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsRingingVolumeL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
 
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsRingingVolumeL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsRingingVolumeL begin" ) );
    MProEngToneSettings& toneSettings( aProfile.ToneSettings() );
    TProfileRingingVolume ringvol( toneSettings.RingingVolume() );
    RDebug::Print( _L( "ProEng Validation test ringing volume=%d" ), toneSettings.RingingVolume() );    
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::VibratingAlertL
// Test:    MProEngToneSettings::VibratingAlertL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::VibratingAlertL()
    {
     
   RDebug::Print( _L( "ProEng Validation test VibratingAlertL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsVibratingAlertL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
 
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsVibratingAlertL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsVibratingAlertL begin" ) );
    MProEngToneSettings& toneSettings( aProfile.ToneSettings() );
    TBool vibra( toneSettings.VibratingAlert() );
    RDebug::Print( _L( "ProEng Validation test vibra=%d" ), vibra );    
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::WarningAndGameTonesL
// Test:    MProEngToneSettings::WarningAndGameTonesL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::WarningAndGameTonesL()
    {
     
   RDebug::Print( _L( "ProEng Validation test WarningAndGameTonesL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsWarningAndGameTonesL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
 
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsWarningAndGameTonesL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsWarningAndGameTonesL begin" ) );
    MProEngToneSettings& toneSettings( aProfile.ToneSettings() );
    TBool warntones( toneSettings.WarningAndGameTones() );
    RDebug::Print( _L( "ProEng Validation test warntones = %d" ), warntones );    
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::TextToSpeechL
// Test:    MProEngToneSettings::TextToSpeechL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::TextToSpeechL()
    {
     
   RDebug::Print( _L( "ProEng Validation test TextToSpeechL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsTextToSpeechL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
 
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsTextToSpeechL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsTextToSpeechL begin" ) );
    MProEngToneSettings& toneSettings( aProfile.ToneSettings() );
    TBool tts( toneSettings.TextToSpeech() );
    RDebug::Print( _L( "ProEng Validation test TextToSpeech =%d" ), tts);    
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::SetRingingType
// Test:    MProEngToneSettings::SetRingingType(TProfileRingingType)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::SetRingingTypeL()
    {
     
   RDebug::Print( _L( "ProEng Validation test SetRingingTypeL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsSetRingingTypeL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
 
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsSetRingingTypeL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsSetRingingTypeL begin" ) );
    MProEngToneSettings& toneSettings( aProfile.ToneSettings() );
    TProfileRingingType rtype( toneSettings.RingingType() );
    toneSettings.SetRingingType( rtype );
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::SetKeypadVolumeL
// Test:    MProEngToneSettings::SetKeypadVolumeL(TProfileKeypadVolume)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::SetKeypadVolumeL()
    {
     
   RDebug::Print( _L( "ProEng Validation test SetKeypadVolumeL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsSetKeypadVolumeL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
 
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsSetKeypadVolumeL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsSetKeypadVolumeL begin" ) );
    MProEngToneSettings& toneSettings( aProfile.ToneSettings() );
    TProfileKeypadVolume keyvol( toneSettings.KeypadVolume() );
    toneSettings.SetKeypadVolume( keyvol );    
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::SetRingingVolumeL
// Test:    MProEngToneSettings::RingingVolumeL(TProfileRingingVolume)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::SetRingingVolumeL()
    {
     
   RDebug::Print( _L( "ProEng Validation test SetRingingVolumeL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsSetRingingVolumeL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
 
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsSetRingingVolumeL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsSetRingingVolumeL begin" ) );
    MProEngToneSettings& toneSettings( aProfile.ToneSettings() );
    TProfileRingingVolume ringvol( toneSettings.RingingVolume() );
    toneSettings.SetRingingVolume( ringvol );    
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::SetVibratingAlertL
// Test:    MProEngToneSettings::SetVibratingAlertL(TBool)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::SetVibratingAlertL()
    {
     
   RDebug::Print( _L( "ProEng Validation test SetVibratingAlertL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsSetVibratingAlertL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
 
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsSetVibratingAlertL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsSetVibratingAlertL begin" ) );
    MProEngToneSettings& toneSettings( aProfile.ToneSettings() );
    TBool vibra( toneSettings.VibratingAlert() );
    toneSettings.SetVibratingAlert( vibra );    
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::SetWarningAndGameTonesL
// Test:    MProEngToneSettings::WarningAndGameTonesL(TBool)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::SetWarningAndGameTonesL()
    {
     
   RDebug::Print( _L( "ProEng Validation test SetWarningAndGameTonesL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsSetWarningAndGameTonesL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsSetWarningAndGameTonesL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsSetWarningAndGameTonesL begin" ) );
    MProEngToneSettings& toneSettings( aProfile.ToneSettings() );
    TBool warntones( toneSettings.WarningAndGameTones() );
    toneSettings.SetWarningAndGameTones( warntones );    
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::SetTextToSpeechL
// Test:    MProEngToneSettings::SetTextToSpeechL(TBool)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::SetTextToSpeechL()
    {
     
   RDebug::Print( _L( "ProEng Validation test SetTextToSpeechL" ) );
   MProEngEngine* engine = ProEngFactory::NewEngineLC( iFs );
   MProEngProfile* prof = NULL;
   TRAP_IGNORE( prof = engine->ProfileL( EProfileGeneralId ) );
   if( prof )
       {
       CleanupReleasePushL( *prof );
       TRAP_IGNORE( TestProfileSettingsSetTextToSpeechL( *prof ) );
       CleanupStack::PopAndDestroy( prof );
       prof = NULL;
       }
   CleanupStack::PopAndDestroy(); // engine
   return KErrNone;
    }

void CProEngWrapAPI::TestProfileSettingsSetTextToSpeechL(MProEngProfile& aProfile)
    {
    RDebug::Print( _L( "ProEng Validation test TestProfileSettingsSetTextToSpeechL begin" ) );
    MProEngToneSettings& toneSettings( aProfile.ToneSettings() );
    TBool tts( toneSettings.TextToSpeech() );
    toneSettings.SetTextToSpeech( tts );    
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::NewAlertToneSeekerL
// Test:    ProEngFactory::NewAlertToneSeekerL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::NewAlertToneSeekerL()
    {
       RDebug::Print( _L( "ProEng Validation test NewAlertToneSeekerL" ) );
       MProEngAlertToneSeeker* seeker( NULL );
       CProEngVTAlertToneSeekerWrapper* wrapper = new ( ELeave )
           CProEngVTAlertToneSeekerWrapper();
       CleanupStack::PushL( wrapper );
       TRAP_IGNORE( seeker = ProEngFactory::NewAlertToneSeekerL() );
       if( seeker )
           {
           wrapper->SetObject( seeker ); //  takes ownership
           }
       CleanupStack::PopAndDestroy( wrapper );
       return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::FetchAlertToneListL
// Test : MProEngAlertToneSeeker::FetchAlertToneListL(MProEngAlertToneSeekerObserver&)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::FetchAlertToneListL()
    {
       RDebug::Print( _L( "ProEng Validation test FetchAlertToneListL" ) );
       MProEngAlertToneSeeker* seeker( NULL );
       CProEngVTAlertToneSeekerWrapper* wrapper = new ( ELeave )
           CProEngVTAlertToneSeekerWrapper();
       CleanupStack::PushL( wrapper );
       TRAP_IGNORE( seeker = ProEngFactory::NewAlertToneSeekerL() );
       if( seeker )
           {
           wrapper->SetObject( seeker ); //  takes ownership
           }
       CProEngVTSeekerObserver* observer = new ( ELeave ) CProEngVTSeekerObserver(*iSchedulerUtility);
       CleanupStack::PushL( observer );
       TRAP_IGNORE( seeker->FetchAlertToneListL( *observer ) );
       iSchedulerUtility->Start();
       CleanupStack::PopAndDestroy( observer );
       CleanupStack::PopAndDestroy( wrapper );
       return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngWrapAPI::CancelFetchL
// Test : MProEngAlertToneSeeker::CancelFetch()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProEngWrapAPI::CancelFetchL()
    {
       RDebug::Print( _L( "ProEng Validation test CancelFetchL" ) );
       MProEngAlertToneSeeker* seeker( NULL );
       CProEngVTAlertToneSeekerWrapper* wrapper = new ( ELeave )
           CProEngVTAlertToneSeekerWrapper();
       CleanupStack::PushL( wrapper );
       TRAP_IGNORE( seeker = ProEngFactory::NewAlertToneSeekerL() );
       if( seeker )
           {
           wrapper->SetObject( seeker ); //  takes ownership
           }
       CProEngVTSeekerObserver* observer = new ( ELeave ) CProEngVTSeekerObserver(*iSchedulerUtility);
       CleanupStack::PushL( observer );
       TRAP_IGNORE( seeker->FetchAlertToneListL( *observer ) );
       iSchedulerUtility->Start();
       seeker->CancelFetch();
       CleanupStack::PopAndDestroy( observer );
       CleanupStack::PopAndDestroy( wrapper );
       return KErrNone;
    }

TInt CProEngWrapAPI::NewEngineLWithRFsL()
	{
    MProEngEngine* engine = ProEngFactory::NewEngineL( iFs );
    if( engine )
    	{
    	return KErrNone;
    	}
    return KErrGeneral;
	}

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
