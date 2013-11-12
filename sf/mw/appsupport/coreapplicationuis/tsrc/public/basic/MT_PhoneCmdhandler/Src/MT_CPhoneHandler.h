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


#ifndef __MT_CPHONEHANDLER_H__
#define __MT_CPHONEHANDLER_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>
#include <PhoneHandler.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS


//  CLASS DEFINITION
NONSHARABLE_CLASS( MT_CPhoneHandler )
	: public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static MT_CPhoneHandler* NewL();
        static MT_CPhoneHandler* NewLC();
        /**
         * Destructor
         */
        ~MT_CPhoneHandler();

    private:    // Constructors and destructors

        MT_CPhoneHandler();
        void ConstructL();

    private:    // New methods

         void EmptySetupL();
        
         void EmptyTeardown();
        
         void T_CPhoneHandler_NewL1L();
         
         void T_CPhoneHandler_NewL2L();
        

    private:    // Data
		
        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __MT_CPHONEHANDLER_H__

// End of file
