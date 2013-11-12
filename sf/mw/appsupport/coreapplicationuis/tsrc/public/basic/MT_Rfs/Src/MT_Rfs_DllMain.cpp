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


//  CLASS HEADER
#include "MT_RRfsClient.h"
#include "MT_CRfsHandler.h"

//  EXTERNAL INCLUDES
#include <CEUnitTestSuite.h>

EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC( _L("Rfs API tests") );

    rootSuite->AddL( MT_RRfsClient::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( MT_CRfsHandler::NewLC() );
    CleanupStack::Pop();
    
    CleanupStack::Pop( rootSuite );
    return rootSuite;
    }