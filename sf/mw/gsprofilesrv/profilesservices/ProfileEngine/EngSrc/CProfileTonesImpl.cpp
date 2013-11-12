/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the CProfileTonesImpl
*
*/



// INCLUDE FILES
#include "CProfileTonesImpl.h"
#include "ProfileEngUtils.h"
#include "ProfileEnginePrivateCRKeys.h"

#include <s32strm.h>
#include <centralrepository.h>
#include <hwrmvibrasdkcrkeys.h>

#include <psmsettings.h>
#include <psmsrvdomaincrkeys.h>
#include <ProfileEngineInternalCRKeys.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileTonesImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileTonesImpl::ConstructL()
    {
    const TDesC& des = KNullDesC();
    iRingingTone1 = des.AllocL();
    iRingingTone2 = des.AllocL();
    iMessageAlertTone = des.AllocL();
    }

// -----------------------------------------------------------------------------
// CProfileTonesImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileTonesImpl::ConstructL(
    const MProfileTones& aProfileTones )
    {
    iRingingTone1 = aProfileTones.RingingTone1().AllocL();
    iRingingTone2 = aProfileTones.RingingTone1().AllocL();
    iMessageAlertTone = aProfileTones.MessageAlertTone().AllocL();
    iProfileToneSettings = aProfileTones.ToneSettings();
    }

// -----------------------------------------------------------------------------
// CProfileTonesImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileTonesImpl* CProfileTonesImpl::NewL()
    {
    CProfileTonesImpl* self = new( ELeave ) CProfileTonesImpl;

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// -----------------------------------------------------------------------------
// CProfileTonesImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileTonesImpl* CProfileTonesImpl::NewL(
    const MProfileTones& aProfileTones )
    {
    CProfileTonesImpl* self = new( ELeave ) CProfileTonesImpl;

    CleanupStack::PushL( self );
    self->ConstructL( aProfileTones );
    CleanupStack::Pop();

    return self;
    }

// Destructor
CProfileTonesImpl::~CProfileTonesImpl()
    {
    delete iRingingTone1;
    delete iRingingTone2;
    delete iMessageAlertTone;
    }


// -----------------------------------------------------------------------------
// CProfileTonesImpl::InternalizeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileTonesImpl::InternalizeL( CRepository& aCenRep, TInt aProfileId )
    {
    delete iRingingTone1;
    iRingingTone1 = NULL;
    delete iRingingTone2;
    iRingingTone2 = NULL;
    delete iMessageAlertTone;
    iMessageAlertTone = NULL;
    TFileName toneFile;

    // Ringing tone 1
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngRingingTone,
            aProfileId ), toneFile ) );
    iRingingTone1 = toneFile.AllocL();

    // Ringing tone 2
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngRingingTone2,
            aProfileId ), toneFile ) );
    iRingingTone2 = toneFile.AllocL();

    // Message alert tone
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngMsgTone,
            aProfileId ), toneFile ) );
    iMessageAlertTone = toneFile.AllocL();

    // Settings
    TInt value( 0 );
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngRingingType,
            aProfileId ), value ) );
    iProfileToneSettings.iRingingType = TProfileRingingType( value );
    
    //Since 10.1, check if silence mode
    User::LeaveIfError(
            aCenRep.Get( KProEngSilenceMode , value) );
    if ( value )
        {
        iProfileToneSettings.iRingingType = EProfileRingingTypeSilent;
        }
    
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngKeypadVolume,
            aProfileId ), value ) );
    iProfileToneSettings.iKeypadVolume = TProfileKeypadVolume( value );
 /* 
  *   User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngRingingVolume,
            aProfileId ), iProfileToneSettings.iRingingVolume ) );
     */
    
    //Since 10.1, Only master volume used.
    User::LeaveIfError(
            aCenRep.Get( KProEngMasterVolume , iProfileToneSettings.iRingingVolume ) );
    
    
    /*
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngVibra,
            aProfileId ), iProfileToneSettings.iVibratingAlert ) );
    */
    
    //Since 10.1,Only master vibra used
    User::LeaveIfError(
            aCenRep.Get( KProEngMasterVibra, iProfileToneSettings.iVibratingAlert ) );
            
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngWarnAndGameTones,
            aProfileId ), iProfileToneSettings.iWarningAndGameTones ) );
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngMediaVolume,
            aProfileId ), iProfileToneSettings.iMediaVolume ) );
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngTextToSpeechTone,
            aProfileId ), iProfileToneSettings.iTextToSpeech ) );
            
    InitPowerSaveMode();

    if ( iPowerSaveMode )
        {
        iProfileToneSettings.iVibratingAlert = iPsmConfigVibra;
        iProfileToneSettings.iKeypadVolume = 
                             TProfileKeypadVolume( iPsmConfigKeypadVolume );
        }
    }

