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


#include "eunittools.h"
#include <e32def.h>
#include <eunitmacros.h>
#include <ximprestrictedobjectcollection.h>

#include "prfwtestmessaging.h"
#include "prfwtestmessenger.h"

// ============================ MEMBER FUNCTIONS ===============================

// CONSTRUCTION
EXPORT_C CXIMPTestMessenger* CXIMPTestMessenger::NewForClientSideL( TInt aId )
    {
    CXIMPTestMessenger* self = new (ELeave) CXIMPTestMessenger( aId );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    self->DeletePubSubKeys();
    self->ResetPubSub();
    return self;
    }


EXPORT_C CXIMPTestMessenger* CXIMPTestMessenger::NewForAdaptationSideL( TInt aId )
    {
    CXIMPTestMessenger* self = new (ELeave) CXIMPTestMessenger( aId );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }



void CXIMPTestMessenger::ConstructL()
    {
    iDescription.CreateL( 512 );
    }

// reset the given pubsub keys
// and any helper variables

_LIT_SECURITY_POLICY_PASS( KAllowReadToAll );
_LIT_SECURITY_POLICY_PASS( KAllowWriteToAll );

EXPORT_C void CXIMPTestMessenger::ResetPubSub()
    {
    // check if we've already defined the keys. if we redefine and set
    // defaults, the test logic breaks.
    TInt pubSubDefined = GetValueFor( EXIMPTestMetaPubSubDefined );
    if ( pubSubDefined )
        {
        // all done
        return;
        }

    TInt r = 0;

    // special behavioral keys
    r = RProperty::Define( iMyUid, EXIMPTestMetaPubSubDefined, RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );

    r = RProperty::Define( iMyUid, EXIMPPlgTestDeath,                RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPTestPlgSessionLostStrategy,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );

    // behavioral keys
    r = RProperty::Define( iMyUid, EXIMPTestPlgBehaveLeave,    RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPTestPlgBehaveError,    RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPTestPlgBehaveCloseHandle, RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPTestPlgBehaveInvalidReqId, RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    // TODO add more here

    // "method called" keys
    r = RProperty::Define( iMyUid, EXIMPPlgTestOpenSessionCalled,       RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestShareSessionCalled,      RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestUnshareSessionCalled,    RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestCloseSessionCalled,      RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestPublishPresenceCalled,   RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestUpdateInterestCalled,    RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestSubscribeOwnPresenceCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestUnsubscribeOwnPresenceCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestUpdateOwnSubscriptionCalled, RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestSubscribeGroupListCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestUnsubscribeGroupListCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestCreatePresentityGroupCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestDeletePresentityGroupCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestUpdatePresentityGroupDisplayNameCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestSubscribePresentityPresesenceCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestUnsubscribePresentityPresesenceCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestUpdatePresentityPresesenceCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestSubscribePresentityGroupContentCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestUnsubscribePresentityGroupContentCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestAddPresentityGroupMemberCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestRemovePresentityGroupMemberCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestUpdatePresentityGroupMemberDisplayNameCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestSubscribePresenceWatcherListCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestUnsubscribePresenceWatcherListCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestSubscribePresentityGroupMembersPresesenceCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestUnsubscribePresentityGroupMembersPresesenceCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestUpdatePresentityGroupMembersPresesenceCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestSubscribePresenceGrantRequestListCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestUnsubscribePresenceGrantRequestListCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );

    r = RProperty::Define( iMyUid, EXIMPPlgTestGrantPresenceForPresentityCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestGrantPresenceForEveryoneCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestUpdateGrantPresenceForPresentityCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestUpdateGrantPresenceForPresentityGroupMembersCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestUpdateGrantPresenceForEveryoneCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestWithdrawPresenceFromPresentityCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestWithdrawPresenceFromPresentityGroupMembersCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestWithdrawPresenceFromEveryoneCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    
    r = RProperty::Define( iMyUid, EXIMPPlgTestSubscribePresenceBlockListCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestUnsubscribePresenceBlockListCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestBlockPresenceForPresentityCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestCancelPresenceBlockFromPresentityCalled,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );

//Pia
    r = RProperty::Define( iMyUid, EXIMPPlgTestSubscribePresentityGroupListCalled, RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestUnsubscribePresentityGroupListCalled, RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestHandlePresentityGroupListCalled, RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );
    r = RProperty::Define( iMyUid, EXIMPPlgTestHandlePresentityPresenceCalled, RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );

    r = RProperty::Define( iMyUid, EXIMPPlgTestExistsGroupContentInCache,  RProperty::EInt, KAllowReadToAll, KAllowWriteToAll );

    // TODO add more here

    // reset keys which have special meaning to defaults which will cause
    // no actions - a happy path scenario
    SetValueFor( EXIMPPlgTestDeath, EXIMPPrtValPluginDeathEmpty );
    SetValueFor( EXIMPTestPlgSessionLostStrategy, EXIMPPrtValSessionLostEmpty );

    // set other behavioral things to "happy path" by default
    SetValueFor( EXIMPTestPlgBehaveLeave, KErrNone );
    SetValueFor( EXIMPTestPlgBehaveError, KErrNone );
    SetValueFor( EXIMPTestPlgBehaveCloseHandle, 0 );
    SetValueFor( EXIMPTestPlgBehaveInvalidReqId, 0 );

    // reset method called messages
    SetValueFor( EXIMPPlgTestOpenSessionCalled, 0 );
    SetValueFor( EXIMPPlgTestShareSessionCalled, 0 );
    SetValueFor( EXIMPPlgTestUnshareSessionCalled, 0 );
    SetValueFor( EXIMPPlgTestCloseSessionCalled, 0 );
    SetValueFor( EXIMPPlgTestPublishPresenceCalled, 0 );
    SetValueFor( EXIMPPlgTestUpdateInterestCalled, 0 );
    SetValueFor( EXIMPPlgTestSubscribeOwnPresenceCalled, 0 );
    SetValueFor( EXIMPPlgTestUnsubscribeOwnPresenceCalled, 0 );
    SetValueFor( EXIMPPlgTestUpdateOwnSubscriptionCalled, 0 );
    SetValueFor( EXIMPPlgTestSubscribeGroupListCalled, 0 );
    SetValueFor( EXIMPPlgTestUnsubscribeGroupListCalled, 0 );
    SetValueFor( EXIMPPlgTestCreatePresentityGroupCalled, 0 );
    SetValueFor( EXIMPPlgTestDeletePresentityGroupCalled, 0 );
    SetValueFor( EXIMPPlgTestUpdatePresentityGroupDisplayNameCalled, 0 );
    SetValueFor( EXIMPPlgTestSubscribePresentityPresesenceCalled, 0 );
    SetValueFor( EXIMPPlgTestUnsubscribePresentityPresesenceCalled, 0 );
    SetValueFor( EXIMPPlgTestUpdatePresentityPresesenceCalled, 0 );
    SetValueFor( EXIMPPlgTestSubscribePresentityGroupContentCalled, 0 );
    SetValueFor( EXIMPPlgTestUnsubscribePresentityGroupContentCalled, 0 );
    SetValueFor( EXIMPPlgTestAddPresentityGroupMemberCalled, 0 );
    SetValueFor( EXIMPPlgTestRemovePresentityGroupMemberCalled, 0 );
    SetValueFor( EXIMPPlgTestUpdatePresentityGroupMemberDisplayNameCalled, 0 );
    SetValueFor( EXIMPPlgTestSubscribePresenceWatcherListCalled, 0 );
    SetValueFor( EXIMPPlgTestUnsubscribePresenceWatcherListCalled, 0 );
    SetValueFor( EXIMPPlgTestSubscribePresentityGroupMembersPresesenceCalled, 0 );
    SetValueFor( EXIMPPlgTestUnsubscribePresentityGroupMembersPresesenceCalled, 0 );
    SetValueFor( EXIMPPlgTestUpdatePresentityGroupMembersPresesenceCalled, 0 );
    SetValueFor( EXIMPPlgTestSubscribePresenceGrantRequestListCalled, 0 );
    SetValueFor( EXIMPPlgTestUnsubscribePresenceGrantRequestListCalled, 0 );

    SetValueFor( EXIMPPlgTestGrantPresenceForPresentityCalled, 0 );
    SetValueFor( EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled, 0 );
    SetValueFor( EXIMPPlgTestGrantPresenceForEveryoneCalled, 0 );
    SetValueFor( EXIMPPlgTestUpdateGrantPresenceForPresentityCalled, 0 );
    SetValueFor( EXIMPPlgTestUpdateGrantPresenceForPresentityGroupMembersCalled, 0 );
    SetValueFor( EXIMPPlgTestUpdateGrantPresenceForEveryoneCalled, 0 );
    SetValueFor( EXIMPPlgTestWithdrawPresenceFromPresentityCalled, 0 );
    SetValueFor( EXIMPPlgTestWithdrawPresenceFromPresentityGroupMembersCalled, 0 );
    SetValueFor( EXIMPPlgTestWithdrawPresenceFromEveryoneCalled, 0 );

    SetValueFor( EXIMPPlgTestSubscribePresenceBlockListCalled, 0 );
    SetValueFor( EXIMPPlgTestUnsubscribePresenceBlockListCalled, 0 );
    SetValueFor( EXIMPPlgTestBlockPresenceForPresentityCalled, 0 );
    SetValueFor( EXIMPPlgTestCancelPresenceBlockFromPresentityCalled, 0 );
    //Pia
    SetValueFor( EXIMPPlgTestSubscribePresentityGroupListCalled, 0 );  // DoSubscribePresentityGroupListL
    SetValueFor( EXIMPPlgTestUnsubscribePresentityGroupListCalled, 0 );  // DoUnsubscribePresentityGroupListL
    SetValueFor( EXIMPPlgTestHandlePresentityGroupListCalled, 0 ); //EXIMPPlgTestHandlePresentityGroupListCalled
    SetValueFor( EXIMPPlgTestHandlePresentityPresenceCalled, 0 );

    SetValueFor( EXIMPPlgTestExistsGroupContentInCache, 0 );

    // TODO add any new keys also to DeletePubSubKeys!!!


   // TODO add more here

    // TODO make the integer-valued keys and values into an array with
    // default values, then loop over the array

    // pubsub things have been defined. keep this as last line!
    SetValueFor( EXIMPTestMetaPubSubDefined, 1 );
    }


EXPORT_C void CXIMPTestMessenger::SetPluginIndex( TInt aIndex /* = KErrNotFound */ )
    {
    // NOTE! Plugin index is ALWAYS set to base UID!
    TInt val = 0;
    TInt r = RProperty::Get( KXIMPTestPropertyCatBase,EXIMPTestMetaPluginIndex, val );
    if ( r == KErrNotFound )
        {
        // value was not defined
        TInt r = RProperty::Define( KXIMPTestPropertyCatBase, EXIMPTestMetaPluginIndex, RProperty::EInt );
        }

    r = RProperty::Set( KXIMPTestPropertyCatBase, EXIMPTestMetaPluginIndex, aIndex );

    iMyUid = TUid::Uid( KXIMPTestPropertyCatBase.iUid + aIndex );
    }


EXPORT_C TInt CXIMPTestMessenger::GetPluginIndex()
    {
    TInt val = 0;
    TInt r = RProperty::Get( KXIMPTestPropertyCatBase,EXIMPTestMetaPluginIndex, val );
    if ( r == KErrNotFound )
        {
        val = KErrNotFound;
        }

    return val;
    }


CXIMPTestMessenger::~CXIMPTestMessenger()
    {
    DeletePubSubKeys();
    iDescription.Close();
    delete iReqCompleteParams;
    }


CXIMPTestMessenger::CXIMPTestMessenger( TInt aId )
    {
    iId = aId;
    iMyUid = TUid::Uid( KXIMPTestPropertyCatBase.iUid + iId );
    }


// ===========================================================================
// PUBLIC FUNCTIONS
// ===========================================================================
//

//
// Setters and getters for error code
//
EXPORT_C void CXIMPTestMessenger::SetNoError()
    {
    SetValueFor( EXIMPTestPlgBehaveError, KErrNone );
    }

EXPORT_C void CXIMPTestMessenger::SetError( TInt aErrorCode )
    {
    SetValueFor( EXIMPTestPlgBehaveError, aErrorCode );
    }

EXPORT_C TInt CXIMPTestMessenger::GetError()
    {
    return GetValueFor( EXIMPTestPlgBehaveError );
    }

//
// Setters and getters for boolean flags
//
EXPORT_C void CXIMPTestMessenger::SetBoolean( TXIMPTestPropertyKeys aKey )
    {
    // aKey takes boolean value, e.g. "*BehaveInvalidReqId"
    SetValueFor( aKey, 1 );
    }

EXPORT_C TBool CXIMPTestMessenger::GetBoolean( TXIMPTestPropertyKeys aKey ) const
    {
    // aKey takes boolean value, e.g. "*BehaveInvalidReqId"
    return GetValueFor( aKey ) == 1;
    }

//
// Setters, getters and utility methods for leave codes
//
EXPORT_C void CXIMPTestMessenger::SetLeave( TInt aLeaveCode )
    {
    SetValueFor( EXIMPTestPlgBehaveLeave, aLeaveCode );
    }

EXPORT_C TInt CXIMPTestMessenger::GetLeave()
    {
    return GetValueFor( EXIMPTestPlgBehaveLeave );
    }

// call this method to automatically handle leaves
EXPORT_C void CXIMPTestMessenger::HandleLeaveL()
    {
    User::LeaveIfError( GetLeave() );
    }


//
//
// General messaging support for direct access,
// also used internally as setters and getters.
//
EXPORT_C void CXIMPTestMessenger::SetValueFor( TXIMPTestPropertyKeys aKey, TInt aValue )
    {
    TXIMPTestMsg testMsg;
    testMsg.iKey = aKey;
    testMsg.iVal = aValue;
    Send( testMsg );
    }

EXPORT_C TInt CXIMPTestMessenger::GetValueFor( TXIMPTestPropertyKeys aKey ) const
    {
    TXIMPTestMsg testMsg;
    testMsg.iKey = aKey;
    Receive( testMsg );
    return testMsg.iVal;
    }

//
// Actual pubsub setters and getters
//
void CXIMPTestMessenger::Send( TXIMPTestMsg& aMsg )
    {
    TInt r = RProperty::Set( iMyUid, aMsg.iKey, aMsg.iVal );
    // TODO handle error in r
    }

void CXIMPTestMessenger::Receive( TXIMPTestMsg& aMsg ) const
    {
    TInt val = 0;
    TInt r = RProperty::Get( iMyUid,
            aMsg.iKey, val );
    aMsg.iVal = val;
    // TODO handle error in r, set val appropriately (0)?
    }

// ----------------------------------------------------------------
//
// assert helpers
//
void CXIMPTestMessenger::CommonAssert( TXIMPTestPropertyKeys aKey,
        TBool aCompareState,
        const TDesC8& aTrueDesc,
        const TDesC8& aFalseDesc ) const
    {
    iDescription.Zero();
    TBool value = GetBoolean( aKey );

    if ( aCompareState )
        {
        // the true case means success here
        iDescription = aFalseDesc;
        PRFW_ASSERT_DESC( value, iDescription.PtrZ() );
        }
    else
        {
        // the false case means success here
        iDescription = aTrueDesc;
        PRFW_ASSERT_DESC( ! value, iDescription.PtrZ() );
        }
    }

//
// One Assert-method for each boolean-value (see prfwtestmessaging.h)
//
EXPORT_C void CXIMPTestMessenger::AssertPluginDied( TBool aState )
    {
    // ETrue == Yes, plugin died.
/*    CommonAssert( EXIMPPlgTestDeath, aState,
                 _L8( "Plugin was destroyed" ),
                 _L8( "Plugin was not destroyed" ) );*/
    }

EXPORT_C void CXIMPTestMessenger::AssertOpenSessionCalled( TBool aState )
    {
    CommonAssert( EXIMPPlgTestOpenSessionCalled, aState,
                 _L8( "OpenSession called" ),
                 _L8( "OpenSession not called" ) );
    }

EXPORT_C void CXIMPTestMessenger::AssertShareSessionCalled( TBool aState )
    {
    CommonAssert( EXIMPPlgTestShareSessionCalled, aState,
                 _L8( "ShareSession called" ),
                 _L8( "ShareSession not called" ) );
    }

EXPORT_C void CXIMPTestMessenger::AssertCloseSessionCalled( TBool aState )
    {
    CommonAssert( EXIMPPlgTestCloseSessionCalled, aState,
                 _L8( "CloseSession called" ),
                 _L8( "CloseSession not called" ) );
    }

EXPORT_C void CXIMPTestMessenger::AssertUnshareSessionCalled( TBool aState )
    {
    CommonAssert( EXIMPPlgTestUnshareSessionCalled, aState,
                 _L8( "UnshareSession called" ),
                 _L8( "UnshareSession not called" ) );
    }

EXPORT_C void CXIMPTestMessenger::AssertSubscribePresentityGroupContentCalled( TBool aState )
    {
    CommonAssert( EXIMPPlgTestSubscribePresentityGroupContentCalled, aState,
            _L8( "DoSubscribePresentityGroupContentL called" ),
            _L8( "DoSubscribePresentityGroupContentL not called" ) );
    }

EXPORT_C void CXIMPTestMessenger::AssertSubscribePresentityGroupListCalled( TBool aState )
    {
    CommonAssert( EXIMPPlgTestSubscribeGroupListCalled, aState,
            _L8( "DoSubscribePresentityGroupListL called" ),
            _L8( "DoSubscribePresentityGroupListL not called" ) );
    }

EXPORT_C void CXIMPTestMessenger::AssertUnsubscribePresentityGroupContentCalled( TBool aState )
    {
    CommonAssert( EXIMPPlgTestUnsubscribePresentityGroupContentCalled, aState,
                 _L8( "DoUnsubscribePresentityGroupContentL called" ),
                 _L8( "DoUnsubscribePresentityGroupContentL not called" ) );
    }

EXPORT_C void CXIMPTestMessenger::AssertUnsubscribePresentityGroupListCalled( TBool aState )
    {
    CommonAssert( EXIMPPlgTestUnsubscribeGroupListCalled, aState,
                 _L8( "DoUnsubscribePresentityGroupListL called" ),
                 _L8( "DoUnsubscribePresentityGroupListL not called" ) );
    }

EXPORT_C void CXIMPTestMessenger::AssertAddPresentityGroupMemberCalled( TBool aState )
    {
    CommonAssert( EXIMPPlgTestAddPresentityGroupMemberCalled, aState,
            _L8( "DoAddPresentityGroupMemberL called" ),
            _L8( "DoAddPresentityGroupMemberL not called" ) );
    }

EXPORT_C void CXIMPTestMessenger::AssertRemovePresentityGroupMemberCalled( TBool aState )
    {
    CommonAssert( EXIMPPlgTestRemovePresentityGroupMemberCalled, aState,
                 _L8( "DoRemovePresentityGroupMemberL called" ),
                 _L8( "DoRemovePresentityGroupMemberL not called" ) );
    }

EXPORT_C void CXIMPTestMessenger::AssertUpdatePresentityGroupMemberDisplayNameCalled( TBool aState )
    {
    CommonAssert( EXIMPPlgTestUpdatePresentityGroupMemberDisplayNameCalled, aState,
                 _L8( "DoUpdatePresentityGroupMemberDisplayNameL called" ),
                 _L8( "DoUpdatePresentityGroupMemberDisplayNameL not called" ) );
    }

EXPORT_C void CXIMPTestMessenger::AssertUpdatePresentityGroupDisplayNameCalled( TBool aState )
    {
    CommonAssert( EXIMPPlgTestUpdatePresentityGroupDisplayNameCalled, aState,
                 _L8( "DoUpdatePresentityGroupDisplayNameL called" ),
                 _L8( "DoUpdatePresentityGroupDisplayNameL not called" ) );
    }

EXPORT_C void CXIMPTestMessenger::AssertCreatePresentityGroupCalled( TBool aState )
    {
    CommonAssert( EXIMPPlgTestCreatePresentityGroupCalled, aState,
                 _L8( "DoCreatePresentityGroupL called" ),
                 _L8( "DoCreatePresentityGroupL not called" ) );
    }

EXPORT_C void CXIMPTestMessenger::SetReqCompleteParams( MXIMPRestrictedObjectCollection* aParams )
    {
    delete iReqCompleteParams;
    iReqCompleteParams = aParams;
    }
EXPORT_C MXIMPRestrictedObjectCollection* CXIMPTestMessenger::GetReqCompleteParams()
    {
    MXIMPRestrictedObjectCollection* retParam = iReqCompleteParams;
    iReqCompleteParams = NULL;
    return retParam;
    }

// TODO add more

EXPORT_C void CXIMPTestMessenger::DeletePubSubKeys()
    {
    TInt r = RProperty::Delete( iMyUid, EXIMPPlgTestDeath );
    r = RProperty::Delete( iMyUid, EXIMPTestPlgSessionLostStrategy );

    // behavioral keys
    r = RProperty::Delete( iMyUid, EXIMPTestPlgBehaveLeave );
    r = RProperty::Delete( iMyUid, EXIMPTestPlgBehaveError );
    r = RProperty::Delete( iMyUid, EXIMPTestPlgBehaveCloseHandle );
    r = RProperty::Delete( iMyUid, EXIMPTestPlgBehaveInvalidReqId );
    // TODO add more here

    // "method called" keys
    r = RProperty::Delete( iMyUid, EXIMPPlgTestOpenSessionCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestShareSessionCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestUnshareSessionCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestCloseSessionCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestPublishPresenceCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestUpdateInterestCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestSubscribeOwnPresenceCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestUnsubscribeOwnPresenceCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestUpdateOwnSubscriptionCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestSubscribeGroupListCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestUnsubscribeGroupListCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestCreatePresentityGroupCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestDeletePresentityGroupCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestUpdatePresentityGroupDisplayNameCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestSubscribePresentityPresesenceCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestUnsubscribePresentityPresesenceCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestUpdatePresentityPresesenceCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestSubscribePresentityGroupMembersPresesenceCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestUnsubscribePresentityGroupMembersPresesenceCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestUpdatePresentityGroupMembersPresesenceCalled );

    r = RProperty::Delete( iMyUid, EXIMPPlgTestSubscribePresentityGroupContentCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestUnsubscribePresentityGroupContentCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestAddPresentityGroupMemberCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestRemovePresentityGroupMemberCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestUpdatePresentityGroupMemberDisplayNameCalled );

    r = RProperty::Delete( iMyUid, EXIMPPlgTestSubscribePresenceWatcherListCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestUnsubscribePresenceWatcherListCalled );

    r = RProperty::Delete( iMyUid, EXIMPPlgTestSubscribePresenceGrantRequestListCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestUnsubscribePresenceGrantRequestListCalled );

    r = RProperty::Delete( iMyUid, EXIMPPlgTestGrantPresenceForPresentityCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestGrantPresenceForEveryoneCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestUpdateGrantPresenceForPresentityCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestUpdateGrantPresenceForPresentityGroupMembersCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestUpdateGrantPresenceForEveryoneCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestWithdrawPresenceFromPresentityCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestWithdrawPresenceFromPresentityGroupMembersCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestWithdrawPresenceFromEveryoneCalled );

    r = RProperty::Delete( iMyUid, EXIMPPlgTestSubscribePresenceBlockListCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestUnsubscribePresenceBlockListCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestBlockPresenceForPresentityCalled );
    r = RProperty::Delete( iMyUid, EXIMPPlgTestCancelPresenceBlockFromPresentityCalled );

    
       //Pia
    r = RProperty::Delete( iMyUid, EXIMPPlgTestSubscribePresentityGroupListCalled );  // DoSubscribePresentityGroupListL
    r = RProperty::Delete( iMyUid, EXIMPPlgTestUnsubscribePresentityGroupListCalled );  // DoUnsubscribePresentityGroupListL
    r = RProperty::Delete( iMyUid, EXIMPPlgTestHandlePresentityGroupListCalled); //EXIMPPlgTestHandle
    r = RProperty::Delete( iMyUid, EXIMPPlgTestHandlePresentityPresenceCalled); //


    r = RProperty::Delete( iMyUid, EXIMPTestMetaPubSubDefined );

    r = RProperty::Delete( iMyUid, EXIMPPlgTestExistsGroupContentInCache );    
    }


// end of file


