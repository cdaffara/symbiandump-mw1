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
* Description:  Interface for  protocol IM groups object.
*
*/



#ifndef MPROTOCOLIMGROUP_H
#define MPROTOCOLIMGROUP_H
#include <e32std.h>
#include <badesca.h>
#include <ximpbase.h>
#include "improtocolifids.hrh"
class MXIMPIdentity;
/**
 * Interface for IM protocol groups object.
 * Interface must be implemented by a XIMP Framework protocol
 * plug-ins.
 *
 * This interface defines actions that XIMP Framework
 * requests from a IM protocol connection to
 * execute groups related tasks.
 *
 * Protocol implementation must route requests received through
 * this interface, to same remote IM service destination,
 * as where the parent MXIMPProtocolConnection is connected.
 * (Here the parent MXIMPProtocolConnection means the protocol
 * connection instance from where this interface instance
 * was retrieved.)
 *
 * @see MXIMPProtocolConnection
 * @ingroup protocolimpluginapi
 *
 */
class MProtocolImGroup : public MXIMPBase
    {
public:

    /** Interface ID for the MProtocolImGroups. */
    enum { KInterfaceId = IM_IF_ID_PROTOCOL_GROUPS };


protected:

    /**
     * Protected destructor. MProtocolImuginroups
     * instancies can't be destroyed via this interface.
     */
    virtual inline ~MProtocolImGroup() {};

public: // 
    /**
     * Subscribe to the specified group change notification
     * 
     * @param [in] aGroupId
     *		Identification for IM group.
     * @param [in] aReqId 
     *		Request id identifying the issued request.
     */
     virtual void DoSubscribeImGroupChangeL( MXIMPIdentity* aGroupId,
     								  TXIMPRequestId aReqId )= 0;


	/**
     * Unsubscribe to the specified group change notification
     * 
     * @param [in] aGroupId
     *		Identification for IM group.
     * @param [in] aReqId 
     *		Request id identifying the issued request.
     */
     virtual void DoUnsubscribeImGroupChangeL( MXIMPIdentity* aGroupId,
     									TXIMPRequestId aReqId )= 0;

    /**
     * Requests the protocol to create group
     *
	 * @param [in] aGroupId
     *		Identification for IM group.
     * @param [in] aProperties
     *      To be created IM group properties.
     * @param [in] aScreenName
     *		Screen Name for the group
     * @param [in] aJoinGroup
     *		Whether joining this group is allowed
     * @param [in] aSubscribeNotification
     *		When the group is changed whether the notification is required
     * @param [in] aReqId 
     *		Request id identifying the issued request.
     */
    virtual void DoCreateGroupL(const MXIMPIdentity& aGroupId,
    					const MImGroupInfo& aProperties,
    					const TDesC& aScreenName,
                        TBool aJoinGroup,
                        TBool aSubscribeNotification,
                        TXIMPRequestId aReqId) = 0;
     /**
     * Requests the protocol to join IM Group
     *
	 * @param [in] aGroupId
     *		Identification for IM group.
     * @param [in] aScreenName
     *		Screen Name for the group
     * @param [in] aUsers
     *		Boolean value - mentioning whether the already 
     *		joined users list is required as a response to this request
     * @param [in] aSubscribeNotification
     *		When the group is changed whether the notification is required
     * @param [in] aReqId 
     *		Request id identifying the issued request.
     */
    virtual void DoJoinGroupL(const MXIMPIdentity& aGroupId,
                                  const TDesC& aScreenName,
                                  TBool aUsers,
                                  TBool aSubscribeNotification,
                                  TXIMPRequestId aReqId) = 0;
   /**
     * Requests the protocol to delete IM group
     *
	 * @param [in] aGroupId
     *		Identification for IM group.
     * @param [in] aReqId 
     *		Request ID identifying the issued request.
     */
    virtual void DoDeleteGroupL(const MXIMPIdentity& aGroupId,
    							 TXIMPRequestId aReqId ) = 0;
    
    
    /**
     * Requests the protocol to leave IM group
     *
	 * @param [in] aGroupId
     *		Identification for IM group.
	 * @param [in] aReqId 
     *		Request ID identifying the issued request.
     */
    virtual void DoLeaveGroupL(const MXIMPIdentity& aGroupId,
    							TXIMPRequestId aReqId ) = 0;                               

	/**
     * Requests the protocol to get the specified group properties
     *
	 * @param [in] aGroupId
     *		Identification for IM group.
     * @param [in] aReqId 
     *		Request id identifying the issued request.
     */
     virtual void DoGetGroupPropertiesL( MXIMPIdentity* aGroupId,
     									  TXIMPRequestId aReqId )= 0;


	/**
     * Requests the protocol to set/modify the specified group properties
     *
	 * @param [in] aGroupId
     *		Identification for IM group.
     * @param [in] aGroupProps
     *		Group properties need to be set for the specified group
     * @param [in] aReqId 
     *		Request id identifying the issued request.
     */
     virtual void DoSetGroupPropertiesL( MXIMPIdentity* aGroupId,
                                           const MImGroupInfo* aGroupProps,
                                           TXIMPRequestId aReqId )= 0;
     /**
     * Requests the protocol to Get access control list of users of the specified group
     *
	 * @param [in] aGroupId
     *		Identification for IM group.
     * @param [in] aReqId 
     *		Request ID identifying the issued request.
     * @return The request ID identifying the issued request.
     *
	 * /b Note: Possible error conditions
     *	/n		1. Group does't exists
     *	/n		2. User does't exists
     * If the request is success responce will have access control list for the specified group
     *
     */
	virtual TXIMPRequestId DoGetImGroupAccessControlListL( const MXIMPIdentity& aGroupId,
	                                                                           TXIMPRequestId aReqId )=0;
     /**
     * Requests the protocol to Set access control list of users for the specified group
     
	 * @param [in] aGroupId
     *		Identification for IM group.
	 * @param [in] acl
     *		access control list.
     * @param [in] aReqId 
     *		Request id identifying the issued request.
     * @return The request id identifying the issued request.
     *
	 * /b Note: Possible error conditions
     *	/n		1. Group does't exists
     *	/n		2. User does't exists
	 *  /n      3. User don't have access control   
     * If the request is success responce will have access control for the specified group
     *
     */
   
   virtual TXIMPRequestId DoSetImGroupAccessControLlistL( const MXIMPIdentity& aGroupId,
                                           MImAccessControlList* aCl,TXIMPRequestId aReqId ) = 0;
        
	  /**
     * Requests the protocol to set rejected list of users of the specified group
     *
	 * @param [in] aGroupId
     *		Identification for IM group.
     * @param [in] aRejectedList
     *      List of users need to be rejected from the group  
     * @param [in] aEnabledList
     *      List of users need to be added to the group      
     * @param [in] aReqId 
     *		Request id identifying the issued request.
     */
   virtual void DoSetImGroupRejectListL( const TDesC& aGroupId,
                                      const MDesCArray* aRejectedList,
                                      const MDesCArray* aEnabledList,
                                      TXIMPRequestId aReqId )=0;		
     /**
     * Requests the protocol to get list of members of the indicated group
	 * 
     * @param [in] aGroupId
     *		Identification for IM group.
     * @param [in] aReqId 
     *		Request ID identifying the issued request.
     *
	 * @return The request id identifying the issued request.
     *
     * /b Note: Possible error conditions
     *	/n		1. Group does't exists  - KImErrGroupDoesNotExists
     *	/n		2. Not joined in this group  - KImErrGroupNotJoined
     *
     * If the request is success,response will have list of members of the specified group
     *
     */
     virtual TXIMPRequestId DoGetImGroupMembersL( MXIMPIdentity* aGroupId,TXIMPRequestId aReqId )= 0;
      /**
     * Requests the protocol to Get lest of joined users for the specified group
     *
	 * @param [in] aGroupId
     *		Identification for IM group.
     * @param [in] aReqId 
     *		Request ID identifying the issued request.
     * @return The request ID identifying the issued request.
     * 
	 * /b Note: Possible error conditions
     *	/n		1. Group does't exists
     *	/n		2. User does't exists
     * If the Request is success client will get list of joined users for the specified group
     *
     */
   virtual TXIMPRequestId DoGetImGroupJoinedUsersListL( const MXIMPIdentity& aGroupId,TXIMPRequestId aReqId )=0;
    
    /**
     * Add members to IM Group
     *    
	 * @param [in] aGroupId
     *		Identification for IM group.
     * @param [in] aUserList
     *      List of users need to be added to the group     
     * @param [in] aReqId 
     *		Request id identifying the issued request.
     */
     
	virtual void DoAddImGroupMembersL( const MXIMPIdentity& aGroupId,
								 const MDesCArray& aUserList,
								 TXIMPRequestId aReqId )= 0;

   /**
     * Remove members from IM group
     *
	 * @param [in] aGroupId
     *		Identification for IM group.
     * @param [in] aUserList
     *      List of users need to be removed from the group     
     * @param [in] aReqId 
     *		Request id identifying the issued request.
     */
   virtual void DoRemoveImGroupMembersL( const MXIMPIdentity& aGroupId,
   									const MDesCArray& aUserList,
   									TXIMPRequestId aReqId  )= 0;
 
    };



#endif // MPROTOCOLIMGROUP_H
