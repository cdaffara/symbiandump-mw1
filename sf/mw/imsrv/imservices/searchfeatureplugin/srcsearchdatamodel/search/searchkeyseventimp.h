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
* Description:  MSearchKeysEvent object implementation.
 *
*/

#ifndef CSEARCHKEYSEVENTIMP_H
#define CSEARCHKEYSEVENTIMP_H

#include  <s32strm.h>
#include "ximpapieventbase.h"
#include <searchkeysevent.h>
#include "searchapiobjbase.h"
#include "searchtypehelpers.h"

class RReadStream;

class CSearchKeyInfoImp;

/**
 * MSearchKeysEvent API object implementation.
 *
 * @lib searchdatamodel.dll
 */
NONSHARABLE_CLASS( CSearchKeysEventImp ): public CXIMPApiEventBase,
                                             public MSearchKeysEvent
    {
	public:
	    /** The class ID. */
	    enum { KClassId = IMP_CLSID_CSEARCHKEYSEVENTIMP  };

	public:
	    /**
	     * Exported instantiation method for initializing
	     * new event object.
	     */
	    IMPORT_C static CSearchKeysEventImp* NewLC( RSearchKeyInfoImpArray* aKeyInfoArray );
	            

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
	     virtual ~CSearchKeysEventImp();
	     
	public: // From MSearchKeysEvent
        /**
         * 
         * @see MSearchKeysEvent
         */
         TInt  InfoCount() const;
         /**
          * 
          * @see MSearchKeysEvent
          */ 
		 const MSearchKeyInfo& SearchKeyInfo( TInt aIndex ) const;
		 

	private:
		/**
	     * constructor
	     */
	    CSearchKeysEventImp();
	    
	    /**
	     * 2nd phase constructor
	     */
	    void ConstructL( RSearchKeyInfoImpArray* aKeyInfoArray );							
	            
		/**
	     * 2nd phase constructor
	     */
	    void ConstructL( RReadStream& aStream );
  

	    /**
	     *  internalize method 
	     */
	    void InternalizeL( RReadStream& aStream ) ;
        
	    /**
         * Helper for externalize
         */
        void WriteArrayToStreamL( RSearchKeyInfoImpArray& aKeyInfoArray,
        						  RWriteStream& aStream ) const;
        
        /**
         * Helper for internalize
         */
        void FillArrayFromStreamL( RSearchKeyInfoImpArray& aKeyInfoArray, 
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


	
	private: // data

       /**
        * RSupportedSearchKeyImpArray :  owned
        */
	    RSearchKeyInfoImpArray* iInfoArray;
	    
	
    };


#endif // CSEARCHKEYSEVENTIMP_H
