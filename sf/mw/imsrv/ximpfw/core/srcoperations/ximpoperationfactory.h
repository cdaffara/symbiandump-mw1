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


#ifndef XIMPCPSCOPERATIONFACTORY_H
#define XIMPCPSCOPERATIONFACTORY_H

#include <e32base.h>
class CXIMPOperationBase;

/**
 * Operation factory interface
 *
 * @lib ximpoperation.dll
 * @since S60 v4.0
 */
class MXIMPOperationFactory
    {
	public: // Allow desctruction through this interface.

		virtual ~MXIMPOperationFactory(){}

    public: // From MXIMPOperationFactory

		/**
		 * Create operation by operation id.
		 * @param aOperation. Identifies operation needed to create.
		 * @return Operation as base object.
	 	 */
		virtual CXIMPOperationBase* CreateOperationL( TInt aOperation ) = 0;
    };

#endif      // XIMPCPSCOPERATIONFACTORY_H


