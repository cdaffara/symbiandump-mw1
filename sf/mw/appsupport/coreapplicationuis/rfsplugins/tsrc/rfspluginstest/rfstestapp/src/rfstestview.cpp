/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CRfsTestView class
*
*/


#include "rfstestview.h"
#include "trace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CRfsTestView::NewL
// ---------------------------------------------------------------------------
//
CRfsTestView* CRfsTestView::NewL( const TRect& aRect )
    {
    FUNC_LOG;

    CRfsTestView* self = new( ELeave ) CRfsTestView;
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CRfsTestView::~CRfsTestView
// ---------------------------------------------------------------------------
//
CRfsTestView::~CRfsTestView()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CRfsTestView::CRfsTestView
// ---------------------------------------------------------------------------
//
CRfsTestView::CRfsTestView()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CRfsTestView::ConstructL
// ---------------------------------------------------------------------------
//
void CRfsTestView::ConstructL( const TRect& aRect )
    {
    FUNC_LOG;

    CreateWindowL();
    SetRect( aRect );
    ActivateL();
    }


// ---------------------------------------------------------------------------
// CRfsTestView::CountComponentControls
// ---------------------------------------------------------------------------
TInt CRfsTestView::CountComponentControls() const
    {
    FUNC_LOG;

    return 0;
    }


// ---------------------------------------------------------------------------
// CRfsTestView::ComponentControl
// ---------------------------------------------------------------------------
CCoeControl* CRfsTestView::ComponentControl( TInt /*aIndex*/ ) const
    {
    FUNC_LOG;

    return NULL;
    }


// ---------------------------------------------------------------------------
// CRfsTestView::Draw
// ---------------------------------------------------------------------------
//
void CRfsTestView::Draw( const TRect& aRect ) const
    {
    FUNC_LOG;

	CWindowGc& gc = SystemGc();

    gc.Clear( aRect );
    gc.SetPenStyle( CGraphicsContext::ENullPen );
    gc.SetBrushColor( KRgbWhite );
    gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc.DrawRect( aRect );
    }

