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
* Description:  CFContextOperationUtils class declaration.
*
*/



#ifndef CFCONTEXTOPERATIONUTILS_H
#define CFCONTEXTOPERATIONUTILS_H

// INCLUDES
#include <e32base.h>

#include "cfcontextoperation.h"

// FORWARD DECLARATIONS
class CMDXMLNode;

// CLASS DECLARATION

/**
*  Context Framework Context Operation utils class.
*
*  @lib CFScriptEngine
*  @since S60 v5.0
*/
NONSHARABLE_CLASS( CFContextOperationUtils )
    {
public: // Util functions

    /**
    * Parses data for a comparison operation having two arguments. Iterates
    * through all siblings so only two nodes are allowed to have real content.
    * @param aNode is the operation node whose two arguments will be parsed.
    * @param aContextSource will be set to context source of the contextRef.
    * @param aContextType will be set to context type of the contextRef.
    * @param aCompareType will be set to compare type of the comparison.
    * @param aCompareValue will be set to compare value of the comparison.
    * @param aContextLevelDelay will be set to the value of context level delay
    *   attribute if it was present.
    * @return ETrue if exactly two arguments were found and parsed successfully,
    *   EFalse otherwise.
    */
    static TBool ParseTwoComparisonArgs( CMDXMLNode& aNode,
            TPtrC& aContextSource,
            TPtrC& aContextType,
            CCFContextOperation::TCmpType& aCompareType,
            TPtrC& aCompareValue,
            TInt& aContextLevelDelay );

    /**
    * Parses data from a contextRef xml node.
    * @param aNode is the node to be parsed.
    * @param aContextSource will be set to context source of the contextRef.
    * @param aContextType will be set to context type of the contextRef.
    * @param aContextValue will be set to context value of the contextRef or
    *   its' data is cleared if value was not defined by the contextRef.
    * @param aContextLevelDelay will be set to the value of context level delay
    *   attribute if it was present.
    * @return ETrue if context ref was found and at least source and type were
    *   parsed successfully, EFalse otherwise.
    */
    static TBool ParseContextRef( CMDXMLNode& aNode,
            TPtrC& aContextSource,
            TPtrC& aContextType,
            TPtrC& aContextValue,
            TInt& aContextLevelDelay );

    /**
    * Parses a script data type definition xml node (e.g. used by comparisons).
    * @param aNode is the node to be parsed.
    * @param aCompareType will be set to compare type of the comparison.
    * @param aCompareValue will be set to compare value of the comparison.
    * @return ETrue if script data type definition was found and parsed
    *   successfully, EFalse otherwise.
    */
    static TBool ParseComparisonTypeValue( CMDXMLNode& aNode,
            CCFContextOperation::TCmpType& aCompareType,
            TPtrC& aCompareValue );

    /**
    * Gets comparison type for a context operation.
    * @param aComparisonType is the descriptor defining comparison type.
    * @return CCFContextOperation::TCmpType comparison type.
    *   CCFContextOperation::EInvalidCmpType if aComparisonType is unknown.
    */
    static CCFContextOperation::TCmpType ComparisonType(
        const TDesC& aComparisonType );


    /**
    * Parses a descriptor to TTime object.
    * @param aString is the time in a descriptor.
    * @return TTime object based on aString.
    */
    static TTime StringToTimeL( const TDesC& aString );

    /**
    * Converts a string to TReal object.
    * @param aString is the string to convert.
    * @return TReal object created from aString.
    */
    static TReal StringToRealL( const TDesC& aString );

    /**
    * Converts a string containing latitude and longitude values into two reals.
    * Note! Unable to handle position values larger than 128 characters.
    * @param aString is the string containing position of format
    *   "latitude:longitude".
    * @param aLatitude will be the latitude extracted from the string.
    * @param aLongitude will be the longitude extracted from the string.
    */
    static void PositionToRealsL( const TDesC& aString,
            TReal& aLatitude,
            TReal& aLongitude );

    /**
    * Converts a string to an integer value.
    * @param aString is the string to convert to an integer.
    * @return TInt converted from aString.
    */
    static TInt StringToIntL( const TDesC& aString );

    };


#endif // CFCONTEXTOPERATIONUTILS_H
