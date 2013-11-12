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

#ifndef CPRESENTITYGROUPLISTEVENTIMP_H
#define CPRESENTITYGROUPLISTEVENTIMP_H

#include "ximpapieventbase.h"
#include "presencetypehelpers.h"
#include "ximpdatasubscriptionstateimp.h"
#include <presentitygrouplistevent.h>

class RReadStream;
class CPresentityGroupInfoImp;
class CXIMPDataSubscriptionStateImp;

/**
 * MOwnPresenceEvent API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresentityGroupListEventImp ): public CXIMPApiEventBase,
                                                       public MPresentityGroupListEvent
    {
public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENTITYGROUPLISTEVENTIMP };

public:
    /**
     * Exported instantiation method for initializing
     * new event object.
     */
    IMPORT_C static CPresentityGroupListEventImp* NewLC(
            RPrGrpInfoImpArray* aCreated,
            RPrGrpInfoImpArray* aUpdated,
            RPrGrpInfoImpArray* aDeleted,
            CXIMPDataSubscriptionStateImp* aState
            );

    /**
     * Exported instantiation method for initializing
     * new event object.
     */
    IMPORT_C static CPresentityGroupListEventImp* NewLC(
            RPrGrpInfoImpArray* aCreated,
            RPrGrpInfoImpArray* aUpdated,
            RPrGrpInfoImpArray* aDeleted,
            RPrGrpInfoImpArray* aCurrent,
            CXIMPDataSubscriptionStateImp* aState
            );
    IMPORT_C static CPresentityGroupListEventImp* NewL(
            RPrGrpInfoImpArray* aCreated,
            RPrGrpInfoImpArray* aUpdated,
            RPrGrpInfoImpArray* aDeleted,
            RPrGrpInfoImpArray* aCurrent,
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

    virtual ~CPresentityGroupListEventImp();


private:
    CPresentityGroupListEventImp();

    void ConstructL(
            RPrGrpInfoImpArray* aCreated,
            RPrGrpInfoImpArray* aUpdated,
            RPrGrpInfoImpArray* aDeleted,
            CXIMPDataSubscriptionStateImp* aState
            );

    void ConstructL(
            RPrGrpInfoImpArray* aCreated,
            RPrGrpInfoImpArray* aUpdated,
            RPrGrpInfoImpArray* aDeleted,
            RPrGrpInfoImpArray* aCurrent,
            CXIMPDataSubscriptionStateImp* aState
            );

    void ConstructL( RReadStream& aStream );

    /**
     * Helper for externalize.
     */
    void WriteArrayToStreamL( RPrGrpInfoImpArray& aArray,
            RWriteStream& aStream ) const;

    /**
     * Helper for internalize
     */
    void FillArrayFromStreamL( RPrGrpInfoImpArray& aArray,
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


public: // From MPresentityGroupList

    TInt NewGroupsCount() const;
    const MPresentityGroupInfo& NewGroup( TInt aIndex ) const;

    TInt UpdatedGroupsCount() const;
    const MPresentityGroupInfo& UpdatedGroup( TInt aIndex ) const;

    TInt DisappearedGroupsCount() const;
    const MPresentityGroupInfo& DisappearedGroup( TInt aIndex ) const;

    TInt CurrentGroupsCount() const;
    const MPresentityGroupInfo& CurrentGroup( TInt aIndex ) const;

    const MXIMPDataSubscriptionState& DataSubscriptionState() const;

private:

    /**
     * Compare the given arrays for equality.
     * Arrays must be sorted. This is a method specific
     * to the group info imp array.
     * @return ETrue if the arrays match, EFalse otherwise.
     */
    static TBool CompareArrays( RPrGrpInfoImpArray* aA, RPrGrpInfoImpArray* aB );

private: // data

    /**
     * ETrue if constructed from stream.
     */
    TBool iConstructedFromStream;

    /**
     * ETrue if current groups array is owned.
     */
    TBool iOwnsCurrentGroupsArray;

    /**
     * Group informations.
     * Owns if constructed from stream. Otherwise does not own.
     */
    RPrGrpInfoImpArray* iCreatedGroups;
    RPrGrpInfoImpArray* iUpdatedGroups;
    RPrGrpInfoImpArray* iDeletedGroups;
    RPrGrpInfoImpArray* iCurrentGroups;

    /**
     *
     */
    CXIMPDataSubscriptionStateImp* iSubscriptionState;
    };


#endif // CPRESENTITYGROUPLISTEVENTIMP_H
