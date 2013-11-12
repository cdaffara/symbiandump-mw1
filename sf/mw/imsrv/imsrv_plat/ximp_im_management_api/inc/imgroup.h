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
* Description:  IM group handling interface for clients use.
*
*/

#ifndef MIMGROUPS_H
#define MIMGROUPS_H


#include <ximpbase.h>
#include <ximpidentity.h>
#include  "imaccesscontrollist.h"
#include  "imaccesscontrollistimp.h"
#include  <bamdesca.h>

class MImGroupInfo;
class MImAccessControlList;
/**
 * IM group handling interface for clients use.
 * Interface is implemented by the IM.
 *
 * MImGroups provides services for
 * Im clients to manage groups
 * and group members. It provides also services to 
 * subscribe notifications from IM group list changes
 * and from individual IM group changes.
 *
 * Client can retrieve MImGroups interface from
 * MImContext interface.
 *
 * @ingroup Immanagementapi
 * 
 */
class MImGroups : public MXIMPBase
    {
public:

    /** Interface ID for the MImGroups. */
    enum { KInterfaceId = IM_IF_ID_GROUPS };


protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MImGroups() {}


public: //IM Group management requests
   
   	/**
     * Subscribe to the specified group change notification
     * 
     * @return The request ID identifying the issued request.
     *
     * /b Note: Possible error conditions
     *	/n		1. Group does't exists  - KImErrGroupDoesNotExists
     *	/n		2. Not joined in this group  - KImErrGroupNotJoined
     *
     * If the request is success whenever there is a change in the group
     * user will be notified.
     *
     */
     virtual TXIMPRequestId SubscribeImGroupChangeL(const MXIMPIdentity& aGroupId)= 0;
     
     /**
     * UnSubscribe to the specified group change notification
     * 
     * @param [in] aGroupId
     *		Identification for IM group.
	 *
     * @return The request ID identifying the issued request.
     *
     * /b Note: Possible error conditions
     *	/n		1. Group does't exists - KImErrGroupDoesNotExists
     *	/n		2. Not joined in this group  - KImErrGroupNotJoined
     *
     * If the request is success whenever there is a change in the group
     * user will not be notified.
     *
     */
     virtual TXIMPRequestId UnsubscribeImGroupChangeL(const MXIMPIdentity& aGroupId)= 0;
        /** 
     * Creates IM group
     * 				(By default private group)
     * @param [in] aProperties
     *      To be created IM group Properties.
     *
     * @return The request ID identifying the issued request.
     *
     * /b Note: Possible error conditions
     *	/n		1. Group already exists -  KImErrGroupAlreadyExists
     *	/n		2. Incorrect group properties is not  - KImErrGroupPropertyNotSupported
     *	/n		3. Not allowed to create a group - KImErrNotHavingPermission
     *
     * If the request is success it will create a group
     *
     */
    virtual TXIMPRequestId CreateImGroupL(const MImGroupInfo& aProperties) = 0;
	/**
     * Join IM group
     *
	 * @param [in] aGroupId	
     *		Identification for IM group.
     *
	 * @return The request ID identifying the issued request.
     *
     * /b Note: Possible error conditions
     *	/n		1. Group does't exists - KImErrGroupDoesNotExists 
     *	/n		2. Group already joined - KImErrGroupAlreadyJoined
     *	/n		3. Joining this group is rejected - KImErrJoiningNotAllowed
     *	/n		4. Reached the maximum limit to users can be joined - KImErrTooManyIMGroups
	 * If the request is success user will be joined in the 
	 * specified group with the mentioned screenname
     */
    virtual TXIMPRequestId JoinImGroupL(const MXIMPIdentity& aGroupId) = 0;
   /**
     * Delete IM group
     *
	 * @param [in] aGroupId
     *		Identification for IM group.
     *
	 * @return The request ID identifying the issued request.
     *
     * /b Note: Possible error conditions
     *	/n		1. Group does't exists - KImErrGroupDoesNotExists 
     *	/n		2. Not having access rigts to delete the group -KImErrNotHavingPermission
     * If the request is success specified group will be deleted
     */
    virtual TXIMPRequestId DeleteImGroupL(const MXIMPIdentity& aGroupId) = 0;
    
    
    /**
     * Leave IM group
     *
	 * @param [in] aGroupId
     *		Identification for IM group.
     *
	 * @return The request ID identifying the issued request.
     *
     * /b Note: Possible error conditions
     *	/n		1. Group does't exists - KImErrGroupDoesNotExists 
     *	/n		2. Not joined in this group - KImErrGroupNotJoined
     * If the request is success user will leave the group.
     */
    virtual TXIMPRequestId LeaveImGroupL(const MXIMPIdentity& aGroupId) = 0;
   
    /**
     * Get the specified group properties
     *
	 * @param [in] aGroupId
     *		Identification for IM group.
     *
	 * @return The request ID identifying the issued request.
     *
     * /b Note: Possible error conditions
     *	/n		1. Group does't exists - KImErrGroupDoesNotExists
     *	/n		2. Not joined in this group - KImErrGroupNotJoined
     *
     * If the request is success response will have the specified group properties
     *
     */
     virtual TXIMPRequestId GetImGroupPropertiesL( MXIMPIdentity* aGroupId )= 0;


	/**
     * Set/Modify the specified group properties
     *
	 * @param [in] aGroupId
     *		Identification for IM group.
     * @param [in] aGroupProps
     *		Group properties need to be set for the specified group
     *
	 * @return The request ID identifying the issued request.
     *
     * /b Note: Possible error conditions
     *	/n		1. Group does't exists - KImErrGroupDoesNotExists
     *	/n		2. Incorrect group properties - KImErrGroupPropertyNotSupported
     *	/n		3. Not allowed(having permission) to change the group properties - KImErrNotHavingPermission
     *
     * If the request is success specified group properties will be updated
     *
     */
     virtual TXIMPRequestId SetImGroupPropertiesL( MXIMPIdentity* aGroupId,
                                            MImGroupInfo* aGroupProps)= 0;

	 /**
     * Get access control list of users of the specified group
     *
	 * @param [in] aGroupId
     *		Identification for IM group.
     *
	 * @return The request ID identifying the issued request.
	 *
     * /b Note: Possible error conditions
     *	/n		1. Group does't exists
     *	/n		2. User does't exists
     * If the request is success responce will have access control list for the specified group
     *
     */
   virtual TXIMPRequestId GetImGroupAccessControlListL( const MXIMPIdentity& aGroupId )=0;
    /**
     * Set access control list of users for the specified group
     *
	 * @param [in] aGroupId
     *		Identification for IM group.
	 * @param [in] acl
     *		access control list.

     * @return The request ID identifying the issued request.
     * 
	 *  /b Note: Possible error conditions
     *	/n		1. Group does't exists
     *	/n		2. User does't exists
	 *  /n      3. User don't have access control   
     * If the request is success responce will have access control for the specified group
     *
     */
   
   virtual TXIMPRequestId SetImGroupAccessControLlistL( const MXIMPIdentity& aGroupId,
                                           MImAccessControlList* aCl ) = 0;

  	/**
     * Set rejected list of users of the specified group
     *
	 * @param [in] aGroupId
     *		Identification for IM group.
     * @param [in] aRejectedList
     *      List of users need to be rejected from the group  
     * @param [in] aEnabledList
     *      List of users need to be added to the group      
     *
	 * @return The request ID identifying the issued request.
     *
     * /b Note: Possible error conditions
     *	/n		1. Group does't exists
     *	/n		2. User does't exists
     *  /n		3. Not allowed(having permission) to chage/update users list of the group
     * If the request is success user permissions will be  
     * granted as per the request in the specified group
     *
     */
   virtual TXIMPRequestId SetImGroupRejectListL( const TDesC& aGroupId,
                                      const MDesCArray* aRejectedList,
                                      const MDesCArray* aEnabledList )= 0;

    

 public: //Prototypes for IM group member management requests
 
   /**
     * Get list of members of the indicated group
	 * 
     * @param [in] aGroupId
     *		Identification for IM group.
     *
	 * @return The request ID identifying the issued request.
     *
     * /b Note: Possible error conditions
     *	/n		1. Group does't exists  - KImErrGroupDoesNotExists
     *	/n		2. Not Joined in this group  - KImErrGroupNotJoined
     *
     * If the request is success,response will have list of members of the specified group
     *
     */
     virtual TXIMPRequestId GetImGroupMembersL( MXIMPIdentity* aGroupId )= 0;
      /**
     * Get lest of joined users for the specified group
     *
	 * @param [in] aGroupId
     *		Identification for IM group.
     *
	 * @return The request ID identifying the issued request.
     *
	 * /b Note: Possible error conditions
     *	/n		1. Group does't exists
     *	/n		2. User does't exists
     * If the Request is success client will get list of joined users for the specified group
     *
     */
   virtual TXIMPRequestId GetImGroupJoinedUsersListL( const MXIMPIdentity& aGroupId )=0;
       
	 /**
     * Add members to IM group
     *
	 * @param [in] aGroupId
     *		Identification for IM group.
     * @param [in] aUserList
     *      List of users need to be added to the group     
     *
	 * @return The request ID identifying the issued request.
     *
     * /b Note: Possible error conditions
     *	/n		1. Group does't exists
     *	/n		2. Not allowed(having permission) to change the group properties
     *
     * If the request is success user list will be added to the Group
     *
     */
   
	virtual TXIMPRequestId AddImGroupMembersL( const MXIMPIdentity& aGroupId,const MDesCArray& aUserList )= 0;
										

  /**
     * Remove members from IM group
     *
	 * @param [in] aGroupId
     *		Identification for IM group.
     * @param [in] aUserList
     *      List of users need to be removed from the group     
     *
	 * @return The request ID identifying the issued request.
     *
     * /b Note: Possible error conditions
     *	/n		1. Group does't exists
     *	/n		2. Not allowed(having permission) to change the group properties
     *
     * If the request is success user list will be removed to the Group
     *
     */
    virtual TXIMPRequestId RemoveImGroupMembersL( const MXIMPIdentity& aGroupId,
   											  const MDesCArray& aUserList )= 0;

    
    
    
    
    
    };



#endif // MIMGROUPS_H