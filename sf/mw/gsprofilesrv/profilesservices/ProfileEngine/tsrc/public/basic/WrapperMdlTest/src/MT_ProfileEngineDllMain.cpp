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
* Description:  ProfileEngine wrapper test suite.
*
*/

//  CLASS HEADER
#include "MT_ProEngFactory.h"
#include "MT_MProEngNotifyHandler.h"
#include "MT_MProEngEngine.h"
#include "MT_MProEngProfile.h"
#include "MT_MProEngProfileName.h"
#include "MT_MProEngProfileNameArray.h"
#include "MT_MProEngTones.h"
#include "MT_MProEngToneSettings.h"
#include "MT_MProEngAlertToneSeeker.h"
#include "T_ProfileTestTools.h"

//  EXTERNAL INCLUDES
#include <CEUnitTestSuite.h>

/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    // First, make sure there are default settings in all profiles:
    T_ProfileTestTools::ResetRepository();

    CEUnitTestSuite* mainSuite = CEUnitTestSuite::NewLC(_L("T_ProfileEngineWrapper API Tests"));

    // NewLC leaves the pointer to cleanupstack until AddL finishes
    mainSuite->AddL( MT_ProEngFactory::NewLC() );
    mainSuite->AddL( MT_MProEngNotifyHandler::NewLC() );
    mainSuite->AddL( MT_MProEngAlertToneSeeker::NewLC() );
    mainSuite->AddL( MT_MProEngEngine::NewLC() );
    mainSuite->AddL( MT_MProEngProfile::NewLC() );
    mainSuite->AddL( MT_MProEngProfileName::NewLC() );
    mainSuite->AddL( MT_MProEngProfileNameArray::NewLC() );
    mainSuite->AddL( MT_MProEngTones::NewLC() );
    mainSuite->AddL( MT_MProEngToneSettings::NewLC() );

    CleanupStack::Pop( 9 );

    CleanupStack::Pop( mainSuite );
    return mainSuite;
    }

//  END OF FILE
