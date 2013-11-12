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
 * Description:
 * Source file for the MMF Player Window.
 * 
 */

#include "mmfplayerwindow.h"

bool IgnoreError( TInt aError )
    {
    bool ignore = false;
    // Some errors should be ignored.
    
    if( aError == -12017 )  
        {
        // partial playback - do not treat as an error
        ignore = true;
        RDebug::Printf( "CONSOLEPLAYER partial playback" );            
        }
    
    return ignore;
    }

CMmfPlayerWindow* CMmfPlayerWindow::NewL( RFs& aFs, 
                                          RWsSession& aWs,
                                          CWsScreenDevice& aScreenDevice,
                                          const RWindowTreeNode &aParentWindow,
                                          TPoint aTopRight,
                                          TSize aSize,
                                          bool aTransparent,
                                          TRgb aBackgroundColor,
                                          bool aSuppressGraphicsContext,
                                          TUint32 aControllerUid )
    {
    CMmfPlayerWindow* self = new(ELeave) CMmfPlayerWindow( aFs, aWs, aScreenDevice, aSuppressGraphicsContext, aControllerUid );
    CleanupStack::PushL( self );
    self->ConstructL( aParentWindow, aTopRight, aSize, aTransparent, aBackgroundColor );
    CleanupStack::Pop();
    return self;   
    }
    
CMmfPlayerWindow::CMmfPlayerWindow( RFs& aFs, RWsSession& aWs, CWsScreenDevice& aScreenDevice, bool aSuppressGraphicsContext, TUint32 aControllerUid ) :
    CPlayerWindow( aFs, aWs, aScreenDevice, aSuppressGraphicsContext ),
    iControllerUid( aControllerUid )
    {    
    }

CMmfPlayerWindow::~CMmfPlayerWindow()
    {    
    delete iVideoPlayer;
    }

void CMmfPlayerWindow::ConstructL( const RWindowTreeNode &aParentWindow, TPoint aTopRight, TSize aSize, bool aTransparent, TRgb aBackgroundColor )
    {
    BaseConstructL( aParentWindow, aTopRight, aSize, aTransparent, aBackgroundColor );

#ifdef __USE_VIDEOPLAYER_1__
    iVideoPlayer = CVideoPlayerUtility::NewL(*this,
                                             EMdaPriorityNormal,
                                             EMdaPriorityPreferenceTimeAndQuality,
                                             iWs,
                                             iScreenDevice,
                                             iWindow,
                                             TRect(iScreenDevice.SizeInPixels()),
                                             TRect(iScreenDevice.SizeInPixels()));
#else
    iVideoPlayer = CVideoPlayerUtility2::NewL(*this,EMdaPriorityNormal,EMdaPriorityPreferenceTimeAndQuality);
#endif
    }


void CMmfPlayerWindow::InitializeParameters( TSize& aVideoSize, 
                                             TTimeIntervalMicroSeconds& aDuration, 
                                             TInt& aMaxVolume, 
                                             TInt& aCurrentVolume,
                                             TInt& aBalanceCenter,
                                             TInt& aBalanceMaxLeft,
                                             TInt& aBalanceMaxRight,
                                             TInt& aDefaultBalance, 
                                             TInt& aCurrentBalance )
    {
    TRAPD( err, iVideoPlayer->VideoFrameSizeL(aVideoSize) );
    CheckError( err, "VideoFrameSizeL" );
    TRAP( err, aDuration = iVideoPlayer->DurationL() );
    CheckError( err, "DurationL" );
    aMaxVolume = iVideoPlayer->MaxVolume();
    aCurrentVolume = iVideoPlayer->Volume();
    aBalanceCenter = KMMFBalanceCenter;
    aBalanceMaxLeft = KMMFBalanceMaxLeft;
    aBalanceMaxRight = KMMFBalanceMaxRight;
    aDefaultBalance = iVideoPlayer->Balance();
    aCurrentBalance = aDefaultBalance;
    }

