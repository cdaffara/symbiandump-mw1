/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface for group info object.
*
*/

#ifndef MIMGROUPINFO_H
#define MIMGROUPINFO_H

#include <e32std.h>
#include <ximpbase.h>
#include <imdatamodelifids.hrh>


class MXIMPIdentity;


/**
 * Interface for group info object.
 *
 * Group info object presents single group and the group members
 * in the group list.
 * Group info object holds the identity,
 * display name and members for the group.
 *
 * @ingroup Imdatamodelapi
 * 
 */
class MImGroupInfo : public MXIMPBase
    {
public:

    /** Interface ID for the MImGroupInfo. */
    enum { KInterfaceId = IM_IF_ID_GROUP_INFO };


public:


    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MImGroupInfo() {}



public:

    /**
     * Returns group identity value.
     *
     * @return The group identity value.
     */
    virtual const MXIMPIdentity& GroupId() const = 0;



    /**
     * Returns group display name value.
     *
     * @return The group display name value.
     */
    virtual const TDesC16& GroupDisplayName() const = 0;


    /**
     * Returns group member identity value.
     *
     * @return The group member identity value.
     */
    virtual const MXIMPIdentity& GroupMemberId() const = 0;


    /**
     * Returns group member display name value.
     *
     * @return The group member display name value.
     */
    virtual const TDesC16& GroupMemberDisplayName() const = 0;


    /**
     * Sets group identity value.
     *
     * @param [in] aIdentity
     *        New group identity value.
     *        Ownership of the argument is transfered to
     *        callee, if method succeeds without a leave.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument were provided.
     */
    virtual void SetGroupIdL(
                MXIMPIdentity* aIdentity ) = 0;



    /**
     * Sets group display name value.
     *
     * @param [in] aDisplayName
     *        New display name value.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual void SetGroupDisplayNameL(
                const TDesC16& aDisplayName ) = 0;


    /**
     * Sets group member identity value.
     *
     * @param [in] aIdentity
     *        New group member identity value.
     *        Ownership of the argument is transfered to
     *        callee, if method succeeds without a leave.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument were provided.
     */
    virtual void SetGroupMemberIdL(
                MXIMPIdentity* aIdentity ) = 0;


    /**
     * Sets group member display name value.
     *
     * @param [in] aDisplayName
     *        New display name value.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual void SetGroupMemberDisplayNameL(
                const TDesC16& aDisplayName ) = 0;



    };


#endif // MIMGROUPINFO_H



