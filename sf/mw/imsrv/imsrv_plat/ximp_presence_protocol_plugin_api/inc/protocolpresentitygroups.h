/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface for presence protocol presentity groups object.
*
*/

#ifndef MXIMPFWPROTOCOLPRESENTITYGROUPS_H
#define MXIMPFWPROTOCOLPRESENTITYGROUPS_H


#include <e32std.h>
#include <badesca.h>
#include <presenceprotocolpluginifids.hrh>
#include <ximpbase.h>


class MXIMPIdentity;



/**
 * Interface for presence protocol presentity groups object.
 * Interface must be implemented by a XIMP Framework protocol
 * plug-ins.
 *
 * This interface defines actions that XIMP Framework
 * requests from a presence protocol connection to
 * execute presentity groups related tasks.
 *
 * Protocol implementation must route requests received through
 * this interface, to same remote presence service destination,
 * as where the parent MXIMPProtocolConnection is connected.
 * (Here the parent MXIMPProtocolConnection means the protocol
 * connection instance from where this interface instance
 * was retrieved.)
 *
 * @see MXIMPProtocolConnection
 * @ingroup ximppresprotocolpluginapi
 * @since S60 v3.2
 */
class MProtocolPresentityGroups : public MXIMPBase
    {
public:

    /** Interface ID for the MProtocolPresentityGroups. */
    enum { KInterfaceId = PRES_IF_ID_PROTOCOL_PRESENTITY_GROUPS };


protected:

    /**
     * Protected destructor. MProtocolPresentityGroups
     * instancies can't be destroyed via this interface.
     */
    virtual inline ~MProtocolPresentityGroups() {};



public: // Subscribe presentity group list content


    /**
     * Requests the protocol to subscribe presentity
     * group list content from remote service.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoSubscribePresentityGroupListL(
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to terminate presentity
     * group list content subscription.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoUnsubscribePresentityGroupListL(
                    TXIMPRequestId aReqId ) = 0;



public: // Manage presentity groups


    /**
     * Requests the protocol to create new presentity
     * group to remote service.
     *
     * @param [in] aGroupId
     *        Identification of the presentity group to create.
     *
     * @param [in] aDisplayName
     *        Display name for the created presentity group.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoCreatePresentityGroupL(
                    const MXIMPIdentity& aGroupId,
                    const TDesC16& aDisplayName,
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to delete existing presentity
     * group from remote service.
     *
     * @param [in] aGroupId
     *        Identification of the presentity group to delete.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoDeletePresentityGroupL(
                    const MXIMPIdentity& aGroupId,
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to update existing presentity
     * group display name on remote service.
     *
     * @param [in] aGroupId
     *        Identification of the presentity group which
     *        display name to update.
     *
     * @param [in] aDisplayName
     *        New display name for the presentity group.

     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoUpdatePresentityGroupDisplayNameL(
                    const MXIMPIdentity& aGroupId,
                    const TDesC16& aDisplayName,
                    TXIMPRequestId aReqId ) = 0;



public: // Subscribe presentity group content

    /**
     * Requests the protocol to subscribe presentity
     * group content from remote service.
     *
     * @param [in] aGroupId
     *        Identification of the presentity group of which
     *        content to subscribe.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoSubscribePresentityGroupContentL(
                    const MXIMPIdentity& aGroupId,
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to terminate presentity
     * group content subscription.
     *
     * @param [in] aGroupId
     *        Identification of the presentity group of which
     *        content subscribtion to terminate.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoUnsubscribePresentityGroupContentL(
                    const MXIMPIdentity& aGroupId,
                    TXIMPRequestId aReqId ) = 0;




public: // Manage presentity group members


    /**
     * Requests the protocol to add new member to
     * identified presentity group.
     *
     * @param [in] aGroupId
     *        Identification of the presentity group where to
     *        add new member.
     *
     * @param [in] aMemberId
     *        Identification of the new member.
     *
     * @param [in] aMemberDisplayName
     *        Display name for the added member.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoAddPresentityGroupMemberL(
                    const MXIMPIdentity& aGroupId,
                    const MXIMPIdentity& aMemberId,
                    const TDesC16& aMemberDisplayName,
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to remove member from
     * identified presentity group.
     *
     * @param [in] aGroupId
     *        Identification of the presentity group from
     *        where to remove member.
     *
     * @param [in] aMemberId
     *        Identification of the member to remove.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoRemovePresentityGroupMemberL(
                    const MXIMPIdentity& aGroupId,
                    const MXIMPIdentity& aMemberId,
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to update presentity group
     * member display name.
     *
     * @param [in] aGroupId
     *        Identification of the presentity group of which
     *        member display name to update.
     *
     * @param [in] aMemberId
     *        Identification of the member which display
     *        name to update.
     *
     * @param [in] aMemberDisplayName
     *        New display name for the member.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoUpdatePresentityGroupMemberDisplayNameL(
                    const MXIMPIdentity& aGroupId,
                    const MXIMPIdentity& aMemberId,
                    const TDesC16& aMemberDisplayName,
                    TXIMPRequestId aReqId ) = 0;

    };



#endif // MXIMPFWPROTOCOLPRESENTITYGROUPS_H
