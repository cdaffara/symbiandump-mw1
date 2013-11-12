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
* Description:  CCFInRange class declaration. Currently supports the following
*                (cep) types:
*                                       "cep:int"
*                                       "cep:string"
*                                       "cep:double"
*                                       "cep:position"
*                                       "cep:date"
*
*/



#ifndef C_CFINRANGE_H
#define C_CFINRANGE_H


// INCLUDES
#include "cfcontextoperation.h"

#include <e32base.h>

// FORWARD DECLARATIONS
class CCFContextObject;

// CLASS DECLARATION

/**
*   This class implements InRange -operation on Context Framework Scripts.
*
*  @lib CFScriptEngine
*  @since Series 60 2.6
*/
NONSHARABLE_CLASS( CCFInRange ): public CCFContextOperation
    {
public: // Constructors and destructor

    /**
    * Creates a CCFInRange object
    * @param aServices is the operation services interface for nodes.
    * @param aParent is parent node for this node.
    * @param aName is name of the context object this operation operates on.
    * @param aSource is the source of the context object this operation
    *   operates on.
    * @param aType is the interpretation of aCmpVal
    * @param aMinVal is a string representation of the minimum comparison value
    * @param aMinVal is a string representation of the maximum comparison value
    * @return a pointer to the created instance of CCFInRange
    */
    static CCFInRange* NewL( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            TDesC& aName,
            TDesC& aSource,
            const CCFContextOperation::TCmpType aType,
            const TDesC& aMinVal,
            const TDesC& aMaxVal );

    /**
    * Creates a CCFInRange object
    * @param aServices is the operation services interface for nodes.
    * @param aParent is parent node for this node.
    * @param aName is name of the context object this operation operates on.
    * @param aSource is the source of the context object this operation
    *   operates on.
    * @param aType is the interpretation of aCmpVal
    * @param aMinVal is a string representation of the minimum comparison value
    * @param aMinVal is a string representation of the maximum comparison value
    * @return a pointer to the created instance of CCFInRange
    */
    static CCFInRange* NewLC( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            TDesC& aName,
            TDesC& aSource,
            const CCFContextOperation::TCmpType aType,
            const TDesC& aMinVal,
            const TDesC& aMaxVal );

    /**
    * Creates and parses a CCFInRange instance from an xml node.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is the parent of this node or NULL if this is a root.
    *   The ownership of the parent is not transferred.
    * @param aNode is the node to be parsed.
    * @return a pointer to the created instance of CCFInRange, or NULL if the
    *   node is not this in range node. May leave with any system wide error
    *   code.
    */
    static CCFInRange* ParseL( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            CMDXMLNode& aNode );

    /**
    * Destructor.
    */
    ~CCFInRange();

private:

    /**
    * C++ default constructor.
    */
    CCFInRange( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            HBufC* aName,
            HBufC* aSource,
            const CCFContextOperation::TCmpType aType );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL( const TDesC& aMinVal, const TDesC& aMaxVal );

protected: // From CCFContextOperation

    /**
    * Returns ETrue if this operation is true.
    */
    TBool IsTrueL( const CCFContextObject& aContextObject );

private: // New functions

    /**
    * Method that takes two GPS positions as (latitude,longitude)-pairs
    * (units==DEGREES) and calculates their ellipsoid distance.
    * This is used in calculation of the value for cep:position types.
    * NOTE: This algorithm can be found from the following book:
    *       "Astronomical Algorithms" by Jean Meeus.
    * @param aPos1Latitude latitude of point one in degrees.
    * @param aPos1Longitude longitude of point one in degrees.
    * @param aPos2Latitude latitude of point two in degrees.
    * @param aPos2Longitude longitude of point two in degrees.
    * @return calculated distance (in meters) if successful,
    *   otherwise KErrGeneral.
    */
    TReal EllipsoidDistanceBetweenTwoPositions(
            TReal aPos1Latitude, TReal aPos1Longitude,
            TReal aPos2Latitude, TReal aPos2Longitude );

private: // Data

    CCFContextOperation::TCmpType iType;
    HBufC*                        iMinVal;
    HBufC*                        iMaxVal;
    };

#endif // C_CFINRANGE_H
