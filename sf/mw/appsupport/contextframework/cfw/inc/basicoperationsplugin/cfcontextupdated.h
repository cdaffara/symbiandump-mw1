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
* Description:  CCFContextUpdated class declaration.
*
*/



#ifndef C_CFCONTEXTUPDATED_H
#define C_CFCONTEXTUPDATED_H

// INCLUDES
#include "cfcontextoperation.h"

#include <e32base.h>

// FORWARD DECLARATIONS
class CCFContextObject;

// CLASS DECLARATION

/**
*  This class implements ContextUpdated context object event.
*
*  @lib CFScriptEngine
*  @since Series 60 2.6
*/
NONSHARABLE_CLASS( CCFContextUpdated ): public CCFContextOperation
    {
public: // Constructors and destructor

    /**
    * Two-phased constructor.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is parent node for this node.
    * @param aName is name of the context object this operation operates on.
    * @param aSource is the source of the context object this operation
    *   operates on.
    * @param aValue is the expected value of the context object this operation
    *   operates on. Use KNullDesC if no particular value is expected.
    */
    static CCFContextUpdated* NewL( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            const TDesC& aName,
            const TDesC& aSource,
            const TDesC& aValue );

    /**
    * Two-phased constructor that leaves the class into CleanupStack.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is parent node for this node.
    * @param aName is name of the context object this operation operates on.
    * @param aSource is the source of the context object this operation
    *   operates on.
    * @param aValue is the expected value of the context object this operation
    *   operates on. Use KNullDesC if no particular value is expected.
    */
    static CCFContextUpdated* NewLC( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            const TDesC& aName,
            const TDesC& aSource,
            const TDesC& aValue );

    /**
    * Creates and parses a CCFContextUpdated instance from an xml node.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is the parent of this node or NULL if this is a root.
    *   The ownership of the parent is not transferred.
    * @param aNode is the node to be parsed.
    * @return a pointer to the created instance of CCFContextUpdated, or NULL if
    *   the node is not this context updated node. May leave with any system
    *   wide error code.
    */
    static CCFContextUpdated* ParseL( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            CMDXMLNode& aNode );

    /**
    * Destructor.
    */
    ~CCFContextUpdated();

private:

    /**
    * C++ default constructor.
    */
    CCFContextUpdated( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            HBufC* aName,
            HBufC* aSource );

    /**
    * By default Symbian 2nd phase constructor is private.
    * @param aValue is the expected value of the context object this operation
    *   operates on. Use KNullDesC if no particular value is expected.
    */
    void ConstructL( const TDesC& aValue );

protected: // From CFContextOperation

    /**
    * Tests if this listener requires notification of all contexts.
    * @returns ETrue if this listener is interested in all contexts,
    *   EFalse otherwise.
    */
    TBool IsAllRequired() const;

    /**
    * Evaluates a node with a context.
    * @param aContext is the context for evaluation.
    * @param aContextLevelDelay contains the context level delay defined by
    *  contextRef after the call.
    * @return ETrue if the operation was evaluated, EFalse otherwise.
    */
    virtual TBool Evaluate( const CCFContextObject& aContext,
            TInt& aContextLevelDelay );

    /**
    * Method that returns whether or not this this operation is true.
    * @returns ETrue if this operation is true, EFalse otherwise
    */
    TBool IsTrueL( const CCFContextObject& aContextObject );

private: // Data

    // Flag for context used with previous truth check (call to isTrueL),
    // tells whether it was the referenced context or not.
    TBool iPreviousTruthCheckWithReferencedContext;

    // When set, the required context value for making the operation true.
    HBufC* iCmpValue;
    };

#endif // C_CFCONTEXTUPDATED_H
