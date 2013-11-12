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
* Description:  ?Description
*
*/


#include <e32cmn.h>
#include <S32MEM.h>

#include <CFContextObject.h>
#include <CFContextInterface.h>
#include <cfcontextsourcesetting.h>
#include <cfcontextsourcesettingarray.h>
#include <cfcontextsubscription.h>
#include <cfcontextindication.h>

#include "TestContextSourcePlugIn.h"
#include "cfpropertylistener.h"
#include "cfcontextobjectimpl.h"
#include "testcontextdata.h"
#include "TestContextSourcePluginConst.hrh"

// CONSTANTS

// Security policy
_LIT_SECURITY_POLICY_PASS( KSecurity );

// Panic category
_LIT( KPanicCat, "TestContextSource" );

// Panic reason
enum TPanicReason
    {
    EHandleSettingNotCalled,
    EHandleSettingIncorrectParams,
    EDefineContextsNotCalled,
    EInitializeNotCalled
    };
    
// Configuration file 'testcontextsourcesettings1.xml'
_LIT( KSettingItem1, "SettingItem1" );
_LIT( KAttribute1, "Attribute1" );
_LIT( KAttrvalue1, "Attrvalue1" );
_LIT( KSettingItemParam1, "SettingItemParam1" );
_LIT( KParam1, "Param1" );
_LIT( KParamvalue1, "Paramvalue1" );

const TInt KSettingItem1Count = 2;
const TInt KAttribute1Count = 3;
const TInt KAttrvalue1Count = 3;
const TInt KSettingItemParam1Count = 3;
const TInt KParam1Count = 4;
const TInt KParamvalue1Count = 4;

// Configuration file 'testcontextsourcesettings2.xml'
_LIT( KSettingItem2, "SettingItem2" );
_LIT( KAttribute2, "Attribute2" );
_LIT( KAttrvalue2, "Attrvalue2" );
_LIT( KSettingItemParam2, "SettingItemParam2" );
_LIT( KParam2, "Param2" );
_LIT( KParamvalue2, "Paramvalue2" );

const TInt KSettingItem2Count = 3;
const TInt KAttribute2Count = 3;
const TInt KAttrvalue2Count = 3;
const TInt KSettingItemParam2Count = 3;
const TInt KParam2Count = 4;
const TInt KParamvalue2Count = 4;

const TInt KNumOfSettingItems = 5;

// Configuration file 'testcontextsourcesettings3.xml'
_LIT( KSettingItem3, "SettingItem3" );
_LIT( KAttribute3, "Attribute3" );
_LIT( KAttrvalue3, "Attrvalue3" );
_LIT( KSettingItemParam3, "SettingItemParam3" );
_LIT( KParam3, "Param3" );
_LIT( KParamvalue3, "Paramvalue3" );

const TInt KSettingItem3Count = 2;
const TInt KAttribute3Count = 3;
const TInt KAttrvalue3Count = 3;
const TInt KSettingItemParam3Count = 3;
const TInt KParam3Count = 4;
const TInt KParamvalue3Count = 4;

const TInt KNumOfSettingItemsAfterInstall3 = 7;

// Configuration file 'testcontextsourcesettings4.xml'
_LIT( KSettingItem4, "SettingItem4" );

const TInt KSettingItem4Count = 1;

// Configuration file 'testcontextsourcesettings5.xml'
_LIT( KSettingItem5, "SettingItem5" );
_LIT( KAttribute5, "Attribute5" );
_LIT( KAttrvalue5, "Attrvalue5" );
_LIT( KSettingItemParam5, "SettingItemParam5" );
_LIT( KParam5, "Param5" );
_LIT( KParamvalue5, "Paramvalue5" );

const TInt KSettingItem5Count = 1;
const TInt KAttribute5Count = 1;
const TInt KAttrvalue5Count = 1;
const TInt KSettingItemParam5Count = 1;
const TInt KParam5Count = 1;
const TInt KParamvalue5Count = 1;

const TInt KNumOfSettingItemsAfterInstall3And5 = 8;

// Configuration file 'settingupgrade\testcontextsourcesettings3.xml'
_LIT( KSettingItem6, "SettingItem6" );
_LIT( KAttribute6, "Attribute6" );
_LIT( KAttrvalue6, "Attrvalue6" );
_LIT( KSettingItemParam6, "SettingItemParam6" );
_LIT( KParam6, "Param6" );
_LIT( KParamvalue6, "Paramvalue6" );

// Counts are the same as for setting item 5, using them

const TInt KNumOfSettingItemsAfterUpgrading3 = 6;

// Configuration file 'testcontextsourcesettings7.xml'
_LIT( KSettingItem7, "SettingItem7" );

const TInt KSettingItem7Count = 1;


