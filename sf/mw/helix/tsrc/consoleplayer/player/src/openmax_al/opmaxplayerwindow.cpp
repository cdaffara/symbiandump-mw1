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
 * Source file for the main class of the ConsolePlayer test app.
 *
 */

#include "opmaxplayerwindow.h"
#include "openmaxplayerutility.h"


/**
Balance values from mmfstandardcustomcommands.h
*/
#if 0
const TInt KMMFBalanceCenter = 0;
const TInt KMMFBalanceMaxLeft = -100;
const TInt KMMFBalanceMaxRight = 100;
#endif

COpMaxPlayerWindow* COpMaxPlayerWindow::NewL( RFs& aFs,
                                          RWsSession& aWs,
                                          CWsScreenDevice& aScreenDevice,
                                          const RWindowTreeNode &aParentWindow,
                                          TPoint aTopRight,
                                          TSize aSize,
                                          bool aTransparent,
                                          TRgb aBackgroundColor,
                                          bool aSuppressGraphicsContext)
    {
    COpMaxPlayerWindow* self = new(ELeave) COpMaxPlayerWindow( aFs, aWs, aScreenDevice, aSuppressGraphicsContext);
    CleanupStack::PushL( self );
    self->ConstructL( aParentWindow, aTopRight, aSize, aTransparent, aBackgroundColor );
    CleanupStack::Pop();
    return self;
    }

COpMaxPlayerWindow::COpMaxPlayerWindow( RFs& aFs, RWsSession& aWs, CWsScreenDevice& aScreenDevice, bool aSuppressGraphicsContext ) :
    CPlayerWindow( aFs, aWs, aScreenDevice, aSuppressGraphicsContext )
    {
    }

COpMaxPlayerWindow::~COpMaxPlayerWindow()
    {
    delete iVideoPlayer;
    }

void COpMaxPlayerWindow::ConstructL( const RWindowTreeNode &aParentWindow, TPoint aTopRight, TSize aSize, bool aTransparent, TRgb aBackgroundColor )
    {
    BaseConstructL( aParentWindow, aTopRight, aSize, aTransparent, aBackgroundColor );

    iVideoPlayer = new (ELeave) COpenMaxPlayerUtility;
    iVideoPlayer->AddDisplayWindow( iWs, iScreenDevice, iWindow );
    }


void COpMaxPlayerWindow::InitializeParameters( TSize& aVideoSize,
                                             TTimeIntervalMicroSeconds& aDuration,
                                             TInt& aMaxVolume,
                                             TInt& aCurrentVolume,
                                             TInt& aBalanceCenter,
                                             TInt& aBalanceMaxLeft,
                                             TInt& aBalanceMaxRight,
                                             TInt& aDefaultBalance,
                                             TInt& aCurrentBalance )
    {
    iVideoPlayer->VideoFrameSize(aVideoSize);
    aDuration = iVideoPlayer->Duration();
    aMaxVolume = iVideoPlayer->MaxVolume();
    aCurrentVolume = iVideoPlayer->Volume();
    aBalanceCenter = KMMFBalanceCenter;
    aBalanceMaxLeft = KMMFBalanceMaxLeft;
    aBalanceMaxRight = KMMFBalanceMaxRight;
    aDefaultBalance = iVideoPlayer->Balance();
    aCurrentBalance = aDefaultBalance;
    }

void COpMaxPlayerWindow::SetDefaultAutoscaleMode( TAutoscaleMode aDefaultAutoscale )
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
    iVideoPlayer->SetAutoScale(iWindow, iAutoScale);
    }

void COpMaxPlayerWindow::PreparePlayerWithFile( RFile& aFile )
    {
    TFileName fileName;
    aFile.FullName(fileName);
    iVideoPlayer->OpenFile(fileName);

    // Now wait for MvpuoOpenComplete callback.
    PlayInitiated();
    }

void COpMaxPlayerWindow::PreparePlayerWithUrl( const TDesC& aUrl )
    {
    iVideoPlayer->OpenFile(aUrl);

    // Now wait for MvpuoOpenComplete callback.
    }

TInt COpMaxPlayerWindow::DoExecuteOperation( TInt /*aOperation*/ )
    {
    // Currently there are no MMF-specific commands.
    return KErrNotSupported;
    }

TTimeIntervalMicroSeconds COpMaxPlayerWindow::Position()
    {
    TTimeIntervalMicroSeconds position = iVideoPlayer->Position();
    return position;
    }

TInt COpMaxPlayerWindow::SetVolume( TInt aVolume )
    {
    iVideoPlayer->SetVolume( aVolume );
    return 0;
    }

TInt COpMaxPlayerWindow::SetBalance( TInt aBalance )
    {
    iVideoPlayer->SetBalance( aBalance );
    return 0;
    }

