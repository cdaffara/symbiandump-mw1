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
 * Description: Interface for CA database implementations
 *
 */

#ifndef CASTORAGEPROXY_H
#define CASTORAGEPROXY_H

#include "cadef.h"
#include "castorage_global.h"
#include "caoperationparams.h"



// FORWARD DECLARATIONS
class CCaStorage;
class CCaInnerQuery;
class CCaInnerEntry;
class CCaLocalizationEntry;
class MCaSessionNorifier;

CA_STORAGE_TEST_CLASS( TestCaClient )

/**
 * Interface for CA database implementations.
 *
 */
class CCaStorageProxy: public CBase
    {
public:

    /**
     * Two-phased constructor.
     */
    static CCaStorageProxy* NewL();

    /**
     * Two-phased constructor.
     */
    static CCaStorageProxy* NewLC();

    /**
     * Destructor.
     */
    ~CCaStorageProxy();

    /**
     * Localizes one entry attribute
     *
     * @param aLocalization information about one localization row.
     */
    IMPORT_C void LocalizeEntryL( CCaLocalizationEntry& aLocalization );

    /**
     * Add localization row to the storage
     *
     * @param aLocalization entry containing localization row to be added
     */
    IMPORT_C void AddLocalizationL(const CCaLocalizationEntry& aLocalization);
    
    /**
     * Fetches localization data from database
     *
     * @param aResultContainer Target for results.
     */
    IMPORT_C void GetLocalizationsL(
            RPointerArray<CCaLocalizationEntry>& aResultContainer );

    /**
     * Fetches data from storage.
     * @param aQuery Query filter for entries to be fetched.
     * @param aResult Result entry container.
     */
    IMPORT_C void GetEntriesL( const CCaInnerQuery* aQuery,
            RPointerArray<CCaInnerEntry>& aResultContainer );

    /**
     * Fetches entry ids from storage.
     * @param aQuery Query filter for entries' ids to be fetched.
     * @param aResultIdArray Result entry id array.
     */
    IMPORT_C void GetEntriesIdsL( const CCaInnerQuery* aQuery,
            RArray<TInt>& aResultIdArray );

    /**
     * Adds or Updates data to storage.
     * In case when data is added the input aEntry serves.
     * also as an output entry.
     * @param aEntry Entry data.
     * @param aUpdate is entry updated by a client.
     * @param aItemAppearanceChange a type of change of the item.
     */
    IMPORT_C void AddL( CCaInnerEntry* aEntry,
        TBool aUpdate = EFalse,
        TItemAppearance aItemAppearanceChange = EItemAppearanceNotChanged );

    /**
     * Removes entries from storage.
     * @param aEntryIds Ids of entries to be removed.
     */
    IMPORT_C void RemoveL( const RArray<TInt>& aEntryIds );

    /**
     * Organizes entries in storage.
     * @param aEntryIds Entry ids to be organized
     * (insert, remove, append, prepend are possible).
     * @param aParams Organize parameters.
     */
    IMPORT_C void OrganizeL( const RArray<TInt>& aEntryIds,
            TCaOperationParams aParams );

    /**
     * Executes touch specific actions on storage.
     * Should be called when e.g. entry is clicked.
     * @param aEntry entry to be touched
     */
    IMPORT_C void TouchL( CCaInnerEntry* aEntry );

    /**
     * Get database property on storage.
     *
     * @param aProperty The property to get.
     * @param aPropertyValue The value of property.
     */
    IMPORT_C void DbPropertyL( const TDesC& aProperty, TDes& aPropertyValue );

    /**
     * Set property action on storage.
     *
     * @param aProperty The property to set.
     * @param aPropertyValue The value of property.
     */
    IMPORT_C void SetDBPropertyL( const TDesC& aProperty,
            const TDesC& aPropertyValue );

    /**
     * Organizes data within a group in custom order in storage.
     *
     * @param aEntryIds Ids of entries in custom order.
     * @param aGroupId Group to sort.
     */
    IMPORT_C void CustomSortL( const RArray<TInt>& aEntryIds,
            const TInt aGroupId );

    
    /**
     * Saves a copy of database to private.
     */
	IMPORT_C void SaveDatabaseL();
	
    /**
     * Marks db to be restored from backup by next restart
     */
	IMPORT_C void RestoreDatabaseL();
    
    
    /**
     * Loads data base from rom.
     */
    IMPORT_C void LoadDataBaseFromRomL();

    /**
     * Add a session. Ownership not taken, this object only keeps a list of
     * sessions that are dependent on its engine.
     * @param aSession Session.
     */
    void AddSessionL( MCaSessionNorifier* aHandlerNotifier );

    /**
     * Remove a session. Safe to call if not added.
     * When no more sessions remain dependent on this object, timed
     * self-deletion is scheduled.
     */
    void RemoveSession( MCaSessionNorifier* aHandlerNotifier );

private:

    /**
     * Perform the second phase construction of a CCpStorageEngine object.
     */
    void ConstructL();

    /**
     * C++ default constructor.
     */
    CCaStorageProxy();
    
    CCaLocalizationEntry* LocalizeTextL( CCaInnerEntry* aEntry );
    
    CCaLocalizationEntry* LocalizeDescriptionL( CCaInnerEntry* aEntry );
    
    void AddTitleNameL( CCaInnerEntry* aEntry );
    
    TBool InitializeTranslatorL( const TDesC& aQmFilename );

private:
    //Data

    /**
     * Data storage. Own.
     */
    CCaStorage* iStorage;

    /**
     * Sessions using this engine. Own.
     */
    RPointerArray<MCaSessionNorifier> iHandlerNotifier;

    /**
     * Translated user collection name. Own.
     */
    RBuf iTitleUserColName;
    
    CA_STORAGE_TEST_FRIEND_CLASS( TestCaClient )
    
    };

#endif //CASTORAGEPROXY_H
