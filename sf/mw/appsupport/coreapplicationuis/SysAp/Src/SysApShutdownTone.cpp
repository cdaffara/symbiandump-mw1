/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  
*     This class is the container class of the CSysApShutdownTone.
*     Is used to play shutdown tone.
*
*/


// INCLUDE FILES
#include <Profile.hrh>
#include <centralrepository.h>
#include <startupdomaincrkeys.h>
#include <ProfileEngineSDKCRKeys.h>
#include "SysApShutdownTone.h"
#include "SysApShutdownAnimation.h"
#include "SysAp.hrh"
#include "AudioPreference.h"

//=============================== MEMBER FUNCTIONS ============================
// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CSysApShutdownTone::CSysApShutdownTone( CSysApShutdownAnimation* aCallback ) :
    iCallback( aCallback ),
    iTone( NULL ),
    iAudioReady( EFalse ),
    iPlaying( EFalse )
    {
    }

// ---------------------------------------------------------
// Destructor 
// ---------------------------------------------------------
//
CSysApShutdownTone::~CSysApShutdownTone()
{
    TRACES( RDebug::Print( _L("CSysApShutdownTone::~CSysApShutdownTone()" )) );

    if (iTone)
        {
        if (iPlaying)
            {
            TRACES( RDebug::Print( _L("CSysApShutdownTone: Still playing shutdown tone. Stop it!" )) );
            iTone->Stop();
            }
        delete iTone;
        iTone = NULL;
        }
}

// ----------------------------------------------------
// CSysApShutdownTone::NewL()
// ----------------------------------------------------
CSysApShutdownTone* CSysApShutdownTone::NewL(TDesC& aShutdownTonePath, CSysApShutdownAnimation* aCallback)
{
    CSysApShutdownTone* self = new (ELeave) CSysApShutdownTone(aCallback);
    
    CleanupStack::PushL( self );
    self->ConstructL(aShutdownTonePath);
    CleanupStack::Pop(); // self

    return self;
}

// ----------------------------------------------------
// CSysApShutdownTone::ConstructL()
// ----------------------------------------------------
void CSysApShutdownTone::ConstructL(TDesC& aShutdownTonePath)
{
    TRACES( RDebug::Print( _L("CSysApShutdownTone::ConstructL()" )) );
    // Check tone volume
    iVolume = GetRingingToneVolumeL();
    TInt err( KErrNone );

    if (iVolume)
        {
        TPath tonePath;
        TRACES( RDebug::Print( _L( "CSysApShutdownTone::ConstructL(): Get tone to play, Path = %S"), &aShutdownTonePath));

        RFs fs;
        err = fs.Connect();
        TFindFile findExe(fs);
        err = findExe.FindByPath( aShutdownTonePath, NULL );
        fs.Close();
        if (err != KErrNone)
            {
            TRACES( RDebug::Print( _L( "CSysApShutdownTone::ConstructL(): Tone to play: Cannot find tone. err = %d"), err));
            User::Leave(err);
            }
        else
            {
            TRACES( RDebug::Print( _L( "CSysApShutdownTone::ConstructL(): Tone found")));
            iTone = CMdaAudioPlayerUtility::NewFilePlayerL(
                        aShutdownTonePath, 
                        *this, KAudioPriorityPhonePower, 
                        TMdaPriorityPreference( KAudioPrefDefaultTone));
            }
        }
    else // Phone is in silent mode, tone can be set to be ready instantly
        {
        TRACES( RDebug::Print( _L( "CSysApShutdownTone::ConstructL(): No tone initialization performed, signaling callback instantly")));
        
        if ( iCallback )
            {
            iCallback->ToneReady();
            }
        }            
}
// ---------------------------------------------------------
// void CSysApShutdownTone::Play()
// ---------------------------------------------------------
//

TInt CSysApShutdownTone::Play()
{
    TRACES( RDebug::Print( _L("CSysApShutdownTone::Play()" )) );
    if (iAudioReady && iVolume && iTone)
        {
        TRACES( RDebug::Print( _L("CSysApShutdownTone::Play(): Audio ready. Play tone." )) );
        iVolume = Max( 0, Min( iVolume, 10 ) );
        iTone->SetVolume(iVolume);
        iTone->Play();
        iPlaying = ETrue;
        return KErrNone;
        }
    else
        {
        TRACES( RDebug::Print( _L( "CSysApShutdownTone::Play(): Audio not ready, hidden reset, volume null or tone is not initialized. Unable to play tone.")));
        TRACES( RDebug::Print( _L( "CSysApShutdownTone::Play(): Audio ready:  %d"),iAudioReady));
        TRACES( RDebug::Print( _L( "CSysApShutdownTone::Play(): Volume:       %d"),iVolume));
        TRACES( RDebug::Print( _L( "CSysApShutdownTone::Play(): Tone:         %d"),iTone));
        return KErrNotReady;
        }
}

