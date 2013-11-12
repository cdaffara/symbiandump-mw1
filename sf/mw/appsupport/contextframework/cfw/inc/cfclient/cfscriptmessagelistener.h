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
* Description:  CCFScriptMessageListener class declaration.
*
*/


#ifndef CFSCRIPTMESSAGELISTENER_H
#define CFSCRIPTMESSAGELISTENER_H

// SYSTEM INCLUDE FILES
#include <e32base.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS
class RCFClientSession;
class MCFListener;

// DATA TYPES

// CLASS DECLARATION

/**
 * Script message listener in an active object which responsibility is to
 * receive messages from cfserver when there are scripts removed.
 *
 * @lib None.
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CCFScriptMessageListener ): public CActive
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param aSession Client side session to cfserver.
     * @param aListener CF client.
     * @return CCFScriptMessageListener*.
     */
    static CCFScriptMessageListener* NewL( RCFClientSession& aSession,
        MCFListener& aListener );
    static CCFScriptMessageListener* NewLC( RCFClientSession& aSession,
        MCFListener& aListener );

    /**
     * C++ destructor.
     */
    virtual ~CCFScriptMessageListener( );
    
private:

    // @see CActive
    void RunL();
    
    // @see CActive
    void DoCancel();

    // @see CActive
    TInt RunError( TInt aError );
    
public:

    /**
     * Starts listening notifications from cfserver related to script removal.
     */
    void RequestNotification();
    
private:

    // Notify client
    void NotifyClientL();

private:

    CCFScriptMessageListener( RCFClientSession& aSession,
        MCFListener& aListener );
    void ConstructL( );

private: // Data

    /** Session to cfserver */
    RCFClientSession& iSession;
    
    /** CF client callback */
    MCFListener& iListener;

    /** Message buffer */
    HBufC8* iBuffer;
    
    /** Message buffer pointer */
    TPtr8 iBufferPtr;
    
    /** Buffer size */
    TInt iBufferSize;
    
    /** Buffer size package */
    TPckg<TInt> iBufferSizePckg;
    
    /** Existence flag */
    TBool* iDied;
    };

#endif // CFSCRIPTMESSAGELISTENER_H

