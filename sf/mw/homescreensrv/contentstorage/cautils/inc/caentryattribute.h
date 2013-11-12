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
 * Description: Definition of entry attribute
 *
 */

#ifndef __CAENTRYATTRIBUTE_H__
#define __CAENTRYATTRIBUTE_H__

#include <e32base.h>

//FORWARD DECLARATIONS
class RWriteStream;
class RReadStream;

/**
 *  CA entry attribute.
 *  @lib .lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CCaEntryAttribute ): public CBase
    {

public:

    /**
     * Destructor.
     */
    virtual ~CCaEntryAttribute();

    /**
     * Two-phased constructor.
     * @param aName Attribute name.
     * @return The created object.
     */
    IMPORT_C static CCaEntryAttribute* NewL( const TDesC& aName );

    /**
     * Two-phased constructor.
     * @param aName Attribute name.
     * @return The created object.
     */
    IMPORT_C static CCaEntryAttribute* NewLC( const TDesC& aName );

    /**
     * Get name.
     * @return Name.
     */
    TPtrC Name() const
        {
        return iName;
        }

    /**
     * Get value.
     * @return Value, or NULL if attribute is not set.
     */
    TPtrC Value() const
        {
        return iValue;
        }

    /**
     * Set value. Sets changed status as appropriate.
     * @param aValue Value, or NULL if attribute is not set (==remove attr).
     * Ownership taken.
     */
    IMPORT_C void SetValueL( const TDesC& aName );

    /**
     * Externalizes array to the stream
     * @param aStream a write stream
     */
    void ExternalizeL( RWriteStream& aStream ) const;

    /**
     * Internalizes stream to the array
     * @param aStream a read stream
     */
    void InternalizeL( RReadStream& aStream );

protected:

    /**
     * Second phased constructor.
     * @param aName Attribute name.
     */
    void ConstructL( const TDesC& aName );

private:
    // data

    /*
     * Attribute name. Own
     */
    RBuf iName;

    /*
     * Attribute value. Own
     */
    RBuf iValue;

    };

/**
 * Attribute array.
 *  @lib camenu.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( RCaEntryAttrArray ): public RPointerArray<CCaEntryAttribute>
    {

public:
    /**
     * Find attribute by name.
     * @param aId Id.
     * @param aName Name.
     * @param aAttr Attribute value.
     * @return TBool ,ETrue if attribute was found
     */
    IMPORT_C TBool Find( const TDesC& aName, TDes& aAttrVal ) const;

    /**
     * Find attribute by name.
     * @param aId Id.
     * @param aName Name.
     * @param aAttr Attribute value.
     * @return TBool, ETrue if attribute was found
     */
    IMPORT_C TBool Find( const TDesC& aName, TPtrC& aAttrVal ) const;

    /*
     * Check if attribute exists
     * @param aId Id
     * @param aName Name
     * @return ETrue if attribute exists
     */
    IMPORT_C TBool Exist( const TDesC& aName ) const;

    /**
     * Remove attribute
     * @param aId Id.
     * @param aAttrName Attribute name
     */
    IMPORT_C void RemoveAttribute( const TDesC& aAttrName );

    /**
     * Externalizes array to the stream
     * @param aStream a write stream
     */
    void ExternalizeL( RWriteStream& aStream ) const;

    /**
     * Internalizes stream to the array
     * @param aStream a read stream
     */
    void InternalizeL( RReadStream& aStream );

    };

#endif // __CAENTRYATTRIBUTE_H__
