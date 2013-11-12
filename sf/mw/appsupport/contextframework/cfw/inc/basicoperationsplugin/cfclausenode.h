/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFClauseNode class declaration.
*
*/



#ifndef C_CFCLAUSENODE_H
#define C_CFCLAUSENODE_H

// INCLUDES
#include <e32base.h>
#include <cfkeyvaluepair.h>
#include <cfoperationnode.h>

// FORWARD DECLARATIONS
class CMDXMLNode;
class CCFScriptAction;
class CCFContextSourceCommandParameter;

// CLASS DECLARATION

/**
*  This class implements abstract clause node as a base class for XML clause
*  nodes (like if, elseif, and else) on Context Framework Scripts.
*
*  @lib CFScriptEngine
*  @since S60 v5.0
*/
NONSHARABLE_CLASS( CCFClauseNode ): public CCFOperationNode
    {
public: // Constructors and destructor

    /**
    * Destructor.
    */
    virtual ~CCFClauseNode();

public: // New functions

    /**
    * Fires actions of the node.
    * @return None.
    */
    virtual void FireActionsL() = 0;

public: // Functions from CCFOperationNode

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
    * @return None.
    */
    virtual void Evaluate() = 0;

protected:

    /**
    * C++ default constructor.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is parent node for this node. The ownership of the parent
    *   is not transffered.
    */
    CCFClauseNode( MCFOperationServices& aServices, CCFOperationNode* aParent );

protected: // New functions

    /**
    * Parses actions from an xml node.
    * @param aNode is the node to be parsed, root action node.
    * @param aActions is the array where parsed actions are stored.
    * @return None, may leave with any system wide error code.
    */
    void ParseActionsL( CMDXMLNode& aNode,
            RPointerArray< CCFScriptAction >& aActions );

private: // New functions

    /**
    * Parses a notify action from an xml node and returns a pointer to
    * CCFScriptAction instance.
    * @param aNode is the node to be parsed.
    * @return a ptr to CCFScriptAction instance parsed from the given node,
    *   Note that the ownership is transferred. Leaves with code
    *   KErrNotSupported if the node cannot be parsed.
    */
    CCFScriptAction* ParseNotifyActionL( CMDXMLNode& aNode );

    /**
    * Parses a publish context action from an xml node and returns a pointer to
    * CCFScriptAction instance.
    * @param aNode is the node to be parsed.
    * @return a ptr to CCFScriptAction instance parsed from the given node,
    *   Note that the ownership is transferred. Leaves with code
    *   KErrNotSupported if the node cannot be parsed.
    */
    CCFScriptAction* ParsePublishContextActionL( CMDXMLNode& aNode );

    /**
    * Parses a source command action from an xml node and returns a pointer to
    * CCFScriptAction instance.
    * @param aNode is the node to be parsed.
    * @return a ptr to CCFScriptAction instance parsed from the given node,
    *   Note that the ownership is transferred. Leaves with code
    *   KErrNotSupported if the node cannot be parsed.
    */
    CCFScriptAction* ParseSourceCommandActionL( CMDXMLNode& aNode );

    /**
    * Gets attributes from an xml node as key value pairs.
    * @param aNode is the node to be parsed for attributes.
    * @param aAttributes is the array for the attributes as key value pairs.
    * @return None.
    */
    void GetAttributesL( CMDXMLNode& aNode, RKeyValueArray& aAttributes );

    /**
    * Gets nested source command parameters containing optional nested
    * parameters (via recursive calls to itself) from an xml node.
    * @param aNode is the node to be parsed for the parameter.
    * @param aParent is the parent parameter.
    * @return None.
    */
    void GetNestedSourceCommandParametersL( CMDXMLNode& aNode,
            CCFContextSourceCommandParameter& aParent );

private: // Data

    };


#endif // C_CFCLAUSENODE_H
