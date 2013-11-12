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
* Description:   Storage handler
*
*/



#ifndef CBSIMPORTSTORAGEMANAGER_H
#define CBSIMPORTSTORAGEMANAGER_H

#include <e32base.h>

class CBSImportStorage;
class RWriteStream;
class RFs;

/**
 *  Storage handler
 *
 *  @lib brandimporter.exe
 *  @since S60 v3.2
 */
class CBSImportStorageManager : public CBase
{
public:

    static CBSImportStorageManager* NewL();
    
    static CBSImportStorageManager* NewLC();
    
    ~CBSImportStorageManager();

    /**
     * Create a new storage and set it to current.
     *
     * @since S60 3.2
     * @return none
     */
    void CreateStorageL();
    
    /**
     * Return the count of storages
     *
     * @since S60 3.2
     * @return count of elements
     */
    TInt StorageCount();

    /**
     * Get stored storage at index or current storage.
     *
     * @since S60 3.2
     * @param aIndex index of storage to fetch. 
     * @return Stored storage
     */
    CBSImportStorage* Storage( TInt aIndex );
    CBSImportStorage* Storage();
    
    /**
     * Write stored data to Branding server
     *
     * @since S60 3.2
     * @return none
     */
    void WriteDataToServerL();
    
    
private:

    CBSImportStorageManager();

    void ConstructL();
    
    /**
     * Merge storages with same header data
     *
     * @since S60 3.2
     * @return none
     */
    void MergeStorages();
    
private: // Data

    // Own. Array of stored storages.
    RPointerArray<CBSImportStorage> iStorages;
    
    // Index of currently active storage
    TInt iCurrent;
    
};

#endif //CBSIMPORTSTORAGEMANAGER_H
