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
* Description:  CCFIfClause class declaration.
*
*/



#ifndef C_CFIFCLAUSE_H
#define C_CFIFCLAUSE_H

// INCLUDES
#include <e32base.h>

#include "cfclausenode.h"

// CLASS DECLARATION

/**
*   This class implements IfClause XML node of CFW script in Script Engine
*   scripts.
*
*  @lib CFScriptEngine
*  @since Series 60 2.6
*/
NONSHARABLE_CLASS( CCFIfClause ): public CCFClauseNode
    {
public: // Constructors and destructor

    /**
    * Creates a CCFIfClause object
    * @param aServices is the operation services interface for nodes.
    * @param aParent parent of this node or NULL if this is a root. The
    *   ownership of the parent is not transferred.
    * @return a pointer to the created instance of CCFIfClause
    */
    static CCFIfClause* NewL( MCFOperationServices& aServices,
            CCFOperationNode* aParent );

    /**
    * Creates a CCFIfClause object
    * @param aServices is the operation services interface for nodes.
    * @param aParent parent of this node or NULL if this is a root. The
    *   ownership of the parent is not transferred.
    * @return a pointer to the created instance of CCFIfClause
    */
    static CCFIfClause* NewLC( MCFOperationServices& aServices,
            CCFOperationNode* aParent );

    /**
    * Creates and parses a CCFIfClause instance from an xml node.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is the parent of this node or NULL if this is a root. The
    *   ownership of the parent is not transferred.
    * @param aNode is the node to be parsed.
    * @return a pointer to the created instance of CCFIfClause, or NULL if the
    *   node is not if clause node. May leave with any system wide error code.
    */
    static CCFIfClause* ParseL( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            CMDXMLNode& aNode );

    /**
    * Destructor.
    */
    ~CCFIfClause();

protected: // From CCFClauseNode

    /**
    * Fires actions of the node.
    * @return None.
    */
    void FireActionsL();

protected: // From CCFOperationNode

    /**
    * Activates this node by passing the activate call to its' child.
    * @return None.
    */
    void ActivateL();

    /**
    * Deactivates this node by passing the deactivate call to its' child.
    * @return None.
    */
    void Deactivate();

    /**
    * Asks this node to check its security (via services interface). Checks
    * the security of contexts to be subscribed and actions to be performed.
    * @return KErrNone if security check passed, otherwise any of the system
    *   wide error codes.
    */
    TInt CheckSecurity();

    /**
    * Called by child node, to ask the parent to re-evaluate its value, since
    * the child's value has changed.
    * @return None.
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
    CCFIfClause( MCFOperationServices& aServices, CCFOperationNode* aParent );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

private: // Data

    // Must have one "root" operation, a child.
    CCFOperationNode* iChild;

    // If clause actions.
    RPointerArray< CCFScriptAction > iActions;
    };

#endif // C_CFIFCLAUSE_H
