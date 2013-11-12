/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*     This class is the container class of the CSysApShutdownAnimation.
*     Is used to show welcome animation.
*
*/



#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION

//#include <aknappui.h>
#include <sysapappui.h>
#include <SecondaryDisplay/SecondaryDisplayStartupAPI.h>
#include <startupdomaincrkeys.h>

#include "SysApShutdownAnimation.h"
#include "SysApShutdownImage.h"
#include "SysAp.hrh"
#include <eikdef.h>
#include <eikenv.h>
#include <coemain.h>
#include <coecntrl.h>
#include <coeaui.h>

#define iSysApUiHandle ((CSysApAppUi*)CEikonEnv::Static()->EikAppUi())

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::NewL
//
// ---------------------------------------------------------------------------
//
CSysApShutdownAnimation* CSysApShutdownAnimation::NewL( CSysApShutdownImage& aView )
    {
    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::NewL()") ) );

    CSysApShutdownAnimation* self =
        new( ELeave ) CSysApShutdownAnimation( aView );

    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::NewL(): End") ) );
    return self;
    }


// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::~CSysApShutdownAnimation
//
// ---------------------------------------------------------------------------
//
CSysApShutdownAnimation::~CSysApShutdownAnimation()
    {
    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::~CSysApShutdownAnimation()") ) );

    Cancel();

    if ( iCtrl )
        {
        iCtrl->Cancel();
        iView.RemoveComponent();
        delete iCtrl;
        }

    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::~CSysApShutdownAnimation(): end") ) );
    }


// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::HasContent
//
// ---------------------------------------------------------------------------
//
TBool CSysApShutdownAnimation::HasContent() const
    {
    if ( iCtrl ) return iCtrl->HasContent();
    return EFalse;
    }


// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::Play
//
// ---------------------------------------------------------------------------
//
void CSysApShutdownAnimation::Play( const TCallBack& aCallBack )
    {
    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::Load()") ) );

    if ( !IsActive() )
        {
        if ( iCtrl )
            {
            delete iCtrl;
            iCtrl = NULL;
            }

//        iAvkonAppUi->StatusPane()->MakeVisible( EFalse );
//        iView.SetRect( iAvkonAppUi->ApplicationRect() );
//      iAvkonAppUi->StatusPane()->MakeVisible( EFalse );
//        iSysApUiHandle->StatusPane()->MakeVisible( EFalse );
        iView.SetRectForAnimation(); 
        iState = ELoading;

        TRAPD( errorCode, iCtrl = CSAnimStartupCtrl::NewL( iView.Rect(), iView ) );
        if ( errorCode == KErrNone )
            {
            iCallBack = aCallBack;

            iCtrl->SetRect( iView.Rect() );

            CSAnimStartupCtrl::TAnimationParams params;
            params.iRepositoryUid = KCRUidStartupConf;
                params.iAnimationPathKey = KShutdownAnimationPath;
                params.iFrameDelayKey = KShutdownAnimationFrameDelay;
                params.iEnableScalingKey = KShutdownAnimationScaling;
                params.iTonePathKey = KShutdownTonePath;
                params.iVolumeKey = KShutdownToneVolume;

            iCtrl->Load(
                params,
                EFalse,
                SecondaryDisplay::EStartShutdownAnimation,
                iStatus );
            SetActive();
            }
        else
            {
            TRACES( RDebug::Print(_L( "CSysApShutdownAnimation: failed to create animation control: %d"), errorCode ) );

            aCallBack.CallBack();
            }
        }

    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::Load(): End") ) );
    }


// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::DoCancel
//
// ---------------------------------------------------------------------------
//
void CSysApShutdownAnimation::DoCancel()
    {
    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::DoCancel()") ) );

    if ( iCtrl ) iCtrl->Cancel();
    iCallBack.CallBack();

    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::DoCancel(): End") ) );
    }


// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::RunL
//
// ---------------------------------------------------------------------------
//
void CSysApShutdownAnimation::RunL()
    {
    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::RunL(): iStatus = %d"), iStatus.Int() ) );

    if ( iState == ELoading )
        {
        iState = EPlaying;
        iView.SetComponent( *iCtrl );
        iCtrl->Start( iStatus );
        SetActive();
        }
    else if ( iState == EPlaying )
        {
        iState = EFinished;
        iCallBack.CallBack();
        }

    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::RunL(): End") ) );
    }


// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::CSysApShutdownAnimation
//
// ---------------------------------------------------------------------------
//
CSysApShutdownAnimation::CSysApShutdownAnimation( CSysApShutdownImage& aView )
  : CActive( EPriorityStandard ),
    iView( aView ),
    iState( EInitial )
    {
    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::CSysApShutdownAnimation()") ) );

    CActiveScheduler::Add( this );

    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::CSysApShutdownAnimation(): End") ) );
    }


#else // RD_STARTUP_ANIMATION_CUSTOMIZATION

// INCLUDE FILES
#include <aknappui.h>
#include <aknnotewrappers.h>
#include <barsread.h> //use of TResourceReader
#include <aknbitmapanimation.h>
#include <ConeResLoader.h>
#include "SysApShutdownAnimation.h"
#include <SysApShutdownAnimation.rsg>
#include "SysAp.hrh"
#include "SysApAppUi.h"
#include "SysApShutdownTone.h"

// CONSTANTS
_LIT( KShutdownAnimationResource, "z:sysapshutdownanimation.rsc");

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
void CSysApShutdownAnimation::ConstructL()
    {
    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::ConstructL()") ) );

    CreateWindowL();

    iAnimTimer = CPeriodic::NewL( EPriorityNormal );

    iAvkonAppUi->StatusPane()->MakeVisible(EFalse);

    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation : animation loading started") ) );
    iAnim = CAknBitmapAnimation::NewL();
    iAnim->SetContainerWindowL( *this );
    iAnim->SetScaleModeForAnimationFrames(EAspectRatioPreservedAndUnusedSpaceRemoved);
    TResourceReader rr;
    RConeResourceLoader loader( *iCoeEnv );

    TParse* fp = new(ELeave) TParse();
    fp->Set(KShutdownAnimationResource, &KDC_APP_RESOURCE_DIR, NULL);
    #ifdef _DEBUG
        RDebug::Print( _L( "CSysApShutdownAnimation : Operator animation resource path: %S"), &fp->FullName() );
    #endif
    TFileName name( fp->FullName() );
    delete fp;

    TInt fileError = loader.Open( name );
    if ( fileError == KErrNone )
        {
        CleanupClosePushL( loader );
        iCoeEnv->CreateResourceReaderLC(rr, R_SHUTDOWN_ANIM);
        TRAPD(err, iAnim->ConstructFromResourceL( rr ));
        if( err == KErrNone )
            {
            TResourceReader timeReader;
            iCoeEnv->CreateResourceReaderLC(timeReader, R_ANIM_DURATION);
            iShowingTime = timeReader.ReadInt16();
            #ifdef _DEBUG
                RDebug::Print( _L( "CSysApShutdownAnimation : Operator animation showing time: %d"), iShowingTime );
            #endif
            CleanupStack::PopAndDestroy(); // pop timeReader

            TRACES( RDebug::Print( _L( "CSysApShutdownAnimation::ConstructL: Initialise shutdown tone") ) );

            if ( iShowingTime > 0 )
                {
                // Initialize shutdown tone. Animation can be shown without any tone, so trap all leaves.
                // To maintain binary compatibility , check resource availability first to avoid panic in case tone is missing.
                TBool isTonePathAvailable(EFalse);
                TRAP(err, isTonePathAvailable = iCoeEnv->IsResourceAvailableL(R_SHUTDOWN_TONE_PATH));
                if ( isTonePathAvailable )
                    {
                    HBufC* tonePath = NULL;
                    TRAP(err, tonePath = iCoeEnv->AllocReadResourceL(R_SHUTDOWN_TONE_PATH));
                    if ( err != KErrNone )
                        {
                        TRACES( RDebug::Print( _L( "CSysApShutdownAnimation::ConstructL: Unable to allocate resource for shutdown tone: %d"), err ) );
                        }
                    else
                        {
                        TRAP(err, iShutdownTone = CSysApShutdownTone::NewL(*tonePath, this));
                        if ( err != KErrNone )
                            {
                            TRACES( RDebug::Print( _L( "CSysApShutdownAnimation::ConstructL: Unable to initialize shutdown tone: %d"), err ) );
                            }
                        }
                    delete tonePath;
                    }
                else
                    {
                    TRACES( RDebug::Print( _L( "CSysApShutdownAnimation::ConstructL: Resource not available for shutdown tone: %d"), err ) );
                    }
                }
            }
        else
            {
            iShowingTime = 0;
            TRACES( RDebug::Print(_L( "CSysApShutdownAnimation : animation loading failed") ) );
            }

        CleanupStack::PopAndDestroy(); //pop rr
        TRACES( RDebug::Print(_L( "CSysApShutdownAnimation : animation loading ended") ) );

        CleanupStack::PopAndDestroy(); //pop loader
        }
    else
        {
        TRACES( RDebug::Print(_L( "CSysApShutdownAnimation : resource file loading failed") ) );
        }

    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation : ConstructL ended") ) );
    }

