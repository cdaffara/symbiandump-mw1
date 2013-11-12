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
* Description:  Implementation of CSAnimMmfPlugin class
*
*/


#include <AudioPreference.h>

#include "sanimmmfplugin.h"
#include "assert.h"
#include "trace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSAnimMmfPlugin::NewL
//
// ---------------------------------------------------------------------------
//
CSAnimMmfPlugin* CSAnimMmfPlugin::NewL()
    {
    FUNC_LOG;

    CSAnimMmfPlugin* self = new( ELeave ) CSAnimMmfPlugin();
    return self;
    }


// ---------------------------------------------------------------------------
// CSAnimMmfPlugin::~CSAnimMmfPlugin
//
// ---------------------------------------------------------------------------
//
CSAnimMmfPlugin::~CSAnimMmfPlugin()
    {
    FUNC_LOG;

    if ( iAudioPlayer )
        {
        iAudioPlayer->Close();
        }
    delete iAudioPlayer;
    }


// ---------------------------------------------------------------------------
// CSAnimMmfPlugin::Load
//
// ---------------------------------------------------------------------------
//
void CSAnimMmfPlugin::Load(
    RFs& /*aFs*/,
    const TDesC& aFileName,
    TRequestStatus& aStatus )
    {
    FUNC_LOG;
    INFO_1( "File name: %S", &aFileName );

    SetClientRequest( aStatus );

    if ( iAudioPlayer )
        {
        iAudioPlayer->Close();
        delete iAudioPlayer;
        iAudioPlayer = NULL;
        }

    TRAPD( errorCode, iAudioPlayer = CMdaAudioPlayerUtility::NewFilePlayerL(
        aFileName,
        *this,
        KAudioPriorityPhonePower,
        TMdaPriorityPreference( KAudioPrefDefaultTone ) ) );
    ERROR( errorCode, "Failed to load tone file" );
    if ( errorCode != KErrNone )
        {
        CompleteClientRequest( errorCode );
        }
    }


// ---------------------------------------------------------------------------
// CSAnimMmfPlugin::Start
//
// ---------------------------------------------------------------------------
//
void CSAnimMmfPlugin::Start( TRequestStatus& aStatus )
    {
    FUNC_LOG;
    ASSERT_TRACE( iAudioPlayer, SAnimPanic::ENotInitialized );

    SetClientRequest( aStatus );

    INFO_2( "Volume: %d, volume ramp: %Ld", iVolume, iVolumeRamp.Int64() );

    iAudioPlayer->SetVolume( iVolume );
    iAudioPlayer->SetVolumeRamp( iVolumeRamp );
    iAudioPlayer->Play();
    }


// ---------------------------------------------------------------------------
// CSAnimMmfPlugin::Cancel
//
// ---------------------------------------------------------------------------
//
void CSAnimMmfPlugin::Cancel()
    {
    FUNC_LOG;

    CompleteClientRequest( KErrCancel );
    if ( iAudioPlayer )
        {
        iAudioPlayer->Close();
        }
    }


// ---------------------------------------------------------------------------
// CSAnimMmfPlugin::MapcInitComplete
//
// ---------------------------------------------------------------------------
//
void CSAnimMmfPlugin::MapcInitComplete(
    TInt aError,
	const TTimeIntervalMicroSeconds& /*aDuration*/ )
	{
    FUNC_LOG;
    ERROR( aError, "Audio init failed" );

    CompleteClientRequest( aError );
	}


// ---------------------------------------------------------------------------
// CSAnimMmfPlugin::MapcPlayComplete
//
// ---------------------------------------------------------------------------
//
void CSAnimMmfPlugin::MapcPlayComplete( TInt aError )
    {
    FUNC_LOG;
    ERROR( aError, "Audio play failed" );

    CompleteClientRequest( aError );
    }


// ---------------------------------------------------------------------------
// CSAnimMmfPlugin::CSAnimMmfPlugin
//
// ---------------------------------------------------------------------------
//
CSAnimMmfPlugin::CSAnimMmfPlugin() : iAudioPlayer( NULL )
    {
    FUNC_LOG;
    }