void CMmfPlayerWindow::SetDefaultAutoscaleMode( TAutoscaleMode aDefaultAutoscale )
    {
    switch( aDefaultAutoscale )
        {
        case EDoNotAutoscale:
            iAutoScale = EAutoScaleNone;
            break;
        case EBestFit:
            iAutoScale = EAutoScaleBestFit;
            break;
        case EStretch:
            iAutoScale = EAutoScaleStretch;
            break;
        }
    iDefaultAutoScale = iAutoScale;
#ifdef __USE_VIDEOPLAYER_1__
    TRAPD(err, iVideoPlayer->SetAutoScaleL(iAutoScale));
#else
    TRAPD(err, iVideoPlayer->SetAutoScaleL(iWindow, iAutoScale));
#endif
    if( err == KErrNotReady )
        {
        // Too soon to call this.  Will get called later, at the appropriate time.
        }
    else
        {
        CheckError( err, "SetDefaultAutoscaleMode" );
        }
    }    

void CMmfPlayerWindow::PreparePlayerWithFile( RFile& aFile )
    {
    const TUid KUidController = { iControllerUid };
    TRAPD( err, iVideoPlayer->OpenFileL(aFile, KUidController) );
    CheckError( err, "OpenFileL" );
    
    // Now wait for MvpuoOpenComplete callback.            
    }

void CMmfPlayerWindow::PreparePlayerWithUrl( const TDesC& aUrl )
    {
    TRAPD( err, iVideoPlayer->OpenUrlL(aUrl) );
    CheckError( err, "OpenUrlL" );

    // Now wait for MvpuoOpenComplete callback.                
    }

TInt CMmfPlayerWindow::DoExecuteOperation( TInt /*aOperation*/ )
    {
    // Currently there are no MMF-specific commands.
    return KErrNotSupported;
    }

TTimeIntervalMicroSeconds CMmfPlayerWindow::Position()
    {
    TTimeIntervalMicroSeconds position = 0;
    TRAPD( err, position = iVideoPlayer->PositionL() );
    CheckError( err, "PositionL" );
    return position;
    }

TInt CMmfPlayerWindow::SetVolume( TInt aVolume )
    {
    TRAPD( err, iVideoPlayer->SetVolumeL( aVolume ) );
    if( err != KErrNone )
        {
        RDebug::Printf( "CONSOLEPLAYER SetVolumeL %i err=%i", aVolume, err );
        }
    return err;
    }

TInt CMmfPlayerWindow::SetBalance( TInt aBalance )
    {
    TRAPD( err, iVideoPlayer->SetBalanceL( aBalance ) );
    if( err != KErrNone )
        {
        RDebug::Printf( "CONSOLEPLAYER SetBalanceL %i err=%i", aBalance, err );
        }
    return err;    
    }

TInt CMmfPlayerWindow::RotationAngle()
    {
    switch( iRotation )
        {
        case EVideoRotationClockwise90:
            return 90;
        case EVideoRotationClockwise180:
            return 180;
        case EVideoRotationClockwise270:
            return 270;
        default:
            return 0;
        }        
    }

TInt CMmfPlayerWindow::NextRotationAngle()
    {
    switch( iRotation )
        {
        case EVideoRotationNone:
            iRotation = EVideoRotationClockwise90;
            break;
        case EVideoRotationClockwise90:
            iRotation = EVideoRotationClockwise180;
            break;
        case EVideoRotationClockwise180:
            iRotation = EVideoRotationClockwise270;
            break;
        case EVideoRotationClockwise270:
            iRotation = EVideoRotationNone;
            break;
        }
#ifdef __USE_VIDEOPLAYER_1__
    TRAPD( err, iVideoPlayer->SetRotationL(iRotation) );
#else
    TRAPD( err, iVideoPlayer->SetRotationL(iWindow, iRotation) );
#endif
    if( err != KErrNone )
        {
        RDebug::Printf( "CONSOLEPLAYER SetRotationL(%i) err=%i", iRotation, err );
        }    
    return err;
    }

TInt CMmfPlayerWindow::ResetRotation()
    {
    iRotation = EVideoRotationNone;
#ifdef __USE_VIDEOPLAYER_1__
    TRAPD( err, iVideoPlayer->SetRotationL(iRotation) );
#else
    TRAPD( err, iVideoPlayer->SetRotationL(iWindow, iRotation) );
#endif
    if( err != KErrNone )
        {
        RDebug::Printf( "CONSOLEPLAYER SetRotationL(reset) err=%i", err );
        }        
    return err;
    }

