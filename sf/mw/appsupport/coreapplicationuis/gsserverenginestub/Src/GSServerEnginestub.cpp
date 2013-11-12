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
* Description:  GSServer is responsible for initializing the TV
*                driver during the boot and always when the cable is connected.
*
*/


// INCLUDE FILES
#include "GSServerEngine.h"

#include <e32svr.h>
#include <settingsinternalcrkeys.h>
#include <centralrepository.h>
#include <data_caging_path_literals.hrh>
#include <tvoutconfig.h>
#include <f32file.h>

// EXTERNAL DATA STRUCTURES
            
// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS


// LOCAL CONSTANTS AND MACROS


// Helper macro for ReadSettings method, cleans up the code.
// Calls the given function f, which return error code.
// If error code is anything else than KErrNone, returns with the error code.
#define RETURN_IF_ERROR(f) \
    { \
    TInt _____err = f; \
    if (_____err != KErrNone)  \
        { \
        return _____err; \
        } \
    } \

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS


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
    //__GSLOGSTRING( "[CGSServerEngine] ConstructL()" );
    }


// -----------------------------------------------------------------------------
// CGSServerEngine::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CGSServerEngine* CGSServerEngine::NewL()
    {
    //__GSLOGSTRING( "[CGSServerEngine] NewL()" );
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
    //__GSLOGSTRING( "[CGSServerEngine] NewLC()" );
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
    //__GSLOGSTRING( "[CGSServerEngine] ~CGSServerEngine()" );

    }


// -----------------------------------------------------------------------------
// CGSServerEngine::TvSystem
// Get the value from the Central repository.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CGSServerEngine::TvSystemL()
    {
    //__GSLOGSTRING( "[CGSServerEngine] TvSystemL()" );
    return 0;
    }


// -----------------------------------------------------------------------------
// CGSServerEngine::SetTvSystem
// Set the value to the Central repository and initialize the driver.
// -----------------------------------------------------------------------------
//
EXPORT_C void CGSServerEngine::SetTvSystemL( const TInt /*aTvSystem*/ )
    {
    //__GSLOGSTRING( "[CGSServerEngine] SetTvSystemL()" );
    }


// -----------------------------------------------------------------------------
// CGSServerEngine::AspectRatioL
// Get the value from the Central repository.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CGSServerEngine::AspectRatioL()
    {
    //__GSLOGSTRING( "[CGSServerEngine] AspectRatioL()" );
    return 0;
    }


// -----------------------------------------------------------------------------
// CGSServerEngine::SetAspectRatioL
// Set the value to the Central repository and initialize the driver.
// -----------------------------------------------------------------------------
//
EXPORT_C void CGSServerEngine::SetAspectRatioL( const TInt /*aRatio*/ )
    {
    //__GSLOGSTRING( "[CGSServerEngine] SetAspectRatioL()" );

    }
    

// -----------------------------------------------------------------------------
// CGSServerEngine::FlickerFilter
// Get the value from the Central repository.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CGSServerEngine::FlickerFilterL()
    {
    //__GSLOGSTRING( "[CGSServerEngine] FlickerFilterL()" );
    
    return 0;
    }


// -----------------------------------------------------------------------------
// CGSServerEngine::SetFlickerFilter
// Set the value to the Central repository and initialize the driver.
// -----------------------------------------------------------------------------
//
EXPORT_C void CGSServerEngine::SetFlickerFilterL( TInt /*aFlickerFilter*/ )
    {
    //__GSLOGSTRING( "[CGSServerEngine] SetFlickerFilterL()" );
                                                    
    }
    

// ----------------------------------------------------------------------------
// CGSServerEngine::SwitchValue
// 
// Switching value from 1 to 0 and back
// ----------------------------------------------------------------------------
//
void CGSServerEngine::SwitchValue( TInt& /*aValue*/ )
    {
    //__GSLOGSTRING( "[CGSServerEngine] SwitchValue()" );

    }


// ----------------------------------------------------------------------------
// CGSServerEngine::ConvertAspectRatio
// 
// 
// ----------------------------------------------------------------------------
//
void CGSServerEngine::ConvertAspectRatio( TInt& /*aValue*/ )
    {

    }


// ----------------------------------------------------------------------------
// CGSServerEngine::ConvertTvSystem
// 
// 
// ----------------------------------------------------------------------------
//
void CGSServerEngine::ConvertTvSystem( TInt& /*aValue*/ )
    {

    }


//  End of File  
