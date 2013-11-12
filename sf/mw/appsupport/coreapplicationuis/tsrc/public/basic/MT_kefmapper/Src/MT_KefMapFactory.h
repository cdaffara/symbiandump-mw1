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


#ifndef __MT_KefMapFactory_H__
#define __MT_KefMapFactory_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>
#include <keffactory.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS


//  CLASS DEFINITION
/**
 * Auto-generated test suite
 *
 */
NONSHARABLE_CLASS( MT_KefMapFactory )
	: public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static MT_KefMapFactory* NewL();
        static MT_KefMapFactory* NewLC();
        /**
         * Destructor
         */
        ~MT_KefMapFactory();

    private:    // Constructors and destructors

        MT_KefMapFactory();
        void ConstructL();

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void T_KefMapFactory_CreateKefMapLL();
        

    private:    // Data
		
        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __MT_KefMapFactory_H__

// End of file
