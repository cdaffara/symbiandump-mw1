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
* Description:  XIMP Framework XIMPTestEventFactory
*
*/


#include "prfwtestcaseutilspanics.h"
#include "prfwtesteventfactory.h"

#include <eunitmacros.h>
#include <ximpcontextstateevent.h>
#include <ximprequestcompleteevent.h>
#include <ximpstatus.h>
#include <presentitypresenceevent.h>
#include <presentitygrouplistevent.h>
#include <presentitygroupcontentevent.h>
#include <presenceinfo.h>

#include "ximpidentityimp.h"
#include "presencetypehelpers.h"
#include "presentitygroupinfoimp.h"
#include "presentitygroupmemberinfoimp.h"
#include "presentitygroupcontenteventimp.h"
#include "presentitygrouplisteventimp.h"



using namespace NXIMPTestCaseUtilsPanic;



// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// create a group content event with given arrays
// ---------------------------------------------------------------------------
//
EXPORT_C CPresentityGroupContentEventImp*
XIMPTestEventFactory::CreateGroupContentEventLCX(
        const TDesC& aGroupId,
        const TDesC& aIdentityForDelta,
        const TDesC& aDispNameForDelta,
        TTestPGLContentArraySpecifier aArraySpec,
        const TDesC& aIdentityCurrent,
        const TDesC& aDispNameCurrent,
        MXIMPDataSubscriptionState::TSubscriptionState aSubscriptionState,
        MXIMPDataSubscriptionState::TDataState aDataState
        )
    {
    // id for group
    CXIMPIdentityImp* idForGroup = CXIMPIdentityImp::NewLC( aGroupId );

    // create the arrays
    RPrGrpMemInfoImpArray* addedList = new ( ELeave ) RPrGrpMemInfoImpArray;
    CleanupDeletePushL( addedList );

    RPrGrpMemInfoImpArray* updatedList = new ( ELeave ) RPrGrpMemInfoImpArray;
    CleanupDeletePushL( updatedList );

    RPrGrpMemInfoImpArray* removedList = new ( ELeave ) RPrGrpMemInfoImpArray;
    CleanupDeletePushL( removedList );

    RPrGrpMemInfoImpArray* currentList = new ( ELeave ) RPrGrpMemInfoImpArray;
    CleanupDeletePushL( currentList );

    // populate the current list
    if ( aIdentityCurrent.Length() > 0 )
        {
        // but don't put "KNullDesC" there if it's really empty
        CXIMPIdentityImp* idForCurrent = CXIMPIdentityImp::NewLC( aIdentityCurrent );
        CPresentityGroupMemberInfoImp* grpInfoForCurrent =
            CPresentityGroupMemberInfoImp::NewLC( *idForCurrent, aDispNameCurrent );
        currentList->AppendL( grpInfoForCurrent );
        CleanupStack::Pop( grpInfoForCurrent );
        CleanupStack::PopAndDestroy( idForCurrent );
        }

    // create the delta list
    CXIMPIdentityImp* idForEvent = CXIMPIdentityImp::NewLC( aIdentityForDelta );
    CPresentityGroupMemberInfoImp* grpInfoForEvent =
        CPresentityGroupMemberInfoImp::NewLC( *idForEvent, aDispNameForDelta );

    // notice the cleanup stack usage. AppendL takes ownership, but
    // it might not be done always.
    TBool didAppend = EFalse;

    // put the given group info into the specified array.
    switch ( aArraySpec )
        {
        case ETestPGLCntAdded :
            {
            if ( aIdentityForDelta.Length() > 0 )
                {
                addedList->AppendL( grpInfoForEvent );
                didAppend = ETrue;
                }
            break;
            }

        case ETestPGLCntUpdated :
            {
            if ( aIdentityForDelta.Length() > 0 )
                {
                updatedList->AppendL( grpInfoForEvent );
                didAppend = ETrue;
                }
            break;
            }

        case ETestPGLCntRemoved :
            {
            if ( aIdentityForDelta.Length() > 0 )
                {
                removedList->AppendL( grpInfoForEvent );
                didAppend = ETrue;
                }
            break;
            }

        case ETestPGLCntEmpty :
            {
            // don't add, thus creating an empty array.
            break;
            }

        default:
            {
            User::Leave( KErrArgument );
            break;
            }
        };

    // remove the event and id from cleanupstack. removal
    // method depends on whether we did append or not
    if ( didAppend )
        {
        CleanupStack::Pop( grpInfoForEvent );
        CleanupStack::PopAndDestroy( idForEvent );
        }
    else
        {
        CleanupStack::PopAndDestroy( 2, idForEvent );
        }

    CXIMPDataSubscriptionStateImp* stateImp = CXIMPDataSubscriptionStateImp::NewLC();
    stateImp->SetSubscriptionStateL( aSubscriptionState );
    stateImp->SetDataStateL( aDataState );
        
    // create the actual event
    CPresentityGroupContentEventImp* tmp =
        CPresentityGroupContentEventImp::NewLC(
                *idForGroup,
                addedList,
                updatedList,
                removedList,
                currentList,
                stateImp
                );
    CleanupStack::Pop( 2, stateImp );
    CleanupStack::PushL( tmp );

    // test the accessors
    if ( aIdentityForDelta.Length() > 0 )
        {
        switch ( aArraySpec )
            {
            case ETestPGLCntAdded :
                {
                EUNIT_ASSERT_DESC( tmp->NewMembersCount() == 1, "Added group member count mismatch" );
                TBool same = 0 == tmp->NewMember( 0 ).GroupMemberId().Identity().Compare( aIdentityForDelta );
                EUNIT_ASSERT_DESC( same, "Added group member id mismatch" );
                break;
                }

            case ETestPGLCntUpdated :
                {
                EUNIT_ASSERT_DESC( tmp->UpdatedMembersCount() == 1, "Updated group member count mismatch" );
                TBool same = 0 == tmp->UpdatedMember( 0 ).GroupMemberId().Identity().Compare( aIdentityForDelta );
                EUNIT_ASSERT_DESC( same, "Updated group member id mismatch" );
                break;
                }

            case ETestPGLCntRemoved :
                {
                EUNIT_ASSERT_DESC( tmp->DisappearedMembersCount() == 1, "Removed group member count mismatch" );
                TBool same = 0 == tmp->DisappearedMember( 0 ).GroupMemberId().Identity().Compare( aIdentityForDelta );
                EUNIT_ASSERT_DESC( same, "Removed group member id mismatch" );
                break;
                }

            case ETestPGLCntEmpty :
                {
                // don't add, thus creating an empty array.
                break;
                }

            default :
                {
                break;
                }
            }
        }

    if ( aIdentityCurrent.Length() > 0 )
        {
        EUNIT_ASSERT_DESC( tmp->CurrentMembersCount() == 1, "Current group member count mismatch" );
        
        TBool same = 0 == tmp->CurrentMember( 0 ).GroupMemberId().Identity().Compare( aIdentityCurrent );
        EUNIT_ASSERT_DESC( same, "Current group member id mismatch" );
        
        same = 0 == tmp->CurrentMember( 0 ).GroupMemberDisplayName().Compare( aDispNameCurrent );
        EUNIT_ASSERT_DESC( same, "Current group member display name mismatch" );
        }
    else
        {
        EUNIT_ASSERT_DESC( tmp->CurrentMembersCount() == 0, "Current group member count mismatch" );
        }

    /*
     * In cleanupstack there are 6 items, FIFO:
     *   - identity object
     *   - added list
     *   - updated list
     *   - removed list
     *   - current member list
     *   - event imp
     */

    return tmp;
    }


