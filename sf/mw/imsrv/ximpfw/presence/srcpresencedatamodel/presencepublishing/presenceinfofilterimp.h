/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MPresenceInfoFilter API object implementation.
*
*/

#ifndef CPRESENCEINFOFILTERIMP_H
#define CPRESENCEINFOFILTERIMP_H

#include "ximpapidataobjbase.h"
#include "presenceapiobjbase.h"
#include <presenceinfofilter.h>
#include <s32strm.h>

class CDesC8ArraySeg;

/**
 * MPresenceInfoFilter API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresenceInfoFilterImp ): public CXIMPApiDataObjBase,
                                                 public MPresenceInfoFilter
    {
    private:

    /**
     * Container class to do 1:many infoname -> fieldname mappings.
     */
    class CFilterField : public CBase
        {
        public:

            /**
             * @param aInfoName The name of info associated
             * with this filter
             */
            static CFilterField* NewL( 
                    const TDesC8& aFieldName );

            static CFilterField* NewLC();

            ~CFilterField();

            /**
             * @return The info name
             */
            const TDesC8& InfoName();

            /**
             * Add a field name
             * @param aFieldName The name of field
             */
            void AddFieldNameL( const TDesC8& aFieldName );

            static TInt OrderOfFields(
                    const CFilterField& aFirst, 
                    const CFilterField& aSecond );

            static TInt OrderOfFieldsByName(
                    const TDesC8* aName, 
                    const CFilterField& aSecond );

            void ExternalizeL( RWriteStream& aStream ) const; 
            void InternalizeL( RReadStream& aStream );

            /**
             * Merge the contents of given filter field into this filter field.
             * The field name of the fields must match! Otherwise this routine
             * is meaningless.
             */
            void MergeL( CFilterField& aOtherField );
            
            /**
             * Check if source is same as this
             * @param aSource Source for check
             * @return ETrue if objects are same             
             */
            TBool operator==( const CFilterField& aSource );
            TBool operator!=( const CFilterField& aSource );

        private:

            CFilterField();
            void ConstructL( const TDesC8& aInfoName );
            void ConstructL();

        public:
            // owns, accepted info name
            RBuf8 iInfoName;

            // owns: accepted field names
            CDesC8ArraySeg* iFieldNames;
        };


    public:

    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENCEINFOFILTERIMP };


public:

    IMPORT_C static CPresenceInfoFilterImp* NewLC();
    IMPORT_C static CPresenceInfoFilterImp* NewL();
    virtual ~CPresenceInfoFilterImp();

private:

    CPresenceInfoFilterImp();
    void ConstructL();

public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS

public: // From CXIMPApiDataObjBase

    /**
     * @see CXIMPApiDataObjBase
     */
    XIMPIMP_DECLARE_DATAOBJ_BASE_METHODS


public: // From MPresenceInfoFilter

    /**
     * @see MPresenceInfoFilter
     */
    void AcceptPersonFilterL( const TDesC8& aFieldName );
    
    void AcceptServiceFilterL( 
            const TDesC8& aServiceName,
            const TDesC8& aFieldName );
    
    void AcceptDeviceFilterL( 
            const TDesC8& aDeviceName,
            const TDesC8& aFieldName );
    
    TBool MatchPersonFilter( const TDesC8& aFieldName ) const;
    
    TBool MatchServiceFilterL( 
            const TDesC8& aServiceName,
            const TDesC8& aFieldName ) const;
    
    TBool MatchDeviceFilterL( 
            const TDesC8& aDeviceName,
            const TDesC8& aFieldName ) const;

public: // Internal versions of api