// Context definitions
_LIT( KTestContextSource, "TestContextSource" );
_LIT( KTestContextType, "SubsNoSubsTest" );
_LIT( KTestContextType2, "SubsNoSubsTest2" );
_LIT( KTestContextValueType, "TestValue" );
_LIT( KTestContextValueSubsOk, "SubsOk" );
_LIT( KTestContextValueNosubsOk, "NoSubsOk" );
_LIT( KTestContextValueNok, "Nok" );

const TUid KEUnitSid = {0x20000FB1};

_LIT( KSourceCommandId, "sourceCommand" );
_LIT( KSourceCommandUidId, "uid" );
_LIT( KSourceCommandParamCmd, "cmd" );
_LIT( KSourceCommandAttributeType, "type" );
_LIT( KSourceCommandAttributeValueActivate, "Activate" );
_LIT( KSourceCommandAttributeValueDeactivate, "Deactivate" );
_LIT( KSourceCommandSignalContextSource, "TestSourcePlugin" );
_LIT( KSourceCommandSignalContextType, "SourceCommand" );
_LIT( KSourceCommandSignalContextValueActivated, "Activated" );
_LIT( KSourceCommandSignalContextValueDeactivated, "Deactivated" );
_LIT( KSourceCommandSignalContextValueActiveReqActivate, "ActiveReqActivate" );
_LIT( KSourceCommandSignalContextValueActiveReqDeactivate, "ActiveReqDeactivate" );
_LIT( KSourceCommandSignalContextValueDeactiveReqDeactivate, "DeactiveReqDeactivate" );
_LIT( KSourceCommandSignalContextValueError, "Error" );

// Help macros
#define __ASSERT( x )\
    {\
    if( !x )\
        {\
        User::Panic( KPanicCat, EHandleSettingIncorrectParams );\
        }\
    }

#define __ASSERT_CODE( x, code )\
    {\
    if( !x )\
        {\
        User::Panic( KPanicCat, code );\
        }\
    }

/**
 * Orders source command sender identities.
 * @param aFirst First source command sender id.
 * @param aSecond Second source command sender id.
 * @return Zero if the uid and the script id of the first and second are equal.
 *   Negative value if the uid of the first is less than the uid of the second
 *   or script id of the first is less than second's when uid are equal.
 *   Positive value if the uid of the first is greater than the uid of the
 *   second or script id of the first is greater than second's when uid are
 *   equal.
 */
 LOCAL_C TInt SourceCommandSenderIdOrder(
     const TCFSourceCommandSenderId& aFirst,
     const TCFSourceCommandSenderId& aSecond )
     {
     TInt orderValue = 1;

     if ( aFirst.iSender == aSecond.iSender )
         {
         if ( aFirst.iScriptId == aSecond.iScriptId )
             {
             orderValue = 0;
             }
         else if ( aFirst.iScriptId < aSecond.iScriptId )
             {
             orderValue = -1;
             }
         }
     else if ( aFirst.iSender < aSecond.iSender )
         {
         orderValue = -1;
         }

     return orderValue;
     }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestContextSourcePlugIn::CTestContextSourcePlugIn
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTestContextSourcePlugIn::CTestContextSourcePlugIn( 
    TContextSourceInitParams* aParams ) : CCFContextSourcePlugIn( aParams )
    {
    }

// -----------------------------------------------------------------------------
// CTestContextSourcePlugIn::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTestContextSourcePlugIn* CTestContextSourcePlugIn::NewL( 
TContextSourceInitParams* aParams )
    {
    CTestContextSourcePlugIn* self = CTestContextSourcePlugIn::NewLC(
         aParams );
        
    CleanupStack::Pop();
    
    return self;
    }
  
// -----------------------------------------------------------------------------
// CTestContextSourcePlugIn::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTestContextSourcePlugIn* CTestContextSourcePlugIn::NewLC( 
TContextSourceInitParams* aParams )
    {
    CTestContextSourcePlugIn* self 
    	= new( ELeave ) CTestContextSourcePlugIn( aParams );
    self->ConstructL();
    CleanupStack::PushL( self );
    
    return self;
    }
  
// Destructor
CTestContextSourcePlugIn::~CTestContextSourcePlugIn()
    {
    delete iPSListenerA;
    TInt check = 0;
    TInt err = RProperty::Get( KContextSourceTestCategory, KDisableChecks, check );
    if( err != KErrNone )
        {
        check = 0;
        }
    if( check )
        {
        __ASSERT_CODE( iHandleSettings, EHandleSettingNotCalled );
        __ASSERT_CODE( iDefineContexts, EDefineContextsNotCalled );
        __ASSERT_CODE( iInitialize, EInitializeNotCalled );
        }

    if ( iSubscriberSub1Owned )
        {
        delete iSubscriberSub1;
        iSubscriberSub1 = NULL;
        }
    if ( iSubscriberSub2Owned )
        {
        delete iSubscriberSub2;
        iSubscriberSub2 = NULL;
        }

    iActiveRequests.Close();
    }
    
