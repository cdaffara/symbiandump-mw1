/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MXIMPProtocolRequestCompleteEvent API object implementation.
*
*/

#include <s32strm.h>
#include "ximpprotocolrequestcompleteeventimp.h"



// ---------------------------------------------------------------------------
// CXIMPProtocolRequestCompleteEventImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPProtocolRequestCompleteEventImp*
    CXIMPProtocolRequestCompleteEventImp::NewL()
    {
    CXIMPProtocolRequestCompleteEventImp* self =
            new( ELeave ) CXIMPProtocolRequestCompleteEventImp();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPProtocolRequestCompleteEventImp::NewLC()
// ---------------------------------------------------------------------------
//
CXIMPProtocolRequestCompleteEventImp*
    CXIMPProtocolRequestCompleteEventImp::NewFromStreamLC( RReadStream& aStream )
    {
    CXIMPProtocolRequestCompleteEventImp* self = 
        new( ELeave ) CXIMPProtocolRequestCompleteEventImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    self->InternalizeL( aStream );
    return self;
    }



// ---------------------------------------------------------------------------
// CXIMPProtocolRequestCompleteEventImp::~CXIMPProtocolRequestCompleteEventImp()
// ---------------------------------------------------------------------------
//
CXIMPProtocolRequestCompleteEventImp::~CXIMPProtocolRequestCompleteEventImp()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPProtocolRequestCompleteEventImp::CXIMPProtocolRequestCompleteEventImp()
// ---------------------------------------------------------------------------
//
CXIMPProtocolRequestCompleteEventImp::CXIMPProtocolRequestCompleteEventImp()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPProtocolRequestCompleteEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPProtocolRequestCompleteEventImp::ConstructL()
    {
    }




// ---------------------------------------------------------------------------
// From class MXIMPProtocolRequestCompleteEvent.
// CXIMPProtocolRequestCompleteEventImp::RequestId()
// ---------------------------------------------------------------------------
//
const TXIMPRequestId& CXIMPProtocolRequestCompleteEventImp::RequestId() const
    {
    return iReqId;
    }


// ---------------------------------------------------------------------------
// From class MXIMPProtocolRequestCompleteEvent.
// CXIMPProtocolRequestCompleteEventImp::ResultCode()
// ---------------------------------------------------------------------------
//
TInt CXIMPProtocolRequestCompleteEventImp::ResultCode() const
    {
    return iResult;
    }



// ---------------------------------------------------------------------------
// CXIMPProtocolRequestCompleteEventImp::SetRequestId()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPProtocolRequestCompleteEventImp::SetRequestId(
    const TXIMPRequestId& aRequestId )
    {
    iReqId = aRequestId;
    }


// ---------------------------------------------------------------------------
// CXIMPProtocolRequestCompleteEventImp::SetResultCode()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPProtocolRequestCompleteEventImp::SetResultCode(
    TInt aResult )
    {
    iResult = aResult;
    }


// ---------------------------------------------------------------------------
// CXIMPProtocolRequestCompleteEventImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPProtocolRequestCompleteEventImp::ExternalizeL( 
    RWriteStream& aStream ) const
    {
    TPckgBuf< TXIMPRequestId > bufReqId( iReqId );
    aStream.WriteL( bufReqId );

    aStream.WriteInt32L( iResult );
    }
    

// ---------------------------------------------------------------------------
// CXIMPProtocolRequestCompleteEventImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPProtocolRequestCompleteEventImp::InternalizeL( 
    RReadStream& aStream )
    {
    TPckgBuf< TXIMPRequestId > bufReqId;
    aStream.ReadL( bufReqId );
    iReqId = bufReqId();

    iResult = aStream.ReadInt32L();
    }


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CXIMPProtocolRequestCompleteEventImp,
                                     MXIMPProtocolRequestCompleteEvent )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CXIMPProtocolRequestCompleteEventImp,
                                           MXIMPProtocolRequestCompleteEvent )
XIMPIMP_IF_BASE_GET_INTERFACE_END()




// End of file

