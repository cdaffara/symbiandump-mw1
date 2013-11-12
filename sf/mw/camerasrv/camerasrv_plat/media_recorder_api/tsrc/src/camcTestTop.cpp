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
* Description:  Camcorder MMF API Test DLL
*
*/



// INCLUDE FILES


#include "CamcTestTop.h"
#include "CamcTest_1.h"
#include "CamcTest_2.h"
#include "CamcTest_3.h"
#include "CamcTest_4.h"
#include "CamcTest_5.h"
#include "CamcTest_6.h"
#include "CamcTest_7.h"
#include "CamcTest_8.h"
#include "CamcTest_9.h"
#include "CamcTest_visualcheckdsp.h"
#include "CamcTest_visualcheck.h"
#include <e32des8.h>


// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES 
 
// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

//////////////////
// CppUnit things
//////////////////

#ifndef EKA2
// Starting point of the DLL (Always needed) 
GLDEF_C TInt E32Dll(TDllReason)
    {
    return(KErrNone);
    }
#endif
// Exactly one exported function returning the suite of 
// test functions for the test runner in the framework.
// (Always needed)
//




EXPORT_C MTest* CreateTopFct ()
    {
    MTest * testSuite = NULL;
    TRAPD(err, testSuite = CCamcTestTop::suiteL() ) ;
    if (err)
        {
        User::Panic(_L("suiteL has left !"), 99);
        }
    return ( testSuite );
    }

 
CCamcTestTop::~CCamcTestTop()
    {
    }


MTest* CCamcTestTop::suiteL ()
    {
    CTestSuite *suite = CTestSuite::NewL(_L8("Test Suite Container"));
    CleanupStack::PushL( suite );

    suite->addTestL(CCamcTest_1::suiteL());
    suite->addTestL(CCamcTest_2::suiteL());
    suite->addTestL(CCamcTest_3::suiteL());
    suite->addTestL(CCamcTest_4::suiteL());
    suite->addTestL(CCamcTest_5::suiteL());   
    suite->addTestL(CCamcTest_6::suiteL());
    suite->addTestL(CCamcTest_7::suiteL());    
    suite->addTestL(CCamcTest_8::suiteL());
    suite->addTestL(CCamcTest_9::suiteL());

    // Visual checks are flagged out because tests are very long !
    
    // Only one of these suite should be selected, depending on 
    // HW used for the test.
    // suite->addTestL(CCamcTest_visualcheckdsp::suiteL()); 
    // suite->addTestL(CCamcTest_visualcheck::suiteL()); 

    CleanupStack::Pop( suite );
    return suite;
    
    }


