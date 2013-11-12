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
* Description:  Interface for managing cached Search information.
*
*/

#ifndef MPROTOCOLIMSEARCHDATAHOST_H
#define MPROTOCOLIMSEARCHDATAHOST_H


#include <e32std.h>
#include <protocolimpluginhostifids.hrh>
#include <ximpbase.h>


/**
 * Interface for managing cached search information.
 *
 * Interface is used by protocol adaptations to manage
 * search related information in
 * IM internal data cache
 *
 * This interface is implemented by IM.
 * Protocol adaptation can obtain reference to this
 * interface from MXIMPProtocolConnectionHost interface.
 *
 * For each separated protocol connection, protocol
 * adaptation must use the specific
 * MProtocolImSearchDataHost instance,
 * what is received from the connection specific
 * MXIMPProtocolConnectionHost instance.
 *
 * @ingroup protocolimpluginhostapi
 * @since S60 
 */
class MProtocolImSearchDataHost : public MXIMPBase
    {
public:

    /** Interface ID for the MProtocolImSearchDataHost. */
    enum { KInterfaceId = IM_IF_ID_PROTOCOL_SEARCH_DATAHOST };


protected:


    /**
     * Protected destructor. Object instancies
     * can't be deleted via this interface.
     */
    virtual ~MProtocolImSearchDataHost() {}



public: // Handling search information


    /**
     * Notifies IM from 
     * search result information.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from search
     * result information. IM
     * updates its cached status, and
     * notifies the subscribing clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aSearchId
	 * 			search operation id, used when search is continued or stopped
	 * @param [in]  aResultsFound - Indicates the number of the current findings. 
     * @param [in]  aIndex    -  Indicates the particular index from which the 
     *        next search can start. It is 0 in error case.
     * @param [in]  aCompleted - TRUE if server has completed the search, 
     *        FALSE if there are more results available.
     * @param [in]  aUsrIdResults -List of results( UserId's), 
     *        NULL if none.    
     *
     * @return The request id identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */

    virtual TXIMPRequestId HandleSearchL(MXIMPIdentity* aSearchId,
    							TInt aResultsFound,  
                                TBool aCompleted, 
                                TInt aIndex,                                 
                                MDesCArray* aUsrIdResults)  = 0;

	/**
     * Notifies IM from 
     * search result information.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from search
     * result information. IM
     * updates its cached status, and
     * notifies the subscribing clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aSearchId
	 * 			search operation id, used when search was stopped
     *
     * @return The request id identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */

    virtual TXIMPRequestId HandleSearchStoppedL(MXIMPIdentity* aSearchId)  = 0;
    };


#endif // MProtocolImSEARCHDATAHOST_H
