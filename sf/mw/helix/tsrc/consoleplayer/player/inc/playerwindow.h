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
 * Header specifying the abstract Player Window.
 * 
 */

#ifndef __PLAYER_WINDOW_H__
#define __PLAYER_WINDOW_H__

#include <e32base.h>
#include <f32file.h>
#include <w32std.h>

#include "testappbase.h"

enum TPlayerWindowOperations
    {
    EOperation_PauseResume = KOperation_FirstCustomIndex+100,
    EOperation_Pause,
    EOperation_Resume,
    EOperation_StopPlay,
    EOperation_SeekBack,
    EOperation_SeekForward,
    EOperation_SeekStart,
    EOperation_SeekEnd,
    EOperation_Rotate,
    EOperation_Autoscale,
    EOperation_IncreaseHeightScale,
    EOperation_DecreaseHeightScale,
    EOperation_IncreaseWidthScale,
    EOperation_DecreaseWidthScale,
    EOperation_ClipRectangle,
    EOperation_Extent,
    EOperation_ResetToDefaults,
    EOperation_IncreaseVolume,
    EOperation_DecreaseVolume,
    EOperation_MaximumVolume,
    EOperation_Mute,
    EOperation_BalanceLeft,
    EOperation_BalanceRight,
    EOperation_BalanceCenter,
    EOperation_BalanceMaxLeft,
    EOperation_BalanceMaxRight,
    EOperation_FrameStepForward,
    EOperation_FrameStepBackward,
    EOperation_IncreasePlayVelocity,
    EOperation_DecreasePlayVelocity,
    EOperation_MoveWindowUp,
    EOperation_MoveWindowDown,
    EOperation_MoveWindowLeft,
    EOperation_MoveWindowRight,
    EOperation_ChangeWindowSize,
    EOperation_ToggleWindowTransparency,
    EOperation_DrawWhiteBorderAroundWindow,
    EOperation_DrawRedBorderAroundWindow,
    EOperation_ClearBorderAroundWindow
    };

class MPlayerWindowObserver
    {
public:
    
    virtual void PlayInitiated() = 0;
    
    virtual void PlayStateChanged() = 0;
    
    virtual void Error(TInt aError) = 0;
    };

class MPlayerCommands
    {
public:
    
    virtual TInt ExecuteOperation( TInt aOperation ) = 0;
    
    virtual void MoveWindow( TPoint aNewCenter ) = 0;
    
    virtual void SetOrdinalPosition( TInt aPosition ) = 0;
    };