// 2nd pahse constructor
void CTestContextSourcePlugIn::ConstructL()
    {
    RProperty::Define( KContextSourceTestCategory,
        KPublishContextKey,
        RProperty::ELargeByteArray,
        KSecurity,
        KSecurity );
    RProperty::Define( KContextSourceTestCategory,
        KDisableChecks,
        RProperty::EInt,
        KSecurity,
        KSecurity );
    iPSListenerA = CCFPropertyListener::NewL( KContextSourceTestCategory,
        KPublishContextKey,
        *this );
    }

// METHODS

TAny* CTestContextSourcePlugIn::Extension( const TUid& aExtensionUid ) const
    {
    if ( aExtensionUid == KCFContextSourceInterfaceUid )
        {
        return ( ( MCFContextSource* ) this );
        }
    else if ( aExtensionUid == KCFContextSourceUpgradeUid )
        {
        return ( ( MCFContextSourceUpgrade* ) this );
        }
    else
        {
        return NULL;
        }
    }

void CTestContextSourcePlugIn::HandleSettingL(
    CCFContextSourceSettingArray* aSettingList )
	{
	CleanupStack::PushL( aSettingList );
	iHandleSettings = ETrue;
	
	// We should have five setting items
	TInt count = aSettingList->Count();
	__ASSERT( count == KNumOfSettingItems );
	
	// Check names
    const CCFContextSourceSetting* settingItem = NULL;
    TInt settingItem1Count = 0;
    TInt attribute1Count = 0;
    TInt settingItemParam1Count = 0;
    TInt settingItem2Count = 0;
    TInt attribute2Count = 0;
    TInt settingItemParam2Count = 0;
	for( TInt i = 0; i < count; i++ )
	    {
	    settingItem = &aSettingList->Item( i );
	    if( settingItem->Name() == KSettingItem1 )
	        {
	        settingItem1Count++;
	        const RKeyValueArray& attributes = settingItem->Attributes();
	        attribute1Count += attributes.Count();
	        for( TInt j = 0; j < attributes.Count(); j++ )
	            {
	            __ASSERT( attributes[j]->Key().CompareF( KAttribute1) == KErrNone );
	            __ASSERT( attributes[j]->Value().CompareF( KAttrvalue1 ) == KErrNone );
	            }
	        const RSettingParameterArray& parameters = settingItem->Parameters();
	        settingItemParam1Count += parameters.Count();
	        for( TInt j = 0; j < parameters.Count(); j++ )
	            {
                __ASSERT( parameters[j]->Name().CompareF( KSettingItemParam1 ) == KErrNone );
                const RKeyValueArray& paramAttributes = parameters[j]->Attributes();
                for( TInt x = 0; x < paramAttributes.Count(); x++ )
                    {
                    __ASSERT( paramAttributes[x]->Key().CompareF( KParam1 ) == KErrNone );
                    __ASSERT( paramAttributes[x]->Value().CompareF( KParamvalue1 ) == KErrNone );
                    }
	            }
	        }
	    else if( settingItem->Name() == KSettingItem2 )
	        {
	        settingItem2Count++;
	        const RKeyValueArray& attributes = settingItem->Attributes();
	        attribute2Count += attributes.Count();
	        for( TInt j = 0; j < attributes.Count(); j++ )
	            {
	            __ASSERT( attributes[j]->Key().CompareF( KAttribute2 ) == KErrNone );
	            __ASSERT( attributes[j]->Value().CompareF( KAttrvalue2 ) == KErrNone );
	            }
	        const RSettingParameterArray& parameters = settingItem->Parameters();
	        settingItemParam2Count += parameters.Count();
	        for( TInt j = 0; j < parameters.Count(); j++ )
	            {
                __ASSERT( parameters[j]->Name().CompareF( KSettingItemParam2 ) == KErrNone );
                const RKeyValueArray& paramAttributes = parameters[j]->Attributes();
                for( TInt x = 0; x < paramAttributes.Count(); x++ )
                    {
                    __ASSERT( paramAttributes[x]->Key().CompareF( KParam2 ) == KErrNone );
                    __ASSERT( paramAttributes[x]->Value().CompareF( KParamvalue2 ) == KErrNone );
                    }
	            }
	        }
        else
            {
            __ASSERT( EFalse );
            }
	    }
    __ASSERT( settingItem1Count == KSettingItem1Count );
    __ASSERT( attribute1Count == KAttribute1Count );
    __ASSERT( settingItemParam1Count == KSettingItemParam1Count );
	
    __ASSERT( settingItem2Count == KSettingItem2Count );
    __ASSERT( attribute2Count == KAttribute2Count );
    __ASSERT( settingItemParam2Count == KSettingItemParam2Count );

    CleanupStack::PopAndDestroy( aSettingList );
	}

