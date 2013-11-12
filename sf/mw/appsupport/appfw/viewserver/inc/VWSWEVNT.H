// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef __VWSWEVNT_H__
#define __VWSWEVNT_H__

#include <w32std.h>


//
// Forward declarations.
//

class CVwsServer;


/**
 * The CVwsWServEventHandler class actively monitors the window server for screen device change notifications.
 */ 
class CVwsWServEventHandler : public CActive
	{
public:
 	static CVwsWServEventHandler* NewL(CVwsServer& aServer, RWsSession& aWsSession, TBool aIgnoreFirstScreenDeviceChangedEvent);

	~CVwsWServEventHandler();
	void Queue();
private: // From CActive.
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);
private:
	CVwsWServEventHandler(CVwsServer& aServer, RWsSession& aWsSession, TBool aIgnoreFirstScreenDeviceChangedEvent);
	void ConstructL();
private:
	CVwsServer& iServer;
	RWsSession& iWsSession;
	TBool		iIgnoreNextScreenDeviceChangedEvent;
	};

/**
 * The CVwsWServRedrawer class handles redraw events for the window used by the view server to manage WSERV activity
 * during view transistions. The window is created in front of normal applications and is put up for the duration of
 * a view transistion. The window is transparent for ordinary view switches and opaque for screen orientation changes. 
 */ 
class CVwsWServRedrawer : public CActive
	{
public:
	static CVwsWServRedrawer* NewL(RWsSession& aWsSession, CWindowGc& aGc, RWindow& aWindow);
	~CVwsWServRedrawer();
	void SetDrawMode(TBool aOpaque);
	void Queue();
private: // From CActive
	void RunL();
	void DoCancel();
private:
	void HandleRedrawEvent(const TRect& aRect);
	void ConstructL();
	CVwsWServRedrawer(RWsSession& aWsSession, CWindowGc& aGc, RWindow& aWindow);
private:
	RWsSession& iWsSession;
	CWindowGc& iGc;
	RWindow& iWindow;
	TBool iOpaque;
	};

/**
 * The CVwsWServSessionHandler class connects to the window server and manages WSERV activity.
 * It uses an event handler to detect screen device changes for the view server and puts up a window in front of normal
 * applications to manage activity during view transistions.
 */ 
class CVwsWServSessionHandler : public CBase
	{
public:
	static CVwsWServSessionHandler* NewL(CVwsServer& aServer);
	~CVwsWServSessionHandler();
	void DisplayWindow();
	void HideWindow();
	void ConfigureScreenDevice();
	TInt GetScreenMode();
	void UpdateScreenAndDisplayWindow();
	inline RWsSession& WsSession() const;
	void SetWindowBackgroundColor(const TRgb& aBgColor);
private:
	void ConstructL();
	void CreateWServSessionL();
	CVwsWServSessionHandler(CVwsServer& aServer);

	CVwsWServEventHandler* iWServEventHandler;
	CVwsWServRedrawer* iWServRedrawer;
	CVwsServer& iServer;
	TBool iFirstDisplay;

	RWsSession iWsSession;
	CWsScreenDevice* iScreen;
	CWindowGc* iGc;
	RWindowGroup iRootWin;
	RWindow* iWindow;
	};


//
// Inlines
//

inline RWsSession& CVwsWServSessionHandler::WsSession() const
	{ return((RWsSession&)iWsSession); }

#endif
