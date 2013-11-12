/*
* Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies). 
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


// INCLUDES
#include "MT_CFClient.h"

#include <EUnitMacros.h>
#include <cfclient.h>
#include <cfcontextsubscription.h>
#include <cfcontextindication.h>
#include <cfactionsubscription.h>
#include <cfactionindication.h>
#include <cfcontextsourcecommand.h>
#include <f32file.h>
#include <S32MEM.h>
#include <e32property.h>
#include <BAUTILS.H>

#include "cfcommon.h"
#include "testcontextdata.h"
#include "cfcontextobjectimpl.h"
#include "testcontextsourceplugin.h"
#include "testcontextsourcepluginconst.hrh"
#include "CFTestDelay.h"

// CONSTANTS

_LIT_SECURITY_POLICY_PASS( KPassSec );
_LIT_SECURITY_POLICY_FAIL( KFailSec );

_LIT( KContextSourceA, "TestSourceA" );
_LIT( KContextTypeA, "TestTypeA" );
_LIT( KContextValueA, "TestValueA" );
_LIT( KContextValueB, "TestValueB" );

_LIT( KContextSourceFormatWild, "Source" );
_LIT( KContextTypeFormatWild, "Type" );
_LIT( KContextSourceFormat, "Source.%d" );
_LIT( KContextTypeFormat, "Type.%d" );
_LIT( KContextValueFormat, "Value.%d" );

_LIT( KActionIdFormat, "AcionId.%d" );

_LIT( KActionA, "TestActionA" );
_LIT( KActionAParamA1, "ParamA1" );
_LIT( KActionAValueA1, "ValueA1" );

_LIT( KActionB, "TestActionB" );
_LIT( KActionBParamB1, "ParamB1" );
_LIT( KActionBValueB1, "ValueB1" );
_LIT( KActionBParamB2, "ParamB2" );
_LIT( KActionBValueB2, "ValueB2" );

_LIT( KActionC, "TestActionC" );

_LIT( KTestContextSource, "TestContextSource" );
_LIT( KTestContextType, "SubsNoSubsTest" );
_LIT( KTestContextType2, "SubsNoSubsTest2" );
_LIT( KTestContextValueType, "TestValue" );
_LIT( KTestContextValueSubsOk, "SubsOk" );
_LIT( KTestContextValueNosubsOk, "NoSubsOk" );
_LIT( KTestContextValueNok, "Nok" );

const TUid KTestContextSourcePluginImplUid =
    {
        KTestContextSourcePluginImplementationUid
    };
const TUid KBogusContextSourcePluginImplUid =
    {
        0xBAD50BAD
    };

const TInt KSecond = 1000000;

_LIT( KSourceCommandId, "sourceCommand" );
_LIT( KSourceCommandUidId, "uid" );
_LIT( KSourceCommandParamCmd, "cmd" );
_LIT( KSourceCommandParamCmdParam, "cmdParam" );
_LIT( KSourceCommandParamCmdInfo, "cmdInfo" );
_LIT( KSourceCommandParamModifiers, "modifiers" );
_LIT( KSourceCommandAttributeType, "type" );
_LIT( KSourceCommandAttributeMode, "mode" );
_LIT( KSourceCommandAttributeOverride, "override" );
_LIT( KSourceCommandAttributeText, "text" );
_LIT( KSourceCommandAttributeValueText, "activate from really clean state" );
_LIT( KSourceCommandAttributeValue, "value" );
_LIT( KSourceCommandAttributeValueYes, "yes" );
_LIT( KSourceCommandAttributeValueActivate, "Activate" );
_LIT( KSourceCommandAttributeValueDeactivate, "Deactivate" );
_LIT( KSourceCommandAttributeValueClean, "clean" );
_LIT( KSourceCommandAttributeValueReally, "really" );
_LIT( KSourceCommandAttributeValueFull, "full" );
_LIT( KSourceCommandAttributeValueNoException, "noException" );

_LIT( KSourceCommandPluginScriptContextSource, "TestSourcePlugIn" );
_LIT( KSourceCommandClientScriptContextSource, "TestSourceClient" );
_LIT( KSourceCommandScriptContextType, "SendSourceCommand" );
_LIT( KSourceCommandScript1ContextValueActivate, "A" );
_LIT( KSourceCommandScript1ContextValueDeactivate, "B" );
_LIT( KSourceCommandScript2ContextValueActivate, "A2" );
_LIT( KSourceCommandScript2ContextValueDeactivate, "B2" );

_LIT( KSourceCommandSignalContextSource, "TestSourcePlugin" );
_LIT( KSourceCommandSignalContextType, "SourceCommand" );
_LIT( KSourceCommandSignalContextValueActivated, "Activated" );
_LIT( KSourceCommandSignalContextValueDeactivated, "Deactivated" );
_LIT( KSourceCommandSignalContextValueActiveReqActivate, "ActiveReqActivate" );
_LIT( KSourceCommandSignalContextValueActiveReqDeactivate, "ActiveReqDeactivate" );
_LIT( KSourceCommandSignalContextValueDeactiveReqDeactivate, "DeactiveReqDeactivate" );
_LIT( KSourceCommandSignalContextValueError, "Error" );

_LIT( KContextValueTriggerForSourceCommandClient, "SourceCommandClient" );

_LIT( KReservedActionPublishContext, "publishContext" );
_LIT( KReservedActionPublishContext2, "PUBLISHCONTEXT" );
_LIT( KReservedActionSourceCommand, "sourceCommand" );
_LIT( KReservedActionSourceCommand2, "SOURCECOMMAND" );

_LIT( KRomScriptAutoDefineContextSource, "RomScript" );
_LIT( KRomScriptAutoDefineContextType, "NewContext" );
_LIT( KRomScriptAutoDefineContextValue, "AutoDefined" );
_LIT( KRomScriptTriggerAutoDefineContextSource, "TestRomScript" );
_LIT( KRomScriptTriggerAutoDefineContextType, "PublishContext" );
_LIT( KRomScriptTriggerAutoDefineContextValue, "AutoDefine" );

_LIT( KRomScriptDetectedAutoDefinedContextSource, "AutoDefined" );
_LIT( KRomScriptDetectedAutoDefinedContextType, "RomScriptContext" );
_LIT( KRomScriptDetectedAutoDefinedContextValue, "Detected" );

// LOCAL DEFINITIONS

/**
 * Clean up item for RContextObjectArray
 */
LOCAL_C void CleanupContextObjectArray( TAny* aArray )
    {
    RContextObjectArray* array = static_cast<RContextObjectArray*>( aArray );
    array->ResetAndDestroy();
    }

/**
 * Clean up function for RContextObjectArray
 */
LOCAL_C void CleanupResetAndDestroyPushL( RContextObjectArray& aArray )
    {
    TCleanupItem item( CleanupContextObjectArray, &aArray );
    CleanupStack::PushL( item );
    }

/**
 * Clean up item for RContextQueryArray
 */
LOCAL_C void CleanupContextQueryArray( TAny* aArray )
    {
    RContextQueryArray* array = static_cast<RContextQueryArray*>( aArray );
    array->ResetAndDestroy();
    }

/**
 * Clean up function for RContextQueryArray
 */
LOCAL_C void CleanupResetAndDestroyPushL( RContextQueryArray& aArray )
    {
    TCleanupItem item( CleanupContextQueryArray, &aArray );
    CleanupStack::PushL( item );
    }

/**
 * Wait info.
 */
NONSHARABLE_CLASS( TWaitInfo )
    {
public:

    CPeriodic* iPeriodic;
    CActiveSchedulerWait* iWait;
    };

/**
 * Call back method when we need to stop active scheduler wait.
 */
LOCAL_C TInt WaitCallBack( TAny* aSelf )
    {
    if( aSelf )
        {
        TWaitInfo* info = static_cast<TWaitInfo*>( aSelf );
        if( info->iPeriodic )
            {
            info->iPeriodic->Cancel();
            }
        if( info->iWait )
            {
            if( info->iWait->IsStarted() )
                {
                info->iWait->AsyncStop();
                }
            }
        }

    return KErrNone;
    }

/**
 * Helper method to wait current scheduler before teardown is completed.
 */
LOCAL_C void Wait( TInt aIntervalInMicorseconds )
    {
    TWaitInfo info;

    // Construct periodic
    CPeriodic* periodic = CPeriodic::NewL( CActive::EPriorityStandard );
    CleanupStack::PushL( periodic );
    info.iPeriodic = periodic;

    // Construct active scheduler wait
    CActiveSchedulerWait* wait = new( ELeave ) CActiveSchedulerWait;
    CleanupStack::PushL( wait );
    info.iWait = wait;

    // Start timer and wait
    TCallBack cb( WaitCallBack, &info );
    periodic->Start( aIntervalInMicorseconds, aIntervalInMicorseconds, cb );
    wait->Start();

    // Cleanup
    CleanupStack::PopAndDestroy( wait );
    CleanupStack::PopAndDestroy( periodic );
    }

const TInt KDelay = 2000000; // 2 seconds
const TInt KConfigDelay = 4000;

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


// CONSTRUCTION
MT_CFClient* MT_CFClient::NewL( )
    {
    MT_CFClient* self = MT_CFClient::NewLC ( );
    CleanupStack::Pop ( );

    return self;
    }

MT_CFClient* MT_CFClient::NewLC( )
    {
    MT_CFClient* self = new( ELeave ) MT_CFClient();
    CleanupStack::PushL ( self );

    self->ConstructL ( );

    return self;
    }

// Destructor (virtual by CBase)
MT_CFClient::~MT_CFClient( )
    {
    Teardown();
    }

// Default constructor
MT_CFClient::MT_CFClient( )
    {
    }

// Second phase construct
void MT_CFClient::ConstructL( )
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL ( );
    }

//  METHODS

void MT_CFClient::ContextIndicationL(const CCFContextIndication& aChangedContext )
    {
    if ( iCheckContextData )
        {
        CheckContextDataIndicationL ( aChangedContext );
        }
    else
        {
        CheckContextIndicationL ( aChangedContext );
        }

    // Common for both indications
    if ( iWait && !iCheckA && !iCheckB )
        {
        if ( iWait->IsStarted ( ) )
            {
            iWait->AsyncStop ( );
            }
        }
    if ( iDeleteClient )
        {
        delete iClient;
        iClient = NULL;
        iDeleteClient = EFalse;
        }
    }

void MT_CFClient::ActionIndicationL(const CCFActionIndication& aActionToExecute )
    {
    TPtrC actionId( aActionToExecute.Identifier() );
    if ( iLastActionId )
        {
        delete iLastActionId;
        iLastActionId = NULL;
        }
    iLastActionId = actionId.AllocL();

    if ( iCheckA )
        {
        const RKeyValueArray& params = aActionToExecute.Parameters ( );
        EUNIT_ASSERT_DESC( actionId == KActionA,
            "Incorrect action identifier!" );
        EUNIT_ASSERT_DESC( params.Count() == 1,
            "Incorrect action param count!" )
        EUNIT_ASSERT_DESC( params[0]->Key() == KActionAParamA1,
            "Incorrect action param count!" )
        EUNIT_ASSERT_DESC( params[0]->Value() == KActionAValueA1,
            "Incorrect action param count!" )
        if ( iCountActionAIndications )
            {
            --iExpectedActionAIndicationCount;
            if ( !iExpectedActionAIndicationCount )
                {
                iCheckA = EFalse;
                }
            }
        else
            {
            iCheckA = EFalse;
            }
        }
    else if ( iCheckB )
        {
        const RKeyValueArray& params = aActionToExecute.Parameters ( );
        EUNIT_ASSERT_DESC( actionId == KActionB,
            "Incorrect action identifier!" );
        EUNIT_ASSERT_DESC( params.Count() == 2,
            "Incorrect action param count!" )
        EUNIT_ASSERT_DESC( params[0]->Key() == KActionBParamB1,
            "Incorrect action param count!" )
        EUNIT_ASSERT_DESC( params[0]->Value() == KActionBValueB1,
            "Incorrect action param count!" )
        EUNIT_ASSERT_DESC( params[1]->Key() == KActionBParamB2,
            "Incorrect action param count!" )
        EUNIT_ASSERT_DESC( params[1]->Value() == KActionBValueB2,
            "Incorrect action param count!" )
        iCheckB = EFalse;
        }
    else if ( iCheckRegisterScriptAction )
        {
        const RKeyValueArray& params = aActionToExecute.Parameters ( );
        EUNIT_ASSERT_DESC( actionId == _L("RegisterScriptAction"),
            "Incorrect action identifier!" );
        EUNIT_ASSERT_DESC( params.Count() == 1,
            "Incorrect action param count!" )
        EUNIT_ASSERT_DESC( params[0]->Key() == _L("ParamA1"),
            "Incorrect action param count!" )
        EUNIT_ASSERT_DESC( params[0]->Value() == _L("ValueA1"),
            "Incorrect action param count!" )
        iCheckRegisterScriptAction = EFalse;
        }
    else if ( iCheckCustomAction )
        {
        EUNIT_ASSERT_DESC( iCustomActionId == actionId,
            "Incorrect action id" );
        const RKeyValueArray
            & params = aActionToExecute.Parameters ( );
        EUNIT_ASSERT_DESC( params.Count() == iCustomActionParams.Count(),
            "Invalid amount of action parameters" );
        for (TInt i = 0; i < iCustomActionParams.Count ( ); i++ )
            {
            CCFKeyValuePair* pair1 = iCustomActionParams[i];
            CCFKeyValuePair* pair2 = params[i];
            EUNIT_ASSERT_DESC( pair2->Key() == pair1->Key(),
                "Invalid key" );
            EUNIT_ASSERT_DESC( pair2->Value() == pair1->Value(),
                "Invalid value" );
            }
        iCheckCustomAction = EFalse;
        iCustomActionId.Zero ( );
        iCustomActionParams.ResetAndDestroy ( );
        }

    if ( iWait && !iCheckA && !iCheckB && !iCheckRegisterScriptAction &&!iCheckCustomAction )
        {
        if ( iWait->IsStarted ( ) )
            {
            iWait->AsyncStop ( );
            }
        }

    if ( iDeleteClient )
        {
        delete iClient;
        iClient = NULL;
        iDeleteClient = EFalse;
        }
    }

void MT_CFClient::HandleContextFrameworkError( TCFError aError,
    const TDesC& aSource, const TDesC& aType )
    {
    if ( iCheckB )
        {
        EUNIT_ASSERT_DESC( aError == MCFListener::ESecurityCheckErrorForContextSubscription,
            "Incorrect HandleContextFrameworkError error code!" );
        EUNIT_ASSERT_DESC( aSource == iContextB->Source(),
            "Incorrect HandleContextFrameworkError context source!" );
        EUNIT_ASSERT_DESC( aType == iContextB->Type(),
            "Incorrect HandleContextFrameworkError context type!" );
        iCheckB = EFalse;
        }
    else if ( iCheckC )
        {
        EUNIT_ASSERT_DESC( aError == MCFListener::ESecurityCheckErrorForContextSubscription,
            "Incorrect HandleContextFrameworkError error code!" );
        EUNIT_ASSERT_DESC( aSource == iContextC->Source(),
            "Incorrect HandleContextFrameworkError context source!" );
        EUNIT_ASSERT_DESC( aType == iContextC->Type(),
            "Incorrect HandleContextFrameworkError context type!" );
        iCheckC = EFalse;
        }
    else if ( iCheckRomScriptContextAutoDefine )
        {
        EUNIT_ASSERT_DESC( aError == MCFListener::ESecurityCheckErrorForContextSubscription,
            "Incorrect HandleContextFrameworkError error code!" );
        EUNIT_ASSERT_DESC( aSource == KRomScriptAutoDefineContextSource,
            "Incorrect HandleContextFrameworkError context source!" );
        EUNIT_ASSERT_DESC( aType == KRomScriptAutoDefineContextType,
            "Incorrect HandleContextFrameworkError context type!" );
        iCheckRomScriptContextAutoDefine = EFalse;
        if ( iTestDelay )
            {
            iTestDelay->Stop();
            }
        }

    if ( iWait && !iCheckA && !iCheckB && !iCheckC && !iCheckRomScriptContextAutoDefine )
        {
        if ( iWait->IsStarted ( ) )
            {
            iWait->AsyncStop ( );
            }
        }
    }

TAny* MT_CFClient::Extension( const TUid& aExtensionUid ) const
    {
    switch ( aExtensionUid.iUid )
        {
        case KCFContextSourceInterfaceUidValue:
            {
            if ( iReturnContextSourceInterface )
                {
                MCFContextSource* csinterface = (MCFContextSource*) this;
                return (TAny*) csinterface;
                }
            break;
            }
        default:
            {
            return NULL;
            }
        }
    return NULL;
    }

// from MCFContextSource
void MT_CFClient::Subscribers( const TDesC& aContextSource,
    const TDesC& aContextType )
    {
    if ( iCheckSubscribersNoSubscribersCalled )
        {
        iCheckSubscribersNoSubscribersCalled = EFalse;
        iSubscribersCalled = ETrue;
        EUNIT_ASSERT_DESC(
                iExpectedSubscribersSource.Compare( aContextSource ) == 0,
                "No expected source");
        EUNIT_ASSERT_DESC(
                iExpectedSubscribersType.Compare( aContextType ) == 0,
                "No expected type");

        if ( iWait )
            {
            if ( iWait->IsStarted ( ) )
                {
                iWait->AsyncStop ( );
                }
            }
        }
    }

// from MCFContextSource
void MT_CFClient::NoSubscribers( const TDesC& aContextSource,
    const TDesC& aContextType )
    {
    if ( iCheckSubscribersNoSubscribersCalled )
        {
        iCheckSubscribersNoSubscribersCalled = EFalse;
        iNoSubscribersCalled = ETrue;
        EUNIT_ASSERT_DESC(
                iExpectedSubscribersSource.Compare( aContextSource ) == 0,
                "No expected source");
        EUNIT_ASSERT_DESC(
                iExpectedSubscribersType.Compare( aContextType ) == 0,
                "No expected type");

        if ( iWait )
            {
            if ( iWait->IsStarted ( ) )
                {
                iWait->AsyncStop ( );
                }
            }
        }
    }

// from MCFContextSource
void MT_CFClient::HandleCommand( const CCFContextSourceCommand& aCommand )
    {
    TPtrC id( aCommand.Name() );
    EUNIT_ASSERT_DESC( id.CompareF( KSourceCommandId ) == 0,
            "No expected source command id" );

    // Parse context source command
    const RKeyValueArray& attributes = aCommand.Attributes();
    EUNIT_ASSERT_DESC( attributes.Count() == 1, "No expected attribute count" );

    TUid sourceUid = aCommand.SourceUid();
    RThread thread;
    EUNIT_ASSERT_DESC( sourceUid == thread.SecureId(), "No expected uid" );
    thread.Close();

    const RSourceCommandParameterArray& params = aCommand.Parameters();
    EUNIT_ASSERT_DESC( params.Count() >= 1, "No expected parameter count" );

    TPtrC paramName( params[ 0 ]->Name() );
    EUNIT_ASSERT_DESC( paramName.CompareF( KSourceCommandParamCmd ) == 0,
            "No expected param name" );

    const RKeyValueArray& paramAttributes = params[ 0 ]->Attributes();
    EUNIT_ASSERT_DESC( paramAttributes.Count() == 1, "No expected parameter attribute count" );

    CCFKeyValuePair* keyValuePair = paramAttributes[ 0 ];
    EUNIT_ASSERT_DESC( keyValuePair->Key().CompareF(
            KSourceCommandAttributeType ) == 0,
            "Type attribute expected" );

    TLinearOrder< TCFSourceCommandSenderId > cmdSenderOrder(
            SourceCommandSenderIdOrder );
    TCFSourceCommandSenderId sender = aCommand.Sender();
    TInt pos = iActiveRequests.FindInOrder( sender, cmdSenderOrder );

    TPtrC cmdType( keyValuePair->Value() );
    if ( cmdType.CompareF( KSourceCommandAttributeValueActivate ) == 0 )
        {
        if ( pos == KErrNotFound )
            {
            iActiveRequests.InsertInOrder( sender, cmdSenderOrder );
            }
        CheckActivateSourceCommandParams( params );
        }
    else if ( cmdType.CompareF( KSourceCommandAttributeValueDeactivate ) == 0 )
        {
        if ( pos != KErrNotFound )
            {
            iActiveRequests.Remove( pos );
            }
        CheckDeactivateSourceCommandParams( params );
        }
    else
        {
        EUNIT_ASSERT_DESC( EFalse, "WRONG type attribute value" );
        }

    iHandleCommandCalled = ETrue;

    if ( iWait && iWait->IsStarted() )
        {
        iWait->AsyncStop ( );
        }
    }

void MT_CFClient::CheckActivateSourceCommandParams(
    const RSourceCommandParameterArray& aParams )
    {
    EUNIT_ASSERT_DESC( aParams.Count() == 2, "No expected param count" );

    // Check 1. param
    CCFContextSourceCommandParameter* param = aParams[ 0 ];
    TPtrC paramName( param->Name() );
    EUNIT_ASSERT_DESC( paramName.CompareF( KSourceCommandParamCmd ) == 0,
            "No expected param name" );

    // Check 1. param attributes
    const RKeyValueArray& attributes = param->Attributes();
    EUNIT_ASSERT_DESC( attributes.Count() == 1, "No expected attribute count" );
    CCFKeyValuePair* keyValuePair = attributes[ 0 ];
    EUNIT_ASSERT_DESC( keyValuePair->Key().CompareF(
            KSourceCommandAttributeType ) == 0,
            "Type attribute expected" );
    EUNIT_ASSERT_DESC( keyValuePair->Value().CompareF(
            KSourceCommandAttributeValueActivate ) == 0,
            "No expected type attribute value" );

    // Check 1. param nested params
    const RSourceCommandParameterArray& nestedParams = param->Parameters();
    EUNIT_ASSERT_DESC( nestedParams.Count() == 2, "No expected nested parameter count" );

    // Check 1. param 1. nested params attributes
    CCFContextSourceCommandParameter* nestedParam = nestedParams[ 0 ];
    EUNIT_ASSERT_DESC( nestedParam->Name().CompareF(
            KSourceCommandParamCmdParam ) == 0,
            "Parameter cmdParam expected" );
    const RKeyValueArray& nestedParamAttributes = nestedParam->Attributes();
    EUNIT_ASSERT_DESC( nestedParamAttributes.Count() == 1,
            "No expected nested parameter attribute count" );
    keyValuePair = nestedParamAttributes[ 0 ];
    EUNIT_ASSERT_DESC( keyValuePair->Key().CompareF(
            KSourceCommandAttributeMode ) == 0,
            "Mode attribute expected" );
    EUNIT_ASSERT_DESC( keyValuePair->Value().CompareF(
            KSourceCommandAttributeValueClean ) == 0,
            "No expected mode attribute value" );

    // Check 1. param 1. nested params parameters
    const RSourceCommandParameterArray& nested2Params = nestedParam->Parameters();
    EUNIT_ASSERT_DESC( nested2Params.Count() == 1, "No expected nested parameter count" );

    // Check 1. param 1. nested params parameters attributes
    nestedParam = nested2Params[ 0 ];
    EUNIT_ASSERT_DESC( nestedParam->Name().CompareF(
            KSourceCommandParamModifiers ) == 0,
            "Modifiers param expected" );
    const RKeyValueArray& nested2ParamAttributes = nestedParam->Attributes();
    EUNIT_ASSERT_DESC( nested2ParamAttributes.Count() == 1,
            "No expected nested parameter attribute count" );
    keyValuePair = nested2ParamAttributes[ 0 ];
    EUNIT_ASSERT_DESC( keyValuePair->Key().CompareF(
            KSourceCommandAttributeValue ) == 0,
            "Value attribute expected" );
    EUNIT_ASSERT_DESC( keyValuePair->Value().CompareF(
            KSourceCommandAttributeValueReally ) == 0,
            "No expected value attribute value" );

    const RSourceCommandParameterArray& nested3Params = nestedParam->Parameters();
    EUNIT_ASSERT_DESC( nested3Params.Count() == 0, "No expected nested parameter count" );

    // Check 1. param 2. nested params attributes
    nestedParam = nestedParams[ 1 ];
    EUNIT_ASSERT_DESC( nestedParam->Name().CompareF(
            KSourceCommandParamCmdParam ) == 0,
            "Parameter cmdParam expected" );
    const RKeyValueArray& nested3ParamAttributes = nestedParam->Attributes();
    EUNIT_ASSERT_DESC( nested3ParamAttributes.Count() == 1,
            "No expected nested parameter attribute count" );
    keyValuePair = nested3ParamAttributes[ 0 ];
    EUNIT_ASSERT_DESC( keyValuePair->Key().CompareF(
            KSourceCommandAttributeOverride ) == 0,
            "Override attribute expected" );
    EUNIT_ASSERT_DESC( keyValuePair->Value().CompareF(
            KSourceCommandAttributeValueYes ) == 0,
            "No expected override attribute value" );

    const RSourceCommandParameterArray& nested4Params = nestedParam->Parameters();
    EUNIT_ASSERT_DESC( nested4Params.Count() == 0, "No expected nested parameter count" );

    // Check 2. param
    param = aParams[ 1 ];
    paramName.Set( param->Name() );
    EUNIT_ASSERT_DESC( paramName.CompareF( KSourceCommandParamCmdInfo ) == 0,
            "No expected param name" );

    // Check 2. param attributes
    const RKeyValueArray& attributes2 = param->Attributes();
    EUNIT_ASSERT_DESC( attributes2.Count() == 1, "No expected attribute count" );
    keyValuePair = attributes2[ 0 ];
    EUNIT_ASSERT_DESC( keyValuePair->Key().CompareF(
            KSourceCommandAttributeType ) == 0,
            "Type attribute expected" );
    EUNIT_ASSERT_DESC( keyValuePair->Value().CompareF(
            KSourceCommandAttributeValueActivate ) == 0,
            "No expected type attribute value" );

    // Check 2. param nested params
    const RSourceCommandParameterArray& nestedParams2 = param->Parameters();
    EUNIT_ASSERT_DESC( nestedParams2.Count() == 1, "No expected nested parameter count" );

    // Check 2. param nested params attributes
    nestedParam = nestedParams2[ 0 ];
    EUNIT_ASSERT_DESC( nestedParam->Name().CompareF(
            KSourceCommandParamCmdParam ) == 0,
            "Parameter cmdParam expected" );
    const RKeyValueArray& nestedParamAttributes2 = nestedParam->Attributes();
    EUNIT_ASSERT_DESC( nestedParamAttributes2.Count() == 1,
            "No expected nested parameter attribute count" );
    keyValuePair = nestedParamAttributes2[ 0 ];
    EUNIT_ASSERT_DESC( keyValuePair->Key().CompareF(
            KSourceCommandAttributeText ) == 0,
            "Text attribute expected" );
    EUNIT_ASSERT_DESC( keyValuePair->Value().CompareF(
            KSourceCommandAttributeValueText ) == 0,
            "No expected text attribute value" );

    // Check 2. param 1. nested params parameters
    const RSourceCommandParameterArray& nested2Params2 = nestedParam->Parameters();
    EUNIT_ASSERT_DESC( nested2Params2.Count() == 0, "No expected nested parameter count" );
    }

