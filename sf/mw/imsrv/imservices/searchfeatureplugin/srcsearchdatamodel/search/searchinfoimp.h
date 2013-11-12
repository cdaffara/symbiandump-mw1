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
* Description:  MsearchInfo API object implementation.
*
*/

#ifndef CSEARCHINFOIMP_H
#define CSEARCHINFOIMP_H


#include <s32strm.h>
#include <badesca.h>
#include <searchinfo.h>
#include "ximpapidataobjbase.h"
#include "searchapiobjbase.h"


class CSearchInfoImp;
class CXIMPIdentityImp;
/**
 * MsearchInfo API object implementation.
 *
 * @lib searchdatamodel.dll
 */
NONSHARABLE_CLASS( CSearchInfoImp ): public CXIMPApiDataObjBase,public MSearchInfo
    {
    public:
    /** The class ID. */
    enum { KClassId = IMP_CLSID_CSEARCHINFOIMP };

	public:
		/**
    	 * Construction :NewLC
     	 */
	    IMPORT_C static CSearchInfoImp* NewLC();
	    /**
    	 * Construction :NewL
     	 */
	    IMPORT_C static CSearchInfoImp* NewL();
	    
	    /**
         * Destructor
         */
	    virtual ~CSearchInfoImp();

	private:
		/**
	     * constructor
	     */
	    CSearchInfoImp();
	    
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

	                         
	public: // New functions

	    /**
	     * Internalizes object data from given stream.
	     * @param aStream Stream to read.
	     */
	    IMPORT_C void InternalizeL( RReadStream& aStream );
	    
	    /**
	     * 
	     * @see MSearchInfo
	     */
	   const TDesC16& GetFieldInfo(TInfoType aType ) const;

		/**
	     * 
	     * @see MSearchInfo
	     */
	   void SetFieldInfoL(const TDesC16& aFieldInfo, TInfoType aType) ;
	   
		/**
	     * returns the order of elements
	     * @  return '1' always here  : 
	     */
	   IMPORT_C  static TInt InfoLinearOrder(const CSearchInfoImp& aA,
	        						 const CSearchInfoImp& aB ) ;
	        

	private: // data

	    
	    /**
	     *  buffers : all owned 
	     */
	    RBuf16 iFirstname; // owned
	    
	    RBuf16 iLastname;  // owned
	    
	    RBuf16 iEmailId;   // owned
	    
    };


#endif // CSEARCHINFOIMP_H
