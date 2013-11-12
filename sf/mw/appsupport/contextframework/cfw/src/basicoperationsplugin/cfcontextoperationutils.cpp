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
* Description:  CFContextOperationUtils class implementation.
*
*/



// INCLUDES
#include <gmxmlelement.h>
#include <gmxmltext.h>
#include <f32file.h> // For delimiter definitions.

#include "cfcontextoperationutils.h"
#include "cfbasicoptrace.h"

// CONSTANTS
_LIT( KScriptContextRefName,            "contextRef"            );

_LIT( KScriptCepInt,                    "cep:int"               );
_LIT( KScriptInt,                       "int"                   );
_LIT( KScriptCepString,                 "cep:string"            );
_LIT( KScriptString,                    "string"                );
_LIT( KScriptCepFloat,                  "cep:double"            );
_LIT( KScriptFloat,                     "double"                );
_LIT( KScriptCepPosition,               "cep:position"          );
_LIT( KScriptCepDate,                   "cep:date"              );
_LIT( KScriptDate,                      "date"                  );

_LIT( KScriptSourceAttribute,           "source"                );
_LIT( KScriptTypeAttribute,             "type"                  );
_LIT( KScriptValueAttribute,            "value"                 );
_LIT( KScriptEvaluationDelayAttribute,  "evaluationDelay"       );

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CFContextOperationUtils::ParseTwoComparisonArgs
// -----------------------------------------------------------------------------
//
TBool CFContextOperationUtils::ParseTwoComparisonArgs( CMDXMLNode& aNode,
    TPtrC& aContextSource,
    TPtrC& aContextType,
    CCFContextOperation::TCmpType& aCompareType,
    TPtrC& aCompareValue,
    TInt& aContextLevelDelay )
    {
    FUNC_LOG;

    TPtrC contextValue;
    TBool contextRefOK( EFalse );
    TBool comparisonTypeValueOK( EFalse );
    TBool argsOK( ETrue ); // Will be set to false if unknown nodes detected.
    CMDXMLNode* child = aNode.FirstChild();
    while ( child )
        {
        if ( child->NodeType() == CMDXMLNode::EElementNode )
            {
            if ( child->NodeName().CompareF( KScriptContextRefName ) == 0 )
                {
                if ( !contextRefOK )
                    {
                    contextRefOK = CFContextOperationUtils::ParseContextRef(
                            *child,
                            aContextSource,
                            aContextType,
                            contextValue,
                            aContextLevelDelay );
                    if ( !contextRefOK )
                        {
                        argsOK = EFalse;
                        break;
                        }
                    }
                else
                    {
                    INFO( "CFContextOperationUtils::ParseTwoComparisonArgs - Redefinition not allowed, context ref already defined" );
                    argsOK = EFalse;
                    break;
                    }
                }
            else
                {
                if ( !comparisonTypeValueOK )
                    {
                    comparisonTypeValueOK
                        = CFContextOperationUtils::ParseComparisonTypeValue(
                                *child,
                                aCompareType,
                                aCompareValue );
                    if ( !comparisonTypeValueOK )
                        {
                        argsOK = EFalse;
                        break;
                        }
                    }
                else
                    {
                    INFO( "CFContextOperationUtils::ParseTwoComparisonArgs - Redefinition not allowed, comparison type value already defined" );
                    argsOK = EFalse;
                    break;
                    }
                }
            }
        else if ( child->NodeType() != CMDXMLNode::ECommentNode )
            {
            TPtrC nodeName( child->NodeName() );
            INFO_1( "CFContextOperationUtils::ParseTwoComparisonArgs - Unsupported node [%S]",
                    &nodeName );
            argsOK = EFalse;
            break;
            }
        child = child->NextSibling();
        }

    TBool parsed( EFalse );
    if ( argsOK && contextRefOK )
        {
        if ( ( comparisonTypeValueOK && !contextValue.Length() )
            || ( !comparisonTypeValueOK && contextValue.Length() ) )
            {
            if ( !comparisonTypeValueOK )
                {
                aCompareType = CCFContextOperation::EStringCmp;
                aCompareValue.Set( contextValue );
                }
            parsed = ETrue;
            }
        else
            {
            if ( !comparisonTypeValueOK )
                {
                INFO( "CFContextOperationUtils::ParseTwoComparisonArgs - Value to compare the context with not defined" );
                }
            else
                {
                INFO( "CFContextOperationUtils::ParseTwoComparisonArgs - Ambiguous, value to compare the context with defined twice" );
                }
            }
        }

    return parsed;
    }

