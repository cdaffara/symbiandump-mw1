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
* Description:  CCFScriptService class declaration.h
*
*/


#ifndef CFSCRIPTSERVICE_H
#define CFSCRIPTSERVICE_H

// SYSTEM INCLUDE FILES

// USER INCLUDE FILES
#include "cfservicebase.h"

// FORWARD DECLARATIONS
class RCFClientSession;
class CCFScriptMessageListener;

// DATA TYPES

// CLASS DECLARATION

/**
 * Service class for script related features.
 *
 * @lib cfclient.lib
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CCFScriptService ): public CCFServiceBase
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param None.
     * @return CCFScriptService
     */
    static CCFScriptService* NewL( RCFClientSession& aSession,
        MCFListener& aListener );
    static CCFScriptService* NewLC( RCFClientSession& aSession,
        MCFListener& aListener );

    /**
     * C++ destructor.
     */
    virtual ~CCFScriptService( );

public:

    // @see CCFClient    
    TInt RegisterScript( const TDesC& aScriptFileName );
    
    // @see CCFClient
    TInt DeregisterScript( TInt aScriptId );
    
    // @see CCFClient
    TInt RegisterScript( const TDesC8& aScript,
        const TDesC& aScriptFileName );

    // @see CCFClient
    TInt SaveScript( const TDesC& aScriptFileName );

    // @see CCFClient
    TInt SaveScript( const TDesC8& aScript,
        const TDesC& aScriptFileName );

    // @see CCFClient
    TInt DeleteScript( const TDesC& aScriptFileName );

    // @see CCFClient
    TInt DeleteScripts();

    // @see CCFClient
    TInt UpgradeRomScript( const TDesC& aScriptFileName );

    // @see CCFClient
    TInt UpgradeRomScriptDes( const TDesC& aScriptFileName,
        const TDesC8& aScript );

    // @see CCFClient
    TInt RestoreRomScript( const TDesC& aScriptFileName );
    
private:

    // Share a file handle with server and send request
    TInt ShareFileAndSendSyncL( const TDesC& aScriptFileName, TInt aOpCode );

private:

    CCFScriptService( RCFClientSession& aSession, MCFListener& aListener );
    void ConstructL( );

private: // Data

    /** Message listener for removed scripts */
    CCFScriptMessageListener* iMessageListener;
    };

#endif // CFSCRIPTSERVICE_H