TInt CMmfPlayerWindow::NextAutoscaleMode()
    {
    switch( iAutoScale )
        {
        case EAutoScaleNone:
            iAutoScale = EAutoScaleBestFit;
            break;
        case EAutoScaleBestFit:
            iAutoScale = EAutoScaleClip;
            break;
        case EAutoScaleClip:
            iAutoScale = EAutoScaleStretch;
            break;
        case EAutoScaleStretch:
            iAutoScale = EAutoScaleNone;
            break;
        }     
#ifdef __USE_VIDEOPLAYER_1__
    TRAPD( err, iVideoPlayer->SetAutoScaleL(iAutoScale) );
#else
    TRAPD( err, iVideoPlayer->SetAutoScaleL(iWindow, iAutoScale) );
#endif
    if( err != KErrNone )
        {
        RDebug::Printf( "CONSOLEPLAYER SetAutoScaleL(%i) err=%i", iAutoScale, err );
        }    
    return err;
    }

TInt CMmfPlayerWindow::ResetAutoscale()
    {
    iAutoScale = iDefaultAutoScale;
#ifdef __USE_VIDEOPLAYER_1__
    TRAPD( err, iVideoPlayer->SetAutoScaleL(iAutoScale) );
#else
    TRAPD( err, iVideoPlayer->SetAutoScaleL(iWindow, iAutoScale) );
#endif
    if( err != KErrNone )
        {
        RDebug::Printf( "CONSOLEPLAYER SetAutoScaleL(%i) err=%i", iAutoScale, err );
        }    
    return err;
    }

void CMmfPlayerWindow::AppendAutoscaleModeText( TDes& aString )
    {
    switch( iAutoScale )
        {
        case EAutoScaleNone:
            aString.Append(_L("None"));
            break;
        case EAutoScaleBestFit:
            aString.Append(_L("Best Fit"));
            break;
        case EAutoScaleClip:
            aString.Append(_L("Clip"));
            break;
        case EAutoScaleStretch:
            aString.Append(_L("Stretch"));
            break;
        } 
    }

TInt CMmfPlayerWindow::SetClipRectangle( TRect aRect )
    {
#ifdef __USE_VIDEOPLAYER_1__   
    TRAPD( err, iVideoPlayer->SetCropRegionL(aRect) );
    if( err != KErrNone )
        {
        RDebug::Printf( "CONSOLEPLAYER SetCropRegionL err=%i", err );
        }    
#else
    TRAPD( err, iVideoPlayer->SetWindowClipRectL(iWindow, aRect) );
    if( err != KErrNone )
        {
        RDebug::Printf( "CONSOLEPLAYER SetCropRegionL err=%i", err );
        }    
#endif
    return err;
    }

TInt CMmfPlayerWindow::SetVideoExtent( TRect aExtent )
    {
#ifdef __USE_VIDEOPLAYER_1__
    // TODO: HOW DO YOU DO THIS WITH OLD VIDEO PLAYER?
    TInt err = KErrNotSupported;
    RDebug::Printf( "CONSOLEPLAYER SetVideoExtent not supported with video player 1" );
#else
    TRAPD( err, iVideoPlayer->SetVideoExtentL(iWindow, aExtent) );
    if( err != KErrNone )
        {
        RDebug::Printf( "CONSOLEPLAYER SetVideoExtentL err=%i", err );
        }    
#endif
    return err;
    }

TInt CMmfPlayerWindow::SetScaleFactor( TReal aWidthScaleFactor, TReal aHeightScaleFactor )
    {
#ifdef __USE_VIDEOPLAYER_1__
    // TODO: WHAT VALUE SHOULD BE USED FOR aAntiAliasFiltering?    
    TRAPD( err, iVideoPlayer->SetScaleFactorL(aWidthScaleFactor, aHeightScaleFactor, ETrue) );
#else
    TRAPD( err, iVideoPlayer->SetScaleFactorL(iWindow, aWidthScaleFactor, aHeightScaleFactor) );
#endif
    if( err != KErrNone )
        {
        RDebug::Printf( "CONSOLEPLAYER SetScaleFactorL err=%i", err );
        }    
    return err;
    }

TInt CMmfPlayerWindow::Play()
    {
    iVideoPlayer->Play();
    return KErrNone;
    }

TInt CMmfPlayerWindow::Pause()
    {
    TRAPD(err, iVideoPlayer->PauseL());
    if( err != KErrNone )
        {
        RDebug::Printf( "CONSOLEPLAYER PauseL err=%i", err );
        }        
    return err;
    }

