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
* Description:  Abstract base class for all Script operations. Implements
*                defined inner node in condition tree.
*
*/

#ifndef C_CFOPERATIONNODE_H
#define C_CFOPERATIONNODE_H

// INCLUDES
#include <e32base.h>

#include "cfoperationservices.h"

// FORWARD DECLARATIONS
class RReadStream;
class RWriteStream;
class CMDXMLNode;

// CLASS DECLARATION

/**
*   CCFOperationNode is an abstract base class for all Context Framework Script
*   operations.
*   All operations have ternary logic boolean value:
*   ECFConditionUndefined, ECFConditionFalse, or ECFConditionTrue.
*
*  @lib CFScriptEngine
*  @since Series 60 5.0
*/
class CCFOperationNode : public CBase
    {
public: // Types

    /**
    * Condition values for script operations. False and true must match with
    * the boolean equivalents to enable safe type casting.
    */
    enum TCFConditionValue
        {
        ECFConditionUndefined = -1, 
        ECFConditionFalse = 0, 
        ECFConditionTrue = 1 
        };

public: // Constructors and destructor

    /**
    * Destructor.
    */
    IMPORT_C virtual ~CCFOperationNode();

public: // New functions

    /**
    * Activates this node, e.g. subscribes contexts or restores values
    * from persistent storage. Depending on the goal of the node, it should
    * pass the activate call to its' children.
    */
    virtual void ActivateL() = 0;

    /**
    * Deactivates this node, e.g. removes subscriptions. Depending on the
    * goal of the node, it should pass the deactivate call to its' children.
    */
    virtual void Deactivate() = 0;

    /**
    * Asks this node to check its security (via services interface). For
    * example, the security of contexts to be subscribed or actions to be
    * performed.
    * @return KErrNone if security check passed, otherwise any of the system
    *   wide error codes.
    */
    virtual TInt CheckSecurity() = 0;

    /**
    * Called by child node, to ask the parent to re-evaluate its value,
    * since the child's value has changed.
    */
    virtual void Evaluate() = 0;

    /**
    * Returns the value of the node.
    * @returns the ternary value of the condition.
    */
    IMPORT_C virtual TCFConditionValue Value() const;

    /**
    * Internalizes operation.
    * @param aStream Read stream.
    * @return None.
    */
    IMPORT_C virtual void InternalizeL( RReadStream& aStream );

    /**
    * Externalizes operation.
    * @param aStream Write stream.
    * @return None.
    */
    IMPORT_C virtual void ExternalizeL( RWriteStream& aStream );
    
    /**
    * Cleanup long term resources like persistent data files.
    * This is called by the context framework when the script is
    * uninstalled from the system.
    */ 
    IMPORT_C virtual void Cleanup();
    
    /**
    * Get a pointer to an extension.
    * Every extension is identified with a particular UID.
    * @param aUid Uid of the extension.
    * @return TAny*.
    */
    IMPORT_C virtual TAny* Extension( const TUid& aUid ) const;

protected:

    /**
    * C++ default constructor.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is the parent of this node, NULL if this is a root.
    *   The ownership of the parent node is not transferred.
    */
    IMPORT_C CCFOperationNode( MCFOperationServices& aServices,
            CCFOperationNode* aParent );

protected: // New functions

    /**
    * Generates DOM tree location info for this node.
    * @param aNode is the DOM node based on which info is generated.
    * @returns None.
    */
    IMPORT_C virtual void GenerateDomTreeLocationInfoL( CMDXMLNode& aNode );

protected: // Data

    // Operation service interface.
    MCFOperationServices&               iServices;

    // Node parent, NULL if this is root node.
    CCFOperationNode*                   iParent;

    // Condition value of the node.
    TCFConditionValue                   iValue;

    // Debug info for DOM tree location of this node.
    HBufC*                              iDomTreeLocationInfo;
    };

#endif // C_CFOPERATIONNODE_H

// End of File
