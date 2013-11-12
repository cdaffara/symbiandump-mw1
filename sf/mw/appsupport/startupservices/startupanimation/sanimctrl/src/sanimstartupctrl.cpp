/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CSAnimStartupCtrl class
*
*/



 #include <eikappui.h>

#include <centralrepository.h>
#include <featmgr.h>
#include <MediatorDomainUIDs.h>
#include <Profile.hrh>
#include <ProfileEngineSDKCRKeys.h>
#include "sanimengine.h"
#include <SecondaryDisplay/SecondaryDisplayStartupAPI.h>

#include "sanimstartupctrl.h"
#include "trace.h"

const TInt KMinVolume( 0 );  /** Minimum allowed volume level. */
const TInt KMaxVolume( 10000 ); /** Maximum allowed volume level. */

const TInt KDefaultRepeatCount( 1 ); /** Default repeat count for animation and tone. */
const TInt KDefaultVolumeRamp( 0 );  /** Default volume ramp value in microseconds. */

//static const TInt KMediatorTimeout( 1000000 ); /** Default timeout for Mediator commands. */

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// FileExists
//
// ---------------------------------------------------------------------------
//
static TBool FileExists( RFs& aFs, const TDesC& aPath )
    {
    FUNC_LOG;
    INFO_1( "File name: %S", &aPath );

    if ( aPath.Length() > 0 )
        {
        RFile file;
        TInt errorCode = file.Open( aFs, aPath, EFileRead | EFileShareReadersOnly );
        ERROR( errorCode, "File open failed" );

        file.Close();
        return ( errorCode == KErrNone );
        }

    return EFalse;
    }