TInt CMmfPlayerWindow::Stop()
    {
    TInt err = iVideoPlayer->Stop();
    if( err != KErrNone )
        {
        RDebug::Printf( "CONSOLEPLAYER Stop err=%i", err );
        }        
    return err;
    }

TInt CMmfPlayerWindow::SetPosition( TTimeIntervalMicroSeconds aPositionInMs )
    {
    TRAPD( err, iVideoPlayer->SetPositionL( aPositionInMs ) );
    if( err != KErrNone )
        {
        RDebug::Printf( "CONSOLEPLAYER SetPositionL err=%i", err );
        }        
    return err;
    }

TInt CMmfPlayerWindow::StepForward()
    {
    TRAPD( err, iVideoPlayer->StepFrameL(1) );
    if( err != KErrNone )
        {
        RDebug::Printf( "CONSOLEPLAYER StepFrameL(1) err=%i", err );
        }            
    return err;    
    }

TInt CMmfPlayerWindow::StepBackward()
    {
    TRAPD( err, iVideoPlayer->StepFrameL(-1) );
    if( err != KErrNone )
        {
        RDebug::Printf( "CONSOLEPLAYER StepFrameL(-1) err=%i", err );
        }            
    return err;   
    }

TInt CMmfPlayerWindow::SetPlayVelocity( TInt aVelocity )
    {
    TRAPD( err,iVideoPlayer->SetPlayVelocityL(aVelocity) );
    if( err != KErrNone )
        {
        RDebug::Printf( "CONSOLEPLAYER SetPlayVelocityL %i err=%i", aVelocity, err );
        }            
    return err;
    }
    
// inherited from MVideoPlayerUtilityObserver

void CMmfPlayerWindow::MvpuoOpenComplete(TInt aError)
    {
    RDebug::Printf( "CONSOLEPLAYER MvpuoOpenComplete(%i)", aError );
    
    if( aError == KErrNone )
        {
        iVideoPlayer->Prepare();
    
        // Now wait for MvpuoPrepareComplete callback.
        }
    else if( !IgnoreError( aError ) )
        {
        CheckError( aError, "MvpuOpenComplete" );
        }
    }

void CMmfPlayerWindow::MvpuoPrepareComplete(TInt aError)
    {
    RDebug::Printf( "CONSOLEPLAYER MvpuoPrepareComplete(%i)", aError );
    
    if( aError == KErrNone )
        {
#ifdef __USE_VIDEOPLAYER_1__   
        TRAPD( err, iVideoPlayer->SetDisplayWindowL( iWs, 
                                                     iScreenDevice, 
                                                     iWindow, 
                                                     TRect(iWindow.Position(), iWindow.Size()),
                                                     TRect(iWindow.Position(), iWindow.Size())) );
#else
        TRAPD( err, iVideoPlayer->AddDisplayWindowL( iWs, iScreenDevice, iWindow ) );
#endif
        if( err == KErrNone )
            {
            Play();
            ResetAutoscale();
            }
        else
            {
            RDebug::Printf("CONSOLEPLAYER AddDisplayWindowL failed, err=%i", err );
            }
        PlayInitiated();
        }
    else if( !IgnoreError( aError ) )
        {
        Error( aError );
        }
    }

void CMmfPlayerWindow::MvpuoFrameReady(CFbsBitmap& /*aFrame*/,TInt aError)
    {
    RDebug::Printf( "CONSOLEPLAYER MvpuoFrameReady(%i)", aError );
    
    if( (aError != KErrNone) && !IgnoreError( aError ) )
        {
        Error( aError );
        }
    }

void CMmfPlayerWindow::MvpuoPlayComplete(TInt aError)
    {
    RDebug::Printf( "CONSOLEPLAYER MvpuoPlayComplete(%i)", aError );
    
    if( aError == KErrNone )
        {
        PlayComplete();
        }
    else if( !IgnoreError( aError ) )
        {
        Error( aError );
        }
    }

void CMmfPlayerWindow::MvpuoEvent(const TMMFEvent& aEvent)
    {   
    RDebug::Printf( "CONSOLEPLAYER MvpuoEvent event=%x error=%i", aEvent.iEventType.iUid, aEvent.iErrorCode );    

    if( aEvent.iErrorCode != KErrNone && !IgnoreError( aEvent.iErrorCode ) )
        {
        Error( aEvent.iErrorCode );
        }
    }