public: // New functions
    /**
     * Internalizes object data from given stream.
     * @param aStream Stream to read.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );

    /**
     * Compares this filter to the given filter in a set-theoretic
     * sense, whether aSource is a proper subset of this class.
     * @param aCompare The filter to compare with 
     * @return ETrue if the given filter is a proper subset of this filter
     */
    IMPORT_C TBool Contains( CPresenceInfoFilterImp& aCompare );

    /**
     * Compares this filter to the given filter
     * @param aSource The filter to compare with 
     * @return ETrue if the given filter is same
     */
    IMPORT_C TBool operator==( CPresenceInfoFilterImp& aSource );

    /**
     * Compares this filter to the given filter
     * @param aSource The filter to compare with 
     * @return ETrue if the given filter is not same
     */
    IMPORT_C TBool operator!=( CPresenceInfoFilterImp& aSource );

    /**
     * Create a new filter which is the union of "first" filter and the 
     * "second" filter.
     * @param aFirst The first filter to use
     * @param aSecond The second filter to use
     * @return A new filter which is the union, ownership transferred
     */
    IMPORT_C static CPresenceInfoFilterImp* UnionL( 
            CPresenceInfoFilterImp& aFirst,
            CPresenceInfoFilterImp& aSecond );

private: // Helper functions

    /**
     * Check if the collection of one-to-many filter infoname to fieldnames
     * mappings accepts the given filter parameters.
     * @param aFieldSet The field set to use
     * @param aInfoName The info name string with wildcards (*, ?)
     * @param aFieldName The field name string with wildcards (*, ?)
     * @return ETrue if there was a match
     */
    TBool MatchFilterL(
            const RXIMPObjOwningPtrArray< CFilterField >& aFieldSet, 
            const TDesC8& aInfoName,
            const TDesC8& aFieldName ) const;

    /**
     * Externalize given array.
     */
    void ExternalizeArrayL( 
        const CDesC8ArraySeg* aArray, RWriteStream& aStream ) const;

    /**
     * Internalize given array.
     */
    void InternalizeArrayL( 
        CDesC8ArraySeg* aArray, RReadStream& aStream );

    /**
     * Externalize a field set.
     */
    void ExternalizeFieldSetL(
            const RXIMPObjOwningPtrArray<CFilterField>& aFieldSet,
            RWriteStream& aStream ) const;
    
    /**
     * Internalize a field set.
     */
    void InternalizeFieldSetL(
            RXIMPObjOwningPtrArray<CFilterField>& aFieldSet,
            RReadStream& aStream );

    /**
     * Helper for adding a filter field to the filter field
     * set.
     * @param aFieldSet the set into which to add
     * @param aServiceType The service type
     * @param aFieldType The field type
     */
    void AcceptFilterL( 
            RXIMPObjOwningPtrArray< CFilterField >& aFieldSet,
            const TDesC8& aServiceType,
            const TDesC8& aFieldType );

    /**
     * Create an union of the given field sets.
     * @aSource Source for union
     * @aTarget Target for union
     */
    void UnionOfFieldsL( 
            RXIMPObjOwningPtrArray< CFilterField>& aSource, 
            RXIMPObjOwningPtrArray< CFilterField>& aTarget );

    /** 
     * @return ETrue iff aCompare is a proper subset of aA, i.e. it's contained fully,
     * based on person filter fields
     */
    TBool ContainsPersonFilter( 
            CDesC8ArraySeg* aA,
            CDesC8ArraySeg* aCompare );

    /**
     * @return ETrue iff aCompare is a proper subset of aA, i.e. it's contained fully,
     * based on filter field array comparison
     */
    TBool ContainsFilterFields(
            RXIMPObjOwningPtrArray<CFilterField>& aA,
            RXIMPObjOwningPtrArray<CFilterField>& aCompare 
            );

private: // data

    // owned
    CDesC8ArraySeg* iPersonFilter;

    // owned, set of fields
    RXIMPObjOwningPtrArray<CFilterField> iServiceFilter;
    
    // owned, set of fields
    RXIMPObjOwningPtrArray<CFilterField> iDeviceFilter;
    };


#endif // CPRESENCEINFOFILTERIMP_H
