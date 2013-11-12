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
* Description:  header for MProtocolSearchFeatureDataHost
*
*/

#ifndef MPROTOCOLSEARCHFEATUREDATAHOST_H
#define MPROTOCOLSEARCHFEATUREDATAHOST_H


#include <searchprotocolpluginhostifids.hrh>
#include <ximpbase.h>


class MXIMPIdentity;
class MXIMPObjectCollection;

/**
 * Interface for managing cached search information.
 *
 * Interface is used by protocol adaptations to manage
 * search related information 
 *
 * This interface is implemented by Search plugin
 * Protocol adaptation can obtain reference to this
 * interface from 
 * 
 * @ingroup ProtocolSearchpluginhostapi
 */
class MProtocolSearchFeatureDataHost : public MXIMPBase
    {
public:

    /** Interface ID for the MProtocolSearchFeatureDataHost. */
    enum { KInterfaceId = SEARCH_IF_ID_PROTOCOL_SEARCH_DATAHOST };


protected:


    /**
     * Protected destructor. Object instancies
     * can't be deleted via this interface.
     */
    virtual ~MProtocolSearchFeatureDataHost() {}



public: // Handling search information


    /**
     * Notifies Search plugin for 
     * search result information.
     *
     *
     * Search plugin takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aSearchId
	 * 			search  id of the search request

     * @param [in]  aSearchResults -List of results( MsearchInfos), 
     *        NULL if none.    
     *
     * @return The request id identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */

    virtual TXIMPRequestId HandleSearchResultsL(MXIMPIdentity* aSearchId,
    										    MXIMPObjectCollection* aSearchResults)  = 0;
   /**
    * Notifies Search plugin for 
    * supported search Keys and serach labels  information.
    * 
    *
    * Search plugin takes ownership of the argument(s),
    * if method succeeds without a leave.
    *
    * @param [in] aSearchKeyInfos - List of search key info objects 
    *        
    *
    * @return The request id identifying the issued request.
    *         Data cache request is processed asynchronously and
    *         its completion is signalled to adaptation direction.
    *
    * @leave KErrNoMemory if failed to allocate memory.
    * @leave KErrArgument if NULL argument(s) were provided.
    */							                               
    virtual TXIMPRequestId HandleSearchKeysL( MXIMPObjectCollection* aSearchKeyInfos )  = 0;                            

	/**
     * Notifies  search plugin for 
     * search stopped.
     *
     *
     *  search plugin takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aSearchId
	 * 			search  id of the search request
     *
     * @return The request id identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation.
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */

  //  virtual TXIMPRequestId HandleSearchCancelL(MXIMPIdentity* aSearchId)  = 0;
    };


#endif // MPROTOCOLSEARCHFEATUREDATAHOST_H
