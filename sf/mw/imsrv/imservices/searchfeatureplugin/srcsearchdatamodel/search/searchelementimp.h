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
* Description:  Msearchelement  implementation.
*
*/

#ifndef CSEARCHELEMENTIMP_H
#define CSEARCHELEMENTIMP_H


#include <s32strm.h>
#include <badesca.h>
#include <searchelement.h>
#include "ximpapidataobjbase.h"
#include "searchapiobjbase.h"


class CSearchElementImp;
class CXIMPIdentityImp;
/**
 * MSearchElement  implementation.
 *
 * @lib searchdatamodel.dll
 */
NONSHARABLE_CLASS( CSearchElementImp ): public CXIMPApiDataObjBase,public MSearchElement
    {
    public:
    /** The class ID. */
    enum { KClassId = IMP_CLSID_CSEARCHELEMENTIMP };

	public:
	 	/**
    	 * Construction :NewLC
     	 */
	    IMPORT_C static CSearchElementImp* NewLC();
	    /**
    	 * Construction :NewL
     	 */
	    IMPORT_C static CSearchElementImp* NewL();
	    /**
         * Destructor
         */
	    virtual ~CSearchElementImp();

	private:
		/**
	     * constructor
	     */
	    CSearchElementImp();
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
	     * @see MSearchElement
	     */
	    void SetRequestL( MXIMPIdentity& aSearchId,TSearchKey aKey) ;
	    /**
         * 
         * @see MSearchElement
         */
	    void SetRequestL( MXIMPIdentity& aSearchId, MXIMPIdentity& aLabelId) ;  
	    /**
         * 
         * @see MSearchElement
         */
	    MXIMPIdentity&  GetSearchLabel() const;
	    
	   	/**
	     * 
	     * @see MSearchElement
	     */   
	    MXIMPIdentity&  GetSearchId() const;
	   
	   	/**
	     * 
	     * @see MSearchElement
	     */     
	    TSearchKey  GetSearchKey() const;  
	     
	private:
	    /**
         * CXIMPIdentityImp for search id :  owned
         */
	    CXIMPIdentityImp* iIdentity; 
	    /**
         * CXIMPIdentityImp for label :   owned
         */
	    CXIMPIdentityImp* iLabelIdentity; 
	    /**
         * search key
         */
	    TInt iSearchKey;
	    
	   
    };


#endif // CSEARCHELEMENTIMP_H
