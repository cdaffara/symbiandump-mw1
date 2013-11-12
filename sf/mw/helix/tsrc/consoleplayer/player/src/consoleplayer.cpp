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

#include "consoleplayer.h"

#include <e32base.h>
#include <e32debug.h>
#include <f32file.h>
#include <bldvariant.hrh>

#include "MmfPlayerWindow.h"
#include "ExternalPlayer.h"

#ifdef FF_OPENMAX_AL
#include "opmaxplayerwindow.h"
#endif

const TInt KFontSize = 15;

const TInt KOrdinalPositionConfigurationCount = 7;
const TInt KPlayerCount = 5;

enum TPlayerNames
    {
    EVideoPlayer,
    EVideoPlayer2,
    EVideoPlayer3,
    EExternalPlayer,
    EExternalPlayer2
    };

// entries are sorted with foreground windows first
const TPlayerNames KOrdinalPositions[KOrdinalPositionConfigurationCount][KPlayerCount] =
    {
        {EVideoPlayer3, EVideoPlayer2, EExternalPlayer, EExternalPlayer2, EVideoPlayer},
        {EVideoPlayer2, EVideoPlayer3, EExternalPlayer2, EExternalPlayer, EVideoPlayer},
        {EExternalPlayer, EExternalPlayer2, EVideoPlayer3, EVideoPlayer2, EVideoPlayer},
        {EVideoPlayer3, EExternalPlayer2, EVideoPlayer2, EExternalPlayer, EVideoPlayer},
        {EVideoPlayer3, EVideoPlayer2, EVideoPlayer, EExternalPlayer, EExternalPlayer2},
        {EExternalPlayer, EExternalPlayer2, EVideoPlayer, EVideoPlayer3, EVideoPlayer2},
        {EVideoPlayer, EVideoPlayer3, EVideoPlayer2, EExternalPlayer, EExternalPlayer2}
    };

// Key map
// This section maps the options to keys for each page of options.

const TInt KKeyMapPageCount = 9;

const TInt KWindow2Page = 5;
const TInt KWindow3Page = 6; 
const TInt KExternalPlayerPage = 7;
const TInt KExternalPlayer2Page = 8;
const TInt KAllPlayersPage = 9;

