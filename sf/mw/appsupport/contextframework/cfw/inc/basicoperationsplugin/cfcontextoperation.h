/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFContextOperation class declaration.
*
*/



#ifndef C_CFCONTEXTOPERATION_H
#define C_CFCONTEXTOPERATION_H

// INCLUDES
#include <e32base.h>
#include <cfcontextobject.h>

#include "cfoperationnode.h"
#include "cfscriptlistener.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  CCFContextOperation class is the base class for all Context Framework script
*  operations that operate on contexts.
*  All instances of this class are leaf nodes in operation tree.
*
*  @lib CFScriptEngine
*  @since Series 60 2.6
*/
NONSHARABLE_CLASS( CCFContextOperation ): public CCFOperationNode,
    public MCFScriptListener
    {
public: // Constructors and destructor

    /**
    * Destructor.
    */
    virtual ~CCFContextOperation();

public: // Enumerations

    enum TCmpType
        {
        EInvalidCmpType = -1,
        EIntCmp,
        EFloatCmp,
        EPositionCmp,
        ETimeCmp,
        EStringCmp
        };


public: // From MCFScriptListener

    /**
    * Performs evaluation based on a context.
    * @param aContext is the context for evaluation.
    * @param aContextLevelDelay is storage for context level delay required by
    *   the evaluation.
    * @return ETrue if evaluation was performed and firing actions should be
    *   considered based on clause conditions, EFalse otherwise.
    */
    TBool Evaluate( const CCFContextObject& aContext,
            TInt& aContextLevelDelay );

    /**
    * Returns a context source this listener is interested in.
    * @returns the source of the context.
    */
    TPtrC Source() const;

    /**
    * Returns a context type this listener is interested in.
    * @returns the type of the context.
    */
    TPtrC Type() const;

    /**
    * Tests if this listener requires notification of all contexts.
    * @returns ETrue if this listener is interested in all contexts,
    *   EFalse otherwise.
    */
    TBool IsAllRequired() const;

protected:

    /**
    * C++ default constructor.
    */
    CCFContextOperation( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            HBufC* aType,
            HBufC* aSource );

protected: // New functions

    /**
    * All concrete classes must implement this method.
    * @param aContext is context that has changed.
    * @return the result of the comparison.
    */
    virtual TBool IsTrueL( const CCFContextObject& aContext ) = 0;

    /**
    * Updates the previous value the operation was evaluated with.
    * @param aNewValue is the new previous value.
    * @return KErrNone when update was successful, KErrNoMemory if update failed
    *   due to low memory.
    */
    TInt UpdatePreviousValue( const TDesC& aNewValue );

    /**
    * Gets the previous value the operation was evaluated with.
    * @return the previous value the operation was evaluated with.
    */
    const TDesC& PreviousValue();

protected: // From CCFOperationNode

    /**
    * Activates this node by subscribing to a context.
    * @return None.
    */
    void ActivateL();

    /**
    * Deactivates this node by removing context subscription.
    * @return None.
    */
    void Deactivate();

    /**
    * Asks this node to check its security (via services interface). For
    * example, the security of contexts to be subscribed.
    * @return KErrNone if security check passed, otherwise any of the system
    *   wide error codes.
    */
    TInt CheckSecurity();

private: // From CCFOperationNode

    /**
    * Called by child node, to ask the parent to re-evaluate its value,
    * since the child's value has changed.
    */
    void Evaluate();

protected: // Data

    /**
    * Context level delay for firing actions after evaluation
    * (defined by contextRef).
    */
    TInt iContextLevelDelay;

private: // Data

    /**
    * Context source and type as defined by contextRef for the operation.
    */
    HBufC* iSource;
    HBufC* iType;

    /** Previous context value. */
    HBufC* iPreviousValue;
    };

#endif // C_CFCONTEXTOPERATION_H
