/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
#include "MT_CFContextSourceSettingsManager.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <cfcontextsourcesettingsmanager.h>
#include <cfcontextsourcesetting.h>
#include <cfcontextsourcesettingarray.h>
#include <cfcontextsourcesettingparameter.h>
#include <cfkeyvaluepair.h>


//  INTERNAL INCLUDES

// Cleans up RKeyValueArray instance
LOCAL_C void CleanupKeyValueArray( TAny* aArray )
    {
    static_cast<RKeyValueArray*>( aArray )->ResetAndDestroy();
    }

// Cleanup item for RKeyValueArray object
LOCAL_C void CleanupResetAndDestroyPushL( RKeyValueArray& aArray )
    {
    TCleanupItem item( CleanupKeyValueArray, &aArray );
    CleanupStack::PushL( item );
    }

// CONSTRUCTION
MT_CFContextSourceSettingsManager* MT_CFContextSourceSettingsManager::NewL()
    {
    MT_CFContextSourceSettingsManager* self = MT_CFContextSourceSettingsManager::NewLC();
    CleanupStack::Pop();

    return self;
    }

MT_CFContextSourceSettingsManager* MT_CFContextSourceSettingsManager::NewLC()
    {
    MT_CFContextSourceSettingsManager* self = new( ELeave ) MT_CFContextSourceSettingsManager();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
MT_CFContextSourceSettingsManager::~MT_CFContextSourceSettingsManager()
    {
    Teardown();
    }

// Default constructor
MT_CFContextSourceSettingsManager::MT_CFContextSourceSettingsManager()
    {
    }

// Second phase construct
void MT_CFContextSourceSettingsManager::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void MT_CFContextSourceSettingsManager::SetupEmptyL(  )
    {
    
    }

void MT_CFContextSourceSettingsManager::SetupL(  )
    {
    User::LeaveIfError( iFs.Connect() );
    
    iContextSourceSettingsManager =
        CCFContextSourceSettingsManager::NewL( iFs );
    }

void MT_CFContextSourceSettingsManager::Teardown(  )
    {
    delete iContextSourceSettingsManager;
    iContextSourceSettingsManager = NULL;
    
    iFs.Close();
    }

//-----------------------------------------------------------------------------
// MT_CFContextSourceSettingsManager::MT_CCFContextSourceSettingsManager_NewLL
//-----------------------------------------------------------------------------
//
void MT_CFContextSourceSettingsManager::MT_CCFContextSourceSettingsManager_NewLL(  )
    {
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    CCFContextSourceSettingsManager* obj =
        CCFContextSourceSettingsManager::NewL( fs );
    CleanupStack::PushL( obj );
    
    EUNIT_ASSERT_DESC( obj, "CCFContextSourceSettingsManager instance not created!" );
    
    CleanupStack::PopAndDestroy( obj );
    CleanupStack::PopAndDestroy( &fs );
    }

//-----------------------------------------------------------------------------
// MT_CFContextSourceSettingsManager::MT_CCFContextSourceSettingsManager_NewLCL
//-----------------------------------------------------------------------------
//
void MT_CFContextSourceSettingsManager::MT_CCFContextSourceSettingsManager_NewLCL(  )
    {
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    CCFContextSourceSettingsManager* obj =
        CCFContextSourceSettingsManager::NewLC( fs );
    
    EUNIT_ASSERT_DESC( obj, "CCFContextSourceSettingsManager instance not created!" );
    
    CleanupStack::PopAndDestroy( obj );
    CleanupStack::PopAndDestroy( &fs );
    }

//-----------------------------------------------------------------------------
// MT_CFContextSourceSettingsManager::MT_CCFContextSourceSettingsManager_ParseSettingsL1L
//-----------------------------------------------------------------------------
//
void MT_CFContextSourceSettingsManager::MT_CCFContextSourceSettingsManager_ParseSettingsL1L(  )
    {
    _LIT( KSettingsPath, "z:\\data\\cftestdata\\testsettings\\validsettingsfile.xml" );
    
    _LIT( KSettingName, "PluginName" );
    _LIT( KParamName, "Param" );

    _LIT( KAttributeFormat, "attribute%d" );
    _LIT( KAttributeValueFormat, "attributevalue%d" );
    _LIT( KParamFormat, "param%d" );
    _LIT( KParamValueFormat, "value%d" );
    
    TBuf<20> key( KNullDesC );
    TBuf<20> value( KNullDesC );
    
    CCFContextSourceSettingArray* array = CCFContextSourceSettingArray::NewLC();
    iContextSourceSettingsManager->ParseSettingsL( KSettingsPath, *array );
            
    // Check settings -
    // Should have two setting blocks
    EUNIT_ASSERT_DESC( array->Count() == 2, "Invalid number of settings!" );
    
    // Check setting names
    TInt count = array->Count();
    for( TInt i = 0; i < count; i++ )
        {
        EUNIT_ASSERT_DESC( array->Item( i ).Name() == KSettingName, "Invalid setting name!" );
        }
        
    // Check attributes
    for( TInt i = 0; i < count; i++ )
        {
        const RKeyValueArray& attributes = array->Item( i ).Attributes();
        EUNIT_ASSERT_DESC( attributes.Count() == i + 1, "Invalid attribute count!" );
        TInt attributeCount = attributes.Count();
        for( TInt j = 0; j < attributeCount; j++ )
            {
            key.Format( KAttributeFormat, j + 1 );
            value.Format( KAttributeValueFormat, j + 1 );
            EUNIT_ASSERT_DESC( attributes[j]->Key() == key,
                "Invalid key!" );
            EUNIT_ASSERT_DESC( attributes[j]->Value() == value,
                "Invalid value!" );
            }
        }
        
    // Check parameters
    count = array->Count();
    TInt parameterCount = 0;
    for( TInt a = 0; a < count; a++ )
        {
        const RSettingParameterArray& paramArray = array->Item( a ).Parameters();
        parameterCount = paramArray.Count();
        for( TInt b = 0; b < parameterCount; b++ )
            {
            EUNIT_ASSERT_DESC( paramArray[b]->Name() == KParamName, "Invalid parameter name!" );
            const RKeyValueArray& attributes = paramArray[b]->Attributes();
            TInt attributesCount = attributes.Count();
            for( TInt c = 0; c < attributesCount; c++ )
                {
                key.Format( KParamFormat, c + 1 );
                value.Format( KParamValueFormat, c + 1 );
                EUNIT_ASSERT_DESC( attributes[c]->Key() == key,
                    "Invalid key!" );
                EUNIT_ASSERT_DESC( attributes[c]->Value() == value,
                    "Invalid value!" );
                }
            }
        }
            
    CleanupStack::PopAndDestroy( array );
    }

//-----------------------------------------------------------------------------
// MT_CFContextSourceSettingsManager::MT_CCFContextSourceSettingsManager_ParseSettingsL2L
//-----------------------------------------------------------------------------
//
void MT_CFContextSourceSettingsManager::MT_CCFContextSourceSettingsManager_ParseSettingsL2L(  )
    {
    _LIT( KSettingsPath, "z:\\data\\cftestdata\\testsettings\\validsettingsfile2.xml" );
    
    _LIT( KSettingNameNode, "SettingNameNode" );
    _LIT( KParamNameNode, "ParamNameNode" );

    _LIT( KAttributeFormat, "attribute%d" );
    _LIT( KAttributeValueFormat, "attributevalue%d" );
    _LIT( KParamFormat, "param%d" );
    _LIT( KParamValueFormat, "value%d" );

    TBuf<20> key( KNullDesC );
    TBuf<20> value( KNullDesC );
    
    CCFContextSourceSettingArray* array = CCFContextSourceSettingArray::NewL();
    CleanupStack::PushL( array );
    iContextSourceSettingsManager->ParseSettingsL( KSettingsPath, *array );
    
    // Should have one setting block
    EUNIT_ASSERT_DESC( array->Count() == 1, "Invalid number of settings!" );

    // Check setting names
    TInt count = array->Count();
    for( TInt i = 0; i < count; i++ )
        {
        EUNIT_ASSERT_DESC( array->Item( i ).Name() == KSettingNameNode, "Invalid setting name!" );
        }

    // Check attributes
    for( TInt i = 0; i < count; i++ )
        {
        const RKeyValueArray& attributes = array->Item( i ).Attributes();
        EUNIT_ASSERT_DESC( attributes.Count() == i + 1, "Invalid attribute count!" );
        TInt attributeCount = attributes.Count();
        for( TInt j = 0; j < attributeCount; j++ )
            {
            key.Format( KAttributeFormat, j + 1 );
            value.Format( KAttributeValueFormat, j + 1 );
            EUNIT_ASSERT_DESC( attributes[j]->Key() == key,
                "Invalid key!" );
            EUNIT_ASSERT_DESC( attributes[j]->Value() == value,
                "Invalid value!" );
            }
        }

    // Check parameters
    count = array->Count();
    TInt parameterCount = 0;
    for( TInt a = 0; a < count; a++ )
        {
        const RSettingParameterArray& paramArray = array->Item( a ).Parameters();
        parameterCount = paramArray.Count();
        for( TInt b = 0; b < parameterCount; b++ )
            {
            EUNIT_ASSERT_DESC( paramArray[b]->Name() == KParamNameNode, "Invalid parameter name!" );
            const RKeyValueArray& attributes = paramArray[b]->Attributes();
            TInt attributesCount = attributes.Count();
            for( TInt c = 0; c < attributesCount; c++ )
                {
                key.Format( KParamFormat, c + 1 );
                value.Format( KParamValueFormat, c + 1 );
                EUNIT_ASSERT_DESC( attributes[c]->Key() == key,
                    "Invalid key!" );
                EUNIT_ASSERT_DESC( attributes[c]->Value() == value,
                    "Invalid value!" );
                }
            }
        }

    CleanupStack::PopAndDestroy( array );
    }

//-----------------------------------------------------------------------------
// MT_CFContextSourceSettingsManager::MT_CCFContextSourceSettingsManager_ParseSettingsL3L
//-----------------------------------------------------------------------------
//
void MT_CFContextSourceSettingsManager::MT_CCFContextSourceSettingsManager_ParseSettingsL3L(  )
    {
    _LIT( KSettingsPath, "z:\\data\\cftestdata\\testsettings\\invalidsettingsfile.xml" );

    CCFContextSourceSettingArray* array = CCFContextSourceSettingArray::NewLC();
    iContextSourceSettingsManager->ParseSettingsL( KSettingsPath, *array );
    
    // Should have one setting block
    EUNIT_ASSERT_DESC( array->Count() == 0, "Invalid number of settings!" );
    
    CleanupStack::PopAndDestroy( array );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    MT_CFContextSourceSettingsManager,
    "CFContextSourceSettingsManager module test.",
    "MODULE" )

EUNIT_TEST(
    "NewL - test ",
    "CCFContextSourceSettingsManager",
    "NewL",
    "FUNCTIONALITY",
    SetupEmptyL, MT_CCFContextSourceSettingsManager_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFContextSourceSettingsManager",
    "NewLC",
    "FUNCTIONALITY",
    SetupEmptyL, MT_CCFContextSourceSettingsManager_NewLCL, Teardown)

EUNIT_TEST(
    "ParseSettingsL 1 - test ",
    "CCFContextSourceSettingsManager",
    "ParseSettingsL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextSourceSettingsManager_ParseSettingsL1L, Teardown)

EUNIT_TEST(
    "ParseSettingsL 2 - test ",
    "CCFContextSourceSettingsManager",
    "ParseSettingsL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextSourceSettingsManager_ParseSettingsL2L, Teardown)

EUNIT_TEST(
    "ParseSettingsL 3 - test ",
    "CCFContextSourceSettingsManager",
    "ParseSettingsL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextSourceSettingsManager_ParseSettingsL3L, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
