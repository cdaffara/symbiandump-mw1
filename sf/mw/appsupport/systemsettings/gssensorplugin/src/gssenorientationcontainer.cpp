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
* Description:  Sensor orientation view container
*
*/


// INCLUDE FILES
#include "gssenorientationcontainer.h" 
#include "gssensorplugin.hrh"
#include "gssensorplugin_debug.h"

#include <aknlists.h>
#include <gssensorpluginrsc.rsg>

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSSenOrientationContainer::CGSSenOrientationContainer
// Default constructor
// ---------------------------------------------------------------------------
//
CGSSenOrientationContainer::CGSSenOrientationContainer( CGSSensorPluginModel* aModel )
    {
    TRACE_( "[GSSensorPlugin] CGSSenOrientationContainer::CGSSenOrientationContainer()" );
    iModel = aModel;
    TRACE_( "[GSSensorPlugin] CGSSenOrientationContainer::CGSSenOrientationContainer() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenOrientationContainer::ConstructL
// Symbian OS two phased constructor
// ---------------------------------------------------------------------------
//
void CGSSenOrientationContainer::ConstructL( const TRect& aRect )
    {
    TRACE_( "[GSSensorPlugin] CGSSenOrientationContainer::ConstructL()" );
    // Has to create listbox before calling BaseConstructL because base class
    // uses iListBox
    iListBox = new( ELeave ) CAknSingleGraphicStyleListBox();
    BaseConstructL( aRect, R_GS_ORIENTATION_VIEW_TITLE, R_GS_ORIENTATION_LBX );
    TRACE_( "[GSSensorPlugin] CGSSenOrientationContainer::ConstructL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenOrientationContainer::~CGSSenOrientationContainer()
// Destructor
// ---------------------------------------------------------------------------
//
CGSSenOrientationContainer::~CGSSenOrientationContainer()
    {
    TRACE_( "[GSSensorPlugin] CGSSenOrientationContainer::~CGSSenOrientationContainer()" );
    }

// ---------------------------------------------------------------------------
// CGSSenOrientationContainer::ConstructListBoxL()
// ---------------------------------------------------------------------------
//
void CGSSenOrientationContainer::ConstructListBoxL( TInt aResLbxId )
    {
    TRACE_( "[GSSensorPlugin] CGSSenOrientationContainer::ConstructListBoxL()" );
    CGSSenBaseContainer::ConstructListBoxL( aResLbxId, R_GS_ORIENTATION_MS );
    TRACE_( "[GSSensorPlugin] CGSSenOrientationContainer::ConstructListBoxL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenOrientationContainer::InteractionId
// ---------------------------------------------------------------------------
//
TInt CGSSenOrientationContainer::InteractionId()
    {
    TRACE_1( "[GSSensorPlugin] CGSSenOrientationContainer::InteractionId() - %i", EGSSenOrientation );
    return EGSSenOrientation;
    }

// ---------------------------------------------------------------------------
// CGSSenOrientationContainer::UpdateCheckboxIconsL
// ---------------------------------------------------------------------------
//
void CGSSenOrientationContainer::UpdateCheckboxIconsL()
    {
    TRACE_( "[GSSensorPlugin] CGSSenOrientationContainer::UpdateCheckboxIconsL()" );
    // Camera
    UpdateCheckboxIconL( KGSOrientationCam );
    // Phone display
    UpdateCheckboxIconL( KGSOrientationPD );
    TRACE_( "[GSSensorPlugin] CGSSenOrientationContainer::UpdateCheckboxIconsL() - return" );
    }
