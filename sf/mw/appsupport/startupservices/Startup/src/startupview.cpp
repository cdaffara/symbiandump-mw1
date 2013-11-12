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
* Description:  Implementation of CStartupView class
*
*/




#include "startupview.h"
#include "StartupDefines.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CStartupView::NewL
//
// ---------------------------------------------------------------------------
//
CStartupView* CStartupView::NewL( const TRect& aRect )
    {
    TRACES("CStartupView::NewL()");

    CStartupView* self = new( ELeave ) CStartupView;
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    CleanupStack::Pop( self );

    TRACES("CStartupView::NewL(): End");
    return self;
    }


// ---------------------------------------------------------------------------
// CStartupView::~CStartupView
//
// ---------------------------------------------------------------------------
//
CStartupView::~CStartupView()
    {
    TRACES("CStartupView::~CStartupView()");
    
    TRACES("CStartupView::~CStartupView(): End");
    }


// ---------------------------------------------------------------------------
// CStartupView::SetComponent
//
// ---------------------------------------------------------------------------
//
void CStartupView::SetComponent( CCoeControl& aComponent )
    {
    TRACES("CStartupView::SetComponent()");

    iComponent = &aComponent;

    TRACES("CStartupView::SetComponent(): End");
    }


// ---------------------------------------------------------------------------
// CStartupView::RemoveComponent
//
// ---------------------------------------------------------------------------
//
void CStartupView::RemoveComponent()
    {
    TRACES("CStartupView::RemoveComponent()");

    iComponent = NULL;

    TRACES("CStartupView::RemoveComponent(): End");
    }


// ---------------------------------------------------------------------------
// CStartupView::SizeChanged
//
// ---------------------------------------------------------------------------
void CStartupView::SizeChanged()
    {
    TRACES("CStartupView::SizeChanged()");
    if ( iComponent )
        {
        iComponent->SetRect( Rect() );
        }

    DrawNow();

    TRACES("CStartupView::SizeChanged(): End");
    }


// ---------------------------------------------------------------------------
// CStartupView::CountComponentControls
//
// ---------------------------------------------------------------------------
//
TInt CStartupView::CountComponentControls() const
    {
    if ( iComponent ) return 1;
    return 0;
    }


// ---------------------------------------------------------------------------
// CStartupView::ComponentControl
//
// ---------------------------------------------------------------------------
//
CCoeControl* CStartupView::ComponentControl( TInt /*aIndex*/ ) const
    {
    return iComponent;
    }


// ---------------------------------------------------------------------------
// CStartupView::Draw
//
// ---------------------------------------------------------------------------
//
void CStartupView::Draw( const TRect& aRect ) const
    {
    TRACES("CStartupView::Draw()");

    CWindowGc& gc = SystemGc();
 
    gc.SetPenStyle( CGraphicsContext::ENullPen );
    gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc.SetClippingRect( aRect );
    gc.SetBrushColor( KRgbWhite );
    gc.Clear();
    TRACES("CStartupView::Draw(): End");
    }

// ---------------------------------------------------------------------------
// CStartupView::CStartupView
//
// ---------------------------------------------------------------------------
//
CStartupView::CStartupView()
    {
    TRACES("CStartupView::CStartupView()");
    TRACES("CStartupView::CStartupView(): End");
    }

// ---------------------------------------------------------------------------
// CStartupView::ConstructL
//
// ---------------------------------------------------------------------------
//
void CStartupView::ConstructL( const TRect& aRect )
    {
    TRACES("CStartupView::ConstructL()");
    CreateWindowL();
    SetRect( aRect );

	// Create background drawing context    
    TRect bgrect(aRect.Size());
    ActivateL();
    TRACES("CStartupView::ConstructL(): End");
    }
