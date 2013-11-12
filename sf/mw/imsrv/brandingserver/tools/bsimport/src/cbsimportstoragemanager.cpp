/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:   Stores element data and writes it to stream
*
*/



#include <s32strm.h>
#include <s32file.h>
#include <f32file.h>
#include <bautils.h>
#include <cbsfactory.h>
#include <mbsupdater.h>

#include "cbsimportstoragemanager.h"
#include "cbsimportstorage.h"
#include "importlogwriter.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CBSImportStorageManager::NewL
// ---------------------------------------------------------------------------
//
CBSImportStorageManager* CBSImportStorageManager::NewL()
    {
    CBSImportStorageManager* self = NewLC();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// CBSImportStorageManager::NewLC
// ---------------------------------------------------------------------------
//
CBSImportStorageManager* CBSImportStorageManager::NewLC()
    {
    CBSImportStorageManager* self = 
        new (ELeave) CBSImportStorageManager();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CBSImportStorageManager::ConstructL
// ---------------------------------------------------------------------------
//
void CBSImportStorageManager::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// CBSImportStorageManager::CBSImportStorageManager
// ---------------------------------------------------------------------------
//
CBSImportStorageManager::CBSImportStorageManager() : 
    iCurrent( KErrNotFound )
    {
    }
    
// ---------------------------------------------------------------------------
// CBSImportStorageManager::~CBSImportStorageManager
// ---------------------------------------------------------------------------
//
CBSImportStorageManager::~CBSImportStorageManager()
    {
    iStorages.ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// CBSImportStorageManager::CreateStorageL
// ---------------------------------------------------------------------------
//
void CBSImportStorageManager::CreateStorageL()
    {
    CBSImportStorage* tmp = CBSImportStorage::NewL();
    iStorages.Append( tmp );
    iCurrent = iStorages.Find( tmp );
    }

// ---------------------------------------------------------------------------
// CBSImportStorageManager::StorageCount
// ---------------------------------------------------------------------------
//
TInt CBSImportStorageManager::StorageCount()
    {
    return iStorages.Count();
    }

// ---------------------------------------------------------------------------
// CBSImportStorageManager::Storage
// ---------------------------------------------------------------------------
//
CBSImportStorage* CBSImportStorageManager::Storage( TInt aIndex )
    {
    return iStorages[ aIndex ];
    }

// ---------------------------------------------------------------------------
// CBSImportStorageManager::Storage
// ---------------------------------------------------------------------------
//
CBSImportStorage* CBSImportStorageManager::Storage()
    {
    return iStorages[ iCurrent ];
    }

// ---------------------------------------------------------------------------
// CBSImportStorageManager::WriteDataToServerL
// ---------------------------------------------------------------------------
//
void CBSImportStorageManager::WriteDataToServerL()
    {
    // Merge storages
    MergeStorages();
    
    // Flush each storage
    TInt count = iStorages.Count();
    for( TInt i = 0; i < count; i++ )
        {
        iStorages[i]->FlushToServerL();
        }
    }

// ---------------------------------------------------------------------------
// CBSImportStorageManager::MergeStoragesL
// ---------------------------------------------------------------------------
//
void CBSImportStorageManager::MergeStorages()
    {
    for( TInt i = 0; i < iStorages.Count(); i++ )
        {
        for( TInt j = i + 1; j < iStorages.Count(); j++ )
            {
            if( iStorages[i]->Compare( iStorages[j] ) )
                {
                // they have same header data -> merge them
                iStorages[i]->TakeElements( iStorages[j] );
                delete iStorages[j];
                iStorages.Remove( j );
                j--; // correct index
                }
            }
        }
    }

// End of file