const TOperationsPage KKeyMap[KKeyMapPageCount] =
    {        
        {STR("Standard controls"),
         5, // default softkey index
            {
                // Enter
                {STR("Pause/Resume"),                EOperation_PauseResume}, 
                // Up / Down / Left / Right
                {STR("Seek back 5 seconds"),         EOperation_SeekBack},
                {STR("Seek forward 5 seconds"),      EOperation_SeekForward},
                {STR("Seek to start of clip"),       EOperation_SeekStart},
                {STR("Seek to end minus 5 secs"),    EOperation_SeekEnd},
                // 0 - 9
                {STR("Help text on/off"),            KOperation_ToggleHelpVisibility},  
                {STR("Go to next options page"),     KOperation_NextOptionPage},  
                {STR("Stop/Play"),                   EOperation_StopPlay},  
                {STR("Obscure entire screen"),       EOperation_ObscureWindow},
                {STR("Toggle help transparency"),    KOperation_ToggleHelpTransparency},  
                {STR("Change video window size"),    EOperation_ChangeWindowSize},
                {STR("Panic this process"),          EOperation_AbnormalTermination},  
                {STR(""),                            KOperation_None},  
                {STR(""),                            KOperation_None},  
                {STR(""),                            KOperation_None},  
                // *, #
                {STR("Change display mode"),         EOperation_ChangeDisplayMode},  
                {STR("Exit"),                        KOperation_Exit}                
            }
        },
        {STR("Video controls"),
         7, // default softkey index
            {
                // Enter
                {STR("Pause/Resume"),                EOperation_PauseResume},
                // Up / Down / Left / Right
                {STR("Increase height scale"),       EOperation_IncreaseHeightScale},
                {STR("Decrease height scale"),       EOperation_DecreaseHeightScale},
                {STR("Increase width scale"),        EOperation_IncreaseWidthScale},
                {STR("Decrease width scale"),        EOperation_DecreaseWidthScale},
                // 0 - 9
                {STR("Help text on/off"),            KOperation_ToggleHelpVisibility},
                {STR("Go to next options page"),     KOperation_NextOptionPage},  
                {STR("Change rotation angle"),       EOperation_Rotate},
                {STR("Change autoscale mode"),       EOperation_Autoscale},
                {STR("Change clipping rectangle"),   EOperation_ClipRectangle},
                {STR("Change extent rectangle"),     EOperation_Extent},
                {STR(""),                            KOperation_None},
                {STR(""),                            KOperation_None},  
                {STR(""),                            KOperation_None},  
                {STR("Reset to defaults"),           EOperation_ResetToDefaults},
                // *, #
                {STR("Change display mode"),         EOperation_ChangeDisplayMode},
                {STR("Exit"),                        KOperation_Exit}
            }
        },
        {STR("Trick play"),
         5, // default softkey index
            {   
                // Enter
                {STR("Pause/Resume"),                EOperation_PauseResume}, 
                // Up / Down / Left / Right
                {STR("Frame step forward"),          EOperation_FrameStepForward}, 
                {STR("Frame step backward"),         EOperation_FrameStepBackward},  
                {STR("Decrease play velocity"),      EOperation_DecreasePlayVelocity},
                {STR("Increase play velocity"),      EOperation_IncreasePlayVelocity},
                // 0 - 9
                {STR("Help text on/off"),            KOperation_ToggleHelpVisibility},
                {STR("Go to next options page"),     KOperation_NextOptionPage},  
                {STR(""),                            KOperation_None},
                {STR(""),                            KOperation_None},
                {STR(""),                            KOperation_None},
                {STR(""),                            KOperation_None},
                {STR(""),                            KOperation_None},
                {STR(""),                            KOperation_None},  
                {STR(""),                            KOperation_None},  
                {STR("Reset to defaults"),           EOperation_ResetToDefaults},
                // *, #
                {STR("Change display mode"),         EOperation_ChangeDisplayMode},
                {STR("Exit"),                        KOperation_Exit}
            } 
        },
        {STR("Audio controls"),
         0, // default softkey index
            {
                // Enter
                {STR("Pause/Resume"),                EOperation_PauseResume},
                // Up / Down / Left / Right
                {STR("Increase volume 10%"),         EOperation_IncreaseVolume},
                {STR("Decrease volume 10%"),         EOperation_DecreaseVolume},
                {STR("Balance left"),                EOperation_BalanceLeft},
                {STR("Balance right"),               EOperation_BalanceRight},
                // 0 - 9
                {STR("Help text on/off"),            KOperation_ToggleHelpVisibility},
                {STR("Go to next options page"),     KOperation_NextOptionPage},  
                {STR("Maximum volume"),              EOperation_MaximumVolume},
                {STR("Mute"),                        EOperation_Mute},
                {STR("Balance maximum left"),        EOperation_BalanceMaxLeft},
                {STR("Balance maximum right"),       EOperation_BalanceMaxRight},
                {STR(""),                            KOperation_None},
                {STR(""),                            KOperation_None},  
                {STR(""),                            KOperation_None},  
                {STR("Reset to defaults"),           EOperation_ResetToDefaults},
                // *, #
                {STR("Change display mode"),         EOperation_ChangeDisplayMode},
                {STR("Exit"),                        KOperation_Exit}
            }
        },        
        {STR("Video window #2 (same process)"),
         7, // default softkey index
            {   
                // Enter
                {STR("Pause/Resume"),                EOperation_PauseResume},
                // Up / Down / Left / Right
                {STR("Move window up"),              EOperation_MoveWindowUp},
                {STR("Move window down"),            EOperation_MoveWindowDown},
                {STR("Move window left"),            EOperation_MoveWindowLeft},
                {STR("Move window right"),           EOperation_MoveWindowRight},
                // 0 - 9
                {STR("Help text on/off"),            KOperation_ToggleHelpVisibility},
                {STR("Go to next options page"),     KOperation_NextOptionPage},  
                {STR("Toggle window on/off"),        EOperation_ToggleSecondVideo},
                {STR("Drag window"),                 EOperation_DragWindow},
                {STR("Toggle window transparency"),  EOperation_ToggleWindowTransparency},
                {STR("Change video window size"),    EOperation_ChangeWindowSize},
                {STR("Change autoscale mode"),       EOperation_Autoscale},
                {STR(""),                            KOperation_None},
                {STR(""),                            KOperation_None},  
                {STR("Reset to defaults"),           EOperation_ResetToDefaults},
                // *, #
                {STR("Change display mode"),         EOperation_ChangeDisplayMode},
                {STR("Exit"),                        KOperation_Exit}
            }
        },
        {STR("Video window #3 (same process)"),
         7, // default softkey index
            {   
                // Enter
                {STR("Pause/Resume"),                EOperation_PauseResume},
                // Up / Down / Left / Right
                {STR("Move window up"),              EOperation_MoveWindowUp},
                {STR("Move window down"),            EOperation_MoveWindowDown},
                {STR("Move window left"),            EOperation_MoveWindowLeft},
                {STR("Move window right"),           EOperation_MoveWindowRight},
                // 0 - 9
                {STR("Help text on/off"),            KOperation_ToggleHelpVisibility},
                {STR("Go to next options page"),     KOperation_NextOptionPage},  
                {STR("Toggle window on/off"),        EOperation_ToggleThirdVideo},
                {STR("Drag window"),                 EOperation_DragWindow},
                {STR("Toggle window transparency"),  EOperation_ToggleWindowTransparency},
                {STR("Change video window size"),    EOperation_ChangeWindowSize},
                {STR("Change autoscale mode"),       EOperation_Autoscale},
                {STR(""),                            KOperation_None},
                {STR(""),                            KOperation_None},  
                {STR("Reset to defaults"),           EOperation_ResetToDefaults},
                // *, #
                {STR("Change display mode"),         EOperation_ChangeDisplayMode},
                {STR("Exit"),                        KOperation_Exit}
            }
        },
        {STR("Video window #4 (another process)"),
         7, // default softkey index
            {   
                // Enter
                {STR("Pause/Resume"),                EOperation_PauseResume},
                // Up / Down / Left / Right
                {STR("Move window up"),              EOperation_MoveWindowUp},
                {STR("Move window down"),            EOperation_MoveWindowDown},
                {STR("Move window left"),            EOperation_MoveWindowLeft},
                {STR("Move window right"),           EOperation_MoveWindowRight},
                // 0 - 9
                {STR("Help text on/off"),            KOperation_ToggleHelpVisibility},
                {STR("Go to next options page"),     KOperation_NextOptionPage},  
                {STR("Toggle window on/off"),        EOperation_ToggleExternalPlayer},
                {STR("Drag window"),                 EOperation_DragWindow},
                {STR("Toggle window transparency"),  EOperation_ToggleWindowTransparency},
                {STR("Change video window size"),    EOperation_ChangeWindowSize},
                {STR("Change autoscale mode"),       EOperation_Autoscale},
                {STR(""),                            KOperation_None},
                {STR(""),                            KOperation_None},  
                {STR("Reset to defaults"),           EOperation_ResetToDefaults},
                // *, #
                {STR("Change display mode"),         EOperation_ChangeDisplayMode},
                {STR("Exit"),                        KOperation_Exit}
            } 
        },
        {STR("Video window #5 (another process)"),
         7, // default softkey index
            {   
                // Enter
                {STR("Pause/Resume"),                EOperation_PauseResume},
                // Up / Down / Left / Right
                {STR("Move window up"),              EOperation_MoveWindowUp},
                {STR("Move window down"),            EOperation_MoveWindowDown},
                {STR("Move window left"),            EOperation_MoveWindowLeft},
                {STR("Move window right"),           EOperation_MoveWindowRight},
                // 0 - 9
                {STR("Help text on/off"),            KOperation_ToggleHelpVisibility},
                {STR("Go to next options page"),     KOperation_NextOptionPage},  
                {STR("Toggle window on/off"),        EOperation_ToggleExternalPlayer2},
                {STR("Drag window"),                 EOperation_DragWindow},
                {STR("Toggle window transparency"),  EOperation_ToggleWindowTransparency},
                {STR("Change video window size"),    EOperation_ChangeWindowSize},
                {STR("Change autoscale mode"),       EOperation_Autoscale},
                {STR(""),                            KOperation_None},
                {STR(""),                            KOperation_None},  
                {STR("Reset to defaults"),           EOperation_ResetToDefaults},
                // *, #
                {STR("Change display mode"),         EOperation_ChangeDisplayMode},
                {STR("Exit"),                        KOperation_Exit}
            } 
        },
        {STR("Control all windows"),
         7, // default softkey index
            {   
                // Enter
                {STR("Pause/Resume"),                EOperation_PauseResume},
                // Up / Down / Left / Right
                {STR("Seek back 5 seconds"),         EOperation_SeekBack},
                {STR("Seek forward 5 seconds"),      EOperation_SeekForward},
                {STR("Seek to start of clip"),       EOperation_SeekStart},
                {STR("Seek to end minus 5 secs"),    EOperation_SeekEnd},
                // 0 - 9
                {STR("Help text on/off"),            KOperation_ToggleHelpVisibility},
                {STR("Go to next options page"),     KOperation_NextOptionPage},  
                {STR("Create all windows"),          EOperation_LaunchAllWindows},
                {STR("Toggle ordinal positions"),    EOperation_ToggleOrdinalPositions},
                {STR("Pause all"),                   EOperation_Pause},  
                {STR("Resume all"),                  EOperation_Resume},  
                {STR("Stop/Play all"),               EOperation_StopPlay},  
                {STR(""),                            KOperation_None},  
                {STR(""),                            KOperation_None},  
                {STR("Reset to defaults"),           EOperation_ResetToDefaults},
                // *, #
                {STR("Change display mode"),         EOperation_ChangeDisplayMode},
                {STR("Exit"),                        KOperation_Exit}
            } 
        }
    };


