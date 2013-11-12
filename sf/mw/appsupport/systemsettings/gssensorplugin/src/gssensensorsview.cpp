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
* Description:  Sensors active status view
*
*/


// INCLUDE FILES
#include "gssensensorsview.h"
#include "gssensensorscontainer.h"
#include "gssensorplugin.hrh"
#include "gssensorpluginlocalviewids.h"
#include "gssensorplugin_debug.h"

#include <hlplch.h>             // For HlpLauncher
#include <AknViewAppUi.h>
#include <AknRadioButtonSettingPage.h>
#include <FeatMgr.h>
#include <gssensorpluginrsc.rsg>
#include <GSCommon.hrh>


// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSSenSensorsView::CGSSenSensorsView
// Default constructor
// ---------------------------------------------------------------------------
//
CGSSenSensorsView::CGSSenSensorsView( CGSSensorPluginModel* aModel ) :
    iModel( aModel )
    {
    TRACE_( "[GSSensorPlugin] CGSSenSensorsView::CGSSenSensorsView()" );
    }

// ---------------------------------------------------------------------------
// CGSSenSensorsView::NewL
// Symbian OS two-phased constructor
// ---------------------------------------------------------------------------
CGSSenSensorsView* CGSSenSensorsView::NewL( CGSSensorPluginModel* aModel )
    {
    TRACE_( "[GSSensorPlugin] CGSSenSensorsView::NewL()" );
    CGSSenSensorsView* self = NewLC( aModel );
    CleanupStack::Pop( self );
    TRACE_( "[GSSensorPlugin] CGSSenSensorsView::NewL() - return" );
    return self;
    }

// ---------------------------------------------------------------------------
// CGSSenSensorsView::NewLC
// Symbian OS two-phased constructor
// ---------------------------------------------------------------------------
CGSSenSensorsView* CGSSenSensorsView::NewLC( CGSSensorPluginModel* aModel )
    {
    TRACE_( "[GSSensorPlugin] CGSSenSensorsView::NewLC()" );
    CGSSenSensorsView* self = new( ELeave ) CGSSenSensorsView( aModel );
    CleanupStack::PushL( self );
    self->ConstructL();
    TRACE_( "[GSSensorPlugin] CGSSenSensorsView::NewLC() - return" );
    return self;
    }

// ---------------------------------------------------------------------------
// CGSSenSensorsView::~CGSSenSensorsView
// Destructor
// ---------------------------------------------------------------------------
CGSSenSensorsView::~CGSSenSensorsView()
    {
    TRACE_( "[GSSensorPlugin] CGSSenSensorsView::~CGSSenSensorsView()" );
    iModel = NULL;
    }

// ---------------------------------------------------------------------------
// CGSSenSensorsView::ConstructL
// Symbian OS two-phased constructor
// ---------------------------------------------------------------------------
void CGSSenSensorsView::ConstructL()
    {
    TRACE_( "[GSSensorPlugin] CGSSenSensorsView::ConstructL()" );
    BaseConstructL( R_GS_SENSORS_VIEW );
    TRACE_( "[GSSensorPlugin] CGSSenSensorsView::ConstructL() - return" );
    }

// ---------------------------------------------------------------------------
// TUid CGSSenSensorsView::Id
// ---------------------------------------------------------------------------
TUid CGSSenSensorsView::Id() const
    {
    TRACE_1( "[GSSensorPlugin] CGSSenSensorsView::Id() - ID: %i", KSenSensorsViewId );
    return KSenSensorsViewId;
    }

// ---------------------------------------------------------------------------
// CGSSenSensorsView::HandleCommandL
// Command handling for sensor activation status view
// ---------------------------------------------------------------------------
void CGSSenSensorsView::HandleCommandL( TInt aCommand )
    {
    TRACE_1( "[GSSensorPlugin] CGSSenSensorsView::HandleCommandL() - command: %i", 
        aCommand );

    switch ( aCommand )
        {
        case EGSMSKCmdAppChange:
        case EGSSenMskGeneral:
            HandleListBoxSelectionL();
            break;
        case EAknSoftkeyBack:
            iAppUi->ActivateLocalViewL( KGSSensorPluginUid );
            break;
        case EAknCmdHelp:
            {
            if( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                {
                HlpLauncher::LaunchHelpApplicationL(
                    iEikonEnv->WsSession(), iAppUi->AppHelpContextL() );
                }
            break;
            }
        default:
            iAppUi->HandleCommandL( aCommand );
            break;
        }
    TRACE_( "[GSSensorPlugin] CGSSenSensorsView::HandleCommandL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenSensorsView::UpdateListBoxL
// ---------------------------------------------------------------------------
//
void CGSSenSensorsView::UpdateListBoxL( TInt aItemId )
    {
    TRACE_1( "[GSSensorPlugin] CGSSenSensorsView::UpdateListBoxL() - ItemId: %i", aItemId );
    Container()->UpdateListBoxL( aItemId );
    TRACE_( "[GSSensorPlugin] CGSSenSensorsView::UpdateListBoxL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenSensorsView::Container
// Returns call container item
// ---------------------------------------------------------------------------
//
CGSSenSensorsContainer* CGSSenSensorsView::Container()
    {
    TRACE_( "[GSSensorPlugin] CGSSenSensorsView::Container()" );
    return static_cast<CGSSenSensorsContainer*>( iContainer );
    }


// ---------------------------------------------------------------------------
// CGSSenSensorsView::NewContainerL
// Creates new iContainer.
// ---------------------------------------------------------------------------
//
void CGSSenSensorsView::NewContainerL()
    {
    TRACE_( "[GSSensorPlugin] CGSSenSensorsView::NewContainerL()" );
    iContainer = new ( ELeave ) CGSSenSensorsContainer( iModel );
    TRACE_( "[GSSensorPlugin] CGSSenSensorsView::NewContainerL() - return" );
    }


// ---------------------------------------------------------------------------
// CGSSenSensorsView::HandleListBoxSelectionL()
// ---------------------------------------------------------------------------
void CGSSenSensorsView::HandleListBoxSelectionL() 
    {
    TRACE_( "[GSSensorPlugin] CGSSenSensorsView::HandleListBoxSelectionL()" );
    const TInt currentItem = Container()->CurrentFeatureId();

    switch ( currentItem )
        {
        case KGSSenorsActive:
            ChangeSensorActiveSettingL();
            break;
        default:
            break;
        }
    TRACE_( "[GSSensorPlugin] CGSSenSensorsView::HandleListBoxSelectionL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenSensorsView::ChangeSensorActiveSettingL
// Change sensors active status setting .
// ---------------------------------------------------------------------------
//
void CGSSenSensorsView::ChangeSensorActiveSettingL()
    {
    TRACE_( "[GSSensorPlugin] CGSSenSensorsView::ChangeSensorActiveSettingL()" );
    TInt currentValue = iModel->SensorActiveStatusL();
    
    if( currentValue == KGSSenOff )
        {
        currentValue = KGSSenOn;
        }
    else
        {
        currentValue = KGSSenOff;
        }

    iModel->SetSensorActiveStatusL( currentValue );
    UpdateListBoxL( KGSSenorsActive );

    TRACE_( "[GSSensorPlugin] CGSSenSensorsView::ChangeSensorActiveSettingL() - return" );
    }
