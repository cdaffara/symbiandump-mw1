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
* Description:  MPresenceGrantRequestList API object implementation.
 *
*/

#ifndef CPRESENCEGRANTREQUESTLISTEVENTIMP_H
#define CPRESENCEGRANTREQUESTLISTEVENTIMP_H

#include <presencegrantrequestlistevent.h>
#include "ximpapieventbase.h"
#include "presencetypehelpers.h"

class RReadStream;
class CPresenceGrantRequestInfoImp;
class CXIMPIdentityImp;
class CXIMPDataSubscriptionStateImp;

/**
 * MPresenceGrantRequestList API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresenceGrantRequestListEventImp ): public CXIMPApiEventBase,
                                                            public MPresenceGrantRequestListEvent
    {
public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENCEGRANTREQUESTLISTEVENTIMP };

public:
    /**
     * Exported instantiation method for initializing
     * new event object.
     */
    IMPORT_C static CPresenceGrantRequestListEventImp* NewLC(
            RPrGrntReqInfoImpArray* aNew,
            RPrGrntReqInfoImpArray* aPending,
            RPrGrntReqInfoImpArray* aObsoleted,
            CXIMPDataSubscriptionStateImp* aState
            );

    /**
     * Instantiation method for event automation.
     * Method signature must be exactly this to work
     * with event delivery automation system.
     *
     * Event implementation must be registered to
     * XIMPEventCodec KXIMPEventConstructorTable.
     */
    static CXIMPApiEventBase* NewFromStreamLC( RReadStream& aStream );

    virtual ~CPresenceGrantRequestListEventImp();


private:
    CPresenceGrantRequestListEventImp();

    void ConstructL(
            RPrGrntReqInfoImpArray* aNew,
            RPrGrntReqInfoImpArray* aPending,
            RPrGrntReqInfoImpArray* aObsoleted,
            CXIMPDataSubscriptionStateImp* aState
            );

    void ConstructL( RReadStream& aStream );

    /**
     * Helper for externalize.
     */
    void WriteArrayToStreamL( RPrGrntReqInfoImpArray& aArray,
            RWriteStream& aStream ) const;

    /**
     * Helper for internalize
     */
    void FillArrayFromStreamL( RPrGrntReqInfoImpArray& aArray,
            RReadStream& aStream );

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


public: // From MPresenceGrantRequestListEvent


    TInt NewRequestsCount() const;
    const MPresenceGrantRequestInfo& NewRequest( TInt aIndex ) const;

    TInt DisappearedRequestsCount() const;
    const MPresenceGrantRequestInfo& DisappearedRequest( TInt aIndex ) const;

    TInt CurrentRequestsCount() const;
    const MPresenceGrantRequestInfo& CurrentRequest( TInt aIndex ) const;
    
    const MXIMPDataSubscriptionState& DataSubscriptionState() const;

private: // data

    /**
     * ETrue if constructed from stream.
     */
    TBool iConstructedFromStream;

    /**
     * Group informations.
     * Owns if constructed from stream. Otherwise does not own.
     */
    RPrGrntReqInfoImpArray* iNewRequests;
    RPrGrntReqInfoImpArray* iPending;
    RPrGrntReqInfoImpArray* iObsoleted;

    /**
     * Pending array own status
     */
    TBool iOwnsPending;

    /**
     * Subscription state
     */
    CXIMPDataSubscriptionStateImp* iSubscriptionState;
    };


#endif // CPRESENCEGRANTREQUESTLISTEVENTIMP_H
