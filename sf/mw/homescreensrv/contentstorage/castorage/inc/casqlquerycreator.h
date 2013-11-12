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
 * Description: CASqlQueryCreator declaration
 *
 */

#ifndef CASQLQUERYCREATOR_H
#define CASQLQUERYCREATOR_H

// INCLUDES
#include <e32std.h>
#include "cainnerquery.h"

// CLASS DECLARATION

class CCaLocalizationEntry; 
class CCaSqlQuery;
class CCaInnerEntry;
class CCaInnerQuery;
class RSqlDatabase;
class TCaOperationParams;

/**
 *  @ingroup group_catorage.
 *  CaSqlQueryCreator Helper class which prepares sql statements for 
 *  execution.
 */
class CaSqlQueryCreator
    {
public:
    
    /**
     * The icon types.
     */
    enum TIconType
        {
        ENullIcon,
        ENullIconToRemove,
        EProperIcon
        };

    /**
     * Creates sql queries to be used to perform an add or 
     * update operation on the database.
     *
     * @param aEntry The entry to be added or updated in the database.
     * @param aList A list to fill in with queries neccessary for the operation.
     * @param iSqlDb The handle to the database needed for 
     * the created queries' member (RSqlStatement iStatement).
     */
    static void CreateAddQueryL( CCaInnerEntry* aEntry, RPointerArray<
            CCaSqlQuery>& aSqlQuery, RSqlDatabase& aSqlDb );
    /**
     * Creates an sql query to fetch data from the database.
     *
     * @param aQuery The query object which specifies 
     * the type of data to be fetched.
     * @param aSqlQuery The sql query created to select the data.
     */
    static void CreateGetEntriesQueryL( const CCaInnerQuery* aQuery,
            CCaSqlQuery* aSqlQuery );

    /**
     * Creates an sql query to fetch parents ids from the database.
     *
     * @param aEntryIdArray array with entries ids.
     * @param aSqlQuery The sql query created to select parents ids.
     * @param aParentIdArray array with groups ids.
     */
    static void CreateGetParentsIdsQueryL(
            const RArray<TInt>& aEntryIdArray, CCaSqlQuery* aSqlQuery,
            const RArray<TInt>& aParentIdArray );

    /**
     * Creates an sql query to remove data from the database.
     *
     * @param aList The result sql queries to be executed for remove.
     * @param aSqlDb The handle to the database needed for 
     * the created queries' member (RSqlStatement iStatement).
     */
    static void CreateRemoveQueryL( RPointerArray<CCaSqlQuery>& aSqlQuery,
            RSqlDatabase& aSqlDb );

    /**
     * Creates an sql query to fetch attributes for a given number of entries.
     *
     * @param aIdEntryCount The number of entry ids for which to get 
     * attributes.
     * @param aSqlQuery The result fetch attributes query.
     */
    static void CreateGetAttributesQueryL( const TInt aIdEntryCount,
            CCaSqlQuery* aSqlQuery );

    /**
     * Creates an sql query to modify touch data in the database.
     *
     * @param aSqlQuery The list of result queries needed for the touch 
     * operation.
     * @param aSqlDb The handle to the database needed for the created. 
     * @param aRemovable to create query to change used flag only for 
     * removable entries.
     * queries' member (RSqlStatement iStatement).
     */
    static void CreateTouchQueryL( RPointerArray<CCaSqlQuery>& aSqlQuery,
            RSqlDatabase& aSqlDb, TBool aRemovable );

    /**
     * Creates an sql query to organize data within a group.
     *
     * @param aEntryIds The array of entry ids which are organized.
     * @param aParams The specific parameters describing the type of 
     * organize to be done.
     * @param aSqlDb The handle to the database needed for the created 
     * queries' member (RSqlStatement iStatement).
     * @param aSqlRemoveQueries A result array of remove queries to be 
     * executed.
     * @param aSqlPrependInsertQueries A result array of prepend insert 
     * queries queries to be executed.
     * @param aSqlInsertQueries A result array of insert queries to be 
     * executed.
     */

    static TBool CreateOrganizeQueryL( const RArray<TInt>& aEntryIds,
            TCaOperationParams aParams, RSqlDatabase& aSqlDb,
            RPointerArray<CCaSqlQuery>& aSqlQueries );

    /**
     * Populates a given query with a list of ids.
     *
     * @param The number of ids with which to populate the query.
     * @param aQuery The query where to set the result query text.
     * @param aSqlQueryText the sql text wich is populated 
     * with ids (in place of %S sequence);
     */
    static void PopulateQueryWithIdListL( const TInt aIdEntryCount,
            CCaSqlQuery* aQuery, const TDesC& aSqlQueryText );
    
    /**
     * Creates an sql query to add, update or get data from localization table.
     *
     * @param aSqlQuery The result query.
     * @param aStatement statement
     */
    static void CreateLocalizationTableQueryL(CCaSqlQuery* aSqlQuery,
            const TDesC& aStatement);
    
    /**
     * Creates an sql query to custom sort order.
     *
     * @param aEntryIds The array of entry ids.
     * @param aSqlQuery A result array of insert queries to be executed.
     * @param aSqlDb The handle to the database. 
     */
    static void CreateCustomSortQueryL( const RArray<TInt>& aEntryIds,
            RPointerArray<CCaSqlQuery>& aSqlQuery,
            RSqlDatabase& aSqlDb );

    /**
     * Creates an sql query for finding an icon.
     *
     * @param aEntry The entry which has icon description.
     * @param aQuery The result query.
     */
    static void CreateFindIconQueryL( CCaInnerEntry* aEntry,
            CCaSqlQuery* aQuery);
    
private:

    /**
     * Creates an sql query to add an icon into the icon table.
     *
     * @param aEntry the entry for which to add an icon.
     * @param aQuery The result query.
     * @param aSqlDb The handle to the database needed for 
     * the created queries' member (RSqlStatement iStatement).
     */
    static void CreateAddIconQueryL( CCaInnerEntry* aEntry,
            CCaSqlQuery* aQuery, RSqlDatabase& aSqlDb );

    /**
     * Creates an sql query to add an icon for a new entry.
     *
     * @param aEntry the entry for which to add an icon.
     * @param aQuery The result query.
     * @param aSqlDb The handle to the database needed for 
     * the created queries' member (RSqlStatement iStatement).
     */
    static void CreateAddIconQueryForNewL( CCaInnerEntry* aEntry,
            CCaSqlQuery* aQuery, RSqlDatabase& aSqlDb );

    /**
     * Creates an sql query to add an icon for an existing entry.
     *
     * @param aEntry the entry for which to add an icon.
     * @param aQuery The result query.
     * @param aSqlDb The handle to the database needed for 
     * the created queries' member (RSqlStatement iStatement).
     */
    static void CreateAddIconQueryForUpdateL( CCaInnerEntry* aEntry,
            CCaSqlQuery* aQuery, RSqlDatabase& aSqlDb );

    /**
     * Creates an sql query to update an icon for an existing entry.
     *
     * @param aEntry the entry for which to update an icon.
     * @param aQuery The result query.
     */
    static void CreateUpdateIconQueryL( const CCaInnerEntry* aEntry,
            CCaSqlQuery* aQuery );

    /**
     * Creates an sql query to update an entry.
     *
     * @param aEntry The entry to be updated.
     * @param aQuery The result query.
     */
    static void CreateUpdateQueryEntryL( const CCaInnerEntry* aEntry,
            CCaSqlQuery* aQuery );
    
    /**
     * Creates an sql query to get a list of entries by the given ids.
     *
     * @param aIdEntryCount The number of entries to fetch.
     * @param aQuery The result query.
     */
    static void CreateGetEntriesQueryByIdL( const TInt aIdEntryCount,
            CCaSqlQuery* aQuery );

    /**
     * Creates an sql query to get a list of entries by query data.
     *
     * @param aQuery The query to analize for 
     * a specific selection (result list of entries).
     * @param aSqlQuery The result query.
     */
    static void CreateGetEntriesQueryByQueryL( const CCaInnerQuery* aQuery,
            CCaSqlQuery* aSqlQuery );

    /**
     * Helper function which creates a string with 
     * a list of id mappings. e.g. (id0,id1,id2)
     *
     * @param aIdEntryCount the number of entries to create the id list.
     * @param aEntryIdList The result text.
     * @param aColName column name with specified id.
     */
    static void CreateIdListL( const TInt aIdEntryCount,
            RBuf& aEntryIdList, const TDesC& aColName );

    /**
     * Modifies a given query with sort order information.
     *
     * @param aSortType The sort order type.
     * @param aQuery the query where the sort order will be included.
     * @param aSortByGroup Additional information if to sort within a group.
     */
    static void ModifyQueryForSortOrderL( 
            const CCaInnerQuery::TSortAttribute aSortType, 
            RBuf& aQuery, TBool aSortByGroup );
    
    /**
     * Modifies a given query with sort order last or most used.
     *
     * @param aSortType The sort order type.
     * @param aQuery the query where the sort order will be included.
     */
    static void ModifyQueryForSortOrderLastMostUsedL(
            const CCaInnerQuery::TSortAttribute aSortType, RBuf& aQuery );
    
    /**
     * Check if icon is set proper in CCaInnerEntry.
     *
     * @param aEntry The entry with icon params.
     * @return TIconType .
     */
    static TIconType CheckIconType( const CCaInnerEntry* aEntry );

    };

#endif // CASQLQUERYCREATOR_H
