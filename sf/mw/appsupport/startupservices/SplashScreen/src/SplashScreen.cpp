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
* Description:  Displays single startup screen. Animation not supported.
*
*/


// SYSTEM INCLUDES
#include <w32std.h>
#include <eikenv.h>
#include <coedef.h>
#include <data_caging_path_literals.hrh>
#include <startupdomainpskeys.h>
#include <SVGEngineInterfaceImpl.h>
#include <gdi.h>
#include <coemain.h> 

// USER INCLUDES
#include "SplashScreen.h"
#include "SplashScreenDefines.h"


// ==================== LOCAL FUNCTIONS ====================
LOCAL_C void DoItL();

GLDEF_C TInt E32Main()
	{
    TRACES("E32Main(): Start");

	__UHEAP_MARK;
	CTrapCleanup* cleanup=CTrapCleanup::New(); // get clean-up stack

	TRAPD(error,DoItL()); // more initialization, then do Splash
	__ASSERT_ALWAYS(!error,User::Panic(KPanicMsg,error));
	delete cleanup; // destroy clean-up stack
	__UHEAP_MARKEND; // check no memory leak
    TRACES("E32Main(): End");
	return KErrNone;
	}

// 120 seconds (the splash should have been killed
// by the Starter before this runs out)
const TInt KSplashTimeout=(2* 60 * 1000000);


///////////////////////////////////////////////////////////////////////////////
////////////////////////// CWindow implementation /////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CWindow::CWindow(CWsClient* aClient)
: iClient(aClient)
	{
    TRACES("CWindow::CWindow()");
	}

void CWindow::ConstructL (const TRect& aRect, CWindow* aParent)
	{
    TRACES("CWindow::ConstructL(): Start");

	// If a parent window was specified, use it; if not, use the window group
	// (aParent defaults to 0).
	RWindowTreeNode* parent= aParent ? (RWindowTreeNode*) &(aParent->Window()) : &(iClient->iGroup);
	iWindow=RWindow(iClient->iWs); // use app's session to window server
	User::LeaveIfError(iWindow.Construct(*parent,(TUint32)this));
    TRACES1("CWindow::ConstructL(): Start - window handle is: 0x%08x", this);
	iRect = aRect;
	iWindow.SetExtent(iRect.iTl, iRect.Size()); // set extent relative to group coords
	iWindow.Activate(); // window is now active
    TRACES("CWindow::ConstructL(): End");
	}


CWindow::~CWindow()
	{
    TRACES("CWindow::~CWindow(): Start");
	iWindow.Close(); // close our window
    TRACES("CWindow::~CWindow(): End");
	}

RWindow& CWindow::Window()
	{
    TRACES("CWindow::Window()");
	return iWindow;
	}

CWindowGc* CWindow::SystemGc()
	{
    TRACES("CWindow::SystemGc()");
	return iClient->iGc;
	}


///////////////////////////////////////////////////////////////////////////////
////////////////////////// CWsRedrawer implementation /////////////////////////
///////////////////////////////////////////////////////////////////////////////

CWsRedrawer::CWsRedrawer()
: CActive(CActive::EPriorityLow)
	{
    TRACES("CWsRedrawer::CWsRedrawer()");
	}

void CWsRedrawer::ConstructL(CWsClient* aClient)
	{
    TRACES("CWsRedrawer::ConstructL(): Start");
	iClient=aClient; // remember WsClient that owns us
	CActiveScheduler::Add(this); // add ourselves to the scheduler
	IssueRequest(); // issue request to draw
    TRACES("CWsRedrawer::ConstructL(): End");
	}

CWsRedrawer::~CWsRedrawer()
	{
    TRACES("CWsRedrawer::~CWsRedrawer(): Start");
	Cancel();
    TRACES("CWsRedrawer::~CWsRedrawer(): End");
	}

void CWsRedrawer::IssueRequest()
	{
    TRACES("CWsRedrawer::IssueRequest(): Start");
	iClient->iWs.RedrawReady(&iStatus);
	SetActive();
    TRACES("CWsRedrawer::IssueRequest(): End");
	}

void CWsRedrawer::DoCancel()
	{
    TRACES("CWsRedrawer::DoCancel(): Start");
	iClient->iWs.RedrawReadyCancel();
    TRACES("CWsRedrawer::DoCancel(): End");
	}