// ---------------------------------------------------------------------------
// IsSilentL
//
// ---------------------------------------------------------------------------
//
static TBool IsSilentL()
    {
    FUNC_LOG;

    TBool retVal = ETrue;
    CRepository* repository = CRepository::NewLC( KCRUidProfileEngine );

    TInt type( EProfileRingingTypeSilent );
    User::LeaveIfError( repository->Get( KProEngActiveRingingType, type ) );

    INFO_1( "Ringing type: %d", type );

    if ( type != EProfileRingingTypeSilent )
        {
        TInt volume( 0 );
        User::LeaveIfError(
            repository->Get( KProEngActiveRingingVolume, volume ) );

        INFO_1( "Ringing volume: %d", volume );

        if ( volume != 0 && volume != EProfileRingingVolumeLevel1 )
            {
            retVal = EFalse;
            }
        }

    CleanupStack::PopAndDestroy( repository );

    INFO_1( "Is silent: %d", retVal );

    return retVal;
    }


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSAnimStartupCtrl::NewL
//
// ---------------------------------------------------------------------------
//
EXPORT_C CSAnimStartupCtrl* CSAnimStartupCtrl::NewL(
    const TRect& aRect,
    const CCoeControl& aContainer )
    {
    FUNC_LOG;

    CSAnimStartupCtrl* self = new( ELeave ) CSAnimStartupCtrl;
    CleanupStack::PushL( self );
    self->ConstructL( aRect, aContainer );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrl::~CSAnimStartupCtrl
//
// ---------------------------------------------------------------------------
//
CSAnimStartupCtrl::~CSAnimStartupCtrl()
    {
    FUNC_LOG;

    delete iCommandResponder;
    delete iCommandInitiator;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrl::HasContent
//
// ---------------------------------------------------------------------------
//
TBool CSAnimStartupCtrl::HasContent() const
    {
    FUNC_LOG;

    return iHasContent;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrl::Load
//
// ---------------------------------------------------------------------------
//
void CSAnimStartupCtrl::Load(
    const TAnimationParams& aParams,
    const TBool aPlayDefaultBeep,
    const TInt aSyncCommand,
    TRequestStatus& aStatus )
    {
    FUNC_LOG;

    iPlayDefaultBeep = aPlayDefaultBeep;
    iSyncCommand = aSyncCommand;

    TRAPD( errorCode, LoadL( aParams, aStatus ) );
    ERROR( errorCode, "Failed to load" );
    if ( errorCode != KErrNone )
        {
        TRequestStatus* status = &aStatus;
        User::RequestComplete( status, errorCode );
        }
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrl::Start
//
// ---------------------------------------------------------------------------
//
void CSAnimStartupCtrl::Start( TRequestStatus& aStatus )
    {
    FUNC_LOG;

    aStatus = KRequestPending;

    if ( !iClientStatus )
        {
        if ( !HasContent() )
            {
            TRequestStatus* status = &aStatus;
            User::RequestComplete( status, KErrNone );
            }
        else if ( iCommandInitiator )
            {
            INFO_1( "Secondary display data: %d", iSyncCommand );
		
            iClientStatus = &aStatus;
            iWaitingForSyncResponse = ETrue;
            TInt errorCode = iCommandInitiator->IssueCommand(
                KMediatorSecondaryDisplayDomain,
                SecondaryDisplay::KCatStartup,
                SecondaryDisplay::ECmdStartupSync,
                TVersion( 0, 0, 0 ),
                TPckgBuf<TInt>( iSyncCommand ) );
            if ( errorCode != KErrNone )
                {
                ERROR( errorCode, "Failed issue command" );

                iClientStatus = &aStatus;
                StartAnimation();
                }
            }
        else
            {
            iClientStatus = &aStatus;
            StartAnimation();
            }
        }
    else
        {
        TRequestStatus* status = &aStatus;
        User::RequestComplete( status, KErrNotReady );
        }
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrl::HandlePointerEventL
//
// ---------------------------------------------------------------------------
void CSAnimStartupCtrl::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {
    FUNC_LOG;

    if ( aPointerEvent.iType == TPointerEvent::EButton1Down ||
         aPointerEvent.iType == TPointerEvent::EButton2Down ||
         aPointerEvent.iType == TPointerEvent::EButton3Down )
        {
        INFO( "Pointer event -> cancel animation" );

        Cancel();
        }

    CCoeControl::HandlePointerEventL( aPointerEvent );
    }


// ----------------------------------------------------------------------------
// CSAnimStartupCtrl::MediatorCommandL
//
// ----------------------------------------------------------------------------
//
void CSAnimStartupCtrl::MediatorCommandL(
    TUid aDomain,
    TUid aCategory,
    TInt aCommandId,
    TVersion /*aVersion*/,
    const TDesC8& /*aData*/ )
    {
    FUNC_LOG;

    if ( aCommandId == SecondaryDisplay::ECmdStartupPhaseSkip )
        {
        INFO( "Command ECmdStartupPhaseSkip -> cancel animation" );

        Cancel();
        }

    if ( iCommandResponder )
        {
        iCommandResponder->IssueResponse(
            aDomain, aCategory, aCommandId, KErrNone, TPckgBuf<TInt>( 0 ) );
        }
    }


// ----------------------------------------------------------------------------
// CSAnimStartupCtrl::CancelMediatorCommand
//
// ----------------------------------------------------------------------------
//
void CSAnimStartupCtrl::CancelMediatorCommand(
    TUid /*aDomain*/,
    TUid /*aCategory*/,
    TInt /*aCommandId*/ )
    {
    FUNC_LOG;
    }


// ----------------------------------------------------------------------------
// CSAnimStartupCtrl::CommandResponseL
//
// ----------------------------------------------------------------------------
//
void CSAnimStartupCtrl::CommandResponseL(
    TUid aDomain,
    TUid aCategory,
    TInt aCommandId,
    TInt /*aStatus*/,
    const TDesC8& /*aData*/ )
    {
    FUNC_LOG;
    INFO_3( "Domain: 0x%08x, category: 0x%08x, %d", aDomain.iUid, aCategory.iUid, aCommandId );

    if ( aDomain == KMediatorSecondaryDisplayDomain &&
         aCategory == SecondaryDisplay::KCatStartup &&
         aCommandId == SecondaryDisplay::ECmdStartupSync &&
         iWaitingForSyncResponse )
        {
        iWaitingForSyncResponse = EFalse;
        StartAnimation();
       }
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrl::CSAnimStartupCtrl
//
// ---------------------------------------------------------------------------
//
EXPORT_C CSAnimStartupCtrl::CSAnimStartupCtrl()
  : iPlayDefaultBeep( EFalse ),
    iSyncCommand( 0 ),
    iHasContent( EFalse ),
    iWaitingForSyncResponse( EFalse )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrl::ConstructL
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CSAnimStartupCtrl::ConstructL(
    const TRect& aRect,
    const CCoeControl& aContainer )
    {
    FUNC_LOG;

    FeatureManager::InitializeLibL();
    TBool secondaryDisplaySupported =
        FeatureManager::FeatureSupported( KFeatureIdCoverDisplay );
    FeatureManager::UnInitializeLib();

   

    CSAnimCtrl::BaseConstructL( aRect, aContainer );
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrl::LoadL
//
// ---------------------------------------------------------------------------
//
void CSAnimStartupCtrl::LoadL(
    const TAnimationParams& aParams,
    TRequestStatus& aStatus )
    {
    FUNC_LOG;

    CRepository* repository = CRepository::NewLC( aParams.iRepositoryUid );

    TPath img;
    ReadFileNameL(
        iCoeEnv->FsSession(), *repository, aParams.iAnimationPathKey, img );
    TTimeIntervalMicroSeconds32 frameDelay( 0 );
    TInt scaling( 0 );
    if ( img.Length() > 0 )
        {
        frameDelay = ReadIntValue( *repository, aParams.iFrameDelayKey );
        scaling = ReadIntValue( *repository, aParams.iEnableScalingKey );
        }

    TPath tone;
    TInt volume = ReadVolume( *repository, aParams.iVolumeKey );
    if ( volume > 0 )
        {
        ReadFileNameL(
            iCoeEnv->FsSession(), *repository, aParams.iTonePathKey, tone );

        iPlayDefaultBeep = iPlayDefaultBeep && tone.Length() <= 0;
        }

    iHasContent = ( img.Length() > 0 || tone.Length() > 0 );

    CleanupStack::PopAndDestroy( repository );

    CSAnimCtrl::Load(
        img, frameDelay, scaling, KDefaultRepeatCount,
        tone, volume, KDefaultVolumeRamp, KDefaultRepeatCount,
        aStatus );
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrl::ReadFileNameL
//
// ---------------------------------------------------------------------------
//
void CSAnimStartupCtrl::ReadFileNameL(
    RFs& aFs,
    CRepository& aRepository,
    const TUint32 aKey,
    TDes& aBuf )
    {
    FUNC_LOG;

    TInt errorCode = aRepository.Get( aKey, aBuf );
    ERROR_1( errorCode, "Failed to read value of key %d", aKey );
    if ( errorCode != KErrNone || !FileExists( aFs, aBuf ) )
        {
        aBuf = KNullDesC;
        }
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrl::ReadVolume
//
// ---------------------------------------------------------------------------
//
TInt CSAnimStartupCtrl::ReadVolume(
    CRepository& aRepository,
    const TUint32 aKey )
    {
    FUNC_LOG;

    TInt volume( 0 );
    TInt errorCode = aRepository.Get( aKey, volume );
    ERROR( errorCode, "Failed to get volume level from central repository" );

    if ( volume > 0 )
        {
        TBool isSilent = ETrue;
        TRAPD_ERR( errorCode, isSilent = IsSilentL() );
        ERROR( errorCode, "Failed to silent info" );
        if ( isSilent )
            {
            volume = 0;
            }
        }

    TInt retVal = Max( KMinVolume, Min( volume, KMaxVolume ) );

    INFO_1( "Volume: %d", retVal );

    return retVal;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrl::ReadIntValue
//
// ---------------------------------------------------------------------------
//
TInt CSAnimStartupCtrl::ReadIntValue(
    CRepository& aRepository,
    const TUint32 aKey )
    {
    FUNC_LOG;

    TInt value( 0 );
    TInt errorCode = aRepository.Get( aKey, value );
    ERROR_1( errorCode, "Failed to read value of key 0x%08x from central repository", aKey );
    return value;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrl::StartAnimation
//
// ---------------------------------------------------------------------------
//
void CSAnimStartupCtrl::StartAnimation()
    {
    FUNC_LOG;

    if ( iClientStatus )
        {
        iBackgroundColour = iEngine->BackroundColour();

        if ( iPlayDefaultBeep )
            {
            INFO( "Default startup beep requested" );
            /*
            CAknAppUi* appUi = static_cast<CAknAppUi*>( iEikonEnv->EikAppUi() );
            if ( appUi )
                {
                INFO( "Playing default startup beep" );

                appUi->KeySounds()->PlaySound( EAvkonSIDPowerOnTone );
                }
             */
            }

        iEngine->Start( *iClientStatus );
        iClientStatus = NULL;
        }
    }

