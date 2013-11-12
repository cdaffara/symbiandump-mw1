/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Operation node v1 class declaration.
*
*/


#ifndef OPERATIONNODE_H
#define OPERATIONNODE_H

// SYSTEM INCLUDE FILES
#include <cfoperationnode.h>
#include <cfscriptlistener.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS
class CCFContextObject;

// DATA TYPES

// CLASS DECLARATION

/**
 * Operation node v1.
 *
 * @lib None.
 * @since S60 5.0
 */
class COperationNode_V1 : public CCFOperationNode,
public MCFScriptListener
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param None.
     * @return COperationNode_V1
     */
    static COperationNode_V1* NewL( MCFOperationServices& aServices,
        CCFOperationNode* aParent,
        const TDesC& aSource,
        const TDesC& aType,
        const TDesC& aValue );
    static COperationNode_V1* NewLC( MCFOperationServices& aServices,
        CCFOperationNode* aParent,
        const TDesC& aSource,
        const TDesC& aType,
        const TDesC& aValue );

    /**
     * C++ destructor.
     */
    virtual ~COperationNode_V1( );
    
public:

    /**
     * Parses operation node.
     */
    static CCFOperationNode* ParseL( MCFOperationServices& aServices,
        CCFOperationNode* aParent,
        CMDXMLNode& aNode );

public:

    // @see CCFOperationNode
    void ActivateL();

    // @see CCFOperationNode
    void Deactivate();

    // @see CCFOperationNode
    TInt CheckSecurity();
    
    // @see CCFOperationNode
    void Evaluate();
    
    // @see MCFScriptListener
    TBool Evaluate( const CCFContextObject& aContext,
        TInt& aContextLevelDelay );
    
    // @see MCFScriptListener
    TPtrC Source() const;

    // @see MCFScriptListener
    TPtrC Type() const;

    // @see MCFScriptListener
    TBool IsAllRequired() const;
    
private:

    // Parse text node from element node and store data
    static void ParseTextNode( CMDXMLNode& aNode, TPtrC& aData );

private:

    COperationNode_V1( MCFOperationServices& aServices, CCFOperationNode* aParent );
    void ConstructL( const TDesC& aSource, const TDesC& aType, const TDesC& aValue);

private: // Data

    /** Context object which to observer */
    CCFContextObject* iCo;
    };

#endif // OPERATIONNODE_H