// -----------------------------------------------------------------------------
// CProfileTonesImpl::ExternalizeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileTonesImpl::ExternalizeL( CRepository& aCenRep, TInt aProfileId ) const
    {
    // Ringing tone 1
    User::LeaveIfError(
            aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngRingingTone,
            aProfileId ), *iRingingTone1 ) );

    // Ringing tone 2
    User::LeaveIfError(
            aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngRingingTone2,
            aProfileId ), *iRingingTone2 ) );

    // Message alert tone
    User::LeaveIfError(
            aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngMsgTone,
            aProfileId ), *iMessageAlertTone ) );

    // Tone Settings
    User::LeaveIfError(
            aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngRingingType,
            aProfileId ), iProfileToneSettings.iRingingType ) );
    User::LeaveIfError(
            aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngKeypadVolume,
            aProfileId ), iProfileToneSettings.iKeypadVolume ) );
    User::LeaveIfError(
            aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngRingingVolume,
            aProfileId ), iProfileToneSettings.iRingingVolume ) );
    
    if ( !ProfileEngUtils::PowerSaveMode() )
        {
        User::LeaveIfError(
                aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngKeypadVolume,
                aProfileId ), iProfileToneSettings.iKeypadVolume ) );
        User::LeaveIfError(
                aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngVibra,
                aProfileId ), iProfileToneSettings.iVibratingAlert ) );        
        }
    
    User::LeaveIfError(
            aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngWarnAndGameTones,
            aProfileId ), iProfileToneSettings.iWarningAndGameTones ) );
    User::LeaveIfError(
            aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngMediaVolume,
            aProfileId ), iProfileToneSettings.iMediaVolume ) );
    User::LeaveIfError(
            aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngTextToSpeechTone,
            aProfileId ), iProfileToneSettings.iTextToSpeech ) );
    }

// -----------------------------------------------------------------------------
// CProfileTonesImpl::SetRingingTone2L
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileTonesImpl::SetRingingTone2L(
    const TDesC& aRingingTone )
    {
    HBufC* tmp = aRingingTone.Left( KMaxFileName ).AllocL();
    delete iRingingTone2;
    iRingingTone2 = tmp;
    }

// -----------------------------------------------------------------------------
// CProfileTonesImpl::SetRingingTone1L
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileTonesImpl::SetRingingTone1L(
    const TDesC& aRingingTone )
    {
    HBufC* tmp = aRingingTone.Left( KMaxFileName ).AllocL();
    delete iRingingTone1;
    iRingingTone1 = tmp;
    }

// -----------------------------------------------------------------------------
// CProfileTonesImpl::MessageAlertTone
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& CProfileTonesImpl::MessageAlertTone() const
    {
    return *iMessageAlertTone;
    }

// -----------------------------------------------------------------------------
// CProfileTonesImpl::RingingTone1
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& CProfileTonesImpl::RingingTone1() const
    {
    return *iRingingTone1;
    }

// -----------------------------------------------------------------------------
// CProfileTonesImpl::RingingTone2
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& CProfileTonesImpl::RingingTone2() const
    {
    return *iRingingTone2;
    }

// -----------------------------------------------------------------------------
// CProfileTonesImpl::SetMessageAlertToneL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileTonesImpl::SetMessageAlertToneL(
    const TDesC& aMessageAlertTone )
    {
    HBufC* tmp = aMessageAlertTone.Left( KMaxFileName ).AllocL();
    delete iMessageAlertTone;
    iMessageAlertTone = tmp;
    }

// -----------------------------------------------------------------------------
// CProfileTonesImpl::ToneSettings
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TProfileToneSettings& CProfileTonesImpl::ToneSettings() const
    {
    return iProfileToneSettings;
    }

// -----------------------------------------------------------------------------
// CProfileTonesImpl::SetToneSettings
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TProfileToneSettings& CProfileTonesImpl::SetToneSettings()
    {
    return iProfileToneSettings;
    }


// -----------------------------------------------------------------------------
// CProfileTonesImpl::InitPowerSaveMode
//
// -----------------------------------------------------------------------------
//
void CProfileTonesImpl::InitPowerSaveMode()
    {
    const TInt KVibraOff = 0;
    const TInt KMinVolume = 0;

    iPsmConfigVibra = KVibraOff;
    iPsmConfigKeypadVolume = KMinVolume;

    iPowerSaveMode = ProfileEngUtils::PowerSaveMode();
    
    if ( iPowerSaveMode )
        {
        TRAP_IGNORE( ReadPowerSaveConfigL() );
        }
    }


// -----------------------------------------------------------------------------
// CProfileTonesImpl::ReadPowerSaveConfigL
//
// -----------------------------------------------------------------------------
//
void CProfileTonesImpl::ReadPowerSaveConfigL()
    {

    //
    // Read psm specific vibra and keypad volume from profiles public cenrep.
    // Psm plugin updates those when psm mode changes.
    //
    
    CRepository* cenrep = CRepository::NewLC( KCRUidVibraCtrl );
    User::LeaveIfError( cenrep->Get( KVibraCtrlProfileVibraEnabled, iPsmConfigVibra ) );
    CleanupStack::PopAndDestroy( cenrep );
    
    cenrep = CRepository::NewLC( KCRUidProfileEngine );
    User::LeaveIfError( cenrep->Get( KProEngActiveKeypadVolume, iPsmConfigKeypadVolume ) );
    CleanupStack::PopAndDestroy( cenrep );
    }
    
//  End of File
