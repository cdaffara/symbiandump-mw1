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
* Description:  Interface for invitation info object.
*
*/

#ifndef MIMINVITATIONINFO_H
#define MIMINVITATIONINFO_H

#include <e32std.h>
#include <ximpbase.h>
#include <imdatamodelifids.hrh>


class MXIMPIdentity;


/**
 * Interface for invitation info object.
 *
 * Invitation info object presents single invitation and the member
 * in the invitation list. Invitation info object holds the identity,
 * display name, response and the member.
 *
 * @ingroup Imdatamodelapi
 * 
 */
class MImInvitationInfo : public MXIMPBase
    {
public:

    /** Interface ID for the MImInvitationInfo. */
    enum { KInterfaceId = IM_IF_ID_INVITATION_INFO };


public:


    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MImInvitationInfo() {}



public:

    /**
     * Returns invitation identity value.
     *
     * @return The invitation identity value.
     */
    virtual const MXIMPIdentity& InvitationID() const = 0;


    /**
     * Returns inviting user ID value.
     *
     * @return The inviting user ID value.
     */
    virtual const TDesC16& UserID() const = 0;


    /**
     * Returns group name value.
     *
     * @return The group name value.
     */
    virtual const TDesC16& GroupName() const = 0;


    /**
     * Returns screen name value.
     *
     * @return The screen name value.
     */
    virtual const TDesC16& ScreenName() const = 0;
 

   /**
     * Returns acceptance of the invitation.
     *
     * @return The acceptance of the invitation.
     */
    virtual const TBool Accept() const = 0;


   /**
     * Returns response/description of acceptance.
     *
     * @return The response/description of acceptance.
     */
    virtual const TDesC16& Response() const = 0;
        
    

    /**
     * Sets invitation identity value.
     *
     * @param [in] aIdentity
     *        New invitation identity value.
     *        Ownership of the argument is transfered to
     *        callee, if method succeeds without a leave.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument were provided.
     */
    virtual void SetInvitationIdL(
                MXIMPIdentity* aIdentity ) = 0;



    /**
     * Sets inviting user ID value.
     *
     * @param [in] aUser
     *        New User ID value.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual void SetUserIDL(
                const TDesC16& aUser ) = 0;


    /**
     * Sets Group name value.
     *
     * @param [in] aGroup
     *        Group name value.
     *        Ownership of the argument is transfered to
     *        callee, if method succeeds without a leave.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument were provided.
     */
    virtual void SetGroupNameL(
                const TDesC16& aGroup ) = 0;


    /**
     * Sets screen name value.
     *
     * @param [in] aScreen
     *        screen name value.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual void SetScreenNameL(
                const TDesC16& aScreen ) = 0;


    /**
     * Sets acceptance value.
     *
     * @param [in] aAccept
     *        Acceptance value.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
	virtual void SetAcceptL(
				const TBool aAccept ) = 0;
	
	
    /**
     * Sets response value.
     *
     * @param [in] aResponse
     *        response value.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
	virtual void SetResponseL(
				const TDesC16& aResponse ) = 0;


    };


#endif // MImINVITATIONINFO_H



