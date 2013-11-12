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
 * The source of the common Player Window functionality.
 * 
 */

#include "playerwindow.h"
#include "consoleplayer.h"

const TInt  KDefaultScalePercentagesIndex = 5;
const TInt  KScalePercentagesCount = 10;
const TReal KScalePercentages[] = {10.0, 25.0, 50.0, 75.0, 100.0, 125.0, 150.0, 175.0, 200.0, 250.0};

// this constant must be consistent with the Rectangle function
const TInt KRectangleModeCount = 5;

const TInt KMoveWindowDelta = 8;


CPlayerWindow::CPlayerWindow( RFs& aFs, RWsSession& aWs, CWsScreenDevice& aScreenDevice, bool aSuppressGraphicsContext ) :
    iFs( aFs ),
    iWs( aWs ),
    iScreenDevice( aScreenDevice ),
    iWindow( aWs ),
	iSuppressGraphicsContext( aSuppressGraphicsContext ),
    iCurrentVelocity( 100 ),
    iWidthScaleIndex( KDefaultScalePercentagesIndex ),
    iHeightScaleIndex( KDefaultScalePercentagesIndex )
    {    
    }

void CPlayerWindow::BaseConstructL( const RWindowTreeNode &aParentWindow, TPoint aTopLeft, TSize aSize, bool aTransparent, TRgb aBackgroundColor )
    {
    // A graphics context should not be used with performance mode.
    if( !iSuppressGraphicsContext )
    {
        User::LeaveIfError( iScreenDevice.CreateContext(iGc) );
    }
    
    User::LeaveIfError( iWindow.Construct( aParentWindow, KNullWsHandle ) );

    iWindowTopLeft = aTopLeft;
    iDefaultWindowSize = aSize;
    iWindowSize = aSize;
    iWindowSizeDivisor = 1;
    
    iWindow.SetExtent( aTopLeft, aSize );
    if( aTransparent )
        {
        iWindow.SetTransparencyAlphaChannel();
        }
    else
        {
        iWindow.SetNonTransparent();    
        }
    iWindow.SetBackgroundColor(aBackgroundColor); 
    iWindow.SetVisible(false);
    iWindow.Activate();
    
    // enable drag events
    iWindow.PointerFilter(EPointerFilterDrag, 0);
    }

CPlayerWindow::~CPlayerWindow()
    {
    delete iGc;
    iFile.Close();
    }

void CPlayerWindow::StartWithFilenameL( const TDesC& aFullFilename,
                                        TAutoscaleMode aDefaultAutoscale,
                                        MPlayerWindowObserver* aObserver )
    {
    iFullFilename.Copy( aFullFilename );
    iObserver = aObserver;        

    SetDefaultAutoscaleMode( aDefaultAutoscale );
    
    User::LeaveIfError( iFile.Open( iFs, iFullFilename, EFileShareReadersOnly | EFileStream | EFileRead ) );

    iFile.Name( iFilename );
    
    PreparePlayerWithFile( iFile );
    
    // Now wait for subclass to call PrepareComplete.        
    }

void CPlayerWindow::StartWithUrlL( const TDesC& aUrl,
                                   TAutoscaleMode aDefaultAutoscale,
                                   MPlayerWindowObserver* aObserver )
    {
    iUrl.Copy( aUrl );
    iObserver = aObserver;        

    SetDefaultAutoscaleMode( aDefaultAutoscale );
    
    PreparePlayerWithUrl( aUrl );
    
    // Now wait for subclass to call PrepareComplete.        
    }

RWindow& CPlayerWindow::Window()
    {
    return iWindow;
    }

const TDesC& CPlayerWindow::FullFilename()
    {
    return iFullFilename;
    }

const TDesC& CPlayerWindow::Filename()
    {
    return iFilename;
    }

const TDesC& CPlayerWindow::Url()
    {
    return iUrl;
    }

