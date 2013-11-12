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
* Description:  Implementation for managing cached presentity group information.
*
*/

#ifndef CPROTOCOLPRESENTITYGROUPSDATAHOSTIMP
#define CPROTOCOLPRESENTITYGROUPSDATAHOSTIMP

#include <protocolpresentitygroupsdatahost.h>

#include "ximpapiobjbase.h"
#include "presenceapiobjbase.h"
#include "ximpitemparent.h"
#include "presencetypehelpers.h"
#include "ximpoperationdefs.h"

class MXIMPHost;
class COwnPresenceSubscriptionItem;
class CPresentityPresenceSubscriptionItem;
class CGroupContentSubscriptionItem;
class CWatcherListSubscriptionItem;
class MPresentityGroupInfo;
class CXIMPIdentityImp;
class MXIMPContext;
class CXIMPSubscriptionItemBase;
class MPresenceWatcherInfo;
class MXIMPStatus;
class MPresenceBlockInfo;
class CBlockListSubscriptionItem;

/**
 * Implementation for managing cached presentity group information
 *
 * @since S60 v3.2
 */
class CProtocolPresentityGroupsDataHostImp : public CXIMPApiObjBase,
                                                 public MProtocolPresentityGroupsDataHost
    {
public: // Definitions

    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPROTOCOLPRESENTITYGROUPSDATAHOST };

public: // Construction and destruction

    /**
     * Construction
     */
    IMPORT_C static CProtocolPresentityGroupsDataHostImp* NewL( MXIMPHost& aHost );

    /**
     * Destruction
     */
    virtual ~CProtocolPresentityGroupsDataHostImp();

private:

    CProtocolPresentityGroupsDataHostImp( MXIMPHost& aHost );
    void ConstructL();

public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS

public: //From MProtocolPresentityGroupsDataHost

    // Handling presentity groups
    TXIMPRequestId HandlePresentityGroupListL(
                MXIMPObjectCollection* aGroupList );
    TXIMPRequestId HandlePresentityGroupCreatedL(
                MPresentityGroupInfo* aGroupInfo );
    TXIMPRequestId HandlePresentityGroupDeletedL(
                MXIMPIdentity* aGroupId );
    TXIMPRequestId HandlePresentityGroupDisplayNameUpdatedL(
                MPresentityGroupInfo* aGroupInfo );
    const MXIMPDataSubscriptionState& PresentityGroupListDataSubscriptionState() const;
    TXIMPRequestId SetPresentityGroupListDataSubscriptionStateL( 
                MXIMPDataSubscriptionState* aSubscriptionState, 
                MXIMPStatus* aStatus );
                
    // Handling presentity group content

    TXIMPRequestId HandlePresentityGroupContentL(
                MXIMPIdentity* aGroupId,
                MXIMPObjectCollection* aGroupMembers );
    TXIMPRequestId HandlePresentityGroupMemberAddedL(
                MXIMPIdentity* aGroupId,
                MPresentityGroupMemberInfo* aGroupMember );
    TXIMPRequestId HandlePresentityGroupMemberRemovedL(
                MXIMPIdentity* aGroupId,
                MXIMPIdentity* aGroupMemberId );
    TXIMPRequestId HandlePresentityGroupMemberDisplayNameUpdatedL(
                MXIMPIdentity* aGroupId,
                MPresentityGroupMemberInfo* aGroupMember );
    const MXIMPDataSubscriptionState& PresentityGroupContentDataSubscriptionState( 
                const MXIMPIdentity& aGroupId ) const;
    TXIMPRequestId SetPresentityGroupContentDataSubscriptionStateL( 
                MXIMPIdentity* aGroupId, 
                MXIMPDataSubscriptionState* aSubscriptionState, 
                MXIMPStatus* aStatus );                

private: // Data

    /**
     * Access to host.
     */
    MXIMPHost& iHost;
    };


/* ======================================================================== */



#endif // CPROTOCOLPRESENTITYGROUPSDATAHOSTIMP