// ---------------------------------------------------------
// void CSysApShutdownTone::Stop()
// ---------------------------------------------------------
//
void CSysApShutdownTone::Stop()
{
    TRACES( RDebug::Print( _L("CSysApShutdownTone::Stop()" )) );
    if (iTone)
        {
        iPlaying=EFalse;
        iTone->Stop();
        }
    
    // Set null to callback as notifying tone readiness is no longer needed and causes incorrect
    // functionality.
    iCallback = NULL;
}

// ---------------------------------------------------------
// void CSysApShutdownTone::MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds& aDuration)
// ---------------------------------------------------------
//
void CSysApShutdownTone::MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds& /*aDuration*/)
{
    TRACES( RDebug::Print( _L("CSysApShutdownTone::MapcInitComplete(), aError = %d" ),aError ) );
    if (aError == KErrNone)
        {
        TRACES( RDebug::Print( _L("CSysApShutdownTone::MapcInitComplete(): Ready to play shutdown tone." )) );
        iAudioReady = ETrue;
        }
    else
        {
        TRACES( RDebug::Print( _L("CSysApShutdownTone::MapcInitComplete(): Unable to play shutdown tone." )) );
        }
    
    if ( iCallback )
        {
        iCallback->ToneReady();
        }
}

// ---------------------------------------------------------
// void CSysApShutdownTone::MapcPlayComplete(TInt /*aError*/)
// ---------------------------------------------------------
//
void CSysApShutdownTone::MapcPlayComplete(TInt /*aError*/)
{
    TRACES( RDebug::Print( _L("CSysApShutdownTone::MapcPlayComplete()" )) );
    iPlaying=EFalse;
}

// ---------------------------------------------------------
// TBool CSysApShutdownTone::Playing()
// ---------------------------------------------------------
//
TBool CSysApShutdownTone::Playing()
{
    TRACES( RDebug::Print( _L( "CSysApShutdownTone::Playing(): Playing(): %d"), iPlaying ));
    return iPlaying;
}

// ----------------------------------------------------------
// CSysApShutdownTone::GetRingingToneVolumeL
// Shutdown tone volume is always 4 but when ringing type is 
// silent or ringing volume is 0 or 1 startup tone is silent.
// ----------------------------------------------------------
//
TInt CSysApShutdownTone::GetRingingToneVolumeL()
    {
    TRACES( RDebug::Print( _L("CSysApShutdownTone::GetRingingToneVolumeL()" )) );

    TInt retval(0);
    TInt ringingType(EProfileRingingTypeSilent);
    TInt ringingVol(0);
    
    CRepository* repository = NULL;

    TRAPD( err, repository = CRepository::NewL( KCRUidProfileEngine ) );
    if ( err != KErrNone )
        {
        return 0;    
        }
    
    if ( repository )
        {
        CleanupStack::PushL(repository);        
        User::LeaveIfError( repository->Get( KProEngActiveRingingVolume, ringingVol ));
        User::LeaveIfError( repository->Get( KProEngActiveRingingType, ringingType ));
        CleanupStack::PopAndDestroy(repository);
        }

    TRACES( RDebug::Print( _L( "CSysApShutdownTone::GetRingingToneVolumeL(): Ringing tone volume = %d"), ringingVol ));
    TRACES( RDebug::Print( _L( "CSysApShutdownTone::GetRingingToneVolumeL(): Ringing type = %d"), ringingType ));

    if ((ringingType != EProfileRingingTypeSilent) && 
        (ringingVol != 0))
        {
        TInt defaultRingingVol;
        CRepository* repository = NULL;

        TRAPD( err, repository = CRepository::NewL( KCRUidStartupConf ) );
        if ( err != KErrNone )
            {
            return 0;    
            }

        if ( repository )
            {
            CleanupStack::PushL(repository);
            User::LeaveIfError( repository->Get( KStartupToneVolume, defaultRingingVol ));
            CleanupStack::PopAndDestroy(repository);
            }

        ringingVol = defaultRingingVol;
        retval =  ringingVol;
        }

    TRACES( RDebug::Print( _L( "CSysApShutdownTone::GetRingingToneVolumeL(): Shutdown tone volume = %d"), retval ));
    return retval;
    }

// End of File