void CWsRedrawer::RunL()
	{
    TRACES("CWsRedrawer::RunL(): Start");

	// find out what must be done
	TWsRedrawEvent redrawEvent;
    iClient->iWs.GetRedraw(redrawEvent); // get event
	CWindow* window=(CWindow*)(redrawEvent.Handle()); // get window
	if (window)
		{
        TRACES1("CWsRedrawer::RunL(): window - 0x%08x", window);
		TRect rect=redrawEvent.Rect(); // and rectangle that needs redrawing
		// now do drawing
		iClient->iGc->Activate(window->Window());
		window->Window().BeginRedraw(rect);
		window->Draw(rect);
		window->Window().EndRedraw();
		iClient->iGc->Deactivate();
		}
	// maintain outstanding request
	IssueRequest();
    TRACES("CWsRedrawer::RunL(): End");
	}

CTimeout::CTimeout()
:   CTimer(0)
	{
    TRACES("CTimeout::CTimeout()");
	CActiveScheduler::Add(this);
	}

void CTimeout::ConstructL(CMainWindow* aWindow)
	{
    TRACES("CTimeout::ConstructL(): Start");
    CTimer::ConstructL();
	SetMainWindow( aWindow );
    TRACES("CTimeout::ConstructL(): End");
	}

void CTimeout::SetMainWindow( CMainWindow* aWindow )
    {
	iWindow = aWindow;
    }

void CTimeout::RunL()
	{
    TRACES("CTimeout::RunL(): Start");
	CActiveScheduler::Stop();
    if  ( iWindow )
        {
    	iWindow ->Client()->Group().SetOrdinalPosition(0,ECoeWinPriorityNeverAtFront);	// Background - so it's no more visible
        }
    TRACES("CTimeout::RunL(): End");
	}


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// CWsClient implementation ////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
CWsClient::CWsClient()
: CActive(CActive::EPriorityStandard),
  iPSRemoveSplashState( ESplashRunning )
	{
    TRACES("CWsClient::CWsClient()");
	}

void CWsClient::ConstructL()
	{
    TRACES("CWsClient::ConstructL(): Start");

    RProperty::Define(KPSUidStartup,
                      KPSSplashShutdown,
                      RProperty::EInt,
                      KReadPolicy,
                      KWritePolicy );
    RProperty::Set( KPSUidStartup, KPSSplashShutdown, iPSRemoveSplashState );
	CActiveScheduler::Install(new (ELeave) CActiveScheduler);
	CActiveScheduler::Add(this);
    iProperty.Attach( KPSUidStartup, KPSSplashShutdown );
    iProperty.Subscribe( iStatus );

	// get a session going
	User::LeaveIfError(iWs.Connect());

	// construct screen device and graphics context
	iScreen=new (ELeave) CWsScreenDevice(iWs); // make device for this session
	User::LeaveIfError(iScreen->Construct( 0 )); // and complete its construction
	User::LeaveIfError(iScreen->CreateContext(iGc)); // create graphics context
    iLastScreenMode = iScreen->CurrentScreenMode();
    TRACES1("CWsClient::ConstructL() - iLastScreenMode: %d", iLastScreenMode);

	// construct our one and only window group
	iGroup=RWindowGroup(iWs);
	User::LeaveIfError(iGroup.Construct( (TInt) this,ETrue)); // meaningless handle; enable focus
    // Set our name for sending commands via key-events
    iGroup.SetName(KSplashScreenWindowGroup);

    TInt error = iGroup.EnableScreenChangeEvents();
    TRACES1("CWsClient::ConstructL() - enable screen change events: %d", error);

	// construct redrawer
	iRedrawer=new (ELeave) CWsRedrawer;
	iRedrawer->ConstructL(this);
	// construct main window
	ConstructMainWindowL();

    iWs.EventReady(&iStatus); // request an event
	SetActive(); // so we're now active
	// request first event and start scheduler

    TRACES("CWsClient::ConstructL(): About to start CActiveScheduler");

	CActiveScheduler::Start(); // start the active scheduler

    TRACES("CWsClient::CWsClient(): End");
	}

CWsClient::~CWsClient()
	{
    TRACES("CWsClient::~CWsClient(): Start");
	// neutralize us as an active object
	Deque(); // cancels and removes from scheduler
	// get rid of scheduler and all attached objects
	delete CActiveScheduler::Current(); // delete the scheduler
	// get rid of everything we allocated
	delete iGc;
	delete iScreen;
	delete iRedrawer;
	// destroy window group
	iGroup.Close();
	// finish with window server
    Cancel();
    iProperty.Close();
	iWs.Close();
    TRACES("CWsClient::~CWsClient(): End");
	}

void CWsClient::Exit()
	{
    TRACES("CWsClient::Exit(): Start");
	CActiveScheduler::Stop();
    TRACES("CWsClient::Exit(): End");
	}

