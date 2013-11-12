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
* Description:  Implementation for managing cached subscribed presence information.
*
*/

#ifndef CPROTOCOLPRESENCEWATCHINGDATAHOSTIMP_H
#define CPROTOCOLPRESENCEWATCHINGDATAHOSTIMP_H

#include <protocolpresencewatchingdatahost.h>

#include "ximpapiobjbase.h"
#include "presenceapiobjbase.h"
#include "ximpitemparent.h"
#include "presencetypehelpers.h"
#include "ximpoperationdefs.h"

class MXIMPHost;
class MPresenceInfo;
class MXIMPObjectCollection;

/**
 * Implementation for managing cached subscribed presence information.
 *
 * @since S60 v3.2
 */
class CProtocolPresenceWatchingDataHostImp : public CXIMPApiObjBase,
                                                public MProtocolPresenceWatchingDataHost
    {
public: // Definitions

    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPROTOCOLPRESENCEWATCHINGDATAHOST };

public: // Construction and destruction

    /**
     * Construction
     */
    IMPORT_C static CProtocolPresenceWatchingDataHostImp* NewL( MXIMPHost& aHost );

    /**
     * Destruction
     */
    virtual ~CProtocolPresenceWatchingDataHostImp();

private:

    CProtocolPresenceWatchingDataHostImp( MXIMPHost& aHost );
    void ConstructL();

public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS

public: //From MProtocolPresenceWatchingDataHost

    TXIMPRequestId HandleSubscribedPresentityPresenceL(
                MXIMPIdentity* aPresentityId,
                MPresenceInfo* aPresenceInfo );
    const MXIMPDataSubscriptionState& PresentityPresenceDataSubscriptionState( 
                const MXIMPIdentity& aPresentityId ) const;
    TXIMPRequestId SetPresentityPresenceDataSubscriptionStateL( 
                MXIMPIdentity* aPresentityId, 
                MXIMPDataSubscriptionState* aSubscriptionState, 
                MXIMPStatus* aStatus );
private: // Data

    /**
     * Access to host.
     */
    MXIMPHost& iHost;

    };


#endif // CPROTOCOLPRESENCEWATCHINGDATAHOSTIMP_H
