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
* Description:  CCFCountRepeating class implementation.
*
*/



// INCLUDES
#include "cfcountrepeating.h"
#include "cfcontextoperationutils.h"
#include "cfbasicoptrace.h"

#include <cfcontextobject.h>
#include <gmxmlelement.h>

// CONSTANTS
_LIT( KScriptCountName,                 "count"             );
_LIT( KScriptRepeatIntervalAttribute,   "repeatInterval"    );
_LIT( KScriptTypeAttribute,             "type"              );
_LIT( KScriptCountTypeChangeName,       "change"            );
_LIT( KScriptCountTypeTriggerName,      "trigger"           );
_LIT( KScriptContextRefName,            "contextRef"        );
_LIT( KScriptPersistencyFileAttribute,  "persistencyFile"   );

static const TInt KCountMinAttributes = 1;
static const TUint KInitialCount = 0;


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CCFCountRepeating::CCFCountRepeating
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CCFCountRepeating::CCFCountRepeating( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    HBufC* aName,
    HBufC* aSource,
    const CCFContextOperation::TCmpType aType,
    const TUint aCount,
    const TCountType aCountType )
    :   CCFCount( aServices, aParent, aName, aSource, aType, aCount,
            aCountType )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// CCFCountRepeating::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CCFCountRepeating::ConstructL( const TDesC& aCmpVal )
    {
    FUNC_LOG;

    CCFCount::ConstructL( aCmpVal );
    }