void CTestContextSourcePlugIn::DefineContextsL()
    {
    _LIT_SECURITY_POLICY_PASS( KPassSec );

    iDefineContexts = ETrue;
    iCF.DefineContext( KTestContextSource, KTestContextType, KPassSec, *this );
    iCF.DefineContext( KTestContextSource, KTestContextType2, KPassSec, *this );
    iCF.DefineContext( KTestContextSource, KTestContextValueType, KPassSec );
    iCF.DefineContext( KSourceCommandSignalContextSource, KSourceCommandSignalContextType, KPassSec );
    }

void CTestContextSourcePlugIn::InitializeL()
    {
    iInitialize = ETrue;
    }

void CTestContextSourcePlugIn::HandlePropertyChangedL()
    {
    HBufC8* buf = HBufC8::NewLC( 1024 );
    TPtr8 bufPtr( buf->Des() );
    TInt err = iPSListenerA->Property().Get( bufPtr );
    if( err == KErrNone )
        {
        CCFContextObjectImpl* context = CCFContextObjectImpl::NewLC();
        RDesReadStream stream( bufPtr );
        stream.PushL();
        context->InternalizeL( stream );
        CTestContextData* contextData =
            CTestContextData::NewLC( _L("Key_A"), _L("Value_A") );
            
        RThread thread;
        err = iCF.PublishContext( *context, *contextData, thread );
        thread.Close();
        
        // Clean up
        CleanupStack::PopAndDestroy( contextData );
        CleanupStack::PopAndDestroy( &stream );
        CleanupStack::PopAndDestroy( context );
        }
    
    // Clean up
    CleanupStack::PopAndDestroy( buf );
    }

// from MCFContextSource
void CTestContextSourcePlugIn::Subscribers( const TDesC& aContextSource,
    const TDesC& aContextType )
    {
    CCFContextObjectImpl* context = NULL;
    TRAPD( err, context = ContextWithValueForSubscriberNotifyL( aContextSource,
            aContextType,
            KTestContextValueSubsOk ) );

    if ( err == KErrNone && context )
        {
        RThread thread;
        err = iCF.PublishContext( *context, thread );
        thread.Close();
        }

    delete context;
    }

// from MCFContextSource
void CTestContextSourcePlugIn::NoSubscribers( const TDesC& aContextSource,
    const TDesC& aContextType )
    {
    CCFContextObjectImpl* context = NULL;
    TRAPD( err, context = ContextWithValueForSubscriberNotifyL( aContextSource,
            aContextType,
            KTestContextValueNosubsOk ) );

    if ( err == KErrNone && context )
        {
        RThread thread;
        err = iCF.PublishContext( *context, thread );
        thread.Close();
        }

    delete context;
    }

// from MCFContextSource
void CTestContextSourcePlugIn::HandleCommand(
    const CCFContextSourceCommand& aCommand )
    {
    TPtrC id( aCommand.Name() );
    if ( id.CompareF( KSourceCommandId ) != 0 )
        {
        return; // Incorrect command indentifier
        }

    // Parse context source command
    const RKeyValueArray& attributes = aCommand.Attributes();
    TInt count = attributes.Count();
    if ( count != 1 )
        {
        return; // Wrong attribute count, skip publish context
        }

    TUid sourceUid = aCommand.SourceUid();
    if ( sourceUid != TUid::Uid( KTestContextSourcePluginImplementationUid ) )
        {
        return; // Wrong uid value, skip publish context
        }

    const RSourceCommandParameterArray& params = aCommand.Parameters();
    count = params.Count();
    if ( count != 1 )
        {
        return; // Wrong attribute count, skip publish context
        }

    TPtrC paramName( params[ 0 ]->Name() );
    if ( paramName.CompareF( KSourceCommandParamCmd ) != 0 )
        {
        return; // Incorrect command indentifier
        }

    const RKeyValueArray& paramAttributes = params[ 0 ]->Attributes();
    count = paramAttributes.Count();
    if ( count != 1 )
        {
        return; // Wrong attribute count, skip publish context
        }

    const RSourceCommandParameterArray& nestedParams = params[ 0 ]->Parameters();
    count = nestedParams.Count();
    if ( count != 0 )
        {
        return; // Wrong attribute count, skip publish context
        }

    TPtrC key( paramAttributes[ 0 ]->Key() );
    TPtrC value( paramAttributes[ 0 ]->Value() );
    if( key.CompareF( KSourceCommandAttributeType ) != 0 )
        {
        return; // Wrong attribute value, skip publish context
        }

    TPtrC responseValue( KNullDesC );
    GetSourceCommandResponseValue( responseValue, aCommand, value );

    // below                                        // CLEANUP<< context
    CCFContextObjectImpl* context = CCFContextObjectImpl::NewLC();
    context->SetSourceL( KSourceCommandSignalContextSource );
    context->SetTypeL( KSourceCommandSignalContextType );
    context->SetValueL( responseValue );

    RThread thread;
    TInt err = iCF.PublishContext( *context, thread );
    thread.Close();

    CleanupStack::PopAndDestroy( context );         // CLEANUP>> context
    }