// -----------------------------------------------------------------------------
// CSysApShutdownAnimation::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSysApShutdownAnimation* CSysApShutdownAnimation::NewL( CSysApAppUi* aSysApAppUi)
    {
    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::NewL()") ) );
    CSysApShutdownAnimation* self = new (ELeave) CSysApShutdownAnimation( aSysApAppUi );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------
// CSysApShutdownAnimation::CSysApShutdownAnimation()
// ---------------------------------------------------------
CSysApShutdownAnimation::CSysApShutdownAnimation( CSysApAppUi* aSysApAppUi ) :
    iSysApAppUi( aSysApAppUi ),
    iAnimationShowing( EFalse ),
    iAnimationCancelled ( EFalse ),
    iShutdownTone( NULL ),
    iHandleNextKey( ETrue )
    {
    }

// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::StartL()
// ---------------------------------------------------------------------------
void CSysApShutdownAnimation::StartL( TBool iShortPowerKeyPressShutdown )
    {
    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::StartL() begin: iShortPowerKeyPressShutdown=%d"),
                          iShortPowerKeyPressShutdown ) );

    ControlEnv()->WsSession().Flush(); // force draw of the context

    SetRect(iAvkonAppUi->ApplicationRect());
    iAnim->SetPosition( TPoint( (iAvkonAppUi->ApplicationRect().Width()/2) - (iAnim->BitmapAnimData()->Size().iWidth/2),
                                (iAvkonAppUi->ApplicationRect().Height()/2) - (iAnim->BitmapAnimData()->Size().iHeight/2) ) );

    ActivateL();

    iReadyForToneSync = ETrue;

    // If the device is switched off with a long power key press, the next event key must be ignored as it is leaked from application class
    if ( !iShortPowerKeyPressShutdown )
        {
        iHandleNextKey = EFalse;
        }

    if ( iShutdownTone )
        {
        // Shutdown tone takes a while to initialize before it can be played, so synchronize here.
        if ( iToneReady )
            {
            // Tone already ready, so continue start
            ContinueStart();
            }
        else
            {
            TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::StartL() - Waiting for shutdown tone to finish initializing") ) );
            }
        }
    else
        {
        // No tone, so pretend that sync is done
        ContinueStart();
        }

    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::StartL() end") ) );
    }

// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::ContinueStart()
// ---------------------------------------------------------------------------
void CSysApShutdownAnimation::ContinueStart()
    {
    iAnimationShowing = ETrue;

    if ( iShutdownTone )
        {
        TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::ContinueStart() Playing the tone") ) );
        iShutdownTone->Play();
        }

    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::ContinueStart() Start animation") ) );
    TRAPD(err, iAnim->StartAnimationL());

    if ( err != KErrNone )
        {
        TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::ContinueStart() Starting animation failed") ) );
        }

    iSysApAppUi->StartAnimTiming();
    }

// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::ToneReady()
// ---------------------------------------------------------------------------
void CSysApShutdownAnimation::ToneReady()
    {
    // If animation has been canceled, do not continue
    if ( !iAnimationCancelled )
        {
        iToneReady = ETrue;

        if ( iReadyForToneSync )
            {
            TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::ToneReady() Tone is ready, continue animation start") ) );
            ContinueStart();
            }
        else
            {
            TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::ToneReady() Tone is ready before animation") ) );
            }
        }
    }

// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::DoDrawingL()
// ---------------------------------------------------------------------------
void CSysApShutdownAnimation::DoDrawingL() const
    {
    }

// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::~CSysApShutdownAnimation()
// ---------------------------------------------------------------------------
CSysApShutdownAnimation::~CSysApShutdownAnimation()
    {
    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::~CSysApShutdownAnimation() begin") ) );

    if( iAnim )
        {
        if( iAnimationShowing )
            {
            iAnim->CancelAnimation();
            TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::~CSysApShutdownAnimation() animation cancelled") ) );
            }
        }
    delete iAnim;
    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::~CSysApShutdownAnimation() iAnim deleted") ) );

    if (iBackgroundBitmap)
        {
        delete iBackgroundBitmap;
        TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::~CSysApShutdownAnimation() iBackgroundBitmap deleted") ) );
        }

    if( iAnimTimer )
        {
        iAnimTimer->Cancel();
        TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::~CSysApShutdownAnimation() timer cancelled") ) );
        }
    delete iAnimTimer;
    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::~CSysApShutdownAnimation() iAnimTimer deleted") ) );

    delete iShutdownTone;

    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::~CSysApShutdownAnimation() end") ) );
    }

// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::ComponentControl(TInt aIndex)
// ---------------------------------------------------------------------------
CCoeControl* CSysApShutdownAnimation::ComponentControl(TInt aIndex) const
    {
    switch ( aIndex )
        {
        case 0:
            {
            return iAnim;
            }
        default:
            {
            return NULL;
            }
        }
    }

// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::CountComponentControls()
// ---------------------------------------------------------------------------
TInt CSysApShutdownAnimation::CountComponentControls() const
    {
    return iAnim ? 1 : 0; // return nbr of controls inside this container
    }

// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::Draw(const TRect& aRect) const
// ---------------------------------------------------------------------------
void CSysApShutdownAnimation::Draw(const TRect& ) const
    {
    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::Draw()") ) );
    TRAPD(err, DoDrawingL());
    if (err != KErrNone)
        {
        TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::DoDrawingL() error: %d"), err ) );
        }
    }

// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::ShowingTime()
// ---------------------------------------------------------------------------
TInt CSysApShutdownAnimation::ShowingTime()
    {
    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::ShowingTime() returns: %d"), iShowingTime ) );
    return iShowingTime;
    }

// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::SizeChanged()
// ---------------------------------------------------------------------------
void CSysApShutdownAnimation::SizeChanged()
    {
    if( iAnim )
        {
        iAnim->SetRect(Rect());
        }
    }

// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::EndAnimation()
// ---------------------------------------------------------------------------
void CSysApShutdownAnimation::EndAnimation()
    {
    if( iAnim && iAnimationShowing && !iAnimationCancelled )
        {
        TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::EndAnimation() Cancel animation.")) );
        iAnim->CancelAnimation();
        iAnimationCancelled = ETrue;
        iAnimationShowing = EFalse;
        // Stop shutdown tone
        if ( iShutdownTone )
            {
            iShutdownTone->Stop();
            }

        // Continue shutdown
        iAnimTimer->Start( 1000, 1000, TCallBack( iSysApAppUi->DoStopAnimTiming, iSysApAppUi ) );
        }
    }

// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::HandleControlEventL(...)
// ---------------------------------------------------------------------------
void CSysApShutdownAnimation::HandleControlEventL(
        CCoeControl* ,
        TCoeEvent )
    {
    //pure virtual from MCoeControlObserver
    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::HandleControlEventL()") ) );
    }

// ---------------------------------------------------------------------------
// CSysApShutdownAnimation::OfferKeyEventL(...)
// ---------------------------------------------------------------------------
TKeyResponse CSysApShutdownAnimation::OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::OfferKeyEventL() begin: aType=%d, iHandleNextKey=%d"), aType, iHandleNextKey ) );

    // This block is for filtering the key events that are leaked from CSysApAppUi before the animation is active
    // e.g. when device is shutdown with a long press of power key
    if ( !iHandleNextKey )
        {
        if ( aType == EEventKeyDown || aType == EEventKeyUp )
            {
            iHandleNextKey = ETrue;
            }
        }

    if ( aType == EEventKey )
        {
        if ( !iSysApAppUi->ResourcesFreed() )
            {
            TBool isDeviceModeKey = iSysApAppUi->IsDeviceModeKey( aKeyEvent );

            // aType is used for filtering key events that are received when this control is visible
            if( iAnimationShowing && !iAnimationCancelled && !isDeviceModeKey && iHandleNextKey )
                {
                EndAnimation();
                iSysApAppUi->NotifyShutdownAnimationSkip();
                }

            if ( isDeviceModeKey ) // device mode keys are not consumed, so that application class is able to receive them
                {
                TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::OfferKeyEventL() end: return EKeyWasNotConsumed")) );
                return EKeyWasNotConsumed;
                }
            }
        else  // SysAp has relased almost all the allocations, just cancel
            {
            EndAnimation();
            }
        }
    TRACES( RDebug::Print(_L( "CSysApShutdownAnimation::OfferKeyEventL() end: return EKeyWasConsumed")) );

    return EKeyWasConsumed;
    }

#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

//  End of File
