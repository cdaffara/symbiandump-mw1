/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CFContextSourceSettingsManagerImpl class implementation.
*
*/


// INCLUDE FILES

#include "cfcontextsourcesettingsmanagerimpl.h"

#include <gmxmldocument.h>
#include <gmxmlelement.h>

#include "cfcontextsourcesettingimpl.h"
#include "cfcontextsourcesettingparameterimpl.h"
#include "cfcontextsourcesettingarrayimpl.h"
#include "cftrace.h"

// CONSTANTS

// Root node name
_LIT( KRootNode, "ContextSourceSettings" );

// MEMBER FUNCTIONS

EXPORT_C CCFContextSourceSettingsManagerImpl* CCFContextSourceSettingsManagerImpl::NewL(
    RFs& aFs )
    {
    FUNC_LOG;
    
    CCFContextSourceSettingsManagerImpl* self =
        CCFContextSourceSettingsManagerImpl::NewLC( aFs );
    CleanupStack::Pop( self );
    
    return self;
    }

EXPORT_C CCFContextSourceSettingsManagerImpl* CCFContextSourceSettingsManagerImpl::NewLC(
    RFs& aFs )
    {
    FUNC_LOG;
    
    CCFContextSourceSettingsManagerImpl* self =
        new( ELeave ) CCFContextSourceSettingsManagerImpl( aFs );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }
    
EXPORT_C CCFContextSourceSettingsManagerImpl::~CCFContextSourceSettingsManagerImpl()
    {
    FUNC_LOG;
    
    if( iWait )
        {
        if( iWait->IsStarted() )
            {
            iWait->AsyncStop();
            }
        delete iWait;
        }
    delete iParser;
    }
    
CCFContextSourceSettingsManagerImpl::CCFContextSourceSettingsManagerImpl(
    RFs& aFs ):
    iFs( aFs )
    {
    FUNC_LOG;
    }

void CCFContextSourceSettingsManagerImpl::ConstructL()
    {
    FUNC_LOG;
    
    iParser = CMDXMLParser::NewL( this );
    iWait = new( ELeave ) CActiveSchedulerWait;
    }
    
// METHODS

//----------------------------------------------------------------------------
// CCFContextSourceSettingsManagerImpl::ParseSettingsL
//----------------------------------------------------------------------------
//
void CCFContextSourceSettingsManagerImpl::ParseSettingsL(
    const TDesC& aSettingsFile,
    CCFContextSourceSettingArray& aSettings )
    {
    FUNC_LOG;
    
    INFO_1( "Started parsing context source setting file: %S",
        &aSettingsFile );
    TIMESTAMP( "Context source setting file parse start" );

    TInt err = iParser->ParseFile( iFs, aSettingsFile );
    if( err == KErrNone )
        {
        iSettings =
            static_cast<CCFContextSourceSettingArrayImpl*>( &aSettings );
        iWait->Start();
        }
    }

//----------------------------------------------------------------------------
// CCFContextSourceSettingsManagerImpl::ParseSettingsL
//----------------------------------------------------------------------------
//
void CCFContextSourceSettingsManagerImpl::ParseSettingsL( RFile& aSettingsFile,
    CCFContextSourceSettingArray& aSettings )
    {
    FUNC_LOG;
    
    INFO( "Started parsing context source setting file from open file handle" );
    TIMESTAMP( "Context source setting file parse start" );

    TInt err = iParser->ParseFile( aSettingsFile );
    if( err == KErrNone )
        {
        iSettings =
            static_cast<CCFContextSourceSettingArrayImpl*>( &aSettings );
        iWait->Start();
        }
    }

//----------------------------------------------------------------------------
// CCFContextSourceSettingsManagerImpl::ParseFileCompleteL
//----------------------------------------------------------------------------
//
void CCFContextSourceSettingsManagerImpl::ParseFileCompleteL()
    {
    FUNC_LOG;
    
    TIMESTAMP( "Context source setting file parse stop" );

    CMDXMLDocument* document = iParser->DetachXMLDoc();
    if( document )
        {
        CMDXMLElement* documentElement = document->DocumentElement();
        if( documentElement )
            {
            // Get first node
            CMDXMLNode* root = documentElement->FirstChild();
            if( root )
                {
                // Check that we have a correct root
                if( root->NodeName().CompareF( KRootNode ) == KErrNone )
                    {
                    // Start parsing setting blocks
                    CMDXMLNode* child = root->FirstChild();
                    while( child )
                        {
                        // Parse current child and check if we have more
                        // settings defined
                        if( child->NodeType() == CMDXMLNode::EElementNode )
                            {
                            CMDXMLElement* element =
                                static_cast<CMDXMLElement*>( child );
                            // Trap possible leaves so that we can still
                            // stop wait
                            TRAP_IGNORE( ParseSettingBlockL( *element ) );
                            }
                        child = child->NextSibling();
                        }
                    }
                }
            }
        }
    delete document;
    
    // We have completed, stop wait and continue
    iWait->AsyncStop();
    }