// ---------------------------------------------------------------------------
// create a group list event with given arrays
// ---------------------------------------------------------------------------
//
EXPORT_C CPresentityGroupListEventImp* XIMPTestEventFactory::CreateGroupListEventLCX(
        const TDesC& aUri,
        const TDesC& aDispName,
        TTestPGLArraySpecifier aArraySpec,
        const TDesC& aUriCurrent,
        const TDesC& aDispNameCurrent,
        MXIMPDataSubscriptionState::TSubscriptionState aSubscriptionState,
        MXIMPDataSubscriptionState::TDataState aDataState
        )
    {
    // create the arrays
    RPrGrpInfoImpArray* createdList = new ( ELeave ) RPrGrpInfoImpArray;
    CleanupDeletePushL( createdList );

    RPrGrpInfoImpArray* updatedList = new ( ELeave ) RPrGrpInfoImpArray;
    CleanupDeletePushL( updatedList );

    RPrGrpInfoImpArray* deletedList = new ( ELeave ) RPrGrpInfoImpArray;
    CleanupDeletePushL( deletedList );
    
    RPrGrpInfoImpArray* currentList = new ( ELeave ) RPrGrpInfoImpArray;
    CleanupDeletePushL( currentList );

    // create the group info
    CXIMPIdentityImp* idForEvent = CXIMPIdentityImp::NewLC( aUri );
    CPresentityGroupInfoImp* grpInfoForEvent =
        CPresentityGroupInfoImp::NewLC( *idForEvent, aDispName );

    // populate the current list
    if ( aUriCurrent.Length() > 0 )
        {
        // but don't put "KNullDesC" there if it's really empty
        CXIMPIdentityImp* idForCurrent = CXIMPIdentityImp::NewLC( aUriCurrent );
        CPresentityGroupInfoImp* grpInfoForCurrent =
            CPresentityGroupInfoImp::NewLC( *idForCurrent, aDispNameCurrent );
        currentList->AppendL( grpInfoForCurrent );
        CleanupStack::Pop( grpInfoForCurrent );
        CleanupStack::PopAndDestroy( idForCurrent );
        }
    
    // notice the cleanup stack usage. AppendL takes ownership, but
    // it might not be done always.
    TBool didAppend = EFalse;

    // put the given group info into the specified array
    switch ( aArraySpec )
        {
        case ETestPGLCreated :
            {
            if ( aUri.Length() > 0 )
                {
                createdList->AppendL( grpInfoForEvent );
                didAppend = ETrue;
                }
            break;
            }

        case ETestPGLUpdated :
            {
            if ( aUri.Length() > 0 )
                {
                updatedList->AppendL( grpInfoForEvent );
                didAppend = ETrue;
                }
            break;
            }

        case ETestPGLDeleted :
            {
            if ( aUri.Length() > 0 )
                {
                deletedList->AppendL( grpInfoForEvent );
                didAppend = ETrue;
                }
            break;
            }

        case ETestPGLEmpty :
            {
            // don't add, thus creating an empty array
            break;
            }

        default:
            {
            User::Leave( KErrArgument );
            break;
            }
        };

    // remove the event and id from cleanupstack. removal
    // method depends on whether we did append or not
    if ( didAppend )
        {
        CleanupStack::Pop( grpInfoForEvent );
        CleanupStack::PopAndDestroy( idForEvent );
        }
    else
        {
        CleanupStack::PopAndDestroy( 2, idForEvent );
        }

    CXIMPDataSubscriptionStateImp* stateImp = CXIMPDataSubscriptionStateImp::NewLC();
    stateImp->SetSubscriptionStateL( aSubscriptionState );
    stateImp->SetDataStateL( aDataState );
        
    // create the actual event
    CPresentityGroupListEventImp* tmp =
        CPresentityGroupListEventImp::NewLC(
                createdList,
                updatedList,
                deletedList,
                currentList,
                stateImp
                );                
    CleanupStack::Pop( 2, stateImp );
    CleanupStack::PushL( tmp );

    // verify that the event contains correct information
    if ( aUri.Length() > 0 )
        {
        switch ( aArraySpec )
            {
            case ETestPGLCreated :
                {
                EUNIT_ASSERT_DESC( tmp->NewGroupsCount() == 1, "Created group count mismatch" );
                TBool same = 0 == tmp->NewGroup( 0 ).GroupId().Identity().Compare( aUri );
                EUNIT_ASSERT_DESC( same, "Created group id mismatch" );
                break;
                }

            case ETestPGLUpdated :
                {
                EUNIT_ASSERT_DESC( tmp->UpdatedGroupsCount() == 1, "Updated group count mismatch" );
                TBool same = 0 == tmp->UpdatedGroup( 0 ).GroupId().Identity().Compare( aUri );
                EUNIT_ASSERT_DESC( same, "Updated group id mismatch" );
                break;
                }

            case ETestPGLDeleted :
                {
                EUNIT_ASSERT_DESC( tmp->DisappearedGroupsCount() == 1, "Deleted group count mismatch" );
                TBool same = 0 == tmp->DisappearedGroup( 0 ).GroupId().Identity().Compare( aUri );
                EUNIT_ASSERT_DESC( same, "Deleted group id mismatch" );
                break;
                }
            }
        }
    
    if ( aUriCurrent.Length() > 0 )
        {
        EUNIT_ASSERT_DESC( tmp->CurrentGroupsCount() == 1, "Current group count mismatch" );
   
        TBool same = 0 == tmp->CurrentGroup( 0 ).GroupId().Identity().Compare( aUriCurrent );
        EUNIT_ASSERT_DESC( same, "Current group id mismatch" );
        
        same = 0 == tmp->CurrentGroup( 0 ).GroupDisplayName().Compare( aDispNameCurrent );
        EUNIT_ASSERT_DESC( same, "Current group display name mismatch" );
        }
    else
        {
        EUNIT_ASSERT_DESC( tmp->CurrentGroupsCount() == 0, "Current group count mismatch, expected 0" );
        }

    /*
     * In cleanupstack there are 5 items, FIFO:
     *   - created list
     *   - updated list
     *   - deleted list
     *   - current list
     *   - event imp
     */

    return tmp;
    }

// end of file

