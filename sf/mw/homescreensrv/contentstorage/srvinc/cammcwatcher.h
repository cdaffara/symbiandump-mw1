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
* Description:
*
*/

#ifndef C_CAMMCWATCHER_H
#define C_CAMMCWATCHER_H


#include <e32base.h>
#include <f32file.h>

/**
 *  Interface for observing MMC events.
 *
 *  @since S60 S60 v3.1
 */
class MMmcWatcherCallback
    {

public:
    /**
     * MmcChangeL is called when the MMC is removed or inserted.
     */
    virtual void MmcChangeL() = 0;

    };

/**
 *  This active object waits for an indication that the media has changed.
 *
 *  @since S60 S60 v3.1
 */
NONSHARABLE_CLASS( CCaMmcWatcher ) : public CActive
    {

public:

    /**
    * Two-phased constructor.
    * @param aObserver Pointer to notifier interface.
    * @param aFs file server session.
    */
IMPORT_C static CCaMmcWatcher* NewL( RFs& aFs,
            MMmcWatcherCallback* aObserver );

    /**
    * Two-phased constructor.
    * @param aObserver Pointer to notifier interface.
    * @param aFs file server session.
    */
IMPORT_C static CCaMmcWatcher* NewLC( RFs& aFs,
            MMmcWatcherCallback* aObserver );

    /**
    * Destructor
    */
    ~CCaMmcWatcher();

private:

    /**
    * C++ default constructor
    * @param aObserver Pointer to notifier interface.
    * @param aFs file server session.
    */
    CCaMmcWatcher( RFs& aFs, MMmcWatcherCallback* aObserver );

    /**
    * By default Symbian OS constructor is private.
    */
    void ConstructL();

    /**
    * Set notification request
    */
    void WaitForChangeL();

    /**
    * From CActive.
    */
    void DoCancel();

    /**
    * From CActive.
    */
    void RunL();

    /**
     * From CActive.
     */
    TInt RunError( TInt aError );

private:

    /**
     * File server session.
     * Not Own
     */
    RFs iFs;

    /**
     * MMC events observer
     * Not own.
     */
    MMmcWatcherCallback* iObserver;
    };


#endif      // C_CAMMCWATCHER_H
