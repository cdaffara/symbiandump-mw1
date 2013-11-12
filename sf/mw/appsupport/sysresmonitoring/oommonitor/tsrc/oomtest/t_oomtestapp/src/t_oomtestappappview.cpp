/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/




// INCLUDE FILES
#include <coemain.h>
#include <eikenv.h>
#include <aknutils.h>
#include "t_oomtestappAppUi.h"
#include "t_oomtestappAppView.h"

_LIT( KTextSystemOn, "System on" );
_LIT( KTextSystemOff, "System off" );
const TInt KTextBufferSize = 100;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Ct_oomtestappAppView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
Ct_oomtestappAppView* Ct_oomtestappAppView::NewL(const TRect& aRect, Ct_oomtestappAppUi& aAppUi)
	{
	Ct_oomtestappAppView* self = new ( ELeave ) Ct_oomtestappAppView( aAppUi );
	CleanupStack::PushL(self);
	self->ConstructL( aRect );
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// Ct_oomtestappAppView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void Ct_oomtestappAppView::ConstructL(const TRect& aRect)
	{
	// Create a window for this application view
	CreateWindowL();

	// Set the windows size
	SetRect(aRect);

    iFont = AknLayoutUtils::FontFromId( EAknLogicalFontPrimarySmallFont );

	// Activate the window, which makes it ready to be drawn
	ActivateL();
	}

// -----------------------------------------------------------------------------
// Ct_oomtestappAppView::Ct_oomtestappAppView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
Ct_oomtestappAppView::Ct_oomtestappAppView( Ct_oomtestappAppUi& aAppUi ) :
    iAppUi( aAppUi )
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// Ct_oomtestappAppView::~Ct_oomtestappAppView()
// Destructor.
// -----------------------------------------------------------------------------
//
Ct_oomtestappAppView::~Ct_oomtestappAppView()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// Ct_oomtestappAppView::Draw()
// Draws the display.
// -----------------------------------------------------------------------------
//
void Ct_oomtestappAppView::Draw(const TRect& /*aRect*/) const
	{
	// Get the standard graphics context
	CWindowGc& gc = SystemGc();

	// Gets the control's extent
	TRect drawRect(Rect());

	// Clears the screen
	gc.Clear(drawRect);
	
    gc.UseFont(iFont);
    
    TBuf<KTextBufferSize> buffer;
    TInt allocSize( iAppUi.AllocatedHeap() / KKiloByte );
    TPtrC priority( iAppUi.Priority() );
	if ( CEikonEnv::Static()->IsSystem() )
	    {
        buffer.Format( _L("%S : Heap %d KB : Priority %S"),
            &(KTextSystemOn()), allocSize, &priority );
	    }
    else
        {
        buffer.Format( _L("%S : Heap %d KB : Priority %S"),
            &(KTextSystemOff()), allocSize, &priority );
        }
    TPoint textPos( 0, drawRect.Height() / 2 );
    gc.DrawText( buffer, textPos );
    gc.DiscardFont();
	}

// -----------------------------------------------------------------------------
// Ct_oomtestappAppView::SizeChanged()
// Called by framework when the view size is changed.
// -----------------------------------------------------------------------------
//
void Ct_oomtestappAppView::SizeChanged()
	{
	DrawDeferred();
	}

// End of File
