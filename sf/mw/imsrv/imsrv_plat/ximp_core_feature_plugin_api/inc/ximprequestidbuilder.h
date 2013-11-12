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



#ifndef XIMPREQUESTIDBUILDER_H
#define XIMPREQUESTIDBUILDER_H

#include <e32base.h>

class TXIMPRequestId;

/**
 * The request ID builder for XIMP internal use to
 * construct valid TXIMPRequestId:s
 *
 * @since S60 v3.2
 */
class TXIMPRequestIdBuilder
    {
public:

    /**
     * C++ default constructor.
     * Initialises all parts to to their default values.
     */
    IMPORT_C TXIMPRequestIdBuilder();


    /**
     * Sets the protocol ID part.
     */
    IMPORT_C void SetProtocolId( TUint32 aProtocol );


    /**
     * Sets the session ID part.
     */
    IMPORT_C void SetSessionId( TUint32 aSession );


    /**
     * Sets the request ID part.
     */
    IMPORT_C void SetRequestId( TUint32 aRequest );


    /**
     * Constructs a TXIMPRequestId object having
     * valued as specified in member parts.
     */
    IMPORT_C TXIMPRequestId BuildRequestId() const;



private: // data

    //OWN: Spare value reserved for future extension.
    TUint32 iSpare;

    //OWN: Protocol id part
    TUint32 iProtocol;

    //OWN: Session id part
    TUint32 iSession;

    //OWN: Request id part
    TUint32 iRequest;
    };


#endif // XIMPREQUESTIDBUILDER_H

// End of file
