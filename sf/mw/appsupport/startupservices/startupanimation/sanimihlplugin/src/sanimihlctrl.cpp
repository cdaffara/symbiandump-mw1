/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CSAnimIhlCtrl class
*
*/


#include <IHLImageFactory.h>
#include <MIHLBitmap.h>
#include <MIHLFileImage.h>
#include <IHLViewerFactory.h>
#include <MIHLImageViewer.h>
#include "sanimobserver.h"

#include "sanimihlctrl.h"
#include "assert.h"
#include "trace.h"

/** Some GIF animations have all frame delays set to zero. */
const TInt KDefaultFrameDelay = 300000;

// ======== LOCAL FUNCTIONS ========

static TBool operator<=( const TSize& aLhs, const TSize& aRhs )
    {
    return aLhs.iWidth <= aRhs.iWidth && aLhs.iHeight <= aRhs.iHeight;
    }


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::NewL
//
// ---------------------------------------------------------------------------
//
CSAnimIhlCtrl* CSAnimIhlCtrl::NewL(
    MSAnimObserver& aObserver,
    RFs& aFs,
    const TDesC& aFileName,
    const TDisplayMode aDisplayMode,
    const TSize& aSize,
    const TTimeIntervalMicroSeconds32& aFrameDelay,
    const TBool aScalingEnabled )
    {
    FUNC_LOG;

    CSAnimIhlCtrl* self = new( ELeave ) CSAnimIhlCtrl( aObserver, aFrameDelay );
    CleanupStack::PushL( self );
    self->ConstructL( aFs, aFileName, aDisplayMode, aSize, aScalingEnabled );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::~CSAnimIhlCtrl
//
// ---------------------------------------------------------------------------
//
CSAnimIhlCtrl::~CSAnimIhlCtrl()
    {
    FUNC_LOG;

    Cancel();
    iTimer.Close();
    delete iViewer;
    iFrames.ResetAndDestroy();
    delete iImage;
    iFile.Close();
    delete iBuffer;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::BackroundColour
//
// ---------------------------------------------------------------------------
//
TRgb CSAnimIhlCtrl::BackroundColour() const
    {
    FUNC_LOG;

    return iImage->BackgroundColor();
    }


// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::Load
//
// ---------------------------------------------------------------------------
//
void CSAnimIhlCtrl::Load( const TCallBack& aCallBack )
    {
    FUNC_LOG;
    ASSERT_TRACE( iState == EInitial, SAnimPanic::EInternalError );

    if ( iState == EInitial )
        {
        iState = ELoading;
        iCallBack = aCallBack;
        iSuccessCode = KErrNone;
        delete iViewer;
        iViewer = NULL;
    
        if ( ( iType == EAnimation || iType == EStillImage ) && iFrames.Count() > 0 )
            {
            MIHLBitmap& frame = *( iFrames[ 0 ] );
            TRAP( iSuccessCode, iViewer = IHLViewerFactory::CreateImageViewerL(
                iViewerSize, *iImage, frame, *this ) );
            if ( iSuccessCode != KErrNone )
                {
                Notify( iSuccessCode, EFailed );
                }
            }
        else if ( iType == EMultiFrame && iFrameIndex < iFrameCount - 1 )
            {
            LoadNextSubImage();
            }
        else
            {
            Notify( KErrNotFound, EFailed );
            }
        }
    else
        {
        iSuccessCode = KErrAlreadyExists;
        aCallBack.CallBack();
        }
    }


// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::Play
//
// ---------------------------------------------------------------------------
//
void CSAnimIhlCtrl::Play( const TCallBack& aCallBack )
    {
    FUNC_LOG;
    ASSERT_TRACE( iState == ELoaded, SAnimPanic::EInternalError );

    if ( iState == ELoaded )
        {
        iState = EPlaying;
        iCallBack = aCallBack;
        iSuccessCode = KErrNone;
        iFrameIndex = -1;
        ShowNextFrame();
        }
    else
        {
        iSuccessCode = KErrNotReady;
        aCallBack.CallBack();
        }
    }


// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::SuccessCode
//
// ---------------------------------------------------------------------------
//
TInt CSAnimIhlCtrl::SuccessCode() const
    {
    FUNC_LOG;
    INFO_1( "Success code: %d", iSuccessCode );

    return iSuccessCode;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::Stop
//
// ---------------------------------------------------------------------------
//
void CSAnimIhlCtrl::Stop()
    {
    FUNC_LOG;
    INFO_1( "Stop: state %d", iState );

    if ( iState == ELoading || iState == EPlaying )
        {
        StopAndNotify( KErrCancel, EFailed );
        }
    }


// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::DoCancel
//
// ---------------------------------------------------------------------------
//
void CSAnimIhlCtrl::DoCancel()
    {
    FUNC_LOG;

    iImage->CancelLoad();
    iTimer.Cancel();
    }


// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::RunL
//
// ---------------------------------------------------------------------------
//
void CSAnimIhlCtrl::RunL()
    {
    FUNC_LOG;
    ERROR( iStatus.Int(), "Timer completed with error" );

    if ( iStatus.Int() == KErrNone )
        {
        if ( iState == ELoading )
            {
            if ( iType == EMultiFrame && iFrameIndex < iFrameCount - 1 )
                {
                LoadNextSubImage();
                }
            else
                {
                iFrameIndex = -1;
                Notify( KErrNone, ELoaded );
                }
            }
        else if ( iState == EPlaying )
            {
            ShowNextFrame();
            }
        }
    else
        {
        Notify( iStatus.Int(), EFailed );
        }
    }


// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::CSAnimIhlCtrl
//
// ---------------------------------------------------------------------------
//
CSAnimIhlCtrl::CSAnimIhlCtrl(
    MSAnimObserver& aObserver,
    const TTimeIntervalMicroSeconds32& aFrameDelay )
  : CActive( EPriorityStandard ),
    iObserver( aObserver ),
    iFrameDelay( aFrameDelay ),
    iFrameIndex( -1 ),
    iSuccessCode( KErrNone ),
    iType( EStillImage ),
    iState( EInitial )
    {
    FUNC_LOG;

    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::ConstructL
//
// ---------------------------------------------------------------------------
//
void CSAnimIhlCtrl::ConstructL(
    RFs& aFs,
    const TDesC& aFileName,
    const TDisplayMode /*aDisplayMode*/,
    const TSize& aSize,
    const TBool aScalingEnabled )
    {
    FUNC_LOG;

    User::LeaveIfError( iTimer.CreateLocal() );
    User::LeaveIfError( iFile.Open(
        aFs, aFileName, EFileRead | EFileShareReadersOnly ) );
	
    TInt fileSize;
    iFile.Size( fileSize );
    iBuffer = HBufC8::NewL( fileSize );
    
    TPtr8 fileDes = iBuffer->Des();
    iFile.Read( fileDes, fileSize );
    iImage = IHLImageFactory::OpenBufferedFileImageL(aFs,*iBuffer );

    iFrameCount = 1; // If it is not an animation, frame count is 1.
    if ( iImage->IsAnimation() )
        {
        iFrameCount = iImage->AnimationFrameCount();
        iType = EAnimation;
        }
    else if ( iImage->ImageCount() > 1 )
        {
        iFrameCount = iImage->ImageCount();
        iType = EMultiFrame;
        }

    INFO_2( "Type: %d, frame count: %d", iType, iFrameCount );

    iViewerSize = SelectSize( aSize, aScalingEnabled );
    if ( iType == EAnimation || iType == EStillImage )
        {
        MIHLBitmap* frame = IHLBitmap::CreateL();
        CleanupStack::PushL( frame );
        iFrames.AppendL( frame );
        CleanupStack::Pop( frame );
        }
    else if ( iType == EMultiFrame )
        {
        for ( TInt i = 0; i < iFrameCount; i++ )
            {
            MIHLBitmap* frame = IHLBitmap::CreateL();
            CleanupStack::PushL( frame );
            User::LeaveIfError( frame->Create( iViewerSize, iImage->DisplayMode(), EGray256 ) );
            iFrames.AppendL( frame );
            CleanupStack::Pop( frame );
            }
        }
    else
        {
        ERROR_GEN_1( "ConstructL: type %d unexpected", iType );
        User::Leave( KErrGeneral );
        }
    }


// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::SelectSize
//
// ---------------------------------------------------------------------------
//
TSize CSAnimIhlCtrl::SelectSize(
    const TSize& aSize,
    const TBool aScalingEnabled ) const
    {
    FUNC_LOG;

    TSize imageSize = iImage->Size(); // Default for all elses
    if ( aScalingEnabled && aSize != TSize( 0, 0 ) ) // Scaling requested
        {
        if ( iImage->IsFullyScaleable() )
            {
            INFO( "Image is fully scaleable" );

            imageSize = aSize;
            }
        else
            {
            const RArray<TSize>& loadSizeArray = iImage->CustomLoadSizeArray();
            if ( loadSizeArray.Count() > 0 )
                {
                INFO( "Image has custom load size array" );

                imageSize = FindClosestMatch( aSize, imageSize, loadSizeArray );
                }
            }
        }

    return imageSize;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::FindClosestMatch
//
// ---------------------------------------------------------------------------
//
TSize CSAnimIhlCtrl::FindClosestMatch(
    const TSize& aTarget,
    const TSize& aOptimalSize,
    const RArray<TSize>& aOptions ) const
    {
    FUNC_LOG;
    INFO_2( "Target size: (%d, %d)", aTarget.iWidth, aTarget.iHeight );
    INFO_2( "Optimal size: (%d, %d)", aOptimalSize.iWidth, aOptimalSize.iHeight );

    TSize selected( 0, 0 );
    if ( aOptimalSize <= aTarget )
        {
        selected = aOptimalSize;
        }

    TInt count = aOptions.Count();
    for ( TInt i = 0; i < count; i++ )
        {
        TSize item = aOptions[i];

        INFO_3( "Option %d: (%d, %d)", i, item.iWidth, item.iHeight );

        if ( item.iHeight <= aTarget.iHeight && item.iWidth <= aTarget.iWidth ) // Acceptable
            {
            if ( ( aTarget.iHeight - item.iHeight ) <
                 ( aTarget.iHeight - selected.iHeight ) )
                {
                selected = item;
                }
            }
        }

    if ( selected == TSize( 0, 0 ) ) // No match found
        {
        selected = aOptimalSize;
        }

    INFO_2( "Selected option: (%d, %d)", selected.iWidth, selected.iHeight );
    return selected;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::LoadNextSubImage
//
// ---------------------------------------------------------------------------
//
void CSAnimIhlCtrl::LoadNextSubImage()
    {
    FUNC_LOG;
    ASSERT_TRACE( iState == ELoading && !IsActive(), SAnimPanic::EInternalError );

    iFrameIndex++;

    INFO_2( "LoadNextSubImage: index: %d, frame count: %d", iFrameIndex, iFrameCount );

    ASSERT_TRACE( iFrameIndex < iFrames.Count(), SAnimPanic::EInternalError );
    MIHLBitmap* nextFrame = iFrames[ iFrameIndex ];

    iSuccessCode = iImage->Load( iStatus, *nextFrame, iFrameIndex );

    if ( iSuccessCode == KErrNone )
        {
        SetActive();
        }
    else
        {
        Notify( iSuccessCode, EFailed );
        }
    }


// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::ShowNextFrame
//
// ---------------------------------------------------------------------------
//
void CSAnimIhlCtrl::ShowNextFrame()
    {
    FUNC_LOG;
    ASSERT_TRACE( iState == EPlaying, SAnimPanic::EInternalError );

    if ( iFrameIndex < iFrameCount - 1 )
        {
        iFrameIndex++;

        INFO_2( "ShowNextFrame: index: %d, frame count: %d", iFrameIndex, iFrameCount );

        if ( iType == EAnimation && iViewer )
            {
            UpdateFrameToScreen( 0 );
            iViewer->Play();
            }
        else if ( iType == EStillImage )
            {
            UpdateFrameToScreen( 0 );
            StartFrameTimer();
            }
        else if ( iType == EMultiFrame )
            {
            UpdateFrameToScreen( iFrameIndex );
            StartFrameTimer();
            }
        else
            {
            ERROR_GEN_1( "ShowNextFrame: type %d unexpected", iType );
            Notify( KErrGeneral, EFailed );
            }
        }
    else
        {
        StopAndNotify( KErrNone, EFinished );
        }
    }


// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::UpdateFrameToScreen
//
// ---------------------------------------------------------------------------
//
void CSAnimIhlCtrl::UpdateFrameToScreen( TInt aFrameIndex ) const
    {
    FUNC_LOG;

    if ( aFrameIndex >= 0 && aFrameIndex < iFrames.Count() )
        {
        MIHLBitmap& frame = *( iFrames[ aFrameIndex ] );
        if ( frame.HasMask() )
            {
            iObserver.UpdateScreen( frame.Bitmap(), frame.Mask() );
            }
        else
            {
            iObserver.UpdateScreen( frame.Bitmap() );
            }
        }
    else
        {
        ERROR_GEN_1( "UpdateFrameToScreen: aFrameIndex %d out of bounds", aFrameIndex );
        }
    }


// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::StartFrameTimer
//
// ---------------------------------------------------------------------------
//

void CSAnimIhlCtrl::StartFrameTimer()
    {
    FUNC_LOG;
    ASSERT_TRACE( !IsActive(), SAnimPanic::EInternalError );

    if ( !IsActive() )
        {
        TTimeIntervalMicroSeconds32 delay = ( iFrameDelay.Int() == 0 ?
            KDefaultFrameDelay : iFrameDelay );
        INFO_2( "Frame delay: %d, default delay: %d", delay.Int(), iFrameDelay.Int() );
        iTimer.Cancel();
        iTimer.After( iStatus, delay );
        SetActive();
        }
    }


// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::StopAndNotify
//
// ---------------------------------------------------------------------------
//
void CSAnimIhlCtrl::StopAndNotify( TInt aError, TInt aNextState )
    {
    FUNC_LOG;

    Cancel();
    if ( iViewer )
        {
        iViewer->Stop();
        }
    Notify( aError, aNextState );
    }


// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::Notify
//
// ---------------------------------------------------------------------------
//
void CSAnimIhlCtrl::Notify( TInt aError, TInt aNextState )
    {
    FUNC_LOG;
    ERROR_1( aError, "Notify: state %d", aNextState );

    iSuccessCode = aError;
    iState = aNextState;
    iCallBack.CallBack();
    }


// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::ViewerBitmapChangedL
//
// ---------------------------------------------------------------------------
//
void CSAnimIhlCtrl::ViewerBitmapChangedL()
    {
    FUNC_LOG;
    INFO_1( "ViewerBitmapChangedL: state %d", iState );

    if ( iState == ELoading )
        {
        iFrameIndex = -1;
        Notify( KErrNone, ELoaded );
        }
    else if ( iState == EPlaying )
        {
        ShowNextFrame();
        }
    else
        {
        StopAndNotify( KErrGeneral, EFailed );
        }
    }


// ---------------------------------------------------------------------------
// CSAnimIhlCtrl::ViewerError
//
// ---------------------------------------------------------------------------
//
void CSAnimIhlCtrl::ViewerError( TInt aError )
    {
    FUNC_LOG;

    StopAndNotify( aError, EFailed );
    }

