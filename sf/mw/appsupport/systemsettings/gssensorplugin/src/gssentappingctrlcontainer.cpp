/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Sensor tapping control view container
*
*/


// INCLUDE FILES
#include "gssentappingctrlcontainer.h" 
#include "gssensorplugin.hrh"
#include "gssensorplugin_debug.h"

#include <aknlists.h>
#include <gssensorpluginrsc.rsg>

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSSenTappingCtrlContainer::CGSSenTappingCtrlContainer
// Default constructor
// ---------------------------------------------------------------------------
//
CGSSenTappingCtrlContainer::CGSSenTappingCtrlContainer( 
    CGSSensorPluginModel* aModel, 
    MGSSensorMskObserver& aMskObserver ) : 
    CGSSenBaseContainer( aMskObserver )
    {
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlContainer::CGSSenTappingCtrlContainer()" );
    iModel = aModel;
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlContainer::CGSSenTappingCtrlContainer() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenTappingCtrlContainer::ConstructL
// Symbian OS two phased constructor
// ---------------------------------------------------------------------------
//
void CGSSenTappingCtrlContainer::ConstructL( const TRect& aRect )
    {
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlContainer::ConstructL()" );
    // Has to create listbox before calling BaseConstructL because base class
    // uses iListBox
    iListBox = new( ELeave ) CAknSingleGraphicStyleListBox();
    BaseConstructL( aRect, R_GS_TAPCTRL_VIEW_TITLE, R_GS_TAPPING_CONTROL_LBX );
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlContainer::ConstructL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenTappingCtrlContainer::~CGSSenTappingCtrlContainer()
// Destructor
// ---------------------------------------------------------------------------
//
CGSSenTappingCtrlContainer::~CGSSenTappingCtrlContainer()
    {
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlContainer::~CGSSenTappingCtrlContainer()" );
    }

// ---------------------------------------------------------------------------
// CGSSenTappingCtrlContainer::ConstructListBoxL
// ---------------------------------------------------------------------------
//
void CGSSenTappingCtrlContainer::ConstructListBoxL( TInt aResLbxId )
    {
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlContainer::ConstructListBoxL()" );
    CGSSenBaseContainer::ConstructListBoxL( aResLbxId, R_GS_TAPPING_CONTROL_MS );
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlContainer::ConstructListBoxL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenTappingCtrlContainer::InteractionId
// ---------------------------------------------------------------------------
//
TInt CGSSenTappingCtrlContainer::InteractionId()
    {
    TRACE_1( "[GSSensorPlugin] CGSSenTappingCtrlContainer::InteractionId() - %i", EGSSenTapping );
    return EGSSenTapping;
    }

// ---------------------------------------------------------------------------
// CGSSenTappingCtrlContainer::UpdateCheckboxIconsL
// ---------------------------------------------------------------------------
//
void CGSSenTappingCtrlContainer::UpdateCheckboxIconsL()
    {
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlContainer::UpdateCheckboxIconsL()" );
    // Silence Call
    UpdateCheckboxIconL( KGSTapCtrlSC );
    // Silence Alarm
    UpdateCheckboxIconL( KGSTapCtrlSA );
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlContainer::UpdateCheckboxIconsL() - return" );
    }