// Console player implementation

void CConsolePlayer::ExecuteL()
    {
    CConsolePlayer* self = new(ELeave) CConsolePlayer;
    CleanupStack::PushL( self );
    self->InitL();
    self->Main();
    CleanupStack::PopAndDestroy( self );    
    }
    
CConsolePlayer::CConsolePlayer() : 
    CTestAppBase( KFontSize ),
    iLastError( KErrNone ),
    iVideoWindowTextMode( 1 ),
    iFinalResult( 0 )
    {    
    }
    
CConsolePlayer::~CConsolePlayer()
    {
    Cancel();
    
    if( iGc && iFullScreenWindow )
    {
        iGc->Activate(*iFullScreenWindow);
        
        iFullScreenWindow->Invalidate();
        iFullScreenWindow->BeginRedraw();
        
        iGc->Reset();
        
        iGc->UseFont(iFont);                
        iGc->SetBrushColor(KRgbDarkBlue);
        
        iGc->Clear();

        if( iFinalResult == KErrNone )
            {
            // KRgbWhite seems to be having problems (0xffffff) in some emulators,
            // but 0xfefefe is working, so use that instead of white.        
            iGc->SetPenColor(0xfefefe);                 
            }
        else
            {
            iGc->SetPenColor(KRgbRed);
            }
    
        iBuffer.Format( _L("CONSOLEPLAYER EXIT, result = %i"), iFinalResult );
        iGc->DrawText( iBuffer, TPoint(5, 20) );
       
        iFullScreenWindow->EndRedraw();
        
        iGc->Deactivate();
        
        iFullScreenWindow->SetVisible( ETrue );
        
        WaitForAnyKey();
    }    
    
    delete iExternalPlayer;
    delete iExternalPlayer2;
    delete iVideoPlayer;        
    delete iVideoPlayer2;
    delete iVideoPlayer3;
    delete iTimerCallback;
    delete iFullScreenWindow;
    }
    
void CConsolePlayer::InitL()
    {
    BaseConstructL( KKeyMap, KKeyMapPageCount );
    
    iFullScreenWindow = new(ELeave) RWindow( iWs );
    User::LeaveIfError( iFullScreenWindow->Construct( *iWindowGroup, KNullWsHandle ) );
    iFullScreenWindow->SetExtent( TPoint(0,0), iDisplaySize );
    iFullScreenWindow->SetNonTransparent();
    iFullScreenWindow->SetBackgroundColor(KRgbBlack);
    iFullScreenWindow->SetVisible(false);
    iFullScreenWindow->Activate();
        
    iGc->Activate(*iFullScreenWindow);

    iFullScreenWindow->BeginRedraw();
    
    iGc->Reset();    
    iGc->UseFont(iFont);                
    iGc->SetBrushStyle(CWindowGc::ESolidBrush);
    iGc->SetBrushColor(KRgbBlack);    
    iGc->Clear();
    
    iFullScreenWindow->EndRedraw();
    
    iGc->Deactivate();    
    }

void CConsolePlayer::Main()
    {    
    TRAPD( err, MainL() ); 
    
    RDebug::Printf( "CONSOLEPLAYER Completed, err=%i", err );    
    }
    
