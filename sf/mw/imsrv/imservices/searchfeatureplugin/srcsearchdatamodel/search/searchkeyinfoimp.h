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
* Description:  MSupportedSearchKey API object implementation.
*
*/

#ifndef CSEARCHKEYINFOIMP_H
#define CSEARCHKEYINFOIMP_H


#include <s32strm.h>
#include <badesca.h>
#include <searchkeyinfo.h>
#include "ximpapidataobjbase.h"
#include "searchapiobjbase.h"


/**
 * MSearchKeyInfo API object implementation.
 *
 * @lib searchdatamodel.dll
 */
NONSHARABLE_CLASS( CSearchKeyInfoImp ): public CXIMPApiDataObjBase, public MSearchKeyInfo
    {
    public:
    /** The class ID. */
    enum { KClassId = IMP_CLSID_CSEARCHKEY_INFO_IMP };

	public:
		/**
    	 * Construction :NewLC
     	 */
	    IMPORT_C static CSearchKeyInfoImp* NewLC();
	    /**
    	 * Construction :NewL
     	 */
	    IMPORT_C static CSearchKeyInfoImp* NewL();
	    
	    /**
         * Destructor
         */
	    virtual ~CSearchKeyInfoImp();

	private:
		/**
	     * constructor
	     */
	    CSearchKeyInfoImp();
	    
	    /**
	     * 2nd phase constructor
	     */
	    void ConstructL();
	    
	public: // From CXIMPApiDataObjBase

	    /**
	     * @see CXIMPApiDataObjBase
	     */
	    XIMPIMP_DECLARE_DATAOBJ_BASE_METHODS
	    XIMPIMP_DECLARE_IF_BASE_METHODS

	                         
	public: // from  MSearchKeyInfo

	    /**
	     * Internalizes object data from given stream.
	     * @param aStream Stream to read.
	     */
	    IMPORT_C void InternalizeL( RReadStream& aStream );
	    
	    /**
	     * 
	     * @see MSearchKeyInfo
	     */
	    TSearchKey Key() const ; 
	     void SetKey( TSearchKey aKey) ;
	     
	    /**
         * 
         * @see MSearchKeyInfo
         */
	     TKeyType  Type()const ;
	     void SetType(TKeyType aType);
	     
	     /**
          * 
          * @see MSearchKeyInfo
          */
	     const TDesC16&  Label() const;
	     void  SetLabelL(const TDesC16& aLabel);
	     
		/**
	     * returns the order of elements
	     * @  return '1' always here  : 
	     */
	   IMPORT_C  static TInt LinearOrder(  const CSearchKeyInfoImp& aA,
	        						       const CSearchKeyInfoImp& aB  ) ;
	        

	private: // data

	    // key
	    TInt iKey;
	    
	    //key type
	    TInt iKeyType;
	    
	    //Lablel : owned
	    RBuf16 iLabel; 
    };


#endif // CSEARCHKEYINFOIMP_H
