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
* Description:  ProfileEngine test suite.
*
*/

////////////////////////////////////////////////////////////////////////
// DllMain for T_ProfileEngine.dll


// INCLUDES
#include <CEUnitTestSuite.h>
#include "T_MProfileEngine.h"
#include "T_MProfile.h"
#include "T_CProfileChangeNotifyHandler.h"
#include "T_MProfilesNamesArray.h"
#include "T_MProfileName.h"
#include "T_MProfileTones.h"
#include "T_MProfileExtraTones.h"
#include "T_MProfileExtraSettings.h"
#include "T_MProfilePttSettings.h"
#include "T_MProfileFeedbackSettings.h"
#include "T_MProfile3DToneSettings.h"

/**
 * T_ProfileEngine.dll test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* mainSuite = CEUnitTestSuite::NewLC(_L("T_ProfileEngine API Tests"));

    // NewLC leaves the pointer to cleanupstack until AddL finishes
    mainSuite->AddL( T_MProfileName::NewLC() );
    mainSuite->AddL( T_MProfileTones::NewLC() );
    mainSuite->AddL( T_MProfileExtraTones::NewLC() );
    mainSuite->AddL( T_MProfileExtraSettings::NewLC() );
    mainSuite->AddL( T_MProfilePttSettings::NewLC() );
    mainSuite->AddL( T_MProfileFeedbackSettings::NewLC() );
    mainSuite->AddL( T_MProfile3DToneSettings::NewLC() );
    mainSuite->AddL( T_MProfilesNamesArray::NewLC() );
    mainSuite->AddL( T_MProfile::NewLC() );
    mainSuite->AddL( T_CProfileChangeNotifyHandler::NewLC() );
    mainSuite->AddL( T_MProfileEngine::NewLC() );

    CleanupStack::Pop( 11 );

    CleanupStack::Pop( mainSuite );
    return mainSuite;
    }

