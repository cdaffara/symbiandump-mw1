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
* Description:  CCFCountOneShot class implementation.
*
*/



// INCLUDES
#include "cfcountoneshot.h"
#include "cfcontextoperationutils.h"
#include "cfbasicoptrace.h"

#include <cfcontextobject.h>
#include <gmxmlelement.h>

// CONSTANTS
_LIT( KScriptCountName,                "count"              );
_LIT( KScriptOneShotAtAttribute,       "oneShotAt"          );
_LIT( KScriptTypeAttribute,            "type"               );
_LIT( KScriptCountTypeRemainName,      "remain"             );
_LIT( KScriptCountTypeChangeName,      "change"             );
_LIT( KScriptCountTypeTriggerName,     "trigger"            );
_LIT( KScriptContextRefName,           "contextRef"         );
_LIT( KScriptPersistencyFileAttribute, "persistencyFile"    );

static const TInt KCountMinAttributes = 1;
static const TUint KInitialCount = 0;


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CCFCountOneShot::CCFCountOneShot
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CCFCountOneShot::CCFCountOneShot( MCFOperationServices& aServices,
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
// CCFCountOneShot::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CCFCountOneShot::ConstructL( const TDesC& aCmpVal )
    {
    FUNC_LOG;

    CCFCount::ConstructL( aCmpVal );
    }

// ---------------------------------------------------------------------------
// CCFCountOneShot::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCFCountOneShot* CCFCountOneShot::NewL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    TDesC& aName,
    TDesC& aSource,
    const CCFContextOperation::TCmpType aType,
    const TDesC& aCmpVal,
    const TUint aCount,
    const TCountType aCountType )
    {
    FUNC_LOG;

    CCFCountOneShot* self = NewLC( aServices,
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
// CCFCountOneShot::NewLC
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCFCountOneShot* CCFCountOneShot::NewLC( MCFOperationServices& aServices,
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
    CCFCountOneShot* self = new( ELeave ) CCFCountOneShot( aServices,
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
// CCFCountOneShot::ParseL
// Construction with parsing from a DOM node.
// -----------------------------------------------------------------------------
//
CCFCountOneShot* CCFCountOneShot::ParseL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    CMDXMLNode& aNode )
    {
    FUNC_LOG;

    if ( aNode.NodeName().CompareF( KScriptCountName ) != 0
        || aNode.NodeType() != CMDXMLNode::EElementNode )
        {
        return NULL; // Cannot create count operation from the given node.
        }

    // Check & get count attributes.
    CMDXMLElement& element = static_cast< CMDXMLElement& >( aNode );
    TPtrC countAttributeValue;
    if ( KErrNone != element.GetAttribute( KScriptOneShotAtAttribute,
            countAttributeValue ) )
        {
        INFO( "CCFCountOneShot::ParseL - count missing required attribute" );
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
            ERROR_GEN( "CCFCountOneShot::ParseL - Persistency File attribute length is zero!" );
            return NULL;
            }
        expectedAttributeCount++;
        }

    CCFCount::TCountType countType( CCFCount::ECountTypeRemain );
    TPtrC countTypeValue;
    element.GetAttribute( KScriptTypeAttribute, countTypeValue );
    if ( countTypeValue.Length() )
        {
        expectedAttributeCount++;
        if ( element.NumAttributes() != expectedAttributeCount )
            {
            INFO_1( "CCFCountOneShot::ParseL - count attributes != %d",
                    expectedAttributeCount );
            return NULL;
            }

        if ( countTypeValue.CompareF( KScriptCountTypeRemainName ) == 0 )
            {
            // Default value, already set.
            }
        else if ( countTypeValue.CompareF( KScriptCountTypeChangeName ) == 0 )
            {
            countType = CCFCount::ECountTypeChange;
            }
        else if ( countTypeValue.CompareF( KScriptCountTypeTriggerName ) == 0 )
            {
            countType = CCFCount::ECountTypeTrigger;
            }
        else
            {
            INFO_1( "CCFCountOneShot::ParseL - unknown attribute value [%S] for type",
                    &countTypeValue );
            return NULL;
            }
        }
    else
        {
        if ( element.NumAttributes() != expectedAttributeCount )
            {
            INFO_1( "CCFCountOneShot::ParseL - count attributes != %d",
                    expectedAttributeCount );
            return NULL;
            }
        }
    // Convert the attribute value to TUint.
    TUint countValue( 0 );
    TLex lex( countAttributeValue );
    TInt err = lex.Val( countValue );
    ERROR_1( err,
        "CCFCountOneShot::ParseL - Extracting count value from string [%S] failed!",
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
            INFO( "CCFCountOneShot::ParseL - Ambiguous definition for comparison value" );
            return NULL;
            }
        parsed = ETrue;
        }

    CCFCountOneShot* self = NULL;
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
        INFO( "CCFCountOneShot::ParseL - Unsupported arguments" );
        }

    return self;
    }


// Destructor
CCFCountOneShot::~CCFCountOneShot()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CCFCountOneShot::IsTrueL
// ---------------------------------------------------------------------------
//
TBool CCFCountOneShot::IsTrueL( const CCFContextObject& /*aContext*/ )
    {
    FUNC_LOG;

    TBool value( EFalse );
    if ( iCurrentCount == iCmpCount )
        {
        value = ETrue;

        if ( iCountType == CCFCount::ECountTypeRemain )
            {
            Finished();
            }
        }
    else if ( iCurrentCount > iCmpCount )
        {
        if ( iCmpCount == KInitialCount
            || iCountType != CCFCount::ECountTypeRemain )
            {
            Finished();
            }
        }

    return value;
    }

// ---------------------------------------------------------------------------
// CCFCountOneShot::Finished
// ---------------------------------------------------------------------------
//
void CCFCountOneShot::Finished()
    {
    FUNC_LOG;

    iFinished = ETrue;
    Deactivate();
    }
