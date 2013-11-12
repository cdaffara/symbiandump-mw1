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
* Description:  CCFLessOrEqual class declaration.
*
*/



#ifndef C_CFLESSOREQUAL_H
#define C_CFLESSOREQUAL_H

// INCLUDES
#include "cfcontextoperation.h"

#include <e32base.h>

// FORWARD DECLARATIONS
class CCFContextObject;

// CLASS DECLARATION

/**
*   This class implements lessOrEqual -operation on Context Framework Scripts.
*
*  @lib CFScriptEngine
*  @since Series 60 2.6
*/
NONSHARABLE_CLASS( CCFLessOrEqual ): public CCFContextOperation
    {
public:

    /**
    * Creates a CCFLessOrEqual object
    * @param aServices is the operation services interface for nodes.
    * @param aParent is parent node for this node.
    * @param aName is name of the context object this operation operates on.
    * @param aSource is the source of the context object this operation
    *   operates on.
    * @param aType is the interpretation of aCmpVal
    * @param aCmpVal is a string representation of the comparison value
    * @return a pointer to the created instance of CCFLessOrEqual
    */
    static CCFLessOrEqual* NewL( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            TDesC& aName,
            TDesC& aSource,
            const CCFContextOperation::TCmpType aType,
            const TDesC& aCmpVal );

    /**
    * Creates a CCFLessOrEqual object
    * @param aServices is the operation services interface for nodes.
    * @param aParent is parent node for this node.
    * @param aName is name of the context object this operation operates on.
    * @param aSource is the source of the context object this operation
    *   operates on.
    * @param aType is the interpretation of aCmpVal
    * @param aCmpVal is a string representation of the comparison value
    * @return a pointer to the created instance of CCFLessOrEqual
    */
    static CCFLessOrEqual* NewLC( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            TDesC& aName,
            TDesC& aSource,
            const CCFContextOperation::TCmpType aType,
            const TDesC& aCmpVal );

    /**
    * Creates and parses a CCFLessOrEqual instance from an xml node.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is the parent of this node or NULL if this is a root.
    *   The ownership of the parent is not transferred.
    * @param aNode is the node to be parsed.
    * @return a pointer to the created instance of CCFLessOrEqual, or NULL if
    *   the node is not this less or equal node. May leave with any system wide
    *   error code.
    */
    static CCFLessOrEqual* ParseL( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            CMDXMLNode& aNode );

    /**
    * Destructor.
    */
    ~CCFLessOrEqual();

private:

    /**
    * Constructs this object
    */
    CCFLessOrEqual( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            HBufC* aName,
            HBufC* aSource,
            const CCFContextOperation::TCmpType aType );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL( const TDesC& aCmpVal );

protected: // From CCFContextOperation

    /**
    * Returns ETrue if this operation is true
    */
    TBool IsTrueL( const CCFContextObject& aContextObject );

private: // Data

    CCFContextOperation::TCmpType iType;
    HBufC*                        iCmpValue;
    };

#endif // C_CFLESSOREQUAL_H
