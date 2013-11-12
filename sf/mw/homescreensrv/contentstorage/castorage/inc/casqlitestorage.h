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
 * Description:  Database implementation
 *
 */

#ifndef C_CASTORAGE_ENGINE_H
#define C_CASTORAGE_ENGINE_H

#include <bautils.h>
#include <e32cons.h>
#include <sqldb.h>
#include "castorage.h"
#include "castorage_global.h"
CA_STORAGE_TEST_CLASS(CCAStorageTest)

// FORWARD DECLARATIONS
class CaSqlQueryCreator;
class RCaEntryAttrArray;
class CCaSqlQuery;
class TCaOperationParams;
class RFs;
class CCaLocalizationEntry;

// CLASS DECLARATION
/**
 *  SQLite based databse for Content Publisher
 *
 *  @lib castorage.dll
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CCaSqLiteStorage ): public CCaStorage
    {
    CA_STORAGE_TEST_FRIEND_CLASS(CCAStorageTest)

public:

    /**
     * Two-phased constructor.
     */
    static CCaSqLiteStorage* NewL();

    /**
     * Two-phased constructor.
     */
    static CCaSqLiteStorage* NewLC();

    /**
     * Destructor.
     */
    ~CCaSqLiteStorage();

    //from CCpStorage
    /**
     * Loads data base from rom.
     */
    void LoadDataBaseFromRomL();
    
    //from CCpStorage
    /**
     * Saves a copy of database to private.
     */
    void SaveDatabaseL();

    //from CCpStorage
    /**
     * Restores a copy of database from backup to private.
     */
    void RestoreDatabaseL();

    /**
     * Localizes one entry attribute.
     *
     * @param aLocalization Localization information.
     */
    void LocalizeEntryL( CCaLocalizationEntry& aLocalization );

    /**
     * Adds or updates localization row in the localization table
     *
     * @param aLocalization localization information about entry.
     */
    void AddLocalizationL( const CCaLocalizationEntry& aLocalization );
    
    /**
     * Checks if entry already exists in localization table
     *
     * @param aLocalization localization information about entry.
     */
    TBool LocalizationEntryPresentL(const CCaLocalizationEntry& aLocalization);

    /**
     * Updates localization row in the localization table
     *
     * @param aLocalization localization information about entry.
     * @param aStatement SQL statement
     */
    void ExecuteLocalizationStatementL(const CCaLocalizationEntry& aLocalization, 
            const TDesC& aStatement);
   
    /**
     * Fetches data from database.
     *
     * @param aMap Filtering and sorting criteria
     */
    void GetLocalizationsL(
            RPointerArray<CCaLocalizationEntry>& aResultContainer );

    /**
     * Fetches data from database.
     *
     * @param aQuery .
     * @param aResultContainer Target for results.
     */
    void GetEntriesL( const CCaInnerQuery* aQuery,
            RPointerArray<CCaInnerEntry>& aResultContainer );

    /**
     * Fetches entry ids from database.
     *
     * @param aQuery Filtering and sorting criteria.
     * @param aResultIdArray Target for results.
     */
    void GetEntriesIdsL( const CCaInnerQuery* aQuery,
            RArray<TInt>& aResultIdArray );

    /**
     * Fetches parent id from database by entry ids.
     *
     * @param aEntryIdArray List of enties ids.
     * @param aParentIdArray The result list of parents
     * ids for a specific select.
     * @param aCheckParentsParent falg if set to true then 
     * search also for parent of the parent 
     */
    void GetParentsIdsL( const RArray<TInt>& aEntryIdArray,
            RArray<TInt>& aParentIdArray, 
            TBool aCheckParentsParent = ETrue );

    /**
     * Adds or Updates data & action to database.
     * @param aEntry Map Containing data item.
     * @param aUpdate true means there goes an update
     * @return id of added entry.
     */
    void AddL( CCaInnerEntry* aEntry, TBool aUpdate = EFalse);

    /**
     * Remove data item from database.
     * @param aEntryIds Filtering and sorting criteria.
     */
    void RemoveL( const RArray<TInt>& aEntryIds );

    /**
     * Organize data.
     * @param aEntryIds Ids array.
     * @param aParams TCaOperationParams describe operation type.
     */
    void OrganizeL( const RArray<TInt>& aEntryIds,
            TCaOperationParams aParams );

    /**
     * Add ifno launch to db.
     * @param aEntryId.
     * @param aRemovable.
     */
    void TouchL( const TInt aEntryId, TBool aRemovable );

    /**
     * Get database property from db.
     *
     * @param aProperty The property to get.
     * @param aPropertyValue The value of property.
     */
    void DbPropertyL( const TDesC& aProperty, TDes& aPropertyValue );

    /**
     * Set property .
     *
     * @param aProperty The property to set.
     * @param aPropertyValue The value of property.
     */
    void SetDBPropertyL( const TDesC& aProperty,
        const TDesC& aPropertyValue );

    /**
     * Remove from localization table .
     *
     * @param aEntryId Entry Id to remove.
     */
    void RemoveFromLocalizationL( const TInt aEntryId );

    /**
     * Organizes data within a group in custom order.
     *
     * @param aEntryIds Ids of entries in custom order.
     * @param aGroupId Group to sort.
     */
    void CustomSortL( const RArray<TInt>& aEntryIds, const TInt aGroupId );

