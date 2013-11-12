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
* Description:  Base class for API implementations.
*
*/


#include <s32strm.h>

#include "ximpapidataobjfactory.h"
#include "ximpapidataobjbase.h"

// to access the internal factory methods of feature plug-ins
#include "ximpfeaturepluginholder.h"
#include "apidataobjfactoryaccessor.h"
#include "apidataobjfactory.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// XIMPApiDataObjFactory::InternalizeL
// ---------------------------------------------------------------------------
//
EXPORT_C void XIMPApiDataObjFactory::InternalizeL( 
        RReadStream& aStream,
        RPrReqParamArray& aArray )
    {
    TInt32 arrLen = aStream.ReadInt32L();

    for ( TInt i = 0; i < arrLen; i++ )
        {
        SXIMPReqParam param;
        param.iParamType = aStream.ReadInt32L();
        CXIMPApiDataObjBase* dataObject = NewDataObjectFromStreamLC( param.iParamType, aStream );

        // Try to create presence data object
        MApiDataObjFactoryAccessor* presenceObjFactoryAcc = FeaturePluginHolder::PresenceObjFactoryAccessor();
        if ( !dataObject && presenceObjFactoryAcc )
            {
            dataObject = presenceObjFactoryAcc->ApiDataObjFactory().NewFromStreamLC( param.iParamType, aStream );
            }
 
        if ( !dataObject )
	        {
	        TInt accesorCount = FeaturePluginHolder::AccessorCount() ;
	        for (TInt i=0; i<accesorCount; i++)
		        {
	        	MApiDataObjFactoryAccessor* pFactoryAcc = FeaturePluginHolder::Accessor(i) ;
	        	dataObject = pFactoryAcc->ApiDataObjFactory().NewFromStreamLC( param.iParamType, aStream );
	        	if(dataObject)
	    	    	{
	        		break ;
	        		}
	        	}
        	}
        
        
        param.iParam = dataObject;
        aArray.AppendL( param );
        CleanupStack::Pop( dataObject );
        }
    }

// ---------------------------------------------------------------------------
// XIMPApiDataObjFactory::ExternalizeL
// ---------------------------------------------------------------------------
//
EXPORT_C void XIMPApiDataObjFactory::ExternalizeL( 
            RWriteStream& aWs,
            const RPrReqParamArray& aArray )
    {
    // write array length
    aWs.WriteInt32L( aArray.Count() );

    for ( TInt i = 0; i < aArray.Count(); i++ )
        {
        // trust the type within the param struct
        aWs.WriteInt32L( aArray[ i ].iParamType );

        // write the actual object
        CXIMPApiDataObjBase* theBase = aArray[ i ].iParam;
        theBase->ExternalizeL( aWs );
        }

    aWs.CommitL();
    }

// ---------------------------------------------------------------------------
// XIMPApiDataObjFactory::InternalizeLC
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPApiDataObjBase* XIMPApiDataObjFactory::InternalizeLC(
        RReadStream& aStream )
    {
    TInt32 objType = aStream.ReadInt32L();
    CXIMPApiDataObjBase* dataObject = NewDataObjectFromStreamLC( objType, aStream );
    return dataObject;
    }

// ---------------------------------------------------------------------------
// XIMPApiDataObjFactory::ExternalizeL
// ---------------------------------------------------------------------------
//
EXPORT_C void XIMPApiDataObjFactory::ExternalizeL(
        RWriteStream& aWs,
        const CXIMPApiDataObjBase& aDataObj )
    {
    aWs.WriteInt32L( aDataObj.Base().GetInterfaceId() );
    aDataObj.ExternalizeL( aWs );
    aWs.CommitL();
    }

// End of file

