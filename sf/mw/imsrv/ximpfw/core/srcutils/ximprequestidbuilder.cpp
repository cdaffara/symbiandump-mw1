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
* Description:  TXIMPRequestId builder.
 *
*/

#include <ximpbase.h>
#include "ximprequestidbuilder.h"


/**
 * Internal request ID layout.
 * This structure size must match structly to TXIMPRequestId size.
 */
struct SXIMPRequestId
    {
    TUint32 iSpare;
    TUint32 iProtocol;
    TUint32 iSession;
    TUint32 iRequest;
    };
    
__ASSERT_COMPILE( sizeof( TXIMPRequestId ) == sizeof( SXIMPRequestId ) );    


// -----------------------------------------------------------------------------
// TXIMPRequestIdBuilder::TXIMPRequestIdBuilder()
// -----------------------------------------------------------------------------
//
EXPORT_C TXIMPRequestIdBuilder::TXIMPRequestIdBuilder()
    : iSpare( 0 ),
      iProtocol( 0 ),
      iSession( 0 ),
      iRequest( 0 )
    {
    }


// -----------------------------------------------------------------------------
// TXIMPRequestIdBuilder::SetProtocolId()
// -----------------------------------------------------------------------------
//
EXPORT_C void TXIMPRequestIdBuilder::SetProtocolId( TUint32 aProtocol )
    {
    iProtocol = aProtocol;
    }


// -----------------------------------------------------------------------------
// TXIMPRequestIdBuilder::SetSessionId()
// -----------------------------------------------------------------------------
//
EXPORT_C void TXIMPRequestIdBuilder::SetSessionId( TUint32 aSession )
    {
    iSession = aSession;
    }


// -----------------------------------------------------------------------------
// TXIMPRequestIdBuilder::SetRequestId()
// -----------------------------------------------------------------------------
//
EXPORT_C void TXIMPRequestIdBuilder::SetRequestId( TUint32 aRequest )
    {
    iRequest = aRequest;
    }


// -----------------------------------------------------------------------------
// TXIMPRequestIdBuilder::BuildRequestId()
// -----------------------------------------------------------------------------
//
EXPORT_C TXIMPRequestId TXIMPRequestIdBuilder::BuildRequestId() const
    {
    SXIMPRequestId regIdStruct;
    regIdStruct.iSpare = iSpare;
    regIdStruct.iProtocol = iProtocol;
    regIdStruct.iSession = iSession;
    regIdStruct.iRequest = iRequest;

    TXIMPRequestId regId;
    Mem::Copy( &regId, &regIdStruct, sizeof( TXIMPRequestId ) );

    return regId;
    }



// END OF FILE
