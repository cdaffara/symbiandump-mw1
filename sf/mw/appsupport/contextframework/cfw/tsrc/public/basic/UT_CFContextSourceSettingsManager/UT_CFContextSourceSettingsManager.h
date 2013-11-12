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


#ifndef __UT_CFCONTEXTSOURCESETTINGSMANAGER_H__
#define __UT_CFCONTEXTSOURCESETTINGSMANAGER_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <f32file.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CCFContextSourceSettingImpl;
class CCFContextSourceSettingsManagerImpl;
class CCFContextSourceSettingsManager;
class CCFContextSourceSettingParameterImpl;
class CCFContextSourceSettingArray;
class CCFContextSourceSettingArrayImpl;

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
NONSHARABLE_CLASS( UT_CFContextSourceSettingsManager )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CFContextSourceSettingsManager* NewL();
        static UT_CFContextSourceSettingsManager* NewLC();
        /**
         * Destructor
         */
        ~UT_CFContextSourceSettingsManager();

    private:    // Constructors and destructors

        UT_CFContextSourceSettingsManager();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

        void SetupL();
        void SetupEmptyL();
        void Teardown();

        void UT_CCFContextSourceSettingParameterImpl_NewLL();
        void UT_CCFContextSourceSettingParameterImpl_NewLCL();
        void UT_CCFContextSourceSettingParameterImpl_NameL();
        void UT_CCFContextSourceSettingParameterImpl_AttributesL();
        
        void UT_CCFContextSourceSettingImpl_NewLL();
        void UT_CCFContextSourceSettingImpl_NewLCL();
        void UT_CCFContextSourceSettingImpl_NameL();
        void UT_CCFContextSourceSettingImpl_AttributesL();
        void UT_CCFContextSourceSettingImpl_NumOfParametersL();
        void UT_CCFContextSourceSettingImpl_ParametersL();

        void UT_CCFContextSourceSettingsManagerImpl_NewLL();
        void UT_CCFContextSourceSettingsManagerImpl_NewLCL();
        void UT_CCFContextSourceSettingsManagerImpl_ParseSettingsLL();
        
        void UT_CCFContextSourceSettingArray_NewLL();
        void UT_CCFContextSourceSettingArray_NewLCL();

        void UT_CCFContextSourceSettingArrayImpl_NewLL();
        void UT_CCFContextSourceSettingArrayImpl_NewLCL();
        void UT_CCFContextSourceSettingArrayImpl_AppendItemLL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CCFContextSourceSettingImpl* iContextSourceSettingImpl;
        CCFContextSourceSettingParameterImpl* iContextSourceSettingParameterImpl;
        CCFContextSourceSettingsManagerImpl* iContextSourceSettingsManagerImpl;
        RFs iFs;
    };

#endif      //  __UT_CFCONTEXTSOURCESETTINGSMANAGER_H__
