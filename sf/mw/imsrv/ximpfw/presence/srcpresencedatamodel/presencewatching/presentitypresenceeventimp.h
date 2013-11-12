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
* Description:  MPresentityPresenceEvent API object implementation.
 *
*/

#ifndef CPRESENTIYPRESENCEEVENT_H
#define CPRESENTIYPRESENCEEVENT_H

#include "ximpapieventbase.h"
#include <presentitypresenceevent.h>
#include "ximpdatasubscriptionstateimp.h"
#include "presenceinfo.h"
#include "presenceapiobjbase.h"

class RReadStream;
class CXIMPIdentityImp;
class CPresenceInfoImp;
class CXIMPDataSubscriptionStateImp;


/**
 * MPresentityPresenceEvent API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresentityPresenceEventImp ): public CXIMPApiEventBase,
                                                      public MPresentityPresenceEvent
    {
public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENTITYPRESENCEEVENTIMP };

public:
    /**
     * Exported instantiation method for initializing
     * new event object.
     */
    IMPORT_C static CPresentityPresenceEventImp* NewLC(
            CXIMPDataSubscriptionStateImp* aState );
    IMPORT_C static CPresentityPresenceEventImp* NewL(
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

    virtual ~CPresentityPresenceEventImp();


private:
    CPresentityPresenceEventImp();
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


public: // From MPresentityPresenceEvent

    /**
     * Implementation of MPresentityPresenceEvent interface methods
     * @see MPresentityPresenceEvent
     */
    const MXIMPIdentity& PresentityId() const;
    const MPresenceInfo& SubscribedPresence() const;
    const MXIMPDataSubscriptionState& DataSubscriptionState() const;

public:  // new methods

    /**
     * Set new presence info.
     * Ownership transferred.
     * @param aInfoImp The info imp
     */
    void SetPresenceInfo( CPresenceInfoImp* aInfoImp );

    /**
     * Set identity. 
     * Copy is made.
     * @param aIdentity The identity
     */
    void SetIdentityL( const CXIMPIdentityImp& aIdentity );

private: // data

    /**
     * Presentity
     * Own.
     */
    CXIMPIdentityImp* iPresentityIdentity;


    /**
     *
     */
    CXIMPDataSubscriptionStateImp* iSubscriptionState;


    /**
     * Presence Info
     * Own.
     */
     CPresenceInfoImp* iPresenceInfo;


    };


#endif // CPRESENTIYPRESENCEEVENT_H
