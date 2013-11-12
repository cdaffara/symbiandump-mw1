/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef __MT_RRFSCLIENT_H__
#define __MT_RRFSCLIENT_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>


//  INTERNAL INCLUDES
#include "rfsClient.h"

//  FORWARD DECLARATIONS


//  CLASS DEFINITION
/**
 * Auto-generated EUnit test suite
 */
NONSHARABLE_CLASS( MT_RRfsClient )
		: public CEUnitTestSuiteClass
	{
	public: // Constructors and destructors
	
		/**
		 * Two phase construction
		 */
		static MT_RRfsClient* NewL();
		static MT_RRfsClient* NewLC();
		/**
		 * Destructor
		 */
		~MT_RRfsClient();
	
	private: // Constructors and destructors
	
		MT_RRfsClient();
		void ConstructL();
	
	private: // New methods
		void SetupEmptyL();
		void SetupL();
		void Teardown();
		void TeardownEmpty();
		
		void TestConnectAndCloseL();
		void TestPerformRfsNormalL();
		void TestPerformRfsDeepL();
		void TestPerformPostEikonRfsL();
		void TestRunScriptInDescriptorNoErrorL();
		void TestRunScriptInDescriptorSyntaxErrorL();
		void TestRunScriptInFileNoErrorL();
		void TestRunScriptInFileNotFoundL();
	
	private: // Data	
		RRfsClient iRRfsClient;
		
		EUNIT_DECLARE_TEST_TABLE;
	};

#endif      //  __MT_RRFSCLIENT_H__
