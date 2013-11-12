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
* Description:  Interface for presentity group member info object.
*
*/

#ifndef MXIMPFWPRESENTITYGROUPMEMBERINFO_H
#define MXIMPFWPRESENTITYGROUPMEMBERINFO_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencedatamodelifids.hrh>


class MXIMPIdentity;


/**
 * Interface for presentity group member info object.
 *
 * PresentityGroupMemberInfo presents single
 * presentity group member. Object holds the identity
 * and display name for the presentity group member.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MPresentityGroupMemberInfo : public MXIMPBase
    {
public:

    /** Interface ID for the MPresentityGroupMemberInfo. */
    enum { KInterfaceId = PRES_IF_ID_PRESENTITY_GROUP_MEMBER_INFO };


public:


    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MPresentityGroupMemberInfo() {}



public:


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


#endif // MXIMPFWPRESENTITYGROUPMEMBERINFO_H



