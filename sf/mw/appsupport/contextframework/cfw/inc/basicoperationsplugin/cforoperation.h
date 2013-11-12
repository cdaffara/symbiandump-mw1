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
* Description:  Class that defines logical OR operation in an XML tree.
*
*/



#ifndef C_CFOROPERATION_H
#define C_CFOROPERATION_H

// INCLUDES
#include <e32base.h>
#include "cfoperationnode.h"

// CLASS DECLARATION

/**
*  This class implements boolean OR operation XML node of Context Framework
*  Scripts.
*
*  @lib CFScriptEngine
*  @since Series 60 2.6
*/
NONSHARABLE_CLASS( CCFOrOperation ): public CCFOperationNode
    {
public: // Constructors and destructor

    /**
    * Creates a CCFOrOperation object
    * @param aServices is the operation services interface for nodes.
    * @param aParent parent of this node or NULL if this is a root. The
    *   ownership of the parent is not transferred.
    * @return a pointer to the created instance of CCFOrOperation
    */
    static CCFOrOperation* NewL( MCFOperationServices& aServices,
            CCFOperationNode* aParent );

    /**
    * Creates a CCFOrOperation object
    * @param aServices is the operation services interface for nodes.
    * @param aParent parent of this node or NULL if this is a root. The
    *   ownership of the parent is not transferred.
    * @return a pointer to the created instance of CCFOrOperation
    */
    static CCFOrOperation* NewLC( MCFOperationServices& aServices,
            CCFOperationNode* aParent );

    /**
    * Creates and parses a CCFOrOperation instance from an xml node.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is the parent of this node or NULL if this is a root.
    *   The ownership of the parent is not transferred.
    * @param aNode is the node to be parsed.
    * @return a pointer to the created instance of CCFOrOperation, or NULL
    *   if the node is not this or node. May leave with any system wide
    *   error code.
    */
    static CCFOrOperation* ParseL( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            CMDXMLNode& aNode );

    /**
    * Destructor.
    */
    ~CCFOrOperation();

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
    * Evaluates the OR operation.
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
    CCFOrOperation( MCFOperationServices& aServices,
            CCFOperationNode* aParent );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

private: // Data

    // Node children.
    RPointerArray< CCFOperationNode > iChildren;
    };


#endif // C_CFOROPERATION_H
