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
* Description:  Implementation of MImSearchRequest
*
*/


#ifndef MIMSEARCHELEMENT_H
#define MIMSEARCHELEMENT_H
#include <e32std.h>
#include <ximpbase.h>
#include "imsearchelementimp.h"
#include "imapiobjbase.h"
#include "ximpidentity.h"
#include <immanagementifids.hrh>

/*
* Supported search elements 
*/
enum TSearchElement
    {
    EUserAlias,
    EUserOnlineStatus,
    EUserEmailAddress,
    EUserFirstName,
    EUserID,
    EUserLastName,
    EUserMobileNumber,
    EUserAgeMin,
    EUserAgeMax,
    EUserCountry,
    EUserFriendlyName,
    EUserCity,
    EUserGender,
    EUserIntention,
    EUserInterestHobbies,
    EUserMaritialStatus,
    EGroupID,
    EGroupName,
    EGroupTopic,
    EGroupUserIDJoined,
    EGroupUserIDOwner
    };

/*
* Search result element
*/
enum TSearchResultType
    {
    EUserResult,
    EGroupResult
    };
    
/**
 * Interface for IM search request object.
 *
 * 
 *
 * @ingroup Imdatamodelapi
 * 
 */
class MImSearchRequest : public MXIMPBase
{
    public:

        /** Interface ID for the MImSearchRequest. */
    	enum { KInterfaceId = IM_IF_ID_SEARCHREQUEST };

    public:
    
    	/**
     	* Public destructor.
     	* Objects instancies can be deleted through this interface.
     	*/
    	virtual ~MImSearchRequest() {}
    	
    public: 	
        /**
        * Set search values. 
        * @param [in] aSearchId
	    *		Identification for IM search.
        * @param [in] aElement 
        *			  search key
        * @param [in] aValue 
        *			  search value
        * @return The request ID identifying the issued request.
        *
        * Note: Possible error conditions
	    *		 1. Invalid/Unsupported Search-Element [KImErrFieldTypeNotSupported].
	    *		 2. Invalid/Unsupported Search-Value [KImErrFieldValueNotSupported].
	    */
        virtual TXIMPRequestId SetRequestL( MXIMPIdentity* aSearchId,
        										  TSearchElement aElement, 
                                   				  const TDesC& aValue ) = 0;
        
        /**
        * Reset the object 
        * @param [in] aSearchId
	    *		Identification for IM search.
        * @return The request ID identifying the issued request.
	    */
        virtual TXIMPRequestId ResetL( MXIMPIdentity* aSearchId ) = 0;
        
        

};


#endif //MIMSEARCHELEMENT_H
            
// End of File
