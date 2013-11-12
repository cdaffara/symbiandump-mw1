/*
 * Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
 * Description:  GSServer is responsible for initializing the TV
 *                driver during the boot and always when the cable is connected.
 *
 */

// INCLUDE FILES
#include "GSLogger.h"
#include "GSServerEngine.h"
#include <AccessoriesCRKeys.h>

#include <e32svr.h>
#include <centralrepository.h>
#include <data_caging_path_literals.hrh>
#include <f32file.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS


// LOCAL CONSTANTS AND MACROS

//For SwitchValue()
const TInt KGSIndexOff = 1;
const TInt KGSIndexOn = 0;
const TInt KGSSettingOff = 0;
const TInt KGSSettingOn = 1;
//For aspect ratio conversion
const TInt KGSTvAspectNormal = 0;
const TInt KGSTvAspectWide = 1;
//For tv system value conversion
const TInt KGSTvSystemPal = 0;
const TInt KGSTvSystemPalm = 1;
const TInt KGSTvSystemNtsc = 2;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CGSServerEngine::CGSServerEngine
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CGSServerEngine::CGSServerEngine()
    {
    }

// -----------------------------------------------------------------------------
// CGSServerEngine::ConstructL
// Create the display driver and central repository.
// -----------------------------------------------------------------------------
//
void CGSServerEngine::ConstructL()
    {
    __GSLOGSTRING( "[CGSServerEngine] ConstructL()" );
    iRepository = CRepository::NewL( KCRUidTvoutSettings );
    }

// -----------------------------------------------------------------------------
// CGSServerEngine::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CGSServerEngine* CGSServerEngine::NewL()
    {
    __GSLOGSTRING( "[CGSServerEngine] NewL()" );
    CGSServerEngine* self = CGSServerEngine::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CGSServerEngine::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CGSServerEngine* CGSServerEngine::NewLC()
    {
    __GSLOGSTRING( "[CGSServerEngine] NewLC()" );
    CGSServerEngine* self = new ( ELeave ) CGSServerEngine();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CGSServerEngine::NewLC
// Destructor, close and release the handles.
// -----------------------------------------------------------------------------
//
EXPORT_C CGSServerEngine::~CGSServerEngine()
    {
    __GSLOGSTRING( "[CGSServerEngine] ~CGSServerEngine()" );
    delete iRepository;
    }

// -----------------------------------------------------------------------------
// CGSServerEngine::TvSystem
// Get the value from the Central repository.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CGSServerEngine::TvSystemL()
    {
    __GSLOGSTRING( "[CGSServerEngine] TvSystemL()" );
    TInt value;
    User::LeaveIfError( iRepository->Get( KSettingsTvSystemInfo, value ) );

    return value;
    }

// -----------------------------------------------------------------------------
// CGSServerEngine::SetTvSystem
// Set the value to the Central repository and initialize the driver.
// -----------------------------------------------------------------------------
//
EXPORT_C void CGSServerEngine::SetTvSystemL( const TInt aTvSystem )
    {
    __GSLOGSTRING( "[CGSServerEngine] SetTvSystemL()" );
    TInt err = iRepository->Set( KSettingsTvSystemInfo, aTvSystem );
    User::LeaveIfError( err );
    }

// -----------------------------------------------------------------------------
// CGSServerEngine::AspectRatioL
// Get the value from the Central repository.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CGSServerEngine::AspectRatioL()
    {
    __GSLOGSTRING( "[CGSServerEngine] AspectRatioL()" );
    TInt value;
    User::LeaveIfError( iRepository->Get( KSettingsTvAspectRatio, value ) );

    return value;
    }

// -----------------------------------------------------------------------------
// CGSServerEngine::SetAspectRatioL
// Set the value to the Central repository and initialize the driver.
// -----------------------------------------------------------------------------
//
EXPORT_C void CGSServerEngine::SetAspectRatioL( const TInt aRatio )
    {
    __GSLOGSTRING( "[CGSServerEngine] SetAspectRatioL()" );
    TInt err = iRepository->Set( KSettingsTvAspectRatio, aRatio );
    User::LeaveIfError( err );
    }

// -----------------------------------------------------------------------------
// CGSServerEngine::FlickerFilter
// Get the value from the Central repository.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CGSServerEngine::FlickerFilterL()
    {
    __GSLOGSTRING( "[CGSServerEngine] FlickerFilterL()" );
    TInt value;
    User::LeaveIfError( iRepository->Get( KSettingsTvoutFlickerFilter, value ) );
    SwitchValue( value );
    return value;
    }

// -----------------------------------------------------------------------------
// CGSServerEngine::SetFlickerFilter
// Set the value to the Central repository and initialize the driver.
// -----------------------------------------------------------------------------
//
EXPORT_C void CGSServerEngine::SetFlickerFilterL( TInt aFlickerFilter )
    {
    __GSLOGSTRING( "[CGSServerEngine] SetFlickerFilterL()" );
    SwitchValue( aFlickerFilter );
    TInt err = iRepository->Set( KSettingsTvoutFlickerFilter, aFlickerFilter );
    User::LeaveIfError( err );
    }

// ----------------------------------------------------------------------------
// CGSServerEngine::SwitchValue
// 
// Switching value from 1 to 0 and back
// ----------------------------------------------------------------------------
//
void CGSServerEngine::SwitchValue( TInt& aValue )
    {
    __GSLOGSTRING( "[CGSServerEngine] SwitchValue()" );
    switch( aValue )
        {
        case KGSSettingOff: //0
            aValue = KGSIndexOff; //1
            break;
        case KGSSettingOn: //1
            aValue = KGSIndexOn; //0
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CGSServerEngine::ConvertAspectRatio
// 
// 
// ----------------------------------------------------------------------------
//
void CGSServerEngine::ConvertAspectRatio( TInt& aValue )
    {
    switch( aValue )
        {
        case KGSTvAspectNormal:
            aValue = 1;
            break;
        case KGSTvAspectWide:
            aValue = 2;
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CGSServerEngine::ConvertTvSystem
// 
// 
// ----------------------------------------------------------------------------
//
void CGSServerEngine::ConvertTvSystem( TInt& aValue )
    {
    switch( aValue )
        {
        case KGSTvSystemPal:
            aValue = 3;
            break;
        case KGSTvSystemPalm:
            aValue = 4;
            break;
        case KGSTvSystemNtsc:
            aValue = 1;
        default:
            break;
        }
    }

//  End of File  
