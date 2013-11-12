/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFPersistentData class declaration.
*
*/


#ifndef C_CFPERSISTENTDATA_H
#define C_CFPERSISTENTDATA_H

// INCLUDES
#include <e32base.h>
#include <e32property.h>
#include "cfoperationnode.h"

// FORWARD DECLARATIONS
class RFs;
class CCFPendingPersistencyTask;

// CLASS DECLARATION

/**
 *  This class implements persistent data for Context Framework Scripts.
 *
 *  @lib CFScriptEngine
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CCFPersistentData ) : public CActive
    {
public: // Constructors and destructor

    /**
     * Creates a CCFPersistentData object.
     * @return a pointer to the created instance of CCFPersistentData.
     */
    static CCFPersistentData* NewL( RFs& aFs,
        const TUid& aOwner,
        const TDesC& aName );

    /**
     * Creates a CCFPersistentData object.
     * @return a pointer to the created instance of CCFPersistentData.
     */
    static CCFPersistentData* NewLC( RFs& aFs,
        const TUid& aOwner,
        const TDesC& aName );

    /**
     * Destructor.
     */
    ~CCFPersistentData();

public: // From MCFPersistentDataInterface

    /**
     * Restores operation data from a persistent file. If the file exists
     * InternalizeL is called for the operation.
     * @param aFile is the persistent file containing operation data.
     * @param aOperation is the operation to be restored.
     * @return None.
     */
    void RestoreL( const TDesC& aFile,
        CCFOperationNode& aOperation );

    /**
     * Stores operation data into a persistent file. If the file exists
     * the old data is replaced by calling ExternalizeL for the operation.
     * @param aFile is the persistent file for operation data.
     * @param aOperation is the operation to be stored.
     * @return None.
     */
    void StoreL( const TDesC& aFile,
        CCFOperationNode& aOperation );

    /**
     * Delete persistent file.
     */
    void Delete( const TDesC& aFile );

private:

    // Rstores a persistent data from disk
    void DoRestoreL( const TDesC& aFile, CCFOperationNode& aOperation );
    
    // Stores a persistent data in to disk
    void DoStoreL( const TDesC& aFile, CCFOperationNode& aOperation );
    
    // Deletes a persistent file from disk
    void DoDelete( const TDesC& aFile );
    
    // Process the pending tasks queue
    void ProcessPendingTasksL();

private:

    /**
     * C++ default constructor.
     */
    CCFPersistentData( RFs& aFs, const TUid& aOwner, const TDesC& aName );

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

private: // From CActive
    // Handle completion
    void RunL();

    // How to cancel me
    void DoCancel();

    // Override to handle leaves from RunL(). Default implementation causes
    // the active scheduler to panic.
    TInt RunError( TInt aError );

private: // New functions

    /**
     * Creates stream name based on owner uid and file name.
     */
    HBufC* CreateStreamName( const TDesC& aFile );

    /**
     * Starts listenging back up events.
     */
    void StartListeningL();

private: // Data

    // Active file server session.
    RFs& iFs;

    // Owner of the script
    TUid iOwner;
    
    // Script name
    TPtrC iName;

    // Own: pending persistency tasks
    RPointerArray<CCFPendingPersistencyTask> iPendingTasks;

    // is persistency operations allowed
    TBool iStoreRestoreDenied;

    // Handler to backup property
    RProperty iProperty;
    };

#endif // C_CFPERSISTENTDATA_H

