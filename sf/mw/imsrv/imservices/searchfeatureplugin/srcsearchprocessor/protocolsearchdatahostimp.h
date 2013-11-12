/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  searchdata host header
 *
*/

#ifndef CPROTOCOLSEARCHDATAHOSTIMP_H
#define CPROTOCOLSEARCHDATAHOSTIMP_H

#include <e32base.h>
#include <ximpprotocolconnectionhost.h>
#include <ximpcontext.h>
#include <ximpbase.h>
#include <protocolsearchdatahost.h>

#include "searchapiobjbase.h"
#include "ximpapiobjbase.h"
#include "ximphost.h"


class MXIMPHost;
class MSearchObjectFactory;
class CSearchObjectFactoryImp;

class CXIMPPscContext;
class MSearchDataCache;
class CSearchDataCache;
class CXIMPContextStateEventImp;
class CPeriodic;
class MXIMPProtocolConnection;

class CProtocolSearchFeatureDataHostImp;

class MProtocolSearchFeatureDataHost;



	/**
	 * SearchData  host.
	 *
	 * @lib searchprocessor.dll
	 */

NONSHARABLE_CLASS(CProtocolSearchDataHostImp)  : public MProtocolSearchDataHost
    	{
		public:
		    /** The class ID. */
		    enum { KClassId = IMP_CLSID_CPROTOCOLSEARCHDATAHOSTIMP };
		
		public:
		
	    	/**
	    	 * Construction :NewL
	     	 */
		    static CProtocolSearchDataHostImp* NewL( MXIMPHost& aHost );
		    
		    /**
	    	 * Construction :NewL
	     	 */
		    static CProtocolSearchDataHostImp* NewLC( MXIMPHost& aHost );
		    
	       /**
	         * Destructor
	         */
		    virtual ~CProtocolSearchDataHostImp();
		
		private:
		    /**
		     * constructor
		     */
		    CProtocolSearchDataHostImp( MXIMPHost& aHost );
		    
		    /**
		     * 2nd phase constructor
		     */
		    void ConstructL();
	
		public: // MProtocolSearchDataHost
		   /**
		     * 
		     * @see MProtocolSearchDataHost
		     */
		   MSearchObjectFactory& SearchObjectFactory();
		   
		   /**
		     * 
		     * @see MProtocolSearchDataHost
		     */
		   MProtocolSearchFeatureDataHost& SearchFeatureDataAccess() ;
		   
		   /**
		     * returns the  searchdatacache
		     *
		     * @return  reference to CSearchDataCache : ownershipr not transfered
		     */
		   IMPORT_C CSearchDataCache& SearchDataCache();
		
		private: // data
			/**
		     * Access to host. Not owned
		     */
		    MXIMPHost& iHost;
		    
			/**
		     * CProtocolSearchFeatureDataHostImp : owned
		     */
		  CSearchObjectFactoryImp* iSearchObjFactory;
		  	/**
		     * CProtocolSearchFeatureDataHostImp : owned
		     */
		  CProtocolSearchFeatureDataHostImp* iSrchFeatureDataHost;
		  	/**
		     * CProtocolSearchFeatureDataHostImp : owned
		     */
		  CSearchDataCache* iSearchDataCache;

    	};


#endif // CCPROTOCOLSEARCHDATAHOSTIMP_H

