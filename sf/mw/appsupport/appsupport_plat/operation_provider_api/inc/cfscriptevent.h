/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFScriptEvent class declaration.
*
*/



#ifndef C_CFSCRIPTEVENT_H
#define C_CFSCRIPTEVENT_H

// INCLUDES
#include <e32base.h>
#include <cfkeyvaluepair.h>

// CLASS DECLARATION

/**
*   Class to wrap all event data that is stored into event queue.
*
*  @lib OperationPlugInManager
*  @since Series 60 5.0
*/
NONSHARABLE_CLASS( CCFScriptEvent ): public CBase
    {
public:

    /**
    * Creates a new CCFScriptEvent object.
    * @param aScriptId is id of the script that created the event
    * @param aParameters contains the event parameters
    * @param aIdentifier defines the event
    */
    IMPORT_C static CCFScriptEvent* NewL( const TInt& aScriptId, 
            const RKeyValueArray& aParameters, 
            const TDesC& aIdentifier );

    /**
    * Creates a new CCFScriptEvent object. Leaves it into CleanupStack.
    * @param aScriptId is id of the script that created the event
    * @param aParameters contains the event parameters
    * @param aIdentifier defines the event
    */
    static CCFScriptEvent* NewLC( const TInt& aScriptId, 
            const RKeyValueArray& aParameters, 
            const TDesC& aIdentifier );

    /**
    * Returns id of the script that created this event.
    */
    IMPORT_C TInt ScriptId() const;

    /**
    * Returns a constant pointer to the event identifier.
    */
    IMPORT_C TPtrC Identifier() const;

    /**
    * Returns a constant reference to the event parameters.
    */
    IMPORT_C const RKeyValueArray& Parameters() const;

    /**
    * Destructor.
    */
    ~CCFScriptEvent();

protected:

    /**
    * C++ default constructor. 
    */
    CCFScriptEvent( const TInt& aScriptId );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL( const TDesC& aIdentifier,
            const RKeyValueArray& aParameters );

private: // Data

    // Associated script's Id
    TInt            iScriptId;

    // Uses: Parameters, does not own the pointers in the array
    RKeyValueArray  iParameters;

    // Own: Identifier
    HBufC*          iIdentifier;
    };

#endif // C_CFSCRIPTEVENT_H
