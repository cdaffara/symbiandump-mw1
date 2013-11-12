/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFAsyncCleanup class declaration.
 *
*/


#ifndef CFASYNCCLEANUP_H
#define CFASYNCCLEANUP_H

// SYSTEM INCLUDE FILES
#include <e32base.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS

// DATA TYPES

// CLASS DECLARATION
class CCFActionPlugInThread;
class MCFAsyncCleanupCallback;

/**
 * Performs asynchronous action plug-in cleanup.
 *
 * @lib None.
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CCFAsyncCleanup ): public CActive
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param None.
     * @return CCFAsyncCleanup
     */
    static CCFAsyncCleanup* NewL( TBool aUpgrade,
        CCFActionPlugInThread& aThread,
        MCFAsyncCleanupCallback& aCallback );
    static CCFAsyncCleanup* NewLC( TBool aUpgrade,
        CCFActionPlugInThread& aThread,
        MCFAsyncCleanupCallback& aCallback );
    
    /**
     * C++ destructor.
     */
    virtual ~CCFAsyncCleanup ( );
    
public:

    /**
     * Cleanup type.
     * @param None.
     * @return ETrue if clean up was an upgrade based.
     */
    TBool Upgrade() const;
    
    /**
     * Action thread associated with the cleanup.
     * @param None.
     * @return CCFActionPlugInThread&.
     */
    CCFActionPlugInThread& Thread() const;

public:

    // @see CActive
    void RunL();
    
    // @see CActive
    void DoCancel();
    
    // @see CActive
    TInt RunError( TInt aError );

private:

    CCFAsyncCleanup( TBool aUpgrade,
        CCFActionPlugInThread& aThread,
        MCFAsyncCleanupCallback& aCallback );
    void ConstructL( );

private: // Data

    /** Cleanup type */
    TBool iUpgrade;

    /** Action plug-in thread */
    CCFActionPlugInThread& iThread;
    
    /** Async cleanup callback */
    MCFAsyncCleanupCallback& iCallback;
    };

#endif // CFASYNCCLEANUP_H
