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

#ifndef MSEARCH_H
#define MSEARCH_H

#include <ximpbase.h>
#include <searchmanagementifids.hrh>

class  MXIMPIdentity;
class  MSearchElement ;
class  MXIMPObjectCollection;
/**
 * Search interface for clients use.
 * Interface is implemented by the IM.
 *
 * @ingroup Searchmanagementapi
 *
 */
class MSearch : public MXIMPBase
    {
public:

    /** Interface ID for the MSearch. */
    enum { KInterfaceId = SEARCH_IF_ID_SEARCH };


protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MSearch() {}



public: 

    /**
     * start the search, issues the serach request to server 
     *
     * @param [in] aSearchId
     *		Identification for a search.
     *
     * @param [in] aObjs
     *        Its array of search elements.Elements contain what is to be serached. 
     *
     * @param [in] aSearchLimit 
     *        How many results client wants at a time.
     *
	 * @return The request ID identifying the issued request.
	 * 
	 * 
     */
    virtual TXIMPRequestId SearchL( const MXIMPIdentity& aSearchId, 
    								const RPointerArray< MSearchElement >& aObjs,
                                    TInt aSearchLimit ) = 0;

     

    /**
     * Cancels  the  search request which was issued.
     *
	 * @param [in] aImSearchId
     *		Identification for the search to be stopped.
     * 
     *
	 * @return The request ID identifying the issued request.
	 *
     * 
     */
	//virtual TXIMPRequestId CancelSearchL( const MXIMPIdentity& aSearchId ) = 0;
											 
    /**
     *  Requests the protocol plugin for search keys supported.
     *  Search key basically says client that server supports serach on this
     *  particular key. Key may be e.g. EUserFirstname 
     * 
     */
	virtual TXIMPRequestId GetSearchKeysL() = 0;
    };


#endif // MSEARCH_H