class CPlayerWindow : public CBase, public MPlayerCommands
    {
public:

    enum TPlayState
        {
        EStopped,
        EPlaying,
        EPaused
        };

    enum TAutoscaleMode
        {
        EDoNotAutoscale,
        EBestFit,
        EStretch
        };
    
    virtual ~CPlayerWindow();

    // inherited from MPlayerCommands
    virtual TInt ExecuteOperation( TInt aOperation );
    virtual void MoveWindow( TPoint aNewCenter );    
    virtual void SetOrdinalPosition( TInt aPosition );

    void StartWithFilenameL( const TDesC& aFullFilename,
                             TAutoscaleMode aDefaultAutoscale,
                             MPlayerWindowObserver* aObserver = NULL );

    void StartWithUrlL( const TDesC& aUrl,
                        TAutoscaleMode aDefaultAutoscale,
                        MPlayerWindowObserver* aObserver = NULL );

    RWindow& Window();

    void WriteAdvancedText( CWindowGc& aGc, TInt aColumn, TInt& aRow, TInt aRowIncrement );        
    
    const TDesC& FullFilename();
    
    const TDesC& Filename();
    
    const TDesC& Url();
    
    TPlayState PlayState();

    TSize VideoSize();
        
    TInt PositionInMs();
    
    TInt DurationInMs();
    
    virtual TInt SetVideoExtent( TRect aExtent ) = 0;

    virtual TInt Play() = 0;
    
    virtual TInt Pause() = 0;

protected:
    
    CPlayerWindow( RFs& aFs, RWsSession& aWs, CWsScreenDevice& aScreenDevice, bool aSuppressGraphicsContext );
    
    void CheckWindowLimits();
    
    void DrawBorderAroundWindow();
    
    // These functions must be called by subclasses
    
    void BaseConstructL( const RWindowTreeNode &aParentWindow, TPoint aTopLeft, TSize aSize, bool aTransparent, TRgb aBackgroundColor );
    void PlayInitiated();
    void PlayComplete();    
    void CheckError( TInt aError, const char* pDescription );
    void Error( TInt aError, const char* pDescription = NULL );
    
    // These functions must be implemented by subclasses
    
    virtual void InitializeParameters( TSize& aVideoSize, 
                                       TTimeIntervalMicroSeconds& aDuration, 
                                       TInt& aMaxVolume, 
                                       TInt& aCurrentVolume,
                                       TInt& aBalanceCenter,
                                       TInt& aBalanceMaxLeft,
                                       TInt& aBalanceMaxRight,
                                       TInt& aDefaultBalance, 
                                       TInt& aCurrentBalance ) = 0;
    
    virtual void SetDefaultAutoscaleMode( TAutoscaleMode aDefaultAutoscale ) = 0;
    
    virtual void PreparePlayerWithFile( RFile& aFile ) = 0;

    virtual void PreparePlayerWithUrl( const TDesC& aUrl ) = 0;

    virtual TInt DoExecuteOperation( TInt aOperation ) = 0;

    virtual TTimeIntervalMicroSeconds Position() = 0; // microseconds
    
    virtual TInt SetVolume( TInt aVolume ) = 0;
    
    virtual TInt SetBalance( TInt aBalance ) = 0;
    
    virtual TInt RotationAngle() = 0;
    
    virtual TInt NextRotationAngle() = 0;
    
    virtual TInt ResetRotation() = 0;
    
    virtual TInt NextAutoscaleMode() = 0;
    
    virtual TInt ResetAutoscale() = 0;
    
    virtual void AppendAutoscaleModeText( TDes& aString ) = 0;
    
    virtual TInt SetClipRectangle( TRect aRect ) = 0;
    
    virtual TInt SetScaleFactor( TReal aWidthScaleFactor, TReal aHeightScaleFactor ) = 0;
    
    virtual TInt Stop() = 0;
    
    virtual TInt SetPosition( TTimeIntervalMicroSeconds ) = 0;
    
    virtual TInt StepForward() = 0;
    
    virtual TInt StepBackward() = 0;
    
    virtual TInt SetPlayVelocity( TInt aVelocity ) = 0;
    
    RFs&                    iFs;
    RFile                   iFile;
    RWsSession&             iWs;
    CWsScreenDevice&        iScreenDevice;
    CWindowGc*              iGc;
    
    RWindow                 iWindow;
    TBuf<120>               iBuffer;

private:
    
    TRect Rectangle( TInt aModeIndex );
    
    TInt Seek( TTimeIntervalMicroSeconds aPosition ); 
    
    TInt UpdateScaleFactor();

    TInt ResetToDefaults();
    
    TFileName                 iFullFilename;
    TFileName                 iFilename;
    TFileName                 iUrl;
    MPlayerWindowObserver*    iObserver;
    bool                      iSuppressGraphicsContext;
    
    TPlayState                iPlayState;
    TTimeIntervalMicroSeconds iDuration;
    TInt                      iDurationInMs;
    TInt                      iCurrentVelocity;
    
    // Video attributes
    TSize                     iVideoSize;
    TInt                      iWidthScaleIndex;
    TInt                      iHeightScaleIndex;   
    TInt                      iClipModeIndex;
    TInt                      iExtentModeIndex;
    TPoint                    iWindowTopLeft;
    TSize                     iDefaultWindowSize;
    TSize                     iWindowSize;
    TInt                      iWindowSizeDivisor;
    bool                      iWindowTransparent;
    TRgb                  iBorderColor;
        
    // Audio attributes
    TInt                      iMaxVolume;
    TInt                      iDefaultVolume;
    TInt                      iCurrentVolume;
    TInt                      iBalanceCenter;
    TInt                      iBalanceMaxRight;
    TInt                      iBalanceMaxLeft;
    TInt                      iDefaultBalance;
    TInt                      iCurrentBalance;
  
    };

#endif
