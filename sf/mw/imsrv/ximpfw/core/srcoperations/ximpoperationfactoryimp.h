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
* Description:  Presence Service Connection operation factory
 *
*/


#ifndef XIMPCPSCOPERATIONFACTORYIMP_H
#define XIMPCPSCOPERATIONFACTORYIMP_H


#include <e32base.h>
class CXIMPOperationBase;
class CPresenceOperationFactory;
class CImOperationFactory;

class MXIMPOperationFactory1 ;

#include "ximpoperationfactory.h"
#include "ximpoperationfactory1.h"

/**
 * Operation factory
 * Class is owned by and handled as singleton by CXIMPGlobals.
 *
 * @lib ximpoperation.dll
 * @since S60 v4.0
 */
class CXIMPOperationFactory : public CBase, 
                                 public MXIMPOperationFactory
    {
    public: // Construction and desctruction

        IMPORT_C static CXIMPOperationFactory* NewL();

        virtual ~CXIMPOperationFactory();

    private: // Construction

        /**
         * C++ default constructor.
         */
        CXIMPOperationFactory();

        /**
         * Symbian 2nd phase constructor
         */
        void ConstructL();
		
		void LoadOperationPluginsL() ;

    public: // From MXIMPOperationFactory

		/**
		 * @see MXIMPOperationFactory
	 	 */
		CXIMPOperationBase* CreateOperationL( TInt aOperation );

    private:    // Data
        CPresenceOperationFactory* iPresenceOperation;
        
		CArrayFixFlat<MXIMPOperationFactory1*>* iOperationPlugins;
    };

#endif      // XIMPCPSCOPERATIONFACTORY_H