void CPlayerWindow::PlayInitiated()
    {
    InitializeParameters( iVideoSize, 
                          iDuration, 
                          iMaxVolume, 
                          iCurrentVolume,
                          iBalanceCenter,
                          iBalanceMaxLeft,
                          iBalanceMaxRight,
                          iDefaultBalance, 
                          iCurrentBalance );
    
    iDurationInMs = iDuration.Int64() / 1000;
    
    if( iBorderColor != 0 )
        {
        DrawBorderAroundWindow();
        }
    
    iWindow.SetVisible(true);  
    
    iPlayState = EPlaying;

    if( iObserver )
        {
        iObserver->PlayInitiated();
        }
    }

void CPlayerWindow::PlayComplete()
    {
    iPlayState = EStopped; 
    
    if( iObserver )
        {
        iObserver->PlayStateChanged();
        }       
    }

void CPlayerWindow::Error( TInt aError, const char* pDescription )
    {
    if( pDescription != NULL )
        {
        RDebug::Printf( "CONSOLEPLAYER ERROR %i %s", pDescription );
        }
    
    if( iObserver )
        {
        iObserver->Error( aError );
        }       
    }

void CPlayerWindow::CheckError( TInt aError, const char* pDescription )
    {
    if( aError != KErrNone )
        {
        Error( aError, pDescription );
        }
    }

CPlayerWindow::TPlayState CPlayerWindow::PlayState()
    {
    return iPlayState;
    }

TInt CPlayerWindow::PositionInMs()
    {    
    return Position().Int64() / 1000;
    }

TInt CPlayerWindow::DurationInMs()
    {    
    return iDurationInMs;;
    }

TSize CPlayerWindow::VideoSize()
    {    
    return iVideoSize;
    }

TInt CPlayerWindow::Seek( TTimeIntervalMicroSeconds position )
    {
    RDebug::Printf( "CONSOLEPLAYER Seek %lu", position.Int64() );	
    	
    if( position < 0 )
        {
        position = 0;
        }           
    else if( position > iDuration )
        {
        position = iDuration;
        }

    TInt err = Pause();
    
    if( err == KErrNone )
        {
        err = SetPosition( position.Int64() );
        }

    if( (err == KErrNone) && (iPlayState == EPlaying) )
        {
        err = Play();
        }
    
    return err;
    }

TInt CPlayerWindow::UpdateScaleFactor()
    {
    return SetScaleFactor(KScalePercentages[iWidthScaleIndex], KScalePercentages[iHeightScaleIndex]);
    }

TInt CPlayerWindow::ResetToDefaults()
    {
    iWidthScaleIndex = KDefaultScalePercentagesIndex;
    iHeightScaleIndex = KDefaultScalePercentagesIndex;
    iExtentModeIndex = 0;
    iClipModeIndex = 0;
    iCurrentVelocity = 100;
    iCurrentVolume = iDefaultVolume;
    iCurrentBalance = iDefaultBalance;
    iWindowSize = iDefaultWindowSize;
    iWindowSizeDivisor = 1;
    ResetAutoscale();
    ResetRotation();    
    UpdateScaleFactor();
    SetClipRectangle(Rectangle(iClipModeIndex));
    SetVideoExtent(Rectangle(iExtentModeIndex));
    SetPlayVelocity(iCurrentVelocity);
    SetVolume(iCurrentVolume);
    SetBalance(iCurrentBalance);    
    return KErrNone;
    }

// Iterates through a standard set of rectangles for the clipping and extent.
TRect CPlayerWindow::Rectangle( TInt aModeIndex )
    {
    TRect rectangle;
    
    TSize windowSize = iWindow.Size();
        
    switch( aModeIndex )
        {
        case 0:
            // Full window.
            rectangle = TRect(windowSize); ;
            break;
        case 1:
            // 100x100 square in center of window.
            rectangle = TRect( windowSize.iWidth/2 - 50, windowSize.iHeight/2 - 50,
                               windowSize.iWidth/2 + 50, windowSize.iHeight/2 + 50 );
            break;
        case 2:
            // Top half of window.
            rectangle = TRect( 0, 0, windowSize.iWidth, windowSize.iHeight/2 );
            break;
        case 3:
            // Left half of window.
            rectangle = TRect( 0, 0, windowSize.iWidth/2, windowSize.iHeight );
            break;
        case 4:
            // Top-Left quarter of window.
            rectangle = TRect( 0, 0, windowSize.iWidth/2, windowSize.iHeight/2 );
            break;
        }
    
    return rectangle;        
    }

