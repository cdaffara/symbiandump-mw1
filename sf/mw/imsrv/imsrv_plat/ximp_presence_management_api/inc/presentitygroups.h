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
* Description:  Presentity group handling interface for clients use.
*
*/

#ifndef MXIMPFWPRESENTITYGROUPS_H
#define MXIMPFWPRESENTITYGROUPS_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencemanagementifids.hrh>

class MXIMPIdentity;


/**
 * Presentity group handling interface for clients use.
 * Interface is implemented by the XIMP Framework.
 *
 * MPresentityGroups provides services for
 * XIMP Framework clients to manage presentity groups
 * and presentity group members. It provides also services
 * to subscribe notifications from presentity group list changes
 * and from individual presentity group changes.
 *
 * Client can retrieve MPresentityGroups interface from
 * MXIMPContext interface.
 *
 * @ingroup ximppresmanagementapi
 * @since S60 v3.2
 */
class MPresentityGroups : public MXIMPBase
    {
public:

    /** Interface ID for the MPresentityGroups. */
    enum { KInterfaceId = PRES_IF_ID_PRESENTITY_GROUPS };


protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MPresentityGroups() {}


public: //Presentity group management requests


    /**
     * Subscribes presentity group list notifications.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId SubscribePresentityGroupListL() = 0;



    /**
     * Unsubscribes presentity group list notifications.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId UnsubscribePresentityGroupListL() = 0;



    /**
     * Creates presentity group.
     *
     * Note: Remote service may alter given presentity group ID and
     *       display name. Altered ID and display names are signalled back
     *       to client through MXIMPRequestCompleteEvent.
     *
     * @param [in] aGroupId
     *        Identification for presentity group.
     *
     * @param [in] aDisplayName
     *        Presentity group display name.
     *
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId CreatePresentityGroupL(
                    const MXIMPIdentity& aGroupId,
                    const TDesC16& aDisplayName ) = 0;


    /**
     * Deletes presentity group.
     *
     * @param [in] aGroupId
     *        Identification for presentity group.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId DeletePresentityGroupL(
                    const MXIMPIdentity& aGroupId ) = 0;



    /**
     * Updates presentity group display name.
     *
     * Note: Remote service may alter given presentity group display name.
     *       Altered display name is signalled back to client through
     *       MXIMPRequestCompleteEvent.
     *
     * @param [in] aGroupId
     *        Identification for presentity group.
     *
     * @param [in] aDisplayName
     *        New display name for the presentity group.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId UpdatePresentityGroupDisplayNameL(
                    const MXIMPIdentity& aGroupId,
                    const TDesC16& aDisplayName ) = 0;



public: //Prototypes for presentity group member management requests


    /**
     * Subscribes presentity group content notifications.
     *
     * @param [in] aGroupId
     *        Identification for presentity group for which
     *        to subscribe content notifications.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId SubscribePresentityGroupContentL(
                    const MXIMPIdentity& aGroupId ) = 0;


    /**
     * Unsubscribes presentity group content notifications.
     *
     * @param [in] aGroupId
     *        Identification for presentity group for which
     *        content notification subscription to terminate.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId UnsubscribePresentityGroupContentL(
                    const MXIMPIdentity& aGroupId ) = 0;


    /**
     * Requests the protocol to add new member to
     * identified presentity group.
     *
     *
     * Note: Remote service may alter given member and
     *       display name. Altered ID and display names
     *       are signalled back to client through MXIMPRequestCompleteEvent.
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
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId AddPresentityGroupMemberL(
                    const MXIMPIdentity& aGroupId,
                    const MXIMPIdentity& aMemberId,
                    const TDesC16& aMemberDisplayName ) = 0;


    /**
     * Requests the XIMPFw to remove member from
     * identified presentity group.
     *
     * @param [in] aGroupId
     *        Identification of the presentity group from
     *        where to remove member.
     *
     * @param [in] aMemberId
     *        Identification of the member to remove.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId RemovePresentityGroupMemberL(
                    const MXIMPIdentity& aGroupId,
                    const MXIMPIdentity& aMemberId ) = 0;



    /**
     * Requests the XIMPFw to update presentity group
     * member display name.
     *
     * Note: Remote service may alter given presentity group member
     *       display name. Altered display name is signalled back
     *       to client through MXIMPRequestCompleteEvent.
     *
     * @param [in] aGroupId
     *        Identification for presentity group.
     *
     * @param [in] aMemberId
     *        Identification of the member which display
     *        name to update.
     *
     * @param [in] aMemberDisplayName
     *        New display name for the member.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId UpdatePresentityGroupMemberDisplayNameL(
                    const MXIMPIdentity& aGroupId,
                    const MXIMPIdentity& aMemberId,
                    const TDesC16& aMemberDisplayName ) = 0;

    };



#endif // MXIMPFWPRESENTITYGROUPS_H



