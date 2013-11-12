/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: CCaLocalizerScannerProxy.h
 *
 */

#ifndef CALOCALSCANNERPROXY_H
#define CALOCALSCANNERPROXY_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include "castorage_global.h"

// FORWARD DECLARATIONS
class CCaLocalizationEntry;
class CCaStorageProxy;
class CCaInnerEntry;

/**
 *  CCaLocalizerScannerProxy
 *
 */
class CCaLocalizerScannerProxy : public CBase
    {
public:
    /**
     * Two-phased constructor.
     */
    static CCaLocalizerScannerProxy* NewL( CCaStorageProxy* storageProxy );
    
    /**
     * Two-phased constructor.
     */
    static CCaLocalizerScannerProxy* NewLC( CCaStorageProxy* aStorageProxy );
    
    /**
     * Destructor.
     */
    ~CCaLocalizerScannerProxy( );
    
    /**
     * Get localization rows and updates database 
     */
    void UpdateLocalNamesL( );
   
private:

    /**
     * Constructor.
     */
    CCaLocalizerScannerProxy( CCaStorageProxy* aStorageProxy );
    
    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL( );

    /**
     * Gets localization rows from database as table of CCaLocalizationEntries
     * @param aLocals pointers to localization entries
     */
    void GetLocalizationRowsL( RPointerArray<CCaLocalizationEntry>& aLocals );
    
    /**
     * Gets localized names from translation files
     * @param aLocEntry localization entry
     * @return localized name
     */
    HBufC* GetLocalizedNameLC( const CCaLocalizationEntry* aLocEntry );
    
    /**
     * Gets entry text from the storage
     * @param aEntries array containing entries
     * @param aId entry id
     * @return entry text
     */
    const TDesC& GetEntryText(
            const RPointerArray<CCaInnerEntry>& aEntries, TInt aId );
    
    const TDesC& GetEntryDescription(
            const RPointerArray<CCaInnerEntry>& aEntries, TInt aId );

    const TPtrC GetAttributeName(
            const RPointerArray<CCaInnerEntry>& aEntries,
            TInt aId,
            const TDesC& aAttrName );
private:
	
    /*
     * Not own
     */
    CCaStorageProxy* iStorageProxy;

    /*
     * Recent translation file name
     */
    RBuf iRecentQmFile;
    };

#endif // CALOCALSCANNERPROXY_H