//----------------------------------------------------------------------------
// CCFContextSourceSettingsManagerImpl::ParseSettingBlockL
//----------------------------------------------------------------------------
//
void CCFContextSourceSettingsManagerImpl::ParseSettingBlockL(
    CMDXMLElement& aElement )
    {
    FUNC_LOG;

    TPtrC nodeName( aElement.NodeName() );
    INFO_1( "Found setting item: [%S]", &nodeName );
    
    // Check that we really have an element    
    CCFContextSourceSettingImpl* setting =
        CCFContextSourceSettingImpl::NewLC();

    // Set plug-in name
    setting->SetNameL( nodeName );
    
    // Set attributes
    ParseAttributesL( aElement, *setting );
        
    // Set parameters
    CMDXMLNode* child = aElement.FirstChild();
    while( child )
        {
        // Parse current child and check if we have more
        // settings defined
        if( child->NodeType() == CMDXMLNode::EElementNode )
            {
            CMDXMLElement* element =
                static_cast<CMDXMLElement*>( child );
            ParseParametersL( *element, *setting );
            }
        child = child->NextSibling();
        }
    
    // Save setting
    iSettings->AppendItemL( setting );
    CleanupStack::Pop( setting );
    }

//----------------------------------------------------------------------------
// CCFContextSourceSettingsManagerImpl::ParseAttributesL
//----------------------------------------------------------------------------
//
void CCFContextSourceSettingsManagerImpl::ParseAttributesL(
    CMDXMLElement& aElement,
    CCFContextSourceSettingImpl& aSetting )
    {
    FUNC_LOG;
    
    TInt numOfAttributes = aElement.NumAttributes();
    TPtrC attributeName( KNullDesC );
    TPtrC attributeValue( KNullDesC );
    for( TInt i = 0; i < numOfAttributes; i++ )
        {
        if( aElement.AttributeDetails(
            i, attributeName, attributeValue ) == KErrNone )
            {
            INFO_2( "Found attribute: '%S'='%S'",
                &attributeName,
                &attributeValue );
            
            CCFKeyValuePair* keyValue = CCFKeyValuePair::NewLC(
                attributeName,
                attributeValue );
            aSetting.AddAttributeL( keyValue );
            CleanupStack::Pop( keyValue );
            }
        }
    }

//----------------------------------------------------------------------------
// CCFContextSourceSettingsManagerImpl::ParseParametersL
//----------------------------------------------------------------------------
//
void CCFContextSourceSettingsManagerImpl::ParseParametersL(
    CMDXMLElement& aElement,
    CCFContextSourceSettingImpl& aSetting )
    {
    FUNC_LOG;
    
    TPtrC nodeName( aElement.NodeName() );
    INFO_1( "Found parameter item: [%S]", &nodeName );
    
    CCFContextSourceSettingParameterImpl* param =
        CCFContextSourceSettingParameterImpl::NewLC();
        
    // Set parameter block name
    param->SetNameL( nodeName );

    // Set attributes for the parameter block
    TInt numOfAttributes = aElement.NumAttributes();
    TPtrC attributeName( KNullDesC );
    TPtrC attributeValue( KNullDesC );
    for( TInt i = 0; i < numOfAttributes; i++ )
        {
        if( aElement.AttributeDetails(
            i, attributeName, attributeValue ) == KErrNone )
            {
            INFO_2( "Found parameter attribute: '%S'='%S'",
                &attributeName,
                &attributeValue );

            CCFKeyValuePair* keyValue = CCFKeyValuePair::NewLC(
                attributeName,
                attributeValue );
            param->AddAttributeL( keyValue );
            CleanupStack::Pop( keyValue );
            }
        }
    aSetting.AddParameterL( param );
    CleanupStack::Pop( param );
    }