// -----------------------------------------------------------------------------
// CFContextOperationUtils::ParseContextRef
// -----------------------------------------------------------------------------
//
TBool CFContextOperationUtils::ParseContextRef( CMDXMLNode& aNode,
    TPtrC& aContextSource,
    TPtrC& aContextType,
    TPtrC& aContextValue,
    TInt& aContextLevelDelay )
    {
    FUNC_LOG;

    if ( aNode.NodeName().CompareF( KScriptContextRefName ) != 0
        || aNode.NodeType() != CMDXMLNode::EElementNode )
        {
        return EFalse; // Cannot parse context ref from the given node.
        }

    TBool contextRefOK( EFalse );
    CMDXMLElement& element = static_cast< CMDXMLElement& >( aNode );
    TPtrC evaluationDelay;
    if ( KErrNone == element.GetAttribute( KScriptSourceAttribute,
            aContextSource ) )
        {
        if ( KErrNone == element.GetAttribute( KScriptTypeAttribute,
                aContextType ) )
            {
            if ( aContextSource.Length() && aContextType.Length()
                && !element.HasChildNodes() )
                {
                contextRefOK = ETrue;
                TInt numAttributes( 2 ); // Two attributes read so far.
                TPtrC evaluationDelay;
                // Extract context level evaluation delay if defined.
                if ( KErrNone == element.GetAttribute(
                        KScriptEvaluationDelayAttribute,
                        evaluationDelay ) )
                    {
                    ++numAttributes;
                    if ( evaluationDelay.Length() )
                        {
                        // Convert delay to integer.
                        TLex parseInt( evaluationDelay );
                        parseInt.SkipSpace();
                        TInt contextLevelDelay( 0 );
                        TInt err = parseInt.Val( contextLevelDelay );
                        if ( err != KErrNone || contextLevelDelay < 0  )
                            {
                            // Evaluation delay defined but invalid.
                            contextRefOK = EFalse;
                            }
                        else if ( contextLevelDelay > 0 )
                            {
                            aContextLevelDelay = contextLevelDelay;
                            }
                        }
                    }
                // Extract context value if defined.
                if ( KErrNone != element.GetAttribute( KScriptValueAttribute,
                        aContextValue ) )
                    {
                    // Ensure empty value when it is not defined.
                    aContextValue.Set( 0, 0 );
                    }
                else
                    {
                    ++numAttributes;
                    }

                if ( numAttributes != element.NumAttributes() )
                    {
                    INFO( "CFContextOperationUtils::ParseContextRef - Unsupported amount of attributes" );
                    contextRefOK = EFalse; // Unsupported contextRef.
                    }
                }
            else
                {
                INFO( "CFContextOperationUtils::ParseContextRef - Unsupported context ref" );
                }
            }
        else
            {
            INFO( "CFContextOperationUtils::ParseContextRef - Getting type failed" );
            }
        }
    else
        {
        INFO( "CFContextOperationUtils::ParseContextRef - Getting source failed" );
        }

    return contextRefOK;
    }

// -----------------------------------------------------------------------------
// CFContextOperationUtils::ParseComparisonTypeValue
// -----------------------------------------------------------------------------
//
TBool CFContextOperationUtils::ParseComparisonTypeValue( CMDXMLNode& aNode,
    CCFContextOperation::TCmpType& aCompareType,
    TPtrC& aCompareValue )
    {
    FUNC_LOG;

    TBool parsed( EFalse );
    if ( aNode.NodeType() == CMDXMLNode::EElementNode )
        {
        aCompareType
            = CFContextOperationUtils::ComparisonType( aNode.NodeName() );

        if ( aCompareType != CCFContextOperation::EInvalidCmpType )
            {
            // Parse comparison value from first text node child.
            CMDXMLNode* child = aNode.FirstChild();
            while ( child )
                {
                // Allow one text node and multiple comments, nothing else.
                if ( child->NodeType() == CMDXMLNode::ETextNode )
                    {
                    if ( parsed )
                        {
                        TPtrC nodeName( child->NodeName() );
                        INFO_1( "CFContextOperationUtils::ParseComparisonTypeValue - One definition is allowed. [%S] is not supported",
                                &nodeName );
                        // Already parsed, flag inability to parse.
                        parsed = EFalse;
                        break;
                        }
                    CMDXMLText* text = static_cast< CMDXMLText* >( child );
                    aCompareValue.Set( text->Data() );
                    parsed = ETrue;
                    }
                else if ( child->NodeType() != CMDXMLNode::ECommentNode )
                    {
                    TPtrC nodeName( child->NodeName() );
                    INFO_1( "CFContextOperationUtils::ParseComparisonTypeValue - Unsupported node [%S]",
                            &nodeName );
                    parsed = EFalse;
                    break;
                    }
                child = child->NextSibling();
                }
            }
        else
            {
            INFO( "CFContextOperationUtils::ParseComparisonTypeValue - Invalid comparison type" );
            }
        }
    else
        {
        TPtrC nodeName( aNode.NodeName() );
        INFO_1( "CFContextOperationUtils::ParseComparisonTypeValue - Unsupported node [%S]",
                &nodeName );
        }

    return parsed;
    }

