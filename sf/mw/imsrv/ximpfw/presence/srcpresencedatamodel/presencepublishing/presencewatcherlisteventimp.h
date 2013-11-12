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
* Description:  MPresentityGroupList API object implementation.
 *
*/

#ifndef CPRESENCEWATCHERLISTEVENTIMP_H
#define CPRESENCEWATCHERLISTEVENTIMP_H

#include <presencewatcherlistevent.h>
#include "ximpapieventbase.h"
#include "presencetypehelpers.h"

class RReadStream;
class CPresenceWatcherInfoImp;
class CXIMPIdentityImp;
class CXIMPDataSubscriptionStateImp;

/**
 * MOwnPresenceEvent API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresenceWatcherListEventImp ): public CXIMPApiEventBase,
                                                       public MPresenceWatcherListEvent
    {
public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENCEWATCHERLISTEVENTIMP };

public:
    /**
     * Exported instantiation method for initializing
     * new event object.
     */
    IMPORT_C static CPresenceWatcherListEventImp* NewLC(
            RPrWatLstInfoImpArray* aNew,
            RPrWatLstInfoImpArray* aCurrent,
            RPrWatLstInfoImpArray* aDisappeared,
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

    virtual ~CPresenceWatcherListEventImp();

private:
    CPresenceWatcherListEventImp();

    void ConstructL(
            RPrWatLstInfoImpArray* aNew,
            RPrWatLstInfoImpArray* aCurrent,
            RPrWatLstInfoImpArray* aDisappeared,
            CXIMPDataSubscriptionStateImp* aState
            );

    void ConstructL( RReadStream& aStream );

    /**
     * Helper for externalize.
     */
    void WriteArrayToStreamL( RPrWatLstInfoImpArray& aArray,
            RWriteStream& aStream ) const;

    /**
     * Helper for internalize
     */
    void FillArrayFromStreamL( RPrWatLstInfoImpArray& aArray,
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


public: // From MPresenceWatcherListEvent

    TInt NewWatchersCount() const;
    const MPresenceWatcherInfo& NewWatcher( TInt aIndex ) const;

    TInt DisappearedWatchersCount() const;
    const MPresenceWatcherInfo& DisappearedWatcher( TInt aIndex ) const;

    TInt CurrentWatchersCount() const;
    const MPresenceWatcherInfo& CurrentWatcher( TInt aIndex ) const;


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
    RPrWatLstInfoImpArray* iNewWatchers;
    RPrWatLstInfoImpArray* iCurrent;
    RPrWatLstInfoImpArray* iDisappeared;

    /**
     * Current array owning status
     */
    TBool iOwnsCurrent;

    /**    
     * Subscription state
     */
    CXIMPDataSubscriptionStateImp* iSubscriptionState;
    };


#endif // CPRESENCEWATCHERLISTEVENTIMP_H
