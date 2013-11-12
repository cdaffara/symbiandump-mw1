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

#include <s32strm.h>
#include <badesca.h>
#include "cainnernotifierfilter.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CCaInnerNotifierFilter::CCaInnerNotifierFilter(
    TNotifierType aInnerNotifierType ) :
    iParentId( 0 ),
    iEntryRole( EItemEntryRole ),
    iTypeNames( NULL ),
    iNotifierType( aInnerNotifierType )
    {
    }
// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CCaInnerNotifierFilter::~CCaInnerNotifierFilter()
    {
    delete iTypeNames;
    iIds.Close();
    }
// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCaInnerNotifierFilter::ConstructL()
    {
    iTypeNames = new ( ELeave ) CDesC16ArrayFlat( KDefaultGranularity );
    }
// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C CCaInnerNotifierFilter* CCaInnerNotifierFilter::NewL(
    TNotifierType aInnerNotifierType )
    {
    CCaInnerNotifierFilter* filter =
        CCaInnerNotifierFilter::NewLC( aInnerNotifierType );
    CleanupStack::Pop( filter );
    return filter;
    }
// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C CCaInnerNotifierFilter* CCaInnerNotifierFilter::NewLC(
    TNotifierType aInnerNotifierType )
    {
    CCaInnerNotifierFilter* self = new (ELeave) CCaInnerNotifierFilter(
        aInnerNotifierType );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCaInnerNotifierFilter::GetParentId() const
    {
    return iParentId;
    }
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaInnerNotifierFilter::SetParentId( TInt aParentId )
    {
    iParentId = aParentId;
    }
// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C RArray<TInt> CCaInnerNotifierFilter::GetIds() const
    {
    return iIds;
    }
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaInnerNotifierFilter::SetIdsL( const RArray<TInt>& aIds )
    {
    for( TInt i( 0 ); i < aIds.Count(); ++i )
        {
        iIds.AppendL( aIds[i] );
        }
    }
// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C TEntryRole CCaInnerNotifierFilter::GetEntryRole() const
    {
    return iEntryRole;
    }
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaInnerNotifierFilter::SetEntryRole( TEntryRole aEntryRole )
    {
    iEntryRole = aEntryRole;
    }
// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C CDesC16ArrayFlat* CCaInnerNotifierFilter::GetTypeNames() const
    {
    return iTypeNames;
    }
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaInnerNotifierFilter::SetTypeNames(
        CDesC16ArrayFlat * aTypeNames )
    {
    ASSERT( aTypeNames );
    delete iTypeNames;
    iTypeNames = aTypeNames;
    }
// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C CCaInnerNotifierFilter::TNotifierType CCaInnerNotifierFilter::GetNotifierType() const
    {
    return iNotifierType;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaInnerNotifierFilter::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteUint32L( static_cast<TUint> ( iEntryRole ) );
    aStream.WriteUint32L( static_cast<TUint> ( iNotifierType ) );
    aStream.WriteInt32L( iParentId );
    aStream.WriteInt32L( iIds.Count() );
    for( TInt i = 0; i < iIds.Count(); i++ )
        {
        aStream.WriteInt32L( iIds[i] );
        }
    aStream.WriteInt32L( iTypeNames->MdcaCount() );
    for( TInt i = 0; i < iTypeNames->MdcaCount(); i++ )
        {
        aStream.WriteInt32L( iTypeNames->MdcaPoint( i ).Length() );
        aStream.WriteL( iTypeNames->MdcaPoint( i ) );
        }
    aStream.CommitL();
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaInnerNotifierFilter::InternalizeL( RReadStream& aStream )
    {
    TUint entryRole = aStream.ReadUint32L();
    TUint notifierType = aStream.ReadUint32L();
    iEntryRole = static_cast<TEntryRole> ( entryRole );
    iNotifierType = static_cast<TNotifierType> ( notifierType );
    TInt count( 0 );
    iParentId = aStream.ReadInt32L();
    count = aStream.ReadInt32L();
    for( TInt i = 0; i < count; i++ )
        {
        iIds.AppendL( aStream.ReadInt32L() );
        }
    count = aStream.ReadInt32L();
    iTypeNames->Reset();
    for( TInt i = 0; i < count; i++ )
        {
        RBuf16 buf;
        CleanupClosePushL( buf );
        TInt length = aStream.ReadInt32L();
        buf.CreateL( length );
        aStream.ReadL( buf, length );
        iTypeNames->AppendL( buf );
        CleanupStack::PopAndDestroy( &buf );
        }
    }

// End of file
