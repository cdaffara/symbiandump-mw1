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
 * Description:  ?Description
 *
 */

#ifndef __CCAINNERQUERY_H__
#define __CCAINNERQUERY_H__

#include <e32base.h>
#include "caentryattribute.h"

//FORWARD DECLARATIONS
class CDesC16ArrayFlat;
class RWriteStream;
class RReadStream;

/**
 * Class represents Content Arsenal query
 */
NONSHARABLE_CLASS( CCaInnerQuery ): public CBase
    {

public:

    enum TSortAttribute
        {
        Name = 1,
        NameDesc,
        CreatedTimestamp,
        CreatedTimestampDesc,
        MostUsed,
        MostUsedDesc,
        LastUsed,
        LastUsedDesc,
        Default, // default - return items in custom order
        DefaultDesc
        };

    enum TEntryRole
        {
        Item = 0x1,
        Group = 0x2
        };

    /**
     * Destructor.
     */
    virtual ~CCaInnerQuery();

    /**
     * Two-phased constructor.
     * @return The created object.
     */
    IMPORT_C static CCaInnerQuery* NewL();

    /**
     * Two-phased constructor.
     * @return The created object.
     */
    IMPORT_C static CCaInnerQuery* NewLC();

    /**
     * Get query entry array ids. 
     * @return Array od entries' ids to be queried.
     */
    IMPORT_C const RArray<TInt>& GetIds() const;

    /**
     * Sets the ids to be queried, entries to be fetched from storage.
     * @param aArray The array of entries to be fetched.
     */
    IMPORT_C void SetIdsL( const RArray<TInt>& aArray );

    /**
     * Returns the parent id set for the query.
     * @return Id of entry's parent.
     */
    IMPORT_C TInt GetParentId() const;

    /**
     * Sets the parent id to be queried. This will cause returning only entries whose parent ids are aId.
     * @param aId The parent id to be set for the query.
     */
    IMPORT_C void SetParentId( TInt aId );

    /**
     * Returns the child id set for the query.
     * @return Id of entry's child.
     */
    IMPORT_C TInt GetChildId() const;

    /**
     * Sets the child id to be queried. This will cause returning parents of the entry with id aId
     * @param aId The child id to be set for the query.
     */
    IMPORT_C void SetChildId( TInt aId );
    
    
    /**
     * Gets the typenames filter set in the query.
     * @return Array of typenames set in this query. 
     */
    IMPORT_C const CDesC16ArrayFlat* GetEntryTypeNames() const;

    /**
     * Sets the typename filter for the query. 
     * @param aArray Sets the typename filter for the query.
     */
    IMPORT_C void SetEntryTypeNames( CDesC16ArrayFlat* aArray );

    /**
     * Reutrns the role of teh queried entries.
     * @return Role filter.
     */
    IMPORT_C TInt GetRole() const;

    /**
     * Sets the role filter for the query.
     * @param The role of which type items should be fetched.
     */
    IMPORT_C void SetRole( TInt aRole );

    /**
     * Gets flags which are on.
     * @return Flags which are on.
     */
    IMPORT_C TUint GetFlagsOn() const;

    /**
     * Sets flags which are on (filter).
     * @param aFlagsOn Flags which are on filter.
     */
    IMPORT_C void SetFlagsOn( TUint aFlagsOn );

    /**
     * Gets flags which are on filter.
     * @return Filter of flags which are on.
     */
    IMPORT_C TUint GetFlagsOff() const;

    /**
     * Sets flags which are off (filter).
     * @param aFlagsOff Flags which are off filter.
     */
    IMPORT_C void SetFlagsOff( TUint aFlagsOff );

    /**
     * The uid filter of the entry to be fetched.
     * @return Uid of entry to be fetched.
     */
    IMPORT_C TUint GetUid() const;

    /**
     * Sets the uid filter.
     * @param aUid Uid filter. Items will be fetched with only the specific uid.
     */
    IMPORT_C void SetUid( TUint aUid );

    /**
     * Specifies the order type in which queried entiries are returned. 
     * @return The sort type of the query.
     */
    IMPORT_C CCaInnerQuery::TSortAttribute GetSort() const;

    /**
     * Sets the sort order to in which items will be returned.
     * @param aSort The sort order to in which items will be returned.
     */
    IMPORT_C void SetSort( CCaInnerQuery::TSortAttribute aSort );

    /**
     * Externalizes to stream.
     * @param aStream Stream to which CCaInnerQuery is externalized.
     */
    IMPORT_C void ExternalizeL( RWriteStream& aStream ) const;

    /**
     * Internalizes from stream.
     * @param aStream Stream from which CCaInnerQuery is internalized.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );

    /**
     * Sets the maximum amount of entries to be fetched.
     * @param aCount The number of entries to be fetched.
     */
    IMPORT_C void SetCount( TUint aCount );

    /**
     * Gets the maximum amount of entries to be fetched.
     * @return The maximum amount of entries to be fetched.
     */
    IMPORT_C TUint GetCount() const;
    
    /**
      * Adds attribute to query.
      * @param aKey Attribute key.
      * @param aValue Attribute value.
      */
     IMPORT_C void AddAttributeL( const TDesC& aKey, const TDesC& aValue );
     
     /**
       * Get attributes array
       * @return attributes array
       */
     IMPORT_C const RCaEntryAttrArray& GetAttributes() const;


private:

    /**
     * Second phased constructor.
     */
    void ConstructL();

    /**
     * Constructor
     */
    CCaInnerQuery();

private:
    // data

    /*
     * Unique identifier of entries. Own
     */
    RArray<TInt> iIds;

    /*
     * Unique identifier of the parent.
     */
    TInt iParentId;

    /*
     * Unique identifier of the child entry.
     */
    TInt iChildId;

    /*
     * Own.
     */
    CDesC16ArrayFlat* iEntryTypeNames;

    /*
     * Entry types.
     */
    TInt iRole;

    /*
     * Flags on.
     */
    TUint iFlagsOn;

    /*
     * Flags off.
     */
    TUint iFlagsOff;

    /*
     * Uid.
     */
    TUint iUid;

    /*
     * Sorting option.
     */
    TUint iSort;

    /*
     * Number of entries to fetch
     */
    TUint iCount;
    
    /*
     * Attributes array. Own
     */
    RCaEntryAttrArray iAttributes;

    };

#endif // __CCAQUERY_H__
