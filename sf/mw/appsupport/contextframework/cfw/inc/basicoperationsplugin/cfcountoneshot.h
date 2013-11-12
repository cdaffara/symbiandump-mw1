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
* Description:  CCFCountOneShot class declaration.
*
*/



#ifndef C_CFCOUNTONESHOT_H
#define C_CFCOUNTONESHOT_H

// INCLUDES
#include "cfcount.h"

// FORWARD DECLARATIONS
class CCFContextObject;

// CLASS DECLARATION

/**
*  This class implements count oneShotAt -operation on Context Framework
*  Scripts.
*
*  @lib CFScriptEngine
*  @since S60 v5.0
*/
NONSHARABLE_CLASS( CCFCountOneShot ) : public CCFCount
    {
public: // Constructors and destructor

    /**
    * Creates a CCFCountOneShot object.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is parent node for this node.
    * @param aName is name of the context object this operation operates on.
    * @param aSource is the source of the context object this operation
    *   operates on.
    * @param aType is the interpretation of aCmpVal.
    * @param aCmpVal is a string representation of the comparison value.
    * @param aCount is the count required for this operation to get true.
    * @param aCountType is the type of the count.
    * @return a pointer to the created instance of CCFCountOneShot.
    */
    static CCFCountOneShot* NewL( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            TDesC& aName,
            TDesC& aSource,
            const CCFContextOperation::TCmpType aType,
            const TDesC& aCmpVal,
            const TUint aCount,
            const TCountType aCountType );

    /**
    * Creates a CCFCountOneShot object.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is parent node for this node.
    * @param aName is name of the context object this operation operates on.
    * @param aSource is the source of the context object this operation
    *   operates on.
    * @param aType is the interpretation of aCmpVal.
    * @param aCmpVal is a string representation of the comparison value.
    * @param aCount is the count required for this operation to get true.
    * @param aCountType is the type of the count.
    * @return a pointer to the created instance of CCFCountOneShot.
    */
    static CCFCountOneShot* NewLC( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            TDesC& aName,
            TDesC& aSource,
            const CCFContextOperation::TCmpType aType,
            const TDesC& aCmpVal,
            const TUint aCount,
            const TCountType aCountType );

    /**
    * Creates and parses a CCFCountOneShot instance from an xml node.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is the parent of this node or NULL if this is a root.
    *   The ownership of the parent is not transferred.
    * @param aNode is the node to be parsed.
    * @return a pointer to the created instance of CCFCountOneShot, or NULL if
    *   the node is not this count node. May leave with any system wide error
    *   code.
    */
    static CCFCountOneShot* ParseL( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            CMDXMLNode& aNode );

    /**
    * Destructor.
    */
    ~CCFCountOneShot();

protected: // From CCFContextOperation

    /**
    * Returns ETrue if this operation is true.
    */
    TBool IsTrueL( const CCFContextObject& aContext );

private:

    /**
    * C++ default constructor.
    */
    CCFCountOneShot( MCFOperationServices& aServices,
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

private: // New functions

    /**
    * Stops further operation execution.
    */
    void Finished();

private: // Data

    };


#endif // C_CFCOUNTONESHOT_H
