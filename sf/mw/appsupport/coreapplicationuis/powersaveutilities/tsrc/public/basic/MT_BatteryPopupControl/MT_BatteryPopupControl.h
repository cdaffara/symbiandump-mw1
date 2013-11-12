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


#ifndef __MT_BATTERYPOPUPCONTROL_H__
#define __MT_BATTERYPOPUPCONTROL_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>
#include <EIKCMOBS.H>


//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS


//  CLASS DEFINITION
/**
 * Auto-generated test suite
 *
 */
NONSHARABLE_CLASS( MT_BatteryPopupControl )
	: public CEUnitTestSuiteClass,
	  public MEikCommandObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static MT_BatteryPopupControl* NewL();
        static MT_BatteryPopupControl* NewLC();
        /**
         * Destructor
         */
        ~MT_BatteryPopupControl();

// From MEikCommandObserver
    /**
     * @see MEikCommandObserver
     */
    void ProcessCommandL( TInt aCommandId );        

    private:    // Constructors and destructors

        MT_BatteryPopupControl();
        void ConstructL();

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void MT_BatteryPopupControl_NewLL(  );
                  
         void MT_BatteryPopupControl_SetCommandObserverL(  );
         
         void MT_BatteryPopupControl_ShowPopUpL(  );
        

    private:    // Data
		

        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __MT_BATTERYPOPUPCONTROL_H__

// End of file
