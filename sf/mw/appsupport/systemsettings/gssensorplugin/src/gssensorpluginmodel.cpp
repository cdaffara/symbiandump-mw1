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
* Description:  Sensor Settings model.
*
*/


// INCLUDE FILES
#include "gssensorpluginmodel.h"
#include "gssensorplugin.hrh"
#include "gssensorplugin_debug.h"

#include <sensorplugindomaincrkeys.h>

// ========================= MEMBER FUNCTIONS =================================

// ----------------------------------------------------------------------------
// CGSSensorPluginModel::NewL
// ----------------------------------------------------------------------------
//
CGSSensorPluginModel* CGSSensorPluginModel::NewL()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginModel::NewL()" );
    CGSSensorPluginModel* self = new( ELeave ) CGSSensorPluginModel;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    TRACE_( "[GSSensorPlugin] CGSSensorPluginModel::NewL() - return" );
    return self;
    }

// ----------------------------------------------------------------------------
// CGSSensorPluginModel::CGSSensorPluginModel
// ----------------------------------------------------------------------------
//
CGSSensorPluginModel::CGSSensorPluginModel()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginModel::CGSSensorPluginModel()" );
    iChangesMade = EFalse;
    }

// ----------------------------------------------------------------------------
// CGSSensorPluginModel::ConstructL
// ----------------------------------------------------------------------------
//
void CGSSensorPluginModel::ConstructL()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginModel::ConstructL()" );
    // Connect to Sensor Settings repository
    iSensorRepository = CRepository::NewL( KCRUidSensorSettings );
    TRACE_( "[GSSensorPlugin] CGSSensorPluginModel::ConstructL() - return" );
    }

// ----------------------------------------------------------------------------
// CGSSensorPluginModel::~CGSSensorPluginModel
// ----------------------------------------------------------------------------
//
CGSSensorPluginModel::~CGSSensorPluginModel()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginModel::~CGSSensorPluginModel()" );
    if ( iSensorRepository )
        {
        delete iSensorRepository;
        }
    TRACE_( "[GSSensorPlugin] CGSSensorPluginModel::~CGSSensorPluginModel() - return" );
    }

// ----------------------------------------------------------------------------
// CGSSensorPluginModel::SensorActiveStatusL
// ----------------------------------------------------------------------------
//
TInt CGSSensorPluginModel::SensorActiveStatusL()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginModel::SensorActiveStatusL()" );
    TInt mode( 0 );
    // Get the sensors active status value from CenRep.
    User::LeaveIfError( iSensorRepository->Get( KSenSettingsSensorsOn, mode ) );
    TRACE_1( "[GSSensorPlugin] CGSSensorPluginModel::SensorActiveStatusL() - return: %i", mode );
    return mode;
    }

// ----------------------------------------------------------------------------
// CGSSensorPluginModel::SetSensorActiveStatusL
// ----------------------------------------------------------------------------
//
void CGSSensorPluginModel::SetSensorActiveStatusL( TInt aStatus )
    {
    TRACE_1( "[GSSensorPlugin] CGSSensorPluginModel::SetSensorActiveStatusL() - %i", aStatus );
    // Update the sensors active status value to CenRep.
    User::LeaveIfError( iSensorRepository->Set( KSenSettingsSensorsOn, aStatus ) );
    TRACE_( "[GSSensorPlugin] CGSSensorPluginModel::SetSensorActiveStatusL() - return");
    }

// ----------------------------------------------------------------------------
// CGSSensorPluginModel::SelectionListL
// ----------------------------------------------------------------------------
//
TUint32 CGSSensorPluginModel::SelectionListL( TInt aInteraction )
    {
    TRACE_1( "[GSSensorPlugin] CGSSensorPluginModel::SelectionListL() - Interaction: %i",
        aInteraction );

    TUint32 keyId( 0 ); // key to central repository
    TInt selections( 0 );

    // Resolve interaction
    switch ( aInteraction )
        {
        case EGSSenTapping:
            {
            keyId = KSenSettingsTapCtrl;
            break;
            }
        case EGSSenTurning:
            {
            keyId = KSenSettingsTurnCtrl;
            break;
            }
        case EGSSenOrientation:
            {
            keyId = KSenSettingsOrientation;
            break;
            }
        default:
            {
            TRACE_( "[GSSensorPlugin] CGSSensorPluginModel::SelectionListL() - Unknown interaction" );
            User::Leave( KErrUnknown );
            }
        }

    // Get interactions selection list from CenRep
    User::LeaveIfError( iSensorRepository->Get( keyId, selections ) );
    TRACE_1( "[GSSensorPlugin] CGSSensorPluginModel::SelectionListL() - return: %b", selections );
    return selections;
    }

