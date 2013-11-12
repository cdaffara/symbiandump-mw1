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
* Description:  Test support file tool server message
 *
*/



#include <e32std.h>
#include <f32file.h>
#include <s32file.h>

#include "prfwtestfilesrvmsg.h"

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CXIMPTestFileSrvMsg::CXIMPTestFileSrvMsg()
    {
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
void CXIMPTestFileSrvMsg::ConstructL()
    {
    iPayload = HBufC8::NewL(0);
    }


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPTestFileSrvMsg* CXIMPTestFileSrvMsg::NewLC()
    {
    CXIMPTestFileSrvMsg* self = new( ELeave ) CXIMPTestFileSrvMsg();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
void CXIMPTestFileSrvMsg::ConstructL( const TDesC8& aPayload )
    {
    iPayload = aPayload.AllocL();
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CXIMPTestFileSrvMsg::CXIMPTestFileSrvMsg( TInt aSrvMsgId ) :
    iSrvMsgId( aSrvMsgId )
    {
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPTestFileSrvMsg* CXIMPTestFileSrvMsg::NewL(
        TInt aSrvMsgId,
        const TDesC8& aPayload )
    {
    CXIMPTestFileSrvMsg* self = new( ELeave ) CXIMPTestFileSrvMsg( aSrvMsgId );
    CleanupStack::PushL( self );
    self->ConstructL( aPayload );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CXIMPTestFileSrvMsg::~CXIMPTestFileSrvMsg()
    {
    delete iPayload;
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPTestFileSrvMsg::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteUint32L( iSrvMsgId );
    aStream.WriteUint32L( iPayload->Length() );
    aStream.WriteL( *iPayload );
    }
        
// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPTestFileSrvMsg::InternalizeL( RReadStream& aStream )
    {
    iSrvMsgId = aStream.ReadUint32L();
    TInt len = aStream.ReadUint32L();

    HBufC8* buffer = HBufC8::NewLC( len );
    TPtr8 ptr = buffer->Des();
    aStream.ReadL( ptr, len );

    delete iPayload;
    CleanupStack::Pop( buffer );
    iPayload = buffer;   
    }
        
// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C TDesC8& CXIMPTestFileSrvMsg::PayloadL()
    {
    return *iPayload;
    }
        
// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CXIMPTestFileSrvMsg::SrvMsgId()
    {
    return iSrvMsgId;
    }

// End of file