void CConsolePlayer::DoSelectAction( TUint aActionMask, 
                                     bool aPresentPerformanceModeOption, 
                                     TActionToPerform& aAction,
                                     bool& aPerformanceMode )
    {
    RPointerArray<TDesC> operations;

    TInt sameAsMainPlayerIndex = -2;
    TInt playLocalFileIndex = -2;
    TInt playLocalFilePerformanceIndex = -2;
    TInt playUrlIndex = -2;
    TInt playUrlPerformanceIndex = -2;
    
    if( aActionMask & EAction_SameAsMainPlayer )
        {
        _LIT( KSameAsMainPlayer, "Same as main player" );
        sameAsMainPlayerIndex = operations.Count();
        operations.Append( &KSameAsMainPlayer );
        }
    
    if( aActionMask & EAction_PlayLocalFile )
        {
        _LIT( KPlayLocalFile, "Play local file" );
        playLocalFileIndex = operations.Count();
        operations.Append( &KPlayLocalFile );
        
        if( aPresentPerformanceModeOption )
            {
            _LIT( KPlayLocalFilePerformance, "Play local file (performance mode)" );
            playLocalFilePerformanceIndex = operations.Count();
            operations.Append( &KPlayLocalFilePerformance );
            }
        }
    
    if( aActionMask & EAction_PlayUrl )
        {
        _LIT( KPlayUrl, "Play URL from c:\\data\\urls.txt" );
        playUrlIndex = operations.Count();
        operations.Append( &KPlayUrl );
        
        if( aPresentPerformanceModeOption )
            {
            _LIT( KPlayUrlPerformance, "Play URL from c:\\data\\urls.txt (performance mode)" );
            playUrlPerformanceIndex = operations.Count();
            operations.Append( &KPlayUrlPerformance );
            }
        }
    
    _LIT( KExit, "Exit" );
    operations.Append( &KExit );

    TInt index = 0;
    
    if( operations.Count() > 1 )
        {
        index = SelectFromListL( TPoint(0,0), iDisplaySize, _L("Select operation to perform:"), operations );
        }
    
    if( index == sameAsMainPlayerIndex )
        {
        aAction = EAction_SameAsMainPlayer;
        }
    else if( index == playLocalFileIndex )
        {
        aAction = EAction_PlayLocalFile;
        aPerformanceMode = false;
        }
    else if( index == playLocalFilePerformanceIndex )
        {    
        aAction = EAction_PlayLocalFile;
        aPerformanceMode = true;
        }
    else if( index == playUrlIndex )
        {    
        aAction = EAction_PlayUrl;
        aPerformanceMode = false;
        }
    else if( index == playUrlPerformanceIndex )
        {    
        aAction = EAction_PlayUrl;
        aPerformanceMode = true;
        }
    else
        {
        aAction = EAction_None;
        }
    
    operations.Close();
    }

bool CConsolePlayer::DoSelectApi( TUint aApiMask, 
                                  TApiToUse& aApi )
    {
    RPointerArray<TDesC> apiList;

    TInt videoPlayerIndex = -2;
    TInt omxAlIndex = -2;
    
    if( aApiMask & EApi_VideoPlayer )
        {
        _LIT( KVideoPlayer, "Video player" );
        videoPlayerIndex = apiList.Count();
        apiList.Append( &KVideoPlayer );
        }
    
    if( aApiMask & EApi_OpenMaxAl )
        {
        _LIT( KOmxAl, "OpenMAX AL" );
        omxAlIndex = apiList.Count();
        apiList.Append( &KOmxAl );
        }
    
    TInt index = 0;
    
    if( apiList.Count() > 1 )
        {
        index = SelectFromListL( TPoint(0,0), iDisplaySize, _L("Select API to use:"), apiList );
        }

    bool selected = true;
    
    if( index == videoPlayerIndex )
        {
        aApi = EApi_VideoPlayer;
        }
    else if( index == omxAlIndex )
        {
        aApi = EApi_OpenMaxAl;
        }
    else
        {
        selected = false;
        }
    
    apiList.Close();
    
    return selected;
    }

void CConsolePlayer::SelectAction( TUint aActionMask,
                                   TUint aApiMask, 
                                   bool aPresentPerformanceModeOption,
                                   TActionToPerform& aAction, 
                                   TApiToUse& aApi, 
                                   bool& aPerformanceMode,
                                   TFileName& aLocation )
    {
    bool done = false;
    
    while( !done )
        {
        DoSelectAction( aActionMask, aPresentPerformanceModeOption, aAction, aPerformanceMode );
    
        switch( aAction )
            {
            case EAction_None:
            case EAction_SameAsMainPlayer:
                
                done = true;
                break;
                
            case EAction_PlayLocalFile:
                
                while( !done && DoSelectApi( aApiMask, aApi ) )
                    {
                    done = SelectFileWithHistoryL( TPoint(0,0), iDisplaySize, aLocation, KHistoryFilename, KMaxHistoryEntries );
                    }
                break;
                
            case EAction_PlayUrl:
                
                while( !done && DoSelectApi( aApiMask, aApi ) )
                    {
                    done = SelectUrlL( aLocation );
                    }                
                break;
            }        
        }    
    }

void CConsolePlayer::MainL()        
    {
    bool performanceMode;
    TFileName location;
    
    TUint actionMask = (EAction_PlayLocalFile | EAction_PlayUrl);  
            
    TUint apiMask = EApi_VideoPlayer;

#ifdef FF_OPENMAX_AL
    apiMask |= EApi_OpenMaxAl; 
#endif
    
    bool done = false;
    
    while( !done )
        {    
        SelectAction( actionMask, apiMask, true, iMainPlayerAction, iMainPlayerApi, performanceMode, location );
    
        switch( iMainPlayerAction )
            {
            case EAction_None:
    
                // Do nothing.  Exit program.
                done = true;
                break;
                
            case EAction_PlayLocalFile:
            case EAction_PlayUrl:
                {
                CPlayerWindow::TAutoscaleMode autoscale = CPlayerWindow::EDoNotAutoscale;
                
                if( performanceMode )
                    {
                    // turn off text
                    iVideoWindowTextMode = 0;
                    
                    // stretch video to full window
                    autoscale = CPlayerWindow::EStretch;
                    
                    // do not use timer in performance mode
                    }
                else
                    {    
                    iTimerCallback = new(ELeave) CTimerCallback( *this );    
                    }
                
                // The first video player window is full screen. 
                iVideoPlayer = PlayerWindowNewL( iMainPlayerApi,
                                                 TPoint(0,0), 
                                                 iDisplaySize, 
                                                 false,  // not transparent 
                                                 KRgbDarkBlue,
                                                 true ); 
                        
                iActivePlayer = iVideoPlayer;

                if( iMainPlayerAction == EAction_PlayLocalFile )
                    {
                    // RDebug::Printf only supports 8-bit strings, but RawPrint can handle 16-bit descriptors.
                    RDebug::Printf( "CONSOLEPLAYER file:" );
                    RDebug::RawPrint( location );
                
                    iVideoPlayer->StartWithFilenameL(location, autoscale, this);
                    }
                else
                    {
                    // RDebug::Printf only supports 8-bit strings, but RawPrint can handle 16-bit descriptors.
                    RDebug::Printf( "CONSOLEPLAYER URL:" );
                    RDebug::RawPrint( location );
                            
                    iVideoPlayer->StartWithUrlL(location, autoscale, this);
                    }
                
                done = true;
                CActiveScheduler::Start();
                }
                break;
                
            }
        }
    }
  
