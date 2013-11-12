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

#ifndef __CAINNERICONDESCRIPTION_H__
#define __CAINNERICONDESCRIPTION_H__

#include <e32base.h>

//FORWARD DECLARATIONS
class RWriteStream;
class RReadStream;

/**
 *  CA entry icon.
 *  @lib .lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CCaInnerIconDescription ): public CBase
    {

public:

    /**
     * Destructor.
     */
    virtual ~CCaInnerIconDescription();

    /**
     * Two-phased constructor.     
     * @return The created object.
     */
    IMPORT_C static CCaInnerIconDescription* NewL();

    /**
     * Two-phased constructor.
     * @return The created object.
     */
    IMPORT_C static CCaInnerIconDescription* NewLC();

    /**
     * Gets icon id.
     * @return iId.
     */
    TInt Id() const
        {
        return iId;
        }

    /**
     * Gets file name.
     * @return iFileName.
     */
    TPtrC FileName() const
        {
        return iFileName;
        }

    /**
     * Gets skin id.
     * @return iSkinId
     */
    TPtrC SkinId() const
        {
        return iSkinId;
        }

    /**
     * Gets application id.
     * @return iApplicationId
     */
    TPtrC ApplicationId() const
        {
        return iApplicationId;
        }

    /**
     * Set icon id.
     * @param aId Value     
     */
    IMPORT_C void SetId( const TInt aId );
    
    /**
     * Set icon file name.
     * @param aFileName Value
     * Ownership taken.
     */
    IMPORT_C void SetFileNameL( const TDesC& aFileName );

    /**
     * Set icon skin id.
     * @param aSkinId Value
     * Ownership taken.
     */
    IMPORT_C void SetSkinIdL( const TDesC& aSkinId );

    /**
     * Set application id.
     * @param aApplicationId Value
     * Ownership taken.
     */
    IMPORT_C void SetApplicationIdL( const TDesC& aApplicationId );
    
    /**
     * Externalizes icon to the stream
     * @param aStream a write stream
     */
    void ExternalizeL( RWriteStream& aStream ) const;

    /**
     * Internalizes stream to the icon
     * @param aStream a read stream
     */
    void InternalizeL( RReadStream& aStream );    
    
private:

    /**
     * Second phased constructor.
     */
    void ConstructL();
    
    /**
     * Constructor
     */
    CCaInnerIconDescription();
    
private:
    // data

    /*
     * icon id
     */
    TInt iId;

    /*
     * Icon filename
     */
    RBuf iFileName;
    
    /*
     * Icon skin id
     */
    RBuf iSkinId;
    
    /*
     * Icon application id
     */
    RBuf iApplicationId;    
    };

#endif // __CAINNERICONDESCRIPTION_H__
