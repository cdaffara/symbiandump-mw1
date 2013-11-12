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
#include "caidsarray.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void RCaIdsArray::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteInt32L( Count() );
    for( TInt i = 0; i < Count(); i++ )
        {
        aStream.WriteInt32L( operator[]( i ) );
        }
    aStream.CommitL();
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void RCaIdsArray::InternalizeL( RReadStream& aStream )
    {
    Reset();
    TInt count = aStream.ReadInt32L();
    for( TInt i = 0; i < count; i++ )
        {
        AppendL( aStream.ReadInt32L() );
        }
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C HBufC8* RCaIdsArray::MarshalDataLC() const
    {
    HBufC8* desc = HBufC8::NewLC( ( Count() + 1 ) * sizeof(TInt32) );
    TPtr8 ptr( desc->Des() );
    RDesWriteStream stream( ptr );
    CleanupClosePushL( stream );
    ExternalizeL( stream );
    stream.CommitL();
    CleanupStack::PopAndDestroy( &stream );
    return desc;
    }
