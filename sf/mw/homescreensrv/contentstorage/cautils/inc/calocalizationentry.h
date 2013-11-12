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

#ifndef __CAINNERLOCALIZATION_H__
#define __CAINNERLOCALIZATION_H__

#include <e32base.h>

/**
 * Class represents Content Arsenal entry
 */
NONSHARABLE_CLASS( CCaLocalizationEntry ): public CBase
    {

public:

    /**
     * Destructor.
     */
    virtual ~CCaLocalizationEntry( );

    /**
     * Two-phased constructor.
     * @return The created object.
     */
    IMPORT_C static CCaLocalizationEntry* NewL( );

    /**
     * Two-phased constructor.
     * @return The created object.
     */
    IMPORT_C static CCaLocalizationEntry* NewLC( );

    //    GETTERS
    /**
     * Gets entry id.
     * @return Entry id.
     */
    IMPORT_C TInt GetRowId( ) const;

    /**
     * Gets entry id.
     * @return Entry id.
     */
    IMPORT_C TInt GetTextId( ) const;

    /**
     * Gets entry text.
     * @return Entry text.
     */
    IMPORT_C const RBuf& GetAttributeName( ) const;

    /**
     * Gets entry text.
     * @return Entry text.
     */
    IMPORT_C const RBuf& GetStringId( ) const;
    
    /**
     * Gets entry text.
     * @return Entry text.
     */
    IMPORT_C const RBuf& GetTableName( ) const;  
    
    /**
     * Gets LocalizedString
     * @return LocalizedString
     */
    IMPORT_C const RBuf& GetLocalizedString() const;
    
    /**
     * Gets GetQmFilename
     * @return GetQmFilename
     */
    IMPORT_C const RBuf& GetQmFilename() const;

    // SETTERS

    /**
    */
    IMPORT_C void SetQmFilenameL( const TDesC& aQmFilename );
    
    /**

     */
    IMPORT_C void SetRowId( TUint aRowId );
    
    /**
      Sets localization id
     */
    IMPORT_C void SetTextId( TUint aId );
    

    /**
     * Sets the entry text.
     * @param aText Entry text.
     */
    IMPORT_C void SetAttributeNameL( const TDesC& aAttribName );

    /**
     * Sets the entry text.
     * @param aText Entry text.
     */
    IMPORT_C void SetStringIdL( const TDesC& aStringId );
    
    /**
     * Sets the entry text.
     * @param aText Entry text.
     */
    IMPORT_C void SetTableNameL( const TDesC& aTableName );

    /**
     * Sets the Localized String 
     * @param aLocalName LocalizedString
     */
    IMPORT_C void SetLocalizedStringL( const TDesC& aLocalName );
    
    
    
/*    *
     * Externalizes object to the stream
     * @param aStream a write stream
     
    IMPORT_C void ExternalizeL( RWriteStream& aStream ) const;

    *
     * Internalizes stream to the object
     * @param aStream a read stream
     
    IMPORT_C void InternalizeL( RReadStream& aStream );*/

private:

    /**
     * Second phased constructor.
     */
    void ConstructL( );

    /**
     * Constructor
     */
    CCaLocalizationEntry( );

private:
    // data
	/*,                                                    
	                                      
	  				
	                                  
	
    /*
     * Unique identifier of the localization entry.
     */
    TInt iId;
    
    /*
     * Unique identifier of the entry to update.
     */
    TInt iRowId;

    /*
     * Name of table to update.
     * Own.
     */
    RBuf iTableName;
    
    /*
     * Name of attribute (column) to update.
     * Own.
     */
    RBuf iAttribName;
    
    /*
     * String it to translate.
     * Own.
     */
    RBuf iStringId;

    /*
     * Translated string.
     * Own.
     */
    RBuf iLocalName;
    
    /*
     * Qm filename.
     * Own.
     */
    RBuf iQmFilename;

     };

#endif // __CAINNERLOCALIZATION_H__
