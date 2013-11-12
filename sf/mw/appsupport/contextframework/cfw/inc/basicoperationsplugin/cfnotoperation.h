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
* Description:  CCFNotOperation class declaration.
*
*/



#ifndef C_CFNOTOPERATION_H
#define C_CFNOTOPERATION_H

// INCLUDES
#include <e32base.h>

#include "cfoperationnode.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  This class implements NotOperation XML node of CFW script on Context
* Framework Scripts.
*
*  @lib CFScriptEngine
*  @since S60 v5.0
*/
NONSHARABLE_CLASS( CCFNotOperation ) : public CCFOperationNode
    {
public: // Constructors and destructor

    /**
    * Creates a CCFNotOperation object.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is parent node for this node. The ownership of the parent
    *   is not transffered.
    * @return A pointer to the created instance of CCFNotOperation.
    */
    static CCFNotOperation* NewL( MCFOperationServices& aServices,
            CCFOperationNode* aParent );

    /**
    * Creates a CCFNotOperation object.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is parent node for this node. The ownership of the parent
    *   is not transffered.
    * @return A pointer to the created instance of CCFNotOperation.
    */
    static CCFNotOperation* NewLC( MCFOperationServices& aServices,
            CCFOperationNode* aParent );

    /**
    * Creates and parses a CCFNotOperation instance from an xml node.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is the parent of this node or NULL if this is a root.
    *   The ownership of the parent is not transferred.
    * @param aNode is the node to be parsed.
    * @return a pointer to the created instance of CCFNotOperation, or NULL if
    *   the node is not this or node. May leave with any system wide error code.
    */
    static CCFNotOperation* ParseL( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            CMDXMLNode& aNode );

    /**
    * Destructor.
    */
    ~CCFNotOperation();

protected: // Functions from CCFOperationNode

    /**
    * Activates this node by passing the activate call to its' child.
    */
    void ActivateL();

    /**
    * Deactivates this node by passing the deactivate call to its' child.
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
    * Called by child node, to ask the parent to re-evaluate its value,
    * since the child's value has changed.
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
    * @param aServices is the operation services interface for nodes.
    * @param aParent is parent node for this node. The ownership of the parent
    *   is not transffered.
    */
    CCFNotOperation( MCFOperationServices& aServices,
            CCFOperationNode* aParent );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

private: // Data

    // May have one operation as a child.
    CCFOperationNode* iChild;
    };


#endif // C_CFNOTOPERATION_H
