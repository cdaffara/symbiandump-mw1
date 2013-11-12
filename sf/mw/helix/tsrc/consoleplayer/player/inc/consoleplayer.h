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
 * The header for the main class of the ConsolePlayer test app.
 * 
 */

#ifndef __CONSOLE_PLAYER_H__
#define __CONSOLE_PLAYER_H__

#include <e32msgqueue.h>

#include "testappbase.h"
#include "timercallback.h"
#include "playerwindow.h"
#include "externalplayer.h"

class CPlayerWindow;

enum TOperations
    {
    EOperation_ChangeDisplayMode = KOperation_FirstCustomIndex,
    EOperation_AbnormalTermination,
    EOperation_ToggleSecondVideo,
    EOperation_ToggleThirdVideo,
    EOperation_ToggleExternalPlayer,
    EOperation_ToggleExternalPlayer2,
    EOperation_ObscureWindow,
    EOperation_DragWindow,
    EOperation_ToggleOrdinalPositions,
    EOperation_LaunchAllWindows    
    };

const TInt KMaxHistoryEntries = 10;
_LIT( KHistoryFilename, "c:\\console_player_history.txt" );

_LIT( KUrlListFilename, "c:\\data\\urls.txt" );

class CConsolePlayer : public CTestAppBase,
                       private MPlayerWindowObserver,
                       private ITimerCallbackClient,
                       private MPlayerCommands
                       
{
public: // Methods

    static void ExecuteL();
    
private: // Methods

    enum TActionToPerform
        {
        EAction_None = 0x0000,
        EAction_PlayLocalFile = 0x0001,
        EAction_PlayUrl = 0x0002,
        EAction_SameAsMainPlayer = 0x0008
        };
    
    enum TApiToUse
        {
        EApi_VideoPlayer = 0x1,
        EApi_OpenMaxAl = 0x2
        };
    
    CConsolePlayer(); 

    ~CConsolePlayer();

    void InitL();

    void Main();
        
    void MainL();
    
    bool SelectUrlL( TDes& aUrl );

    void UpdateVideoWindow();

    void UpdateWindowOrdinalPositions();
    
    void EnableDragging();
    
    void DisableDragging();
    
    void SelectAction( TUint aActionMask, 
                       TUint aApiMask, 
                       bool aPresentPerformanceModeOption, 
                       TActionToPerform& aAction, 
                       TApiToUse& aApi, 
                       bool& aPerformanceMode,
                       TFileName& aLocation );
    
    void DoSelectAction( TUint aActionMask, 
                         bool aPresentPerformanceModeOption, 
                         TActionToPerform& aAction,
                         bool& aPerformanceMode );
    
    bool DoSelectApi( TUint aApiMask, 
                      TApiToUse& aApi ); 
    
    CPlayerWindow* PlayerWindowNewL( TApiToUse aApi,
                                     TPoint aTopLeft,
                                     TSize aSize,
                                     bool aTransparent,
                                     TRgb aBackgroundColor,
                                     bool aSuppressGraphicsContext );
    
    TInt CreateAnotherPlayer( CPlayerWindow** aPlayerWindow, 
                              CExternalPlayer** aExternalPlayer,
                              const TDesC& aExternalPlayerExeName,
                              TSize aSize, 
                              TPoint aTopLeft );
    
    MPlayerCommands* CurrentPlayer();

    void SendOperationToAllPlayers( TInt aOperation );
    
    void DrawBorderAroundActiveWindows();
    
    // inherited from CTestAppBase
    virtual void SoftkeyFunctionUpdated();
    virtual bool ConsumeKeyEvent( TInt aKeyCode );
    virtual void ExecuteOperation( TInt aOperation, const TDesC& aOperationText );
    virtual void HandlePointerEvent( const TAdvancedPointerEvent& aEvent );    

    // inherited from MPlayerWindowObserver
    virtual void PlayInitiated();
    virtual void PlayStateChanged();
    virtual void Error(TInt aError);
        
    // inherited from MPlayerCommands
    virtual TInt ExecuteOperation( TInt aOperation );    
    virtual void MoveWindow( TPoint aNewCenter );    
    virtual void SetOrdinalPosition( TInt aPosition );
    
    // inherited from ITimerCallbackClient    
    void TimerCallback();
    
private: // Attributes
    
    CTimerCallback*       iTimerCallback;    
    TBuf<120>             iBuffer;
    TInt                  iLastError;
    TInt                  iVideoWindowTextMode;
    TBuf<120>             iSelectedOptionText;
    TInt                  iSelectedOptionTextCounter;
    TInt                  iOrdinalIndex;
    bool                  iVideoWindow2Transparent;
    CPlayerWindow*        iVideoPlayer;
    CPlayerWindow*        iVideoPlayer2;
    CPlayerWindow*        iVideoPlayer3;
    CExternalPlayer*      iExternalPlayer;
    CExternalPlayer*      iExternalPlayer2;
    MPlayerCommands*      iActivePlayer;
    MPlayerCommands*      iPlayerBeingDragged;
    RWindow*              iFullScreenWindow;
    bool                  iObscured;
    TInt                  iFinalResult;
    TActionToPerform      iMainPlayerAction;
    TApiToUse             iMainPlayerApi;
    
    };

#endif
