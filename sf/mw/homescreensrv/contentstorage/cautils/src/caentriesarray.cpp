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
#include "caentriesarray.h"
#include "cainnerentry.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void RCaEntriesArray::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteInt32L( Count() );
    for( TInt i = 0; i < Count(); i++ )
        {
        operator[]( i )->ExternalizeL( aStream );
        }
    aStream.CommitL();
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void RCaEntriesArray::InternalizeL( RReadStream& aStream )
    {
    ResetAndDestroy();
    TInt count = aStream.ReadInt32L();
    for( TInt i = 0; i < count; i++ )
        {
        CCaInnerEntry* entry = CCaInnerEntry::NewLC();
        entry->InternalizeL( aStream );
        AppendL( entry );
        CleanupStack::Pop( entry );
        }
    }