void CWsClient::IssueRequest()
	{
    TRACES("CWsClient::IssueRequest(): Start");
	iWs.EventReady(&iStatus); // request an event
	SetActive(); // so we're now active
    TRACES("CWsClient::IssueRequest(): End");
	}

void CWsClient::DoCancel()
	{
    TRACES("CWsClient::DoCancel(): Start");
	iWs.EventReadyCancel(); // cancel event request
    TRACES("CWsClient::DoCancel(): End");
	}



//////////////////////////////////////////////////////////////////////////////
//				Implementation for derived window classes
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//					 CMainWindow implementation							//
//////////////////////////////////////////////////////////////////////////////

/****************************************************************************\
|	Function:	Constructor/Destructor for CMainWindow
|				Doesn't do much, as most initialisation is done by the
|				CWindow base class.
|	Input:		aClient		Client application that owns the window
\****************************************************************************/
CMainWindow::CMainWindow (CWsClient* aClient)
: CWindow (aClient)
	{
    TRACES("CMainWindow::CMainWindow()");
	}


CMainWindow::~CMainWindow ()
	{
    TRACES("CMainWindow::~CMainWindow(): Start");
    delete iBitmap;
    TRACES("CMainWindow::~CMainWindow(): End");
	}

CFbsBitmap* CMainWindow::ReadSVGL (const TFileName& aFileName)
	{
	TRACES("CMainWindow::ReadSVGL(): Start");    
	TFontSpec fontspec;
    TDisplayMode mode = EColor16MA;    
	TSize size(SIZE_X, SIZE_Y);

	//if ( mode >= (TDisplayMode)13 )  { mode = EColor16MA; }

	CFbsBitmap* frameBuffer = new ( ELeave ) CFbsBitmap;
    CleanupStack::PushL( frameBuffer );
    frameBuffer->Create( size, mode );
	
	CSvgEngineInterfaceImpl* svgEngine = NULL;
	svgEngine = CSvgEngineInterfaceImpl::NewL(frameBuffer, NULL, fontspec );    
	
	if (svgEngine == NULL)
		{
		TRACES("CMainWindow::ReadSVGL(): Splashscreen SVG engine creation failed");    
		}
	
	CleanupStack::PushL( svgEngine );
	TInt domHandle = 0;
    svgEngine->PrepareDom( aFileName, domHandle ) ;
	if (domHandle == 0)
		{
		TRACES("CMainWindow::ReadSVGL(): Splashscreen DOM handle creation failed");    
		}

	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;    
    CleanupStack::PushL( bitmap );
    User::LeaveIfError( bitmap->Create( size, EColor64K ) );

	svgEngine->UseDom( domHandle, bitmap, NULL ) ;
	
    MSvgError* err;
    svgEngine->Start( err );
	if (err->HasError())
		{
		TRACES("CMainWindow::ReadSVGL(): Splashscreen SVG Engine Start failed");   
		}

	svgEngine->DeleteDom( domHandle );
    CleanupStack::Pop( bitmap );
    CleanupStack::PopAndDestroy( svgEngine );
    CleanupStack::PopAndDestroy( frameBuffer );
	
    TRACES("CMainWindow::ReadSVGL(): End");
	return bitmap;
	}

void CMainWindow::ConstructL (const TRect& aRect, CWindow* aParent)
	{
    TRACES("CMainWindow::ConstructL(): Start");

    TInt err( KErrNone );

	CWindow::ConstructL(aRect, aParent);

    TParse* fp = new(ELeave) TParse();
    fp->Set(KSplashBitmapName, &KDC_APP_BITMAP_DIR, NULL);
    TRACES1("CMainWindow::ConstructL(): Load Bitmap from %S", &fp->FullName());

    RFs fs;
    TInt thisTry = 0;
    while ( ( err = fs.Connect() ) != KErrNone && ( thisTry++ ) <= KTriesToConnectServer )
        {
        User::After( KTimeBeforeRetryingServerConnection );
        }

    TFindFile findFile( fs );
    err = findFile.FindByPath( fp->FullName(), NULL );
    fs.Close();
    if ( !err )
        {
        TRACES("CMainWindow::ConstructL(): Image found");
		iBitmap = ReadSVGL(fp->FullName());
        }
    else
        {
        TRACES("CMainWindow::ConstructL(): Image not found");
        }

    delete fp;

    TRACES("CMainWindow::ConstructL(): End");
	}


