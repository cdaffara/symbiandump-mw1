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
* Description:  Implementation of CSAnimCtrl class
*
*/


#include "sanimctrl.h"
#include "sanimengine.h"

#include "trace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSAnimCtrl::NewL
//
// ---------------------------------------------------------------------------
//
EXPORT_C CSAnimCtrl* CSAnimCtrl::NewL(
    const TRect& aRect,
    const CCoeControl& aContainer )
    {
    FUNC_LOG;

    CSAnimCtrl* self = new( ELeave ) CSAnimCtrl;
    CleanupStack::PushL( self );
    self->BaseConstructL( aRect, aContainer );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrl::~CSAnimCtrl
//
// ---------------------------------------------------------------------------
//
CSAnimCtrl::~CSAnimCtrl()
    {
    FUNC_LOG;

    delete iEngine;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrl::Load
//
// ---------------------------------------------------------------------------
//
void CSAnimCtrl::Load(
    const TDesC& aImageFileName,
    const TTimeIntervalMicroSeconds32& aFrameDelay,
    const TBool aScalingEnabled,
    const TInt aAnimRepeatCount,
    const TDesC& aToneFileName,
    const TInt aVolume,
    const TTimeIntervalMicroSeconds& aVolumeRamp,
    const TInt aToneRepeatCount,
    TRequestStatus& aStatus )
    {
    FUNC_LOG;

    TDisplayMode dispMode = EColor16M;
    CGraphicsDevice* device = SystemGc().Device();
	if ( device )
	    {
	    dispMode = device->DisplayMode();
	    }

    TInt errorCode = iEngine->SetImageProperties(
        aImageFileName,
        dispMode,
        Size(),
        aFrameDelay,
        aScalingEnabled,
        aAnimRepeatCount );
    ERROR( errorCode, "Failed to set image properties" );

    errorCode = iEngine->SetToneProperties(
        aToneFileName,
        aVolume,
        aVolumeRamp,
        aToneRepeatCount );
    ERROR( errorCode, "Failed to set tone properties" );

    iEngine->Load( aStatus );
    }


// ---------------------------------------------------------------------------
// CSAnimCtrl::Start
//
// ---------------------------------------------------------------------------
//
void CSAnimCtrl::Start( TRequestStatus& aStatus )
    {
    FUNC_LOG;

    iBackgroundColour = iEngine->BackroundColour();
    iEngine->Start( aStatus );
    }


// ---------------------------------------------------------------------------
// CSAnimCtrl::Cancel
//
// ---------------------------------------------------------------------------
//
void CSAnimCtrl::Cancel()
    {
    FUNC_LOG;

    Clear();
    iEngine->Cancel();
    }


// ---------------------------------------------------------------------------
// CSAnimCtrl::Clear
//
// ---------------------------------------------------------------------------
//
void CSAnimCtrl::Clear()
    {
    FUNC_LOG;

    iBackgroundColour = TRgb();
    iImage = NULL;
    iMask = NULL;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrl::CountComponentControls
//
// ---------------------------------------------------------------------------
TInt CSAnimCtrl::CountComponentControls() const
    {
    return 0;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrl::ComponentControl
//
// ---------------------------------------------------------------------------
CCoeControl* CSAnimCtrl::ComponentControl( TInt /*aIndex*/ ) const
    {
    return NULL;
    }


// ---------------------------------------------------------
// CSAnimCtrl::Draw
//
// ---------------------------------------------------------
//
void CSAnimCtrl::Draw( const TRect& /*aRect*/ ) const
    {
    FUNC_LOG;

	CWindowGc& gc = SystemGc();

    TRect rect = Rect();
    gc.Clear( rect );
    gc.SetPenStyle( CGraphicsContext::ENullPen );
    gc.SetBrushColor( iBackgroundColour );
    gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc.Clear();

    TPoint pos = rect.iTl;
    TSize imgSize( 0, 0 );
    if ( iImage )
        {
        imgSize = iImage->SizeInPixels();
        }

    pos.iX += ( rect.Width() - imgSize.iWidth ) / 2;
    pos.iY += ( rect.Height() - imgSize.iHeight ) / 2;

    if ( iImage && iMask )
        {
        INFO_2( "Frame size: %d, %d", imgSize.iWidth, imgSize.iHeight );
        INFO_2( "Mask size: %d, %d", iMask->SizeInPixels().iWidth, iMask->SizeInPixels().iHeight );
        INFO_2( "Position: %d, %d", pos.iX, pos.iY );

        gc.BitBltMasked( pos, iImage, imgSize, iMask, EFalse );
	    }
    else if ( iImage )
        {
        INFO_2( "Frame size: %d, %d", imgSize.iWidth, imgSize.iHeight );
        INFO_2( "Position: %d, %d", pos.iX, pos.iY );

    	gc.BitBlt( pos, iImage );
	    }
	else
	    {
        INFO( "No frame or mask" );
	    }
    }


// ---------------------------------------------------------
// CSAnimCtrl::UpdateScreen
//
// ---------------------------------------------------------
//
void CSAnimCtrl::UpdateScreen(
    const CFbsBitmap& aFrame,
    const CFbsBitmap& aMask )
    {
    FUNC_LOG;

    iImage = &aFrame;
    iMask = &aMask;
    DrawNow();
    }


// ---------------------------------------------------------
// CSAnimCtrl::UpdateScreen
//
// ---------------------------------------------------------
//
void CSAnimCtrl::UpdateScreen( const CFbsBitmap& aFrame )
    {
    FUNC_LOG;

    iImage = &aFrame;
    iMask = NULL;
    DrawNow();
    }


// ---------------------------------------------------------------------------
// CSAnimCtrl::CSAnimCtrl
//
// ---------------------------------------------------------------------------
//
EXPORT_C CSAnimCtrl::CSAnimCtrl()
  : iEngine( NULL ),
    iImage( NULL ),
    iMask( NULL )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrl::BaseConstructL
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CSAnimCtrl::BaseConstructL(
    const TRect& aRect,
    const CCoeControl& aContainer )
    {
    FUNC_LOG;

    iEngine = CSAnimEngine::NewL( iCoeEnv->FsSession(), *this );

    SetContainerWindowL( aContainer );
    SetRect( aRect );
    ActivateL();
    }
