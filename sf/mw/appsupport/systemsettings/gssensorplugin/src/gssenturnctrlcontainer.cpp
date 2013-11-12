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
* Description:  Sensor turn control view container
*
*/


// INCLUDE FILES
#include "gssenturnctrlcontainer.h" 
#include "gssensorplugin.hrh"
#include "gssensorplugin_debug.h"

#include <aknlists.h>
#include <gssensorpluginrsc.rsg>


// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSSenTurnCtrlContainer::CGSSenTurnCtrlContainer
// Default constructor
// ---------------------------------------------------------------------------
//
CGSSenTurnCtrlContainer::CGSSenTurnCtrlContainer( 
    CGSSensorPluginModel* aModel, 
    MGSSensorMskObserver& aMskObserver ) : 
    CGSSenBaseContainer( aMskObserver )
    {
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlContainer::CGSSenTurnCtrlContainer()" );
    iModel = aModel;
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlContainer::CGSSenTurnCtrlContainer() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenTurnCtrlContainer::ConstructL
// Symbian OS two phased constructor
// ---------------------------------------------------------------------------
//
void CGSSenTurnCtrlContainer::ConstructL( const TRect& aRect )
    {
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlContainer::ConstructL()" );
    // Has to create listbox before calling BaseConstructL because base class
    // uses iListBox
    iListBox = new( ELeave ) CAknSingleGraphicStyleListBox();
    BaseConstructL( aRect, R_GS_TURNCTRL_VIEW_TITLE, R_GS_TURNING_CONTROL_LBX );
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlContainer::ConstructL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenTurnCtrlContainer::~CGSSenTurnCtrlContainer()
// Destructor
// ---------------------------------------------------------------------------
//
CGSSenTurnCtrlContainer::~CGSSenTurnCtrlContainer()
    {
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlContainer::~CGSSenTurnCtrlContainer()" );
    }

// ---------------------------------------------------------------------------
// CGSSenTurnCtrlContainer::ConstructListBoxL()
// ---------------------------------------------------------------------------
//
void CGSSenTurnCtrlContainer::ConstructListBoxL( TInt aResLbxId )
    {
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlContainer::ConstructListBoxL()" );
    CGSSenBaseContainer::ConstructListBoxL( aResLbxId, R_GS_TURNING_CONTROL_MS );
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlContainer::ConstructListBoxL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenTurnCtrlContainer::InteractionId
// ---------------------------------------------------------------------------
//
TInt CGSSenTurnCtrlContainer::InteractionId()
    {
    TRACE_1( "[GSSensorPlugin] CGSSenTurnCtrlContainer::InteractionId() - %i", EGSSenTurning );
    return EGSSenTurning;
    }

// ---------------------------------------------------------------------------
// CGSSenTurnCtrlContainer::UpdateCheckboxIconsL
// ---------------------------------------------------------------------------
//
void CGSSenTurnCtrlContainer::UpdateCheckboxIconsL()
    {
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlContainer::UpdateCheckboxIconsL()" );
    // Silence Call
    UpdateCheckboxIconL( KGSTurnCtrlSC );
    // Silence Alarm
    UpdateCheckboxIconL( KGSTurnCtrlSA );
    // Display orientation
    UpdateCheckboxIconL( KGSDisplayOrientation );
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlContainer::UpdateCheckboxIconsL() - return" );
    }
