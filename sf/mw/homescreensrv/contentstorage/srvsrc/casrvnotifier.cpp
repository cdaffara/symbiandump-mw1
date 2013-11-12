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

#include <badesca.h>
#include "casrvnotifier.h"
#include "cainnernotifierfilter.h"
#include "cainnerentry.h"
#include "casrvdef.h"
#include "cautils.h"
#include "caarraycleanup.inl"

// ================= MEMBER FUNCTIONS =======================


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCaSrvNotifier::~CCaSrvNotifier()
    {
    Complete( KErrCancel );
    iBufArrayInfo.ResetAndDestroy();
    iArrayChangeType.Close();
    delete iFilter;
    }

#ifdef COVERAGE_MEASUREMENT
#pragma CTC SKIP
#endif //COVERAGE_MEASUREMENT (only NewLC used in code)
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCaSrvNotifier* CCaSrvNotifier::NewL()
    {
    CCaSrvNotifier* self = CCaSrvNotifier::NewLC();
    CleanupStack::Pop();
    return self;
    }
#ifdef COVERAGE_MEASUREMENT
#pragma CTC ENDSKIP
#endif //COVERAGE_MEASUREMENT

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCaSrvNotifier* CCaSrvNotifier::NewLC()
    {
    CCaSrvNotifier* self = new ( ELeave ) CCaSrvNotifier();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCaSrvNotifier::CCaSrvNotifier()
    {
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCaSrvNotifier::ConstructL()
    {
    iFilter = CCaInnerNotifierFilter::NewL(
            CCaInnerNotifierFilter::EEntryChangedWithId );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCaSrvNotifier::NotifyL( const RMessage2& aMessage )
    {
    PanicIfPendingL( aMessage );
    SetPendingL( aMessage ); // No leaving after this!
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCaSrvNotifier::Cancel()
    {
    Complete( KErrCancel );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCaSrvNotifier::SetPendingL( const RMessage2 &aMessage )
    {
    __ASSERT_DEBUG( !iPending, User::Invariant() );
    iMessage = aMessage;
    iPending = ETrue;
    if( iBufArrayInfo.Count() > 0 )
        {
        Complete();
        }
    else
        {
        MenuUtils::InternalizeObjectL( aMessage, *iFilter );
        }
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCaSrvNotifier::PanicIfPendingL( const RMessage2& aMessage )
    {
    if( iPending )
        {
        // Another message is already being served.
        aMessage.Panic( KCaSrvName, KErrInUse );
        User::Leave( KErrInUse );
        }
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCaSrvNotifier::Complete( TInt aReason )
    {
    if( iPending )
        {
        if( aReason != KErrNone )
            {
            iMessage.Complete( aReason );
            iBufArrayInfo.ResetAndDestroy();
            iArrayChangeType.Close();
            }
        else
            {
            if( iBufArrayInfo.Count() > 0 )
                {
                TPckgBuf<TInt> sizeDes( iBufArrayInfo[0]->Size() );
                iMessage.Write( KOutputPosition, sizeDes );
                }
            iMessage.Complete( KErrNone );
            }
        iPending = EFalse;
        }
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCaSrvNotifier::EntryChangedL( const CCaInnerEntry* aEntry,
        TChangeType aChangeType, const RArray<TInt>& aParentIds )
    {
    if( iFilter && CheckFilterForEntry( aEntry, aParentIds ) )
        {
        if( iFilter->GetNotifierType()
                == CCaInnerNotifierFilter::EEntryChangedWithEntry )
            {
            HBufC8* bufInfo = MenuUtils::MarshalDataL( 
                    *aEntry, KDefaultExpandSize );
            CleanupStack::PushL( bufInfo );
            iBufArrayInfo.AppendL( bufInfo );
            CleanupStack::Pop( bufInfo );
            iArrayChangeType.AppendL( aChangeType );
            Complete();
            }
        else if( iFilter->GetNotifierType()
                == CCaInnerNotifierFilter::EEntryChangedWithId )
            {
            AddEntryWithIdL( aEntry->GetId() );
            iArrayChangeType.AppendL( aChangeType );
            Complete();
            }
        }
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCaSrvNotifier::EntryTouchedL( TInt aId )
    {
    if( iFilter && iFilter->GetNotifierType()
            == CCaInnerNotifierFilter::EEntryTouched && CheckIds(
            iFilter->GetIds(), aId ) )
        {
        AddEntryWithIdL( aId );
        Complete();
        }
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCaSrvNotifier::GroupContentChangedL( const RArray<TInt>& aParentIds )
    {
    if( iFilter && iFilter->GetNotifierType()
            == CCaInnerNotifierFilter::EGroupContentChanged )
        {
        if( iFilter->GetParentId() == 0 && aParentIds.Count() > 0 )
            {
            for( TInt i( 0 ); i < aParentIds.Count(); i++ )
                {
                AddEntryWithIdL( aParentIds[i] );
                }
            Complete();
            }
        else if( aParentIds.Find( iFilter->GetParentId() ) != KErrNotFound )
            {
            AddEntryWithIdL( iFilter->GetParentId() );
            Complete();
            }
        }
    }

// ---------------------------------------------------------
// 
// ---------------------------------------------------------
//
void CCaSrvNotifier::GetChangeInfoL( const RMessage2& aMessage )
    {
    if( iBufArrayInfo.Count() > 0 )
        {
        aMessage.WriteL( KInputPosition1, iBufArrayInfo[0]->Des() );
        if( iArrayChangeType.Count() > 0 )
            {
            TPckg<TChangeType> changeTypePckg( iArrayChangeType[0] );
            aMessage.WriteL( KInputPosition2, changeTypePckg );
            iArrayChangeType.Remove( 0 );
            }
        delete iBufArrayInfo[0];
        iBufArrayInfo.Remove( 0 );
        }
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool CCaSrvNotifier::CheckFilterForEntry( const CCaInnerEntry* aEntry,
        const RArray<TInt>& aParentIds )
    {
    TBool filterCorrect = CheckIds( iFilter->GetIds(), aEntry->GetId() );
    if( filterCorrect )
        {
        filterCorrect = ( static_cast<TUint> ( iFilter->GetEntryRole() )
                == ENoneEntryRole
                || static_cast<TUint> ( iFilter->GetEntryRole() )
                        == aEntry->GetRole() );
        }
    if( filterCorrect )
        {
        filterCorrect = CheckTypeName( iFilter->GetTypeNames(),
                aEntry->GetEntryTypeName() );
        }
    if( filterCorrect && iFilter->GetParentId() > 0 )
        {
        filterCorrect = ( iFilter->GetParentId() == aEntry->GetId()
                || aParentIds.Find( iFilter->GetParentId() )
                        != KErrNotFound );
        }
    return filterCorrect;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCaSrvNotifier::AddEntryWithIdL( TInt aId )
    {
    CCaInnerEntry* entry = CCaInnerEntry::NewLC();
    entry->SetId( aId );
    HBufC8* bufInfo = MenuUtils::MarshalDataL( *entry, KDefaultExpandSize );
    CleanupStack::PushL( bufInfo );
    iBufArrayInfo.AppendL( bufInfo );
    CleanupStack::Pop( bufInfo );
    CleanupStack::PopAndDestroy( entry );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool CCaSrvNotifier::CheckIds( const RArray<TInt>& aIds, TInt aId )
    {
    TBool result( ETrue );
    if( aIds.Count() > 0 && aIds.Find( aId ) == KErrNotFound )
        {
        result = EFalse;
        }
    return result;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool CCaSrvNotifier::CheckTypeName( const CDesC16ArrayFlat* aTypeNames,
        const RBuf& aTypeName )
    {
    TBool result( ETrue );
    TInt position( 0 );
    if( aTypeNames->Count() > 0 && aTypeNames->Find( aTypeName, position )
            != KErrNone )
        {
        result = EFalse;
        }
    return result;
    }

