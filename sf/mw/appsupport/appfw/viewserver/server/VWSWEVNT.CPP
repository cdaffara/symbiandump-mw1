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
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include "vwsinternal.h"
#include "vwsdefpartner.h"
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS
#include "VWSWEVNT.H"
#include "VWSERVER.H"
#include "VWSDEBUG.H"

const TInt KPriorityNeverAtFront = -999;
const TInt KPriorityAlwaysAtFront = 999;//Setting a priority greater than or equal to KPasswordWindowGroupPriority which is 1000,
										//on a window group will require the capability SwEvent.If the client does not have 
						    		    //this capability, then the window group priority will be reduced to KPasswordWindowGroupPriority - 1. 
					 					//As KPasswordWindowGroupPriority has been deprectated 999 is set as the priority.KPriorityAlwaysAtFront
					                    //is used when calling SetOrdinalPosition API.
 

//
// CVwsWServEventHandler.
//

CVwsWServEventHandler::CVwsWServEventHandler(CVwsServer& aServer, RWsSession& aWsSession, TBool aIgnoreFirstScreenDeviceChangedEvent)
	: CActive(CActive::EPriorityStandard),iServer(aServer),iWsSession(aWsSession),
	iIgnoreNextScreenDeviceChangedEvent(aIgnoreFirstScreenDeviceChangedEvent)
	{
	}

CVwsWServEventHandler::~CVwsWServEventHandler()
	{
	Cancel();
	}

