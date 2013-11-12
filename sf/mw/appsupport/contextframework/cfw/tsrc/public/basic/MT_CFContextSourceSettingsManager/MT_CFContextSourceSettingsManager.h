/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef __MT_CFCONTEXTSOURCESETTINGSMANAGER_H__
#define __MT_CFCONTEXTSOURCESETTINGSMANAGER_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <f32file.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CCFContextSourceSettingsManager;

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
NONSHARABLE_CLASS( MT_CFContextSourceSettingsManager )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static MT_CFContextSourceSettingsManager* NewL();
        static MT_CFContextSourceSettingsManager* NewLC();
        /**
         * Destructor
         */
        ~MT_CFContextSourceSettingsManager();

    private:    // Constructors and destructors

        MT_CFContextSourceSettingsManager();
        void ConstructL();

    public:     // From observer interface

    private:    // New methods

        void SetupL();
        void SetupEmptyL();
        void Teardown();

        void MT_CCFContextSourceSettingsManager_NewLL();
        void MT_CCFContextSourceSettingsManager_NewLCL();
        void MT_CCFContextSourceSettingsManager_ParseSettingsL1L();
        void MT_CCFContextSourceSettingsManager_ParseSettingsL2L();
        void MT_CCFContextSourceSettingsManager_ParseSettingsL3L();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 

        RFs iFs;
        CCFContextSourceSettingsManager* iContextSourceSettingsManager;
    };

#endif      //  __MT_CFCONTEXTSOURCESETTINGSMANAGER_H__

// End of file
