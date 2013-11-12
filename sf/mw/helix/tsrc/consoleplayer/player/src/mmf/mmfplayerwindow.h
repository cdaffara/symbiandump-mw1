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
 * Header file specifing the interface for the MMF Player Window.
 * 
 */

#ifndef __MMF_PLAYER_WINDOW_H__
#define __MMF_PLAYER_WINDOW_H__

#include <e32base.h>

#ifdef __USE_VIDEOPLAYER_1__
#include <videoplayer.h>
#else
#include <videoplayer2.h>
#endif

#include "playerwindow.h" 

const TUint32 KUidVideoController = { 0x101f8514 };

class CMmfPlayerWindow : public CPlayerWindow,
                         private MVideoPlayerUtilityObserver
    {
public:

    static CMmfPlayerWindow* NewL( RFs& aFs, 
                                   RWsSession& aWs,
                                   CWsScreenDevice& aScreenDevice,
                                   const RWindowTreeNode &aParentWindow,
                                   TPoint aTopRight,
                                   TSize aSize,
                                   bool aTransparent,
                                   TRgb aBackgroundColor,
                                   bool aSuppressGraphicsContext,
                                   TUint32 aControllerUid = KUidVideoController );
    
    virtual ~CMmfPlayerWindow();

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
    
    CMmfPlayerWindow( RFs& aFs, RWsSession& aWs, CWsScreenDevice& aScreenDevice, bool aSuppressGraphicsContext, TUint32 aControllerUid );
    
    void ConstructL( const RWindowTreeNode &aParentWindow, TPoint aTopRight, TSize aSize, bool aTransparent, TRgb aBackgroundColor );
    
    // inherited from MVideoPlayerUtilityObserver
    virtual void MvpuoOpenComplete(TInt aError);
    virtual void MvpuoPrepareComplete(TInt aError);
    virtual void MvpuoFrameReady(CFbsBitmap& aFrame,TInt aError);
    virtual void MvpuoPlayComplete(TInt aError);
    virtual void MvpuoEvent(const TMMFEvent& aEvent);

#ifdef __USE_VIDEOPLAYER_1__
    CVideoPlayerUtility*   iVideoPlayer;
#else    
    CVideoPlayerUtility2*  iVideoPlayer;
#endif
    
    TVideoRotation iRotation;
    TAutoScaleType iDefaultAutoScale;    
    TAutoScaleType iAutoScale;
    TUint32        iControllerUid;
  
    };

#endif
