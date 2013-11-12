/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CFServiceUtils methods.
*
*/


#include <s32strm.h>

#include "cfserviceutils.h"
#include "cftrace.h"

// CONSTANTS

#ifdef _DEBUG
// Panic category
_LIT( KPanicCat, "CFSERVUTIL" );

// Panic codes
enum TPanicReason
    {
    ENullPointer
    };
    
LOCAL_C void Panic( TInt aCode )
    {
    User::Panic( KPanicCat, aCode );
    }
#endif

//-----------------------------------------------------------------------------
// CFServiceUtils::UpdateBufferL
//-----------------------------------------------------------------------------
//
void CFServiceUtils::UpdateBufferL( HBufC*& aBuffer,
    const TDesC& aNewData )
    {
    FUNC_LOG;
    
    __ASSERT_DEBUG( aBuffer, Panic( ENullPointer ) );
    
    TPtr bufferPtr = aBuffer->Des();
    if( aNewData.Length() < bufferPtr.MaxLength() )
        {
        // Just copy data
        bufferPtr.Copy( aNewData );
        }
    else
        {
        // need to increase buffer and copy
        delete aBuffer;
        aBuffer = NULL;
        aBuffer = aNewData.AllocL();
        }
    }

//-----------------------------------------------------------------------------
// CFServiceUtils::WriteIntoStreamL
//-----------------------------------------------------------------------------
//
void CFServiceUtils::WriteIntoStreamL( const HBufC* const aBuf,
    RWriteStream& aStream )
    {
    FUNC_LOG;

    TPtrC bufPtrC( KNullDesC );
    if( aBuf )
        {
        bufPtrC.Set( *aBuf );
        }
    TInt length = bufPtrC.Length();
    aStream.WriteInt16L( length );
    if( length > 0 )
        {
        aStream.WriteL( bufPtrC, length );
        }
    }

//-----------------------------------------------------------------------------
// CFServiceUtils::ReadFromStreamL
//-----------------------------------------------------------------------------
//
void CFServiceUtils::ReadFromStreamL( HBufC*& aBuffer,
    RReadStream& aStream )
    {
    FUNC_LOG;
    
    TInt length = aStream.ReadInt16L();
    if( length > 0 )
        {
        // Create new temporary buffer
        HBufC* buffer = HBufC::NewLC( length );
        
        // Read data
        TPtr valuePtr = buffer->Des();
        aStream.ReadL( valuePtr, length );
        CleanupStack::Pop( buffer );

        // Update buffer
        delete aBuffer;
        aBuffer = NULL;
        aBuffer = buffer;
        }
    }
