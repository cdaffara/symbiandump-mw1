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
* Description:  CCFCountRepeating class declaration.
*
*/



#ifndef C_CFCOUNTREPEATING_H
#define C_CFCOUNTREPEATING_H

// INCLUDES
#include "cfcount.h"

// FORWARD DECLARATIONS
class CCFContextObject;

// CLASS DECLARATION

/**
*  This class implements count repeatInterval -operation on Context Framework
*  Scripts.
*
*  @lib CFScriptEngine
*  @since S60 v5.0
*/
NONSHARABLE_CLASS( CCFCountRepeating ) : public CCFCount
    {
public: // Constructors and destructor

    /**
    * Creates a CCFCountRepeating object.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is parent node for this node.
    * @param aName is name of the context object this operation operates on.
    * @param aSource is the source of the context object this operation
    *   operates on.
    * @param aType is the interpretation of aCmpVal.
    * @param aCmpVal is a string representation of the comparison value.
    * @param aCount is the count required for this operation to get true.
    * @param aCountType is the type of the count.
    * @return a pointer to the created instance of CCFCountRepeating.
    */
    static CCFCountRepeating* NewL( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            TDesC& aName,
            TDesC& aSource,
            const CCFContextOperation::TCmpType aType,
            const TDesC& aCmpVal,
            const TUint aCount,
            const TCountType aCountType );

    /**
    * Creates a CCFCountRepeating object.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is parent node for this node.
    * @param aName is name of the context object this operation operates on.
    * @param aSource is the source of the context object this operation
    *   operates on.
    * @param aType is the interpretation of aCmpVal.
    * @param aCmpVal is a string representation of the comparison value.
    * @param aCount is the count required for this operation to get true.
    * @param aCountType is the type of the count.
    * @return a pointer to the created instance of CCFCountRepeating.
    */
    static CCFCountRepeating* NewLC( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            TDesC& aName,
            TDesC& aSource,
            const CCFContextOperation::TCmpType aType,
            const TDesC& aCmpVal,
            const TUint aCount,
            const TCountType aCountType );

    /**
    * Creates and parses a CCFCountRepeating instance from an xml node.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is the parent of this node or NULL if this is a root.
    *   The ownership of the parent is not transferred.
    * @param aNode is the node to be parsed.
    * @return a pointer to the created instance of CCFCountRepeating, or NULL if
    *   the node is not this count node. May leave with any system wide error
    *   code.
    */
    static CCFCountRepeating* ParseL( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            CMDXMLNode& aNode );

    /**
    * Destructor.
    */
    ~CCFCountRepeating();

protected: // From CCFContextOperation

    /**
    * Returns ETrue if this operation is true
    */
    TBool IsTrueL( const CCFContextObject& aContext );

protected:

    /**
    * C++ default constructor.
    */
    CCFCountRepeating( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            HBufC* aName,
            HBufC* aSource,
            const CCFContextOperation::TCmpType aType,
            const TUint aCount,
            const TCountType aCountType );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL( const TDesC& aCmpVal );

private: // Data

    };


#endif // C_CFCOUNTREPEATING_H
