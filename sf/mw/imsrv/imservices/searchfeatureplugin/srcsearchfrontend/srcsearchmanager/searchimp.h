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
* Description:  Implementation of MSerach
*
*/

#ifndef CSEARCHIMP_H
#define CSEARCHIMP_H

#include <search.h>
#include <ximpobjectcollection.h>
#include <ximpapiobjbase.h>

#include "searchapiobjbase.h"
#include "searchlogutils.h"


// FORWARD DECLARATIONS
class MXIMPContextInternal;
class MSearchElement;

/**
 * MSearch  implementation.
 *
 * @lib searchmanager.dll
 */
NONSHARABLE_CLASS( CSearchImp ): public CXIMPApiObjBase,public MSearch
    {
    public:
	    /** The class ID. */
	    enum { KClassId = IMP_CLSID_CSEARCHIMP  };

	public:
	   	/**
    	 * Construction :NewL
     	 */
	    static CSearchImp* NewL( MXIMPContextInternal& aContext );
	    
		/**
         * Destructor
         */
	    ~CSearchImp();

	public:
	    /**
	     * constructor
	     */
	    CSearchImp( MXIMPContextInternal& aContext );


	public: // From MXIMPBase

	    /**
	     * Implementation of MXIMPBase interface methods
	     * @see MXIMPBase
	     */
	  	 XIMPIMP_DECLARE_IF_BASE_METHODS
	  	 
		/**
	     * 
	     * @see MSearch
	     */
	 	TXIMPRequestId SearchL( const MXIMPIdentity& aSearchId,
	    						const RPointerArray< MSearchElement >& aObjs,
	                            TInt aSearchLimit ) ;
	  	/**
         * 
         * @see MSearch
         */								  											
		TXIMPRequestId GetSearchKeysL() ;
		
	private: // data

	    /**
	     * Context session proxy for accessing context server.
	     * Ref. : not owned 
	     */
	    MXIMPContextInternal& iContext;


    };


#endif // CSEARCHIMP_H