// Checks to make sure that the second video window is within the limits of the display.
void CPlayerWindow::CheckWindowLimits()
    {
    if( iWindowTopLeft.iX < 0 )
        {
        iWindowTopLeft.iX = 0;
        }
    else if( iWindowTopLeft.iX + iWindowSize.iWidth > iScreenDevice.SizeInPixels().iWidth )
        {
        iWindowTopLeft.iX = iScreenDevice.SizeInPixels().iWidth - iWindowSize.iWidth;
        }

    if( iWindowTopLeft.iY < 0 )
        {
        iWindowTopLeft.iY = 0;
        }
    else if( iWindowTopLeft.iY + iWindowSize.iHeight > iScreenDevice.SizeInPixels().iHeight )
        {
        iWindowTopLeft.iY = iScreenDevice.SizeInPixels().iHeight - iWindowSize.iHeight;
        }
    }

TInt CPlayerWindow::ExecuteOperation( TInt aOperation )
    {
    TInt err = KErrNone;
    
    switch( aOperation )
        {
        case EOperation_StopPlay:
            {
            if( iPlayState == EStopped )
                {
                err = Play();
                iPlayState = EPlaying;
                }
            else
                {
                err = Stop();
                iPlayState = EStopped;
                }
            break;
            }
        case EOperation_PauseResume:
            {
            if( iPlayState == EPaused )
                {
                err = Play();
                iPlayState = EPlaying;
                }
            else if( iPlayState == EPlaying )
                {
                err = Pause();                    
                iPlayState = EPaused;
                }
            break;
            }
        case EOperation_Pause:
            {
            if( iPlayState == EPlaying )
                {
                err = Pause();                    
                iPlayState = EPaused;
                }
            break;
            }
        case EOperation_Resume:
            {
            if( iPlayState == EPaused )
                {
                err = Play();
                iPlayState = EPlaying;
                }
            break;
            }
        case EOperation_SeekBack:
            {
            // Reverse seek 5 seconds
            TInt64 pos = Position().Int64();
            pos -= 5000000;
            err = Seek( pos );
            break;
            }
        case EOperation_SeekForward:
            {
            // Forward 5 seconds
            TInt64 pos = Position().Int64();
            pos += 5000000;
            err = Seek( pos );
            break;
            }
        case EOperation_SeekStart:
            {
            err = Seek( 0 );
            break;
            }
        case EOperation_SeekEnd:
            {
            // Seek to 5 seconds before the end of the clip.
            TInt64 pos = iDuration.Int64() - 5000000;
            err = Seek( pos );
            break;
            }
        case EOperation_FrameStepForward:    
            {
            err = StepForward();
            break;
            }
        case EOperation_FrameStepBackward:    
            {
            err = StepBackward();
            break;
            }
        case EOperation_IncreasePlayVelocity:    
            {
            iCurrentVelocity += 50;
            err = SetPlayVelocity( iCurrentVelocity );
            break;
            }
        case EOperation_DecreasePlayVelocity:    
            {
            iCurrentVelocity -= 50;
            err = SetPlayVelocity( iCurrentVelocity );
            break;
            }
        case EOperation_IncreaseHeightScale:
            {
            if( iHeightScaleIndex < KScalePercentagesCount )
                {
                iHeightScaleIndex++;                            
                }
            err = UpdateScaleFactor();
            break;
            }
        case EOperation_DecreaseHeightScale:
            {
            if( iHeightScaleIndex > 0 )
                {
                iHeightScaleIndex--;
                }                    
            err = UpdateScaleFactor();
            break;
            }                
        case EOperation_IncreaseWidthScale:    
            {
            if( iWidthScaleIndex < KScalePercentagesCount )
                {
                iWidthScaleIndex++;                            
                }                        
            err = UpdateScaleFactor();
            break;
            }                
        case EOperation_DecreaseWidthScale:    
            {
            if( iWidthScaleIndex > 0 )
                {
                iWidthScaleIndex--;                            
                }                        
            err = UpdateScaleFactor();
            break;
            }
        case EOperation_Rotate:
            {
            err = NextRotationAngle();
            break;
            }                
        case EOperation_Autoscale:
            {
            err = NextAutoscaleMode();
            break;
            }                
        case EOperation_ClipRectangle:    
            {
            iClipModeIndex = (iClipModeIndex+1) % KRectangleModeCount;
            err = SetClipRectangle(Rectangle( iClipModeIndex ));                        
            break;
            }                
        case EOperation_Extent:    
            {
            // Advance to next extent mode.
            iExtentModeIndex = (iExtentModeIndex+1) % KRectangleModeCount;
            err = SetVideoExtent(Rectangle( iExtentModeIndex ));
            break;
            }                
        case EOperation_ResetToDefaults:    
            {
            err = ResetToDefaults();
            break;
            }
        case EOperation_IncreaseVolume:
            {
            err = SetVolume( iCurrentVolume + iMaxVolume / 10 );
            break;
            }
        case EOperation_DecreaseVolume:
            {
            err = SetVolume( iCurrentVolume - iMaxVolume / 10 );
            break;
            }
        case EOperation_MaximumVolume:
            {
            err = SetVolume( iMaxVolume );
            break;
            }            
        case EOperation_Mute:
            {
            err = SetVolume( 0 );
            break;
            }
        case EOperation_BalanceLeft:
            {
            err = SetBalance( iCurrentBalance - 10 );
            break;
            }
        case EOperation_BalanceRight:
            {
            err = SetBalance( iCurrentBalance + 10 );
            break;
            }
        case EOperation_BalanceCenter:
            {
            err = SetBalance( iBalanceCenter );
            break;
            }
        case EOperation_BalanceMaxLeft:
            {
            err = SetBalance( iBalanceMaxLeft );
            break;
            }
        case EOperation_BalanceMaxRight:
            {
            err = SetBalance( iBalanceMaxRight );
            break;
            }                 
        case EOperation_MoveWindowUp:
            {
            iWindowTopLeft.iY -= KMoveWindowDelta;
            CheckWindowLimits();
            iWindow.SetExtent( iWindowTopLeft, iWindowSize );
            break;
            }
        case EOperation_MoveWindowDown:
            {
            iWindowTopLeft.iY += KMoveWindowDelta;
            CheckWindowLimits();
            iWindow.SetExtent( iWindowTopLeft, iWindowSize );
            break;
            }
        case EOperation_MoveWindowLeft:
            {
            iWindowTopLeft.iX -= KMoveWindowDelta;
            CheckWindowLimits();
            iWindow.SetExtent( iWindowTopLeft, iWindowSize );
            break;
            }
        case EOperation_MoveWindowRight:
            {
            iWindowTopLeft.iX += KMoveWindowDelta;
            CheckWindowLimits();
            iWindow.SetExtent( iWindowTopLeft, iWindowSize );
            break;
            }
        case EOperation_ChangeWindowSize:                
            {
            iWindowSizeDivisor = (iWindowSizeDivisor % 5) + 1;

            iWindowSize = TSize(iDefaultWindowSize.iWidth/iWindowSizeDivisor,
                                iDefaultWindowSize.iHeight/iWindowSizeDivisor);
            CheckWindowLimits();
            iWindow.SetExtent( iWindowTopLeft, iWindowSize);
            SetVideoExtent( TRect(iWindowSize) );
            
            if( iBorderColor != 0 )
                {
                DrawBorderAroundWindow();
                }
            break;
            }
        case EOperation_ToggleWindowTransparency:                
            {
            // Black is completely transparent, and white is completely opaque.
            iWindowTransparent = !iWindowTransparent;
            iWindow.SetTransparencyFactor( KRgbGray );
            iWindow.SetSurfaceTransparency( iWindowTransparent );
            break;
            }
        case EOperation_DrawWhiteBorderAroundWindow:
            {
            iBorderColor = 0xfefefe;
            DrawBorderAroundWindow();
            break;
            }
        case EOperation_DrawRedBorderAroundWindow:
            {
            iBorderColor = KRgbRed;
            DrawBorderAroundWindow();
            break;
            }
        case EOperation_ClearBorderAroundWindow:
            {            
            iBorderColor = 0;
            if( iGc )
                {
                iGc->Activate(iWindow);            
                iWindow.Invalidate();
                iWindow.BeginRedraw();            
                iGc->Reset();            
                iWindow.EndRedraw();                        
                iGc->Deactivate();
                }
            break;
            }
        default:
            {
            // Give operation to subclass to execute.
            err = DoExecuteOperation( aOperation );
            break;
            }
        }    
    
    return err;
    }

