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
* Description:  Search interface for clients use.
*
*/

#ifndef CIMSEARCH_H
#define CIMSEARCH_H

#include <e32std.h>
#include <ximpbase.h>
#include "imsearchelementimp.h"
#include "immanagementifids.hrh"

/**
 * Search interface for clients use.
 * Interface is implemented by the IM.
 *
 * @ingroup immanagementapi
 *
 */
class MImSearch : public MXIMPBase
    {
public:

    /** Interface ID for the MImSearch. */
    enum { KInterfaceId = IM_IF_ID_SEARCH };


protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MImSearch() {}



public: 

    /**
     * Start search.
     *
     * @param [in] aImSearchId
     *		Identification for IM search.
     * @param [in] aPairs 
     *        Defines what is to be searched.
     * @param [in] aSearchLimit 
     *        How many results you want.
     *
	 * @return The request ID identifying the issued request.
	 * 
	 * /b Note: Possible error conditions
     *	/n	 1. Invalid/Unsupported search-element [KImErrFieldTypeNotSupported].
     */
    virtual TXIMPRequestId ImSearchL( const MXIMPIdentity& aImSearchId,
    										   const CSearchRequest& aPairs,
                                    		   TInt aSearchLimit ) = 0;

    /**
     * Continue search.
     *
     * @param [in] aImSearchId
     *		  Identification for IM search.
     * @param [in] aSearchID 
     *		  Which search is to be continued.
     * @param [in] aIndex 
     *		  From which index the search is continued.
	 *
     * @return The request ID identifying the issued request.
     *
     * /b Note: Possible error conditions
     *	/n	 1. Initial search request was not sent / Invalid searchid [KImErrFieldValueNotSupported].
     *	/n	 2. Invalid Search-index / out of range [KImErrOutOfRangeValue].
     *	/n	 3. Search timeout (in case of continued search the subsequent request primitive is late) [KImErrServicRequestTimeouted].
     */
    virtual TXIMPRequestId ContinueImSearchL( const MXIMPIdentity& aImSearchId,
    										  TInt aSearchID ) = 0;

    /**
     * Stop search.
     *
	 * @param [in] aImSearchId
     *		Identification for IM search.
     * @param [in] aSearchID 
     *		  Which search is to be stopped.
     *
	 * @return The request ID identifying the issued request.
	 *
     * /b Note: Possible error conditions
     *	/n	 1. Initial search request was not sent (invalid searchid) [KImErrFieldValueNotSupported].
     */
	virtual TXIMPRequestId StopImSearchL( const MXIMPIdentity& aImSearchId,
											  TInt aSearchID ) = 0;
	
    };


#endif // CIMSEARCH_H


