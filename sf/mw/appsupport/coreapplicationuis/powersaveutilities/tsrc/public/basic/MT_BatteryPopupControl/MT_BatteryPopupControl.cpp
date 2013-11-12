/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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


//  CLASS HEADER
#include "MT_BatteryPopupControl.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <EUnitDecorators.h>
#include "BatteryPopupControl.h"


//  INTERNAL INCLUDES

//CONSTANTS
_LIT(KText, "Test text");
_LIT(KLinkText, "Link text");

// CONSTRUCTION
MT_BatteryPopupControl* MT_BatteryPopupControl::NewL()
    {
    MT_BatteryPopupControl* self = MT_BatteryPopupControl::NewLC();
    CleanupStack::Pop();

    return self;
    }

MT_BatteryPopupControl* MT_BatteryPopupControl::NewLC()
    {
    MT_BatteryPopupControl* self = new( ELeave ) MT_BatteryPopupControl();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
MT_BatteryPopupControl::~MT_BatteryPopupControl()
    {
    }

// Default constructor
MT_BatteryPopupControl::MT_BatteryPopupControl()
    {
    }

// Second phase construct
void MT_BatteryPopupControl::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void MT_BatteryPopupControl::SetupL(  )
    {
    
    }
    

void MT_BatteryPopupControl::Teardown(  )
    {
    
    }
void MT_BatteryPopupControl::ProcessCommandL( TInt /*aCommandId*/ )
    {
    }    
    

void MT_BatteryPopupControl::MT_BatteryPopupControl_NewLL(  )
    {
    HBufC* linkText = KLinkText().AllocLC();
    TBufC<9> bufText(KText);
    TPtr text = bufText.Des();
    
    CBatteryPopupControl* control = CBatteryPopupControl::NewL( text, 
                                                                *linkText );
    EUNIT_ASSERT_DESC(control, "CBatteryPopupControl::NewL( text, link ) - failed");
    CleanupStack::PopAndDestroy( linkText );
    delete control;
    } 
    
void MT_BatteryPopupControl::MT_BatteryPopupControl_SetCommandObserverL(  )
    {
    HBufC* linkText = KLinkText().AllocLC();
    TBufC<9> bufText(KText);
    TPtr text = bufText.Des();
    CBatteryPopupControl* control = CBatteryPopupControl::NewL( text, 
                                                                *linkText );
    EUNIT_ASSERT_DESC(control, "CBatteryPopupControl::NewL( text, link ) - failed");
    control->SetCommandObserver( *this );
    CleanupStack::PopAndDestroy( linkText );
    delete control;
    }

void MT_BatteryPopupControl::MT_BatteryPopupControl_ShowPopUpL(  )
    {
    HBufC* linkText = KLinkText().AllocLC();
    TBufC<9> bufText(KText);
    TPtr text = bufText.Des();
    CBatteryPopupControl* control = CBatteryPopupControl::NewL( text, 
                                                                *linkText );
    EUNIT_ASSERT_DESC(control, "CBatteryPopupControl::NewL( text, link ) - failed");
    control->ShowPopUpL( );
    CleanupStack::PopAndDestroy( linkText );
    delete control;
    }      


//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    MT_BatteryPopupControl,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test1 ",
    "BatteryPopupControl",
    "NewL",
    "FUNCTIONALITY",
    SetupL, MT_BatteryPopupControl_NewLL, Teardown)

EUNIT_TEST(
    "SetCommandObserver - test ",
    "BatteryPopupControl",
    "SetCommandObserver",
    "FUNCTIONALITY",
    SetupL, MT_BatteryPopupControl_SetCommandObserverL, Teardown)     

EUNIT_TEST(
    "ShowPopUpL - test ",
    "BatteryPopupControl",
    "ShowPopUpL",
    "FUNCTIONALITY",
    SetupL, MT_BatteryPopupControl_ShowPopUpL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
