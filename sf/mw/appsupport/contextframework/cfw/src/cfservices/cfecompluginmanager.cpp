/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFEComPluginManager class description.
*
*/


#include "cfecompluginmanager.h"
#include "cftrace.h"

// MEMBER FUNCTIONS

// ---------------------------------------------------------------------------
// CCFEComPluginManager::~CCFEComPluginManager
// ---------------------------------------------------------------------------
//
EXPORT_C CCFEComPluginManager::~CCFEComPluginManager()
    {
    FUNC_LOG;

    iUidArray.Close();
    }


// ---------------------------------------------------------------------------
// CCFEComPluginManager::AddToBlackList
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCFEComPluginManager::AddToBlackList( const TUid& aImplUid )
    {
    FUNC_LOG;
    INFO_1( "Adding implementation UID 0x%08x to blacklist", aImplUid.iUid );

    TInt err = iUidArray.InsertInSignedKeyOrder( aImplUid );
    if ( err == KErrAlreadyExists )
        {
        err = KErrNone; // Ignore duplicates
        }

    ERROR( err, "Failed to add UID to blacklist" );
    return err;
    }


// ---------------------------------------------------------------------------
// CCFEComPluginManager::RemoveBlackListed
// ---------------------------------------------------------------------------
//
EXPORT_C void CCFEComPluginManager::RemoveBlackListed(
    RImplInfoPtrArray& aImplInfoArray )
    {
    FUNC_LOG;

    TInt numUids = aImplInfoArray.Count();
    for( TInt i = numUids - 1; i >= 0 ; i-- ) // elements are removed from
        {  // array so it's safer to start from the end.
        CImplementationInformation* implInfo = aImplInfoArray[i];
        if ( iUidArray.FindInSignedKeyOrder( implInfo->ImplementationUid() )
             != KErrNotFound )
            {
            INFO_1( "Implementation UID 0x%08x is on blacklist", implInfo->ImplementationUid().iUid );

            delete implInfo;
            aImplInfoArray.Remove( i );
            }
        }
    }


// ---------------------------------------------------------------------------
// CCFEComPluginManager::CCFEComPluginManager
// ---------------------------------------------------------------------------
//
EXPORT_C CCFEComPluginManager::CCFEComPluginManager()
    {
    FUNC_LOG;

    // Nothing to do
    }