TInt COpMaxPlayerWindow::RotationAngle()
    {
    TInt rotAngle = 0;
    switch( iRotation )
        {
        case EVideoRotationClockwise90:
            rotAngle = 90;
        case EVideoRotationClockwise180:
            rotAngle = 180;
        case EVideoRotationClockwise270:
            rotAngle = 270;
        default:
            rotAngle = 0;
        }
    return rotAngle;
    }

TInt COpMaxPlayerWindow::NextRotationAngle()
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
    iVideoPlayer->SetRotation(iWindow, iRotation);
    return 0;
    }

TInt COpMaxPlayerWindow::ResetRotation()
    {
    iRotation = EVideoRotationNone;
    iVideoPlayer->SetRotation(iWindow, iRotation);
    return 0;
    }

TInt COpMaxPlayerWindow::NextAutoscaleMode()
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
    iVideoPlayer->SetAutoScale(iWindow, iAutoScale);
    return 0;
    }

TInt COpMaxPlayerWindow::ResetAutoscale()
    {
    iAutoScale = iDefaultAutoScale;
    iVideoPlayer->SetAutoScale(iWindow, iAutoScale);
    return 0;
    }

void COpMaxPlayerWindow::AppendAutoscaleModeText( TDes& aString )
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

TInt COpMaxPlayerWindow::SetClipRectangle( TRect aRect )
    {
    iVideoPlayer->SetWindowClipRect(iWindow, aRect);
    return 0;
    }

TInt COpMaxPlayerWindow::SetVideoExtent( TRect aExtent )
    {
    iVideoPlayer->SetVideoExtent(iWindow, aExtent);
    return 0;
    }

TInt COpMaxPlayerWindow::SetScaleFactor( TReal aWidthScaleFactor, TReal aHeightScaleFactor )
    {
    iVideoPlayer->SetScaleFactor(iWindow, aWidthScaleFactor, aHeightScaleFactor);
    return 0;
    }

TInt COpMaxPlayerWindow::Play()
    {
    iVideoPlayer->Play();
    return KErrNone;
    }

TInt COpMaxPlayerWindow::Pause()
    {
    iVideoPlayer->Pause();
    return 0;
    }

TInt COpMaxPlayerWindow::Stop()
    {
    return iVideoPlayer->Stop();
    }

TInt COpMaxPlayerWindow::SetPosition( TTimeIntervalMicroSeconds aPositionInMs )
    {
    iVideoPlayer->SetPosition( aPositionInMs );
    return 0;
    }

TInt COpMaxPlayerWindow::StepForward()
    {
    iVideoPlayer->StepFrame(1);
    return 0;
    }

TInt COpMaxPlayerWindow::StepBackward()
    {
    iVideoPlayer->StepFrame(-1);
    return 0;
    }

TInt COpMaxPlayerWindow::SetPlayVelocity( TInt aVelocity )
    {
    TRAPD( err, iVideoPlayer->SetPlayVelocity(aVelocity) );
    return err;
    }

// inherited from MVideoPlayerUtilityObserver
void COpMaxPlayerWindow::MvpuoOpenComplete(TInt aError)
    {
    RDebug::Printf( "CONSOLEPLAYER MvpuoOpenComplete(%i)", aError );

    if( aError == KErrNone )
        {
        iVideoPlayer->Prepare();

        // Now wait for MvpuoPrepareComplete callback.
        }
    else
        {
        Error( aError );
        }
    }

void COpMaxPlayerWindow::MvpuoPrepareComplete(TInt aError)
    {
    RDebug::Printf( "CONSOLEPLAYER MvpuoPrepareComplete(%i)", aError );

    if( aError == KErrNone )
        {
        iVideoPlayer->AddDisplayWindow( iWs, iScreenDevice, iWindow );
        Play();
        ResetAutoscale();
        PlayInitiated();
        }
    else
        {
        Error( aError );
        }
    }

void COpMaxPlayerWindow::MvpuoFrameReady(CFbsBitmap& /*aFrame*/,TInt aError)
    {
    RDebug::Printf( "CONSOLEPLAYER MvpuoFrameReady(%i)", aError );

    if( aError != KErrNone )
        {
        Error( aError );
        }
    }

void COpMaxPlayerWindow::MvpuoPlayComplete(TInt aError)
    {
    RDebug::Printf( "CONSOLEPLAYER MvpuoPlayComplete(%i)", aError );

    if( aError == KErrNone )
        {
        PlayComplete();
        }
    else
        {
        Error( aError );
        }
    }

void COpMaxPlayerWindow::MvpuoEvent(const TMMFEvent& aEvent)
    {
    RDebug::Printf( "CONSOLEPLAYER MvpuoEvent event=%x error=%i", aEvent.iEventType.iUid, aEvent.iErrorCode );

    if( aEvent.iErrorCode != KErrNone )
        {
        Error( aEvent.iErrorCode );
        }
    }
