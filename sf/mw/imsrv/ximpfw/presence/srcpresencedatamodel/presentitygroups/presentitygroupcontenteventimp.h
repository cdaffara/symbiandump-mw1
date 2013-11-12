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
* Description:  MPresentityGroupContentEvent API object implementation.
 *
*/

#ifndef CPRESENTITYGROUPCONTENTEVENTIMP_H
#define CPRESENTITYGROUPCONTENTEVENTIMP_H


#include "ximpapieventbase.h"
#include "presencetypehelpers.h"
#include "ximpdatasubscriptionstateimp.h"
#include <presentitygroupcontentevent.h>


class RReadStream;
class CXIMPIdentityImp;
class CPresentityGroupInfoImp;
class CXIMPDataSubscriptionStateImp;

/**
 * MOwnPresenceEvent API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresentityGroupContentEventImp ): public CXIMPApiEventBase,
                                                       public MPresentityGroupContentEvent
    {
public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENTITYGROUPCONTENTEVENTIMP };

public:

    /**
     * Exported instantiation method for initializing
     * new event object.
     */
    IMPORT_C static CPresentityGroupContentEventImp* NewLC(
            CXIMPIdentityImp& aGroupId,
            RPrGrpMemInfoImpArray* aAdded,
            RPrGrpMemInfoImpArray* aUpdated,
            RPrGrpMemInfoImpArray* aRemoved,
            CXIMPDataSubscriptionStateImp* aState
            );

    IMPORT_C static CPresentityGroupContentEventImp* NewLC(
            CXIMPIdentityImp& aGroupId,
            RPrGrpMemInfoImpArray* aAdded,
            RPrGrpMemInfoImpArray* aUpdated,
            RPrGrpMemInfoImpArray* aRemoved,
            RPrGrpMemInfoImpArray* aCurrent,
            CXIMPDataSubscriptionStateImp* aState
            );

    IMPORT_C static CPresentityGroupContentEventImp* NewL(
            CXIMPIdentityImp& aGroupId,
            RPrGrpMemInfoImpArray* aAdded,
            RPrGrpMemInfoImpArray* aUpdated,
            RPrGrpMemInfoImpArray* aRemoved,
            RPrGrpMemInfoImpArray* aCurrent,
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

    virtual ~CPresentityGroupContentEventImp();


private:
    CPresentityGroupContentEventImp();

    void ConstructL( CXIMPIdentityImp& aGroupId,
            RPrGrpMemInfoImpArray* aAdded,
            RPrGrpMemInfoImpArray* aUpdated,
            RPrGrpMemInfoImpArray* aRemoved,
            CXIMPDataSubscriptionStateImp* aState
            );

    void ConstructL( CXIMPIdentityImp& aGroupId,
            RPrGrpMemInfoImpArray* aAdded,
            RPrGrpMemInfoImpArray* aUpdated,
            RPrGrpMemInfoImpArray* aRemoved,
            RPrGrpMemInfoImpArray* aCurrent,
            CXIMPDataSubscriptionStateImp* aState );

    void ConstructL( RReadStream& aStream );

    /**
     * Helper for externalize.
     */
    void WriteArrayToStreamL( RPrGrpMemInfoImpArray& aArray,
            RWriteStream& aStream ) const;

    /**
     * Helper for internalize
     */
    void FillArrayFromStreamL( RPrGrpMemInfoImpArray& aArray,
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


public: // From MPresentityGroupContentEvent

    const MXIMPIdentity& GroupId() const;

    TInt NewMembersCount() const;
    const MPresentityGroupMemberInfo& NewMember( TInt aIndex ) const;

    TInt UpdatedMembersCount() const;
    const MPresentityGroupMemberInfo& UpdatedMember( TInt aIndex ) const;

    TInt DisappearedMembersCount() const;
    const MPresentityGroupMemberInfo& DisappearedMember( TInt aIndex ) const;

    TInt CurrentMembersCount() const;
    const MPresentityGroupMemberInfo& CurrentMember( TInt aIndex ) const;

    const MXIMPDataSubscriptionState& DataSubscriptionState() const;

private:

    /**
     * Compare the given arrays for equality.
     * Arrays must be sorted. This is a method specific
     * to the group info imp array.
     * @return ETrue if the arrays match, EFalse otherwise.
     */
    static TBool CompareArrays( RPrGrpMemInfoImpArray* aA, RPrGrpMemInfoImpArray* aB );

private: // data

    /**
     * ETrue if constructed from stream.
     */
    TBool iConstructedFromStream;

    /**
     * Kludge to fix current members ownership issues.
     */
    TBool iOwnsCurrentMembersArray;

    /**
     * Group id
     *
     */
    CXIMPIdentityImp* iGroupId;

    /**
     * Group member informations.
     * Owns if constructed from stream. Otherwise does not own.
     * Might own current members in some cases, in which
     * iOwnsCurrentMembersArray
     * will be set.
     */
    RPrGrpMemInfoImpArray* iAddedMembers;
    RPrGrpMemInfoImpArray* iUpdatedMembers;
    RPrGrpMemInfoImpArray* iRemovedMembers;
    RPrGrpMemInfoImpArray* iCurrentMembers;


    /**
     *
     */
    CXIMPDataSubscriptionStateImp* iSubscriptionState;
    };


#endif // CPRESENTITYGROUPCONTENTEVENTIMP_H
