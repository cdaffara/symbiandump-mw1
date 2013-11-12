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
* Description:  Implementation of the CProfileFeedbackSettingsImpl.
*
*/



// INCLUDE FILES
#include "CProfileFeedbackSettingsImpl.h"
#include "ProfileEngUtils.h"
#include "ProfileEnginePrivateCRKeys.h"
#include "ProfilesDebug.h"
#include <centralrepository.h>

#include <hwrmvibrasdkcrkeys.h>
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileFeedbackSettingsImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileFeedbackSettingsImpl* CProfileFeedbackSettingsImpl::NewL()
    {
    CProfileFeedbackSettingsImpl* self = new( ELeave ) CProfileFeedbackSettingsImpl;
    return self;
    }

// -----------------------------------------------------------------------------
// CProfileFeedbackSettingsImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileFeedbackSettingsImpl* CProfileFeedbackSettingsImpl::NewL(
    const MProfileFeedbackSettings& aProfileFeedbackSettings )
    {
    CProfileFeedbackSettingsImpl* self = new( ELeave ) CProfileFeedbackSettingsImpl;

    self->iTactileFeedback = aProfileFeedbackSettings.TactileFeedback();
    self->iAudioFeedback = aProfileFeedbackSettings.AudioFeedback();

    return self;
    }


// Destructor
CProfileFeedbackSettingsImpl::~CProfileFeedbackSettingsImpl()
    {
    }


// -----------------------------------------------------------------------------
// CProfileFeedbackSettingsImpl::InternalizeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileFeedbackSettingsImpl::InternalizeL( CRepository& aCenRep,
                                               TInt aProfileId )
    {
    PRODEBUG( "CProfileFeedbackSettings::InternalizeL begin" );
    
    InitPowerSaveMode();

    if ( iPowerSaveMode )
        {
        iTactileFeedback = TProfileTactileFeedback( iPsmConfigTactileFeedback );
        iAudioFeedback =  TProfileAudioFeedback( iPsmConfigAudioFeedback );
        return;
        }
        
    TInt tmp( 0 );
    // Tactile feedback
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngTactileFeedback,
            aProfileId ), tmp ) );
    iTactileFeedback = TProfileTactileFeedback( tmp );

    // Audio feedback
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngAudioFeedback,
            aProfileId ), tmp ) );
    iAudioFeedback = TProfileAudioFeedback( tmp );
    PRODEBUG( "CProfileFeedbackSettings::InternalizeL return" );
    }

// -----------------------------------------------------------------------------
// CProfileFeedbackSettingsImpl::ExternalizeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileFeedbackSettingsImpl::ExternalizeL( CRepository& aCenRep,
                                               TInt aProfileId ) const
    {
    if ( ProfileEngUtils::PowerSaveMode() )
        {
        return;
        }
        
    // Tactile feedback
    User::LeaveIfError(
            aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngTactileFeedback,
            aProfileId ), iTactileFeedback ) );

    // Audio feedback
    User::LeaveIfError(
            aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngAudioFeedback,
            aProfileId ), iAudioFeedback ) );

    }

// -----------------------------------------------------------------------------
// CProfileFeedbackSettingsImpl::TactileFeedback
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TProfileTactileFeedback CProfileFeedbackSettingsImpl::TactileFeedback() const
    {
    return iTactileFeedback;
    }

// -----------------------------------------------------------------------------
// CProfileFeedbackSettingsImpl::SetTactileFeedback
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileFeedbackSettingsImpl::SetTactileFeedback( TProfileTactileFeedback aTactileFeedback )
    {
    iTactileFeedback = aTactileFeedback;
    }

// -----------------------------------------------------------------------------
// CProfileFeedbackSettingsImpl::AudioFeedback
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TProfileAudioFeedback CProfileFeedbackSettingsImpl::AudioFeedback() const
    {
    return iAudioFeedback;
    }

// -----------------------------------------------------------------------------
// CProfileFeedbackSettingsImpl::SetAudioFeedback
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileFeedbackSettingsImpl::SetAudioFeedback( TProfileAudioFeedback aAudioFeedback )
    {
    iAudioFeedback = aAudioFeedback;
    }

// -----------------------------------------------------------------------------
// CProfileFeedbackSettingsImpl::InitPowerSaveMode
//
// -----------------------------------------------------------------------------
//
void CProfileFeedbackSettingsImpl::InitPowerSaveMode()
    {
    const TInt KTactileFeedbackOff = 0;
    const TInt KAudioFeedbackOff = 0;

    iPsmConfigTactileFeedback = KTactileFeedbackOff;
    iPsmConfigAudioFeedback = KAudioFeedbackOff;

    iPowerSaveMode = ProfileEngUtils::PowerSaveMode();
    
    if ( iPowerSaveMode )
        {
        TRAP_IGNORE( ReadPowerSaveConfigL() );
        }
    }


// -----------------------------------------------------------------------------
// CProfileFeedbackSettingsImpl::ReadPowerSaveConfigL
//
// -----------------------------------------------------------------------------
//
void CProfileFeedbackSettingsImpl::ReadPowerSaveConfigL()
    {
    CRepository* cenrep = CRepository::NewLC( KCRUidVibraCtrl );
    User::LeaveIfError( cenrep->Get( KVibraCtrlProfileFeedbackEnabled,
                                     iPsmConfigTactileFeedback ) );
    CleanupStack::PopAndDestroy( cenrep );
    
    
    cenrep = CRepository::NewLC( KCRUidProfileEngine );
    User::LeaveIfError( cenrep->Get( KProEngActiveAudioFeedback,
                                     iPsmConfigAudioFeedback ) );
    CleanupStack::PopAndDestroy( cenrep );
    }


//  End of File

