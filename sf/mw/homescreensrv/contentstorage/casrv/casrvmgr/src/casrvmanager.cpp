/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: casrvmanager.cpp
 *
 */

#include <mmf/common/mmfcontrollerpluginresolver.h> // for CleanupResetAndDestroyPushL
#include "casrvplugin.h"
#include "casrvmanager.h"

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CCaSrvManager::CCaSrvManager()
    {
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CCaSrvManager::~CCaSrvManager()
    {
    THashMapIter<TInt32, CCaSrvPlugin*> iter( iPluginMap );
    for( CCaSrvPlugin* const * ptr =
            (CCaSrvPlugin* const * ) iter.NextValue(); ptr; ptr
            = (CCaSrvPlugin* const * ) iter.NextValue() )
        {
        delete (CCaSrvPlugin* ) ( *ptr );
        }
    iPluginMap.Close();

    delete iPluginParams;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CCaSrvManager* CCaSrvManager::NewLC( CCaStorageProxy& aCaStorageProxy,
        Usif::RSoftwareComponentRegistry* aSoftwareRegistry,
        CCaSrvEngUtils* aUtils )
    {
    CCaSrvManager* self = new ( ELeave ) CCaSrvManager();
    CleanupStack::PushL( self );
    self->ConstructL( aCaStorageProxy, aSoftwareRegistry, aUtils);
    return self;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C CCaSrvManager* CCaSrvManager::NewL( CCaStorageProxy& aCaStorageProxy,
        Usif::RSoftwareComponentRegistry* aSoftwareRegistry,
        CCaSrvEngUtils* aUtils )
    {
    CCaSrvManager* self = CCaSrvManager::NewLC( aCaStorageProxy,
	        aSoftwareRegistry, aUtils );
    CleanupStack::Pop(); // self;
    return self;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCaSrvManager::ConstructL( CCaStorageProxy& aCaStorageProxy,
        Usif::RSoftwareComponentRegistry* aSoftwareRegistry,
        CCaSrvEngUtils* aUtils )
    {
    iPluginParams = new TPluginParams();
    iPluginParams->storageProxy = &aCaStorageProxy;
    iPluginParams->engUtils = aUtils;
    iPluginParams->softwareRegistry = aSoftwareRegistry;
    LoadPluginsL();
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCaSrvManager::LoadOperationErrorCodeL()
    {
    return iErrorCode;
    }

// ---------------------------------------------------------------------------
// CASpaPluginManager::LoadPluginsL
// Load plugins implementations
// ---------------------------------------------------------------------------
//
void CCaSrvManager::LoadPluginsL()
    {
    // Read info about all implementations into infoArray
    RImplInfoPtrArray infoArray;

    // Note that a special cleanup function is required to reset and destroy
    // all items in the array, and then close it.
    CleanupResetAndDestroyPushL( infoArray );
    CCaSrvPlugin::ListAllImplementationsL( infoArray );

    // Loop through each info for each implementation
    // and create and use each in turn
    CCaSrvPlugin* plug;
    for( TInt i = 0; i < infoArray.Count(); i++ )
        {
        // Slice off first sub-section in the data section
        TUid current_plugin = infoArray[i]->ImplementationUid();
        TRAPD(err, plug = CCaSrvPlugin::NewL( current_plugin, iPluginParams ));
        if( KErrNone==iErrorCode )
        	{
        	// remember error code only if it wasn't set yet
        	iErrorCode = err;
        	}
        CleanupStack::PushL( plug );
        TInt32 key = current_plugin.iUid;
        iPluginMap.InsertL( key, plug );
        CleanupStack::Pop( plug );
        plug = NULL;
        }
    CleanupStack::PopAndDestroy( &infoArray );
    }


// End of file
