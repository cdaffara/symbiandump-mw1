/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Base class for message handlers.
 *
*/


#ifndef C_CFMESSAGEHANDLERSCRIPT_H
#define C_CFMESSAGEHANDLERSCRIPT_H

#include <f32file.h>

#include "CFMessageHandlerBase.h"
#include "CFActionSubscriptionListener.h"
#include "cfscriptowner.h"

class CCFActionIndication;

/**
 * Action messages related handler class.
 */
NONSHARABLE_CLASS( CCFMessageHandlerScript ):
public CCFMessageHandlerBase,
public MCFScriptOwner
    {
public:

    // Two phased constructors
    static CCFMessageHandlerScript* NewL( MCFExtendedContextInterface& aCF,
        MCFActionInterface& aActivatorEngine,
        MCFScriptInterface& aScriptInterface );
    static CCFMessageHandlerScript* NewLC( MCFExtendedContextInterface& aCF,
        MCFActionInterface& aActivatorEngine,
        MCFScriptInterface& aScriptInterface );

    // Destructor
    ~CCFMessageHandlerScript();

protected: // From base classes

    // From CCFMessageHandlerBase
    TBool HandleMessageL( const RMessage2& aMessage );
    
    // From MCFScriptOwner
    void HandleScriptsRemoved( const RArray<TInt>& aScriptIds );

private:

    // C++ constructor
    CCFMessageHandlerScript( MCFExtendedContextInterface& aCFContext,
        MCFActionInterface& aCFAction,
        MCFScriptInterface& aScriptInterface );
    
    // 2nd phase constructor
    void ConstructL();

    // Loads a script from a file
    HBufC8* LoadScriptFromFile( RFile& aFile );
    
    // Registers a new script
    TInt RegisterScript( const RThread& aClient, const TDesC& aName,
        const TDesC8& aScript, TInt& aScriptId );
    
    // Updates an existing script from the client
    TInt UpgradeScript( const TDesC& aName, const TDesC8& aScript );
    
    // Check if there are new removed scripts which needs to be notified
    void NotifyRemovedScriptsL( const RMessage2& aMessage );
    
private: // Data

    /** Script message from client */
    RMessage2 iMessage;

    /** Array containing removed script ids */
    RArray<TInt> iScriptIds;
    };

#endif //C_CFMESSAGEHANDLERSCRIPT_H