// ----------------------------------------------------------------------------
// CGSSensorPluginModel::SelectionListL
// ----------------------------------------------------------------------------
//
TUint32 CGSSensorPluginModel::VariationListL( const TInt aInteraction )
    {
    TRACE_1( "[GSSensorPlugin] CGSSensorPluginModel::VariationListL() - Interaction: %i",
        aInteraction );

    TUint32 keyId( 0 ); // key to central repository
    TInt selections( 0 );

    // Resolve interaction
    switch ( aInteraction )
        {
        case EGSSenTapping:
            {
            keyId = KSenSetVariationTapCtrl;
            break;
            }
        case EGSSenTurning:
            {
            keyId = KSenSetVariationTurnCtrl;
            break;
            }
        case EGSSenOrientation:
        default:
            {
            TRACE_( "[GSSensorPlugin] CGSSensorPluginModel::VariationListL() - Unknown interaction" );
            User::Leave( KErrUnknown );
            }
        }

    // Get interactions variation list from CenRep
    User::LeaveIfError( iSensorRepository->Get( keyId, selections ) );
    TRACE_1( "[GSSensorPlugin] CGSSensorPluginModel::VariationListL() - return: %b", selections );
    return selections;
    }

// ----------------------------------------------------------------------------
// CGSSensorPluginModel::SetSelectionListL
// ----------------------------------------------------------------------------
//
void CGSSensorPluginModel::SetSelectionListL( TInt aInteraction, TInt aList )
    {
    TRACE_2( "[GSSensorPlugin] CGSSensorPluginModel::SetSelectionListL() - Interaction: %i Selections: %b",
        aInteraction, aList );
    TUint32 keyId( 0 ); // key to central repository

    // Resolve interaction
    switch ( aInteraction )
        {
        case EGSSenTapping:
            {
            keyId = KSenSettingsTapCtrl;
            break;
            }
        case EGSSenTurning:
            {
            keyId = KSenSettingsTurnCtrl;
            break;
            }
        case EGSSenOrientation:
            {
            keyId = KSenSettingsOrientation;
            break;
            }
        default:
            {
            TRACE_( "[GSSensorPlugin] CGSSensorPluginModel::SetSelectionListL() - Unknown interaction" );
            User::Leave( KErrUnknown );
            }
        }

    // Set interactions selection list to CenRep
    User::LeaveIfError( iSensorRepository->Set( keyId, aList ) );

    // Set flag on that changes has been made
    iChangesMade = ETrue;

    TRACE_( "[GSSensorPlugin] CGSSensorPluginModel::SetSelectionListL() - return" );
    }

// ----------------------------------------------------------------------------
// CGSSensorPluginModel::ChangesMade
// ----------------------------------------------------------------------------
//
TBool CGSSensorPluginModel::ChangesMade()
    {
    TRACE_1( "[GSSensorPlugin] CGSSensorPluginModel::ChangesMade() - return %i", iChangesMade );
    TBool retVal( iChangesMade );
    iChangesMade = EFalse;
    return retVal;
    }

// ----------------------------------------------------------------------------
// CGSSensorPluginModel::SettingsExists
// ----------------------------------------------------------------------------
//
TBool CGSSensorPluginModel::SettingsExistsL()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginModel::SettingsExists()" );
    TUint32 settingsMask = 0;
    // Get all settings and add them to one bitmask. If even one exists, this function returns ETrue
    settingsMask = SelectionListL( EGSSenTapping );
    settingsMask = settingsMask | SelectionListL( EGSSenTurning );
    settingsMask = settingsMask | SelectionListL( EGSSenOrientation );

    TRACE_1( "[GSSensorPlugin] CGSSensorPluginModel::SettingsExists() - return: %i", settingsMask );
    return settingsMask;
    }
