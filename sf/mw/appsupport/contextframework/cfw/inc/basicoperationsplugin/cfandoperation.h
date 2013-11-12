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
* Description:  Class that defines logical AND operation in an XML tree.
*
*/



#ifndef C_CFANDOPERATION_H
#define C_CFANDOPERATION_H

// INCLUDES
#include <e32base.h>
#include "cfoperationnode.h"

// CLASS DECLARATION

/**
*  Concrete class for inner nodes. Represents a boolean AND operation.
*
*  @lib CFScriptEngine
*  @since Series 60 2.6
*/
NONSHARABLE_CLASS( CCFAndOperation ): public CCFOperationNode
    {
public: // Constructors and destructor

    /**
    * Two-phased constructor.
    * @param aServices is the operation services interface for nodes.
    * @param aParent parent of this node or NULL if this is a root.
    */
    static CCFAndOperation* NewL( MCFOperationServices& aServices,
            CCFOperationNode* aParent );

    /**
    * Two-phased constructor that leaves the class into CleanupStack.
    * @param aServices is the operation services interface for nodes.
    * @param aParent parent of this node or NULL if this is a root.
    */
    static CCFAndOperation* NewLC( MCFOperationServices& aServices,
            CCFOperationNode* aParent );

    /**
    * Creates and parses a CCFAndOperation instance from an xml node.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is the parent of this node or NULL if this is a root.
    *   The ownership of the parent is not transferred.
    * @param aNode is the node to be parsed.
    * @return a pointer to the created instance of CCFAndOperation, or NULL
    *   if the node is not this and node. May leave with any system wide
    *   error code.
    */
    static CCFAndOperation* ParseL( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            CMDXMLNode& aNode );

    /**
    * Destructor.
    */
    ~CCFAndOperation();

protected: // Functions from CCFOperationNode

    /**
    * Activates this node by passing the activate call to its' children.
    */
    void ActivateL();

    /**
    * Deactivates this node by passing the deactivate call to its' children.
    */
    void Deactivate();

    /**
    * Asks this node to check its security (via services interface). For
    * example, the security of contexts to be subscribed.
    * @return KErrNone if security check passed, otherwise any of the system
    *   wide error codes.
    */
    TInt CheckSecurity();

    /**
    * Evaluates the AND operation.
    */
    void Evaluate();

    /**
    * Cleanup long term resources like persistent data files.
    * This is called by the context framework when the script is uninstalled
    * from the system.
    */ 
    void Cleanup();

private:

    /**
    * C++ default constructor.
    */
    CCFAndOperation( MCFOperationServices& aServices,
            CCFOperationNode* aParent );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

private: // Data

    // Node children.
    RPointerArray< CCFOperationNode > iChildren;
    };


#endif // C_CFANDOPERATION_H
