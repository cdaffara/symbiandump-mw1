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
* Description:  Implementation for managing cached authorization information.
*
*/

#ifndef CPROTOCOLPRESENCEAUTHORIZATIONDATAHOSTIMP
#define CPROTOCOLPRESENCEAUTHORIZATIONDATAHOSTIMP

#include <protocolpresenceauthorizationdatahost.h>

#include "ximpapiobjbase.h"
#include "presenceapiobjbase.h"
#include "ximpitemparent.h"
#include "presencetypehelpers.h"
#include "ximpoperationdefs.h"

class MXIMPHost;

/**
 * Implementation for managing cached authorization information
 *
 * @since S60 v3.2
 */
class CProtocolPresenceAuthorizationDataHostImp : public CXIMPApiObjBase,
                                                      public MProtocolPresenceAuthorizationDataHost
    {
public: // Definitions

    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPROTOCOLPRESENCEAUTHORIZATIONDATAHOST };

public: // Construction and destruction

    /**
     * Construction
     */
    IMPORT_C static CProtocolPresenceAuthorizationDataHostImp* NewL( MXIMPHost& aHost );

    /**
     * Destruction
     */
    virtual ~CProtocolPresenceAuthorizationDataHostImp();

private:

    CProtocolPresenceAuthorizationDataHostImp( MXIMPHost& aHost );
    void ConstructL();

public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS

public: //From MProtocolPresenceAuthorizationDataHost

    TXIMPRequestId HandlePresenceGrantRequestListL(
                MXIMPObjectCollection* aGrantRequestList );
    TXIMPRequestId HandlePresenceGrantRequestReceivedL(
                MPresenceGrantRequestInfo* aGrantRequest );
    TXIMPRequestId HandlePresenceGrantRequestObsoletedL(
                MXIMPIdentity* aGrantRequestor );
    const MXIMPDataSubscriptionState& PresenceGrantRequestDataSubscriptionState() const;
    TXIMPRequestId SetPresenceGrantRequestDataSubscriptionStateL(
                MXIMPDataSubscriptionState* aSubscriptionState, 
                MXIMPStatus* aStatus );
                
    // Handling presence blocking

    TXIMPRequestId HandlePresenceBlockListL(
                MXIMPObjectCollection* aBlockList );
    TXIMPRequestId HandlePresenceBlockedL(
                MPresenceBlockInfo* aBlockInfo );
    TXIMPRequestId HandlePresenceBlockCanceledL(
                MXIMPIdentity* aBlockedIdentity );
    const MXIMPDataSubscriptionState& PresenceBlockDataSubscriptionState() const;
    TXIMPRequestId SetPresenceBlockDataSubscriptionStateL(
                MXIMPDataSubscriptionState* aSubscriptionState, 
                MXIMPStatus* aStatus );                
                
private: // Data

    /**
     * Access to host.
     */
    MXIMPHost& iHost;
    };


/* ======================================================================== */



#endif // CPROTOCOLPRESENCEAUTHORIZATIONDATAHOSTIMP
