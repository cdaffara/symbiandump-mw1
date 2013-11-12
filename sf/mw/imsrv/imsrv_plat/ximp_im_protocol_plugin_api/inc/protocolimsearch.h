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
* Description:  Interface for IM Feature Plugin protocol IM object.
*
*/



#ifndef MPROTOCOLIMSEARCH_H
#define MPROTOCOLIMSEARCH_H
#include <e32std.h>
#include <badesca.h>
#include <ximpbase.h>
#include "improtocolifids.hrh"

#include "imsearchelementimp.h"


class MXIMPIdentity;
//class CSearchRequest;
/**
 * Interface for Search object.
 * Interface must be implemented by a XIMP Framework protocol
 * plug-ins.
 *
 * This interface defines actions that XIMP Framework
 * requests from a IM protocol connection to
 * execute search related tasks.
 *
 * Protocol implementation must route requests received through
 * this interface, to same remote IM service destination,
 * as where the parent MXIMPProtocolConnection is connected.
 * (Here the parent MXIMPProtocolConnection means the protocol
 * connection instance from where this interface instance
 * was retrieved.)
 *
 * @see MXIMPProtocolConnection
 * @ingroup ProtocolImpluginapi
 * 
 */
class MProtocolImSearch : public MXIMPBase
    {
public:

    /** Interface ID for the MProtocolImSearch */
    enum { KInterfaceId = IM_IF_ID_PROTOCOL_SEARCH };


protected:

    /**
     * Protected destructor. MProtocolImSearch
     * instancies can't be destroyed via this interface.
     */
    virtual inline ~MProtocolImSearch() {};


public: 

    /**
     * Requests the protocol to start search.
     *
     * @param [in] aImSearchId
     *		Identification for IM search.
     * @param [in] aPairs 
     *        Defines what is to be searched.
     * @param [in] aSearchLimit 
     *        How many results you want.
     * @param [in] aReqId 
     *		Request id identifying the issued request.
 	 * @return The request id identifying the issued request.
	 *
     * /b Note: Possible error conditions
     *	/n	 1. Invalid/unsupported search-element [KImErrFieldTypeNotSupported].
     */
    virtual void DoImSearchL( const MXIMPIdentity& aImSearchId,
    										   const CSearchRequest& aPairs,
                                    		   TInt aSearchLimit,
                                    		   TXIMPRequestId aReqId ) = 0;

    /**
     * Continue search.
     *
     * @param [in] aImSearchId
     *		  Identification for IM search.
     * @param [in] aSearchID 
     *		  Which search is to be continued.
     * @param [in] aIndex 
     *		  From which index the search is continued.
	 * @param [in] aReqId 
     *		Request id identifying the issued request.
     *
	 * @return The request id identifying the issued request.
     *
     * /b Note: Possible error conditions
     *	/n	 1. Initial search request was not sent / invalid searchid [KImErrFieldValueNotSupported].
     *	/n	 2. Invalid Search-Index / out of range [KImErrOutOfRangeValue].
     *	/n	 3. Search timeout (in case of continued search the subsequent request primitive is late) [KImErrServicRequestTimeouted].
     */
    virtual void DoContinueImSearchL( const MXIMPIdentity& aImSearchId,
    										  TInt aSearchID,
                                    		   TXIMPRequestId aReqId ) = 0;

    /**
     * Stop search.
     *
     * @param [in] aImSearchId
     *		Identification for IM search.
     * @param [in] aSearchID 
     *		  Which search is to be stopped.
     * @param [in] aReqId 
     *		Request id identifying the issued request.
	 *
	 * @return The request id identifying the issued request.
	 *
     * /b Note: Possible error conditions
     *	/n	 1. Initial search request was not sent (invalid searchid) [KImErrFieldValueNotSupported].
     */
	virtual void DoStopImSearchL( const MXIMPIdentity& aImSearchId,
											                     TInt aSearchID,
                                    		   TXIMPRequestId aReqId ) = 0;
	


    };



#endif // MPROTOCOLIMSEARCH_H
