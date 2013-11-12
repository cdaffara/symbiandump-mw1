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
#include "t_oomdummyappAppView.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Ct_oomdummyappAppView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
Ct_oomdummyappAppView* Ct_oomdummyappAppView::NewL(const TRect& aRect)
	{
	Ct_oomdummyappAppView* self = Ct_oomdummyappAppView::NewLC(aRect);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// Ct_oomdummyappAppView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
Ct_oomdummyappAppView* Ct_oomdummyappAppView::NewLC(const TRect& aRect)
	{
	Ct_oomdummyappAppView* self = new (ELeave) Ct_oomdummyappAppView;
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	return self;
	}

// -----------------------------------------------------------------------------
// Ct_oomdummyappAppView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void Ct_oomdummyappAppView::ConstructL(const TRect& aRect)
	{
	// Create a window for this application view
	CreateWindowL();

	// Set the windows size
	SetRect(aRect);

	// Activate the window, which makes it ready to be drawn
	ActivateL();
	}

// -----------------------------------------------------------------------------
// Ct_oomdummyappAppView::Ct_oomdummyappAppView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
Ct_oomdummyappAppView::Ct_oomdummyappAppView()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// Ct_oomdummyappAppView::~Ct_oomdummyappAppView()
// Destructor.
// -----------------------------------------------------------------------------
//
Ct_oomdummyappAppView::~Ct_oomdummyappAppView()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// Ct_oomdummyappAppView::Draw()
// Draws the display.
// -----------------------------------------------------------------------------
//
void Ct_oomdummyappAppView::Draw(const TRect& /*aRect*/) const
	{
	// Get the standard graphics context
	CWindowGc& gc = SystemGc();

	// Gets the control's extent
	TRect drawRect(Rect());

	// Clears the screen
	gc.Clear(drawRect);

	}

// -----------------------------------------------------------------------------
// Ct_oomdummyappAppView::SizeChanged()
// Called by framework when the view size is changed.
// -----------------------------------------------------------------------------
//
void Ct_oomdummyappAppView::SizeChanged()
	{
	DrawNow();
	}
// End of File