// from MCFContextSourceUpgrade
TInt CTestContextSourcePlugIn::CheckValidity( RThread& aClientThread,
    const CCFContextSourceSettingArray& aSettingList )
    {
    // Request should be coming from EUnit
    TSecureId clientSid = aClientThread.SecureId();
    __ASSERT( ( KEUnitSid == clientSid ) );

    TInt err = KErrGeneral;

    // Check which settings we are installing
    TInt count = aSettingList.Count();
    __ASSERT( count > 0 );

    const CCFContextSourceSetting& settingItem = aSettingList.Item( 0 );
    if( settingItem.Name() == KSettingItem3 )
        {
        __ASSERT( count == KSettingItem3Count );
        err = KErrNone;
        }
    else if( settingItem.Name() == KSettingItem4 )
        {
        __ASSERT( count == KSettingItem4Count );
        err = KErrPermissionDenied;
        }
    else if( settingItem.Name() == KSettingItem5
        || settingItem.Name() == KSettingItem6 )
        {
        __ASSERT( count == KSettingItem5Count );
        err = KErrNone;
        }
    else if( settingItem.Name() == KSettingItem7 )
            {
            __ASSERT( count == KSettingItem7Count );
            err = KErrNone;
            }

    return err;
    }

// from MCFContextSourceUpgrade
void CTestContextSourcePlugIn::UpdateSettingsL(
    CCFContextSourceSettingArray* aSettingList )
    {
    CleanupStack::PushL( aSettingList );
    
    // Decide based on last setting item the required count
    TInt count = aSettingList->Count();
    if ( count > 0 )
        {
        const CCFContextSourceSetting& checkSettingItem
                = aSettingList->Item( count - 1 );
        if( checkSettingItem.Name() == KSettingItem3 )
            {
            __ASSERT( count == KNumOfSettingItemsAfterInstall3 );
            }
        else if( checkSettingItem.Name() == KSettingItem2 )
            {
            __ASSERT( count == KNumOfSettingItems );
            }
        else if( checkSettingItem.Name() == KSettingItem5 )
            {
            __ASSERT( count == KNumOfSettingItemsAfterInstall3And5 );
            }
        else if( checkSettingItem.Name() == KSettingItem6 )
            {
            __ASSERT( count == KNumOfSettingItemsAfterUpgrading3 );
            }
        else if( checkSettingItem.Name() == KSettingItem7 )
            {
            User::Leave( KErrGeneral );
            }
        }
    
    // Check names
    const CCFContextSourceSetting* settingItem = NULL;
    TInt settingItem1Count = 0;
    TInt attribute1Count = 0;
    TInt settingItemParam1Count = 0;
    TInt settingItem2Count = 0;
    TInt attribute2Count = 0;
    TInt settingItemParam2Count = 0;
    TInt settingItem3Count = 0;
    TInt attribute3Count = 0;
    TInt settingItemParam3Count = 0;
    TInt settingItem5Count = 0;
    TInt attribute5Count = 0;
    TInt settingItemParam5Count = 0;
    TInt settingItem6Count = 0;
    TInt attribute6Count = 0;
    TInt settingItemParam6Count = 0;
    for( TInt i = 0; i < count; i++ )
        {
        settingItem = &aSettingList->Item( i );
        if( settingItem->Name() == KSettingItem1 )
            {
            settingItem1Count++;
            const RKeyValueArray& attributes = settingItem->Attributes();
            attribute1Count += attributes.Count();
            for( TInt j = 0; j < attributes.Count(); j++ )
                {
                __ASSERT( attributes[j]->Key().CompareF( KAttribute1) == KErrNone );
                __ASSERT( attributes[j]->Value().CompareF( KAttrvalue1 ) == KErrNone );
                }
            const RSettingParameterArray& parameters = settingItem->Parameters();
            settingItemParam1Count += parameters.Count();
            for( TInt j = 0; j < parameters.Count(); j++ )
                {
                __ASSERT( parameters[j]->Name().CompareF( KSettingItemParam1 ) == KErrNone );
                const RKeyValueArray& paramAttributes = parameters[j]->Attributes();
                for( TInt x = 0; x < paramAttributes.Count(); x++ )
                    {
                    __ASSERT( paramAttributes[x]->Key().CompareF( KParam1 ) == KErrNone );
                    __ASSERT( paramAttributes[x]->Value().CompareF( KParamvalue1 ) == KErrNone );
                    }
                }
            }
        else if( settingItem->Name() == KSettingItem2 )
            {
            settingItem2Count++;
            const RKeyValueArray& attributes = settingItem->Attributes();
            attribute2Count += attributes.Count();
            for( TInt j = 0; j < attributes.Count(); j++ )
                {
                __ASSERT( attributes[j]->Key().CompareF( KAttribute2 ) == KErrNone );
                __ASSERT( attributes[j]->Value().CompareF( KAttrvalue2 ) == KErrNone );
                }
            const RSettingParameterArray& parameters = settingItem->Parameters();
            settingItemParam2Count += parameters.Count();
            for( TInt j = 0; j < parameters.Count(); j++ )
                {
                __ASSERT( parameters[j]->Name().CompareF( KSettingItemParam2 ) == KErrNone );
                const RKeyValueArray& paramAttributes = parameters[j]->Attributes();
                for( TInt x = 0; x < paramAttributes.Count(); x++ )
                    {
                    __ASSERT( paramAttributes[x]->Key().CompareF( KParam2 ) == KErrNone );
                    __ASSERT( paramAttributes[x]->Value().CompareF( KParamvalue2 ) == KErrNone );
                    }
                }
            }
        else if( settingItem->Name() == KSettingItem3 )
            {
            settingItem3Count++;
            const RKeyValueArray& attributes = settingItem->Attributes();
            attribute3Count += attributes.Count();
            for( TInt j = 0; j < attributes.Count(); j++ )
                {
                __ASSERT( attributes[j]->Key().CompareF( KAttribute3 ) == KErrNone );
                __ASSERT( attributes[j]->Value().CompareF( KAttrvalue3 ) == KErrNone );
                }
            const RSettingParameterArray& parameters = settingItem->Parameters();
            settingItemParam3Count += parameters.Count();
            for( TInt j = 0; j < parameters.Count(); j++ )
                {
                __ASSERT( parameters[j]->Name().CompareF( KSettingItemParam3 ) == KErrNone );
                const RKeyValueArray& paramAttributes = parameters[j]->Attributes();
                for( TInt x = 0; x < paramAttributes.Count(); x++ )
                    {
                    __ASSERT( paramAttributes[x]->Key().CompareF( KParam3 ) == KErrNone );
                    __ASSERT( paramAttributes[x]->Value().CompareF( KParamvalue3 ) == KErrNone );
                    }
                }
            }
        else if( settingItem->Name() == KSettingItem5 )
            {
            settingItem5Count++;
            const RKeyValueArray& attributes = settingItem->Attributes();
            attribute5Count += attributes.Count();
            for( TInt j = 0; j < attributes.Count(); j++ )
                {
                __ASSERT( attributes[j]->Key().CompareF( KAttribute5 ) == KErrNone );
                __ASSERT( attributes[j]->Value().CompareF( KAttrvalue5 ) == KErrNone );
                }
            const RSettingParameterArray& parameters = settingItem->Parameters();
            settingItemParam5Count += parameters.Count();
            for( TInt j = 0; j < parameters.Count(); j++ )
                {
                __ASSERT( parameters[j]->Name().CompareF( KSettingItemParam5 ) == KErrNone );
                const RKeyValueArray& paramAttributes = parameters[j]->Attributes();
                for( TInt x = 0; x < paramAttributes.Count(); x++ )
                    {
                    __ASSERT( paramAttributes[x]->Key().CompareF( KParam5 ) == KErrNone );
                    __ASSERT( paramAttributes[x]->Value().CompareF( KParamvalue5 ) == KErrNone );
                    }
                }
            }
        else if( settingItem->Name() == KSettingItem6 )
            {
            settingItem6Count++;
            const RKeyValueArray& attributes = settingItem->Attributes();
            attribute6Count += attributes.Count();
            for( TInt j = 0; j < attributes.Count(); j++ )
                {
                __ASSERT( attributes[j]->Key().CompareF( KAttribute6 ) == KErrNone );
                __ASSERT( attributes[j]->Value().CompareF( KAttrvalue6 ) == KErrNone );
                }
            const RSettingParameterArray& parameters = settingItem->Parameters();
            settingItemParam6Count += parameters.Count();
            for( TInt j = 0; j < parameters.Count(); j++ )
                {
                __ASSERT( parameters[j]->Name().CompareF( KSettingItemParam6 ) == KErrNone );
                const RKeyValueArray& paramAttributes = parameters[j]->Attributes();
                for( TInt x = 0; x < paramAttributes.Count(); x++ )
                    {
                    __ASSERT( paramAttributes[x]->Key().CompareF( KParam6 ) == KErrNone );
                    __ASSERT( paramAttributes[x]->Value().CompareF( KParamvalue6 ) == KErrNone );
                    }
                }
            }
        else
            {
            __ASSERT( EFalse );
            }
        }
    __ASSERT( settingItem1Count == KSettingItem1Count );
    __ASSERT( attribute1Count == KAttribute1Count );
    __ASSERT( settingItemParam1Count == KSettingItemParam1Count );

    __ASSERT( settingItem2Count == KSettingItem2Count );
    __ASSERT( attribute2Count == KAttribute2Count );
    __ASSERT( settingItemParam2Count == KSettingItemParam2Count );

    // Check counts based on last setting item.
    if( settingItem->Name() == KSettingItem3 )
        {
        __ASSERT( settingItem3Count == KSettingItem3Count );
        __ASSERT( attribute3Count == KAttribute3Count );
        __ASSERT( settingItemParam3Count == KSettingItemParam3Count );

        __ASSERT( settingItem5Count == 0 );
        __ASSERT( attribute5Count == 0 );
        __ASSERT( settingItemParam5Count == 0 );

        __ASSERT( settingItem6Count == 0 );
        __ASSERT( attribute6Count == 0 );
        __ASSERT( settingItemParam6Count == 0 );
        }
    else if( settingItem->Name() == KSettingItem5 )
        {
        __ASSERT( settingItem3Count == KSettingItem3Count );
        __ASSERT( attribute3Count == KAttribute3Count );
        __ASSERT( settingItemParam3Count == KSettingItemParam3Count );

        __ASSERT( settingItem5Count == KSettingItem5Count );
        __ASSERT( attribute5Count == KAttribute5Count );
        __ASSERT( settingItemParam5Count == KSettingItemParam5Count );

        __ASSERT( settingItem6Count == 0 );
        __ASSERT( attribute6Count == 0 );
        __ASSERT( settingItemParam6Count == 0 );
        }
    else if( settingItem->Name() == KSettingItem6 )
        {
        __ASSERT( settingItem3Count == 0 );
        __ASSERT( attribute3Count == 0 );
        __ASSERT( settingItemParam3Count == 0 );

        __ASSERT( settingItem5Count == 0 );
        __ASSERT( attribute5Count == 0 );
        __ASSERT( settingItemParam5Count == 0 );

        __ASSERT( settingItem6Count == KSettingItem5Count );
        __ASSERT( attribute6Count == KAttribute5Count );
        __ASSERT( settingItemParam6Count == KSettingItemParam5Count );
        }
    else
        {
        __ASSERT( settingItem3Count == 0 );
        __ASSERT( attribute3Count == 0 );
        __ASSERT( settingItemParam3Count == 0 );

        __ASSERT( settingItem5Count == 0 );
        __ASSERT( attribute5Count == 0 );
        __ASSERT( settingItemParam5Count == 0 );

        __ASSERT( settingItem6Count == 0 );
        __ASSERT( attribute6Count == 0 );
        __ASSERT( settingItemParam6Count == 0 );
        }

    CleanupStack::PopAndDestroy( aSettingList );
    }

