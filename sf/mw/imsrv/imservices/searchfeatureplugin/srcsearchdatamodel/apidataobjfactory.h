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
* Description:  MApiDataObjFactory Declaration
*
*/



#ifndef M_APIOBJFACTORY_H
#define M_APIOBJFACTORY_H

#include <e32std.h>

class CXIMPApiDataObjBase;

class MApiDataObjFactory
    {  
	public:
	/**
     * instantiation method from the stream.
     *
     * @param [in] aDataObjInterfaceId
     *		interface id of the object to be constructed.
     *
     * @param [in] aStream
     *        read stream from which the object is instantiated.
     *
     *
	 * @return CXIMPApiDataObjBase object.
	 * 
	 * 
     */
	    
	 virtual CXIMPApiDataObjBase* NewFromStreamLC( TInt32 aDataObjInterfaceId,
	                                                            RReadStream& aStream ) = 0;
    
    };

#endif // M_APIOBJFACTORY_H

