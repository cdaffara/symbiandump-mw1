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
* Description:
*
*/
#ifndef MENU_SAT_INTERFACE_TEST_H
#define MENU_SAT_INTERFACE_TEST_H

#include <QtTest>
#include <QObject>
#include <e32property.h>
#include "ca2internalCRkeys.h"


// CLASS DECLARATION

/**
*  CMenuSatInterfaceTest test class for STIF Test Framework TestScripter.
*  Class for testing CMenuSATInterface.
*
*  @lib ?library
*  @since ?Series60_version
*/
class CMenuSatInterfaceTest : public QObject
    {
	Q_OBJECT
    public:  // Constructors and destructor
        CMenuSatInterfaceTest();
		virtual ~CMenuSatInterfaceTest();
				
	private slots: // Functions from base classes
        
        /**
        * Test methods are listed below. 
        */
        void Test_SatUiVisibleL();
        void Test_SatUiVisibleIconIdL();
        void Test_SatInterfaceConstructL();
        
        /**
        * Utility methods are listed below. 
        */
        void WaitL(TInt aMicroSec);
        
    private:    // Data
        /**
        * Data tosave enviroment statebefore testing. 
        */
    	TInt iIconId;
        
    };

#endif      // TESTCLASS_H
            
// End of File
