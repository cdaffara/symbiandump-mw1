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
 * Description:
 *
 */

#ifndef C_CASTORAGE_H
#define C_CASTORAGE_H

#include "caoperationparams.h"

// FORWARD DECLARATIONS
class CCaInnerQuery;
class CCaInnerEntry;
class CCaLocalizationEntry;
/**
 *  @ingroup group_catorage.
 *  Interface for storage.
 */
class CCaStorage: public CBase
    {
public:

    /**
     * Localizes one entry attribute.
     *
     * @param aLocalization localization information about entry.
     */
    virtual void LocalizeEntryL( CCaLocalizationEntry& aLocalization ) = 0;

    /**
     * Adds localization row to the localization table
     *
     * @param aLocalization localization information about entry.
     */
    virtual void AddLocalizationL( const CCaLocalizationEntry& aLocalization ) = 0;
    
    /**
     * Fetches data from database
     *
     * @param aResultContainer container for localizations information.
     */
    virtual void GetLocalizationsL( RPointerArray<CCaLocalizationEntry>& aResultContainer ) = 0;

    /**
     * Fetches data from database.
     *
     * @param aQuery The query information to select specific entries.
     * @param aResultContainer The result list of entries .
     * for a specific select.
     */
    virtual void GetEntriesL( const CCaInnerQuery* aQuery,
            RPointerArray<CCaInnerEntry>& aResultContainer ) = 0;

    /**
     * Fetches data from database by ids.
     *
     * @param aQuery The query information to select specific entries.
     * @param aResultIdArray The result list of entries' ids.
     * for a specific select.
     */
    virtual void GetEntriesIdsL( const CCaInnerQuery* aQuery,
            RArray<TInt>& aResultIdArray ) = 0;

    /**
     * Fetches parent id from database by entry ids.
     *
     * @param aEntryIdArray list of enties ids.
     * @param aParentIdArray The result list of parents' ids
     * for a specific select.
     * @param aCheckParentsParent flag if set to true then 
     * search also for parent of the parent 
     */
    virtual void GetParentsIdsL( const RArray<TInt>& aEntryIdArray,
            RArray<TInt>& aParentIdArray, 
            TBool aCheckParentsParent = ETrue ) = 0;

    /**
     * Adds or Updates data & action to the storage.
     *
     * @param aEntry The entry data to be added or updated.
     */
    virtual void AddL( CCaInnerEntry* aEntry, TBool aUpdate = EFalse ) = 0;

    /**
     * Removes data item from storage.
     *
     * @param aEntryIds The entry ids to be removed.
     */
    virtual void RemoveL( const RArray<TInt>& aEntryIds ) = 0;

    /**
     * Organizes data within a group in the storage.
     *
     * @param aEntryIds Ids of entries to organize.
     * @param aParams Operation parameters to specify the organize type.
     */
    virtual void OrganizeL( const RArray<TInt>& aEntryIds,
            TCaOperationParams aParams ) = 0;

    /**
     * Stores touch data information in storage.
     *
     * @param aEntryId The entry id for which touch data should be stored.
     * @param aRemovable if entry is removable then flag is set if necessary.
     */
    virtual void TouchL( const TInt aEntryId, TBool aRemovable ) = 0;

    /**
     * Get database property from db.
     *
     * @param aProperty The property to get.
     * @param aPropertyValue The value of property.
     */
    virtual void DbPropertyL( const TDesC& aProperty, TDes& aPropertyValue ) = 0;

    /**
     * Set property .
     *
     * @param aProperty The property to set.
     * @param aPropertyValue The value of property.
     */
    virtual void SetDBPropertyL( const TDesC& aProperty, const TDesC& aPropertyValue ) = 0;

    /**
     * Remove from localization table .
     *
     * @param aEntryId Entry Id to remove.
     */
    virtual void RemoveFromLocalizationL( const TInt aEntryId ) = 0;

    /**
     * Organizes data within a group in custom order.
     *
     * @param aEntryIds Ids of entries in custom order.
     * @param aGroupId Group to sort.
     */
    virtual void CustomSortL( const RArray<TInt>& aEntryIds,
            const TInt aGroupId ) = 0;

    
    /**
    * Saves a copy of database to private.
    */
    virtual void RestoreDatabaseL() = 0;
        
    /**
     * Saves a copy of database to private.
     */
    virtual void SaveDatabaseL() = 0;
    
    /**
     * Loads data base from rom.
     */
    virtual void LoadDataBaseFromRomL() = 0;

    };

#endif //C_CASTORAGE_H
