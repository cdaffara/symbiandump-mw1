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
#include "UT_CFContextSourceSettingsManager.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>


//  INTERNAL INCLUDES
#include "cfcontextsourcesettingimpl.h"
#include "cfcontextsourcesettingarrayimpl.h"
#include "cfcontextsourcesettingparameterimpl.h"
#include "cfcontextsourcesettingsmanagerimpl.h"
#include "cfcontextsourcesettingarrayimpl.h"

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
UT_CFContextSourceSettingsManager* UT_CFContextSourceSettingsManager::NewL()
    {
    UT_CFContextSourceSettingsManager* self = UT_CFContextSourceSettingsManager::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CFContextSourceSettingsManager* UT_CFContextSourceSettingsManager::NewLC()
    {
    UT_CFContextSourceSettingsManager* self = new( ELeave ) UT_CFContextSourceSettingsManager();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CFContextSourceSettingsManager::~UT_CFContextSourceSettingsManager()
    {
    Teardown();
    }

// Default constructor
UT_CFContextSourceSettingsManager::UT_CFContextSourceSettingsManager()
    {
    }

// Second phase construct
void UT_CFContextSourceSettingsManager::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CFContextSourceSettingsManager::SetupEmptyL(  )
    {

    }

void UT_CFContextSourceSettingsManager::SetupL(  )
    {
    User::LeaveIfError( iFs.Connect() );
    
    iContextSourceSettingImpl = CCFContextSourceSettingImpl::NewL();
    
    iContextSourceSettingParameterImpl =
        CCFContextSourceSettingParameterImpl::NewL();
    
    iContextSourceSettingsManagerImpl =
        CCFContextSourceSettingsManagerImpl::NewL( iFs );
    }

void UT_CFContextSourceSettingsManager::Teardown(  )
    {
    delete iContextSourceSettingsManagerImpl;
    iContextSourceSettingsManagerImpl = NULL;
    
    delete iContextSourceSettingImpl;
    iContextSourceSettingImpl = NULL;
    
    delete iContextSourceSettingParameterImpl;
    iContextSourceSettingParameterImpl = NULL;
    
    iFs.Close();
    }

//-----------------------------------------------------------------------------
// UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingParameterImpl_NewLL
//-----------------------------------------------------------------------------
//
void UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingParameterImpl_NewLL(  )
    {
    CCFContextSourceSettingParameterImpl* obj =
        CCFContextSourceSettingParameterImpl::NewL();
    CleanupStack::PushL( obj );
    
    EUNIT_ASSERT_DESC( obj, "CCFContextSourceSettingParameterImpl instance not created!" );
    
    CleanupStack::PopAndDestroy( obj );
    }
    
//-----------------------------------------------------------------------------
// UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingParameterImpl_NewLCL
//-----------------------------------------------------------------------------
//
void UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingParameterImpl_NewLCL(  )
    {
    CCFContextSourceSettingParameterImpl* obj =
        CCFContextSourceSettingParameterImpl::NewLC();
    
    EUNIT_ASSERT_DESC( obj, "CCFContextSourceSettingParameterImpl instance not created!" );
    
    CleanupStack::PopAndDestroy( obj );
    }

//-----------------------------------------------------------------------------
// UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingParameterImpl_NameL
//-----------------------------------------------------------------------------
//
void UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingParameterImpl_NameL(  )
    {
    _LIT( KMedium, "Meeeeeeeeeeddddiiiiuuuuumm" );
    _LIT( KShort, "Short" );
    _LIT( KLong, "Llllllllllllllllllllllloooooooooooooooonnnnnnnnnnnnggg" );
    TPtrC buf;

    buf.Set( iContextSourceSettingParameterImpl->Name() );
    EUNIT_ASSERT_DESC( buf == KNullDesC, "Incorrect name returned!" );

    iContextSourceSettingParameterImpl->SetNameL( KMedium );
    buf.Set( iContextSourceSettingParameterImpl->Name() );
    EUNIT_ASSERT_DESC( buf == KMedium, "Incorrect name returned!" );

    iContextSourceSettingParameterImpl->SetNameL( KShort );
    buf.Set( iContextSourceSettingParameterImpl->Name() );
    EUNIT_ASSERT_DESC( buf == KShort, "Incorrect name returned!" );

    iContextSourceSettingParameterImpl->SetNameL( KLong );
    buf.Set( iContextSourceSettingParameterImpl->Name() );
    EUNIT_ASSERT_DESC( buf == KLong, "Incorrect name returned!" );
    
    iContextSourceSettingParameterImpl->SetNameL( KNullDesC );
    buf.Set( iContextSourceSettingParameterImpl->Name() );
    EUNIT_ASSERT_DESC( buf == KNullDesC, "Incorrect name returned!" );    
    }

//-----------------------------------------------------------------------------
// UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingParameterImpl_AttributesL
//-----------------------------------------------------------------------------
//
void UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingParameterImpl_AttributesL(  )
    {
    const TInt KCount = 10;
    _LIT( KKey, "key%d" );
    _LIT( KValue, "value%d" );
    TInt count = 0;
    TBuf<KCount> key( KNullDesC );
    TBuf<KCount> value( KNullDesC );
    
    count = iContextSourceSettingParameterImpl->Attributes().Count();
    EUNIT_ASSERT_DESC( count == 0, "Incorrect attribute count returned!" );
    
    for( TInt i = 0; i < KCount; i++ )
        {
        key.Format( KKey, i );
        value.Format( KValue, i );
        
        CCFKeyValuePair* keyValue = CCFKeyValuePair::NewLC( key, value );
        iContextSourceSettingParameterImpl->AddAttributeL( keyValue );
        CleanupStack::Pop( keyValue );
        }
        
    const RKeyValueArray& array = iContextSourceSettingParameterImpl->Attributes();
    count = array.Count();
    EUNIT_ASSERT_DESC( count == KCount,
        "Incorrect attribute count!" );
    for( TInt i = 0; i < count; i++ )
        {
        key.Format( KKey, i );
        value.Format( KValue, i );
        EUNIT_ASSERT_DESC( array[i]->Key() == key,
            "Incorrect key!" );
        EUNIT_ASSERT_DESC( array[i]->Value() == value,
            "Incorrect value!" );
        }
    }

//-----------------------------------------------------------------------------
// UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingImpl_NewLL
//-----------------------------------------------------------------------------
//
void UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingImpl_NewLL(  )
    {
    CCFContextSourceSettingImpl* obj = CCFContextSourceSettingImpl::NewL();
    CleanupStack::PushL( obj );
    
    EUNIT_ASSERT_DESC( obj, "CCFContextSourceSettingImpl instance not created!" );
    
    CleanupStack::PopAndDestroy( obj );
    }

//-----------------------------------------------------------------------------
// UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingImpl_NewLCL
//-----------------------------------------------------------------------------
//
void UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingImpl_NewLCL(  )
    {
    CCFContextSourceSettingImpl* obj = CCFContextSourceSettingImpl::NewLC();
    
    EUNIT_ASSERT_DESC( obj, "CCFContextSourceSettingImpl instance not created!" );
    
    CleanupStack::PopAndDestroy( obj );
    }

//-----------------------------------------------------------------------------
// UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingImpl_NameL
//-----------------------------------------------------------------------------
//
void UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingImpl_NameL(  )
    {
    _LIT( KMedium, "Meeeeeeeeeeddddiiiiuuuuumm" );
    _LIT( KShort, "Short" );
    _LIT( KLong, "Llllllllllllllllllllllloooooooooooooooonnnnnnnnnnnnggg" );
    TPtrC buf;

    buf.Set( iContextSourceSettingImpl->Name() );
    EUNIT_ASSERT_DESC( buf == KNullDesC, "Incorrect name returned!" );

    iContextSourceSettingImpl->SetNameL( KMedium );
    buf.Set( iContextSourceSettingImpl->Name() );
    EUNIT_ASSERT_DESC( buf == KMedium, "Incorrect name returned!" );

    iContextSourceSettingImpl->SetNameL( KShort );
    buf.Set( iContextSourceSettingImpl->Name() );
    EUNIT_ASSERT_DESC( buf == KShort, "Incorrect name returned!" );

    iContextSourceSettingImpl->SetNameL( KLong );
    buf.Set( iContextSourceSettingImpl->Name() );
    EUNIT_ASSERT_DESC( buf == KLong, "Incorrect name returned!" );
    
    iContextSourceSettingImpl->SetNameL( KNullDesC );
    buf.Set( iContextSourceSettingImpl->Name() );
    EUNIT_ASSERT_DESC( buf == KNullDesC, "Incorrect name returned!" );
    }

//-----------------------------------------------------------------------------
// UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingImpl_AttributesL
//-----------------------------------------------------------------------------
//
void UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingImpl_AttributesL(  )
    {
    const TInt KCount = 10;
    _LIT( KKey, "key%d" );
    _LIT( KValue, "value%d" );
    TInt count = 0;
    TBuf<KCount> key( KNullDesC );
    TBuf<KCount> value( KNullDesC );
    
    count = iContextSourceSettingImpl->Attributes().Count();
    EUNIT_ASSERT_DESC( count == 0, "Incorrect attribute count returned!" );
    
    for( TInt i = 0; i < KCount; i++ )
        {
        key.Format( KKey, i );
        value.Format( KValue, i );
        
        CCFKeyValuePair* keyValue = CCFKeyValuePair::NewLC( key, value );
        iContextSourceSettingImpl->AddAttributeL( keyValue );
        CleanupStack::Pop( keyValue );
        }
        
    const RKeyValueArray& array = iContextSourceSettingImpl->Attributes();
    count = array.Count();
    EUNIT_ASSERT_DESC( count == KCount,
        "Incorrect attribute count!" );
    for( TInt i = 0; i < count; i++ )
        {
        key.Format( KKey, i );
        value.Format( KValue, i );
        EUNIT_ASSERT_DESC( array[i]->Key() == key,
            "Incorrect key!" );
        EUNIT_ASSERT_DESC( array[i]->Value() == value,
            "Incorrect value!" );
        }
    }

//-----------------------------------------------------------------------------
// UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingImpl_NumOfParametersL
//-----------------------------------------------------------------------------
//
void UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingImpl_NumOfParametersL(  )
    {
    const TInt KCount = 10;
    TInt count = iContextSourceSettingImpl->Parameters().Count();
    EUNIT_ASSERT_DESC( count == 0, "Invalid number of parameters!" );

    for( TInt i = 0; i < KCount; i++ )
        {
        CCFContextSourceSettingParameterImpl* parameter =
            CCFContextSourceSettingParameterImpl::NewLC();
        iContextSourceSettingImpl->AddParameterL( parameter );
        count = iContextSourceSettingImpl->Parameters().Count();
        EUNIT_ASSERT_DESC( count == i + 1, "Invalid number of parameters!" );
        CleanupStack::Pop( parameter );
        }
    }

//-----------------------------------------------------------------------------
// UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingImpl_ParametersL
//-----------------------------------------------------------------------------
//
void UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingImpl_ParametersL(  )
    {
    const TInt KParameterCount = 10;
    const TInt KAttributeCountInParameter = 5;
    _LIT( KKey, "key%d" );
    _LIT( KValue, "value%d" );
    TInt count = 0;
    TBuf<KParameterCount> key( KNullDesC );
    TBuf<KParameterCount> value( KNullDesC );
    
    count = iContextSourceSettingImpl->Parameters().Count();
    EUNIT_ASSERT_DESC( count == 0, "Invalid number of parameters!" );
    
    // Add parameters
    for( TInt i = 0; i < KParameterCount; i++ )
        {
        CCFContextSourceSettingParameterImpl* param =
            CCFContextSourceSettingParameterImpl::NewLC();
        for( TInt j = 0; j < KAttributeCountInParameter; j++ )
            {
            key.Format( KKey, j );
            value.Format( KValue, j );
            CCFKeyValuePair* keyValue = CCFKeyValuePair::NewLC( key, value );
            param->AddAttributeL( keyValue );
            CleanupStack::Pop( keyValue );
            }
        iContextSourceSettingImpl->AddParameterL( param );
        CleanupStack::Pop( param );
        }
        
    // Check parameters
    TInt attributeCount = 0;
    const RSettingParameterArray& params = iContextSourceSettingImpl->Parameters();
    count = params.Count();
    EUNIT_ASSERT_DESC( count == KParameterCount, "Invalid number of parameters!" );
    for( TInt i = 0; i < count; i++ )
        {
        const RKeyValueArray& attributes = params[i]->Attributes();
        attributeCount = attributes.Count();
        EUNIT_ASSERT_DESC( attributeCount == KAttributeCountInParameter, "Invalid number of attributes!" );
        for( TInt j = 0; j < attributeCount; j++ )
            {
            key.Format( KKey, j );
            value.Format( KValue, j );
            EUNIT_ASSERT_DESC( attributes[j]->Key() == key,
                "Invalid attribute key!" );
            EUNIT_ASSERT_DESC( attributes[j]->Value() == value,
                "Invalid attribute key!" );
            }
        }
    }

//-----------------------------------------------------------------------------
// UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingsManagerImpl_NewLL
//-----------------------------------------------------------------------------
//
void UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingsManagerImpl_NewLL(  )
    {
    RFs fs;
    CleanupClosePushL( fs );
    User::LeaveIfError( fs.Connect() );
    CCFContextSourceSettingsManagerImpl* obj =
        CCFContextSourceSettingsManagerImpl::NewL( fs );
    CleanupStack::PushL( obj );
    
    EUNIT_ASSERT_DESC( obj,
        "CCFContextSourceSettingsManager instance not created!" );
        
    CleanupStack::PopAndDestroy( 2, &fs );
    }

//-----------------------------------------------------------------------------
// UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingsManagerImpl_NewLCL
//-----------------------------------------------------------------------------
//
void UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingsManagerImpl_NewLCL(  )
    {
    RFs fs;
    CleanupClosePushL( fs );
    User::LeaveIfError( fs.Connect() );
    CCFContextSourceSettingsManagerImpl* obj =
        CCFContextSourceSettingsManagerImpl::NewLC( fs );
    
    EUNIT_ASSERT_DESC( obj,
        "CCFContextSourceSettingsManager instance not created!" );
        
    CleanupStack::PopAndDestroy( 2, &fs );
    }

//-----------------------------------------------------------------------------
// UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingsManagerImpl_ParseSettingsLL
//-----------------------------------------------------------------------------
//
void UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingsManagerImpl_ParseSettingsLL(  )
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
    static_cast<CCFContextSourceSettingsManager*>(
        iContextSourceSettingsManagerImpl )->ParseSettingsL(
            KSettingsPath,
            *array );
            
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
// UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingArray_NewLL
//-----------------------------------------------------------------------------
//
void UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingArray_NewLL()
    {
    CCFContextSourceSettingArray* array = CCFContextSourceSettingArray::NewL();
    CleanupStack::PushL( array );
    
    EUNIT_ASSERT_DESC( array, "CCFContextSourceSettingArray class instance not created!" )
    
    CleanupStack::PopAndDestroy( array );
    }

//-----------------------------------------------------------------------------
// UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingArray_NewLCL
//-----------------------------------------------------------------------------
//
void UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingArray_NewLCL()
    {
    CCFContextSourceSettingArray* array = CCFContextSourceSettingArray::NewLC();
    
    EUNIT_ASSERT_DESC( array, "CCFContextSourceSettingArray class instance not created!" )
    
    CleanupStack::PopAndDestroy( array );
    }

//-----------------------------------------------------------------------------
// UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingArrayImpl_NewLL
//-----------------------------------------------------------------------------
//
void UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingArrayImpl_NewLL()
    {
    CCFContextSourceSettingArrayImpl* array = CCFContextSourceSettingArrayImpl::NewL();
    CleanupStack::PushL( array );
    
    EUNIT_ASSERT_DESC( array, "CCFContextSourceSettingArrayImpl class instance not created!" )
    
    CleanupStack::PopAndDestroy( array );
    }

//-----------------------------------------------------------------------------
// UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingArrayImpl_NewLCL
//-----------------------------------------------------------------------------
//
void UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingArrayImpl_NewLCL()
    {
    CCFContextSourceSettingArrayImpl* array = CCFContextSourceSettingArrayImpl::NewLC();
    
    EUNIT_ASSERT_DESC( array, "CCFContextSourceSettingArrayImpl class instance not created!" )
    
    CleanupStack::PopAndDestroy( array );
    }

//-----------------------------------------------------------------------------
// UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingArrayImpl_AppendItemLL
//-----------------------------------------------------------------------------
//
void UT_CFContextSourceSettingsManager::UT_CCFContextSourceSettingArrayImpl_AppendItemLL()
    {
    CCFContextSourceSettingImpl* settingItem = NULL;
    CCFContextSourceSettingArray* array = CCFContextSourceSettingArray::NewLC();
    
    EUNIT_ASSERT_DESC( !array->Count(), "Invalid setting array count!" );

    _LIT( KFormat, "Setting item %d" );
    TBuf<20> name( KNullDesC );
    static const TInt KCount = 5;
    for( TInt i = 0; i < KCount; i++ )
        {
        name.Format( KFormat, i );
        settingItem = CCFContextSourceSettingImpl::NewLC();
        settingItem->SetNameL( name );
        array->AppendItemL( settingItem );
        CleanupStack::Pop( settingItem );
        }
        
    EUNIT_ASSERT_DESC( array->Count() == KCount, "Invalid setting array count!" );
    for( TInt i = 0; i < KCount; i++ )
        {
        name.Format( KFormat, i );
        EUNIT_ASSERT_DESC( array->Item( i ).Name() == name, "Invalid setting item!" );
        }
    
    // Clean up
    CleanupStack::PopAndDestroy( array );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CFContextSourceSettingsManager,
    "CFContextSourceSettingsManager unit test.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CCFContextSourceSettingParameterImpl",
    "NewL",
    "FUNCTIONALITY",
    SetupEmptyL, UT_CCFContextSourceSettingParameterImpl_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFContextSourceSettingParameterImpl",
    "NewLC",
    "FUNCTIONALITY",
    SetupEmptyL, UT_CCFContextSourceSettingParameterImpl_NewLCL, Teardown)

EUNIT_TEST(
    "Attributes - test ",
    "CCFContextSourceSettingParameterImpl",
    "Attributes",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextSourceSettingParameterImpl_AttributesL, Teardown)

EUNIT_TEST(
    "Name - test ",
    "CCFContextSourceSettingParameterImpl",
    "Name",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextSourceSettingParameterImpl_NameL, Teardown)

EUNIT_TEST(
    "NewL - test ",
    "CCFContextSourceSettingImpl",
    "NewL",
    "FUNCTIONALITY",
    SetupEmptyL, UT_CCFContextSourceSettingImpl_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFContextSourceSettingImpl",
    "NewLC",
    "FUNCTIONALITY",
    SetupEmptyL, UT_CCFContextSourceSettingImpl_NewLCL, Teardown)

EUNIT_TEST(
    "Name - test ",
    "CCFContextSourceSettingImpl",
    "Name",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextSourceSettingImpl_NameL, Teardown)

EUNIT_TEST(
    "Attributes - test ",
    "CCFContextSourceSettingImpl",
    "Attributes",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextSourceSettingImpl_AttributesL, Teardown)

EUNIT_TEST(
    "NumOfParameters - test ",
    "CCFContextSourceSettingImpl",
    "NumOfParameters",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextSourceSettingImpl_NumOfParametersL, Teardown)

EUNIT_TEST(
    "Parameters - test ",
    "CCFContextSourceSettingImpl",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextSourceSettingImpl_ParametersL, Teardown)

EUNIT_TEST(
    "NewL - test ",
    "CCFContextSourceSettingsManagerImpl",
    "NewL",
    "FUNCTIONALITY",
    SetupEmptyL, UT_CCFContextSourceSettingsManagerImpl_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFContextSourceSettingsManagerImpl",
    "NewLC",
    "FUNCTIONALITY",
    SetupEmptyL, UT_CCFContextSourceSettingsManagerImpl_NewLCL, Teardown)

EUNIT_TEST(
    "ParseSettingsL - test ",
    "CCFContextSourceSettingsManagerImpl",
    "ParseSettingsL",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextSourceSettingsManagerImpl_ParseSettingsLL, Teardown)

EUNIT_TEST(
    "NewL - test ",
    "CCFContextSourceSettingArray",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextSourceSettingArray_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFContextSourceSettingArray",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextSourceSettingArray_NewLCL, Teardown)

EUNIT_TEST(
    "NewL - test ",
    "CCFContextSourceSettingArrayImpl",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextSourceSettingArrayImpl_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFContextSourceSettingArrayImpl",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextSourceSettingArrayImpl_NewLCL, Teardown)

EUNIT_TEST(
    "AppendItemL - test ",
    "CCFContextSourceSettingArrayImpl",
    "AppendItemL",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextSourceSettingArrayImpl_AppendItemLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
