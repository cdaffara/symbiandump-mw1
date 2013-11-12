/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MCFPlugInOperation interface.
*
*/



#ifndef M_CFPLUGINOPERATION_H
#define M_CFPLUGINOPERATION_H

// INCLUDES
#include <e32base.h>


// FORWARD DECLARATIONS
class CCFOperationNode;
class CCFScriptRoot;
class MCFOperationServices;
class CMDXMLNode;

// CLASS DECLARATION

/**
*  This is a plugin operation interface for Script Engine scripts. Parsing
*  operations will be executed via this interface.
*
*  @lib CFScriptEngine
*  @since S60 v5.0
*/
class MCFPlugInOperation
    {
public: // New functions

    /**
    * Parses a xml node and returns a pointer to CCFOperationNode instance.
    * @since S60 5.0
    * @param aParent is the parsed parent node for the aNode.
    * @param aNode is the node to be parsed.
    * @param aOperationServices is the operation services reference to be used
    *   by the new operation node.
    * @param aUid Implementation uid of the provider.
    * @return a ptr to CCFOperationNode instance parsed from the given node,
    *   note that the ownership is transferred to the caller. Returns NULL if
    *   the node cannot be parsed.
    */
    virtual CCFOperationNode* ParseL( CCFOperationNode* aParent,
            CMDXMLNode& aNode,
            MCFOperationServices& aOperationServices,
            TUid& aUid ) = 0;

    /**
    * Parses a xml node and returns a pointer to CCFScriptRoot instance.
    * @since S60 5.0
    * @param aParent is the parsed parent node for the aNode.
    * @param aNode is the node to be parsed.
    * @param aOperationServices is the operation services reference to be used
    *   by the new script root node.
    * @param aUid Implementation uid of the provider.
    * @return a ptr to CCFScriptRoot instance parsed from the given node,
    *   note that the ownership is transferred to the caller. Returns NULL if
    *   the node cannot be parsed.
    */
    virtual CCFScriptRoot* ParseScriptRootL( CCFOperationNode* aParent,
            CMDXMLNode& aNode,
            MCFOperationServices& aOperationServices,
            TUid& aUid ) = 0;


protected: // Destructor

    // Deny destruction through this interface.
    virtual ~MCFPlugInOperation(){};

    };


#endif // M_CFPLUGINOPERATION_H
