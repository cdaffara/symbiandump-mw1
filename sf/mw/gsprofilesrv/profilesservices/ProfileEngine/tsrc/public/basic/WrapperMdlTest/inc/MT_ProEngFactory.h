/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: EUnitWizard generated test class.
*
*/
////////////////////////////////////////////////////////////////////////
// MT_ProEngFactory.h
//

#ifndef MT_PROENGFACTORY_H
#define MT_PROENGFACTORY_H

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <f32file.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class ProEngFactory;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class MT_ProEngFactory
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static MT_ProEngFactory* NewL();
        static MT_ProEngFactory* NewLC();
        /**
         * Destructor
         */
        ~MT_ProEngFactory();

    private:    // Constructors and destructors

        MT_ProEngFactory();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void MT_ProEngFactory_NewEngineLL();
        
        
         void MT_ProEngFactory_NewEngineLCL();
        
        
         void MT_ProEngFactory_NewEngineL_1L();
        
        
         void MT_ProEngFactory_NewEngineLC_1L();
        
        
         void MT_ProEngFactory_NewNotifyHandlerLL();
        
        
         void MT_ProEngFactory_NewAlertToneSeekerLL();
        

    private:    // Implementation

		EUNIT_DECLARE_TEST_TABLE; 
		
    private:    // Data

        RFs iFs;
    };

#endif      //  MT_PROENGFACTORY_H

// End of file