// ---------------------------------------------------------------------------
// CTestContextSourcePlugIn::ContextIndicationL
// ---------------------------------------------------------------------------
//
void CTestContextSourcePlugIn::ContextIndicationL(
    CCFContextIndication* aIndication )
    {
    delete aIndication;
    }

// ---------------------------------------------------------------------------
// CTestContextSourcePlugIn::Client
// ---------------------------------------------------------------------------
//
TInt CTestContextSourcePlugIn::Client( RThread& aThread ) const
    {
    aThread = RThread();
    return KErrNone;
    }
        
// ---------------------------------------------------------------------------
// CTestContextSourcePlugIn::HandleContextSubscriptionError
// ---------------------------------------------------------------------------
//
void CTestContextSourcePlugIn::HandleContextSubscriptionError( TInt /*aError*/,
    const TDesC& /*aSource*/,
    const TDesC& /*aType*/ )
    {
    // Nothing to do.
    }

// ---------------------------------------------------------------------------
// CTestContextSourcePlugIn::ContextWithValueForSubscriberNotifyL
// ---------------------------------------------------------------------------
//
CCFContextObjectImpl*
CTestContextSourcePlugIn::ContextWithValueForSubscriberNotifyL(
    const TDesC& aSubscriberContextSource,
    const TDesC& aSubscriberContextType,
    const TDesC& aOkValueForNewContext )
    {
    if ( aOkValueForNewContext == KTestContextValueSubsOk )
        {
        if ( !iObservingContextCount && !iSubscriberSub1 && !iSubscriberSub2 )
            {
            iSubscriberSub1 = CCFContextSubscription::NewL();
            iSubscriberSub1Owned = ETrue;
            iSubscriberSub1->SetContextSourceL ( _L( "dymmySubSource" ) );
            iSubscriberSub1->SetContextTypeL ( _L( "dymmySubType" ) );
    
            iSubscriberSub2 = CCFContextSubscription::NewL();
            iSubscriberSub2Owned = ETrue;
            iSubscriberSub2->SetContextSourceL ( _L( "dymmySub2Source" ) );
            iSubscriberSub2->SetContextTypeL ( _L( "dymmySub2Type" ) );
    
            RThread thread;
            CleanupClosePushL( thread );
    
            TInt err = iCF.SubscribeContext( iSubscriberSub1, this, thread );
            User::LeaveIfError( err );
            iSubscriberSub1Owned = EFalse;
            err = iCF.SubscribeContext( iSubscriberSub2, this, thread );
            User::LeaveIfError( err );
            iSubscriberSub2Owned = EFalse;
    
            CleanupStack::PopAndDestroy( &thread ); 
            }
        ++iObservingContextCount;
        }
    else if ( aOkValueForNewContext == KTestContextValueNosubsOk )
        {
        --iObservingContextCount;
        if ( !iObservingContextCount )
            {
            if ( iSubscriberSub1 )
                {
                iCF.UnsubscribeContext( *iSubscriberSub1, *this );
                iSubscriberSub1 = NULL;
                }
            if ( iSubscriberSub2 )
                {
                iCF.UnsubscribeContext( *iSubscriberSub2, *this );
                iSubscriberSub2 = NULL;
                }
            }
        }

    // below                                        // CLEANUP<< context
    CCFContextObjectImpl* context = CCFContextObjectImpl::NewLC();
    context->SetSourceL( KTestContextSource );
    context->SetTypeL( KTestContextValueType );
    if ( aSubscriberContextSource == KTestContextSource
        && ( aSubscriberContextType == KTestContextType
            || aSubscriberContextType == KTestContextType2 ) )
        {
        context->SetValueL( aOkValueForNewContext );
        }
    else
        {
        context->SetValueL( KTestContextValueNok );
        }
    CleanupStack::Pop( context );                   // CLEANUP>> context
    return context;
    }

