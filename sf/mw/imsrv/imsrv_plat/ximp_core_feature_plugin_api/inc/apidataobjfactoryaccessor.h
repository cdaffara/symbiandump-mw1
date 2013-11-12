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
* Description:  IMplementation of MApiDataObjFactoryAccessor
*
*/



#ifndef M_APIDATAOBJFACTORYACCESSOR_H
#define M_APIDATAOBJFACTORYACCESSOR_H

#include <e32std.h>
#include <ecom/ecom.h>

class MEventCodec;
class MApiDataObjFactory;


/**
 * ?one_line_short_description
 *
 * ?more_complete_description
 *
 * @lib ?library
 * @since S60 v4.0
 */
class MApiDataObjFactoryAccessor
    { 

public:

    inline static MApiDataObjFactoryAccessor* NewPresenceAccessorL( );

    inline static MApiDataObjFactoryAccessor* NewL( TUid aImplUid );
    
    /**
     * Returns the feature plugin specific event codec object
     */
    virtual MEventCodec& EventCodec() = 0;
    
    /**
     * Returns the feature plugin specific api object data factory object
     */    
    virtual MApiDataObjFactory& ApiDataObjFactory() = 0;

    /**
     * dustructor
     */    
    inline virtual ~MApiDataObjFactoryAccessor();
    
    /**
     * To get the list of implementation who implements this interface
     */    
	inline static void ListAllImplementationsL(RImplInfoPtrArray& aImplInfoArray) ;
	
protected:

    /**
     * Default constructor to zero initialize
     * the iEcomDtorID member.
     */
    inline MApiDataObjFactoryAccessor();

    
                                        
private: // Data

    //OWN: ECom destructor ID
    TUid iEcomDtorID;
    };

#include "apidataobjfactoryaccessor.inl"
#endif // M_APIDATAOBJFACTORYACCESSOR_H