bool CConsolePlayer::SelectUrlL( TDes& aUrl )
    {    
    RPointerArray<HBufC> urlList;
    TInt err = ReadFile( KUrlListFilename, urlList );

    bool selected = false; 
    
    if( err == KErrNone )
        {
        TInt index = SelectFromListL( TPoint(0,0), iDisplaySize, _L("Select URL:"), urlList );
        
        if( index >= 0 )
            {
            selected = true;
            aUrl.Copy( urlList[index]->Des() );
            }
        }
    
    urlList.ResetAndDestroy();
    
    return selected;
    }

MPlayerCommands* CConsolePlayer::CurrentPlayer()
    {
    MPlayerCommands* player = NULL;
    
    switch( CurrentPageNumber() )
        {
        case KWindow2Page:
            player = iVideoPlayer2;
            break;
        case KWindow3Page:
            player = iVideoPlayer3;
            break;
        case KExternalPlayerPage:
            player = iExternalPlayer;
            break;
        case KExternalPlayer2Page:
            player = iExternalPlayer2;
            break;
        case KAllPlayersPage:
            player = this;  // means to route command to all players
            break;
        default:
            // By default, route other commands to the main player.
            player = iVideoPlayer;
            break;
        }
    
    return player;
    }
        
void CConsolePlayer::SoftkeyFunctionUpdated()
    {
    MPlayerCommands* currentPlayer = CurrentPlayer();

    if( currentPlayer != iActivePlayer )
        {
        if( iActivePlayer != NULL )
            {
            iActivePlayer->ExecuteOperation( EOperation_ClearBorderAroundWindow );
            }
    
        iActivePlayer = currentPlayer;
        
        DrawBorderAroundActiveWindows();
        }
    
    UpdateVideoWindow();    
    }

void CConsolePlayer::DrawBorderAroundActiveWindows()
    {    
    if( (iActivePlayer != NULL) && (iActivePlayer != iVideoPlayer) )
        {               
        iActivePlayer->ExecuteOperation( EOperation_DrawWhiteBorderAroundWindow );
        }    
    }

bool CConsolePlayer::ConsumeKeyEvent( TInt /*aKeyCode*/ )
    {
    return false;    
    }

void CConsolePlayer::UpdateVideoWindow()
    {
    iGc->Activate(iVideoPlayer->Window());

    iVideoPlayer->Window().Invalidate();
    iVideoPlayer->Window().BeginRedraw();
    
    iGc->Reset();
    iGc->SetBrushColor(KRgbTransparent);
    iGc->Clear();

    iGc->SetPenColor(KRgbCyan);

    if( iVideoWindowTextMode > 0 )
        {
        iGc->UseFont(iFont);                

        if( iSelectedOptionText.Length() > 0 )
            {
            // POPUP WINDOW (AFTER OPTION IS SELECTED)

            iGc->SetBrushStyle(CWindowGc::ESolidBrush);
            iGc->SetBrushColor(KRgbBlue);
            
            const TInt KSelectionWindowWidth = iFont->TextWidthInPixels(iSelectedOptionText)+10;
            const TInt KSelectionWindowHeight = KFontSize+10;
            const TInt KOffsetFromBottom = 20;

            TRect box(iDisplaySize.iWidth/2-KSelectionWindowWidth/2, 
                      iDisplaySize.iHeight-KOffsetFromBottom-KSelectionWindowHeight,
                      iDisplaySize.iWidth/2+KSelectionWindowWidth/2, 
                      iDisplaySize.iHeight-KOffsetFromBottom);
            iGc->DrawText( iSelectedOptionText, box, KFontSize+2, CWindowGc::ECenter);

            iGc->SetBrushStyle(CWindowGc::ENullBrush);
            iGc->SetBrushColor(KRgbTransparent);
            }
        
        // BASIC TEXT

        TInt posInSec = iVideoPlayer->PositionInMs() / 1000;
       
        TInt durationInSec = iVideoPlayer->DurationInMs() / 1000;
        
        const TInt KColumn = 3;
        const TInt KRowIncrement = KFontSize + 1;
        TInt row = KRowIncrement;

        iBuffer.Format( _L("Options page %i: "), CurrentPageNumber() );
        iBuffer.Append( CurrentPageName() );
        iGc->DrawText( iBuffer, TPoint(KColumn, row) );
        row += KRowIncrement;
        
        iBuffer.Copy( _L("Softkey: ") );
        iBuffer.Append( CurrentSoftkeyName() );
        iGc->DrawText( iBuffer, TPoint(KColumn, row) );
        row += KRowIncrement;
        
        iBuffer.Format( _L("Position: %i/%i secs "), posInSec, durationInSec );
        switch( iVideoPlayer->PlayState() )
            {
            case CPlayerWindow::EPlaying:
                iBuffer.Append( _L("<playing>") );
                break;
            case CPlayerWindow::EPaused:
                iBuffer.Append( _L("<paused>") );
                break;
            case CPlayerWindow::EStopped:
                iBuffer.Append( _L("<stopped>") );
                break;
            }
        iGc->DrawText( iBuffer, TPoint(KColumn, row) );
        row += KRowIncrement;
        
        if( iLastError != KErrNone )
            {
            iGc->SetPenColor(KRgbRed);        
            }
        
        iBuffer.Format ( _L("Last error: %i"), iLastError );
        iGc->DrawText( iBuffer, TPoint(KColumn, row) );
        row += KRowIncrement;

        iGc->SetPenColor(KRgbCyan);
        
        RDebug::Printf( "CONSOLEPLAYER position=%i", posInSec );

        
        if( iVideoWindowTextMode > 1 )
            {
            // ADVANCED TEXT

            // skip a row
            row += KRowIncrement;

            iGc->DrawText( iVideoPlayer->Filename(), TPoint(KColumn, row) );
            row += KRowIncrement;
            
            iBuffer.Format( _L("Display size: %ix%i"), iDisplaySize.iWidth, iDisplaySize.iHeight );
            iGc->DrawText( iBuffer, TPoint(KColumn, row) );
            row += KRowIncrement;
            
            iBuffer.Format( _L("Video size: %ix%i"), iVideoPlayer->VideoSize().iWidth, iVideoPlayer->VideoSize().iHeight );
            iGc->DrawText( iBuffer, TPoint(KColumn, row) );
            row += KRowIncrement;            
            
            iVideoPlayer->WriteAdvancedText( *iGc, KColumn, row, KRowIncrement );        
            }
        }
            
    iVideoPlayer->Window().EndRedraw();

    iGc->Deactivate();

    iWs.Flush();
    }