/****************************************************************************\
|	Function:	CMainWindow::Draw
|	Purpose:	Redraws the contents of CMainWindow within a given
|				rectangle. As CMainWindow has no contents, it simply
|				clears the redraw area. A blank window could be used here
|				instead. The Clear() is needed because a redraw should
|				always draw to every pixel in the redraw rectangle.
|	Input:		aRect	Rectangle that needs redrawing
|	Output:		None
\****************************************************************************/
void CMainWindow::Draw(const TRect& aRect)
	{
    TRACES("CMainWindow::Draw(): Start");

	CWindowGc* gc=SystemGc(); // get a gc
	gc->SetClippingRect(aRect); // clip outside this rect
    TRACES2("CMainWindow::Draw(): Rect: X0=%d Y0=%d", aRect.iTl.iX, aRect.iTl.iY);
    TRACES2("CMainWindow::Draw(): Rect: X1=%d Y1=%d", aRect.iBr.iX, aRect.iBr.iY);

    TRACES1("CMainWindow::Draw(): iBitmap = %d", iBitmap);
	if (iBitmap)
		{
        TSize bmpSizeInPixels = iBitmap->SizeInPixels();
        //center image to the center of the screen
        TInt xDelta = ( aRect.Width() - bmpSizeInPixels.iWidth ) / 2;
        TInt yDelta = ( aRect.Height() - bmpSizeInPixels.iHeight ) / 2;
        TPoint pos = TPoint( xDelta , yDelta ); // displacement vector
        TRACES2("CMainWindow::Draw(): Image top left corner: X=%d Y=%d", pos.iX, pos.iY);
        gc->Clear();
		gc->BitBlt(pos, iBitmap);
		}
	else
		{
        TRACES("CMainWindow::Draw(): Image not available. Draw lines from corner to corner");
		TRect rect(Window().Position(), Window().Size());
		gc->DrawLine(rect.iTl, rect.iBr);
		gc->DrawLine(TPoint(rect.iTl.iX, rect.iBr.iY), TPoint(rect.iBr.iX, rect.iTl.iY));
		}
    TRACES("CMainWindow::Draw(): End");
	}

/****************************************************************************\
|	Function:	CMainWindow::HandlePointerEvent
|	Purpose:	Handles pointer events for CMainWindow.  Doesn't do
|				anything except get the co-ordinates where the pointer
|				event occurred.
|	Input:		aPointerEvent	The pointer event
|	Output:		None
\****************************************************************************/
void CMainWindow::HandlePointerEvent (TPointerEvent& /*aPointerEvent*/)
    {
    TRACES("CMainWindow::HandlePointerEvent(): Start");
	
    TRACES("CMainWindow::HandlePointerEvent(): End");
	}

//////////////////////////////////////////////////////////////////////////////
//					 CSplashWsClient implementation						//
//////////////////////////////////////////////////////////////////////////////

/****************************************************************************\
|	Function:	Constructor/Destructor for CSplashWsClient
|				Destructor deletes everything that was allocated by
|				ConstructMainWindowL()
\****************************************************************************/
CSplashWsClient::CSplashWsClient()
	{
    TRACES("CSplashWsClient::CSplashWsClient()");
	}

CSplashWsClient::~CSplashWsClient ()
	{
    TRACES("CSplashWsClient::~CSplashWsClient(): Start");
    if  ( iMainWindow )
        {
        TRACES("CSplashWsClient::~CSplashWsClient(): To background");
    	iMainWindow->Client()->Group().SetOrdinalPosition(-1,ECoeWinPriorityNormal);	// Back to the normal position
        iWs.Flush();
        }
	delete iMainWindow;
	delete iTimeout;
    TRACES("CSplashWsClient::~CSplashWsClient(): End");
	}

/****************************************************************************\
|	Function:	CSplashWsClient::ConstructMainWindowL()
|				Called by base class's ConstructL
|	Purpose:	Allocates and creates all the windows owned by this client
|				(See list of windows in CSplashWsCLient declaration).
\****************************************************************************/

void CSplashWsClient::ConstructMainWindowL()
	{
    TRACES("CSplashWsClient::ConstructMainWindowL(): Start");

	TSize windowSize=iScreen->SizeInPixels();

    CMainWindow* window = new (ELeave) CMainWindow(this);
    CleanupStack::PushL( window );
	window->ConstructL(TRect(TPoint(0,0), windowSize));
	window->Client()->Group().SetOrdinalPosition(0,ECoeWinPriorityAlwaysAtFront + 1);	// in front of the Status Bar
    delete iMainWindow;
    iMainWindow = window;
    CleanupStack::Pop( window );

    if  ( iTimeout == NULL )
        {
	    iTimeout = new (ELeave) CTimeout;
	    iTimeout->ConstructL(iMainWindow);
	    iTimeout->After(KSplashTimeout);
        }
    else
        {
        iTimeout->SetMainWindow( window );
        }

    TRACES("CSplashWsClient::ConstructMainWindowL(): End");
	}


