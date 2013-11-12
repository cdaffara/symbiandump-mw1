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
* Description:  Implementation for managing cached presence publish information.
*
*/

#ifndef CPROTOCOLPRESENCEPUBLISHINGDATAHOSTIMP_H
#define CPROTOCOLPRESENCEPUBLISHINGDATAHOSTIMP_H

#include <protocolpresencepublishingdatahost.h>

#include "ximpapiobjbase.h"
#include "presenceapiobjbase.h"
#include "ximpitemparent.h"
#include "presencetypehelpers.h"
#include "ximpoperationdefs.h"

class MXIMPHost;
class MPresenceInfo;
class MXIMPObjectCollection;


/**
 * Implementation for managing cached presence publish information.
 *
 * @since S60 v3.2
 */
class CProtocolPresencePublishingDataHostImp : public CXIMPApiObjBase,
                                                public MProtocolPresencePublishingDataHost
    {
public: // Definitions

    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPROTOCOLPRESENCEPUBLISHINGDATAHOST };

public: // Construction and destruction

    /**
     * Construction
     */
    IMPORT_C static CProtocolPresencePublishingDataHostImp* NewL( MXIMPHost& aHost );

    /**
     * Destruction
     */
    virtual ~CProtocolPresencePublishingDataHostImp();

private:

    CProtocolPresencePublishingDataHostImp( MXIMPHost& aHost );
    void ConstructL();

public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS

public: //From MProtocolPresencePublishingDataHost

    // Handling user own presence information
    TXIMPRequestId HandleSubscribedOwnPresenceL( MPresenceInfo* aPresenceInfo );
    const MXIMPDataSubscriptionState& OwnPresenceDataSubscriptionState() const;
    TXIMPRequestId SetOwnPresenceDataSubscriptionStateL( 
                MXIMPDataSubscriptionState* aSubscriptionState, 
                MXIMPStatus* aStatus );    
    

    // Handling presence watcher list content
    TXIMPRequestId HandlePresenceWatcherListL( MXIMPObjectCollection* aWatcherList );
    const MXIMPDataSubscriptionState& PresenceWatcherListDataSubscriptionState() const;
    TXIMPRequestId SetPresenceWatcherListDataSubscriptionStateL(
                MXIMPDataSubscriptionState* aSubscriptionState, 
                MXIMPStatus* aStatus );

private: // Data

    /**
     * Access to host.
     */
    MXIMPHost& iHost;

    };


#endif // CPROTOCOLPRESENCEPUBLISHINGDATAHOSTIMP_H