// ---------------------------------------------------------------------------
// CCFCountRepeating::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCFCountRepeating* CCFCountRepeating::NewL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    TDesC& aName,
    TDesC& aSource,
    const CCFContextOperation::TCmpType aType,
    const TDesC& aCmpVal,
    const TUint aCount,
    const TCountType aCountType )
    {
    FUNC_LOG;

    CCFCountRepeating* self = NewLC( aServices,
            aParent,
            aName,
            aSource,
            aType,
            aCmpVal,
            aCount,
            aCountType );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CCFCountRepeating::NewLC
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCFCountRepeating* CCFCountRepeating::NewLC( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    TDesC& aName,
    TDesC& aSource,
    const CCFContextOperation::TCmpType aType,
    const TDesC& aCmpVal,
    const TUint aCount,
    const TCountType aCountType )
    {
    FUNC_LOG;

    HBufC* name = aName.AllocLC();
    HBufC* source = aSource.AllocLC();
    CCFCountRepeating* self = new( ELeave ) CCFCountRepeating( aServices,
            aParent,
            name,
            source,
            aType,
            aCount,
            aCountType );
    CleanupStack::PushL( self );
    self->ConstructL( aCmpVal );

    // Cleanup
    CleanupStack::Pop( self );
    CleanupStack::Pop( source );
    CleanupStack::Pop( name );

    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCFCountRepeating::ParseL
// Construction with parsing from a DOM node.
// -----------------------------------------------------------------------------
//
CCFCountRepeating* CCFCountRepeating::ParseL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    CMDXMLNode& aNode )
    {
    FUNC_LOG;

    if ( aNode.NodeName().CompareF( KScriptCountName ) != 0
        || aNode.NodeType() != CMDXMLNode::EElementNode )
        {
        return NULL; // Cannot create count operation from the given node.
        }

    // Check & get count attribute.
    CMDXMLElement& element = static_cast< CMDXMLElement& >( aNode );
    TPtrC countAttributeValue;
    if ( KErrNone != element.GetAttribute( KScriptRepeatIntervalAttribute,
            countAttributeValue ) )
        {
        INFO( "CCFCountRepeating::ParseL - count missing required attribute" );
        return NULL;
        }

    TInt expectedAttributeCount = KCountMinAttributes;
    // Check optional persistency file.
    TPtrC persistencyFileAttributeValue;
    if ( KErrNone == element.GetAttribute( KScriptPersistencyFileAttribute,
            persistencyFileAttributeValue ) )
        {
        if ( persistencyFileAttributeValue.Length() == 0 )
            {
            ERROR_GEN( "CCFCountRepeating::ParseL - Persistency File attribute length is zero!" );
            return NULL;
            }
        expectedAttributeCount++;
        }

    CCFCount::TCountType countType( CCFCount::ECountTypeChange );
    TPtrC countTypeValue;
    element.GetAttribute( KScriptTypeAttribute, countTypeValue );
    if ( countTypeValue.Length() )
        {
        expectedAttributeCount++;
        if ( element.NumAttributes() != expectedAttributeCount )
            {
            INFO_1( "CCFCountRepeating::ParseL - count attributes != %d",
                    expectedAttributeCount );
            return NULL;
            }

        if ( countTypeValue.CompareF( KScriptCountTypeChangeName ) == 0 )
            {
            // Default value, already set.
            }
        else if ( countTypeValue.CompareF( KScriptCountTypeTriggerName ) == 0 )
            {
            countType = CCFCount::ECountTypeTrigger;
            }
        else
            {
            INFO_1( "CCFCountRepeating::ParseL - unknown attribute value [%S] for type",
                    &countTypeValue );
            return NULL;
            }
        }
    else
        {
        if ( element.NumAttributes() != expectedAttributeCount )
            {
            INFO_1( "CCFCountRepeating::ParseL - count attributes != %d",
                    expectedAttributeCount );
            return NULL;
            }
        }
    // Convert the attribute value to TUint.
    TUint countValue( 0 );
    TLex lex( countAttributeValue );
    TInt err = lex.Val( countValue );
    ERROR_1( err,
        "CCFCountRepeating::ParseL - Extracting count type value from string [%S] failed!",
        &countAttributeValue );
    if ( err != KErrNone )
        {
        User::Leave( KErrArgument );
        }

    TPtrC contextSource;
    TPtrC contextType;
    TPtrC contextValue;
    TCmpType compareType( CCFContextOperation::EInvalidCmpType );
    TPtrC compareValue;
    TInt contextLevelDelay( 0 );

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
                            contextSource,
                            contextType,
                            contextValue,
                            contextLevelDelay );
                    if ( !contextRefOK )
                        {
                        argsOK = EFalse;
                        break;
                        }
                    }
                else
                    {
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
                                compareType,
                                compareValue );
                    if ( !comparisonTypeValueOK )
                        {
                        argsOK = EFalse;
                        break;
                        }
                    }
                else
                    {
                    argsOK = EFalse;
                    break;
                    }
                }
            }
        else if ( child->NodeType() != CMDXMLNode::ECommentNode )
            {
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
                compareType = CCFContextOperation::EStringCmp;
                compareValue.Set( contextValue );
                }
            }
        else if ( comparisonTypeValueOK && contextValue.Length() )
            {
            INFO( "CCFCountRepeating::ParseL - Ambiguous definition for comparison value" );
            return NULL;
            }
        parsed = ETrue;
        }

    CCFCountRepeating* self = NULL;
    if ( parsed )
        {
        // Below                                    // CLEANUP<< self
        self = NewLC( aServices,
                aParent,
                contextType,
                contextSource,
                compareType,
                compareValue,
                countValue,
                countType );
        if ( contextLevelDelay )
            {
            self->iContextLevelDelay = contextLevelDelay;
            }
        if ( persistencyFileAttributeValue.Length() > 0 )
            {
            self->iPersistencyFile = persistencyFileAttributeValue.AllocL();
            }
        CleanupStack::Pop( self );                  // CLEANUP>> self
        }
    else
        {
        INFO( "CCFCountRepeating::ParseL - Unsupported arguments" );
        }

    return self;
    }


// Destructor
CCFCountRepeating::~CCFCountRepeating()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CCFCountRepeating::IsTrueL
// ---------------------------------------------------------------------------
//
TBool CCFCountRepeating::IsTrueL( const CCFContextObject& /*aContext*/ )
    {
    FUNC_LOG;

    TBool value( EFalse );
    if ( iCurrentCount == iCmpCount )
        {
        value = ETrue;

        iCurrentCount = KInitialCount;
        }

    return value;
    }