void CConsolePlayer::UpdateWindowOrdinalPositions()
    {    
    TInt nextOrdinal = 0;
    
    iSelectionWindow->SetOrdinalPosition( nextOrdinal );
    nextOrdinal++;
    
    iHelpWindow->SetOrdinalPosition( nextOrdinal );
    nextOrdinal++;

    iFullScreenWindow->SetOrdinalPosition( nextOrdinal );
    nextOrdinal++;

    for( TInt playerIndex = 0; playerIndex < KPlayerCount; playerIndex++ )
        {
        MPlayerCommands* player = NULL;
        
        switch( KOrdinalPositions[iOrdinalIndex][playerIndex] )
            {
            case EVideoPlayer:
                player = iVideoPlayer;
                break;
            case EVideoPlayer2:
                player = iVideoPlayer2;
                break;
            case EVideoPlayer3:
                player = iVideoPlayer3;
                break;
            case EExternalPlayer:
                player = iExternalPlayer;
                break;
            case EExternalPlayer2:
                player = iExternalPlayer2;
                break;
            }
    
        if( player )
            {
            player->SetOrdinalPosition( nextOrdinal );
            nextOrdinal++;
            }
        }
    }

CPlayerWindow* CConsolePlayer::PlayerWindowNewL( TApiToUse aApi,
                                                 TPoint aTopLeft,
                                                 TSize aSize,
                                                 bool aTransparent,
                                                 TRgb aBackgroundColor,
                                                 bool aSuppressGraphicsContext )
    {
    CPlayerWindow* player = NULL;
    
    switch( aApi )
        {
        case EApi_VideoPlayer:
            
            player = CMmfPlayerWindow::NewL( iFs, 
                                             iWs, 
                                             *iScreenDevice, 
                                             *iWindowGroup,
                                             aTopLeft, 
                                             aSize, 
                                             aTransparent, 
                                             aBackgroundColor,
                                             aSuppressGraphicsContext );
            break;
            
        case EApi_OpenMaxAl: 
#ifdef FF_OPENMAX_AL
        
            player = COpMaxPlayerWindow::NewL( iFs, 
                                               iWs, 
                                               *iScreenDevice, 
                                               *iWindowGroup, 
                                               aTopLeft, 
                                               aSize, 
                                               aTransparent, 
                                               aBackgroundColor,
                                               aSuppressGraphicsContext);
#endif            
            break;            
        }
    
    return player;
    }

void CConsolePlayer::EnableDragging()
    {
    iPlayerBeingDragged = CurrentPlayer();
    
    if( iPlayerBeingDragged )
        {
        iPlayerBeingDragged->ExecuteOperation( EOperation_DrawRedBorderAroundWindow );
        StartReceivingPointerEvents();    
        }
    }

void CConsolePlayer::DisableDragging()
    {
    if( iPlayerBeingDragged )
        {
        // Stop routing pointer events to app.
        StopReceivingPointerEvents(); 
        
        if( iPlayerBeingDragged == CurrentPlayer() )
            {
            iPlayerBeingDragged->ExecuteOperation( EOperation_DrawWhiteBorderAroundWindow );
            }
        else
            {
            iPlayerBeingDragged->ExecuteOperation( EOperation_ClearBorderAroundWindow );
            }
        
        iPlayerBeingDragged = NULL;
        }
    }

TInt CConsolePlayer::CreateAnotherPlayer( CPlayerWindow** aPlayerWindow, 
                                          CExternalPlayer** aExternalPlayer,
                                          const TDesC& aExternalPlayerExeName,
                                          TSize aSize, 
                                          TPoint aTopLeft )
    {
    TInt err = KErrNone;

    bool resumePlayer1 = false;
    bool resumePlayer2 = false;
    bool resumePlayer3 = false;
    bool resumeExternalPlayer1 = false;
    bool resumeExternalPlayer2 = false;
    
    if( iVideoPlayer->PlayState() == CPlayerWindow::EPlaying )
        {
        iVideoPlayer->Pause();
        resumePlayer1 = true;
        }

    if( iVideoPlayer2 && iVideoPlayer2->PlayState() == CPlayerWindow::EPlaying )
        {
        iVideoPlayer2->Pause();
        resumePlayer2 = true;
        }
    
    if( iVideoPlayer3 && iVideoPlayer3->PlayState() == CPlayerWindow::EPlaying  )
        {
        iVideoPlayer3->Pause();
        resumePlayer3 = true;
        }
    
    if( iExternalPlayer )
        {
        iExternalPlayer->ExecuteOperation( EOperation_Pause );
        resumeExternalPlayer1 = true;
        }
    
    if( iExternalPlayer2 )
        {
        iExternalPlayer2->ExecuteOperation( EOperation_Pause );
        resumeExternalPlayer2 = true;
        }
    
    TActionToPerform action;
    TApiToUse api;
    bool dummy;
    TFileName location;
        
    TUint actionMask = (EAction_PlayLocalFile | EAction_PlayUrl | EAction_SameAsMainPlayer );  

    TUint apiMask = EApi_VideoPlayer;

    SelectAction( actionMask, apiMask, false, action, api, dummy, location );

    if( action == EAction_SameAsMainPlayer )
        {
        action = iMainPlayerAction;
        api = iMainPlayerApi;
        
        if( action == EAction_PlayLocalFile )
            {
            location = iVideoPlayer->FullFilename();
            }
        else
            {
            location = iVideoPlayer->Url();            
            }
        }            
    
    if( location.Length() > 0 )
        {
        if( aPlayerWindow != NULL )
            {
            // Create player as another window within this process.
            TRAP( err, (*aPlayerWindow) = PlayerWindowNewL( api,
                                                            aTopLeft, 
                                                            aSize, 
                                                            true,  // transparent 
                                                            KRgbTransparent,
                                                            false ) );            
            if( (*aPlayerWindow ) != NULL )
                {
                if( action == EAction_PlayLocalFile )
                    {
                    TRAP( err, (*aPlayerWindow)->StartWithFilenameL( location, CPlayerWindow::EBestFit) );            
                    }
                else
                    {
                    TRAP( err, (*aPlayerWindow)->StartWithUrlL( location, CPlayerWindow::EBestFit) );                        
                    }
    
                if( err != KErrNone )
                    {
                    delete (*aPlayerWindow);
                    aPlayerWindow = NULL;
                    }
                }
            }
        else
            {
            // Launch player within another process.
            (*aExternalPlayer) = new CExternalPlayer( iWs );
            
            if( (*aExternalPlayer) == NULL )
                {
                err = KErrNoMemory;
                }
            else if( action == EAction_PlayLocalFile )
                {            
                err = (*aExternalPlayer)->Launch( aExternalPlayerExeName,
                                                  *iWindowGroup,
                                                  location,
                                                  true,
                                                  aTopLeft,
                                                  aSize );
                }
            else
                {
                err = (*aExternalPlayer)->Launch( aExternalPlayerExeName,
                                                  *iWindowGroup,
                                                  location,
                                                  false,
                                                  aTopLeft,
                                                  aSize );
                }
            }
        
        UpdateWindowOrdinalPositions();        
        }
    
    if( resumePlayer1 )
        {
        iVideoPlayer->Play();
        }
    
    if( resumePlayer2 )
        {
        iVideoPlayer2->Play();
        }
    
    if( resumePlayer3 )
        {
        iVideoPlayer3->Play();
        }

    if( resumeExternalPlayer1 )
        {
        iExternalPlayer->ExecuteOperation( EOperation_Resume );
        }
    
    if( resumeExternalPlayer2 )
        {
        iExternalPlayer2->ExecuteOperation( EOperation_Resume );
        }
    
    return err;
    }