void CPlayerWindow::DrawBorderAroundWindow()
    {
    if( iGc )
        {
        iGc->Activate(iWindow);
        
        iWindow.Invalidate();
        iWindow.BeginRedraw();
        
        iGc->Reset();
        
        iGc->SetPenColor(iBorderColor);
        iGc->SetPenStyle(CWindowGc::ESolidPen);            
        iGc->DrawRect( TRect(iWindow.Size()) );
                    
        iWindow.EndRedraw();
        
        iGc->Deactivate();
        }
    }


void CPlayerWindow::WriteAdvancedText( CWindowGc& aGc, TInt aColumn, TInt& aRow, TInt aRowIncrement )
    {
    TRect rect = Rectangle( iExtentModeIndex );
    iBuffer.Format( _L("Extent: (%i,%i) (%i,%i)"), rect.iTl.iX, rect.iTl.iY, rect.iBr.iX, rect.iBr.iY );
    aGc.DrawText( iBuffer, TPoint(aColumn, aRow) );
    aRow += aRowIncrement;
                    
    rect = Rectangle( iClipModeIndex );
    iBuffer.Format( _L("Clip: (%i,%i) (%i,%i)"), rect.iTl.iX, rect.iTl.iY, rect.iBr.iX, rect.iBr.iY );
    aGc.DrawText( iBuffer, TPoint(aColumn, aRow) );
    aRow += aRowIncrement;
    
    iBuffer.Copy( _L("Autoscale: ") );
    AppendAutoscaleModeText( iBuffer );
    aGc.DrawText( iBuffer, TPoint(aColumn, aRow) );
    aRow += aRowIncrement;
    
    aGc.DrawText( _L("Scale factor:"), TPoint(aColumn, aRow) );
    aRow += aRowIncrement;
    
    iBuffer.Format( _L("width=%.1f"), KScalePercentages[iWidthScaleIndex] );
    aGc.DrawText( iBuffer, TPoint(aColumn+25, aRow) );
    aRow += aRowIncrement;
    
    iBuffer.Format( _L("height=%.1f"), KScalePercentages[iHeightScaleIndex] );
    aGc.DrawText( iBuffer, TPoint(aColumn+25, aRow) );
    aRow += aRowIncrement;
    
    iBuffer.Format( _L("Rotation: %i"), RotationAngle() );
    aGc.DrawText( iBuffer, TPoint(aColumn, aRow) );
    aRow += aRowIncrement;
    
    iBuffer.Format( _L("Velocity: %i"), iCurrentVelocity );
    aGc.DrawText( iBuffer, TPoint(aColumn, aRow) );
    aRow += aRowIncrement;
    
    iBuffer.Format( _L("Volume: %i/%i"), iCurrentVolume, iMaxVolume );
    aGc.DrawText( iBuffer, TPoint(aColumn, aRow) );
    aRow += aRowIncrement;
    
    iBuffer.Format( _L("Balance: %i"), iCurrentBalance );
    aGc.DrawText( iBuffer, TPoint(aColumn, aRow) );
    aRow += aRowIncrement;    
    }

void CPlayerWindow::MoveWindow( TPoint aCenter )
    {    
    iWindowTopLeft.iX = aCenter.iX - iWindowSize.iWidth / 2;
    iWindowTopLeft.iY = aCenter.iY- iWindowSize.iHeight / 2;;
    CheckWindowLimits();
    iWindow.SetExtent( iWindowTopLeft, iWindowSize );
    }

void CPlayerWindow::SetOrdinalPosition( TInt aPosition )
    {
    iWindow.SetOrdinalPosition( aPosition );
    }
