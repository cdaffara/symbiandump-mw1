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
* Description:  MPresenceBlockListEvent API object implementation.
 *
*/

#ifndef CPRESENCEBLOCKLISTEVENTIMP_H
#define CPRESENCEBLOCKLISTEVENTIMP_H

#include "ximpapieventbase.h"
#include "presencetypehelpers.h"
#include <presenceblocklistevent.h>


class RReadStream;
class CPresenceBlockInfoImp;
class CXIMPIdentityImp;
class CXIMPDataSubscriptionStateImp;

/**
 * MPresenceBlockListEvent API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresenceBlockListEventImp ): public CXIMPApiEventBase,
                                            public MPresenceBlockListEvent
    {
public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENCEBLOCKLISTEVENTIMP };

public:
    /**
     * Exported instantiation method for initializing
     * new event object.
     */
    IMPORT_C static CPresenceBlockListEventImp* NewLC(
            RPrBlockInfoImpArray* aAdded,
            RPrBlockInfoImpArray* aPending,
            RPrBlockInfoImpArray* aRemoved,
            RPrBlockInfoImpArray* aUpdated,
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

    virtual ~CPresenceBlockListEventImp();

private:
    CPresenceBlockListEventImp();

    void ConstructL(
            RPrBlockInfoImpArray* aAdded,
            RPrBlockInfoImpArray* aPending,
            RPrBlockInfoImpArray* aRemoved,
            RPrBlockInfoImpArray* aUpdated,
            CXIMPDataSubscriptionStateImp* aState
            );

    void ConstructL( RReadStream& aStream );

    /**
     * Helper for externalize.
     */
    void WriteArrayToStreamL( RPrBlockInfoImpArray& aArray,
            RWriteStream& aStream ) const;


    /**
     * Helper for internalize
     */
    void FillArrayFromStreamL( RPrBlockInfoImpArray& aArray,
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


public: // From MPresenceBlockListEvent

    TInt NewBlocksCount() const;
    const MPresenceBlockInfo& NewBlock( TInt aIndex ) const;


    TInt UpdatedBlocksCount() const;
    const MPresenceBlockInfo& UpdatedBlock( TInt aIndex ) const;


    TInt DisappearedBlocksCount() const;
    const MPresenceBlockInfo& DisappearedBlock( TInt aIndex ) const;


    TInt CurrentBlocksCount() const;
    const MPresenceBlockInfo& CurrentBlock( TInt aIndex ) const;

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
    RPrBlockInfoImpArray* iAdded;
    RPrBlockInfoImpArray* iCurrent;
    RPrBlockInfoImpArray* iRemoved;
    RPrBlockInfoImpArray* iUpdated;

    /**
     * Subscription state
     */
    CXIMPDataSubscriptionStateImp* iSubscriptionState;
    
    /**
     * Current array owner status
     */
    TBool iOwnsCurrent;
    };


#endif // CPRESENCEBLOCKLISTEVENTIMP_H
