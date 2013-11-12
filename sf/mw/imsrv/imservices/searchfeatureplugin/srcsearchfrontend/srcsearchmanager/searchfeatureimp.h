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
* Description:  Implementation of MsearchFeature
*
*/

#ifndef CSEARCHFEATUREIMP_H
#define CSEARCHFEATUREIMP_H

#include <searchfeature.h>
#include <ximpapiobjbase.h>
#include "searchapiobjbase.h"


class MXIMPContext;
class MXIMPContextInternal;
class CSearchObjectFactoryImp;
class CSearchImp;

/**
 * MSearchFeature  implementation.
 *
 * @lib searchmanager.dll
 */
NONSHARABLE_CLASS( CSearchFeatureImp ):public MSearchFeature,
										public CXIMPApiObjBase
										  
                                          
    {
    public:
    /** The class ID. */
    enum { KClassId = IMP_CLSID_CSEARCHFEATUREIMP };


	public:
	
        /**
    	 * Construction :NewL
     	 */
	    IMPORT_C static CSearchFeatureImp* NewL( MXIMPContext* aContext );
	    /**
         * Destructor
         */
	    virtual ~CSearchFeatureImp();


	private:
        /**
         * Destructor
         */
	    CSearchFeatureImp();
	   	/**
         * inertnal context setting 
         */
	    void SetCtxL(MXIMPContext* aContext);
		/**
	     * constructor
	     */
	    void ConstructL(MXIMPContext* aContext);


	public: // From MXIMPBase

	    /**
	     * Implementation of MXIMPBase interface methods
	     * @see MXIMPBase
	     */
	    XIMPIMP_DECLARE_IF_BASE_METHODS


	public: //From MSearchFeature
		/**
	     * 
	     * @see MSearchFeature
	     */
	   
	    MSearchObjectFactory& SearchObjectFactory() const;
	    
	   	/**
	     * 
	     * @see MSearchFeature
	     */
	    MSearch&  Search() const;



	private: // data

	    /**
	     * Context for having a connection context server.
	     */
	    MXIMPContextInternal* iCtxInternal;
	    

	    /**
	     * Object factory sub interface : OWNED
	     */
	    CSearchObjectFactoryImp* iSearchObjFactory;

	    /**
	     * SERACH  sub interface. : OWNED
	     */
	    CSearchImp* iSearch;
    };


#endif // CSEARCHFEATUREIMP_H
