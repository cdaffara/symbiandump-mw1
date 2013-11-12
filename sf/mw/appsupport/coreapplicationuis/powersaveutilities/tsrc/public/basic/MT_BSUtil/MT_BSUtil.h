/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef __MT_BSUTIL_H__
#define __MT_BSUTIL_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>

//  FORWARD DECLARATIONS
class CBSUtil;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( MT_BSUtil )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static MT_BSUtil* NewL();
        static MT_BSUtil* NewLC();
        /**
         * Destructor
         */
        ~MT_BSUtil();

    private:    // Constructors and destructors

        MT_BSUtil();
        void ConstructL();

    private:    // New methods

         void SetupL();
         
         void Teardown();
        
         void MT_CBSUtil_NewLL();
        
         void MT_CBSUtil_NewLCL();
        
         void MT_CBSUtil_GetBatteryCapacityL();
        

    private:    // Data
    
        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __MT_BSUTIL_H__

// End of file