void MT_CFClient::CheckDeactivateSourceCommandParams(
    const RSourceCommandParameterArray& aParams )
    {
    EUNIT_ASSERT_DESC( aParams.Count() == 1, "No expected param count" );

    // Check param
    CCFContextSourceCommandParameter* param = aParams[ 0 ];
    TPtrC paramName( param->Name() );
    EUNIT_ASSERT_DESC( paramName.CompareF( KSourceCommandParamCmd ) == 0,
            "No expected param name" );

    // Check param attributes
    const RKeyValueArray& attributes = param->Attributes();
    EUNIT_ASSERT_DESC( attributes.Count() == 1, "No expected attribute count" );
    CCFKeyValuePair* keyValuePair = attributes[ 0 ];
    EUNIT_ASSERT_DESC( keyValuePair->Key().CompareF(
            KSourceCommandAttributeType ) == 0,
            "Type attribute expected" );
    EUNIT_ASSERT_DESC( keyValuePair->Value().CompareF(
            KSourceCommandAttributeValueDeactivate ) == 0,
            "No expected type attribute value" );

    // Check param nested params
    const RSourceCommandParameterArray& nestedParams = param->Parameters();
    EUNIT_ASSERT_DESC( nestedParams.Count() == 1, "No expected nested parameter count" );

    // Check param nested params attributes
    CCFContextSourceCommandParameter* nestedParam = nestedParams[ 0 ];
    EUNIT_ASSERT_DESC( nestedParam->Name().CompareF(
            KSourceCommandParamCmdParam ) == 0,
            "Parameter cmdParam expected" );
    const RKeyValueArray& nestedParamAttributes = nestedParam->Attributes();
    EUNIT_ASSERT_DESC( nestedParamAttributes.Count() == 1,
            "No expected nested parameter attribute count" );
    keyValuePair = nestedParamAttributes[ 0 ];
    EUNIT_ASSERT_DESC( keyValuePair->Key().CompareF(
            KSourceCommandAttributeMode ) == 0,
            "Mode attribute expected" );
    EUNIT_ASSERT_DESC( keyValuePair->Value().CompareF(
            KSourceCommandAttributeValueFull ) == 0,
            "No expected mode attribute value" );

    // Check param nested params parameters
    const RSourceCommandParameterArray& nested2Params = nestedParam->Parameters();
    EUNIT_ASSERT_DESC( nested2Params.Count() == 1, "No expected nested parameter count" );

    // Check param nested params parameters attributes
    nestedParam = nested2Params[ 0 ];
    EUNIT_ASSERT_DESC( nestedParam->Name().CompareF(
            KSourceCommandParamModifiers ) == 0,
            "Modifiers param expected" );
    const RKeyValueArray& nested2ParamAttributes = nestedParam->Attributes();
    EUNIT_ASSERT_DESC( nested2ParamAttributes.Count() == 1,
            "No expected nested parameter attribute count" );
    keyValuePair = nested2ParamAttributes[ 0 ];
    EUNIT_ASSERT_DESC( keyValuePair->Key().CompareF(
            KSourceCommandAttributeValue ) == 0,
            "Value attribute expected" );
    EUNIT_ASSERT_DESC( keyValuePair->Value().CompareF(
            KSourceCommandAttributeValueNoException ) == 0,
            "No expected value attribute value" );

    const RSourceCommandParameterArray& nested3Params = nestedParam->Parameters();
    EUNIT_ASSERT_DESC( nested3Params.Count() == 0, "No expected nested parameter count" );
    }


void MT_CFClient::SetupL( )
    {
    }

void MT_CFClient::SetupClientL( )
    {
    User::LeaveIfError ( iFs.Connect ( ) );
    iClient = CCFClient::NewL ( *this );
    iAnotherClient = CCFClient::NewL ( *this );
    iContextA = CCFContextObject::NewL ( );
    iContextB = CCFContextObject::NewL ( );
    iContextC = CCFContextObject::NewL ( );
    iWait = new( ELeave ) CActiveSchedulerWait;
    iContextDataA = CTestContextData::NewL ( _L("Key_A"), _L("Value_A") );
    iContextDataB = CTestContextData::NewL ( _L("Key_B"), _L("Value_B") );
    }

void MT_CFClient::SetupNewClientL( )
    {
    iReturnContextSourceInterface = ETrue;
    iTestDelay = CCFTestDelay::NewL ( );
    SetupClientL ( );
    }

void MT_CFClient::SetupSourceSettingTestClientL( )
    {
    iTestDelay = CCFTestDelay::NewL ( );
    SetupClientL ( );
    }

void MT_CFClient::Teardown( )
    {
    if ( iWait )
        {
        if ( iWait->IsStarted ( ) )
            {
            iWait->AsyncStop ( );
            }
        delete iWait;
        iWait = NULL;
        }

    delete iClient;
    iClient = NULL;

    delete iAnotherClient;
    iAnotherClient = NULL;

    delete iContextA;
    iContextA = NULL;

    delete iContextB;
    iContextB = NULL;

    delete iContextC;
    iContextC = NULL;
    iFs.Close ( );

    delete iContextDataA;
    iContextDataA = NULL;

    delete iContextDataB;
    iContextDataB = NULL;

    iReturnContextSourceInterface = EFalse;

    iCustomActionParams.ResetAndDestroy();

    delete iLastActionId;
    iLastActionId = NULL;

    iActiveRequests.Close();

    if ( iTestDelay )
        {
        // Give some time for everything to be cleaned up.
        iTestDelay->Start ( 100000 ); // 100 ms
        }

    delete iTestDelay;
    iTestDelay = NULL;
    }

void MT_CFClient::TeardownAndScriptCleanup( )
    {
    iClient->DeleteScripts ( );
    iClient->RestoreRomScript ( _L("test-scriptUpgradeRom") );
    iClient->RestoreRomScript ( _L("test-scriptUpgradeRomClientMissingCapability") );
    Teardown ( );
    }

void MT_CFClient::CheckContextIndicationL(
    const CCFContextIndication& aChangedContext )
    {
    if ( iCheckA )
        {
        const CCFContextObject& co = aChangedContext.Context ( );
        EUNIT_ASSERT_DESC( co.Source() == iContextA->Source(),
            "Incorrect context source in context indication!" );
        EUNIT_ASSERT_DESC( co.Type() == iContextA->Type(),
            "Incorrect context type in context indication!" );
        EUNIT_ASSERT_DESC( co.Value() == iContextA->Value(),
            "Incorrect context value in context indication!" );
        iCheckA = EFalse;
        }
    else if ( iCheckB )
        {
        const CCFContextObject& co = aChangedContext.Context ( );
        EUNIT_ASSERT_DESC( co.Source() == iContextB->Source(),
            "Incorrect context source in context indication!" );
        EUNIT_ASSERT_DESC( co.Type() == iContextB->Type(),
            "Incorrect context type in context indication!" );
        EUNIT_ASSERT_DESC( co.Value() == iContextB->Value(),
            "Incorrect context value in context indication!" );
        iCheckB = EFalse;
        }
    else if ( iCheckPluginSubscribersCalled )
        {
        const CCFContextObject& co = aChangedContext.Context ( );
        EUNIT_ASSERT_DESC( iExpectedSubscribersSource == co.Source(),
            "No expected plugin subscriber source");
        EUNIT_ASSERT_DESC( iExpectedSubscribersType == co.Type(),
            "No expected plugin subscriber type");
        EUNIT_ASSERT_DESC( iExpectedSubscribersValue == co.Value(),
            "No expected plugin subscriber value");

        iPluginSubscribersCalled = ETrue;
        iCheckPluginSubscribersCalled = EFalse;

        if ( iWait && iWait->IsStarted ( ) )
            {
            iWait->AsyncStop ( );
            }
        }
    else if ( iCheckSourceCommandHandled )
        {
        const CCFContextObject& co = aChangedContext.Context ( );
        EUNIT_ASSERT_DESC( iExpectedSubscribersSource == co.Source(),
            "No expected source command signal context source");
        EUNIT_ASSERT_DESC( iExpectedSubscribersType == co.Type(),
            "No expected source command signal context type");
        EUNIT_ASSERT_DESC( iExpectedSubscribersValue == co.Value(),
            "No expected source command signal context value");

        iSourceCommandHandled = ETrue;
        iCheckSourceCommandHandled = EFalse;

        if ( iWait && iWait->IsStarted ( ) )
            {
            iWait->AsyncStop ( );
            }
        }
    else if ( iCheckRomScriptContextAutoDefineDetected )
        {
        const CCFContextObject& co = aChangedContext.Context ( );
        EUNIT_ASSERT_DESC( iExpectedSubscribersSource == co.Source(),
            "No expected source command signal context source");
        EUNIT_ASSERT_DESC( iExpectedSubscribersType == co.Type(),
            "No expected source command signal context type");
        EUNIT_ASSERT_DESC( iExpectedSubscribersValue == co.Value(),
            "No expected source command signal context value");

        iCheckRomScriptContextAutoDefineDetected = EFalse;

        if ( iWait && iWait->IsStarted ( ) )
            {
            iWait->AsyncStop ( );
            }
        }
    }

void MT_CFClient::CheckContextDataIndicationL(
    const CCFContextIndication& aChangedContext )
    {
    if ( iCheckA )
        {
        // Check context
        const CCFContextObject& co = aChangedContext.Context ( );
        EUNIT_ASSERT_DESC( co.Source() == iContextA->Source(),
            "Incorrect context source in context indication!" );
        EUNIT_ASSERT_DESC( co.Type() == iContextA->Type(),
            "Incorrect context type in context indication!" );
        EUNIT_ASSERT_DESC( co.Value() == iContextA->Value(),
            "Incorrect context value in context indication!" );

        // Check data object
        EUNIT_ASSERT_DESC( aChangedContext.Uid() == KTestContextDataUid,
            "Incorrect context data UID!" );
        if ( aChangedContext.Uid ( )== KTestContextDataUid )
            {
            CCFContextIndication
                & indication =const_cast<CCFContextIndication&>( aChangedContext );
            CTestContextData* contextData =CTestContextData::NewLC ( KNullDesC,
                KNullDesC );
            indication.CreateDataObjectL ( *contextData );
            EUNIT_ASSERT_DESC( contextData->Key() == iContextDataA->Key(),
                "Incorrect context data object comparison!" );
            EUNIT_ASSERT_DESC( contextData->Value() == iContextDataA->Value(),
                "Incorrect context data object comparison!" );
            CleanupStack::PopAndDestroy ( contextData );
            }
        iCheckA = EFalse;
        }
    else if ( iCheckB )
        {
        // Check context
        const CCFContextObject& co = aChangedContext.Context ( );
        EUNIT_ASSERT_DESC( co.Source() == iContextB->Source(),
            "Incorrect context source in context indication!" );
        EUNIT_ASSERT_DESC( co.Type() == iContextB->Type(),
            "Incorrect context type in context indication!" );
        EUNIT_ASSERT_DESC( co.Value() == iContextB->Value(),
            "Incorrect context value in context indication!" );

        // Check data object
        EUNIT_ASSERT_DESC( aChangedContext.Uid() == KTestContextDataUid,
            "Incorrect context data UID!" );
        if ( aChangedContext.Uid ( )== KTestContextDataUid )
            {
            CCFContextIndication
                & indication =const_cast<CCFContextIndication&>( aChangedContext );
            CTestContextData* contextData =CTestContextData::NewLC (
                KNullDesC, KNullDesC );
            indication.CreateDataObjectL ( *contextData );
            EUNIT_ASSERT_DESC( contextData->Key() == iContextDataB->Key(),
                "Incorrect context data object comparison!" );
            EUNIT_ASSERT_DESC( contextData->Value() == iContextDataB->Value(),
                "Incorrect context data object comparison!" );
            CleanupStack::PopAndDestroy ( contextData );
            }
        iCheckB = EFalse;
        }
    }

void MT_CFClient::ConfigureContextL( CCFContextObject& aContext )
    {
    Wait ( KConfigDelay );

    TBuf<256> buf( KNullDesC );
    TTime time;
    time.HomeTime ( );

    buf.Format ( KContextSourceFormat, time.Int64 ( ) );
    aContext.SetSourceL ( buf );

    buf.Format ( KContextTypeFormat, time.Int64 ( ) );
    aContext.SetTypeL ( buf );

    buf.Format ( KContextValueFormat, time.Int64 ( ) );
    aContext.SetValueL ( buf );
    }

void MT_CFClient::UpdateContextValueL( CCFContextObject& aContext )
    {
    Wait ( KConfigDelay );

    TBuf<256> buf( KNullDesC );
    TTime time;
    time.HomeTime ( );

    buf.Format ( KContextValueFormat, time.Int64 ( ) );
    aContext.SetValueL ( buf );
    }

void MT_CFClient::UpdateContextTypeL( CCFContextObject& aContext )
    {
    Wait ( KConfigDelay );

    TBuf<256> buf( KNullDesC );
    TTime time;
    time.HomeTime ( );

    buf.Format ( KContextTypeFormat, time.Int64 ( ) );
    aContext.SetTypeL ( buf );
    }

HBufC* MT_CFClient::ConfigureActionIdLC( )
    {
    Wait ( KConfigDelay );

    TTime time;
    time.HomeTime ( );
    HBufC* actionId = HBufC::NewLC ( 256 );
    TPtr actionIdPtr = actionId->Des ( );
    actionIdPtr.Format ( KActionIdFormat, time.Int64 ( ) );

    return actionId;
    }

HBufC8* MT_CFClient::LoadScriptFromFile( const TDesC& aFilePath )
    {
    HBufC8* script=  NULL;
    RFile file;
    TFileName fileName;
    fileName.Copy (_L("z:\\data\\cftestdata\\testscripts\\") );
    fileName.Append (aFilePath );
    TInt err = file.Open ( iFs, fileName, EFileRead );
    if ( err == KErrNone )
        {
        TInt size = 0;
        err = file.Size ( size );
        if ( err == KErrNone )
            {
            script = HBufC8::New ( size );
            if ( script )
                {
                TPtr8 scriptPtr = script->Des ( );
                err = file.Read ( scriptPtr );
                if ( err == KErrNone )
                    {
                    // Strip all unnecassary data from script
                    TInt pos = scriptPtr.FindF ( KScriptStartTag );
                    if ( pos != KErrNotFound )
                        {
                        scriptPtr.Copy ( scriptPtr.MidTPtr ( pos ) );
                        }
                    else
                        {
                        // Incorrect script
                        delete script;
                        script = NULL;
                        }
                    }
                else
                    {
                    delete script;
                    script = NULL;
                    }
                }
            }
        }
    else
        {
        EUNIT_PRINT( _L("MT_CFClient::LoadScriptFromFile - Loading file [%S] failed with code = %d"),
                &fileName, err );
        }

    // Cleanup
    file.Close ( );
    return script;
    }

void MT_CFClient::ResetCustomAction()
    {
    iCheckCustomAction = EFalse;
    iCustomActionId.Zero();
    iCustomActionParams.ResetAndDestroy();
    }

TBool MT_CFClient::LastActionIdEquals( const TDesC& aId )
    {
    if ( iLastActionId )
        {
        return ( *iLastActionId == aId );
        }
    else
        {
        return EFalse;
        }
    }

void MT_CFClient::DeleteLastActionId()
    {
    delete iLastActionId;
    iLastActionId = NULL;
    }

//-----------------------------------------------------------------------------
// CFClient module tests
//-----------------------------------------------------------------------------
//
void MT_CFClient::MT_CCFClient_NewLL( )
    {
    CCFClient* client = CCFClient::NewL ( *this );
    CleanupStack::PushL ( client );

    EUNIT_ASSERT_DESC( client, "CCFClient class instance not created!" );

    CleanupStack::PopAndDestroy ( client );
    }

void MT_CFClient::MT_CCFClient_NewLCL( )
    {
    CCFClient* client = CCFClient::NewLC ( *this );

    EUNIT_ASSERT_DESC( client, "CCFClient class instance not created!" );

    CleanupStack::PopAndDestroy ( client );
    }