// ---------------------------------------------------------------------------
// CTestContextSourcePlugIn::GetSourceCommandResponseValue
// ---------------------------------------------------------------------------
//
void CTestContextSourcePlugIn::GetSourceCommandResponseValue(
    TPtrC& aResponseValue,
    const CCFContextSourceCommand& aCommand,
    const TDesC& aCmdType )
    {
    TLinearOrder< TCFSourceCommandSenderId > cmdSenderOrder(
            SourceCommandSenderIdOrder );
    TCFSourceCommandSenderId sender = aCommand.Sender();
    TInt pos = iActiveRequests.FindInOrder( sender, cmdSenderOrder );

    aResponseValue.Set( KSourceCommandSignalContextValueError );

    if ( aCmdType.CompareF( KSourceCommandAttributeValueActivate ) == 0 )
        {
        if ( pos == KErrNotFound )
            {
            TInt err = iActiveRequests.InsertInOrder( sender,
                    cmdSenderOrder );
            if ( err != KErrNone )
                {
                // Do nothing, value already set to error.
                }
            else if ( iActiveRequests.Count() == 1 )
                {
                aResponseValue.Set( KSourceCommandSignalContextValueActivated );
                }
            else
                {
                aResponseValue.Set(
                        KSourceCommandSignalContextValueActiveReqActivate );
                }
            }
        else
            {
            aResponseValue.Set(
                    KSourceCommandSignalContextValueActiveReqActivate );
            }
        }
    else if ( aCmdType.CompareF( KSourceCommandAttributeValueDeactivate ) == 0 )
        {
        if ( pos == KErrNotFound )
            {
            if ( iActiveRequests.Count() == 0 )
                {
                aResponseValue.Set(
                        KSourceCommandSignalContextValueDeactiveReqDeactivate );
                }
            else
                {
                aResponseValue.Set(
                        KSourceCommandSignalContextValueActiveReqDeactivate );
                }
            }
        else
            {
            iActiveRequests.Remove( pos );
            if ( iActiveRequests.Count() == 0 )
                {
                aResponseValue.Set(
                        KSourceCommandSignalContextValueDeactivated );
                }
            else
                {
                aResponseValue.Set(
                        KSourceCommandSignalContextValueActiveReqDeactivate );
                }
            }
        }
    }