TInt CConsolePlayer::ExecuteOperation( TInt aOperation )
    {
    SendOperationToAllPlayers( aOperation );
    return KErrNone;
    }

void CConsolePlayer::MoveWindow( TPoint /*aNewCenter*/ )    
    {
    // Not used.
    }

void CConsolePlayer::SetOrdinalPosition( TInt /*aPosition*/ )
    {
    // Not used.
    }

void CConsolePlayer::SendOperationToAllPlayers( TInt aOperation )
    {
    iVideoPlayer->ExecuteOperation(aOperation);            
    if( iVideoPlayer2 )
        {
        iVideoPlayer2->ExecuteOperation(aOperation);            
        }            
    if( iVideoPlayer3 )
        {
        iVideoPlayer3->ExecuteOperation(aOperation);            
        }            
    if( iExternalPlayer )
        {
        iExternalPlayer->ExecuteOperation(aOperation);            
        }            
    if( iExternalPlayer2 )
        {
        iExternalPlayer2->ExecuteOperation(aOperation);            
        }
    }

void CConsolePlayer::ExecuteOperation( TInt aOperation, const TDesC& aOperationText )
    {
    iSelectedOptionText.Copy( aOperationText );                    
    iSelectedOptionTextCounter = 2;

    RDebug::Printf( "CONSOLE PLAYER executing operation:" );
    RDebug::RawPrint( iSelectedOptionText );

    if( iPlayerBeingDragged != NULL )
        {
        DisableDragging();
        }
    
    iLastError = KErrNone;
    
    switch( aOperation )
        {
        case EOperation_ChangeDisplayMode:
            {                
            // Change video window text mode.
            iVideoWindowTextMode = (iVideoWindowTextMode+1) % 3;                    
            break;
            }
        case EOperation_ToggleSecondVideo:
            {
            if( iVideoPlayer2 )
                {
                delete iVideoPlayer2;
                iVideoPlayer2 = NULL;
                iActivePlayer = NULL;
                }
            else
                {
                iLastError = CreateAnotherPlayer(&iVideoPlayer2,
                                                 NULL,
                                                 KNullDesC,
                                                 TSize(iDisplaySize.iWidth/2, iDisplaySize.iHeight/2), 
                                                 TPoint(0,iDisplaySize.iHeight/2));
                
                iActivePlayer = iVideoPlayer2;
                EnableDragging();
                }
            break;
            }            
        case EOperation_ToggleThirdVideo:
            {
            if( iVideoPlayer3 )
                {
                delete iVideoPlayer3;
                iVideoPlayer3 = NULL;
                iActivePlayer = NULL;
                }
            else
                {
                iLastError = CreateAnotherPlayer(&iVideoPlayer3,
                                                 NULL,
                                                 KNullDesC,
                                                 TSize(iDisplaySize.iWidth/3,iDisplaySize.iHeight/3), 
                                                 TPoint(0,2*iDisplaySize.iHeight/3));

                iActivePlayer = iVideoPlayer3;
                EnableDragging();
                }
            break;
            }
        case EOperation_DragWindow:
            {
            EnableDragging();
            iSelectedOptionTextCounter = 1000;
            break;
            }
        case EOperation_ToggleOrdinalPositions:
            {
            iOrdinalIndex = (iOrdinalIndex+1) % KOrdinalPositionConfigurationCount;
            UpdateWindowOrdinalPositions();
            
            iSelectedOptionTextCounter = 6;
            iSelectedOptionText.Format(_L("Ordinals: %i %i %i %i %i"), 
                    KOrdinalPositions[iOrdinalIndex][0] + 1,
                    KOrdinalPositions[iOrdinalIndex][1] + 1,
                    KOrdinalPositions[iOrdinalIndex][2] + 1,
                    KOrdinalPositions[iOrdinalIndex][3] + 1,
                    KOrdinalPositions[iOrdinalIndex][4] + 1 );
            break;
            }
        case EOperation_AbnormalTermination:
            {
            User::Panic( _L("Abnormal termination"), 123 );
            break;            
            }
        case EOperation_ToggleExternalPlayer:
            {
            if( iExternalPlayer )
                {
                delete iExternalPlayer;
                iExternalPlayer = NULL;
                iActivePlayer = NULL;
                }
            else
                {
                iLastError = CreateAnotherPlayer(NULL,
                                                 &iExternalPlayer,
                                                 KExternalPlayerExe,
                                                 TSize(iDisplaySize.iWidth/2, iDisplaySize.iHeight/2),
                                                 TPoint(iDisplaySize.iWidth/2, 0));
                iActivePlayer = iExternalPlayer;
                EnableDragging();
                }            
            break;
            }
        case EOperation_ToggleExternalPlayer2:
            {
            if( iExternalPlayer2 )
                {
                delete iExternalPlayer2;
                iExternalPlayer2 = NULL;
                iActivePlayer = NULL;
                }
            else
                {
                iLastError = CreateAnotherPlayer(NULL,
                                                 &iExternalPlayer2,
                                                 KExternalPlayerExe2,
                                                 TSize(iDisplaySize.iWidth/2, iDisplaySize.iHeight/2),
                                                 TPoint(iDisplaySize.iWidth/2, iDisplaySize.iHeight/2));            
                iActivePlayer = iExternalPlayer2;
                EnableDragging();
                }
            break;
            }            
        case EOperation_LaunchAllWindows:
            {
            if( iVideoPlayer->FullFilename().Length() > 0 )
                {
                if( iVideoPlayer2 == NULL )
                    {
                    iVideoPlayer2 = PlayerWindowNewL( iMainPlayerApi,
                                                      TPoint(0,iDisplaySize.iHeight/2), 
                                                      TSize(iDisplaySize.iWidth/2, iDisplaySize.iHeight/2), 
                                                      true,  // transparent 
                                                      KRgbTransparent,
                                                      false );
                
                    iVideoPlayer2->StartWithFilenameL( iVideoPlayer->FullFilename(), CPlayerWindow::EBestFit);
                    }

                if( iVideoPlayer3 == NULL )
                    {
                    iVideoPlayer3 = PlayerWindowNewL( iMainPlayerApi,
                                                      TPoint(0,2*iDisplaySize.iHeight/3),
                                                      TSize(iDisplaySize.iWidth/3,iDisplaySize.iHeight/3), 
                                                      true,  // transparent 
                                                      KRgbTransparent,
                                                      false );
                
                    iVideoPlayer3->StartWithFilenameL( iVideoPlayer->FullFilename(), CPlayerWindow::EBestFit);
                    }
            
                if( iExternalPlayer == NULL )
                    {
                    iExternalPlayer = new CExternalPlayer( iWs );
                
                    iExternalPlayer->Launch( KExternalPlayerExe,
                                             *iWindowGroup,
                                             iVideoPlayer->FullFilename(),
                                             true,
                                             TPoint(iDisplaySize.iWidth/2, 0),
                                             TSize(iDisplaySize.iWidth/2, iDisplaySize.iHeight/2) );
                    }
                            
                /* TODO: FIGURE OUT HOW TO LAUNCH TWO EXTERNAL WINDOWS
                if( iExternalPlayer2 == NULL )
                    {
                    iExternalPlayer2 = new CExternalPlayer();
                
                    iExternalPlayer2->Launch( KExternalPlayerExe2,
                                              *iWindowGroup,
                                              iVideoPlayer->FullFilename(),
                                              true,
                                              TPoint(iDisplaySize.iWidth/2, iDisplaySize.iHeight/2),
                                              TSize(iDisplaySize.iWidth/2, iDisplaySize.iHeight/2) );
                                              
                    }                         
                */  
                
                UpdateWindowOrdinalPositions();
                
                DrawBorderAroundActiveWindows();
                }
            break;
            }            
        case EOperation_ObscureWindow:
            {
            iObscured = !iObscured;
            iFullScreenWindow->SetVisible( iObscured );            
            break;
            }
        default:
            {
            // Route the operation to the appropriate player window.
            MPlayerCommands* player = CurrentPlayer();
            if( player )
                {
                iLastError = player->ExecuteOperation( aOperation );
                }
            else
                {
                // Do not show the operation text.
                iSelectedOptionText.SetLength(0);
                }
            break;
            }
        }

    UpdateVideoWindow();
    
    if( iLastError != KErrNone )
        {
        RDebug::Printf( "CONSOLEPLAYER error=%i", iLastError );            
        }        
    }