// -----------------------------------------------------------------------------
// CFContextOperationUtils::ComparisonType
// -----------------------------------------------------------------------------
//
CCFContextOperation::TCmpType CFContextOperationUtils::ComparisonType(
    const TDesC& aComparisonType )
    {
    FUNC_LOG;

    CCFContextOperation::TCmpType ret( CCFContextOperation::EInvalidCmpType );

    if ( aComparisonType.CompareF( KScriptCepInt ) == 0  ||
        aComparisonType.CompareF( KScriptInt ) == 0 )
        {
        ret = CCFContextOperation::EIntCmp;
        }
    else if ( aComparisonType.CompareF( KScriptCepDate ) == 0 ||
        aComparisonType.CompareF( KScriptDate ) == 0 )
        {
        ret = CCFContextOperation::ETimeCmp;
        }
    else if ( aComparisonType.CompareF( KScriptCepPosition ) == 0 )
        {
        ret = CCFContextOperation::EPositionCmp;
        }
    else if ( aComparisonType.CompareF( KScriptCepString ) == 0 ||
        aComparisonType.CompareF( KScriptString ) == 0 )
        {
        ret = CCFContextOperation::EStringCmp;
        }
    else if ( aComparisonType.CompareF( KScriptCepFloat ) == 0 ||
        aComparisonType.CompareF( KScriptFloat ) == 0 )
        {
        ret = CCFContextOperation::EFloatCmp;
        }

    return ret;
    }


// ---------------------------------------------------------------------------
// CFContextOperationUtils::StringToTimeL
// ---------------------------------------------------------------------------
//
TTime CFContextOperationUtils::StringToTimeL( const TDesC& aString )
    {
    FUNC_LOG;

    TTime val;
    TInt err = val.Set( aString );
    if ( err != KErrNone )
        {
        ERROR_1( err, "Cannot interpret string as TTime. Value: %S", &aString );
        User::Leave( err );
        }

    return val;
    }

// ---------------------------------------------------------------------------
// CFContextOperationUtils::StringToRealL
// ---------------------------------------------------------------------------
//
TReal CFContextOperationUtils::StringToRealL( const TDesC& aString )
    {
    FUNC_LOG;

    TLex lexer( aString );
    TInt err( KErrNone );
    TReal val( 0 );
    TChar decPoint( KExtDelimiter ); // '.'

    // Parse float from string; note that decimal point delimiter must be '.'
    err = lexer.Val( val, decPoint );
    if ( err != KErrNone )
        {
        ERROR_1( err, "Cannot interpret string as float. Value: %S", &aString );
        User::Leave( err );
        }

    return val;
    }

// ---------------------------------------------------------------------------
// CFContextOperationUtils::PositionToRealsL
// Converts a string containing latitude and longitude values into two real
// values passed by reference.
// ---------------------------------------------------------------------------
//
void CFContextOperationUtils::PositionToRealsL( const TDesC& aString,
    TReal& aLatitude,
    TReal& aLongitude )
    {
    FUNC_LOG;

    TLex lexer( aString );
    TBuf< KMaxName > latDes( KErrNone );
    TBuf< KMaxName > lonDes( KErrNone );
    TChar posDelimiter( KDriveDelimiter ); // ':'

    // Get the latitude to descriptor.
    while ( lexer.Peek() != posDelimiter && // Delimiter char not found.
            !lexer.Eos()                 )  // End of string not reached.
        {
        latDes.Append( lexer.Get() );
        }

    if ( lexer.Eos()                 || // Eos already.
         lexer.Peek() != posDelimiter ) // Delimiter not found.
        {
        ERROR_GEN_1( "Cannot interpret position as floats. String: %S", &aString );
        User::Leave( KErrBadDescriptor );
        }

    // Skip over delimiter.
    lexer.Inc();

    // Get the longitude to descriptor.
    while ( !lexer.Eos() )
        {
        latDes.Append( lexer.Get() );
        }
    // Parse latitude and longitude into given reals.
    aLatitude =  StringToRealL( latDes );
    aLongitude = StringToRealL( lonDes );
    }

// ---------------------------------------------------------------------------
// CFContextOperationUtils::StringToIntL
// ---------------------------------------------------------------------------
//
TInt CFContextOperationUtils::StringToIntL( const TDesC& aString )
    {
    FUNC_LOG;

    TLex lexer( aString );
    TInt val( KErrNone );
    TInt err = lexer.Val( val );
    if ( err != KErrNone )
        {
        ERROR_1( err, "Cannot interpret string as int. Value: %S", &aString );
        User::Leave( err );
        }
    return val;
    }