CVwsWServEventHandler* CVwsWServEventHandler::NewL(CVwsServer& aServer, RWsSession& aWsSession, TBool aIgnoreFirstScreenDeviceChangedEvent)
	{
	CVwsWServEventHandler* self=new(ELeave) CVwsWServEventHandler(aServer, aWsSession, aIgnoreFirstScreenDeviceChangedEvent);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CVwsWServEventHandler::ConstructL()
	{
	CActiveScheduler::Add(this);
	Queue();
	}

void CVwsWServEventHandler::Queue()
	{
	ASSERT (!IsActive());
	iWsSession.EventReady(&iStatus);
	SetActive();
	}

void CVwsWServEventHandler::RunL()
	{
	TWsEvent event;
	iWsSession.GetEvent(event);
	LOG3(CVwsLog::ELoud,_L("Received %d event type from WServ"),event.Type());
	TInt err=KErrNone;
	switch (event.Type()) 
		{
	case EEventScreenDeviceChanged:
		if (iIgnoreNextScreenDeviceChangedEvent)
			{
			iIgnoreNextScreenDeviceChangedEvent = EFalse;
			}
		else
			{
			TRAP(err,iServer.HandleScreenDeviceChangedL());
			}
		break;
	case EEventFocusGroupChanged:
		iServer.HandleForegroundTaskChange();
		break;
	default:
		break;
		}

	Queue();
	User::LeaveIfError(err);
	}

void CVwsWServEventHandler::DoCancel()
	{
	iWsSession.EventReadyCancel();
	}

#ifdef _DEBUG
TInt CVwsWServEventHandler::RunError(TInt aError)
#else
TInt CVwsWServEventHandler::RunError(TInt /*aError*/)
#endif
	{
	LOG3(CVwsLog::ENormal,_L("CVwsWServEventHandler::RunError: %d"), aError);
	iServer.KickStartEventQ();
	return KErrNone;       
	}

//
// class CVwsWServRedrawer
//

CVwsWServRedrawer::CVwsWServRedrawer(RWsSession& aWsSession, CWindowGc& aGc, RWindow& aWindow)
	: CActive(EPriorityLow),iWsSession(aWsSession),iGc(aGc),iWindow(aWindow),iOpaque(EFalse)
	{
	}

CVwsWServRedrawer::~CVwsWServRedrawer()
	{
	Cancel();
	}

CVwsWServRedrawer* CVwsWServRedrawer::NewL(RWsSession& aWsSession, CWindowGc& aGc, RWindow& aWindow)
	{
	CVwsWServRedrawer* self=new(ELeave) CVwsWServRedrawer(aWsSession,aGc,aWindow);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}	

void CVwsWServRedrawer::ConstructL()
	{
	CActiveScheduler::Add(this);
	Queue();
	}

void CVwsWServRedrawer::RunL()
	{
	TWsRedrawEvent redraw;
	iWsSession.GetRedraw(redraw);
	HandleRedrawEvent(redraw.Rect());
	Queue();
	}

void CVwsWServRedrawer::Queue()
	{
	ASSERT (!IsActive());
	iWsSession.RedrawReady(&iStatus);
	SetActive();
	}

void CVwsWServRedrawer::DoCancel()
	{
	iWsSession.RedrawReadyCancel();
	}

void CVwsWServRedrawer::SetDrawMode(TBool aOpaque)
	{
	iOpaque=aOpaque;
	}

void CVwsWServRedrawer::HandleRedrawEvent(const TRect& aRect)
	{
	// this code is needed to make the screen redraw correctly
	// after a screen mode switch.

	LOG2(CVwsLog::ELoud,_L("CVwsWServRedrawer::HandleRedrawEvent"));
	iGc.Activate(iWindow);
	iWindow.BeginRedraw(aRect);
	iWindow.EndRedraw();
	iGc.Deactivate();
	iWsSession.Flush();
	}

CVwsWServSessionHandler::CVwsWServSessionHandler(CVwsServer& aServer)
	:iServer(aServer),
	iFirstDisplay(ETrue)
	{
	}

CVwsWServSessionHandler::~CVwsWServSessionHandler()
	{
	delete iWServEventHandler;
	delete iWServRedrawer;
	delete iGc;
	delete iScreen;
	iRootWin.Close();
	if (iWindow!=NULL)
		{
		iWindow->Close();
		delete iWindow;
		}
	
	ASSERT(iWsSession.ResourceCount()==0);
	iWsSession.Close();
	}

CVwsWServSessionHandler* CVwsWServSessionHandler::NewL(CVwsServer& aServer)
	{
	CVwsWServSessionHandler* self=new(ELeave) CVwsWServSessionHandler(aServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CVwsWServSessionHandler::ConstructL()
	{
	CreateWServSessionL();
	TBool ignoreFirstScreenDeviceChangedEvent = (GetScreenMode() > 0);
	iWServEventHandler=CVwsWServEventHandler::NewL(iServer,iWsSession,
												   ignoreFirstScreenDeviceChangedEvent);
	iWServRedrawer=CVwsWServRedrawer::NewL(iWsSession,*iGc,*iWindow);
	}

void CVwsWServSessionHandler::CreateWServSessionL()
	{
	User::LeaveIfError(iWsSession.Connect());

	iScreen=new(ELeave) CWsScreenDevice(iWsSession);
	User::LeaveIfError(iScreen->Construct());
	User::LeaveIfError(iScreen->CreateContext(iGc));

	iRootWin=RWindowGroup(iWsSession);
	User::LeaveIfError(iRootWin.Construct((TUint32)this,EFalse));
	iRootWin.SetOrdinalPosition(0,KPriorityNeverAtFront);
	iRootWin.EnableScreenChangeEvents();
	iRootWin.EnableFocusChangeEvents();

	iWindow=new(ELeave) RWindow(iWsSession);
	User::LeaveIfError(iWindow->Construct(iRootWin,((TUint32)this)+1));
	iWindow->Activate();
	iWindow->SetVisible(EFalse);
	}

TInt CVwsWServSessionHandler::GetScreenMode()
	{
	return iScreen->CurrentScreenMode();
	}

void CVwsWServSessionHandler::ConfigureScreenDevice()
	{
	LOG2(CVwsLog::ENormal,_L("CVwsWServSessionHandler:::ConfigureScreenDevice()"));
	TPixelsAndRotation sizeAndRotation;
	iScreen->GetDefaultScreenSizeAndRotation(sizeAndRotation);
	iScreen->SetScreenSizeAndRotation(sizeAndRotation);
	iWindow->SetSize(sizeAndRotation.iPixelSize);
	}

void CVwsWServSessionHandler::DisplayWindow()
	{
	LOG2(CVwsLog::ENormal,_L("CVwsWServSessionHandler::DisplayWindow()"));
	if (iFirstDisplay)
		{
		iRootWin.SetOrdinalPosition(0,KPriorityAlwaysAtFront);
		iFirstDisplay=EFalse;
		}
	iWindow->SetVisible(ETrue);
	iWsSession.Flush();	
	}

void CVwsWServSessionHandler::HideWindow()
	{
	LOG2(CVwsLog::ENormal,_L("CVwsWServSessionHandler::HideWindow()"));
	iWindow->SetVisible(EFalse);
	iWsSession.Flush();
	iWServRedrawer->SetDrawMode(EFalse);
	}

void CVwsWServSessionHandler::UpdateScreenAndDisplayWindow()
	{
	ConfigureScreenDevice();
	iWServRedrawer->SetDrawMode(ETrue);
	DisplayWindow();
	}
	
void CVwsWServSessionHandler::SetWindowBackgroundColor(const TRgb& aBgColor)
	{
	iWindow->SetBackgroundColor(aBgColor);
	}
