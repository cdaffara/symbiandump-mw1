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
* Description:  Presence service operation factory
 *
*/


#include "ximpoperationfactoryimp.h"
#include "ximpoperationdefs.h"
#include "ximpoperationbind.h"
#include "ximpoperationbind2.h"
#include "ximpoperationunbind.h"
#include "ximpoperationforcedteardown.h"
#include "ximppanics.h"

#include "presenceoperationfactoryimp.h"
#include "ximptrace.h"
#include "ximpoperationfactory1.h"

using namespace NXIMPOps;

const TInt KPluginArrayGranularity = 3;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CXIMPOperationFactory::NewL()
// Singleton access method.
// -----------------------------------------------------------------------------
//
EXPORT_C CXIMPOperationFactory* CXIMPOperationFactory::NewL()
    {
    CXIMPOperationFactory* self = new ( ELeave ) CXIMPOperationFactory();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CXIMPOperationFactory::CXIMPOperationFactory()
// -----------------------------------------------------------------------------
//
CXIMPOperationFactory::CXIMPOperationFactory()
    {
    }

// -----------------------------------------------------------------------------
// CXIMPOperationFactory::~CXIMPOperationFactory()
// -----------------------------------------------------------------------------
//
CXIMPOperationFactory::~CXIMPOperationFactory()
    {
    #if _BullseyeCoverage
    cov_write();
    #endif
    delete iPresenceOperation;
    //delete iImOperation;
    if ( iOperationPlugins )
    {
    	TInt idx = KErrNotFound;
    	while( iOperationPlugins->Count() )
        {
	        idx = iOperationPlugins->Count() - 1;
	        // Delete object
	        delete iOperationPlugins->At( idx );
	        // Delete element
	        iOperationPlugins->Delete( idx );
        }
    }

  	delete iOperationPlugins;
    REComSession::FinalClose();
    }


// -----------------------------------------------------------------------------
// CXIMPOperationFactory::ConstructL()
// -----------------------------------------------------------------------------
//
void CXIMPOperationFactory::ConstructL()
    {
        iPresenceOperation = CPresenceOperationFactory::NewL();
        
        //TRAPD(err, iImOperation = MXIMPOperationFactory1::NewL());
        
        iOperationPlugins = new(ELeave)CArrayFixFlat<MXIMPOperationFactory1*>(KPluginArrayGranularity);
        
        LoadOperationPluginsL() ;
    }


// -----------------------------------------------------------------------------
// CXIMPOperationFactory::CreateOperationL()
// -----------------------------------------------------------------------------
//
CXIMPOperationBase* CXIMPOperationFactory::CreateOperationL( TInt aOperation )
    {
    CXIMPOperationBase* op = NULL;
    NXIMPOps::TXIMPOpTypes operation = ( NXIMPOps::TXIMPOpTypes ) aOperation;
    switch( operation )
        {
        case EXIMPBindContext:
            {
            op = new ( ELeave ) CXIMPOperationBind();
            TRACE( _L("Operation Bind created") );
            break;
            }

        case EXIMPBindContext2:
            {
            op = new ( ELeave ) CXIMPOperationBind2();
            TRACE( _L("Operation Bind2 created") );
            break;
            }


        case EXIMPUnbindContext:
            {
            op = new ( ELeave ) CXIMPOperationUnbind();
	        TRACE( _L("Operation Unbind created") );
            break;
            }

        case EXIMPForcedTeardown:
            {
            op = new ( ELeave ) CXIMPOperationForcedTeardown();
	        TRACE( _L("Operation ForcedTeardown created") );
            break;
            }
        default:
        	{
 			
 			// this for time being, change presence interface
        	op = iPresenceOperation->CreateOperationL( aOperation );
        	if(!op)
        		{
				TInt count = iOperationPlugins->Count() ;
	        	
	        	for(TInt i=0; i<count;i++)
	        		{
	        		op = iOperationPlugins->At(i)->CreateOperationL(aOperation) ;
	        		if(op)
	        			{
	        			// got the operation object, no need to pass it to other plugins
	        			break;
	        			}
	        		}
        		}
         		break;
        	}
        }
    return op;
    }
 
void CleanupEComArray(TAny* aArray)
	{
	(static_cast<RImplInfoPtrArray*> (aArray))->ResetAndDestroy();
	(static_cast<RImplInfoPtrArray*> (aArray))->Close();
	}    
// -----------------------------------------------------------------------------
// CXIMPOperationFactory::LoadOperationPluginsL()
// -----------------------------------------------------------------------------
//
void CXIMPOperationFactory::LoadOperationPluginsL() 
{
	RImplInfoPtrArray pluginArray;		
	MXIMPOperationFactory1::ListAllImplementationsL(pluginArray) ;	
	if ( pluginArray.Count() )
	{
		for( TInt i = 0; i < pluginArray.Count(); i++ )
		{
			MXIMPOperationFactory1* implementation = NULL ;			
			CImplementationInformation* info = pluginArray[ i ];        
        	TUid id = info->ImplementationUid();        	
        	TRAPD(err, implementation = MXIMPOperationFactory1::NewL(id) );
        	CleanupStack::PushL(implementation);
        	if(err == KErrNone)
        	{
        		iOperationPlugins->AppendL( implementation );
        	}
        	CleanupStack::Pop(implementation);
		}
	}
	pluginArray.ResetAndDestroy();
}


// End of file









