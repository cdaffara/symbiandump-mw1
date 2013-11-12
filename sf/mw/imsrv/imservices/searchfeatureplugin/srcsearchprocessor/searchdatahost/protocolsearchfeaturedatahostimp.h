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
* Description:   Searchdatafeature  datahost.
*
*/

#ifndef CPROTOCOLSEARCHFEATUREDATAHOSTIMP_H
#define CPROTOCOLSEARCHFEATUREDATAHOSTIMP_H

#include <protocolsearchfeaturedatahost.h>

#include "ximpapiobjbase.h"
#include "searchapiobjbase.h"
#include "searchtypehelpers.h"

class MXIMPHost;
class MXIMPObjectCollection;


/**
 * Implementation for managing cached Invitation information
 *
 * @lib searchprocessor.dll
 */
class CProtocolSearchFeatureDataHostImp : public CXIMPApiObjBase,
                                          public MProtocolSearchFeatureDataHost
    {
	public: // Definitions

	    /** The class ID. */
	    enum { KClassId = IMP_CLSID_CPROTOCOLSEARCHFEATUREDATAHOST };

	public: // Construction and destruction

	    /**
	     * Construction :NewL
	     */
	    IMPORT_C static CProtocolSearchFeatureDataHostImp* NewL( MXIMPHost& aHost );

	    /**
	     * Destruction
	     */
	    virtual ~CProtocolSearchFeatureDataHostImp();

	private:
	 	/**
	     * constructor
	     */
	    CProtocolSearchFeatureDataHostImp( MXIMPHost& aHost );
	    /**
	     * 2nd phase constructor
	     */
	    void ConstructL();
	    

	public: // From MXIMPBase

	    /**
	     * Implementation of MXIMPBase interface methods
	     * @see MXIMPBase
	     */
	    XIMPIMP_DECLARE_IF_BASE_METHODS
	    

	public: //From MProtocolSearchFeatureDataHost
		/**
	     * 
	     * @see MProtocolSearchFeatureDataHost
	     */

	  	TXIMPRequestId HandleSearchResultsL( MXIMPIdentity* aSearchId,
	    								     MXIMPObjectCollection* aSearchResults );
		
	  	/**
         * 
         * @see MProtocolSearchFeatureDataHost
         */
	  	TXIMPRequestId HandleSearchKeysL( MXIMPObjectCollection* aSearchKeyInfos  ) ;   											    
	 
	private: // Data

	    /**
	     * Access to host. Not owned
	     */
	    MXIMPHost& iHost;
    };




#endif // CPROTOCOLSEARCHFEATUREDATAHOSTIMP_H

