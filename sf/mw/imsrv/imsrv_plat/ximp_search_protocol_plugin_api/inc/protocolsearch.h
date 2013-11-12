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
* Description:  Interface for Search Feature Protocol
*
*/



#ifndef MPROTOCOLSEARCH_H
#define MPROTOCOLSEARCH_H

#include <badesca.h>
#include <ximpbase.h>
#include <searchprotocolpluginifids.hrh>

class MXIMPIdentity;
class MSearchElement;
/**
 * Interface for Search object.
 * Interface must be implemented by a XIMP Framework protocol
 * plug-ins.
 *
 * This interface defines actions that XIMP Framework
 * requests from a Search protocol connection to
 * execute search related tasks.
 *
 *
 * @ingroup ProtocolSearchpluginapi
 * 
 */
class MProtocolSearch : public MXIMPBase
    {
public:

    /** Interface ID for the MProtocolSearch */
    enum { KInterfaceId = SEARCH_IF_ID_PROTOCOL_SEARCH };


protected:

    /**
     * Protected destructor. MProtocolSearch
     * instancies can't be destroyed via this interface.
     */
    virtual inline ~MProtocolSearch() {};


public: 

    /**
     * Requests the protocol to start search.
     *
     *
     * @param [in] aSearchId
     *		Identification for a search.
     *
     * @param [in] aObjs
     *        Defines what is to be searched. 
     *
     * @param [in] aSearchLimit
     *        How many results client wants at a time. 
     *
     * @param [in] aReqId
     * 		  request ID identifying the issued request.
     */
    virtual void DoSearchL( const  MXIMPIdentity& aSearchId,
							const  RPointerArray< MSearchElement >& aObjs,
							TInt   aSearchLimit,
	                		TXIMPRequestId aReqId ) = 0;

    

    /**
     * Requests the protocol to  Stop search.
     *
     * @param [in] aSearchId
     *		Identification for a search.
     *
     *
     * @param [in] aReqId
     * 		  request ID identifying the issued request
     */
/*	virtual void DoCancelSearchL( const MXIMPIdentity& aSearchId,
                                        TXIMPRequestId aReqId ) = 0;*/
	
    /**
     * Requests the protocol to get the serach keys
     *
     *
     * @param [in] aReqId
     *        request ID identifying the issued request
     */
    virtual void DoGetSearchKeysL( TXIMPRequestId aReqId ) = 0;

    };



#endif // MPROTOCOLIMSEARCH_H
