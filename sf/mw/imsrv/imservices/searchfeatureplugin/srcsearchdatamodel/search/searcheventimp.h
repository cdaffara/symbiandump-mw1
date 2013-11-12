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
* Description:  MSearchEvent API object implementation.
 *
*/

#ifndef CSEARCHEVENTIMP_H
#define CSEARCHEVENTIMP_H

#include  <s32strm.h>
#include "ximpapieventbase.h"
#include <searchevent.h>
#include "searchapiobjbase.h"
#include "searchtypehelpers.h"

class RReadStream;
class CXIMPIdentityImp;
class CSearchInfoImp;
class MXIMPIdentity;
/**
 * MSearchEvent API object implementation.
 *
 * @lib searchdatamodel.dll
 */
NONSHARABLE_CLASS( CSearchEventImp ): public CXIMPApiEventBase,public MSearchEvent
    {
	public:
	    /** The class ID. */
	    enum { KClassId = IMP_CLSID_CSEARCHEVENTIMP  };

	public:
	    /**
	     * Exported instantiation method for initializing
	     * new event object.
	     */
	    IMPORT_C static CSearchEventImp* NewLC( CXIMPIdentityImp& aSearchId,
                                	            RSearchInfoImpArray* aSearchResults );
     
	    /**
	     * Instantiation method for event automation.
	     * Method signature must be exactly this to work
	     * with event delivery automation system.
	     *
	     * Event implementation must be registered to
	     * SearchEventCodec KApiEventConstructorTable.
	     */
	    static CXIMPApiEventBase* NewFromStreamLC( RReadStream& aStream );
	    
		/**
         * Destructor
         */
	    virtual ~CSearchEventImp();

	private:
		/**
	     * constructor
	     */
	    CSearchEventImp();
	    
	    /**
	     * 2nd phase constructor
	     */
	    void ConstructL( CXIMPIdentityImp& aSearchId,
	            		RSearchInfoImpArray* aSearchResults );
	            
		/**
	     * 2nd phase constructor
	     */
	    void ConstructL( RReadStream& aStream );

	    

	    /**
	     * Helper for internalize
	     */
	    void InternalizeL( RReadStream& aStream ) ;
	     /**
	     * Helper for externalize
	     */        
		void WriteArrayToStreamL(RSearchInfoImpArray& aArray, RWriteStream& aStream ) const;
		
	    /**
	     * Helper for internalize
	     */
	    void FillArrayFromStreamL( RSearchInfoImpArray& aArray,
	            RReadStream& aStream );
	    

	public: // From API base interfaces

	    /**
	     * Implementation of MXIMPBase interface methods
	     * @see MXIMPBase
	     */
	      XIMPIMP_DECLARE_IF_BASE_METHODS

	    /**
	     * Implementation of MSearchEventBase interface and
	     * CSearchApiEventBase methods
	     *
	     */
	   	XIMPIMP_DECLARE_EVENT_BASE_METHODS


	public: // From MSearchEvent
		/**
	     * 
	     * @see MSearchEvent
	     */
	 	virtual TInt SearchResultCount() const ;
	    
	    
	    /**
	     * Gets the searched result by index.
	     *
	     * @param [in] aIndex
	     *        The index of the search.
	     *
	     * @return The searched result from given index.
	     */
	    virtual const MSearchInfo& SearchResult( TInt aIndex ) const ;
	    /**
	     * 
	     * @see MSearchEvent
	     */
	    virtual const MXIMPIdentity& SearchId() const;
	    
	private: // data

		/**
	     * Search Info array 
	     * Own.
	     */
	     RSearchInfoImpArray* iSearchResults;
	     
	     /**
         * CXIMPIdentityImp :  owned
         */
		 CXIMPIdentityImp*    iSearchId ;
		 
		 TBool iConstructedFromStream;
    };


#endif // CSEARCHEVENTIMP_H