void CConsolePlayer::HandlePointerEvent( const TAdvancedPointerEvent& aEvent )
    {    
    TTestAppPointerEvent pointerEvent = CharacterizePointerEvent( aEvent );

    if( pointerEvent == EPointerEvent_Select )
        {
        DisableDragging();
        iSelectedOptionText.Copy( _L("Drag disabled") );
        iSelectedOptionTextCounter = 3;
        UpdateVideoWindow();        
        }
    else         
        {
        MPlayerCommands* player = CurrentPlayer();
        if( player )
            {        
            player->MoveWindow( aEvent.iParentPosition );
            }
        
        UpdateVideoWindow();        
        }    
    }

void CConsolePlayer::PlayInitiated()
    {
    RDebug::Printf( "CONSOLEPLAYER PlayInitiated" );
   
    UpdateWindowOrdinalPositions();
    
    if( iTimerCallback )
        {
        iTimerCallback->Start( 1000000 );  // 1 second
        }

    // Display this text at startup.
    iSelectedOptionText.Copy( _L("Press '0' during playback for help") );
    iSelectedOptionTextCounter = 5;

    StartMonitoringWindowEvents();
    }

void CConsolePlayer::PlayStateChanged()
    {
    UpdateVideoWindow();
    }

void CConsolePlayer::Error(TInt aError)
    {
    RDebug::Printf( "CONSOLEPLAYER error=%i", aError );
    CActiveScheduler::Stop();
    
    iFinalResult = aError;
    }

void CConsolePlayer::TimerCallback()
    {
    if( iSelectedOptionText.Length() > 0 )
        {
        iSelectedOptionTextCounter--;
        
        if( iSelectedOptionTextCounter == 0 )
            {
            iSelectedOptionText.SetLength(0);
            }
        }
    
    if( iVideoWindowTextMode > 0 )
        {
        UpdateVideoWindow();
        }
    }

GLDEF_C TInt E32Main()
    {
    __UHEAP_MARK;
    
    CActiveScheduler* scheduler = new CActiveScheduler;
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if( scheduler != NULL && cleanup != NULL )
    {
        CActiveScheduler::Install( scheduler );
        TRAP_IGNORE( CConsolePlayer::ExecuteL() );
    }
    delete cleanup;
    delete scheduler;
    REComSession::FinalClose();
    __UHEAP_MARKEND;
    return 0;
    }

