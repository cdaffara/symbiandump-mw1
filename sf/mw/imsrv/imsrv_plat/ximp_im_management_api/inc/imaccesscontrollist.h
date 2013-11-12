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
* Description:  Interface for IM context.
*
*/

#ifndef MIMACCESSCONTROLLIST_H
#define MIMACCESSCONTROLLIST_H

#include <e32std.h>
#include <ximpbase.h>
#include <immanagementifids.hrh>
//#include <ximpcontext.h>


class MImObjectFactory;
class MImGroups;
class MImConversation;
class MImInvitation;
class MImSearch;

/**
 * Access control list interface for clients use.
 * Defines the access rules to a resource as an access control list (ACL). 
 * The resource itself is identified at the time the ACL is assigned to the resource.
 * Access control is support for the following resource types: IMGroup..... 


 * 
 * @ingroup immanagementapi
 */
class MImAccessControlList : public MXIMPBase
    {
public:

    /** Interface ID for the MImAccessControlList. */
    enum { KInterfaceId = IM_IF_ID_ACCESSCONTROL_LIST };


public: 
      
	/**
	* Stores moderators of the IM group
	* @return The Array of moderators of the Im group
	*/
   virtual RPointerArray<HBufC> ImGroupModerateList() = 0;
   /**
	* Stores admins of the IM group
	* @return The Array of admins of the Im group
	*/
   virtual RPointerArray<HBufC> ImGroupAdminList() = 0;
    /**
	* Stores ordinaries of the IM group
	* @return The Array of ordinaries of the Im group
	*/
   virtual RPointerArray<HBufC> ImGroupOrdinaryList() = 0;
   
    
protected:

    /**
     * Protected destructor.
     * Instancies can't be destroyed via this interface.
     */
    virtual ~MImAccessControlList() {}

    };




#endif // MIMACCESSCONTROLLIST_H



