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
* Description:  Presence Service Connection globals holder
 *
*/


#include <e32base.h>
#include "ximpfeaturepluginholder.h"
#include "apidataobjfactoryaccessor.h"

const TInt KPluginArrayGranularity = 3;

// ============================ HELPER CLASS ===============================


/**
 * Nested structure to hold global accessor objects.
 */

// FIXME: probably better would be to encapsulate the struct to CBase object
NONSHARABLE_STRUCT( SAccessorHolder )
    {
    /**
     * Presence obj factory accessor
     * Owned.
     */
    MApiDataObjFactoryAccessor* iPresenceAccessor;


    // array of accessor plugins 
    CArrayFixFlat<MApiDataObjFactoryAccessor*>* iAccessorPlugins;
    
    /**
     * TODO: Add here more accessors
     */
     
    /**
     * Reference counter
     */
    TInt iRefCount;
    };

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void FeaturePluginHolder::InitializeL( )
    {
    SAccessorHolder* singleton = ( SAccessorHolder* ) Dll::Tls();

    if( !singleton )
        {
        singleton = new ( ELeave ) SAccessorHolder;
        Dll::SetTls( singleton );
        
        // Initialize the variables
        singleton->iRefCount = 0;
        singleton->iPresenceAccessor = NULL;

        TRAP_IGNORE( singleton->iPresenceAccessor = MApiDataObjFactoryAccessor::NewPresenceAccessorL( ) );
        
        //added to hold accessor plugins
        singleton->iAccessorPlugins = new(ELeave)CArrayFixFlat<MApiDataObjFactoryAccessor*>(KPluginArrayGranularity);
        
        FeaturePluginHolder::LoadAccessorPluginsL() ;
        }
    else
        {
        // increase the reference counter
        singleton->iRefCount++;
        }
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C MApiDataObjFactoryAccessor* FeaturePluginHolder::PresenceObjFactoryAccessor()
    {
    SAccessorHolder* singleton = ( SAccessorHolder* ) Dll::Tls();
    if( !singleton )
        {
        User::Panic( _L("cximpglobals"), KErrNotFound );
        }

    return singleton->iPresenceAccessor;
    }


// -----------------------------------------------------------------------------
// FeaturePluginHolder::Release()
// -----------------------------------------------------------------------------
//
EXPORT_C void FeaturePluginHolder::Release( )
    {
    SAccessorHolder* singleton = ( SAccessorHolder* ) Dll::Tls();
    if ( !singleton->iRefCount )
        {
        // No more users, clean the TLS up
        delete singleton->iPresenceAccessor;
        if ( singleton->iAccessorPlugins )
	    	{
	    	TInt idx = KErrNotFound;
	    	while( singleton->iAccessorPlugins->Count() )
	        	{
		        idx = singleton->iAccessorPlugins->Count() - 1;
		        // Delete object
		        delete singleton->iAccessorPlugins->At( idx );
		        // Delete element
		        singleton->iAccessorPlugins->Delete( idx );
	        	}
	    	}	

        delete singleton->iAccessorPlugins;
        REComSession::FinalClose();
        
        delete singleton;
        Dll::SetTls(NULL);
        }
    else
        {
        singleton->iRefCount--;        
        }
    }
    
void FeaturePluginHolder::LoadAccessorPluginsL()    
{
	SAccessorHolder* singleton = ( SAccessorHolder* ) Dll::Tls();
	if(!singleton)	
	{
		User::Panic( _L("cximpglobals"), KErrNotFound );
	}
	else
	{
		RImplInfoPtrArray pluginArray;	
		MApiDataObjFactoryAccessor::ListAllImplementationsL(pluginArray) ;		
		if ( pluginArray.Count() )
		{
			for( TInt i = 0; i < pluginArray.Count(); i++ )
			{
				MApiDataObjFactoryAccessor* implementation = NULL ;
				
				CImplementationInformation* info = pluginArray[ i ];
            
            	TUid id = info->ImplementationUid();
            	TRAPD(err, implementation = MApiDataObjFactoryAccessor::NewL(id) );
            	CleanupStack::PushL(implementation);
            	if(err == KErrNone)
            	{
            		singleton->iAccessorPlugins->AppendL( implementation );
            	}
            	CleanupStack::Pop(implementation);
			}
		}
		pluginArray.ResetAndDestroy();
	}
}

EXPORT_C TInt FeaturePluginHolder::AccessorCount()
{
	SAccessorHolder* singleton = ( SAccessorHolder* ) Dll::Tls();
	if(!singleton)	
	{
		User::Panic( _L("cximpglobals"), KErrNotFound );
	}

	return singleton->iAccessorPlugins->Count() ;	
	
}

EXPORT_C MApiDataObjFactoryAccessor* FeaturePluginHolder::Accessor(TInt index) 
{
	SAccessorHolder* singleton = ( SAccessorHolder* ) Dll::Tls();
	if(!singleton)	
	{
		User::Panic( _L("cximpglobals"), KErrNotFound );
	}
	
	return singleton->iAccessorPlugins->At(index) ;
}

// End of file


