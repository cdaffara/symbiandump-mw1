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


#ifndef __CAMCTEST_7_H
#define __CAMCTEST_7_H

//  INCLUDES

#include <e32base.h>
#include "camcTest.h"

#include <videorecorder.h>
#include "camctestclient_7.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION


class CCamcTest_7 : public CTestCase
    {
    public:
        
        CCamcTest_7 ();
        ~CCamcTest_7 ();
        
        // Allocate the resources for one test function
        void setUpL ();
        
        // Free the resources reserved in setUpL()
        void tearDown ();
        
        // A function to collect and return a suite of tests
        static MTest* suiteL ();
        
        
        
    protected:
        
        // Own test functions that use assertions and may leave:
        void OpenFileL_007_L();
        void OpenFileL_013_L();
        void OpenFileL_014_L();    
        void OpenFileL_015_L();
        void Close_006_L();
        
    private:
        
        CCamcTestClient_7 * iCamcTestClient;
        
    };

#endif
