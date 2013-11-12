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
* Description:  Interface for presentity group info object.
*
*/

#ifndef MXIMPFWPRESENTITYGROUPINFO_H
#define MXIMPFWPRESENTITYGROUPINFO_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencedatamodelifids.hrh>


class MXIMPIdentity;


/**
 * Interface for presentity group info object.
 *
 * Presentity group info object presents single presentity group
 * in the presentity group list. Presentity group info object
 * holds the identity and display name for the presentity group.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MPresentityGroupInfo : public MXIMPBase
    {
public:

    /** Interface ID for the MPresentityGroupInfo. */
    enum { KInterfaceId = PRES_IF_ID_PRESENTITY_GROUP_INFO };


public:


    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MPresentityGroupInfo() {}



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

    };


#endif // MXIMPFWPRESENTITYGROUPINFO_H



