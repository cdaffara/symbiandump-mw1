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




#ifndef __UT_CRTPTIMER_H__
#define __UT_CRTPTIMER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "rtptimer.h"

//  FORWARD DECLARATIONS
class CRtpTimer;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( UT_CRtpTimer )
     : public CEUnitTestSuiteClass,public MTimeOutNotify
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CRtpTimer* NewL();
        static UT_CRtpTimer* NewLC();
        /**
         * Destructor
         */
        ~UT_CRtpTimer();

    private:    // Constructors and destructors

        UT_CRtpTimer();
        void ConstructL();

    public:     // From observer interface
 		/**
        * MTimeOutNotify Virtual function 
        */
   		void OnExpiredL( TInt aStatus );
   		
        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CRtpTimer_NewLL();
        

    private:    // Data
        CRtpTimer* iRtpTimer;
        TInt iCounter;
        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __UT_CRTPTIMER_H__

// End of file