private:

    /**
     * Perform the second phase construction of a CCpStorageEngine
     * object.
     */
    void ConstructL();

    /**
     * C++ default constructor.
     */
    CCaSqLiteStorage();

    TInt CreatePrivateDirPath( TFileName& aPrivatePath,
            const TDesC& aDrive, const TDesC& aDbName );

    void ExecuteStatementL( const TDesC& aStatement );

    void ExecuteAddL( CCaInnerEntry* aEntry,
            RPointerArray<CCaSqlQuery>& sqlQuery );

    void ExecuteRemoveL( const RArray<TInt>& aEntryIds );

    void ExecuteRemoveFromLocalizationL( const TInt aEntryId );

    void ExecuteOrganizeL( const RArray<TInt>& aEntryIds,
            TCaOperationParams aParams );

    void ExecuteTouchL( const TInt aEntryId, TBool aRemovable );

    void ExecuteDbPropertyL( const TDesC& aProperty, TDes& aPropertyValue );

    void ExecuteSetDbPropertyL( const TDesC& aProperty,
            const TDesC& aPropertyValue );

    void ExecuteCustomSortL( const RArray<TInt>& aEntryIds,
            const TInt aGroupId,
            RPointerArray<CCaSqlQuery>& aSqlQuery );

    void RemoveOldEntriesFromLaunchTableL( TInt aDays );

    void VerifyOrganizeParamsL( const RArray<TInt>& aEntryIds,
            TCaOperationParams aParams );

    /**
     * Sets entries in proper order
     * @param aEntryIds array with ids which are ordered properly
     * @param aResultContainer array containing entries to be ordered
     */
    void SetEntriesInProperOrderL( const RArray<TInt>& aEntryIds,
            RPointerArray<CCaInnerEntry>& aResultContainer );

private:

    /**
     * Database.
     * Own.
     */
    RSqlDatabase iSqlDb;

    /**
     * Path to data base in private path on C-drive.
     */
    TFileName iPrivatePathCDriveDb;

    /**
     * Path to data base in private path on Z-drive.
     */
    TFileName iPrivatePathZDriveDb;

    /**
     * Private path on C-drive.
     */
    TFileName iPrivatePathCDrive;
    
    /**
     * Private path on C-drive for backup db.
     */
    TFileName iPrivatePathCDriveDbBackup;

    /**
     * RFs session.
     */
    RFs iRfs;

    };

#endif //C_CASTORAGE_ENGINE_H
