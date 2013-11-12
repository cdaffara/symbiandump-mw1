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
* Description:  Implementation of the CProEngToneSettingsImpl.
*
*/



// INCLUDE FILES
#include    "CProEngToneSettingsImpl.h"
#include    <MProfileTones.h>
#include    <MProfileExtraTones.h>
#include    <MProfileSetTones.h>
#include    <MProfileSetExtraTones.h>
#include    <MProfileExtended.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngToneSettingsImpl::CProEngToneSettingsImpl
// -----------------------------------------------------------------------------
//
CProEngToneSettingsImpl::CProEngToneSettingsImpl(
        TProfileToneSettings& aToneSettings,
        TUint32 aModifiableFlags )
    : iToneSettings( aToneSettings ), iModifiableFlags( aModifiableFlags )
    {
    }

// -----------------------------------------------------------------------------
// CProEngToneSettingsImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProEngToneSettingsImpl* CProEngToneSettingsImpl::NewL(
        TProfileToneSettings& aToneSettings,
        TUint32 aModifiableFlags )
    {
    return new ( ELeave ) CProEngToneSettingsImpl(
            aToneSettings, aModifiableFlags );
    }

// Destructor
CProEngToneSettingsImpl::~CProEngToneSettingsImpl()
    {
    }

// -----------------------------------------------------------------------------
// CProEngToneSettingsImpl::RingingType
// -----------------------------------------------------------------------------
//
TProfileRingingType CProEngToneSettingsImpl::RingingType() const
    {
    return iToneSettings.iRingingType;
    }

// -----------------------------------------------------------------------------
// CProEngToneSettingsImpl::KeypadVolume
// -----------------------------------------------------------------------------
//
TProfileKeypadVolume CProEngToneSettingsImpl::KeypadVolume() const
    {
    return iToneSettings.iKeypadVolume;
    }

// -----------------------------------------------------------------------------
// CProEngToneSettingsImpl::
// -----------------------------------------------------------------------------
//
TProfileRingingVolume CProEngToneSettingsImpl::RingingVolume() const
    {
    TProfileRingingVolume volume =
        TProfileRingingVolume( iToneSettings.iRingingVolume );
    return volume;
    }

// -----------------------------------------------------------------------------
// CProEngToneSettingsImpl::
// -----------------------------------------------------------------------------
//
TBool CProEngToneSettingsImpl::VibratingAlert() const
    {
    return iToneSettings.iVibratingAlert;
    }

// -----------------------------------------------------------------------------
// CProEngToneSettingsImpl::
// -----------------------------------------------------------------------------
//
TBool CProEngToneSettingsImpl::WarningAndGameTones() const
    {
    return iToneSettings.iWarningAndGameTones;
    }

// -----------------------------------------------------------------------------
// CProEngToneSettingsImpl::
// -----------------------------------------------------------------------------
//
TBool CProEngToneSettingsImpl::TextToSpeech() const
    {
    return iToneSettings.iTextToSpeech;
    }

// -----------------------------------------------------------------------------
// CProEngToneSettingsImpl::
// -----------------------------------------------------------------------------
//
TInt CProEngToneSettingsImpl::SetRingingType( TProfileRingingType aRingingType )
    {
    if( iModifiableFlags & EProfileFlagRingingType )
        {
        iToneSettings.iRingingType = aRingingType;
        return KErrNone;
        }
    return KErrAccessDenied;
    }

// -----------------------------------------------------------------------------
// CProEngToneSettingsImpl::
// -----------------------------------------------------------------------------
//
TInt CProEngToneSettingsImpl::SetKeypadVolume(
        TProfileKeypadVolume aKeypadVolume )
    {
    if( iModifiableFlags & EProfileFlagKeypadVolume )
        {
        iToneSettings.iKeypadVolume = aKeypadVolume;
        return KErrNone;
        }
    return KErrAccessDenied;
    }

// -----------------------------------------------------------------------------
// CProEngToneSettingsImpl::
// -----------------------------------------------------------------------------
//
TInt CProEngToneSettingsImpl::SetRingingVolume(
        TProfileRingingVolume aRingingVolume )
    {
    if( iModifiableFlags & EProfileFlagRingingVolume )
        {
        iToneSettings.iRingingVolume = aRingingVolume;
        return KErrNone;
        }
    return KErrAccessDenied;
    }

// -----------------------------------------------------------------------------
// CProEngToneSettingsImpl::
// -----------------------------------------------------------------------------
//
TInt CProEngToneSettingsImpl::SetVibratingAlert( TBool aVibratingAlert )
    {
    if( iModifiableFlags & EProfileFlagVibratingAlert )
        {
        iToneSettings.iVibratingAlert = aVibratingAlert;
        return KErrNone;
        }
    return KErrAccessDenied;
    }

// -----------------------------------------------------------------------------
// CProEngToneSettingsImpl::
// -----------------------------------------------------------------------------
//
TInt CProEngToneSettingsImpl::SetWarningAndGameTones( TBool aWarningAndGameTones )
    {
    if( iModifiableFlags & EProfileFlagWarningTones )
        {
        iToneSettings.iWarningAndGameTones = aWarningAndGameTones;
        return KErrNone;
        }
    return KErrAccessDenied;
    }

// -----------------------------------------------------------------------------
// CProEngToneSettingsImpl::
// -----------------------------------------------------------------------------
//
TInt CProEngToneSettingsImpl::SetTextToSpeech( TBool aTextToSpeech )
    {
    if( iModifiableFlags & EProfileFlagTextToSpeechTone )
        {
        iToneSettings.iTextToSpeech = aTextToSpeech;
        return KErrNone;
        }
    return KErrAccessDenied;
    }


//  End of File

