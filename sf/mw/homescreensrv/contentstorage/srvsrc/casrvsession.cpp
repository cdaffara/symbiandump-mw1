/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

#include <s32mem.h>
#include "casrvsession.h"
#include "caentriesarray.h"
#include "caarraycleanup.inl"
#include "castorageproxy.h"
#include "caidsarray.h"
#include "cainnerentry.h"
#include "cainnerquery.h"
#include "casrvnotifier.h"
#include "casrvdef.h"
#include "cautils.h"
#include "cainnernotifierfilter.h"

const TInt KParamIndNotif = 3;
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// 
// ---------------------------------------------------------
//
CCaSrvSession::~CCaSrvSession()
    {
    RHashMap<TInt, CCaSrvNotifier*>::TIter iter( iNotifiers );
    while( iter.NextValue() )
        {
        delete *iter.CurrentValue();
        }
    iNotifiers.Close();
    iStorageProxy.RemoveSession( this );
    iCaSrv.DecreaseSessionCount();
    delete iSerializedGetEntriesResult;
    delete iSerializedGetIdsResult;
    }

// ---------------------------------------------------------
// 
// ---------------------------------------------------------
//
CCaSrvSession* CCaSrvSession::NewL( CCaSrv& aCaSrv )
    {
    CCaSrvSession* self = new ( ELeave ) CCaSrvSession( aCaSrv );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------
// 
// ---------------------------------------------------------
//
CCaSrvSession::CCaSrvSession( CCaSrv& aCaSrv ) :
    iCaSrv( aCaSrv ), iNotifiers(), 
    iStorageProxy( *iCaSrv.GetStorageProxy() )
    {
    }

// ---------------------------------------------------------
// 
// ---------------------------------------------------------
//
void CCaSrvSession::ConstructL()
    {
    iCaSrv.IncreaseSessionCount();
    }

// ---------------------------------------------------------
// 
// ---------------------------------------------------------
//
void CCaSrvSession::ServiceL( const RMessage2& aMessage )
    {
    TInt err( KErrNone );
    TBool panicedClient( EFalse );
        TRAP( err , DispatchMessageL( aMessage, panicedClient ) );
    if( !panicedClient && aMessage.Function()
            != EContentArsenalNotifierNotify )
        {
        aMessage.Complete( err );
        }
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCaSrvSession::DispatchMessageL( const RMessage2& aMessage,
        TBool& aPanicedClient )
    {
    switch( aMessage.Function() )
        {
        case EContentArsenalGetList_GetSize:
            GetListSizeL( aMessage, EFull );
            break;
        case EContentArsenalGetList_GetData:
            GetListDataL( aMessage );
            break;
        case EContentArsenalGetIds_GetSize:
            GetListSizeL( aMessage, EIds );
            break;
        case EContentArsenalGetIds_GetData:
            GetIdsDataL( aMessage );
            break;
        case EContentArsenalAdd:
            AddL( aMessage );
            break;
        case EContentArsenalRemove:
            RemoveL( aMessage );
            break;
        case EContentArsenalOrganize:
            OrganizeL( aMessage );
            break;
        case EContentArsenalTouch:
            TouchL( aMessage );
            break;
        case EContentArsenalNotifierOpen:
            NewNotifierL( aMessage );
            break;
        case EContentArsenalNotifierClose:
            RemoveNotifierL( aMessage );
            break;
        case EContentArsenalNotifierNotify:
            NotifierL( aMessage.Int3() )->NotifyL( aMessage );
            break;
        case EContentArsenalNotifierCancel:
            NotifierL( aMessage.Int3() )->Cancel();
            break;
        case EContentArsenalGetChangeInfo:
            GetChangeInfoL( aMessage );
            break;
        case EContentArsenalCustomSort:
            CustomSortL( aMessage );
            break;
        default:
            aMessage.Panic( KCaSrvName, KErrArgument );
            aPanicedClient = ETrue;
            break;
        }
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCaSrvSession::GetListSizeL( const RMessage2& aMessage,
        TGetListType aType )
    {
    CCaInnerQuery* query = CCaInnerQuery::NewLC();
    MenuUtils::InternalizeObjectL( aMessage, *query );
    TInt size( 0 );
    if( aType == EIds )
        {
        size = GetIdsL( query );
        }
    else
        {
        size = GetEntriesL( query );
        }
    TPckg<TInt> sizeDes( size );
    aMessage.WriteL( KOutputPosition, sizeDes );
    CleanupStack::PopAndDestroy( query );
    }

// -----------------------------------------------------------------------------
// 
// --------------- --------------------------------------------------------------
//
TInt CCaSrvSession::GetEntriesL( const CCaInnerQuery* aQuery )
    {
    RCaEntriesArray resultContainer;
    CleanupResetAndDestroyPushL( resultContainer );
    iStorageProxy.GetEntriesL( aQuery, resultContainer );
    delete iSerializedGetEntriesResult;
    iSerializedGetEntriesResult = NULL;
    iSerializedGetEntriesResult = 
            MenuUtils::MarshalDataL( resultContainer, KDefaultExpandSize );
    CleanupStack::PopAndDestroy( &resultContainer );
    return iSerializedGetEntriesResult->Size();
    }

// -----------------------------------------------------------------------------
// 
// --------------- --------------------------------------------------------------
//
TInt CCaSrvSession::GetIdsL( const CCaInnerQuery* aQuery )
    {
    RCaIdsArray resultContainer;
    CleanupClosePushL( resultContainer );
    iStorageProxy.GetEntriesIdsL( aQuery, resultContainer );
    delete iSerializedGetIdsResult;
    iSerializedGetIdsResult = NULL;
    iSerializedGetIdsResult = 
            MenuUtils::MarshalDataL( resultContainer, KDefaultExpandSize );
    CleanupStack::PopAndDestroy( &resultContainer );
    return iSerializedGetIdsResult->Size();
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCaSrvSession::GetListDataL( const RMessage2& aMessage )
    {
    aMessage.WriteL( KOutputPosition, iSerializedGetEntriesResult->Des() );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCaSrvSession::GetIdsDataL( const RMessage2& aMessage )
    {
    aMessage.WriteL( KOutputPosition, iSerializedGetIdsResult->Des() );
    }

// -----------------------------------------------------------------------------
// 
// --------------- --------------------------------------------------------------
//
void CCaSrvSession::AddL( const RMessage2& aMessage)
    {
    CCaInnerEntry* entry = CCaInnerEntry::NewLC();
    MenuUtils::InternalizeObjectL( aMessage, *entry );
    iStorageProxy.AddL( entry, entry->GetId() > 0 );
    TPckg<TInt> resultId( entry->GetId() );
    aMessage.WriteL( KOutputPosition, resultId );
    CleanupStack::PopAndDestroy( entry );
    }

// -----------------------------------------------------------------------------
// 
// --------------- --------------------------------------------------------------
//
void CCaSrvSession::RemoveL( const RMessage2& aMessage )
    {
    RCaIdsArray entryIds;
    CleanupClosePushL( entryIds );
    MenuUtils::InternalizeObjectL( aMessage, entryIds );
    iStorageProxy.RemoveL( entryIds );
    CleanupStack::PopAndDestroy( &entryIds );
    }

// -----------------------------------------------------------------------------
// 
// --------------- --------------------------------------------------------------
//
void CCaSrvSession::OrganizeL( const RMessage2& aMessage )
    {
    RCaIdsArray entryIds;
    CleanupClosePushL( entryIds );
    MenuUtils::InternalizeObjectL( aMessage, entryIds );
    TCaOperationParams operationParams;
    TPckg<TCaOperationParams> data( operationParams );
    aMessage.Read( KInputPosition2, data );
    iStorageProxy.OrganizeL( entryIds, operationParams );
    CleanupStack::PopAndDestroy( &entryIds );
    }

// -----------------------------------------------------------------------------
// 
// --------------- --------------------------------------------------------------
//
void CCaSrvSession::TouchL( const RMessage2& aMessage )
    {
    CCaInnerEntry* entry = CCaInnerEntry::NewLC();
    MenuUtils::InternalizeObjectL( aMessage, *entry );
    
    iStorageProxy.TouchL( entry );
    
    CleanupStack::PopAndDestroy( entry );    
    }

// ---------------------------------------------------------
// 
// ---------------------------------------------------------
//
void CCaSrvSession::NewNotifierL( const RMessage2& aMessage )
    {
    CCaSrvNotifier* obj = CCaSrvNotifier::NewLC();
    TInt handle = reinterpret_cast<TInt> ( obj );
    //address pointer of obj is added as unique key in RHashMap
    iNotifiers.InsertL( handle, obj );
    CleanupStack::Pop( obj ); // Now we manage the notifier by handle.
    TPckgC<TInt> handlePckg( handle );
    TInt err = aMessage.Write( KParamIndNotif, handlePckg );
    if( err )
        {
        delete obj;
        iNotifiers.Remove( handle );
        User::Leave( err );
        }
    if( iNotifiers.Count() == 1 )
        {
        iStorageProxy.AddSessionL( this );
        }
    }

// ---------------------------------------------------------
// 
// ---------------------------------------------------------
//
CCaSrvNotifier* CCaSrvSession::NotifierL( TInt aHandle )
    {
    CCaSrvNotifier** obj = iNotifiers.Find( aHandle ); 
    if( !obj )
        {
        User::Leave( KErrBadHandle );
        }
    return *obj;
    }

// ---------------------------------------------------------
// 
// ---------------------------------------------------------
//
void CCaSrvSession::RemoveNotifierL( const RMessage2& aMessage )
    {
    CCaSrvNotifier* notifier = NotifierL( aMessage.Int3() ); 
    if( notifier )
        {
        delete notifier;
        iNotifiers.Remove( aMessage.Int3() );
        if( iNotifiers.Count() == 0 )
            {
            iStorageProxy.RemoveSession( this );
            }
        }
    }

// ---------------------------------------------------------
// 
// ---------------------------------------------------------
//
void CCaSrvSession::GetChangeInfoL( const RMessage2& aMessage )
    {
    CCaSrvNotifier* notifier = NotifierL( aMessage.Int3() ); 
    if( !notifier )
        {
        User::Leave( KErrBadHandle );
        }
    notifier->GetChangeInfoL( aMessage );
    }

// ---------------------------------------------------------
// 
// ---------------------------------------------------------
//
void CCaSrvSession::CustomSortL( const RMessage2& aMessage )
    {
    RCaIdsArray entryIds;
    CleanupClosePushL( entryIds );
    MenuUtils::InternalizeObjectL( aMessage, entryIds );
        
    TInt gruopId;
    TPckg<TInt> data( gruopId );
    aMessage.Read( KInputPosition2, data );
    
    iStorageProxy.CustomSortL( entryIds, gruopId );
    CleanupStack::PopAndDestroy( &entryIds );
    }

// ---------------------------------------------------------
// 
// ---------------------------------------------------------
//
void CCaSrvSession::EntryChanged( const CCaInnerEntry* aEntry,
        TChangeType aChangeType, const RArray<TInt>& aParentIds )
    {
    RHashMap<TInt, CCaSrvNotifier*>::TIter iter( iNotifiers );
    while( iter.NextValue() )
        {
        TRAP_IGNORE( ( *iter.CurrentValue() )->
                EntryChangedL( aEntry, aChangeType, aParentIds ) );
        }
    }

// ---------------------------------------------------------
// 
// ---------------------------------------------------------
//
void CCaSrvSession::EntryTouched( TInt aId )
    {
    RHashMap<TInt, CCaSrvNotifier*>::TIter iter( iNotifiers );
    while( iter.NextValue() )
        {
        TRAP_IGNORE( ( *iter.CurrentValue() )->EntryTouchedL( aId ) );
        }
    }

// ---------------------------------------------------------
// CCaSrvSession::GroupContentChanged
// ---------------------------------------------------------
//
void CCaSrvSession::GroupContentChanged( const RArray<TInt>& aParentIds )
    {
    RHashMap<TInt, CCaSrvNotifier*>::TIter iter( iNotifiers );
    while( iter.NextValue() )
        {
        TRAP_IGNORE( ( *iter.CurrentValue() )->
                GroupContentChangedL( aParentIds ) );
        }
    }

//  End of File
