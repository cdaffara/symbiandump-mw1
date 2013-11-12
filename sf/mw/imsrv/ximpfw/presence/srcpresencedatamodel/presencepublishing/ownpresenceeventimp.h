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
* Description:  MOwnPresenceEvent API object implementation.
 *
*/

#ifndef COWNPRESENCEEVENTIMP_H
#define COWNPRESENCEEVENTIMP_H

#include <ownpresenceevent.h>
#include "ximpapieventbase.h"
#include "presenceinfo.h"
#include "ximpdatasubscriptionstateimp.h"
#include "presenceapiobjbase.h"


class RReadStream;
class CPresenceInfoImp;
class CXIMPDataSubscriptionStateImp;

/**
 * MOwnPresenceEvent API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( COwnPresenceEventImp ): public CXIMPApiEventBase,
                                               public MOwnPresenceEvent
    {
public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_COWNPRESENCEEVENTIMP };

public:
    /**
     * Exported instantiation method for initializing
     * new event object.
     */
    IMPORT_C static COwnPresenceEventImp* NewLC(
        CXIMPDataSubscriptionStateImp* aState );
    IMPORT_C static COwnPresenceEventImp* NewL(
        CXIMPDataSubscriptionStateImp* aState );


    /**
     * Instantiation method for event automation.
     * Method signature must be exactly this to work
     * with event delivery automation system.
     *
     * Event implementation must be registered to
     * XIMPEventCodec KXIMPEventConstructorTable.
     */
    static CXIMPApiEventBase* NewFromStreamLC( RReadStream& aStream );

    virtual ~COwnPresenceEventImp();


private:
    COwnPresenceEventImp();
    void ConstructL( CXIMPDataSubscriptionStateImp* aState );
    void ConstructL( RReadStream& aStream );

public: // From API base interfaces

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS


    /**
     * Implementation of MXIMPEventBase interface and
     * CXIMPApiEventBase methods
     *
     * @see MXIMPEventBase
     * @see CXIMPApiEventBase
     */
    XIMPIMP_DECLARE_EVENT_BASE_METHODS

    void InternalizeL( RReadStream& aStream );


public: // From MOwnPresenceEvent

    /**
     * Implementation of MOwnPresenceEvent interface methods
     * @see MOwnPresenceEvent
     */
    const MPresenceInfo& SubscribedPresence() const;
    const MXIMPDataSubscriptionState& DataSubscriptionState() const;

public:  // new methods

    /**
     * Set new presence info.
     * Ownership transferred.
     * @param aInfoImp The info imp
     */
    void SetPresenceInfo( CPresenceInfoImp* aInfoImp );

private: // data

    /**
     * Presence Document
     * Own.
     */
     CPresenceInfoImp* iPresenceInfo;

    /**
     *
     */
    CXIMPDataSubscriptionStateImp* iSubscriptionState;
    };


#endif // COWNPRESENCEEVENTIMP_H
