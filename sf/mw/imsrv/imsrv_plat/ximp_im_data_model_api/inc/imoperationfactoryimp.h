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
* Description:  Interface for Search info object.
*
*/


#ifndef IMCOPERATIONFACTORYIMP_H
#define IMCOPERATIONFACTORYIMP_H


#include <e32base.h>
class CXIMPOperationBase;

#include "ximpoperationfactory.h"

/**
 * Operation factory
 * Class is owned by and handled as singleton by CXIMPGlobals.
 *
 * @lib imoperation.dll
 * @since S60 v4.0
 */
class CImOperationFactory : public CBase, 
                                  public MXIMPOperationFactory
    {
    public: // Construction and desctruction

        IMPORT_C static CImOperationFactory* NewL();

        ~CImOperationFactory();

    private: // Construction

        /**
         * C++ default constructor.
         */
        CImOperationFactory();

        /**
         * Symbian 2nd phase constructor
         */
        void ConstructL();


    public: // From MXIMPOperationFactory

		/**
		 * @see MXIMPOperationFactory
	 	 */
		CXIMPOperationBase* CreateOperationL( TInt aOperation );

    private:    // Data

    };

#endif      // IMCOPERATIONFACTORYIMP_H