/****************************************************************************\
|	Function:	CSplashWsClient::RunL()
|				Called by active scheduler when an even occurs
|	Purpose:	Processes events according to their type
|				For key events: calls HandleKeyEventL() (global to client)
|				For pointer event: calls HandlePointerEvent() for window
|                                  event occurred in.
\****************************************************************************/
void CSplashWsClient::RunL()
	{
    TRACES("CSplashWsClient::RunL(): Start");

    TInt state;
    RProperty::Get( KPSUidStartup, KPSSplashShutdown, state );
    TRACES1("CSplashWsClient::RunL(): KPSSplashShutdown state = %d", state);

    if ( state != iPSRemoveSplashState )
        {
        TRACES("CSplashWsClient::RunL(): KPSSplashShutdown state has changed -> PS event");
        if ( state == ESplashShutdown )
            {
            TRACES("CSplashWsClient::RunL(): Exit requested");
            Exit();
            }
        }
    else
        {
        TRACES("CSplashWsClient::RunL(): WS event");

        // get the event
	    iWs.GetEvent(iWsEvent);
	    const TInt eventType = iWsEvent.Type();

        // take action on it
	    switch (eventType)
		    {
    		// window-group related event types
    		case EEventKey:
	    		{
                TRACES("CSplashWsClient::RunL(): EEventKey");
			    TKeyEvent& keyEvent=*iWsEvent.Key(); // get key event
    			HandleKeyEventL (keyEvent);
	            break;
    			}
            // window related events
    		case EEventPointer:
	    		{
                TRACES("CSplashWsClient::RunL(): EEventPointer");
			    CWindow* window=(CWindow*)(iWsEvent.Handle()); // get window
			    TPointerEvent& pointerEvent=*iWsEvent.Pointer();
			    window->HandlePointerEvent (pointerEvent);
    			break;
	    		}
            case EEventScreenDeviceChanged:
                {
                const TInt currentScreenMode = iScreen->CurrentScreenMode();
                TRACES2("CSplashWsClient::RunL() - EEventScreenDeviceChanged - iLastScreenMode: %d, currentScreenMode: %d", iLastScreenMode, currentScreenMode);
                if  ( iLastScreenMode != currentScreenMode )
                    {
                    RDebug::Printf("[SS] CSplashWsClient::RunL() - EEventScreenDeviceChanged - real screen mode change detected!!!!");
                    iScreen->SetAppScreenMode( currentScreenMode );
                    TPixelsTwipsAndRotation currentRot;
                    iScreen->GetScreenModeSizeAndRotation( currentScreenMode, currentRot );
                    iMainWindow->Window().SetExtent( TPoint(0, 0), currentRot.iPixelSize );
	                iMainWindow->Client()->Group().SetOrdinalPosition(0, ECoeWinPriorityAlwaysAtFront + 10000);	// in front of the Status Bar
                    iMainWindow->Window().Invalidate();

                    iLastScreenMode = currentScreenMode;
                    }
                TRACES("CSplashWsClient::RunL() - EEventScreenDeviceChanged - done");
                }
                break;
		    default:
                TRACES("CSplashWsClient::RunL(): default");
    			break;
	        }
        IssueRequest(); // maintain outstanding request
        }
    TRACES("CSplashWsClient::RunL(): End");
	}


/****************************************************************************\
|	Function:	CSplashWsClient::HandleKeyEventL()
|
|	Purpose:	Processes key events for CSplashWsClient
|				Gets the key code from the key event.  Exits on 'Escape'
\****************************************************************************/
void CSplashWsClient::HandleKeyEventL (TKeyEvent& /*aKeyEvent*/)
    {
    TRACES("CSplashWsClient::HandleKeyEventL(): Start");
    TRACES("CSplashWsClient::HandleKeyEventL(): End");
	}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//					 Main program starts here								//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

LOCAL_C void DoItL()
	{
	// make new client
    TRACES("DoItL(): Start");

	CSplashWsClient* client=new (ELeave) CSplashWsClient; // allocate new client
	CleanupStack::PushL(client); // push, just in case
	client->ConstructL(); // construct and run
	CleanupStack::PopAndDestroy(); // destruct

    TRACES("DoItL(): End");
	}

