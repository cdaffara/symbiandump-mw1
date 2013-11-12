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


#ifndef __CAMCTEST_VISUALCHECK_H
#define __CAMCTEST_VISUALCHECK_H

//  INCLUDES

#include <e32base.h>
#include "Camctest.h"

#include <videorecorder.h>
#include "camctestclient_visualcheck.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION



class CCamcTest_visualcheck : public CTestCase
    {
    public:
        
        CCamcTest_visualcheck ();
        ~CCamcTest_visualcheck ();
        
        // Allocate the resources for one test function
        void setUpL ();
        
        // Free the resources reserved in setUpL()
        void tearDown ();
        
        // A function to collect and return a suite of tests
        static MTest* suiteL ();
        
        
        
    protected:
        
        // Own test functions that use assertions and may leave:

       void VisualCheck_101_L();
       void VisualCheck_102_L();
       void VisualCheck_103_L();
       void VisualCheck_104_L();
       void VisualCheck_105_L();
       void VisualCheck_106_L();
       void VisualCheck_107_L();
       void VisualCheck_108_A_L();
       void VisualCheck_108_B_L();
       void VisualCheck_109_L();
       void VisualCheck_110_L();
       void VisualCheck_111_L();
       void VisualCheck_112_L();
       void VisualCheck_113_L();
       void VisualCheck_114_L();
       void VisualCheck_115_L();
       void VisualCheck_116_L();
       void VisualCheck_117_L();
       void VisualCheck_118_L();
       void VisualCheck_119_L();
       void VisualCheck_120_L();
       void VisualCheck_121_L();

    private:
        
        CCamcTestClient_visualcheck * iCamcTestClient;
        
    };

#endif
