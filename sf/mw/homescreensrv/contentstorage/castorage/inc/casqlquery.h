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
 * Description: CSqlQuery declaration
 *
 */
#ifndef CASQLQUERY_H
#define CASQLQUERY_H
// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <sqldb.h>
#include "caentryattribute.h"
#include "castorage_global.h"
#include "cainnerentry.h"
CA_STORAGE_TEST_CLASS(CCAStorageTest)

// CLASS DECLARATION
class RSqlDatabase;
class RSqlStatement;
class CCaInnerEntry;
class CCaInnerQuery;
class TCaOperationParams;
class CCaLocalizationEntry;
/**
 *  CSqlQuery
 *
 */
const TInt KMaxIntNumLength = 10;

/**
 *  @ingroup group_catorage.
 *  CCaSqlQuery represents a query to be executed on the sql database.
 *
 */
NONSHARABLE_CLASS( CCaSqlQuery ) : public CBase
    {
    CA_STORAGE_TEST_FRIEND_CLASS(CCAStorageTest)

public:

    /**
     * The query table types.
     */
    enum TTableType
        {
        ENoTableType,
        EEntryTable,
        EIconTable,
        EAttributeTable,
        EGroupTable,
        EGroupTableRemove,
        EGroupTableInsert,
        ELaunchTable
        };

    /**
     * The query execution result types.
     */
    enum TQueryResultType
        {
        EEntry,
        EAttribute
        };
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CCaSqlQuery();

    /**
     * Two-phased constructor.
     */
    static CCaSqlQuery* NewL( RSqlDatabase& aSql );

    /**
     * Two-phased constructor.
     */
    static CCaSqlQuery* NewLC( RSqlDatabase& aSql );

    /**
     * Set the query text.
     *
     * @param aQuery the new query text.
     */
    void SetQueryL( const TDesC& aQuery );

    /**
     * Sets the table type on which the sql operation
     * will be performed for this query.
     *
     * @param aType the table type.
     */
    void SetTableType( TTableType aType )
        {
        iType = aType;
        }

    /**
     * Returns the table type on which the sql operation 
     * will be performed for this query.
     *
     * @return
     */
    TTableType Type()
        {
        return iType;
        }

    /**
     * Executes prepares for iStatement.
     */
    void PrepareL();

    /**
     * Closes the statement to free resources.
     */
    void CloseStatement();

    /**
     * Reset the statement for future bindings and use.
     */
    void ResetStatement();

    /**
     * Binds values for an add operation.
     *
     * @param aEntry The entry from which to get data for bindings.
     */
    void BindValuesForAddL( const CCaInnerEntry* aEntry );

    /**
     * Binds values for an get localization entry
     *
     * @param aLocalization The entry from which to get data for bindings.
     */
    void BindValuesForGetLocalizationEntryL(
        const CCaLocalizationEntry& aLocalization);
    
    /**
     * Binds values for an add and update localization entry
     *
     * @param aLocalization The entry from which to get data for bindings.
     */
    void BindValuesForLocalizationEntryL(
            const CCaLocalizationEntry& aLocalization);
    
    /**
     * Binds values for an add operation.
     *
     * @param aQuery The query from which to get data for bindings.
     */
    void BindValuesForGetEntriesL( const CCaInnerQuery* aQuery );

    /**
     * Binds values for an add operation.
     *
     * @param aEntry The entry from which to get data for bindings.
     */
    void BindValuesForGetEntriesL( const CCaInnerEntry* aEntry );

    /**
     * Binds values for an add operation.
     *
     * @param aEntry The entry from which to get data for bindings.
     */
    void BindValuesForGetIconL( const CCaInnerEntry* aEntry );

    /**
     * Binds the entry ids from a given entry array.
     *
     * @param aEntryArray An array of entries which 
     * ids to bind with the statement.
     */
    void BindEntryIdsL( const RPointerArray<CCaInnerEntry>& aEntryArray );

    /**
     * Binds the entry ids from a given entry id array.
     *
     * @param aEntryIdArray An array of entry ids to bind with the statement.
     * @param aColName column name with specified id.
     */
    void BindIdsL( const RArray<TInt>& aEntryIdArray,
            const TDesC& aColName );

    /**
     * Bind one entry id to the statement.
     *
     * @param aEntryId the id to bind.
     */
    void BindEntryIdL( const TInt& aEntryId );

    /**
     * Binds values for an add attributes operation.
     *
     * @param aEntry The entry from which to get data for bindings.
     * @param aAttr The attribute data to bind.
     */
    void BindValuesForAddAttributeL( const CCaInnerEntry* aEntry,
            const CCaEntryAttribute* aAttr );

    /**
     * Binds values for a launch operation.
     *
     * @param aEntryId The entry id to bind.
     * @param aLaunchTime the launch time to bind.
     */
    void BindValuesForLaunchL( const TInt aEntryId,
            const TInt64 aLaunchTime );

    /**
     * Binds values for a remove operation.
     *
     * @param aEntryId The entry id to bind.
     * @param aIconId the icon id to bind.
     */
    void BindValuesForRemoveL( const TInt aEntryId, const TInt aIconId );

    /**
     * Binds values for an organize operation.
     *
     * @param aEntryIds The id array to bind.
     * @param aParams Organize operation parameters to bind.
     * @param aEntryId A concrete entry id to bind if needed.
     */
    void BindValuesForOrganizeL( const RArray<TInt>& aEntryIds,
            TCaOperationParams& aParams, const TInt aEntryId = -1 );

    /**
     * Binds values for a localize operation.
     *
     * @param aLocalization Localization data to bind
     */   
    void BindValuesForLocalizeL( CCaLocalizationEntry& aLocalization );
    
    /**
     * Binds values for property operation.
     *
     * @param aProperty Property data to bind.
     */   
    void BindValuesForPropertyL( const TDesC& aProperty );
    
    /**
     * Binds values for property update operation.
     *
     * @param aProperty Property data to bind.
     * @param aPropertyValue Property value data to bind.
     */  
    void BindValuesForPropertyUpdateL( const TDesC& aProperty, const TDesC& aPropertyValue );
    
    /**
     * Binds values for remove from localization.
     *
     * @param aEntryId.
     */ 
    void BindValuesForRemoveFromLocalizationL(  const TInt aEntryId );
    
    /**
     * Binds values for custom sort.
     *
     * @param aGroupId.
     * @param aEntryId.
     * @param aNewPostion.
     */ 
    void BindValuesForCustomSortL( const TInt aGroupId, const TInt aEntryId,
            const TInt aNewPostion );

    /**
     *
     * Executes a statement.
     *
     * @return the id of a last insert operation done.
     */
    TInt ExecuteL();

    /**
     * Executes a statement.
     *
     * @return ETrue if any data was found
     */
    TBool ExecuteEntryPresentL( );
    
    /**
     * Executes a statement.
     *
     * @param[out] aId id of single data the query statement queries for.
     * @return Entry count.
     */
    TInt ExecuteL( TInt& aId );

    /**
     * Executes a statement.
     *
     * @param[out] aId id of single data the query statement queries for.
     * @return Entry count.
     */
    TInt ExecuteL( TInt64& aId );

    /**
     * Executes a statement.
     *
     * @param aEntryArray An entry array to be populated 
     * with data specified by aResultType.
     * @return Entry count.
     */
    TInt ExecuteL( RPointerArray<CCaInnerEntry>& result,
            TQueryResultType aResultType );
    
    /**
     * Executes a statement.
     *
     * @param aResultInput An localization array to be populated.
     * @return localization count.
     */
    TInt ExecuteLocalizationsL(
            RPointerArray<CCaLocalizationEntry>& aResultInput );
    
    /**
     * Executes a statement.
     *
     * @param aPropertyValue An property value. 
     * @return count.
     */
    TInt ExecutePropertyL( TDes& aPropertyValue );
    
    /**
     * Executes a statement.
     *
     * @param aEntryArray An ids array to be populated 
     * with id specified by TableType.
     * @param aType a table type.
     * @return Entry count.
     */
    TInt ExecuteL( RArray<TInt>& aResultIdArray, TTableType aType );

    /**
     * Executes a statement.
     *
     * @param aIconAttributes an icon attributes.
     * @return Entry count.
     */
    TInt ExecuteL( CCaInnerIconDescription* aInnerIconDescription );

private:

    /**
     * Executes a statement.
     *
     * @param aEntryArray An entry array to be populated.
     * @return Entry count.
     */
    TInt ExecuteEntryL( RPointerArray<CCaInnerEntry>& aEntryArray );

    /**
     * Executes a statement.
     *
     * @param aEntryArray An entry array to be populated.
     * @return Entry count.
     */
    TInt ExecuteAttributeL( RPointerArray<CCaInnerEntry>& aEntryArray );

    /**
     * Constructor for performing 1st stage construction
     * @param aSql
     */
    CCaSqlQuery( RSqlDatabase& aSql );

    /**
     * default constructor for performing 2nd stage construction
     */
    void ConstructL();

    /**
     * Binds values for an add entry operation.
     *
     * @param aEntry The entry from which to get data for bindings.
     */
    void BindValuesForAddEntryL( const CCaInnerEntry* aEntry );

    /**
     * Binds values for an add icon operation.
     *
     * @param aEntry The entry from which to get data for bindings.
     */
    void BindValuesForAddIconL( const CCaInnerEntry* aEntry );

    /**
     * Binds a text value to iStatement.
     *
     * @param aParameterIndex The parameter index to bind to.
     * @param aParameterValue The text to bind.
     */
    void BindTextL( TInt aParameterIndex,
            const TDesC& aParameterValue );

    /**
     * Binds a int value to iStatement.
     *
     * @param aParameterIndex The parameter index to bind to.
     * @param aParameterValue The number to bind.
     */
    void BindIntL( TInt aParameterIndex,
            TInt aParameterValue );

    /**
     * Binds a int64 value to iStatement.
     *
     * @param aParameterIndex The parameter index to bind to.
     * @param aParameterValue The number to bind.
     */
    void BindInt64L( TInt aParameterIndex,
            TInt64 aParameterValue );

    /**
     * Gets the column index for a given column name.
     *
     * @param aStmt The statement to search for a column index.
     * @param aColumnName The name of the column.
     * @return The column index.
     */
    TInt ColumnIndexL( RSqlStatement& aStmt, 
            const TDesC& aColumnName ) const;

    /**
     * Sets attributes to entries.
     *
     * @param aResultInput A list of entries for which to set attributes.
     */
    void SetAttributeToEntriesL(
            RPointerArray<CCaInnerEntry>& aResultInput );

    /**
     * Own.
     * The query text to be executed.
     */
    RBuf iQuery;

    /**
     * The table type on which teh operation is done.
     * @return
     */
    TTableType iType;

    /**
     * The statement to be executed.
     * Own.
     */
    RSqlStatement iStatement;

    /*not own*/
    RSqlDatabase& iSqlDb;

    /**
     * The execute method result type.
     * Needs to be specified when the same result type in ExecuteL 
     * is used to perform dufferent operations.
     */
    TQueryResultType iQueryResultType;

    };

#endif // CASQLQUERY_H
