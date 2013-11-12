/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Client Window class for Splash Screen displayer.
*
*/


#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

// INCLUDES
#include <e32property.h>

// CLASS DECLARATION

/**
*  Forward declarations
*
*  @lib SplashScreen.lib
*  @since Series 60 1.0
*/
class CWsRedrawer;
class CWindow;

class CMainWindow;
class CCdlEngineRef;
class CTimeout : public CTimer
	{
public:
	CTimeout();
	void ConstructL(CMainWindow* aWindow);
	void RunL();
	void SetMainWindow(CMainWindow* aWindow);
private:
	CMainWindow* iWindow;
	};

/**
*  Base class for all windows
*
*  @lib SplashScreen.lib
*  @since Series 60 1.0
*/
class CWsClient : public CActive
	{
	protected:
		//construct
		CWsClient();
		CWsScreenDevice* iScreen;
		CWsRedrawer* iRedrawer;
		RWsSession iWs;
		TWsEvent iWsEvent;
		CCdlEngineRef* iCdlEngRef;
	public:
		void ConstructL();
		// destruct
		~CWsClient();
		// main window
		virtual void ConstructMainWindowL()=0;
		// terminate cleanly
		void Exit();
		// active object protocol
		void IssueRequest(); // request an event
		void DoCancel(); // cancel the request
		virtual void RunL() = 0; // handle completed request
		virtual void HandleKeyEventL (TKeyEvent& aKeyEvent) = 0;

		RWindowGroup Group() {return iGroup;};

        TInt            iPSRemoveSplashState;

    protected:
        TInt iLastScreenMode;

    private:
		RWindowGroup    iGroup;
		CWindowGc*      iGc;
		friend class    CWsRedrawer; // needs to get at session
		friend class    CWindow; // needs to get at session
        RProperty       iProperty;

	};


/**
*  CWsRedrawer declaration
*
*  @lib SplashScreen.lib
*  @since Series 60 1.0
*/
class CWsRedrawer : public CActive
	{
	public:
		// construct/destruct
		CWsRedrawer();
		void ConstructL(CWsClient* aClient);
		~CWsRedrawer();
		// drawing
		void IssueRequest();
		void DoCancel();
		void RunL();
	protected:
		CWsClient* iClient;
	private:
		//	CWsClient* iClient;
	};

/**
*  CWindow declaration
*
*  @lib SplashScreen.lib
*  @since Series 60 1.0
*/
class CWindow : public CBase
	{
	protected:
		RWindow iWindow; // window server window
		TRect iRect; // rectangle re owning window
	public:
		CWindow(CWsClient* aClient);
		void ConstructL (const TRect& aRect, CWindow* aParent=0);
		~CWindow();
		// access
		RWindow& Window(); // our own window
		CWindowGc* SystemGc(); // system graphics context
		// drawing
		virtual void Draw(const TRect& aRect) = 0;
		virtual void HandlePointerEvent (TPointerEvent& aPointerEvent) = 0;

		CWsClient* Client() {return iClient;};
	private:
		CWsClient* iClient; // client including session and group
	};

/**
*  Derived window classes
*
*  @lib SplashScreen.lib
*  @since Series 60 1.0
*/
class CMainWindow : public CWindow
	{
	public:
		CMainWindow (CWsClient* aClient);
		~CMainWindow ();
		void Draw (const TRect& aRect);
		CFbsBitmap* ReadSVGL (const TFileName& aFileName);
		void HandlePointerEvent (TPointerEvent& aPointerEvent);
		void ConstructL (const TRect& aRect, CWindow* aParent=0);
	private:
		CFbsBitmap *iBitmap;
		
	};

/**
*  Derived client class
*
*  @lib SplashScreen.lib
*  @since Series 60 1.0
*/
class CSplashWsClient : public CWsClient
	{
	public:
		// constructor
		CSplashWsClient ();
		void ConstructMainWindowL();
		~CSplashWsClient ();
		void RunL ();
		void HandleKeyEventL (TKeyEvent& aKeyEvent);
	private:
		CMainWindow* iMainWindow;	// main window
		CTimeout* iTimeout;			// auto dismiss timer
	};

#endif //SPLASHSCREEN_H

