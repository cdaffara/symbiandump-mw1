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

#ifndef CASRVMMCHISTORY_H
#define CASRVMMCHISTORY_H

#include <e32base.h>

// FORWARD DECLARATION

class RFs;

/**
 * MMC History.
 * Stores a fixed number of MMC ids. When new ID-s are inserted,
 * old ones are purged. Supports file persistence.
 */
NONSHARABLE_CLASS( CCaSrvMmcHistory ): public CBase
    {

public:
    // construction

    /**
     * Creates an instance of CCaSrvMmcHistory.
     * @return a pointer to CCaSrvMmcHistory insctance.
     */
    IMPORT_C static CCaSrvMmcHistory* NewL();

    /**
     * Destructor.
     */
    virtual ~CCaSrvMmcHistory();

public:
    // new methods

    /**
     * Load from file. Safe to call if file does not exist.
     * @param aFs File Server Session.
     * @param aFname File name.
     */
    IMPORT_C void LoadL( RFs& aFs, const TDesC& aFname );

    /**
     * Save to file. Existing file overwritten.
     * @param aFs File Server Session.
     * @param aFname File name.
     */
    IMPORT_C void SaveL( RFs& aFs, const TDesC& aFname );

    /**
     * Insert new ID to first position.
     * If ID is already in the history, now it is moved to first
     * position (->latest).
     * If history is full, oldest ID may be dropped from history.
     * @param aMmc MMC ID.
     */
    IMPORT_C void InsertL( TUint aMmc );

    /**
     * Find ID in history.
     * @param aMmc MMC ID.
     * @return History index, or KErrNotFound.
     */
    IMPORT_C TInt Find( TUint aMmc ); // Find mmc in history.

private:
    /**
     * Constructor.
     */
    CCaSrvMmcHistory();

    /**
     * Second phase symbian constructor.
     */
    void ConstructL();

private:
    // data

    RArray<TUint> iMmcList; ///< ID history list. Own.

    };

#endif // CASRVMMCHISTORY_H
