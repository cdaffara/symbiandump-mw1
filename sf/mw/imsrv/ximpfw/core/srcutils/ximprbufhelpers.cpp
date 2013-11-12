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
* Description:  Helpers for handling RBuf.
*
*/

#include "ximprbufhelpers.h"

// ==================== XIMPRBuf16Helper MEMBER FUNCTIONS ====================

// -----------------------------------------------------------------------------
// XIMPRBuf16Helper::GrowIfNeededL()
// -----------------------------------------------------------------------------
//
EXPORT_C void XIMPRBuf16Helper::GrowIfNeededL( RBuf16& aBuf, const TInt aMaxLength )
    {
    if ( aBuf.MaxLength() < aMaxLength )
        {
        aBuf.ReAllocL( aMaxLength );
        }
    }


// -----------------------------------------------------------------------------
// XIMPRBuf16Helper::SetNewValueL()
// -----------------------------------------------------------------------------
//
EXPORT_C void XIMPRBuf16Helper::SetNewValueL( RBuf16& aBuf,
                                              const TDesC16& aValue )
    {
    GrowIfNeededL( aBuf, aValue.Length() );
    aBuf.Copy( aValue );
    }


// -----------------------------------------------------------------------------
// XIMPRBuf16Helper::ExternalizeL()
// -----------------------------------------------------------------------------
//
EXPORT_C void XIMPRBuf16Helper::ExternalizeL( const RBuf16& aBuf,
                                              RWriteStream& aStream )
    {
    aStream.WriteInt32L( aBuf.Length() );
    aStream.WriteL( aBuf );
    }


// -----------------------------------------------------------------------------
// XIMPRBuf16Helper::InternalizeL()
// -----------------------------------------------------------------------------
//
EXPORT_C void XIMPRBuf16Helper::InternalizeL( RBuf16& aBuf,
                                              RReadStream& aStream )
    {
    TInt length = aStream.ReadInt32L();
    GrowIfNeededL( aBuf, length );
    aStream.ReadL( aBuf, length );
    }


// ==================== XIMPRBuf8Helper MEMBER FUNCTIONS ====================

// -----------------------------------------------------------------------------
// XIMPRBuf8Helper::GrowIfNeededL()
// -----------------------------------------------------------------------------
//
EXPORT_C void XIMPRBuf8Helper::GrowIfNeededL( RBuf8& aBuf, const TInt aMaxLength )
    {
    if ( aBuf.MaxLength() < aMaxLength )
        {
        aBuf.ReAllocL( aMaxLength );
        }
    }


// -----------------------------------------------------------------------------
// XIMPRBuf8Helper::SetNewValueL()
// -----------------------------------------------------------------------------
//
EXPORT_C void XIMPRBuf8Helper::SetNewValueL( RBuf8& aBuf,
                                             const TDesC8& aValue )
    {
    GrowIfNeededL( aBuf, aValue.Length() );
    aBuf.Copy( aValue );
    }


// -----------------------------------------------------------------------------
// XIMPRBuf8Helper::ExternalizeL()
// -----------------------------------------------------------------------------
//
EXPORT_C void XIMPRBuf8Helper::ExternalizeL( const RBuf8& aBuf,
                                             RWriteStream& aStream )
    {
    aStream.WriteInt32L( aBuf.Length() );
    aStream.WriteL( aBuf );
    }


// -----------------------------------------------------------------------------
// XIMPRBuf8Helper::InternalizeL()
// -----------------------------------------------------------------------------
//
EXPORT_C void XIMPRBuf8Helper::InternalizeL( RBuf8& aBuf,
                                             RReadStream& aStream )
    {
    TInt length = aStream.ReadInt32L();
    GrowIfNeededL( aBuf, length );
    aStream.ReadL( aBuf, length );
    }