void MT_CFClient::MT_CCFClient_DefineContextL( )
    {
    CCFContextObject* context = CCFContextObject::NewLC ( );
    TInt err = KErrNone;

    // x. Define context from two different clients when context onwer is set
    //--------------------------------------------------------------------------
    iReturnContextSourceInterface = ETrue;
    CCFClient* client = CCFClient::NewLC ( *this );
    CCFClient* client2 = CCFClient::NewLC ( *this );
    ConfigureContextL ( *context );
    err = client->DefineContext ( context->Source ( ), context->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value! " );
    err = client->DefineContext ( context->Source ( ), context->Type ( ),
        KFailSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value! " );
    err = client2->DefineContext ( context->Source ( ), context->Type ( ),
        KFailSec );
    EUNIT_ASSERT_DESC( err == KErrAlreadyExists, "Incorrect DefineContext return value! " );
    CleanupStack::PopAndDestroy ( client2 );
    CleanupStack::PopAndDestroy ( client );

    // x. Define context, delete client and redefine context
    //--------------------------------------------------------------------------
    iReturnContextSourceInterface = ETrue;
    ConfigureContextL ( *context );
    client = CCFClient::NewLC ( *this );
    err = client->DefineContext ( context->Source ( ), context->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value! " );
    CleanupStack::PopAndDestroy ( client );

    client = CCFClient::NewLC ( *this );
    err = client->DefineContext ( context->Source ( ), context->Type ( ),
        KFailSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value! " );
    err = client->DefineContext ( context->Source ( ), KNullDesC, KFailSec );
    CleanupStack::PopAndDestroy ( client );

    // x. Define context, delete client and redefine context
    //--------------------------------------------------------------------------
    client = CCFClient::NewLC ( *this );
    err = client->DefineContext ( context->Source(), KNullDesC, KFailSec );
    EUNIT_ASSERT_DESC( err == KErrArgument, "Incorrect DefineContext return value! " );
    err = client->DefineContext ( KNullDesC, context->Type ( ), KFailSec );
    EUNIT_ASSERT_DESC( err == KErrArgument, "Incorrect DefineContext return value! " );
    CleanupStack::PopAndDestroy ( client );

    // Clean up
    CleanupStack::PopAndDestroy ( context );
    }

void MT_CFClient::MT_CCFClient_SubscribersNosubscribersL( )
    {
    TInt err = KErrNone;

    // Define context
    ConfigureContextL ( *iContextA );
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KPassSec, KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value! " );

    // Delete & re-create client causing de-registering of publisher.
    delete iClient;
    iClient = NULL;
    iClient = CCFClient::NewL ( *this );

    // in this case redefine should work (same client)
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KPassSec, KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value! " );

    // Test if we get Subscribers() callback if we add subscription
    iCheckSubscribersNoSubscribersCalled = ETrue;
    iSubscribersCalled = EFalse;
    iExpectedSubscribersSource.Copy ( iContextA->Source ( ) );
    iExpectedSubscribersType.Copy ( iContextA->Type ( ) );

    CCFContextSubscription* subscription=  NULL;
    // Define a context and subscribe it
    subscription = CCFContextSubscription::NewLC ( );
    subscription->SetContextSourceL ( iContextA->Source ( ) );
    subscription->SetContextTypeL ( iContextA->Type ( ) );
    err = iClient->SubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscription );

    iWait->Start ( );
    EUNIT_ASSERT_DESC( iSubscribersCalled, "MCFContextSource::Subscribers not called" );
    EUNIT_ASSERT_DESC( !iCheckSubscribersNoSubscribersCalled, "Subscribers not called" );

    // Test if we get NoSubsscribers() callback if we remove subscription
    iCheckSubscribersNoSubscribersCalled = ETrue;
    iSubscribersCalled = EFalse;
    iNoSubscribersCalled = EFalse;
    iExpectedSubscribersSource.Copy ( iContextA->Source ( ) );
    iExpectedSubscribersType.Copy ( iContextA->Type ( ) );

    // Define a context and subscribe it
    subscription = CCFContextSubscription::NewLC ( );
    subscription->SetContextSourceL ( iContextA->Source ( ) );
    subscription->SetContextTypeL ( iContextA->Type ( ) );
    err = iClient->UnsubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscription );

    iWait->Start();
    EUNIT_ASSERT_DESC( iNoSubscribersCalled, "MCFContextSource::NoSubscribers not called" );
    EUNIT_ASSERT_DESC( !iCheckSubscribersNoSubscribersCalled, "Subscribers not called" );
    }

void MT_CFClient::MT_CCFClient_SubscribersNosubscribersPluginL( )
    {
    TInt err = KErrNone;

    // Subscribe to value context signalling plugin "reactions".
    // below                                        // CLEANUP<< valueSub
    CCFContextSubscription* valueSub = CCFContextSubscription::NewLC ( );
    valueSub->SetContextSourceL ( KTestContextSource );
    valueSub->SetContextTypeL ( KTestContextValueType );
    err = iClient->SubscribeContext ( *valueSub );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );

    // Test if plugin gets Subscribers() callback if we add subscription
    iExpectedSubscribersSource.Copy ( KTestContextSource );
    iExpectedSubscribersType.Copy ( KTestContextValueType );
    iExpectedSubscribersValue.Copy ( KTestContextValueSubsOk );

    // 1.Subscription
    iCheckPluginSubscribersCalled = ETrue;
    iPluginSubscribersCalled = EFalse;
    // below                                        // CLEANUP<< pluginSub1
    CCFContextSubscription* pluginSub1 = CCFContextSubscription::NewLC ( );
    pluginSub1->SetContextSourceL ( KTestContextSource );
    pluginSub1->SetContextTypeL ( KTestContextType );
    err = iClient->SubscribeContext ( *pluginSub1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );

    // Wait for correct context as answer.
    iTestDelay->Start ( 250000 ); // 250 ms
    EUNIT_ASSERT_DESC( iPluginSubscribersCalled, "SubscribeContext was not detected by plugin" );

    // 2.Subscription via iAnotherClient
    iCheckPluginSubscribersCalled = ETrue;
    iPluginSubscribersCalled = EFalse;
    // below                                        // CLEANUP<< pluginSub2
    CCFContextSubscription* pluginSub2 = CCFContextSubscription::NewLC ( );
    pluginSub2->SetContextSourceL ( KTestContextSource );
    pluginSub2->SetContextTypeL ( KTestContextType );
    err = iAnotherClient->SubscribeContext ( *pluginSub2 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );

    // Wait and no answer expected.
    iTestDelay->Start ( 500000 ); // 0.5 s
    EUNIT_ASSERT_DESC( iCheckPluginSubscribersCalled, "SubscribeContext was detected by plugin" );
    EUNIT_ASSERT_DESC( !iPluginSubscribersCalled, "SubscribeContext was detected by plugin" );

    // Unsubscribe 2.Subscription via iAnotherClient
    iCheckPluginSubscribersCalled = ETrue;
    iPluginSubscribersCalled = EFalse;
    err = iAnotherClient->UnsubscribeContext ( *pluginSub2 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect UnsubscribeContext return value!" );

    // Wait and no answer expected.
    iTestDelay->Start ( 250000 ); // 250 ms
    EUNIT_ASSERT_DESC( iCheckPluginSubscribersCalled, "UnsubscribeContext was detected by plugin" );
    EUNIT_ASSERT_DESC( !iPluginSubscribersCalled, "UnsubscribeContext was detected by plugin" );

    // Test if plugin gets NoSubscribers() callback if we remove last subscription
    iExpectedSubscribersSource.Copy ( KTestContextSource );
    iExpectedSubscribersType.Copy ( KTestContextValueType );
    iExpectedSubscribersValue.Copy ( KTestContextValueNosubsOk );

    // Unsubscribe 1.Subscription
    iCheckPluginSubscribersCalled = ETrue;
    iPluginSubscribersCalled = EFalse;
    err = iClient->UnsubscribeContext ( *pluginSub1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect UnsubscribeContext return value!" );

    // Wait for correct context as answer.
    iTestDelay->Start ( 500000 ); // 0.5 s
    EUNIT_ASSERT_DESC( iPluginSubscribersCalled, "UnsubscribeContext was not detected by plugin" );

    err = iClient->UnsubscribeContext ( *valueSub );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect UnsubscribeContext return value!" );

    CleanupStack::PopAndDestroy ( pluginSub2 ); //CLEANUP>> pluginSub2
    CleanupStack::PopAndDestroy ( pluginSub1 ); //CLEANUP>> pluginSub1
    CleanupStack::PopAndDestroy ( valueSub ); //CLEANUP>> valueSub
    }

void MT_CFClient::MT_CCFClient_SubscribersNosubscribersPlugin2L( )
    {
    TInt err = KErrNone;

    // Subscribe to value context signalling plugin "reactions".
    // below                                        // CLEANUP<< valueSub
    CCFContextSubscription* valueSub = CCFContextSubscription::NewLC ( );
    valueSub->SetContextSourceL ( KTestContextSource );
    valueSub->SetContextTypeL ( KTestContextValueType );
    err = iClient->SubscribeContext ( *valueSub );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );

    // Test if plugin gets Subscribers() callback if we add subscription
    iExpectedSubscribersSource.Copy ( KTestContextSource );
    iExpectedSubscribersType.Copy ( KTestContextValueType );
    iExpectedSubscribersValue.Copy ( KTestContextValueSubsOk );

    // 1.Subscription
    iCheckPluginSubscribersCalled = ETrue;
    iPluginSubscribersCalled = EFalse;
    // below                                        // CLEANUP<< pluginSub1
    CCFContextSubscription* pluginSub1 = CCFContextSubscription::NewLC ( );
    pluginSub1->SetContextSourceL ( KTestContextSource );
    pluginSub1->SetContextTypeL ( KTestContextType );
    err = iClient->SubscribeContext ( *pluginSub1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );

    // Wait for correct context as answer.
    iTestDelay->Start ( 250000 ); // 250 ms
    EUNIT_ASSERT_DESC( !iCheckPluginSubscribersCalled, "SubscribeContext was not detected by plugin" );
    EUNIT_ASSERT_DESC( iPluginSubscribersCalled, "SubscribeContext was not detected by plugin" );

    // 2.Subscription
    iCheckPluginSubscribersCalled = ETrue;
    iPluginSubscribersCalled = EFalse;
    // below                                        // CLEANUP<< pluginSub2
    CCFContextSubscription* pluginSub2 = CCFContextSubscription::NewLC ( );
    pluginSub2->SetContextSourceL ( KTestContextSource );
    pluginSub2->SetContextTypeL ( KTestContextType2 );
    err = iAnotherClient->SubscribeContext ( *pluginSub2 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );

    // Wait and no answer expected.
    iTestDelay->Start ( 500000 ); // 0.5 s
    EUNIT_ASSERT_DESC( !iCheckPluginSubscribersCalled, "SubscribeContext was not detected by plugin" );
    EUNIT_ASSERT_DESC( iPluginSubscribersCalled, "SubscribeContext was not detected by plugin" );

    // 3.Subscription dummy
    // below                                        // CLEANUP<< dummySub
    CCFContextSubscription* dummySub = CCFContextSubscription::NewLC();
    dummySub->SetContextSourceL( _L( "jusTaDummySubSource" ) );
    dummySub->SetContextTypeL( _L( "jusTaDummySubType" ) );
    err = iClient->SubscribeContext( *dummySub );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );

    // Test if plugin gets NoSubscribers() callback if we remove last subscription
    iExpectedSubscribersSource.Copy ( KTestContextSource );
    iExpectedSubscribersType.Copy ( KTestContextValueType );
    iExpectedSubscribersValue.Copy ( KTestContextValueNosubsOk );

    // Unsubscribe 1.Subscription
    iCheckPluginSubscribersCalled = ETrue;
    iPluginSubscribersCalled = EFalse;
    err = iClient->UnsubscribeContext ( *pluginSub1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect UnsubscribeContext return value!" );

    // Wait and no answer expected.
    iTestDelay->Start ( 500000 ); // 500 ms
    EUNIT_ASSERT_DESC( !iCheckPluginSubscribersCalled, "UnsubscribeContext was not detected by plugin" );
    EUNIT_ASSERT_DESC( iPluginSubscribersCalled, "UnsubscribeContext was not detected by plugin" );

    // Test if plugin gets NoSubscribers() callback if we remove last subscription
    iExpectedSubscribersSource.Copy ( KTestContextSource );
    iExpectedSubscribersType.Copy ( KTestContextValueType );
    iExpectedSubscribersValue.Copy ( KTestContextValueNosubsOk );

    // Unsubscribe 2.Subscription
    iCheckPluginSubscribersCalled = ETrue;
    iPluginSubscribersCalled = EFalse;
    err = iAnotherClient->UnsubscribeContext ( *pluginSub2 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect UnsubscribeContext return value!" );

    // Wait for correct context as answer.
    iTestDelay->Start ( 500000 ); // 0.5 s
    EUNIT_ASSERT_DESC( !iCheckPluginSubscribersCalled, "UnsubscribeContext was not detected by plugin" );
    EUNIT_ASSERT_DESC( iPluginSubscribersCalled, "UnsubscribeContext was not detected by plugin" );

    // Unsubscribe 3.Subscription dummy
    err = iClient->UnsubscribeContext( *dummySub );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect UnsubscribeContext return value!" );

    err = iClient->UnsubscribeContext ( *valueSub );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect UnsubscribeContext return value!" );

    CleanupStack::PopAndDestroy ( dummySub );   //CLEANUP>> dummySub
    CleanupStack::PopAndDestroy ( pluginSub2 ); //CLEANUP>> pluginSub2
    CleanupStack::PopAndDestroy ( pluginSub1 ); //CLEANUP>> pluginSub1
    CleanupStack::PopAndDestroy ( valueSub ); //CLEANUP>> valueSub
    }

void MT_CFClient::MT_CCFClient_SubscribersNosubscribersPlugin3L()
    {
    TInt err = KErrNone;

    // Subscribe to value context signalling plugin "reactions".
    // below                                        // CLEANUP<< valueSub
    CCFContextSubscription* valueSub = CCFContextSubscription::NewLC ( );
    valueSub->SetContextSourceL ( KTestContextSource );
    valueSub->SetContextTypeL ( KTestContextValueType );
    err = iClient->SubscribeContext ( *valueSub );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );

    // Test if plugin gets Subscribers() callback if we add subscription
    iExpectedSubscribersSource.Copy ( KTestContextSource );
    iExpectedSubscribersType.Copy ( KTestContextValueType );
    iExpectedSubscribersValue.Copy ( KTestContextValueSubsOk );

    // 1.Subscription
    iCheckPluginSubscribersCalled = ETrue;
    iPluginSubscribersCalled = EFalse;
    // below                                        // CLEANUP<< pluginSub1
    CCFContextSubscription* pluginSub1 = CCFContextSubscription::NewLC ( );
    pluginSub1->SetContextSourceL ( KTestContextSource );
    pluginSub1->SetContextTypeL ( KTestContextType );
    err = iClient->SubscribeContext ( *pluginSub1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );

    // Wait for correct context as answer.
    iTestDelay->Start ( 250000 ); // 250 ms
    EUNIT_ASSERT_DESC( !iCheckPluginSubscribersCalled, "SubscribeContext was not detected by plugin" );
    EUNIT_ASSERT_DESC( iPluginSubscribersCalled, "SubscribeContext was not detected by plugin" );

    // 2.Subscription
    iCheckPluginSubscribersCalled = ETrue;
    iPluginSubscribersCalled = EFalse;
    // below                                        // CLEANUP<< pluginSub2
    CCFContextSubscription* pluginSub2 = CCFContextSubscription::NewLC ( );
    pluginSub2->SetContextSourceL ( KTestContextSource );
    pluginSub2->SetContextTypeL ( KTestContextType2 );
    err = iAnotherClient->SubscribeContext ( *pluginSub2 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );

    // Wait and no answer expected.
    iTestDelay->Start ( 500000 ); // 0.5 s
    EUNIT_ASSERT_DESC( !iCheckPluginSubscribersCalled, "SubscribeContext was not detected by plugin" );
    EUNIT_ASSERT_DESC( iPluginSubscribersCalled, "SubscribeContext was not detected by plugin" );

    // 3.Subscription dummy
    // below                                        // CLEANUP<< dummySub
    CCFContextSubscription* dummySub = CCFContextSubscription::NewLC();
    dummySub->SetContextSourceL( _L( "jusTaDummySubSource" ) );
    dummySub->SetContextTypeL( _L( "jusTaDummySubType" ) );
    err = iClient->SubscribeContext( *dummySub );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );

    // Unsubscribe Subscriptions by deleting the client
    delete iClient;
    iClient = NULL;
    delete iAnotherClient;
    iAnotherClient = NULL;

    CleanupStack::PopAndDestroy ( dummySub );   //CLEANUP>> dummySub
    CleanupStack::PopAndDestroy ( pluginSub2 ); //CLEANUP>> pluginSub2
    CleanupStack::PopAndDestroy ( pluginSub1 ); //CLEANUP>> pluginSub1
    CleanupStack::PopAndDestroy ( valueSub ); //CLEANUP>> valueSub
    }

void MT_CFClient::MT_CCFClient_ContextSourceCommandPluginL( )
    {
    TInt err = KErrNone;

    // Subscribe to value context signalling plugin "reactions".
    // below                                    // CLEANUP<< sourceCmdSignalSub
    CCFContextSubscription* sourceCmdSignalSub = CCFContextSubscription::NewLC();
    sourceCmdSignalSub->SetContextSourceL( KSourceCommandSignalContextSource );
    sourceCmdSignalSub->SetContextTypeL( KSourceCommandSignalContextType );
    err = iClient->SubscribeContext( *sourceCmdSignalSub );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );

    // Define context                           // CLEANUP<< context
    CCFContextObject* context = CCFContextObject::NewLC(
            KSourceCommandPluginScriptContextSource,
            KSourceCommandScriptContextType,
            KSourceCommandScript1ContextValueDeactivate );
    err = iClient->DefineContext( context->Source(), context->Type(),
        KPassSec, KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value! " );

    // Registering scripts with context source command as action
    HBufC8* script = LoadScriptFromFile(_L("test-sourceCommandPlugIn.xml") );
    TInt scriptId( -1 );
    err = iClient->RegisterScript( _L("test-sourceCommandPlugIn.xml"),
            *script,
            scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when registering script!" );
    delete script;
    script = NULL;

    script = LoadScriptFromFile(_L("test-sourceCommandPlugIn2.xml") );
    TInt scriptId2( -1 );
    err = iClient->RegisterScript( _L("test-sourceCommandPlugIn2.xml"),
            *script,
            scriptId2 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when registering script!" );
    delete script;
    script = NULL;

    // 1. Publish script1 deactivate
    // Test values that plugin responds correctly to source command
    iExpectedSubscribersSource.Copy( KSourceCommandSignalContextSource );
    iExpectedSubscribersType.Copy( KSourceCommandSignalContextType );
    iExpectedSubscribersValue.Copy( KSourceCommandSignalContextValueDeactiveReqDeactivate );
    iCheckSourceCommandHandled = ETrue;
    iSourceCommandHandled = EFalse;
    context->SetValueL( KSourceCommandScript1ContextValueDeactivate );
    err = iClient->PublishContext( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    // Wait for correct context as answer.
    iWait->Start();
    EUNIT_ASSERT_DESC( !iCheckSourceCommandHandled, "Source command was not checked" );
    EUNIT_ASSERT_DESC( iSourceCommandHandled, "Source command was not detected by plugin" );

    // 2. Publish script2 deactivate
    iExpectedSubscribersValue.Copy( KSourceCommandSignalContextValueDeactiveReqDeactivate );
    iCheckSourceCommandHandled = ETrue;
    iSourceCommandHandled = EFalse;
    context->SetValueL( KSourceCommandScript2ContextValueDeactivate );
    err = iClient->PublishContext( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    // Wait for correct context as answer.
    iWait->Start();
    EUNIT_ASSERT_DESC( !iCheckSourceCommandHandled, "Source command was not checked" );
    EUNIT_ASSERT_DESC( iSourceCommandHandled, "Source command was not detected by plugin" );

    // 3. Publish script1 activate
    iExpectedSubscribersValue.Copy( KSourceCommandSignalContextValueActivated );
    iCheckSourceCommandHandled = ETrue;
    iSourceCommandHandled = EFalse;
    context->SetValueL( KSourceCommandScript1ContextValueActivate );
    err = iClient->PublishContext( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    // Wait for correct context as answer.
    iWait->Start();
    EUNIT_ASSERT_DESC( !iCheckSourceCommandHandled, "Source command was not checked" );
    EUNIT_ASSERT_DESC( iSourceCommandHandled, "Source command was not detected by plugin" );

    // 4. Publish script1 activate
    iExpectedSubscribersValue.Copy( KSourceCommandSignalContextValueActiveReqActivate );
    iCheckSourceCommandHandled = ETrue;
    iSourceCommandHandled = EFalse;
    context->SetValueL( KSourceCommandScript1ContextValueActivate );
    err = iClient->PublishContext( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    // Wait for correct context as answer.
    iWait->Start();
    EUNIT_ASSERT_DESC( !iCheckSourceCommandHandled, "Source command was not checked" );
    EUNIT_ASSERT_DESC( iSourceCommandHandled, "Source command was not detected by plugin" );

    // 5. Publish script2 activate
    iExpectedSubscribersValue.Copy( KSourceCommandSignalContextValueActiveReqActivate );
    iCheckSourceCommandHandled = ETrue;
    iSourceCommandHandled = EFalse;
    context->SetValueL( KSourceCommandScript2ContextValueActivate );
    err = iClient->PublishContext( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    // Wait for correct context as answer.
    iWait->Start();
    EUNIT_ASSERT_DESC( !iCheckSourceCommandHandled, "Source command was not checked" );
    EUNIT_ASSERT_DESC( iSourceCommandHandled, "Source command was not detected by plugin" );

    // 6. Publish script2 deactivate
    iExpectedSubscribersValue.Copy( KSourceCommandSignalContextValueActiveReqDeactivate );
    iCheckSourceCommandHandled = ETrue;
    iSourceCommandHandled = EFalse;
    context->SetValueL( KSourceCommandScript2ContextValueDeactivate );
    err = iClient->PublishContext( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    // Wait for correct context as answer.
    iWait->Start();
    EUNIT_ASSERT_DESC( !iCheckSourceCommandHandled, "Source command was not checked" );
    EUNIT_ASSERT_DESC( iSourceCommandHandled, "Source command was not detected by plugin" );

    // 7. Publish script2 deactivate
    iExpectedSubscribersValue.Copy( KSourceCommandSignalContextValueActiveReqDeactivate );
    iCheckSourceCommandHandled = ETrue;
    iSourceCommandHandled = EFalse;
    context->SetValueL( KSourceCommandScript2ContextValueDeactivate );
    err = iClient->PublishContext( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    // Wait for correct context as answer.
    iWait->Start();
    EUNIT_ASSERT_DESC( !iCheckSourceCommandHandled, "Source command was not checked" );
    EUNIT_ASSERT_DESC( iSourceCommandHandled, "Source command was not detected by plugin" );

    // 8. Publish script1 deactivate
    iExpectedSubscribersValue.Copy( KSourceCommandSignalContextValueDeactivated );
    iCheckSourceCommandHandled = ETrue;
    iSourceCommandHandled = EFalse;
    context->SetValueL( KSourceCommandScript1ContextValueDeactivate );
    err = iClient->PublishContext( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    // Wait for correct context as answer.
    iWait->Start();
    EUNIT_ASSERT_DESC( !iCheckSourceCommandHandled, "Source command was not checked" );
    EUNIT_ASSERT_DESC( iSourceCommandHandled, "Source command was not detected by plugin" );

    // 9. Publish script2 deactivate
    iExpectedSubscribersValue.Copy( KSourceCommandSignalContextValueDeactiveReqDeactivate );
    iCheckSourceCommandHandled = ETrue;
    iSourceCommandHandled = EFalse;
    context->SetValueL( KSourceCommandScript2ContextValueDeactivate );
    err = iClient->PublishContext( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    // Wait for correct context as answer.
    iWait->Start();
    EUNIT_ASSERT_DESC( !iCheckSourceCommandHandled, "Source command was not checked" );
    EUNIT_ASSERT_DESC( iSourceCommandHandled, "Source command was not detected by plugin" );

    err = iClient->UnsubscribeContext( *sourceCmdSignalSub );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect UnsubscribeContext return value!" );

    err = iClient->DeregisterScript( scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when de-registering script!" );
    err = iClient->DeregisterScript( scriptId2 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when de-registering script!" );

    CleanupStack::PopAndDestroy( context );     // CLEANUP>> context
    CleanupStack::PopAndDestroy( sourceCmdSignalSub );//CLEANUP>> sourceCmdSignalSub
    }

void MT_CFClient::MT_CCFClient_ContextSourceCommandClientL( )
    {
    TInt err = KErrNone;

    // Define context                           // CLEANUP<< context
    CCFContextObject* context = CCFContextObject::NewLC(
            KSourceCommandClientScriptContextSource,
            KSourceCommandScriptContextType,
            KSourceCommandScript1ContextValueDeactivate );
    err = iClient->DefineContext( context->Source(), context->Type(),
        KPassSec, KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value! " );

    // Registering scripts with context source command as action
    HBufC8* script = LoadScriptFromFile(_L("test-sourceCommand.xml") );
    TInt scriptId( -1 );
    err = iClient->RegisterScript( _L("test-sourceCommand.xml"),
            *script,
            scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when registering script!" );
    delete script;
    script = NULL;

    script = LoadScriptFromFile(_L("test-sourceCommand2.xml") );
    TInt scriptId2( -1 );
    err = iClient->RegisterScript( _L("test-sourceCommand2.xml"),
            *script,
            scriptId2 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when registering script!" );
    delete script;
    script = NULL;

    // 1. Publish script1 deactivate
    iHandleCommandCalled = EFalse;
    context->SetValueL( KSourceCommandScript1ContextValueDeactivate );
    err = iClient->PublishContext( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    // Wait for call to handle command.
    iWait->Start();
    EUNIT_ASSERT_DESC( iHandleCommandCalled, "Source command was not detected by client" );
    EUNIT_ASSERT_DESC( iActiveRequests.Count() == 0, "Client source active request count error" );

    // 2. Publish script2 deactivate
    iHandleCommandCalled = EFalse;
    context->SetValueL( KSourceCommandScript2ContextValueDeactivate );
    err = iClient->PublishContext( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    // Wait for call to handle command.
    iWait->Start();
    EUNIT_ASSERT_DESC( iHandleCommandCalled, "Source command was not detected by client" );
    EUNIT_ASSERT_DESC( iActiveRequests.Count() == 0, "Client source active request count error" );

    // 3. Publish script1 activate
    iHandleCommandCalled = EFalse;
    context->SetValueL( KSourceCommandScript1ContextValueActivate );
    err = iClient->PublishContext( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    // Wait for call to handle command.
    iWait->Start();
    EUNIT_ASSERT_DESC( iHandleCommandCalled, "Source command was not detected by client" );
    EUNIT_ASSERT_DESC( iActiveRequests.Count() == 1, "Client source active request count error" );

    // 4. Publish script1 activate
    iHandleCommandCalled = EFalse;
    context->SetValueL( KSourceCommandScript1ContextValueActivate );
    err = iClient->PublishContext( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    // Wait for call to handle command.
    iWait->Start();
    EUNIT_ASSERT_DESC( iHandleCommandCalled, "Source command was not detected by client" );
    EUNIT_ASSERT_DESC( iActiveRequests.Count() == 1, "Client source active request count error" );

    // 5. Publish script2 activate
    iHandleCommandCalled = EFalse;
    context->SetValueL( KSourceCommandScript2ContextValueActivate );
    err = iClient->PublishContext( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    // Wait for call to handle command.
    iWait->Start();
    EUNIT_ASSERT_DESC( iHandleCommandCalled, "Source command was not detected by client" );
    EUNIT_ASSERT_DESC( iActiveRequests.Count() == 2, "Client source active request count error" );

    // 6. Publish script2 deactivate
    iHandleCommandCalled = EFalse;
    context->SetValueL( KSourceCommandScript2ContextValueDeactivate );
    err = iClient->PublishContext( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    // Wait for call to handle command.
    iWait->Start();
    EUNIT_ASSERT_DESC( iHandleCommandCalled, "Source command was not detected by client" );
    EUNIT_ASSERT_DESC( iActiveRequests.Count() == 1, "Client source active request count error" );

    // 7. Publish script2 deactivate
    iHandleCommandCalled = EFalse;
    context->SetValueL( KSourceCommandScript2ContextValueDeactivate );
    err = iClient->PublishContext( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    // Wait for call to handle command.
    iWait->Start();
    EUNIT_ASSERT_DESC( iHandleCommandCalled, "Source command was not detected by client" );
    EUNIT_ASSERT_DESC( iActiveRequests.Count() == 1, "Client source active request count error" );

    // 8. Publish script1 deactivate
    iHandleCommandCalled = EFalse;
    context->SetValueL( KSourceCommandScript1ContextValueDeactivate );
    err = iClient->PublishContext( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    // Wait for call to handle command.
    iWait->Start();
    EUNIT_ASSERT_DESC( iHandleCommandCalled, "Source command was not detected by client" );
    EUNIT_ASSERT_DESC( iActiveRequests.Count() == 0, "Client source active request count error" );

    // 9. Publish script2 deactivate
    iHandleCommandCalled = EFalse;
    context->SetValueL( KSourceCommandScript2ContextValueDeactivate );
    err = iClient->PublishContext( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    // Wait for call to handle command.
    iWait->Start();
    EUNIT_ASSERT_DESC( iHandleCommandCalled, "Source command was not detected by client" );
    EUNIT_ASSERT_DESC( iActiveRequests.Count() == 0, "Client source active request count error" );

    err = iClient->DeregisterScript( scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when de-registering script!" );
    err = iClient->DeregisterScript( scriptId2 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when de-registering script!" );

    CleanupStack::PopAndDestroy( context );     // CLEANUP>> context
    }

void MT_CFClient::MT_CCFClient_PublishContextL( )
    {
    CCFContextObject* context = CCFContextObject::NewLC ( );
    CCFClient* client = CCFClient::NewLC ( *this );
    TInt err = KErrNone;

    // Try to publish context which has not been defined
    ConfigureContextL ( *context );
    err = client->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNotFound, "Incorrect PublishContext return value!" );

    // Define context and publish it
    err = client->DefineContext ( context->Source ( ), context->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );
    err = client->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );

    // Delete client and publish same context
    CleanupStack::PopAndDestroy ( client );
    client = CCFClient::NewLC ( *this );
    err = client->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );

    // Define a new context and publish it
    ConfigureContextL ( *context );
    err = client->DefineContext ( context->Source ( ), context->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );
    err = client->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );

    // Test incorrect context values
    context->SetSourceL ( KNullDesC );
    err = client->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrArgument, "Incorrect PublishContext return value!" );
    context->SetTypeL ( KNullDesC );
    err = client->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrArgument, "Incorrect PublishContext return value!" );
    context->SetValueL ( KNullDesC );
    err = client->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrArgument, "Incorrect PublishContext return value!" );

    // Define context where we do not have access
    ConfigureContextL ( *context );
    err = client->DefineContext ( context->Source ( ), context->Type ( ),
        KFailSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );
    err = client->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrAccessDenied, "Incorrect PublishContext return value!" );

    // Clean up
    CleanupStack::PopAndDestroy ( client );
    CleanupStack::PopAndDestroy ( context );
    }

void MT_CFClient::MT_CCFClient_PublishContextWithDataL( )
    {
    _LIT( KKey, "Key" );
    _LIT( KValue, "Value" );

    CTestContextData* contextData = CTestContextData::NewLC ( KKey, KValue );
    CCFContextObject* context = CCFContextObject::NewLC ( );
    CCFClient* client = CCFClient::NewLC ( *this );
    TInt err = KErrNone;

    // Try to publish context which has not been defined
    ConfigureContextL ( *context );
    err = client->PublishContext ( *context, *contextData );
    EUNIT_ASSERT_DESC( err == KErrNotFound, "Incorrect PublishContext return value!" );

    // Define context and publish it
    err = client->DefineContext ( context->Source ( ), context->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );
    err = client->PublishContext ( *context, *contextData );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );

    // Delete client and publish same context
    CleanupStack::PopAndDestroy ( client );
    client = CCFClient::NewLC ( *this );
    err = client->PublishContext ( *context, *contextData );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );

    // Define a new context and publish it
    ConfigureContextL ( *context );
    err = client->DefineContext ( context->Source ( ), context->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );
    err = client->PublishContext ( *context, *contextData );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );

    // Test incorrect context values
    context->SetSourceL ( KNullDesC );
    err = client->PublishContext ( *context, *contextData );
    EUNIT_ASSERT_DESC( err == KErrArgument, "Incorrect PublishContext return value!" );
    context->SetTypeL ( KNullDesC );
    err = client->PublishContext ( *context, *contextData );
    EUNIT_ASSERT_DESC( err == KErrArgument, "Incorrect PublishContext return value!" );
    context->SetValueL ( KNullDesC );
    err = client->PublishContext ( *context, *contextData );
    EUNIT_ASSERT_DESC( err == KErrArgument, "Incorrect PublishContext return value!" );

    // Define context where we do not have access
    ConfigureContextL ( *context );
    err = client->DefineContext ( context->Source ( ), context->Type ( ),
        KFailSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );
    err = client->PublishContext ( *context, *contextData );
    EUNIT_ASSERT_DESC( err == KErrAccessDenied, "Incorrect PublishContext return value!" );

    // Publish context data with invalid UID
    ConfigureContextL ( *context );
    contextData->SetUid ( KNullUid );
    err = client->DefineContext ( context->Source ( ), context->Type ( ),
        KFailSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );
    err = client->PublishContext ( *context, *contextData );
    EUNIT_ASSERT_DESC( err == KErrNotSupported, "Incorrect PublishContext return value!" );

    // Publish too big context data
    const TInt length = 16*1024;
    HBufC* data = HBufC::NewLC ( length );
    TPtr dataPtr( data->Des ( ));
    dataPtr.Fill ( 'z', length );
    contextData->SetKeyL ( dataPtr );
    contextData->SetValueL ( dataPtr );
    contextData->SetUid ( KTestContextDataUid );
    CleanupStack::PopAndDestroy ( data );
    ConfigureContextL ( *context );
    err = client->DefineContext ( context->Source ( ), context->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );
    err = client->PublishContext ( *context, *contextData );
    EUNIT_ASSERT_DESC( err == KErrTooBig, "Incorrect PublishContext return value!" );

    // Clean up
    CleanupStack::PopAndDestroy ( client );
    CleanupStack::PopAndDestroy ( context );
    CleanupStack::PopAndDestroy ( contextData );
    }

void MT_CFClient::MT_CCFClient_SubscribeContext1L( )
    {
    CCFContextSubscription* subscription=  NULL;
    TInt err = KErrNone;

    // Define a context and subscribe it
    ConfigureContextL ( *iContextA );
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );
    subscription = CCFContextSubscription::NewLC ( );
    subscription->SetContextSourceL ( iContextA->Source ( ) );
    subscription->SetContextTypeL ( iContextA->Type ( ) );
    err = iClient->SubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscription );

    // Publish a new context value and wait for context indication
    UpdateContextValueL ( *iContextA );
    err = iClient->PublishContext ( *iContextA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    iWait->Start ( );

    // Publish a new context value and wait for context indication
    UpdateContextValueL ( *iContextA );
    err = iClient->PublishContext ( *iContextA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    iWait->Start ( );
    }

void MT_CFClient::MT_CCFClient_SubscribeContext2L( )
    {
    CCFContextSubscription* subscription=  NULL;
    TInt err = KErrNone;

    // Define a new context with no access and wait for error message
    ConfigureContextL ( *iContextB );
    err = iClient->DefineContext ( iContextB->Source ( ), iContextB->Type ( ),
        KFailSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );
    subscription = CCFContextSubscription::NewLC ( );
    subscription->SetContextSourceL ( iContextB->Source ( ) );
    subscription->SetContextTypeL ( iContextB->Type ( ) );
    err = iClient->SubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscription );
    iCheckB = ETrue;
    iWait->Start ( );
    }

void MT_CFClient::MT_CCFClient_SubscribeContext3L( )
    {
    CCFContextSubscription* subscription=  NULL;
    TInt err = KErrNone;

    // Define two contexts with access and subscribe both of them
    ConfigureContextL ( *iContextA );
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );
    ConfigureContextL ( *iContextB );
    err = iClient->DefineContext ( iContextB->Source ( ), iContextB->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    subscription = CCFContextSubscription::NewLC ( );
    subscription->SetContextSourceL ( iContextA->Source ( ) );
    subscription->SetContextTypeL ( iContextA->Type ( ) );
    err = iClient->SubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscription );

    subscription = CCFContextSubscription::NewLC ( );
    subscription->SetContextSourceL ( iContextB->Source ( ) );
    subscription->SetContextTypeL ( iContextB->Type ( ) );
    err = iClient->SubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscription );

    // Publish both context at the same time and wait for two indications
    UpdateContextValueL ( *iContextA );
    err = iClient->PublishContext ( *iContextA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    UpdateContextValueL ( *iContextB );
    err = iClient->PublishContext ( *iContextB );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckB = ETrue;
    iWait->Start ( );

    // Publish both context at the same time and wait for two indications
    UpdateContextValueL ( *iContextA );
    err = iClient->PublishContext ( *iContextA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    UpdateContextValueL ( *iContextB );
    err = iClient->PublishContext ( *iContextB );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckB = ETrue;
    iWait->Start ( );
    }

void MT_CFClient::MT_CCFClient_SubscribeContext4L( )
    {
    CCFContextSubscription* subscription=  NULL;
    TInt err = KErrNone;

    // Define two context where other we have access and other we don't
    // Subscribe both contexts
    ConfigureContextL ( *iContextA );
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );
    ConfigureContextL ( *iContextB );
    err = iClient->DefineContext ( iContextB->Source ( ), iContextB->Type ( ),
        KFailSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    subscription = CCFContextSubscription::NewLC ( );
    subscription->SetContextSourceL ( iContextA->Source ( ) );
    subscription->SetContextTypeL ( iContextA->Type ( ) );
    err = iClient->SubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscription );

    subscription = CCFContextSubscription::NewLC ( );
    subscription->SetContextSourceL ( iContextB->Source ( ) );
    subscription->SetContextTypeL ( iContextB->Type ( ) );
    err = iClient->SubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscription );

    // Publish both context at the same time and wait for two indications
    iCheckA = ETrue;
    iCheckB = ETrue;

    UpdateContextValueL ( *iContextA );
    err = iClient->PublishContext ( *iContextA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    UpdateContextValueL ( *iContextB );
    err = iClient->PublishContext ( *iContextB );
    EUNIT_ASSERT_DESC( err == KErrAccessDenied, "Incorrect PublishContext return value!" );
    if ( iCheckA || iCheckB )
        {
        iWait->Start ( );
        }
    }

void MT_CFClient::MT_CCFClient_SubscribeContext5L( )
    {
    CCFContextSubscription* subscription=  NULL;
    TInt err = KErrNone;

    // First subscribe a context
    ConfigureContextL ( *iContextA );
    subscription = CCFContextSubscription::NewLC ( );
    subscription->SetContextSourceL ( iContextA->Source ( ) );
    subscription->SetContextTypeL ( iContextA->Type ( ) );
    err = iClient->SubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscription );

    // Define the context with access
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    // Publish new context value and wait for indication
    UpdateContextValueL ( *iContextA );
    err = iClient->PublishContext ( *iContextA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    iWait->Start ( );
    }

void MT_CFClient::MT_CCFClient_SubscribeContext6L( )
    {
    CCFContextSubscription* subscription=  NULL;
    TInt err = KErrNone;

    // First subscribe a context
    ConfigureContextL ( *iContextB );
    subscription = CCFContextSubscription::NewLC ( );
    subscription->SetContextSourceL ( iContextB->Source ( ) );
    subscription->SetContextTypeL ( iContextB->Type ( ) );
    err = iClient->SubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscription );

    // Define the context with no access and wait for error
    err = iClient->DefineContext ( iContextB->Source ( ), iContextB->Type ( ),
        KFailSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );
    iCheckB = ETrue;
    iWait->Start ( );
    }

void MT_CFClient::MT_CCFClient_SubscribeContext7L( )
    {
    CCFContextSubscription* subscription=  NULL;
    TInt err = KErrNone;

    // Define two contexts with no access
    ConfigureContextL ( *iContextB );
    err = iClient->DefineContext ( iContextB->Source ( ), iContextB->Type ( ),
        KFailSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );
    ConfigureContextL ( *iContextC );
    err = iClient->DefineContext ( iContextC->Source ( ), iContextC->Type ( ),
        KFailSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    // subscribe both contexts and wait for errors
    subscription = CCFContextSubscription::NewLC ( );
    subscription->SetContextSourceL ( iContextB->Source ( ) );
    subscription->SetContextTypeL ( iContextB->Type ( ) );
    err = iClient->SubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscription );

    subscription = CCFContextSubscription::NewLC ( );
    subscription->SetContextSourceL ( iContextC->Source ( ) );
    subscription->SetContextTypeL ( iContextC->Type ( ) );
    err = iClient->SubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscription );
    iCheckB = ETrue;
    iCheckC = ETrue;
    iWait->Start ( );
    }

void MT_CFClient::MT_CCFClient_SubscribeContext8L( )
    {
    CCFContextSubscription* subscription=  NULL;
    TInt err = KErrNone;

    // Define two contexts
    ConfigureContextL ( *iContextA );
    iContextA->SetTypeL ( KContextTypeFormatWild );
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );
    iContextB->CopyL ( *iContextA );
    UpdateContextTypeL ( *iContextB );
    err = iClient->DefineContext ( iContextB->Source ( ), iContextB->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    // Subscribe with sub type match
    subscription = CCFContextSubscription::NewLC ( );
    subscription->SetContextSourceL ( iContextA->Source ( ) );
    subscription->SetContextTypeL ( iContextA->Type ( ) );
    subscription->SetSubTypeMatch ( ETrue );
    err = iClient->SubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscription );

    // Publish contextB and wait for indication
    UpdateContextValueL ( *iContextB );
    err = iClient->PublishContext ( *iContextB );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckB = ETrue;
    iWait->Start ( );

    // Publish contextA and wait for indication
    UpdateContextValueL ( *iContextA );
    err = iClient->PublishContext ( *iContextA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    iWait->Start ( );
    }

void MT_CFClient::MT_CCFClient_SubscribeContext9L( )
    {
    CCFContextSubscription* subscription=  NULL;
    TInt err = KErrNone;

    // Run this set twice to get regression
    static const TInt KCount = 2;
    for (TInt i = 0; i < KCount; i++ )
        {
        // Define a context
        ConfigureContextL ( *iContextA );
        err = iClient->DefineContext ( iContextA->Source ( ),
            iContextA->Type ( ), KPassSec );
        EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

        // Subscribe context but indicate only changes
        subscription = CCFContextSubscription::NewLC ( );
        subscription->SetContextSourceL ( iContextA->Source ( ) );
        subscription->SetContextTypeL ( iContextA->Type ( ) );
        subscription->SetOnlyChangesIndicated ( ETrue );
        err = iClient->SubscribeContext ( *subscription );
        EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
        CleanupStack::PopAndDestroy ( subscription );

        // Update context value, Publish context and wait for indication
        UpdateContextValueL ( *iContextA );
        err = iClient->PublishContext ( *iContextA );
        EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
        iCheckA = ETrue;
        iWait->Start ( );

        // Publish same context, context indication should not happen
        err = iClient->PublishContext ( *iContextA );
        EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
        iCheckA = ETrue;
        Wait ( KDelay );
        EUNIT_ASSERT_DESC( iCheckA, "Context should not have been indicated!" );
        iCheckA = EFalse;

        // Define a context
        ConfigureContextL ( *iContextA );
        err = iClient->DefineContext ( iContextA->Source ( ),
            iContextA->Type ( ), KPassSec );
        EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

        // Subscribe context but indicate only changes
        subscription = CCFContextSubscription::NewLC ( );
        subscription->SetContextSourceL ( iContextA->Source ( ) );
        subscription->SetContextTypeL ( iContextA->Type ( ) );
        subscription->SetOnlyChangesIndicated ( EFalse );
        err = iClient->SubscribeContext ( *subscription );
        EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
        CleanupStack::PopAndDestroy ( subscription );

        // Publish context and wait for indication
        UpdateContextValueL ( *iContextA );
        err = iClient->PublishContext ( *iContextA );
        EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
        iCheckA = ETrue;
        iWait->Start ( );

        // Publish context and wait for indication
        UpdateContextValueL ( *iContextA );
        err = iClient->PublishContext ( *iContextA );
        EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
        iCheckA = ETrue;
        iDeleteClient = ETrue;
        iWait->Start ( );

        EUNIT_ASSERT_DESC( !iClient, "Client not deleted!" );
        iClient = CCFClient::NewL ( *this );
        }
    }

void MT_CFClient::MT_CCFClient_SubscribeContextFromDifferentClientsL( )
    {
    CCFContextSubscription* subscription=  NULL;
    TInt err = KErrNone;

    // Define two contexts with access and subscribe both of them
    ConfigureContextL ( *iContextA );
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );
    ConfigureContextL ( *iContextB );
    err = iAnotherClient->DefineContext ( iContextB->Source ( ),
        iContextB->Type ( ), KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    subscription = CCFContextSubscription::NewLC ( );
    subscription->SetContextSourceL ( iContextA->Source ( ) );
    subscription->SetContextTypeL ( iContextA->Type ( ) );
    err = iClient->SubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscription );

    subscription = CCFContextSubscription::NewLC ( );
    subscription->SetContextSourceL ( iContextB->Source ( ) );
    subscription->SetContextTypeL ( iContextB->Type ( ) );
    err = iAnotherClient->SubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscription );

    // Publish both context at the same time and wait for two indications
    UpdateContextValueL ( *iContextA );
    err = iClient->PublishContext ( *iContextA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    UpdateContextValueL ( *iContextB );
    err = iAnotherClient->PublishContext ( *iContextB );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckB = ETrue;
    iWait->Start ( );

    // Publish both context at the same time and wait for two indications
    UpdateContextValueL ( *iContextA );
    err = iAnotherClient->PublishContext ( *iContextA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    UpdateContextValueL ( *iContextB );
    err = iClient->PublishContext ( *iContextB );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckB = ETrue;
    iWait->Start ( );
    }

void MT_CFClient::MT_CCFClient_SubscribeContextCombinedL( )
    {
    MT_CCFClient_SubscribeContext1L ( );
    Wait ( 5000 );
    MT_CCFClient_SubscribeContext2L ( );
    Wait ( 5000 );
    MT_CCFClient_SubscribeContext3L ( );
    Wait ( 5000 );
    MT_CCFClient_SubscribeContext4L ( );
    Wait ( 5000 );
    MT_CCFClient_SubscribeContext5L ( );
    Wait ( 5000 );
    MT_CCFClient_SubscribeContext6L ( );
    Wait ( 5000 );
    MT_CCFClient_SubscribeContext7L ( );
    Wait ( 5000 );
    MT_CCFClient_SubscribeContext8L ( );
    Wait ( 5000 );
    MT_CCFClient_SubscribeContext9L ( );
    }

void MT_CFClient::MT_CCFClient_SubscribeContextData1L( )
    {
    iCheckContextData = ETrue;
    CCFContextSubscription* subscription=  NULL;
    TInt err = KErrNone;

    // Define a context
    // Subscribe the context with data
    // Publish new context value with data
    ConfigureContextL ( *iContextA );
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    subscription = CCFContextSubscription::NewLC ( );
    subscription->SetDataObjectEnabled ( ETrue );
    subscription->SetContextSourceL ( iContextA->Source ( ) );
    subscription->SetContextTypeL ( iContextA->Type ( ) );
    err = iClient->SubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );

    UpdateContextValueL ( *iContextA );
    err = iClient->PublishContext ( *iContextA, *iContextDataA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    iWait->Start ( );

    // Unsubscribe context
    // Publish new context value, context indication should not happen
    err = iClient->UnsubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect UnsubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscription );

    UpdateContextValueL ( *iContextA );
    err = iClient->PublishContext ( *iContextA, *iContextDataA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    Wait ( KDelay );
    EUNIT_ASSERT_DESC( iCheckA, "Context should not have been indicated!" );
    }

void MT_CFClient::MT_CCFClient_SubscribeContextData2L( )
    {
    iCheckContextData = ETrue;
    CCFContextSubscription* subscriptionA=  NULL;
    CCFContextSubscription* subscriptionB=  NULL;
    TInt err = KErrNone;

    // Define two contexts
    // Subscribe the contexts with datas
    // Publish new context values with datas
    ConfigureContextL ( *iContextA );
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );
    ConfigureContextL ( *iContextB );
    err = iClient->DefineContext ( iContextB->Source ( ), iContextB->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    subscriptionA = CCFContextSubscription::NewLC ( );
    subscriptionA->SetDataObjectEnabled ( ETrue );
    subscriptionA->SetContextSourceL ( iContextA->Source ( ) );
    subscriptionA->SetContextTypeL ( iContextA->Type ( ) );
    err = iClient->SubscribeContext ( *subscriptionA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscriptionA );
    subscriptionB = CCFContextSubscription::NewLC ( );
    subscriptionB->SetDataObjectEnabled ( ETrue );
    subscriptionB->SetContextSourceL ( iContextB->Source ( ) );
    subscriptionB->SetContextTypeL ( iContextB->Type ( ) );
    err = iClient->SubscribeContext ( *subscriptionB );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscriptionB );

    UpdateContextValueL ( *iContextA );
    err = iClient->PublishContext ( *iContextA, *iContextDataA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    UpdateContextValueL ( *iContextB );
    err = iClient->PublishContext ( *iContextB, *iContextDataB );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckB = ETrue;
    iWait->Start ( );
    }

void MT_CFClient::MT_CCFClient_SubscribeContextData3L( )
    {
    iCheckContextData = ETrue;
    CCFContextSubscription* subscriptionA=  NULL;
    CCFContextSubscription* subscriptionB=  NULL;
    TInt err = KErrNone;

    // Define two contexts from different clients
    // Subscribe the contexts with datas from different clients
    // Publish new context values with datas from different clients
    ConfigureContextL ( *iContextA );
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );
    ConfigureContextL ( *iContextB );
    err = iAnotherClient->DefineContext ( iContextB->Source ( ),
        iContextB->Type ( ), KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    subscriptionA = CCFContextSubscription::NewLC ( );
    subscriptionA->SetDataObjectEnabled ( ETrue );
    subscriptionA->SetContextSourceL ( iContextA->Source ( ) );
    subscriptionA->SetContextTypeL ( iContextA->Type ( ) );
    err = iClient->SubscribeContext ( *subscriptionA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscriptionA );
    subscriptionB = CCFContextSubscription::NewLC ( );
    subscriptionB->SetDataObjectEnabled ( ETrue );
    subscriptionB->SetContextSourceL ( iContextB->Source ( ) );
    subscriptionB->SetContextTypeL ( iContextB->Type ( ) );
    err = iAnotherClient->SubscribeContext ( *subscriptionB );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscriptionB );

    UpdateContextValueL ( *iContextA );
    err = iClient->PublishContext ( *iContextA, *iContextDataA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    UpdateContextValueL ( *iContextB );
    err = iAnotherClient->PublishContext ( *iContextB, *iContextDataB );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckB = ETrue;
    iWait->Start ( );
    }

void MT_CFClient::MT_CCFClient_SubscribeContextData4L( )
    {
    iCheckContextData = ETrue;
    CCFContextSubscription* subscription=  NULL;
    TInt err = KErrNone;

    // Define a context
    // Subscribe the context
    // Publish new context value from context source plug-in
    ConfigureContextL ( *iContextA );
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    subscription = CCFContextSubscription::NewLC ( );
    subscription->SetDataObjectEnabled ( ETrue );
    subscription->SetContextSourceL ( iContextA->Source ( ) );
    subscription->SetContextTypeL ( iContextA->Type ( ) );
    err = iClient->SubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscription );

    CCFContextObjectImpl* contextImpl = static_cast<CCFContextObjectImpl*>( iContextA );
    UpdateContextValueL ( *iContextA );
    HBufC8* buf = HBufC8::NewLC ( contextImpl->Size ( ) );
    TPtr8 bufPtr( buf->Des ( ));
    RDesWriteStream stream( bufPtr);
    stream.PushL ( );
    contextImpl->ExternalizeL ( stream );
    err = RProperty::Set ( KContextSourceTestCategory, KPublishContextKey,
        bufPtr );
    EUNIT_ASSERT_DESC( err == KErrNone, "Failed to set property!" );
    CleanupStack::PopAndDestroy ( &stream );
    CleanupStack::PopAndDestroy ( buf );
    iCheckA = ETrue;
    iWait->Start ( );
    }

void MT_CFClient::MT_CCFClient_SubscribeContextDataCombinedL( )
    {
    MT_CCFClient_SubscribeContextData1L ( );
    Wait ( 5000 );
    MT_CCFClient_SubscribeContextData2L ( );
    Wait ( 5000 );
    MT_CCFClient_SubscribeContextData3L ( );
    Wait ( 5000 );
    MT_CCFClient_SubscribeContextData4L ( );
    }

void MT_CFClient::MT_CCFClient_UnsubscribeContext1L( )
    {
    iCheckContextData = EFalse;
    CCFContextSubscription* subscription=  NULL;
    TInt err = KErrNone;

    // Define a context
    ConfigureContextL ( *iContextA );
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    // Subscribe context
    subscription = CCFContextSubscription::NewLC ( );
    subscription->SetContextSourceL ( iContextA->Source ( ) );
    subscription->SetContextTypeL ( iContextA->Type ( ) );
    err = iClient->SubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );

    // Publish context and wait for indication
    iCheckA = ETrue;
    UpdateContextValueL ( *iContextA );
    err = iClient->PublishContext ( *iContextA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    if ( iCheckA )
        {
        iWait->Start ( );
        }

    // Unsubscribe context
    err = iClient->UnsubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect UnsubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscription );

    // Publish context, context indication should not happen
    iCheckA = ETrue;
    UpdateContextValueL ( *iContextA );
    err = iClient->PublishContext ( *iContextA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    Wait ( KDelay );
    EUNIT_ASSERT_DESC( iCheckA, "Context should not have been indicated!" );
    iCheckA = EFalse;
    }

void MT_CFClient::MT_CCFClient_UnsubscribeContext2L( )
    {
    CCFContextSubscription* subscription=  NULL;
    TInt err = KErrNone;

    // Define a context
    ConfigureContextL ( *iContextA );
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    // Subscribe context
    subscription = CCFContextSubscription::NewLC ( );
    subscription->SetContextSourceL ( iContextA->Source ( ) );
    subscription->SetContextTypeL ( iContextA->Type ( ) );
    err = iClient->SubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscription );

    // Publish context and wait for indication
    UpdateContextValueL ( *iContextA );
    err = iClient->PublishContext ( *iContextA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    iWait->Start ( );

    // Delete client
    delete iClient;
    iClient = NULL;
    iClient = CCFClient::NewL ( *this );

    // Publish context, context indication should not happen
    UpdateContextValueL ( *iContextA );
    err = iClient->PublishContext ( *iContextA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    Wait ( KDelay );
    EUNIT_ASSERT_DESC( iCheckA, "Context should not have been indicated!" );
    iCheckA = EFalse;

    // Define a context
    ConfigureContextL ( *iContextA );
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    // Subscribe context
    subscription = CCFContextSubscription::NewLC ( );
    subscription->SetContextSourceL ( iContextA->Source ( ) );
    subscription->SetContextTypeL ( iContextA->Type ( ) );
    err = iClient->SubscribeContext ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeContext return value!" );
    CleanupStack::PopAndDestroy ( subscription );

    // Publish context and wait for indication
    UpdateContextValueL ( *iContextA );
    err = iClient->PublishContext ( *iContextA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    iWait->Start ( );
    }

void MT_CFClient::MT_CCFClient_UnsubscribeContextCombinedL( )
    {
    MT_CCFClient_UnsubscribeContext1L ( );
    Wait ( 5000 );
    MT_CCFClient_UnsubscribeContext2L ( );
    }

void MT_CFClient::MT_CCFClient_RequestContext1L( )
    {
    CCFContextQuery* query=  NULL;
    TInt err = KErrNone;

    // Define a context
    ConfigureContextL ( *iContextA );
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    // Publish new context value
    err = iClient->PublishContext ( *iContextA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );

    // Configure query to match published context and request context
    query = CCFContextQuery::NewLC ( iContextA->Source ( ), iContextA->Type ( ) );
    RContextObjectArray array;
    CleanupResetAndDestroyPushL ( array );
    err = iClient->RequestContext ( *query, array );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect RequestContext return value!" );
    EUNIT_ASSERT_DESC( array.Count() == 1, "Incorrect result array count!" );
    EUNIT_ASSERT_DESC( array[0]->Source() == iContextA->Source(), "Incorrect context source!" );
    EUNIT_ASSERT_DESC( array[0]->Type() == iContextA->Type(), "Incorrect context type!" );

    // Request same context again
    array.ResetAndDestroy ( );
    err = iClient->RequestContext ( *query, array );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect RequestContext return value!" );
    EUNIT_ASSERT_DESC( array.Count() == 1, "Incorrect result array count!" );
    EUNIT_ASSERT_DESC( array[0]->Source() == iContextA->Source(), "Incorrect context source!" );
    EUNIT_ASSERT_DESC( array[0]->Type() == iContextA->Type(), "Incorrect context type!" );

    // Define a context
    ConfigureContextL ( *iContextA );
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    // Publish new context value
    err = iClient->PublishContext ( *iContextA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );

    // Configure query to match published context and request context
    query->SetSourceL ( iContextA->Source ( ) );
    query->SetTypeL ( iContextA->Type ( ) );
    array.ResetAndDestroy ( );
    err = iClient->RequestContext ( *query, array );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect RequestContext return value!" );
    EUNIT_ASSERT_DESC( array.Count() == 1, "Incorrect result array count!" );
    EUNIT_ASSERT_DESC( array[0]->Source() == iContextA->Source(), "Incorrect context source!" );
    EUNIT_ASSERT_DESC( array[0]->Type() == iContextA->Type(), "Incorrect context type!" );

    // Clean up
    CleanupStack::PopAndDestroy ( &array );
    CleanupStack::PopAndDestroy ( query );
    }

void MT_CFClient::MT_CCFClient_RequestContext2L( )
    {
    CCFContextQuery* query=  NULL;
    TInt err = KErrNone;

    // Define a context where we have no access
    ConfigureContextL ( *iContextA );
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KFailSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    // Configure query to match the defined context and request context
    query = CCFContextQuery::NewLC ( iContextA->Source ( ), iContextA->Type ( ) );
    RContextObjectArray array;
    CleanupResetAndDestroyPushL ( array );
    err = iClient->RequestContext ( *query, array );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect RequestContext return value!" );
    EUNIT_ASSERT_DESC( array.Count() == 0, "Incorrect result array count!" );

    // Define a context where we have no access
    ConfigureContextL ( *iContextA );
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KFailSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    // Configure query to match the defined context and request context
    query->SetSourceL ( iContextA->Source ( ) );
    query->SetTypeL ( iContextA->Type ( ) );
    array.ResetAndDestroy ( );
    err = iClient->RequestContext ( *query, array );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect RequestContext return value!" );
    EUNIT_ASSERT_DESC( array.Count() == 0, "Incorrect result array count!" );

    // Clean up
    CleanupStack::PopAndDestroy ( &array );
    CleanupStack::PopAndDestroy ( query );
    }

void MT_CFClient::MT_CCFClient_RequestContext3L( )
    {
    CCFContextQuery* query=  NULL;
    TInt err = KErrNone;

    // Define a contextA
    ConfigureContextL ( *iContextA );
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    // Publish new contextA value
    err = iClient->PublishContext ( *iContextA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );

    // Define a contextB
    iContextB->CopyL ( *iContextA );
    UpdateContextTypeL ( *iContextB );
    UpdateContextValueL ( *iContextB );
    err = iClient->DefineContext ( iContextB->Source ( ), iContextB->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    // Publish new contextB value
    err = iClient->PublishContext ( *iContextB );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );

    // Define a contextC
    iContextC->CopyL ( *iContextA );
    UpdateContextTypeL ( *iContextC );
    err = iClient->DefineContext ( iContextC->Source ( ), iContextC->Type ( ),
        KFailSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    // Configure query to match the defined contexts and request contexts
    query = CCFContextQuery::NewLC ( iContextA->Source ( ),
        KContextTypeFormatWild );
    query->SetSubTypeMatch ( ETrue );
    RContextObjectArray array;
    CleanupResetAndDestroyPushL ( array );
    err = iClient->RequestContext ( *query, array );
    TInt count = array.Count ( );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect RequestContext return value!" );
    EUNIT_ASSERT_DESC( count == 2, "Incorrect result array count!" );
    CCFContextObject* context=  NULL;
    for (TInt i = 0; i < count; i++ )
        {
        context = array[i];
        EUNIT_ASSERT_DESC( context->Source() == iContextA->Source() ||
            context->Source() == iContextB->Source(),
            "Incorrect context source!" );
        EUNIT_ASSERT_DESC( context->Type() == iContextA->Type() ||
            context->Type() == iContextB->Type(),
            "Incorrect context type!" );
        EUNIT_ASSERT_DESC( context->Type() != iContextC->Type(),
            "Incorrect context type!" );
        }

    // Clean up
    CleanupStack::PopAndDestroy ( &array );
    CleanupStack::PopAndDestroy ( query );
    }

void MT_CFClient::MT_CCFClient_RequestContextSetL( )
    {
    CCFContextQuery* query=  NULL;
    TInt err = KErrNone;

    // Define a contextA
    ConfigureContextL ( *iContextA );
    err = iClient->DefineContext ( iContextA->Source ( ), iContextA->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    // Publish new contextA value
    err = iClient->PublishContext ( *iContextA );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );

    // Define a contextB
    iContextB->CopyL ( *iContextA );
    UpdateContextTypeL ( *iContextB );
    UpdateContextValueL ( *iContextB );
    err = iClient->DefineContext ( iContextB->Source ( ), iContextB->Type ( ),
        KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    // Publish new contextB value
    err = iClient->PublishContext ( *iContextB );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );

    // Define a contextC
    iContextC->CopyL ( *iContextA );
    UpdateContextTypeL ( *iContextC );
    err = iClient->DefineContext ( iContextC->Source ( ), iContextC->Type ( ),
        KFailSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineContext return value!" );

    // Configure three queries to match the defined contexts and request contexts
    RContextQueryArray queryArray;
    CleanupResetAndDestroyPushL ( queryArray );
    query = CCFContextQuery::NewLC ( iContextA->Source ( ), iContextA->Type ( ) );
    queryArray.AppendL ( query );
    CleanupStack::Pop ( query );

    query = CCFContextQuery::NewLC ( iContextB->Source ( ), iContextB->Type ( ) );
    queryArray.AppendL ( query );
    CleanupStack::Pop ( query );

    query = CCFContextQuery::NewLC ( iContextC->Source ( ), iContextC->Type ( ) );
    queryArray.AppendL ( query );
    CleanupStack::Pop ( query );

    // Request context and check results
    RContextObjectArray resultArray;
    CleanupResetAndDestroyPushL ( resultArray );
    err = iClient->RequestContextSet ( queryArray, resultArray );
    TInt count = resultArray.Count ( );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect RequestContext return value!" );
    EUNIT_ASSERT_DESC( count == 2, "Incorrect result array count!" );
    CCFContextObject* context=  NULL;
    for (TInt i = 0; i < count; i++ )
        {
        context = resultArray[i];
        EUNIT_ASSERT_DESC( context->Source() == iContextA->Source() ||
            context->Source() == iContextB->Source(),
            "Incorrect context source!" );
        EUNIT_ASSERT_DESC( context->Type() == iContextA->Type() ||
            context->Type() == iContextB->Type(),
            "Incorrect context type!" );
        EUNIT_ASSERT_DESC( context->Type() != iContextC->Type(),
            "Incorrect context type!" );
        }

    // Request same contexts again and check results
    resultArray.ResetAndDestroy ( );
    err = iClient->RequestContextSet ( queryArray, resultArray );
    count = resultArray.Count ( );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect RequestContext return value!" );
    EUNIT_ASSERT_DESC( count == 2, "Incorrect result array count!" );
    context = NULL;
    for (TInt i = 0; i < count; i++ )
        {
        context = resultArray[i];
        EUNIT_ASSERT_DESC( context->Source() == iContextA->Source() ||
            context->Source() == iContextB->Source(),
            "Incorrect context source!" );
        EUNIT_ASSERT_DESC( context->Type() == iContextA->Type() ||
            context->Type() == iContextB->Type(),
            "Incorrect context type!" );
        EUNIT_ASSERT_DESC( context->Type() != iContextC->Type(),
            "Incorrect context type!" );
        }

    // Clean up
    CleanupStack::PopAndDestroy ( &resultArray );
    CleanupStack::PopAndDestroy ( &queryArray );
    }

void MT_CFClient::MT_CCFClient_RequestContextCombinedL( )
    {
    MT_CCFClient_RequestContext1L ( );
    MT_CCFClient_RequestContext2L ( );
    MT_CCFClient_RequestContext3L ( );
    MT_CCFClient_RequestContextSetL ( );
    }

void MT_CFClient::MT_CCFClient_DefineActionL( )
    {
    TInt err = KErrNone;
    HBufC* actionId=  NULL;

    // Define action with access
    actionId = ConfigureActionIdLC ( );
    err = iClient->DefineAction ( *actionId, KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineAction return value!" );
    CleanupStack::PopAndDestroy ( actionId );

    // Define another action with access
    actionId = ConfigureActionIdLC ( );
    err = iClient->DefineAction ( *actionId, KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineAction return value!" );

    // Try to define already defined action
    err = iClient->DefineAction ( *actionId, KPassSec );
    EUNIT_ASSERT_DESC( err == KErrAlreadyExists, "Incorrect DefineAction return value!" );

    // Delete client, re-instantiate and define the same action than above
    delete iClient;
    iClient = NULL;
    iClient = CCFClient::NewL( *this );
    err = iClient->DefineAction ( *actionId, KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineAction return value!" );
    CleanupStack::PopAndDestroy ( actionId );

    // Define action with no access
    Wait ( KConfigDelay );
    actionId = ConfigureActionIdLC ( );
    err = iClient->DefineAction ( *actionId, KFailSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineAction return value!" );
    CleanupStack::PopAndDestroy ( actionId );

    // Try to define reserved action ids
    err = iClient->DefineAction ( KReservedActionPublishContext, KPassSec );
    EUNIT_ASSERT_DESC( err == KErrAlreadyExists, "Incorrect DefineAction return value!" );
    err = iClient->DefineAction ( KReservedActionPublishContext2, KPassSec );
    EUNIT_ASSERT_DESC( err == KErrAlreadyExists, "Incorrect DefineAction return value!" );
    err = iClient->DefineAction ( KReservedActionSourceCommand, KPassSec );
    EUNIT_ASSERT_DESC( err == KErrAlreadyExists, "Incorrect DefineAction return value!" );
    err = iClient->DefineAction ( KReservedActionSourceCommand2, KPassSec );
    EUNIT_ASSERT_DESC( err == KErrAlreadyExists, "Incorrect DefineAction return value!" );
    }

void MT_CFClient::MT_CCFClient_SubscribeAction1L( )
    {
    TInt err = KErrNone;
    HBufC* actionId=  NULL;
    CCFActionSubscription* subscription = CCFActionSubscription::NewLC ( );

    // Define action with access and subscribe it
    actionId = ConfigureActionIdLC ( );
    err = iClient->DefineAction ( *actionId, KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineAction return value!" );

    subscription->SetActionIdentifierL ( *actionId );
    err = iClient->SubscribeAction ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeAction return value!" );

    // Delete client, re-define action and subscribe it
    delete iClient;
    iClient = NULL;
    iClient = CCFClient::NewL( *this );

    err = iClient->DefineAction ( *actionId, KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineAction return value!" );

    subscription->SetActionIdentifierL ( *actionId );
    err = iClient->SubscribeAction ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeAction return value!" );
    CleanupStack::PopAndDestroy ( actionId );

    // Define action with no access and subscribe it
    Wait ( KConfigDelay );
    actionId = ConfigureActionIdLC ( );
    err = iClient->DefineAction ( *actionId, KFailSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect DefineAction return value!" );

    subscription->SetActionIdentifierL ( *actionId );
    err = iClient->SubscribeAction ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrAccessDenied, "Incorrect SubscribeAction return value!" );
    CleanupStack::PopAndDestroy ( actionId );

    // Try to subscribe an action which has not been defined
    Wait ( KConfigDelay );
    actionId = ConfigureActionIdLC ( );
    subscription->SetActionIdentifierL ( *actionId );
    err = iClient->SubscribeAction ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNotFound, "Incorrect SubscribeAction return value!" );
    CleanupStack::PopAndDestroy ( actionId );

    // Clean up
    CleanupStack::PopAndDestroy ( subscription );
    }

void MT_CFClient::MT_CCFClient_SubscribeAction2L( )
    {
    TInt err = KErrNone;
    CCFActionSubscription* subscription = CCFActionSubscription::NewLC ( );
    CCFContextObject* context = CCFContextObject::NewLC ( );

    // Run following set twice to get regression
    static const TInt KCount = 2;
    for (TInt i = 0; i < KCount; i++ )
        {
        // Define actions used in test-cfclient.rul
        iClient->DefineAction ( KActionA, KPassSec );
        iClient->DefineAction ( KActionB, KPassSec );

        // Define contexts used in test-cfclient.rul
        iClient->DefineContext ( KContextSourceA, KContextTypeA, KPassSec );

        // Subscribe both actions
        subscription->SetActionIdentifierL ( KActionA );
        err = iClient->SubscribeAction ( *subscription );
        EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeAction return value!" );

        subscription->SetActionIdentifierL ( KActionB );
        err = iClient->SubscribeAction ( *subscription );
        EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeAction return value!" );

        // Publish context [SourceA: TypeA: ValueA] and wait for action indication
        context->SetSourceL ( KContextSourceA );
        context->SetTypeL ( KContextTypeA );
        context->SetValueL ( KContextValueA );
        err = iClient->PublishContext ( *context );
        EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
        iCheckA = ETrue;
        iWait->Start ( );

        // Publish context [SourceA: TypeA: ValueB] and wait for action indications
        context->SetSourceL ( KContextSourceA );
        context->SetTypeL ( KContextTypeA );
        context->SetValueL ( KContextValueB );
        err = iClient->PublishContext ( *context );
        EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
        iCheckA = ETrue;
        iCheckB = ETrue;
        iWait->Start ( );

        // Publish context [SourceA: TypeA: ValueA] and wait for action indication
        context->SetSourceL ( KContextSourceA );
        context->SetTypeL ( KContextTypeA );
        context->SetValueL ( KContextValueA );
        err = iClient->PublishContext ( *context );
        EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
        iCheckA = ETrue;
        iWait->Start ( );

        // Publish context [SourceA: TypeA: ValueB] and wait for action indications
        context->SetSourceL ( KContextSourceA );
        context->SetTypeL ( KContextTypeA );
        context->SetValueL ( KContextValueB );
        err = iClient->PublishContext ( *context );
        EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
        iCheckA = ETrue;
        iCheckB = ETrue;
        iWait->Start ( );

        // Publish context [SourceA: TypeA: ValueA] and wait for action indication
        context->SetSourceL ( KContextSourceA );
        context->SetTypeL ( KContextTypeA );
        context->SetValueL ( KContextValueA );
        err = iClient->PublishContext ( *context );
        EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
        iCheckA = ETrue;
        iDeleteClient = ETrue;
        iWait->Start ( );
        EUNIT_ASSERT_DESC( !iClient, "Client instance not deleted!" );
        iClient = CCFClient::NewL ( *this );
        }

    // Clean up
    CleanupStack::PopAndDestroy ( context );
    CleanupStack::PopAndDestroy ( subscription );
    }

void MT_CFClient::MT_CCFClient_SubscribeAction3L( )
    {
    // x. Preconditions
    //--------------------------------------------------------------------------
    TInt err = KErrNone;
    CCFActionSubscription* subscription = CCFActionSubscription::NewLC( );
    CCFContextObject* context = CCFContextObject::NewLC( );

    // Define contexts used in test-cfclient.rul
    iClient->DefineContext( KContextSourceA, KContextTypeA, KPassSec );

    // Set context to "incorrect" state
    context->SetSourceL( KContextSourceA );
    context->SetTypeL( KContextTypeA );
    context->SetValueL( KContextValueB );
    err = iClient->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );

    // x. Define action a and subscribe it, delete client and publish context
    //--------------------------------------------------------------------------
    err = iClient->DefineAction ( KActionA, KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define action failed" );

    subscription->SetActionIdentifierL( KActionA );
    err = iClient->SubscribeAction( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Subscribe action failed" );

    // Check that the action is indicatated
    context->SetSourceL( KContextSourceA );
    context->SetTypeL( KContextTypeA );
    context->SetValueL( KContextValueA );
    err = iClient->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckA = ETrue;
    iWait->Start();

    // Reset context value
    context->SetSourceL( KContextSourceA );
    context->SetTypeL( KContextTypeA );
    context->SetValueL( KContextValueB );
    err = iClient->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );

    // Delete client and publish context
    delete iClient;
    iClient = NULL;
    iClient = CCFClient::NewL( *this );
    context->SetSourceL( KContextSourceA );
    context->SetTypeL( KContextTypeA );
    context->SetValueL( KContextValueA );
    err = iClient->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckA = ETrue;
    Wait( KSecond );
    EUNIT_ASSERT_DESC( iCheckA, "Something went terribly wrong" );

    // Reset context value
    context->SetSourceL( KContextSourceA );
    context->SetTypeL( KContextTypeA );
    context->SetValueL( KContextValueB );
    err = iClient->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );

    // x. Re-define action a and subscribe it
    //--------------------------------------------------------------------------
    err = iClient->DefineAction ( KActionA, KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define action failed" );

    subscription->SetActionIdentifierL( KActionA );
    err = iClient->SubscribeAction( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Subscribe action failed" );

    // Check that the action is indicatated
    context->SetSourceL( KContextSourceA );
    context->SetTypeL( KContextTypeA );
    context->SetValueL( KContextValueA );
    err = iClient->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckA = ETrue;
    iWait->Start();

    // x. Clean up
    //--------------------------------------------------------------------------
    CleanupStack::PopAndDestroy( context );
    CleanupStack::PopAndDestroy( subscription );
    }

void MT_CFClient::MT_CCFClient_SubscribeAction4L( )
    {
    // x. Pre-conditions
    // -------------------------------------------------------------------------
    TInt err = KErrNone;
    CCFActionSubscription* subscription = CCFActionSubscription::NewLC( );
    CCFContextObject* context = CCFContextObject::NewLC( );

    // Define contexts used in test-cfclient.rul
    iClient->DefineContext( KContextSourceA, KContextTypeA, KPassSec );

    // Set context to "incorrect" state
    context->SetSourceL( KContextSourceA );
    context->SetTypeL( KContextTypeA );
    context->SetValueL( KContextValueB );
    err = iClient->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );

    // x. Define action
    // -------------------------------------------------------------------------
    err = iClient->DefineAction ( KActionA, KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define action failed" );

    // x. Delete client and re-initialize
    // -------------------------------------------------------------------------
    delete iClient;
    iClient = NULL;
    iClient = CCFClient::NewL( *this );

    // x. Subscribe action
    // -------------------------------------------------------------------------
    subscription->SetActionIdentifierL( KActionA );
    err = iClient->SubscribeAction( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Subscribe action failed" );

    // x. Publish context and wait for action indication
    // -------------------------------------------------------------------------
    context->SetSourceL( KContextSourceA );
    context->SetTypeL( KContextTypeA );
    context->SetValueL( KContextValueA );
    err = iClient->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );

    // Wait for action indication
    iCheckA = ETrue;
    iWait->Start();

    // x. Clean up
    // -------------------------------------------------------------------------
    CleanupStack::PopAndDestroy( context );
    CleanupStack::PopAndDestroy( subscription );
    }

void MT_CFClient::MT_CCFClient_SubscribeActionFromDifferentClientsL( )
    {
    TInt err = KErrNone;
    CCFActionSubscription* subscription = CCFActionSubscription::NewLC ( );
    CCFContextObject* context = CCFContextObject::NewLC ( );

    // Define actions used in test-cfclient.rul
    iClient->DefineAction ( KActionA, KPassSec );
    iClient->DefineAction ( KActionB, KPassSec );

    // Define contexts used in test-cfclient.rul
    iClient->DefineContext ( KContextSourceA, KContextTypeA, KPassSec );

    // Subscribe both actions but from different clients
    subscription->SetActionIdentifierL ( KActionA );
    err = iClient->SubscribeAction ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeAction return value!" );

    subscription->SetActionIdentifierL ( KActionB );
    err = iAnotherClient->SubscribeAction ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeAction return value!" );

    // Publish context [SourceA: TypeA: ValueA] and wait for action indication
    context->SetSourceL ( KContextSourceA );
    context->SetTypeL ( KContextTypeA );
    context->SetValueL ( KContextValueA );
    err = iClient->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    iWait->Start ( );

    // Publish context [SourceA: TypeA: ValueB] and wait for action indications
    context->SetSourceL ( KContextSourceA );
    context->SetTypeL ( KContextTypeA );
    context->SetValueL ( KContextValueB );
    err = iAnotherClient->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    iCheckB = ETrue;
    iWait->Start ( );

    // Clean up
    CleanupStack::PopAndDestroy ( context );
    CleanupStack::PopAndDestroy ( subscription );
    }

void MT_CFClient::MT_CCFClient_SubscribeActionFromDifferentClients2L( )
    {
    TInt err = KErrNone;
    CCFActionSubscription* subscription = CCFActionSubscription::NewLC ( );
    CCFContextObject* context = CCFContextObject::NewLC ( );

    // Define action used in test-cfclient.rul
    iClient->DefineAction ( KActionA, KPassSec );

    // Define contexts used in test-cfclient.rul
    iClient->DefineContext ( KContextSourceA, KContextTypeA, KPassSec );

    // Subscribe same action from different clients
    subscription->SetActionIdentifierL ( KActionA );
    err = iClient->SubscribeAction ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeAction return value!" );

    err = iAnotherClient->SubscribeAction ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeAction return value!" );

    iCheckB = EFalse;
    iCountActionAIndications = ETrue;
    iExpectedActionAIndicationCount = 2;
    // Publish context [SourceA: TypeA: ValueA] and wait for action indications
    context->SetSourceL ( KContextSourceA );
    context->SetTypeL ( KContextTypeA );
    context->SetValueL ( KContextValueA );
    err = iClient->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    iWait->Start ( );
    EUNIT_ASSERT_DESC( iExpectedActionAIndicationCount == 0, "Unexpected amount of action indications!" );
    iCountActionAIndications = EFalse;

    // Clean up
    CleanupStack::PopAndDestroy ( context );
    CleanupStack::PopAndDestroy ( subscription );
    }

void MT_CFClient::MT_CCFClient_SubscribeActionCombinedL( )
    {
    MT_CCFClient_SubscribeAction1L ( );
    Wait( KSecond );
    delete iClient;
    iClient = NULL;
    iClient = CCFClient::NewL( *this );

    MT_CCFClient_SubscribeAction2L ( );
    Wait( KSecond );
    delete iClient;
    iClient = NULL;
    iClient = CCFClient::NewL( *this );

    MT_CCFClient_SubscribeAction3L ( );
    Wait( KSecond );
    delete iClient;
    iClient = NULL;
    iClient = CCFClient::NewL( *this );

    MT_CCFClient_SubscribeAction4L ( );
    }

void MT_CFClient::MT_CCFClient_UnsubscribeActionL( )
    {
    TInt err = KErrNone;
    CCFActionSubscription* subscription = CCFActionSubscription::NewLC ( );
    CCFContextObject* context = CCFContextObject::NewLC ( );

    // Define actions used in test-cfclient.rul
    iClient->DefineAction ( KActionA, KPassSec );
    iClient->DefineAction ( KActionB, KPassSec );

    // Define contexts used in test-cfclient.rul
    iClient->DefineContext ( KContextSourceA, KContextTypeA, KPassSec );

    // Subscribe both actions
    subscription->SetActionIdentifierL ( KActionA );
    err = iClient->SubscribeAction ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeAction return value!" );

    subscription->SetActionIdentifierL ( KActionB );
    err = iClient->SubscribeAction ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeAction return value!" );

    // Publish context [SourceA: TypeA: ValueA] and wait for action indication
    context->SetSourceL ( KContextSourceA );
    context->SetTypeL ( KContextTypeA );
    context->SetValueL ( KContextValueA );
    err = iClient->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    iWait->Start ( );

    // Publish context [SourceA: TypeA: ValueB] and wait for action indications
    context->SetSourceL ( KContextSourceA );
    context->SetTypeL ( KContextTypeA );
    context->SetValueL ( KContextValueB );
    err = iClient->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    iCheckB = ETrue;
    iWait->Start ( );

    // Unsubscribe actionA
    subscription->SetActionIdentifierL ( KActionA );
    err = iClient->UnsubscribeAction ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect UnsubscribeAction return value!" );

    // Publish context [SourceA: TypeA: ValueA], should not receive action indication
    context->SetSourceL ( KContextSourceA );
    context->SetTypeL ( KContextTypeA );
    context->SetValueL ( KContextValueA );
    err = iClient->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    Wait ( KDelay );
    EUNIT_ASSERT_DESC( iCheckA, "Action indication received!" );
    iCheckA = EFalse;

    // Publish context [SourceA: TypeA: ValueB] and wait for action indication
    context->SetSourceL ( KContextSourceA );
    context->SetTypeL ( KContextTypeA );
    context->SetValueL ( KContextValueB );
    err = iClient->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckB = ETrue;
    iWait->Start ( );

    // Unsubscribe actionB
    subscription->SetActionIdentifierL ( KActionB );
    err = iClient->UnsubscribeAction ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect UnsubscribeAction return value!" );

    // Publish context [SourceA: TypeA: ValueB], should not reveice action indication
    context->SetSourceL ( KContextSourceA );
    context->SetTypeL ( KContextTypeA );
    context->SetValueL ( KContextValueB );
    err = iClient->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckB = ETrue;
    Wait ( KDelay );
    EUNIT_ASSERT_DESC( iCheckB, "Action indication received!" );
    iCheckB = EFalse;

    // subscribe actionA again
    subscription->SetActionIdentifierL ( KActionA );
    err = iClient->SubscribeAction ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeAction return value!" );

    // Publish context [SourceA: TypeA: ValueA] and wait for action indication
    context->SetSourceL ( KContextSourceA );
    context->SetTypeL ( KContextTypeA );
    context->SetValueL ( KContextValueA );
    err = iClient->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    iWait->Start ( );

    // subscribe actionB again
    subscription->SetActionIdentifierL ( KActionB );
    err = iClient->SubscribeAction ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeAction return value!" );

    // Publish context [SourceA: TypeA: ValueB] and wait for action indications
    context->SetSourceL ( KContextSourceA );
    context->SetTypeL ( KContextTypeA );
    context->SetValueL ( KContextValueB );
    err = iClient->PublishContext ( *context );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect PublishContext return value!" );
    iCheckA = ETrue;
    iCheckB = ETrue;
    iWait->Start ( );

    // Clean up
    CleanupStack::PopAndDestroy ( context );
    CleanupStack::PopAndDestroy ( subscription );
    }

void MT_CFClient::MT_CCFClient_RegisterScriptL( )
    {
    TInt err;

    // Define context
    CCFContextObject* co = CCFContextObject::NewLC ( );
    co->SetSourceL (_L("RegisterScriptSource") );
    co->SetTypeL (_L("RegisterScriptType") );
    err = iClient->DefineContext ( co->Source ( ), co->Type ( ), KPassSec );
    //EUNIT_ASSERT( err == KErrNone );

    // define context with no access
    err = iClient->DefineContext ( _L("RegisterScriptSource"),
    _L("RegisterScriptTypeNoAccess"), KFailSec );
    //EUNIT_ASSERT( err == KErrNone );

    // Define action
    iClient->DefineAction ( _L("RegisterScriptAction"), KPassSec );

    // Define action with no access
    iClient->DefineAction ( _L("RegisterScriptActionNoAccess"), KFailSec );

    TInt scriptId;

    // 1. registering script fails because no context defined
    err = iClient->RegisterScript (
    _L("z:\\data\\cftestdata\\testscripts\\test-registerscript-nocontextdefined.xml"),
    scriptId );
    EUNIT_ASSERT_DESC( err == KErrNotFound, "No error when registering script!" );

    // 2. registering script fails because no action defined
    err = iClient->RegisterScript (
    _L("z:\\data\\cftestdata\\testscripts\\test-registerscript-noactiondefined.xml"),
    scriptId );
    EUNIT_ASSERT_DESC( err == KErrNotFound, "No error when registering script!" );

    // 3. registering script fails because no capabilities
    err = iClient->RegisterScript (
    _L("z:\\data\\cftestdata\\testscripts\\test-registerscript-nocontextaccess.xml"),
    scriptId );
    EUNIT_ASSERT_DESC( err == KErrPermissionDenied, "No error when registering script!" );

    // 4. registering script fails because no action defined
    err = iClient->RegisterScript (
    _L("z:\\data\\cftestdata\\testscripts\\test-registerscript-noactionaccess.xml"),
    scriptId );
    EUNIT_ASSERT_DESC( err == KErrPermissionDenied, "No error when registering script!" );

    // 5. registering script succeeds
    err = iClient->RegisterScript (
    _L("z:\\data\\cftestdata\\testscripts\\test-registerscript.xml"),
    scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when registering script!" );

    // Subscribe both actions
    CCFActionSubscription* subscription = CCFActionSubscription::NewLC ( );
    subscription->SetActionIdentifierL ( _L("RegisterScriptAction") );
    err = iClient->SubscribeAction ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone,
        "Incorrect SubscribeAction return value!" );

    // 4. test that we will get action indications
    co->SetValueL (_L("RegisterScriptValue") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone,
        "Incorrect PublishContext return value!" );

    iCheckRegisterScriptAction = ETrue;
    iWait->Start ( );

    CleanupStack::PopAndDestroy ( subscription );
    CleanupStack::PopAndDestroy ( co );

    // 6. Deregister script

    err = iClient->DeregisterScript ( scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Deregistering script caused error!" );

    }

void MT_CFClient::MT_CCFClient_RegisterScriptDesL( )
    {
    TInt err;

    // Define context
    CCFContextObject* co = CCFContextObject::NewLC ( );
    co->SetSourceL (_L("RegisterScriptSource") );
    co->SetTypeL (_L("RegisterScriptType") );
    err = iClient->DefineContext ( co->Source ( ), co->Type ( ), KPassSec );
    //EUNIT_ASSERT( err == KErrNone );

    // define context with no access
    err = iClient->DefineContext ( _L("RegisterScriptSource"),
    _L("RegisterScriptTypeNoAccess"), KFailSec );
    //EUNIT_ASSERT( err == KErrNone );

    // Define action
    iClient->DefineAction ( _L("RegisterScriptAction"), KPassSec );

    // Define action with no access
    iClient->DefineAction ( _L("RegisterScriptActionNoAccess"), KFailSec );

    TInt scriptId;

    // 1. registering script fails because no context defined
    HBufC8* script = LoadScriptFromFile (_L("test-registerscript-nocontextdefined.xml") );
    err = iClient->RegisterScript (
    _L("test-registerscript-nocontextdefined.xml"), *script, scriptId );
    EUNIT_ASSERT_DESC( err == KErrNotFound, "No error when registering script!" );
    delete script;
    script = NULL;

    // 2. registering script fails because no action defined
    script = LoadScriptFromFile (_L("test-registerscript-noactiondefined.xml") );
    err = iClient->RegisterScript (
    _L("test-registerscript-noactiondefined.xml"), *script, scriptId );
    EUNIT_ASSERT_DESC( err == KErrNotFound, "No error when registering script!" );
    delete script;
    script = NULL;

    // 3. registering script fails because no capabilities
    script = LoadScriptFromFile (_L("test-registerscript-nocontextaccess.xml") );
    err = iClient->RegisterScript (
    _L("test-registerscript-nocontextaccess.xml"), *script, scriptId );
    EUNIT_ASSERT_DESC( err == KErrPermissionDenied, "No error when registering script!" );
    delete script;
    script = NULL;

    // 4. registering script fails because no action defined
    script = LoadScriptFromFile (_L("test-registerscript-noactionaccess.xml") );
    err = iClient->RegisterScript (
    _L("test-registerscript-noactionaccess.xml"), *script, scriptId );
    EUNIT_ASSERT_DESC( err == KErrPermissionDenied, "No error when registering script!" );
    delete script;
    script = NULL;

    // 5. registering script succeeds
    script = LoadScriptFromFile (_L("test-registerscript.xml") );
    err = iClient->RegisterScript (
    _L("test-registerscript.xml"), *script, scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when registering script!" );
    delete script;
    script = NULL;

    // Subscribe both actions
    CCFActionSubscription* subscription = CCFActionSubscription::NewLC ( );
    subscription->SetActionIdentifierL ( _L("RegisterScriptAction") );
    err = iClient->SubscribeAction ( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone,
        "Incorrect SubscribeAction return value!" );

    // 6. test that we will get action indications

    co->SetValueL (_L("RegisterScriptValue") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone,
        "Incorrect PublishContext return value!" );

    iCheckRegisterScriptAction = ETrue;
    iWait->Start ( );

    CleanupStack::PopAndDestroy ( subscription );
    CleanupStack::PopAndDestroy ( co );

    // 7. Deregister script

    err = iClient->DeregisterScript ( scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Deregistering script caused error!" );
    }

void MT_CFClient::MT_CCFClient_DeregisterScriptL( )
    {
    // 1. Try to remove script we did not register -> should fail
    TInt err = iClient->DeregisterScript ( 1 );
    EUNIT_ASSERT_DESC( err != KErrNone,
        "Succeeded to deregister script loaded by CF/registered by others" );
    }

void MT_CFClient::MT_CCFClient_SaveContextSourceSettingL( )
    {
    // 1. Install setting succeeds
    TInt err = iClient->SaveContextSourceSetting (
    _L("z:\\data\\cftestdata\\testsettings\\testcontextsourcesetting3.xml"),
    KTestContextSourcePluginImplUid );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when installing settings3!" );

    TBool
        fileExists = BaflUtils::FileExists (
            iFs,
            _L( "C:\\Private\\10282BC4\\Settings\\10002001\\20000fb1_testcontextsourcesetting3.xml" ) );
    EUNIT_ASSERT_DESC( fileExists, "Error, cannot find settings3 file after install!" );

    // 2. Uninstall setting succeeds
    err = iClient->DeleteContextSourceSetting (
    _L("testcontextsourcesetting3.xml"),
    KTestContextSourcePluginImplUid );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when uninstalling settings3!" );

    fileExists = BaflUtils::FileExists (
        iFs,
        _L( "C:\\Private\\10282BC4\\Settings\\10002001\\20000fb1_testcontextsourcesetting3.xml" ) );
    EUNIT_ASSERT_DESC( !fileExists, "Error, file settings3 still exists after uninstall!" );

    if ( iTestDelay )
        {
        iTestDelay->Start ( 100000 ); // 100 ms
        }
    }

void MT_CFClient::MT_CCFClient_SaveContextSourceSetting2L( )
    {
    // 1. Installing bogus setting file, fails
    TInt
        err = iClient->SaveContextSourceSetting (
        _L("z:\\data\\cftestdata\\testsettings\\testcontext_source_setting_4.xml"),
        KTestContextSourcePluginImplUid );
    EUNIT_ASSERT_DESC( err == KErrNotFound, "Error when installing settings_4!" );

    TBool
        fileExists = BaflUtils::FileExists (
            iFs,
            _L( "C:\\Private\\10282BC4\\Settings\\10002001\\20000fb1_testcontext_source_setting_4.xml" ) );
    EUNIT_ASSERT_DESC( !fileExists, "Error, settings_4 file exists after install!" );

    // 2. Installing existing setting file not allowed by the plugin, fails
    err = iClient->SaveContextSourceSetting (
    _L("z:\\data\\cftestdata\\testsettings\\testcontextsourcesetting4.xml"),
    KTestContextSourcePluginImplUid );
    EUNIT_ASSERT_DESC( err == KErrPermissionDenied, "Error when installing settings4!" );

    fileExists = BaflUtils::FileExists (
        iFs,
        _L( "C:\\Private\\10282BC4\\Settings\\10002001\\20000fb1_testcontextsourcesetting4.xml" ) );
    EUNIT_ASSERT_DESC( !fileExists, "Error, settings4 file exists after install!" );

    // 3. Installing existing setting file for bogus plugin, fails
    err = iClient->SaveContextSourceSetting (
    _L("z:\\data\\cftestdata\\testsettings\\testcontextsourcesetting4.xml"),
    KBogusContextSourcePluginImplUid );
    EUNIT_ASSERT_DESC( err == KErrBadHandle, "Error when installing settings4 for bogus source!" );

    // 4. Installing setting passing from CheckValidity but leaving from
    //    UpdateSettingsL because of invalid settings!
    //    This should not happen for real source plugins.
    err = iClient->SaveContextSourceSetting (
    _L("z:\\data\\cftestdata\\testsettings\\testcontextsourcesetting7.xml"),
    KTestContextSourcePluginImplUid );
    EUNIT_ASSERT_DESC( err == KErrGeneral, "Error when installing settings7!" );

    fileExists = BaflUtils::FileExists (
        iFs,
        _L( "C:\\Private\\10282BC4\\Settings\\10002001\\20000fb1_testcontextsourcesetting7.xml" ) );
    EUNIT_ASSERT_DESC( fileExists, "Error, cannot find settings7 file after install!" );

    // 5. Uninstall setting succeeds, must do this because setting file was
    //    stored by the above failing call.
    err = iClient->DeleteContextSourceSetting (
    _L("testcontextsourcesetting7.xml"),
    KTestContextSourcePluginImplUid );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when uninstalling settings3!" );

    fileExists = BaflUtils::FileExists (
        iFs,
        _L( "C:\\Private\\10282BC4\\Settings\\10002001\\20000fb1_testcontextsourcesetting7.xml" ) );
    EUNIT_ASSERT_DESC( !fileExists, "Error, file settings3 still exists after uninstall!" );

    if ( iTestDelay )
        {
        iTestDelay->Start ( 100000 ); // 100 ms
        }
    }

void MT_CFClient::MT_CCFClient_DeleteContextSourceSettingL( )
    {
    // 1. Uninstall bogus setting, fails
    TBool
        fileExists = BaflUtils::FileExists (
            iFs,
            _L( "C:\\Private\\10282BC4\\Settings\\10002001\\20000fb1_testcontext_source_setting3.xml" ) );
    EUNIT_ASSERT_DESC( !fileExists, "Error, file testcontext_source_setting3 exists!" );

    TInt err = iClient->DeleteContextSourceSetting (
    _L("testcontext_source_setting3.xml"),
    KTestContextSourcePluginImplUid );
    EUNIT_ASSERT_DESC( err == KErrNotFound, "Error when uninstalling bogus setting!" );

    // 2. Uninstall setting from bogus source, fails
    err = iClient->DeleteContextSourceSetting (
    _L("testcontextsourcesetting3.xml"),
    KBogusContextSourcePluginImplUid );
    EUNIT_ASSERT_DESC( err == KErrBadHandle, "Error when uninstalling setting from bogus source!" );

    if ( iTestDelay )
        {
        iTestDelay->Start ( 100000 ); // 100 ms
        }
    }

void MT_CFClient::MT_CCFClient_DeleteContextSourceSetting2L( )
    {
    // 1. Install setting succeeds
    TInt err = iClient->SaveContextSourceSetting (
    _L("z:\\data\\cftestdata\\testsettings\\testcontextsourcesetting3.xml"),
    KTestContextSourcePluginImplUid );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when installing settings3!" );

    TBool
        fileExists = BaflUtils::FileExists (
            iFs,
            _L( "C:\\Private\\10282BC4\\Settings\\10002001\\20000fb1_testcontextsourcesetting3.xml" ) );
    EUNIT_ASSERT_DESC( fileExists, "Error, cannot find settings3 file after install!" );

    // 2. Install 2nd setting succeeds
    err = iClient->SaveContextSourceSetting (
    _L("z:\\data\\cftestdata\\testsettings\\testcontextsourcesetting5.xml"),
    KTestContextSourcePluginImplUid );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when installing settings5!" );

    fileExists = BaflUtils::FileExists (
        iFs,
        _L( "C:\\Private\\10282BC4\\Settings\\10002001\\20000fb1_testcontextsourcesetting5.xml" ) );
    EUNIT_ASSERT_DESC( fileExists, "Error, cannot find settings5 file after install!" );

    // 3. Uninstall all client settings succeeds
    err = iClient->DeleteContextSourceSettings (KTestContextSourcePluginImplUid );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when uninstalling all client installed settings!" );

    fileExists = BaflUtils::FileExists (
        iFs,
        _L( "C:\\Private\\10282BC4\\Settings\\10002001\\20000fb1_testcontextsourcesetting3.xml" ) );
    EUNIT_ASSERT_DESC( !fileExists, "Error, file settings3 still exists after uninstall!" );

    fileExists = BaflUtils::FileExists (
        iFs,
        _L( "C:\\Private\\10282BC4\\Settings\\10002001\\20000fb1_testcontextsourcesetting5.xml" ) );
    EUNIT_ASSERT_DESC( !fileExists, "Error, file settings5 still exists after uninstall!" );

    // 4. Uninstall all client settings, no exists, fails
    err = iClient->DeleteContextSourceSettings (KTestContextSourcePluginImplUid );
    EUNIT_ASSERT_DESC( err == KErrNotFound, "Error when uninstalling all client installed settings!" );

    if ( iTestDelay )
        {
        iTestDelay->Start ( 100000 ); // 100 ms
        }
    }

void MT_CFClient::MT_CCFClient_UpgradeContextSourceSettingL( )
    {
    // 1. Install setting succeeds
    TInt err = iClient->SaveContextSourceSetting (
    _L("z:\\data\\cftestdata\\testsettings\\testcontextsourcesetting3.xml"),
    KTestContextSourcePluginImplUid );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when installing settings3!" );

    TBool
        fileExists = BaflUtils::FileExists (
            iFs,
            _L( "C:\\Private\\10282BC4\\Settings\\10002001\\20000fb1_testcontextsourcesetting3.xml" ) );
    EUNIT_ASSERT_DESC( fileExists, "Error, cannot find settings3 file after install!" );

    // 2. Upgrade setting succeeds
    err = iClient->SaveContextSourceSetting (
    _L("z:\\data\\cftestdata\\testsettings\\settingupgrade\\testcontextsourcesetting3.xml"),
    KTestContextSourcePluginImplUid );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when upgrading settings3!" );

    fileExists = BaflUtils::FileExists (
        iFs,
        _L( "C:\\Private\\10282BC4\\Settings\\10002001\\20000fb1_testcontextsourcesetting3.xml" ) );
    EUNIT_ASSERT_DESC( fileExists, "Error, cannot find settings3 file after install!" );

    // 3. Uninstall setting succeeds
    err = iClient->DeleteContextSourceSetting (
    _L("testcontextsourcesetting3.xml"),
    KTestContextSourcePluginImplUid );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when uninstalling settings3!" );

    fileExists = BaflUtils::FileExists (
        iFs,
        _L( "C:\\Private\\10282BC4\\Settings\\10002001\\20000fb1_testcontextsourcesetting3.xml" ) );
    EUNIT_ASSERT_DESC( !fileExists, "Error, file settings3 still exists after uninstall!" );

    if ( iTestDelay )
        {
        iTestDelay->Start ( 100000 ); // 100 ms
        }
    }

void MT_CFClient::MT_CCFClient_SaveScriptL( )
    {
    // Change timestamp to yesterday
    TTime currentTime;
    currentTime.UniversalTime();
    TDateTime dateTime = currentTime.DateTime();
    dateTime.SetHour( dateTime.Hour() - 1 );

    // Update new time
    TTime entryTime( dateTime );

    CCFContextObject* co = CCFContextObject::NewLC ( );
    CCFActionSubscription* actionSubscription = CCFActionSubscription::NewLC ( );

    // Setup preconditions
    // -------------------------------------------------------------------------
    co->SetSourceL ( _L("test") );
    co->SetTypeL (_L("save.script") );
    TInt err = iClient->DefineContext ( co->Source ( ), co->Type ( ), KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone || err == KErrAlreadyExists,
        "Define context failed" );

    co->SetSourceL ( _L("test") );
    co->SetTypeL (_L("save.script.2") );
    err = iClient->DefineContext ( co->Source ( ), co->Type ( ), KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone || err == KErrAlreadyExists,
        "Define context failed" );

    err = iClient->DefineAction ( _L("saveScript"), KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone || err == KErrAlreadyExists,
        "Define action failed" );

    actionSubscription->SetActionIdentifierL ( _L("saveScript") );
    err = iClient->SubscribeAction ( *actionSubscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Subscribe action failed" );

    // x. Save script with full path
    // -------------------------------------------------------------------------
    TInt scriptId = KErrNotFound;
    err = iClient->SaveScript (
    _L("z:\\data\\cftestdata\\testscripts\\test-saveScript.xml"),
    scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Save script failed" );
    EUNIT_ASSERT_DESC( scriptId > 0, "Script id incorrect" );
    TBool exists = BaflUtils::FileExists ( iFs,
    _L("c:\\private\\10282bc4\\rules\\20000fb1\\test-saveScript.rul") );
    EUNIT_ASSERT_DESC( exists, "Script not saved" );

    co->SetSourceL ( _L("test") );
    co->SetTypeL (_L("save.script") );
    co->SetValueL ( _L("trigger") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckCustomAction = ETrue;
    iCustomActionId = _L("saveScript" );
    CCFKeyValuePair* keyValue = CCFKeyValuePair::NewLC (
    _L("id"), _L("triggered") );
    iCustomActionParams.AppendL ( keyValue );
    CleanupStack::Pop ( keyValue );
    iWait->Start ( );

    // x. Save the same script and check that the time stamp changes
    // -------------------------------------------------------------------------
    err = iFs.SetModified(
        _L("c:\\private\\10282bc4\\rules\\20000fb1\\test-saveScript.rul"), entryTime );
    TEntry oldEntry;
    err = iFs.Entry (
        _L("c:\\private\\10282bc4\\rules\\20000fb1\\test-saveScript.rul"),
        oldEntry );
    EUNIT_ASSERT_DESC( err == KErrNone, "Entry get failed" );

    User::After( 100000 );
    err = iClient->SaveScript (
        _L("z:\\data\\cftestdata\\testscripts\\test-saveScript.xml"),
        scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Save script failed" );
    EUNIT_ASSERT_DESC( scriptId > 0, "Script id incorrect" );
    exists = BaflUtils::FileExists ( iFs,
        _L("c:\\private\\10282bc4\\rules\\20000fb1\\test-saveScript.rul") );
    EUNIT_ASSERT_DESC( exists, "Script not saved" );

    TEntry newEntry;
    err = iFs.Entry (
        _L("c:\\private\\10282bc4\\rules\\20000fb1\\test-saveScript.rul"),
        newEntry );
    EUNIT_ASSERT_DESC( err == KErrNone, "Entry get failed" );
    EUNIT_ASSERT_DESC( oldEntry.iModified < newEntry.iModified,
        "Script not updated" );

    co->SetSourceL ( _L("test") );
    co->SetTypeL (_L("save.script") );
    co->SetValueL ( _L("trigger") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckCustomAction = ETrue;
    iCustomActionId = _L("saveScript" );
    keyValue = CCFKeyValuePair::NewLC ( _L("id"), _L("triggered") );
    iCustomActionParams.AppendL ( keyValue );
    CleanupStack::Pop ( keyValue );
    iWait->Start ( );

    // x. Save a script update with the same name
    // -------------------------------------------------------------------------
    HBufC8* script = LoadScriptFromFile ( _L("test-saveScript2.xml") );
    User::LeaveIfNull ( script );
    CleanupStack::PushL ( script );

    err = iClient->SaveScript ( _L("test-saveScript"), *script, scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Save script failed" );

    co->SetSourceL ( _L("test") );
    co->SetTypeL (_L("save.script.2") );
    co->SetValueL ( _L("trigger") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckCustomAction = ETrue;
    iCustomActionId = _L("saveScript" );
    keyValue = CCFKeyValuePair::NewLC ( _L("id"), _L("updateTriggered") );
    iCustomActionParams.AppendL ( keyValue );
    CleanupStack::Pop ( keyValue );
    iWait->Start ( );

    CleanupStack::PopAndDestroy ( script );

    // x. Save a script update with the same name (regression)
    // -------------------------------------------------------------------------
    script = LoadScriptFromFile ( _L("test-saveScript.xml") );
    User::LeaveIfNull ( script );
    CleanupStack::PushL ( script );

    err = iClient->SaveScript ( _L("test-saveScript"), *script, scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Save script failed" );

    co->SetSourceL ( _L("test") );
    co->SetTypeL (_L("save.script") );
    co->SetValueL ( _L("trigger") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckCustomAction = ETrue;
    iCustomActionId = _L("saveScript" );
    keyValue = CCFKeyValuePair::NewLC ( _L("id"), _L("triggered") );
    iCustomActionParams.AppendL ( keyValue );
    CleanupStack::Pop ( keyValue );
    iWait->Start ( );

    CleanupStack::PopAndDestroy ( script );

    // x. Save a new script via full path
    // -------------------------------------------------------------------------
    scriptId = KErrNotFound;
    err = iClient->SaveScript (
        _L("z:\\data\\cftestdata\\testscripts\\test-saveScript2.xml"),
        scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Save script failed" );
    EUNIT_ASSERT_DESC( scriptId > 0, "Script id incorrect" );
    exists = BaflUtils::FileExists ( iFs,
        _L("c:\\private\\10282bc4\\rules\\20000fb1\\test-saveScript2.rul") );
    EUNIT_ASSERT_DESC( exists, "Script not saved" );

    co->SetSourceL ( _L("test") );
    co->SetTypeL (_L("save.script.2") );
    co->SetValueL ( _L("trigger") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckCustomAction = ETrue;
    iCustomActionId = _L("saveScript" );
    keyValue = CCFKeyValuePair::NewLC (
    _L("id"), _L("updateTriggered") );
    iCustomActionParams.AppendL ( keyValue );
    CleanupStack::Pop ( keyValue );
    iWait->Start ( );

    // x. Save script with incorrect parameters
    // -------------------------------------------------------------------------
    err = iClient->SaveScript ( KNullDesC, scriptId );
    EUNIT_ASSERT_DESC( err != KErrNone, "Save script failed" );
    err = iClient->SaveScript ( KNullDesC, KNullDesC8, scriptId );
    EUNIT_ASSERT_DESC( err != KErrNone, "Save script failed" );

    // x. Delete all scripts, register script and then try to save the script
    // -------------------------------------------------------------------------
    err = iClient->DeleteScripts();
    EUNIT_ASSERT_DESC( err == KErrNone, "Delete scripts failed" );
    exists = BaflUtils::FolderExists( iFs,
        _L("c:\\private\\10282bc4\\rules\\20000fb1\\") );
    EUNIT_ASSERT_DESC( !exists, "Delete scripts failed" );

    err = iClient->RegisterScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-saveScript.xml"),
        scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Register script failed" );
    EUNIT_ASSERT_DESC( scriptId > 0, "Register script failed" );

    TInt previousScriptId = scriptId;
    err = iClient->SaveScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-saveScript.xml"),
        scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Save script failed" );
    EUNIT_ASSERT_DESC( scriptId > 0, "Save script failed" );
    EUNIT_ASSERT_DESC( scriptId == previousScriptId, "Save script failed" );

    co->SetSourceL ( _L("test") );
    co->SetTypeL (_L("save.script") );
    co->SetValueL ( _L("trigger") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckCustomAction = ETrue;
    iCustomActionId = _L("saveScript" );
    keyValue = CCFKeyValuePair::NewLC ( _L("id"), _L("triggered") );
    iCustomActionParams.AppendL ( keyValue );
    CleanupStack::Pop ( keyValue );
    iWait->Start ( );

    // Clean up
    // -------------------------------------------------------------------------
    CleanupStack::PopAndDestroy ( actionSubscription );
    CleanupStack::PopAndDestroy ( co );
    }

void MT_CFClient::MT_CCFClient_DeleteScriptL( )
    {
    CCFContextObject* co = CCFContextObject::NewLC( );
    CCFActionSubscription* actionSubscription = CCFActionSubscription::NewLC( );

    // Setup preconditions
    // -------------------------------------------------------------------------
    co->SetSourceL ( _L("test") );
    co->SetTypeL (_L("save.script") );
    TInt err = iClient->DefineContext ( co->Source ( ), co->Type ( ), KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone || err == KErrAlreadyExists,
        "Define context failed" );

    co->SetSourceL ( _L("test") );
    co->SetTypeL (_L("save.script.2") );
    err = iClient->DefineContext ( co->Source ( ), co->Type ( ), KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone || err == KErrAlreadyExists,
        "Define context failed" );

    err = iClient->DefineAction ( _L("saveScript"), KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone || err == KErrAlreadyExists,
        "Define action failed" );

    actionSubscription->SetActionIdentifierL ( _L("saveScript") );
    err = iClient->SubscribeAction ( *actionSubscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Subscribe action failed" );

    TInt scriptId = KErrNotFound;
    err = iClient->SaveScript (
    _L("z:\\data\\cftestdata\\testscripts\\test-saveScript.xml"),
    scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Save script failed" );
    TBool exists = BaflUtils::FileExists ( iFs,
    _L("c:\\private\\10282bc4\\rules\\20000fb1\\test-saveScript.rul") );
    EUNIT_ASSERT_DESC( exists, "Script not saved" );

    err = iClient->SaveScript (
    _L("z:\\data\\cftestdata\\testscripts\\test-saveScript2.xml"),
    scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Save script failed" );
    exists = BaflUtils::FileExists ( iFs,
    _L("c:\\private\\10282bc4\\rules\\20000fb1\\test-saveScript2.rul") );
    EUNIT_ASSERT_DESC( exists, "Script not saved" );

    // x. Delete script and check that action is not indicated
    // -------------------------------------------------------------------------
    err = iClient->DeleteScript ( _L("test-saveScript") );
    EUNIT_ASSERT_DESC( err == KErrNone, "Delete script failed" );
    exists = BaflUtils::FileExists ( iFs,
    _L("c:\\private\\10282bc4\\rules\\20000fb1\\test-saveScript.rul") );
    EUNIT_ASSERT_DESC( !exists, "Script not deleted" );

    co->SetSourceL ( _L("test") );
    co->SetTypeL (_L("save.script") );
    co->SetValueL ( _L("trigger") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckCustomAction = ETrue;
    iCustomActionId = _L("saveScript" );
    CCFKeyValuePair* keyValue = CCFKeyValuePair::NewLC ( _L("id"), _L("triggered") );
    iCustomActionParams.AppendL ( keyValue );
    CleanupStack::Pop ( keyValue );
    Wait ( KDelay );
    EUNIT_ASSERT_DESC( iCheckCustomAction, "Delete script failed" );

    // x. Delete script and check that action is not indicated
    // -------------------------------------------------------------------------
    err = iClient->DeleteScript ( _L("test-saveScript2") );
    EUNIT_ASSERT_DESC( err == KErrNone, "Delete script failed" );
    exists = BaflUtils::FileExists ( iFs,
    _L("c:\\private\\10282bc4\\rules\\20000fb1\\test-saveScript2.rul") );
    EUNIT_ASSERT_DESC( !exists, "Script not deleted" );
    exists = BaflUtils::FolderExists ( iFs, _L("c:\\private\\10282bc4\\rules\\20000fb1\\") );
    EUNIT_ASSERT_DESC( !exists, "Delete script failed" );

    co->SetSourceL ( _L("test") );
    co->SetTypeL (_L("save.script.2") );
    co->SetValueL ( _L("trigger") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckCustomAction = ETrue;
    iCustomActionId = _L("saveScript" );
    keyValue = CCFKeyValuePair::NewLC ( _L("id"), _L("updateTriggered") );
    iCustomActionParams.AppendL ( keyValue );
    CleanupStack::Pop ( keyValue );
    Wait ( KDelay );
    EUNIT_ASSERT_DESC( iCheckCustomAction, "Delete script failed" );

    // x. Delete script after a saved script has been deregistered
    // -------------------------------------------------------------------------
    scriptId = KErrNotFound;
    err = iClient->SaveScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-saveScript.xml"),
        scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Save script failed" );
    exists = BaflUtils::FileExists ( iFs,
        _L("c:\\private\\10282bc4\\rules\\20000fb1\\test-saveScript.rul") );
    EUNIT_ASSERT_DESC( exists, "Script not saved" );

    err = iClient->DeregisterScript( scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Deregister script failed" );

    co->SetSourceL ( _L("test") );
    co->SetTypeL (_L("save.script") );
    co->SetValueL ( _L("trigger") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckCustomAction = ETrue;
    iCustomActionId = _L("saveScript" );
    keyValue = CCFKeyValuePair::NewLC( _L("id"), _L("triggered") );
    iCustomActionParams.AppendL ( keyValue );
    CleanupStack::Pop ( keyValue );
    Wait ( KDelay );
    EUNIT_ASSERT_DESC( iCheckCustomAction, "Deregister script failed" );

    err = iClient->DeleteScript ( _L("test-saveScript") );
    EUNIT_ASSERT_DESC( err == KErrNone, "Delete script failed" );
    exists = BaflUtils::FileExists ( iFs,
        _L("c:\\private\\10282bc4\\rules\\20000fb1\\test-saveScript.rul") );
    EUNIT_ASSERT_DESC( !exists, "Script not deleted" );

    // x. Delete with incorrect parameters
    // -------------------------------------------------------------------------
    err = iClient->DeleteScript ( KNullDesC );
    EUNIT_ASSERT_DESC( err == KErrArgument, "Delete script failed" );
    err = iClient->DeleteScript ( _L("test-saveScript") );
    EUNIT_ASSERT_DESC( err == KErrNotFound, "Delete script failed" );
    err = iClient->DeleteScript ( _L("test-saveScript2") );
    EUNIT_ASSERT_DESC( err == KErrNotFound, "Delete script failed" );
    err = iClient->DeleteScript ( _L("dummyScript") );
    EUNIT_ASSERT_DESC( err == KErrNotFound, "Delete script failed" );

    // Clean up
    // -------------------------------------------------------------------------
    CleanupStack::PopAndDestroy ( actionSubscription );
    CleanupStack::PopAndDestroy ( co );
    }

void MT_CFClient::MT_CCFClient_DeleteScriptsL( )
    {
    CCFContextObject* co = CCFContextObject::NewLC ( );
    CCFActionSubscription* actionSubscription = CCFActionSubscription::NewLC ( );

    // Setup preconditions
    // -------------------------------------------------------------------------
    co->SetSourceL ( _L("test") );
    co->SetTypeL (_L("save.script") );
    TInt err = iClient->DefineContext ( co->Source ( ), co->Type ( ), KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone || err == KErrAlreadyExists,
        "Define context failed" );

    co->SetSourceL ( _L("test") );
    co->SetTypeL (_L("save.script.2") );
    err = iClient->DefineContext ( co->Source ( ), co->Type ( ), KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone || err == KErrAlreadyExists,
        "Define context failed" );

    err = iClient->DefineAction ( _L("saveScript"), KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone || err == KErrAlreadyExists,
        "Define action failed" );

    actionSubscription->SetActionIdentifierL ( _L("saveScript") );
    err = iClient->SubscribeAction ( *actionSubscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Subscribe action failed" );

    TInt scriptId = KErrNotFound;
    err = iClient->SaveScript (
    _L("z:\\data\\cftestdata\\testscripts\\test-saveScript.xml"),
    scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Save script failed" );
    TBool exists = BaflUtils::FileExists ( iFs,
    _L("c:\\private\\10282bc4\\rules\\20000fb1\\test-saveScript.rul") );
    EUNIT_ASSERT_DESC( exists, "Script not saved" );

    err = iClient->SaveScript (
    _L("z:\\data\\cftestdata\\testscripts\\test-saveScript2.xml"),
    scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Save script failed" );
    exists = BaflUtils::FileExists ( iFs,
    _L("c:\\private\\10282bc4\\rules\\20000fb1\\test-saveScript2.rul") );
    EUNIT_ASSERT_DESC( exists, "Script not saved" );

    // x. Delete all scripts by uid
    // -------------------------------------------------------------------------
    err = iClient->DeleteScripts ( );
    EUNIT_ASSERT_DESC( err == KErrNone, "Delete scripts failed" );

    exists = BaflUtils::FileExists ( iFs,
    _L("c:\\private\\10282bc4\\rules\\20000fb1\\test-saveScript.rul") );
    EUNIT_ASSERT_DESC( !exists, "Script not deleted" );
    exists = BaflUtils::FileExists ( iFs,
    _L("c:\\private\\10282bc4\\rules\\20000fb1\\test-saveScript2.rul") );
    EUNIT_ASSERT_DESC( !exists, "Script not deleted" );
    exists = BaflUtils::FolderExists ( iFs, _L("c:\\private\\10282bc4\\rules\\20000fb1\\") );
    EUNIT_ASSERT_DESC( !exists, "Delete script failed" );

    co->SetSourceL ( _L("test") );
    co->SetTypeL (_L("save.script") );
    co->SetValueL ( _L("trigger") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckCustomAction = ETrue;
    iCustomActionId = _L("saveScript" );
    CCFKeyValuePair* keyValue = CCFKeyValuePair::NewLC ( _L("id"), _L("triggered") );
    iCustomActionParams.AppendL ( keyValue );
    CleanupStack::Pop ( keyValue );
    Wait ( KDelay );
    EUNIT_ASSERT_DESC( iCheckCustomAction, "Delete scripts failed" );

    co->SetSourceL ( _L("test") );
    co->SetTypeL (_L("save.script.2") );
    co->SetValueL ( _L("trigger") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckCustomAction = ETrue;
    iCustomActionId = _L("saveScript" );
    keyValue = CCFKeyValuePair::NewLC ( _L("id"), _L("updateTriggered") );
    iCustomActionParams.AppendL ( keyValue );
    CleanupStack::Pop ( keyValue );
    Wait ( KDelay );
    EUNIT_ASSERT_DESC( iCheckCustomAction, "Delete scripts failed" );

    // x. Try to delete all scripts if none found
    // -------------------------------------------------------------------------
    err = iClient->DeleteScripts ( );
    EUNIT_PRINT( _L("DeleteScrips: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNotFound, "Delete scripts failed" );

    // Clean up
    // -------------------------------------------------------------------------
    CleanupStack::PopAndDestroy ( actionSubscription );
    CleanupStack::PopAndDestroy ( co );
    }

void MT_CFClient::MT_CCFClient_UpgradeRomScriptL( )
    {
    CCFContextObject* co = CCFContextObject::NewLC ( );
    CCFActionSubscription* actionSubscription = CCFActionSubscription::NewLC ( );

    // Setup preconditions
    // -------------------------------------------------------------------------
    co->SetSourceL ( _L("test") );
    co->SetTypeL (_L("upgrade") );
    TInt err = iClient->DefineContext ( co->Source ( ), co->Type ( ), KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone || err == KErrAlreadyExists,
        "Define context failed" );

    err = iClient->DefineAction ( _L("romUpgrade"), KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone || err == KErrAlreadyExists,
        "Define action failed" );

    actionSubscription->SetActionIdentifierL ( _L("romUpgrade") );
    err = iClient->SubscribeAction ( *actionSubscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Subscribe action failed" );

    // x. Upgrade 'test-scriptUpgradeRom' script and check that the script
    //    really is upgraded
    // -------------------------------------------------------------------------

    // Upgrade with file name
    err = iClient->UpgradeRomScript (
    _L("z:\\data\\cftestdata\\testscripts\\test-scriptUpgradeRom.xml") );
    EUNIT_PRINT( _L("UpgradeRomScript: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Upgrade rom script failed" );

    co->SetValueL ( _L("true") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckCustomAction = ETrue;
    iCustomActionId = _L("romUpgrade" );
    CCFKeyValuePair* keyValue = CCFKeyValuePair::NewLC (
    _L("id"), _L("true") );
    iCustomActionParams.AppendL ( keyValue );
    CleanupStack::Pop ( keyValue );
    iWait->Start ( );

    // Restore original rom script
    err = iClient->RestoreRomScript ( _L("test-scriptUpgradeRom") );
    EUNIT_PRINT( _L("RestoreRomScript: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Restore script failed" );

    // Upgrade with file name and script
    HBufC8* script = LoadScriptFromFile ( _L("test-scriptUpgradeRom.xml") );
    EUNIT_ASSERT_DESC( script, "Script not loaded" );
    CleanupStack::PushL ( script );

    err = iClient->UpgradeRomScript ( _L("test-scriptUpgradeRom"), *script );
    EUNIT_ASSERT_DESC( err == KErrNone, "Upgrade rom script failed" );

    co->SetValueL ( _L("true") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckCustomAction = ETrue;
    iCustomActionId = _L("romUpgrade" );
    keyValue = CCFKeyValuePair::NewLC ( _L("id"), _L("true") );
    iCustomActionParams.AppendL ( keyValue );
    CleanupStack::Pop ( keyValue );
    iWait->Start ( );

    CleanupStack::PopAndDestroy ( script );

    // Restore original rom script
    err = iClient->RestoreRomScript ( _L("test-scriptUpgradeRom") );
    EUNIT_ASSERT_DESC( err == KErrNone, "Restore script failed" );

    co->SetValueL ( _L("false") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckCustomAction = ETrue;
    iCustomActionId = _L("romUpgrade" );
    keyValue = CCFKeyValuePair::NewLC ( _L("id"), _L("false") );
    iCustomActionParams.AppendL ( keyValue );
    CleanupStack::Pop ( keyValue );
    iWait->Start ( );

    // x. Upgrade 'test-scriptUpgradeRom' script and check that the upgrade
    //    fails with KErrAccessDenied (script is missing capability)
    // -------------------------------------------------------------------------
    script = LoadScriptFromFile ( _L("test-scriptUpgradeRomCapabilitiesMissing.xml") );
    EUNIT_ASSERT_DESC( script, "Script not loaded" );
    CleanupStack::PushL ( script );

    err = iClient->UpgradeRomScript ( _L("test-scriptUpgradeRom"), *script );
    EUNIT_ASSERT_DESC( err == KErrAccessDenied, "Upgrade rom script failed" );

    co->SetValueL ( _L("false") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckCustomAction = ETrue;
    iCustomActionId = _L("romUpgrade" );
    keyValue = CCFKeyValuePair::NewLC ( _L("id"), _L("false") );
    iCustomActionParams.AppendL ( keyValue );
    CleanupStack::Pop ( keyValue );
    iWait->Start ( );

    CleanupStack::PopAndDestroy( script );

    // x. Upgrade 'test-scriptUpgradeRom' script and check that the upgrade
    //    fails with KErrAccessDenied (script is missing upgrade security)
    // -------------------------------------------------------------------------
    script = LoadScriptFromFile ( _L("test-scriptUpgradeRomNoUpgradeSecurity.xml") );
    EUNIT_ASSERT_DESC( script, "Script not loaded" );
    CleanupStack::PushL ( script );

    err = iClient->UpgradeRomScript ( _L("test-scriptUpgradeRom"), *script );
    EUNIT_ASSERT_DESC( err == KErrAccessDenied, "Upgrade rom script failed" );

    co->SetValueL ( _L("false") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckCustomAction = ETrue;
    iCustomActionId = _L("romUpgrade" );
    keyValue = CCFKeyValuePair::NewLC ( _L("id"), _L("false") );
    iCustomActionParams.AppendL ( keyValue );
    CleanupStack::Pop ( keyValue );
    iWait->Start ( );

    CleanupStack::PopAndDestroy( script );

    // x. Upgrade 'test-scriptUpgradeRom' script and check that the upgrade
    //    fails with KErrAccessDenied (client is missing capabilities)
    // -------------------------------------------------------------------------
    script = LoadScriptFromFile ( _L("test-scriptUpgradeRom.xml") );
    EUNIT_ASSERT_DESC( script, "Script not loaded" );
    CleanupStack::PushL ( script );

    err = iClient->UpgradeRomScript (
        _L("test-scriptUpgradeRomClientMissingCapability"), *script );
    EUNIT_ASSERT_DESC( err == KErrAccessDenied, "Upgrade rom script failed" );

    co->SetValueL ( _L("false") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckCustomAction = ETrue;
    iCustomActionId = _L("romUpgrade" );
    keyValue = CCFKeyValuePair::NewLC ( _L("id"), _L("false") );
    iCustomActionParams.AppendL ( keyValue );
    CleanupStack::Pop ( keyValue );
    iWait->Start ( );

    CleanupStack::PopAndDestroy( script );

    // Clean up
    // -------------------------------------------------------------------------
    CleanupStack::PopAndDestroy ( actionSubscription );
    CleanupStack::PopAndDestroy ( co );
    }

void MT_CFClient::MT_CCFClient_RestoreRomScriptL( )
    {
    CCFContextObject* co = CCFContextObject::NewLC ( );
    CCFActionSubscription* actionSubscription = CCFActionSubscription::NewLC ( );

    // Setup preconditions
    // -------------------------------------------------------------------------
    co->SetSourceL ( _L("test") );
    co->SetTypeL (_L("upgrade") );
    TInt err = iClient->DefineContext ( co->Source ( ), co->Type ( ), KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone || err == KErrAlreadyExists,
        "Define context failed" );

    err = iClient->DefineAction ( _L("romUpgrade"), KPassSec );
    EUNIT_ASSERT_DESC( err == KErrNone || err == KErrAlreadyExists,
        "Define action failed" );

    actionSubscription->SetActionIdentifierL ( _L("romUpgrade") );
    err = iClient->SubscribeAction ( *actionSubscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Subscribe action failed" );

    // x. Check that rom script is currently registered
    // -------------------------------------------------------------------------
    co->SetValueL ( _L("false") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckCustomAction = ETrue;
    iCustomActionId = _L("romUpgrade" );
    CCFKeyValuePair* keyValue = CCFKeyValuePair::NewLC( _L("id"), _L("false") );
    iCustomActionParams.AppendL ( keyValue );
    CleanupStack::Pop ( keyValue );
    iWait->Start ( );

    // x. Upgrade rom script and check, restore rom script and check
    // -------------------------------------------------------------------------
    err = iClient->UpgradeRomScript (
        _L("z:\\data\\cftestdata\\testscripts\\test-scriptUpgradeRom.xml") );
    EUNIT_PRINT( _L("UpgradeRomScript: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Upgrade rom script failed" );

    co->SetValueL ( _L("true") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckCustomAction = ETrue;
    iCustomActionId = _L("romUpgrade" );
    keyValue = CCFKeyValuePair::NewLC ( _L("id"), _L("true") );
    iCustomActionParams.AppendL ( keyValue );
    CleanupStack::Pop ( keyValue );
    iWait->Start ( );

    TBool exists = BaflUtils::FileExists ( iFs,
        _L("c:\\private\\10282bc4\\rules\\test-scriptUpgradeRom.rul") );
    EUNIT_ASSERT_DESC( exists, "Script upgrade not found" );

    err = iClient->RestoreRomScript ( _L("test-scriptUpgradeRom") );
    EUNIT_PRINT( _L("RestoreRomScript: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Restore script failed" );
    co->SetValueL ( _L("false") );
    err = iClient->PublishContext ( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    iCheckCustomAction = ETrue;
    iCustomActionId = _L("romUpgrade" );
    keyValue = CCFKeyValuePair::NewLC ( _L("id"), _L("false") );
    iCustomActionParams.AppendL ( keyValue );
    CleanupStack::Pop ( keyValue );
    iWait->Start ( );

    exists = BaflUtils::FileExists ( iFs,
        _L("c:\\private\\10282bc4\\rules\\test-scriptUpgradeRom.rul") );
    EUNIT_ASSERT_DESC( !exists, "Script upgrade not deleted" );

    // x. Try to restore a script which does not exist
    // -------------------------------------------------------------------------
    err = iClient->RestoreRomScript ( _L("dummyRomScript") );
    EUNIT_ASSERT_DESC( err == KErrNotFound, "Restore script failed" );

    // Clean up
    // -------------------------------------------------------------------------
    CleanupStack::PopAndDestroy ( actionSubscription );
    CleanupStack::PopAndDestroy ( co );
    }

void MT_CFClient::MT_CCFClient_OperationUnsubscribingL()
    {
    // Define context
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Test"));
    co1->SetTypeL(_L("Counting"));
    TInt err = iClient->DefineContext( co1->Source(), co1->Type(), KPassSec );
    EUNIT_ASSERT_DESC( ( err == KErrNone || err == KErrAlreadyExists ),
        "DefineContext failed!" );

    // Define actions
    err = iClient->DefineAction( _L("testAction"), KPassSec );
    EUNIT_ASSERT_DESC( ( err == KErrNone || err == KErrAlreadyExists ),
        "Incorrect DefineAction return value!" );
    err = iClient->DefineAction( _L("testAction2"), KPassSec );
    EUNIT_ASSERT_DESC( ( err == KErrNone || err == KErrAlreadyExists ),
        "Incorrect DefineAction return value!" );

    // Subscribe both actions
    CCFActionSubscription* actionSub = CCFActionSubscription::NewLC();
    actionSub->SetActionIdentifierL ( _L("testAction") );
    err = iClient->SubscribeAction ( *actionSub );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeAction return value!" );

    CCFActionSubscription* actionSub2 = CCFActionSubscription::NewLC();
    actionSub2->SetActionIdentifierL ( _L("testAction2") );
    err = iClient->SubscribeAction ( *actionSub2 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Incorrect SubscribeAction return value!" );

    DeleteLastActionId();

    // Publish context before loading so that subscribing will find the initial
    // value used to evaluate the operation
    co1->SetValueL(_L("A"));
    err = iClient->PublishContext( *co1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );

    EUNIT_ASSERT_DESC( !iLastActionId, "Action received although script not registered!" );

    // Register script1
    TInt scriptId1;
    err = iClient->RegisterScript (
            _L("z:\\data\\cftestdata\\testscripts\\test-countOneShotWithoutValue.xml"),
            scriptId1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when registering script!" );
    EUNIT_ASSERT_DESC( scriptId1 >= 0, "Parse error!" );

    // Register script2
    TInt scriptId2;
    err = iClient->RegisterScript (
            _L("z:\\data\\cftestdata\\testscripts\\test-countOneShotWithoutValue2.xml"),
            scriptId2 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Error when registering script!" );
    EUNIT_ASSERT_DESC( scriptId2 >= scriptId1, "Parse error!" );

    EUNIT_ASSERT_DESC( !iLastActionId, "Action should not have been received!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    err = iClient->PublishContext( *co1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iLastActionId, "Action should not have been received!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    err = iClient->PublishContext( *co1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    iWait->Start();
    EUNIT_ASSERT_DESC( LastActionIdEquals( _L("testAction")),
        "Script did not cause event!" );

    DeleteLastActionId();

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("B"));
    err = iClient->PublishContext( *co1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    iWait->Start();
    EUNIT_ASSERT_DESC( LastActionIdEquals( _L("testAction2")),
    "Script did not cause event!" );

    DeleteLastActionId();

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    err = iClient->PublishContext( *co1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iLastActionId, "Action should not have been received!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    err = iClient->PublishContext( *co1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iLastActionId, "Action should not have been received!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    err = iClient->PublishContext( *co1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iLastActionId, "Action should not have been received!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    err = iClient->PublishContext( *co1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iLastActionId, "Action should not have been received!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    err = iClient->PublishContext( *co1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iLastActionId, "Action should not have been received!" );

    CleanupStack::PopAndDestroy( actionSub2 );
    CleanupStack::PopAndDestroy( actionSub );
    CleanupStack::PopAndDestroy( co1 );

    err = iClient->DeregisterScript ( scriptId1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Deregistering script caused error!" );
    err = iClient->DeregisterScript ( scriptId2 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Deregistering script caused error!" );
    }

void MT_CFClient::MT_CCFClient_RomScriptContextAutoDefineL()
    {
    // x. Preconditions
    //--------------------------------------------------------------------------
    CCFContextObject* co = CCFContextObject::NewLC();
    CCFContextSubscription* subscription = CCFContextSubscription::NewLC();
    _LIT_SECURITY_POLICY_PASS( alwaysPass );

    // x. Subscribe context which has not been defined and check
    //    that a context indication is received.
    //--------------------------------------------------------------------------
    
    // Subscribe context:
    //      source.romPublishContext
    //      type.romPublishContext.reply
    subscription->SetContextSourceL( _L("source.romPublishContext") );
    subscription->SetContextTypeL( _L("type.romPublishContext.reply") );
    TInt err = iClient->SubscribeContext( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Subscribe context failed!" );
    
    // Define and publish context:
    //      source.romPublishContext
    //      type.romPublishContext
    //      true
    err = iClient->DefineContext(
        _L("source.romPublishContext"),
        _L("type.romPublishContext"),
        alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed!" );

    co->SetSourceL( _L("source.romPublishContext") );
    co->SetTypeL( _L("type.romPublishContext") );
    co->SetValueL( _L("true") );
    err = iClient->PublishContext( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed!" );
    
    // Wait for context indication, this should not happen
    iCheckA = ETrue;
    iContextA->SetSourceL( _L("source.romPublishContext") );
    iContextA->SetTypeL( _L("type.romPublishContext.reply") );
    iContextA->SetValueL( _L("true") );
    Wait( KSecond * 2 );
    EUNIT_ASSERT_DESC( iCheckA, "Context indication received!" );
    
    // Double check that context is not defined:
    //      source.romPublishContext
    //      type.romPublishContext.reply
    co->SetSourceL( _L("source.romPublishContext") );
    co->SetTypeL( _L("type.romPublishContext.reply") );
    co->SetValueL( _L("false") );
    err = iClient->PublishContext( *co );
    EUNIT_ASSERT_DESC( err == KErrNotFound, "Undefined context was published!" );

    // x. Register script with auto define context, this should fail since the
    //    context is not auto defined.
    //--------------------------------------------------------------------------
    HBufC8* script = LoadScriptFromFile( _L("test-romPublishContext.xml") );
    EUNIT_ASSERT_DESC( script, "Unable to load script 'test-romPublishContext.xml'!" );
    TInt scriptId( KErrNotFound );
    err = iClient->RegisterScript( _L("client-test-romPublishContext.xml"),
        *script, scriptId );
    EUNIT_ASSERT_DESC( err == KErrNotFound, "Script register should fail with code KErrNotFound!" );
    EUNIT_ASSERT_DESC( scriptId == KErrNotFound, "Incorrect script ID!" );
    delete script;

    // x. Clean up
    //--------------------------------------------------------------------------
    CleanupStack::PopAndDestroy( subscription );
    CleanupStack::PopAndDestroy( co );
    }

void MT_CFClient::MT_CCFClient_RomScriptContextAutoDefine2L()
    {
    // x. Preconditions
    //--------------------------------------------------------------------------
    CCFContextObject* co = CCFContextObject::NewLC();
    CCFContextSubscription* subscription = CCFContextSubscription::NewLC();
    _LIT_SECURITY_POLICY_PASS( alwaysPass );

    // x. Subscribe context which has not been defined and check
    //    that a context indication is received.
    //--------------------------------------------------------------------------
    
    // Subscribe context:
    //      source.romPublishContext2
    //      type.romPublishContext2.reply
    subscription->SetContextSourceL( _L("source.romPublishContext2") );
    subscription->SetContextTypeL( _L("type.romPublishContext2.reply") );
    TInt err = iClient->SubscribeContext( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Subscribe context failed!" );
    
    // Define and publish context:
    //      source.romPublishContext2
    //      type.romPublishContext2
    //      true
    err = iClient->DefineContext(
        _L("source.romPublishContext2"),
        _L("type.romPublishContext2"),
        alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed!" );

    co->SetSourceL( _L("source.romPublishContext2") );
    co->SetTypeL( _L("type.romPublishContext2") );
    co->SetValueL( _L("true") );
    err = iClient->PublishContext( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed!" );
    
    // Wait for context indication
    iCheckA = ETrue;
    iContextA->SetSourceL( _L("source.romPublishContext2") );
    iContextA->SetTypeL( _L("type.romPublishContext2.reply") );
    iContextA->SetValueL( _L("true") );
    Wait( KSecond * 2 );
    EUNIT_ASSERT_DESC( !iCheckA, "Context indication not received!" );
    
    // Double check that context is defined:
    //      source.romPublishContext2
    //      type.romPublishContext2.reply
    err = iClient->DefineContext(
        _L("source.romPublishContext2"),
        _L("type.romPublishContext2.reply"),
        alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrAlreadyExists, "Define context should fail with code KErrAlreadyExists!" );

    // x. Register script with auto define context, this should fail with KErrPermissionDenied
    //    since the client can only read (subscribe) the context which is auto defined.
    //--------------------------------------------------------------------------
    HBufC8* script = LoadScriptFromFile( _L("test-romPublishContext2.xml") );
    CleanupStack::PushL( script );
    EUNIT_ASSERT_DESC( script, "Unable to load script 'test-romPublishContext2.xml'!" );
    TInt scriptId( KErrNotFound );
    err = iClient->RegisterScript( _L("client-test-romPublishContext2.xml"),
        *script, scriptId );
    EUNIT_PRINT( _L("RegisterScript completed with code: %d"), err );
    EUNIT_ASSERT_DESC( err == KErrPermissionDenied, "Script register should fail with code KErrPermissionDenied!" );
    EUNIT_ASSERT_DESC( scriptId == KErrNotFound, "Incorrect script ID!" );
    CleanupStack::PopAndDestroy( script );

    // x. Clean up
    //--------------------------------------------------------------------------
    CleanupStack::PopAndDestroy( subscription );
    CleanupStack::PopAndDestroy( co );
    }

void MT_CFClient::MT_CCFClient_RomScriptContextAutoDefine3L()
    {
    // x. Preconditions
    //--------------------------------------------------------------------------
    CCFContextObject* co = CCFContextObject::NewLC();
    CCFContextSubscription* subscription = CCFContextSubscription::NewLC();
    _LIT_SECURITY_POLICY_PASS( alwaysPass );

    // x. Subscribe context which has not been defined and check
    //    that a context indication is received.
    //--------------------------------------------------------------------------
    
    // Subscribe context:
    //      source.romPublishContext3
    //      type.romPublishContext3.reply
    subscription->SetContextSourceL( _L("source.romPublishContext3") );
    subscription->SetContextTypeL( _L("type.romPublishContext3.reply") );
    TInt err = iClient->SubscribeContext( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Subscribe context failed!" );
    
    // Define and publish context:
    //      source.romPublishContext3
    //      type.romPublishContext3
    //      true
    err = iClient->DefineContext(
        _L("source.romPublishContext3"),
        _L("type.romPublishContext3"),
        alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed!" );

    co->SetSourceL( _L("source.romPublishContext3") );
    co->SetTypeL( _L("type.romPublishContext3") );
    co->SetValueL( _L("true") );
    err = iClient->PublishContext( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed!" );
    
    // Wait for context indication, this should not happen
    iCheckA = ETrue;
    iContextA->SetSourceL( _L("source.romPublishContext3") );
    iContextA->SetTypeL( _L("type.romPublishContext3.reply") );
    iContextA->SetValueL( _L("true") );
    Wait( KSecond * 2 );
    EUNIT_ASSERT_DESC( iCheckA, "Context indication received!" );
    
    // Double check that context is not defined:
    //      source.romPublishContext3
    //      type.romPublishContext3.reply
    co->SetSourceL( _L("source.romPublishContext3") );
    co->SetTypeL( _L("type.romPublishContext3.reply") );
    co->SetValueL( _L("false") );
    err = iClient->PublishContext( *co );
    EUNIT_ASSERT_DESC( err == KErrNotFound, "Undefined context was published!" );
    
    // x. Register script with auto define context, this should fail since the
    //    context is not auto defined.
    //--------------------------------------------------------------------------
    HBufC8* script = LoadScriptFromFile( _L("test-romPublishContext3.xml") );
    CleanupStack::PushL( script );
    EUNIT_ASSERT_DESC( script, "Unable to load script 'test-romPublishContext3.xml'!" );
    TInt scriptId( KErrNotFound );
    err = iClient->RegisterScript( _L("client-test-romPublishContext3.xml"),
        *script, scriptId );
    EUNIT_ASSERT_DESC( err == KErrNotFound, "Script register should fail with code KErrNotFound!" );
    EUNIT_ASSERT_DESC( scriptId == KErrNotFound, "Incorrect script ID!" );
    CleanupStack::PopAndDestroy( script );

    // x. Clean up
    //--------------------------------------------------------------------------
    CleanupStack::PopAndDestroy( subscription );
    CleanupStack::PopAndDestroy( co );
    }

void MT_CFClient::MT_CCFClient_RomScriptContextAutoDefine4L()
    {
    // x. Preconditions
    //--------------------------------------------------------------------------
    CCFContextObject* co = CCFContextObject::NewLC();
    CCFContextSubscription* subscription = CCFContextSubscription::NewLC();
    _LIT_SECURITY_POLICY_PASS( alwaysPass );
    
    // x. Publish a context which triggers a publishContext action from a test-romPublishContext4.xml
    //    which will auto define a context. test-romPublishContext5.xml rule file
    //    has subscribed to this context.
    //    A new context is auto defined from test-romPublishContext4.xml
    //    which is subscribed by this test case.
    //    Check that the context indication is received.
    //--------------------------------------------------------------------------
    
    // Subscribe context:
    //      source.romPublishContext5
    //      type.romPublishContext5.reply
    subscription->SetContextSourceL( _L("source.romPublishContext5") );
    subscription->SetContextTypeL( _L("type.romPublishContext5.reply") );
    TInt err = iClient->SubscribeContext( *subscription );
    EUNIT_ASSERT_DESC( err == KErrNone, "Subscribe context failed!" );
    
    // Define and publish context:
    //      source.romPublishContext4
    //      type.romPublishContext4
    //      true
    err = iClient->DefineContext( 
        _L("source.romPublishContext4"),
        _L("type.romPublishContext4"),
        alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone || err == KErrAlreadyExists, "Define context failed!" );
    
    co->SetSourceL( _L("source.romPublishContext4") );
    co->SetTypeL( _L("type.romPublishContext4") );
    co->SetValueL( _L("true") );
    err = iClient->PublishContext( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed!" );
    
    // Wait for context indication
    iCheckA = ETrue;
    iContextA->SetSourceL( _L("source.romPublishContext5") );
    iContextA->SetTypeL( _L("type.romPublishContext5.reply") );
    iContextA->SetValueL( _L("true") );
    Wait( KSecond * 2 );
    EUNIT_ASSERT_DESC( !iCheckA, "Context indication not received!" );
    
    // x. Register scripts with auto define context, this should fail since the
    //    client can only read (subscribe) the auto defined context.
    //--------------------------------------------------------------------------
    HBufC8* script = LoadScriptFromFile( _L("test-romPublishContext4.xml") );
    CleanupStack::PushL( script );
    EUNIT_ASSERT_DESC( script, "Unable to load script 'test-romPublishContext4.xml'!" );
    TInt scriptId( KErrNotFound );
    err = iClient->RegisterScript( _L("client-test-romPublishContext4.xml"),
        *script, scriptId );
    EUNIT_ASSERT_DESC( err == KErrPermissionDenied, "Script register should fail with code KErrPermissionDenied!" );
    EUNIT_ASSERT_DESC( scriptId == KErrNotFound, "Incorrect script ID!" );
    CleanupStack::PopAndDestroy( script );

    script = LoadScriptFromFile( _L("test-romPublishContext5.xml") );
    CleanupStack::PushL( script );
    EUNIT_ASSERT_DESC( script, "Unable to load script 'test-romPublishContext5.xml'!" );
    scriptId = KErrNotFound;
    err = iClient->RegisterScript( _L("client-test-romPublishContext5.xml"),
        *script, scriptId );
    EUNIT_ASSERT_DESC( err == KErrPermissionDenied, "Script register should fail with code KErrPermissionDenied!" );
    EUNIT_ASSERT_DESC( scriptId == KErrNotFound, "Incorrect script ID!" );
    CleanupStack::PopAndDestroy( script );

    // x. Clean up
    //--------------------------------------------------------------------------
    CleanupStack::PopAndDestroy( subscription );
    CleanupStack::PopAndDestroy( co );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    MT_CFClient,
    "CFClient module test.",
    "MODULE" )

EUNIT_TEST(
    "NewL - test ",
    "CCFClient",
    "NewL",
    "FUNCTIONALITY",
    SetupL, MT_CCFClient_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFClient",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, MT_CCFClient_NewLCL, Teardown)

EUNIT_TEST(
    "DefineContext - test ",
    "CCFClient",
    "DefineContext",
    "FUNCTIONALITY",
    SetupL, MT_CCFClient_DefineContextL, Teardown)

EUNIT_TEST(
    "Subscribers-NoSubscribers - test ",
    "CCFClient",
    "Subscribers-NoSubscribers",
    "FUNCTIONALITY",
    SetupNewClientL, MT_CCFClient_SubscribersNosubscribersL, Teardown)

EUNIT_TEST(
    "Subscribers-NoSubscribersPlugin - test ",
    "CCFClient",
    "Subscribers-NoSubscribers",
    "FUNCTIONALITY",
    SetupNewClientL, MT_CCFClient_SubscribersNosubscribersPluginL, Teardown)

EUNIT_TEST(
    "2 Subscribers-NoSubscribersPlugin - test ",
    "CCFClient",
    "Subscribers-NoSubscribers",
    "FUNCTIONALITY",
    SetupNewClientL, MT_CCFClient_SubscribersNosubscribersPlugin2L, Teardown)

EUNIT_TEST(
    "3 Subscribers-NoSubscribersPlugin - test ",
    "CCFClient",
    "Subscribers-NoSubscribers",
    "FUNCTIONALITY",
    SetupNewClientL, MT_CCFClient_SubscribersNosubscribersPlugin3L, Teardown)

EUNIT_TEST(
    "PluginHandleSourceCommand - test ",
    "CCFClient",
    "HandleCommand",
    "FUNCTIONALITY",
    SetupNewClientL, MT_CCFClient_ContextSourceCommandPluginL, Teardown)

EUNIT_TEST(
    "ClientHandleSourceCommand - test ",
    "CCFClient",
    "HandleCommand",
    "FUNCTIONALITY",
    SetupNewClientL, MT_CCFClient_ContextSourceCommandClientL, Teardown)

EUNIT_TEST(
    "PublishContext - test ",
    "CCFClient",
    "PublishContext",
    "FUNCTIONALITY",
    SetupL, MT_CCFClient_PublishContextL, Teardown)

EUNIT_TEST(
    "PublishContextWithData - test ",
    "CCFClient",
    "PublishContext",
    "FUNCTIONALITY",
    SetupL, MT_CCFClient_PublishContextWithDataL, Teardown)

EUNIT_TEST(
    "SubscribeContext 1 - test ",
    "CCFClient",
    "SubscribeContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeContext1L, Teardown)

EUNIT_TEST(
    "SubscribeContext 2 - test ",
    "CCFClient",
    "SubscribeContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeContext2L, Teardown)

EUNIT_TEST(
    "SubscribeContext 3 - test ",
    "CCFClient",
    "SubscribeContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeContext3L, Teardown)

EUNIT_TEST(
    "SubscribeContext 4 - test ",
    "CCFClient",
    "SubscribeContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeContext4L, Teardown)

EUNIT_TEST(
    "SubscribeContext 5 - test ",
    "CCFClient",
    "SubscribeContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeContext5L, Teardown)

EUNIT_TEST(
    "SubscribeContext 6 - test ",
    "CCFClient",
    "SubscribeContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeContext6L, Teardown)

EUNIT_TEST(
    "SubscribeContext 7 - test ",
    "CCFClient",
    "SubscribeContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeContext7L, Teardown)

EUNIT_TEST(
    "SubscribeContext 8 - test ",
    "CCFClient",
    "SubscribeContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeContext8L, Teardown)

EUNIT_TEST(
    "SubscribeContext 9 - test ",
    "CCFClient",
    "SubscribeContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeContext9L, Teardown)

EUNIT_TEST(
    "SubscribeContext from separate clients - test ",
    "CCFClient",
    "SubscribeContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeContextFromDifferentClientsL, Teardown)

EUNIT_TEST(
    "SubscribeContext combined - test ",
    "CCFClient",
    "SubscribeContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeContextCombinedL, Teardown)

EUNIT_TEST(
    "SubscribeContext with data 1 - test ",
    "CCFClient",
    "SubscribeContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeContextData1L, Teardown)

EUNIT_TEST(
    "SubscribeContext with data 2 - test ",
    "CCFClient",
    "SubscribeContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeContextData2L, Teardown)

EUNIT_TEST(
    "SubscribeContext with data 3 - test ",
    "CCFClient",
    "SubscribeContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeContextData3L, Teardown)

EUNIT_TEST(
    "SubscribeContext with data 4 - test ",
    "CCFClient",
    "SubscribeContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeContextData4L, Teardown)

EUNIT_TEST(
    "SubscribeContext with data combined - test ",
    "CCFClient",
    "SubscribeContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeContextDataCombinedL, Teardown)

EUNIT_TEST(
    "UnsubscribeContext 1 - test ",
    "CCFClient",
    "UnsubscribeContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_UnsubscribeContext1L, Teardown)

EUNIT_TEST(
    "UnsubscribeContext 2 - test ",
    "CCFClient",
    "UnsubscribeContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_UnsubscribeContext2L, Teardown)

EUNIT_TEST(
    "UnsubscribeContext combined - test ",
    "CCFClient",
    "UnsubscribeContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_UnsubscribeContextCombinedL, Teardown)

EUNIT_TEST(
    "RequestContext 1 - test ",
    "CCFClient",
    "RequestContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_RequestContext1L, Teardown)

EUNIT_TEST(
    "RequestContext 2 - test ",
    "CCFClient",
    "RequestContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_RequestContext2L, Teardown)

EUNIT_TEST(
    "RequestContext 3 - test ",
    "CCFClient",
    "RequestContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_RequestContext3L, Teardown)

EUNIT_TEST(
    "RequestContextSet - test ",
    "CCFClient",
    "RequestContextSet",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_RequestContextSetL, Teardown)

EUNIT_TEST(
    "RequestContext combined - test ",
    "CCFClient",
    "RequestContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_RequestContextCombinedL, Teardown)

EUNIT_TEST(
    "DefineAction - test ",
    "CCFClient",
    "DefineAction",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_DefineActionL, Teardown)

EUNIT_TEST(
    "SubscribeAction 1 - test ",
    "CCFClient",
    "SubscribeAction",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeAction1L, Teardown)

EUNIT_TEST(
    "SubscribeAction 2 - test ",
    "CCFClient",
    "SubscribeAction",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeAction2L, Teardown)

EUNIT_TEST(
    "SubscribeAction 3 - test ",
    "CCFClient",
    "SubscribeAction",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeAction3L, Teardown)

EUNIT_TEST(
    "SubscribeAction 4 - test ",
    "CCFClient",
    "SubscribeAction",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeAction4L, Teardown)

EUNIT_TEST(
    "SubscribeAction from different client - test ",
    "CCFClient",
    "SubscribeAction",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeActionFromDifferentClientsL, Teardown)

EUNIT_TEST(
    "SubscribeAction (same) from different client 2 - test ",
    "CCFClient",
    "SubscribeAction",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeActionFromDifferentClients2L, Teardown)

EUNIT_TEST(
    "SubscribeAction combined - test ",
    "CCFClient",
    "SubscribeAction",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SubscribeActionCombinedL, Teardown)

EUNIT_TEST(
    "UnsubscribeAction - test ",
    "CCFClient",
    "UnsubscribeAction",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_UnsubscribeActionL, Teardown)

EUNIT_TEST(
    "RegisterScript - test ",
    "CCFClient",
    "RegisterScript",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_RegisterScriptL, Teardown)

EUNIT_TEST(
    "RegisterScript - test ",
    "CCFClient",
    "RegisterScript",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_RegisterScriptL, Teardown)

EUNIT_TEST(
    "DeregisterScript - test ",
    "CCFClient",
    "DeregisterScript",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_DeregisterScriptL, Teardown)

EUNIT_TEST(
    "SaveContextSourceSetting - test ",
    "CCFClient",
    "SaveContextSourceSetting",
    "FUNCTIONALITY",
    SetupSourceSettingTestClientL, MT_CCFClient_SaveContextSourceSettingL, Teardown)

EUNIT_TEST(
    "SaveContextSourceSetting - test2 ",
    "CCFClient",
    "SaveContextSourceSetting",
    "FUNCTIONALITY",
    SetupSourceSettingTestClientL, MT_CCFClient_SaveContextSourceSetting2L, Teardown)

EUNIT_TEST(
    "DeleteContextSourceSetting - test ",
    "CCFClient",
    "DeleteContextSourceSetting",
    "FUNCTIONALITY",
    SetupSourceSettingTestClientL, MT_CCFClient_DeleteContextSourceSettingL, Teardown)

EUNIT_TEST(
    "DeleteContextSourceSetting - test2 ",
    "CCFClient",
    "DeleteContextSourceSetting",
    "FUNCTIONALITY",
    SetupSourceSettingTestClientL, MT_CCFClient_DeleteContextSourceSetting2L, Teardown)

EUNIT_TEST(
    "UpgradeContextSourceSetting - test ",
    "CCFClient",
    "SaveContextSourceSetting",
    "FUNCTIONALITY",
    SetupSourceSettingTestClientL, MT_CCFClient_UpgradeContextSourceSettingL, Teardown)

EUNIT_TEST(
    "SaveScript - test ",
    "CCFClient",
    "SaveScript",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_SaveScriptL, TeardownAndScriptCleanup)

EUNIT_TEST(
    "DeleteScript - test ",
    "CCFClient",
    "DeleteScript",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_DeleteScriptL, TeardownAndScriptCleanup)

EUNIT_TEST(
    "DeleteScripts - test ",
    "CCFClient",
    "DeleteScripts",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_DeleteScriptsL, TeardownAndScriptCleanup)

EUNIT_TEST(
    "UpgradeRomScript - test ",
    "CCFClient",
    "UpgradeRomScript",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_UpgradeRomScriptL, TeardownAndScriptCleanup)

EUNIT_TEST(
    "RestoreRomScript - test ",
    "CCFClient",
    "RestoreRomScript",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_RestoreRomScriptL, TeardownAndScriptCleanup)

EUNIT_TEST(
    "Count op unsubscribing - test with two scripts without value",
    "CCFClient",
    "UnsubscribeContext",
    "FUNCTIONALITY",
    SetupClientL, MT_CCFClient_OperationUnsubscribingL, Teardown)

EUNIT_TEST(
    "RomRule AutoDef - test rom script context auto define",
    "CCFClient",
    "DefineContext",
    "FUNCTIONALITY",
    SetupNewClientL, MT_CCFClient_RomScriptContextAutoDefineL, Teardown)

EUNIT_TEST(
    "RomRul AutoDef 2 - test rom script context auto define",
    "CCFClient",
    "DefineContext",
    "FUNCTIONALITY",
    SetupNewClientL, MT_CCFClient_RomScriptContextAutoDefine2L, Teardown)

EUNIT_TEST(
    "RomRul AutoDef 3 - test rom script context auto define",
    "CCFClient",
    "DefineContext",
    "FUNCTIONALITY",
    SetupNewClientL, MT_CCFClient_RomScriptContextAutoDefine3L, Teardown)

EUNIT_TEST(
    "RomRul AutoDef 4 - test rom script context auto define",
    "CCFClient",
    "DefineContext",
    "FUNCTIONALITY",
    SetupNewClientL, MT_CCFClient_RomScriptContextAutoDefine4L, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
