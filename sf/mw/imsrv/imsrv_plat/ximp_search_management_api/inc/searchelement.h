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
* Description:  Interface for search element
*
*/


#ifndef MSEARCHELEMENT_H
#define MSEARCHELEMENT_H

#include <ximpbase.h>
#include <searchmanagementifids.hrh>

class MXIMPIdentity;
/**
 *  Enums 
 *  Supported search keys  
 *  Searver may not support all of these : 
 *  Client can query server which are all supported 
 */
enum TSearchKey 
    {
    EUserFirstName = 600,
    EUserMiddleName,
    EUserLastName,
    EUserFullName,
    EUserCountry,
    EUserCity,
    EBirthday,
    EState,
    EUserEmailAddress,
    EUser,
    EUserNickName, 
    EOrganisation,
    EOrganisationUnit,
    EReadLabel  // Not for client to set/use 
                // if client trys to set this ,  SetrequestL leaves with KErrArgument
                // In this case protocol plugin  has to read the LABEL
    };

/**
 * Interface for search element object.
 *
 * 
 *
 * @ingroup searchmanagementapi
 * 
 */
class MSearchElement: public MXIMPBase
	{
    public:

        /** Interface ID for the MImSearchRequest. */
    	enum { KInterfaceId = SEARCH_IF_ID_SEARCHELEMENT };

    public:
    
       /**
     	* Public destructor.
     	* Objects instancies can be deleted through this interface.
     	*/
    	virtual ~MSearchElement() {}
    	
    public: 	
       /**
        * Set search values. 
        * @param [in] aSearchStringId
	    *		Identification for  search word.
	    *
	    *
        * @param [in]  aKey
        *			  search key value.
        *
        * e.g. to serach for "james" as a first name
        *      aSearchStringId  has to be set with  "james"
        *      aKey is EUserFirstName
        * 
        *  @leave KErrArgument if  client trys to set 'EReadLabel' as key.
        *  if  aKey = EReadLabel then  leaves with  KErrArgument
	    */
        virtual void  SetRequestL( MXIMPIdentity&  aSearchStringId, TSearchKey aKey ) = 0;
        							
        /**
          * Set search values. 
          * @param [in] aSearchStringId
          *       Identification for  search word.
          *
          *
          * @param [in] aLabelId 
          *             Identification for  label.
          *
          * e.g. to serach for "james" as a first name
          *      aSearchStringId  has to be set with  "james"
          *      aLabelId is  "firstname"  : this changes from server to server
          *  client has to query these labels from server and use
          */   
        virtual void  SetRequestL( MXIMPIdentity& aSearchStringId, MXIMPIdentity& aLabelId  ) = 0;
       
        
        /**
        * returns  search identity 
        *
        *
        * @return identity of the search : adaptation has to get the text from the Id
	    */
        virtual MXIMPIdentity& GetSearchId() const =0 ;
        
       /**
        * returns  key  value of the search 
        *
        *
        * @return any enum value of TSearchKey
	    */
        virtual TSearchKey GetSearchKey()const =0 ;

        /**
         * returns   label identity 
         *
         *
         * @return identity of the label : adaptation has to get the text from the Id
         */  
        virtual MXIMPIdentity& GetSearchLabel() const =0 ;
        
		};


#endif //MSEARCHELEMENT_H
            
// End of File
