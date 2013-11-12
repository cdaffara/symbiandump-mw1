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
* Description:  Search Service  operation factory
 *
*/


#ifndef SEARCHOPERATIONFACTORYIMP_H
#define SEARCHOPERATIONFACTORYIMP_H


#include <e32base.h>
#include <ximpoperationfactory1.h>
#include "searchapiobjbase.h"

class CXIMPOperationBase;

/**
 * Operation factory
 *
 * @lib searchoperation.dll
 */

NONSHARABLE_CLASS(CSearchOperationFactory): public MXIMPOperationFactory1
    {
    
    public:
    /** The class ID. */
    enum { KClassId = IMP_CLSID_CSEARCHOPERATIONFACTORYIMP };
    
        
    public: // Construction and desctruction
    
    	/**
    	 * Construction :NewL
     	 */
        IMPORT_C static CSearchOperationFactory* NewL();
        
       /**
         * Destructor
         */
        virtual ~CSearchOperationFactory();

    private: // Construction

        /**
         * C++ default constructor.
         */
        CSearchOperationFactory();

        /**
         * Symbian 2nd phase constructor
         */
        void ConstructL();


    public: // From MXIMPOperationFactory1

		/**
		 * @see MXIMPOperationFactory1
	 	 */
		CXIMPOperationBase* CreateOperationL( TInt aOperation );

   

    };

#endif      // SEARCHCOPERATIONFACTORYIMP_H

