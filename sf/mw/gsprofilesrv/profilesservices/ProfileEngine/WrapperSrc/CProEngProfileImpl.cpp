/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of CProEngProfileImpl.
*
*/



// INCLUDE FILES
#include    "CProEngProfileImpl.h"
#include    "CProEngProfileNameImpl.h"
#include    "CProEngTonesImpl.h"
#include    "CProEngToneSettingsImpl.h"
#include    "CProEngEngineImpl.h"
#include    <MProfileExtended.h>
#include    <MProfileEngineExtended.h>
#include    <MProfileSetTones.h>
#include    <MProfileExtended.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngProfileImpl::CProEngProfileImpl
// -----------------------------------------------------------------------------
//
CProEngProfileImpl::CProEngProfileImpl( CProEngEngineImpl& aProfileEngine,
                                        MProfileExtended* aProfile )
    : iProfileEngine( aProfileEngine ), iProfile( aProfile )
    {
    }

// -----------------------------------------------------------------------------
// CProEngProfileImpl::ConstructL
// -----------------------------------------------------------------------------
//
void CProEngProfileImpl::ConstructL()
    {
    TUint32 flags = iProfile->ModifiableFlags();
    iProfileName = CProEngProfileNameImpl::NewL(
            iProfile->ProfileName(),
            iProfile->ProfileSetName(),
            iProfileEngine,
            flags & EProfileFlagProfileName );
    iTones = CProEngTonesImpl::NewL( 
            iProfile->ProfileTones(),
            iProfile->ProfileSetTones(),
            iProfile->ProfileExtraTones(),
            iProfile->ProfileSetExtraTones(),
            flags );
    iToneSettings = CProEngToneSettingsImpl::NewL(
            iProfile->ProfileSetTones().SetToneSettings(),
            flags );
    }

// -----------------------------------------------------------------------------
// CProEngProfileImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProEngProfileImpl* CProEngProfileImpl::NewL(
        CProEngEngineImpl& aProfileEngine,
        MProfileExtended* aProfile )
    {
    CProEngProfileImpl* self = new ( ELeave ) CProEngProfileImpl(
            aProfileEngine, aProfile );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// Destructor
CProEngProfileImpl::~CProEngProfileImpl()
    {
    delete iToneSettings;
    delete iTones;
    delete iProfileName;
    if( iProfile )
        {
        iProfile->Release();
        }
    }

// -----------------------------------------------------------------------------
// CProEngProfileImpl::Release
// -----------------------------------------------------------------------------
//
void CProEngProfileImpl::Release()
    {
    delete this;
    }

// -----------------------------------------------------------------------------
// CProEngProfileImpl::AlertForL
// -----------------------------------------------------------------------------
//
const TArray<TContactItemId> CProEngProfileImpl::AlertForL()
    {
    return iProfile->AlertForL();
    }

// -----------------------------------------------------------------------------
// CProEngProfileImpl::SetAlertForL
// -----------------------------------------------------------------------------
//
TInt CProEngProfileImpl::SetAlertForL( const TArray<TContactItemId>& aAlertFor )
    {
    TRAPD( err, iProfile->SetAlertForL( aAlertFor ) );
    if( err == KErrArgument )
        {
        return err;
        }

    User::LeaveIfError( err );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngProfileImpl::IsSilent
// -----------------------------------------------------------------------------
//
TBool CProEngProfileImpl::IsSilent() const
    {
    return iProfile->IsSilent();
    }

// -----------------------------------------------------------------------------
// CProEngProfileImpl::ProfileName
// -----------------------------------------------------------------------------
//
MProEngProfileName& CProEngProfileImpl::ProfileName() const
    {
    return *iProfileName;
    }

// -----------------------------------------------------------------------------
// CProEngProfileImpl::ProfileTones
// -----------------------------------------------------------------------------
//
MProEngTones& CProEngProfileImpl::ProfileTones() const
    {
    return *iTones;
    }

// -----------------------------------------------------------------------------
// CProEngProfileImpl::ToneSettings
// -----------------------------------------------------------------------------
//
MProEngToneSettings& CProEngProfileImpl::ToneSettings() const
    {
    return *iToneSettings;
    }

// -----------------------------------------------------------------------------
// CProEngProfileImpl::CommitChangeL
// -----------------------------------------------------------------------------
//
void CProEngProfileImpl::CommitChangeL()
    {
    if( iProfileName->AlreadyExistsL( iProfileName->Name() ) )
        {
        User::Leave( KErrAlreadyExists );
        }

    User::LeaveIfError( iTones->CheckTonesL( iProfileEngine.ToneHandler() ) );

    // Read the current tones of this profile for automated content removal
    CProEngProfileImpl* current =
        iProfileEngine.ProfilePrivateLC( iProfileName->Id() );

    // Read the current settings (as internal interface) for roll back in case
    // HandleAutomatedContentL() fails:
    MProfileExtended* currentInternal =
        iProfileEngine.ProfileEngineExtended().ProfileLC( iProfileName->Id() );

    iProfileEngine.ProfileEngineExtended().CommitChangeL( *iProfile );

    TInt err( iTones->HandleAutomatedContent( iProfileEngine.ToneHandler(),
                                              *( current->iTones ) ) );
    if( err )
        { // Roll back the original settings:
        iProfileEngine.ProfileEngineExtended().CommitChangeL(
                *currentInternal );
        }

    CleanupStack::PopAndDestroy( 2 ); // currentInternal, current
    }

//  End of File

