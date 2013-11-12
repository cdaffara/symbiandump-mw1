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

#ifndef __OPMAX_PLAYER_WINDOW_H__
#define __OPMAX_PLAYER_WINDOW_H__

#include <e32base.h>
#ifdef __USE_VIDEOPLAYER_1__
#include <videoplayer.h>
#else
#include <videoplayer2.h>
#endif
#include "playerwindow.h"
#include "openmaxplayerutility.h"

class COpMaxPlayerWindow : public CPlayerWindow
                         , private MVideoPlayerUtilityObserver
    {
public:

    static COpMaxPlayerWindow* NewL( RFs& aFs,
                                   RWsSession& aWs,
                                   CWsScreenDevice& aScreenDevice,
                                   const RWindowTreeNode &aParentWindow,
                                   TPoint aTopRight,
                                   TSize aSize,
                                   bool aTransparent,
                                   TRgb aBackgroundColor,
                                   bool aSuppressGraphicsContext);

    virtual ~COpMaxPlayerWindow();

    // inherited from CPlayerWindow
    virtual TTimeIntervalMicroSeconds Position();
    virtual TInt SetVolume( TInt aVolume );
    virtual TInt SetBalance( TInt aBalance );
    virtual TInt RotationAngle();
    virtual TInt NextRotationAngle();
    virtual TInt ResetRotation();
    virtual TInt NextAutoscaleMode();
    virtual TInt ResetAutoscale();
    virtual void AppendAutoscaleModeText( TDes& aString );
    virtual TInt SetClipRectangle( TRect aRect );
    virtual TInt SetVideoExtent( TRect aExtent );
    virtual TInt SetScaleFactor( TReal aWidthScaleFactor, TReal aHeightScaleFactor );
    virtual TInt Play();
    virtual TInt Pause();
    virtual TInt Stop();
    virtual TInt SetPosition( TTimeIntervalMicroSeconds aPositionInMs );
    virtual TInt StepForward();
    virtual TInt StepBackward();
    virtual TInt SetPlayVelocity( TInt aVelocity );
    virtual TInt DoExecuteOperation( TInt aOperation );
    virtual void InitializeParameters( TSize& aVideoSize,
                                       TTimeIntervalMicroSeconds& aDuration,
                                       TInt& aMaxVolume,
                                       TInt& aCurrentVolume,
                                       TInt& aBalanceCenter,
                                       TInt& aBalanceMaxLeft,
                                       TInt& aBalanceMaxRight,
                                       TInt& aDefaultBalance,
                                       TInt& aCurrentBalance );
    virtual void SetDefaultAutoscaleMode( TAutoscaleMode aDefaultAutoscale );
    virtual void PreparePlayerWithFile( RFile& aFile );
    virtual void PreparePlayerWithUrl( const TDesC& aUrl );


private:

    COpMaxPlayerWindow( RFs& aFs, RWsSession& aWs, CWsScreenDevice& aScreenDevice, bool aSuppressGraphicsContext );

    void ConstructL( const RWindowTreeNode &aParentWindow, TPoint aTopRight, TSize aSize, bool aTransparent, TRgb aBackgroundColor );

    // inherited from MVideoPlayerUtilityObserver
    virtual void MvpuoOpenComplete(TInt aError);
    virtual void MvpuoPrepareComplete(TInt aError);
    virtual void MvpuoFrameReady(CFbsBitmap& aFrame,TInt aError);
    virtual void MvpuoPlayComplete(TInt aError);
    virtual void MvpuoEvent(const TMMFEvent& aEvent);

    COpenMaxPlayerUtility*   iVideoPlayer;
    TVideoRotation          iRotation;
    TAutoScaleType          iDefaultAutoScale;
    TAutoScaleType          iAutoScale;

    };

#endif
