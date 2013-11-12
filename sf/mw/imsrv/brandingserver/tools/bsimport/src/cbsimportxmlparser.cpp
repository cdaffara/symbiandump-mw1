/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:   Parses XML content and stores data to storage
*
*/



#include <e32cons.h>
#include <gmxmlelement.h>
#include <gmxmltext.h>
#include <utf.h>
#include <AknUtils.h>
#include <bselementfactory.h>
#include <cbsbitmap.h>

#include "cbsimportxmlparser.h"
#include "cbsimportstoragemanager.h"
#include "cbsimportstorage.h"
#include "cbsimportconstants.h"
#include "importlogwriter.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CBSImportXMLParser::NewL
// ---------------------------------------------------------------------------
//
CBSImportXMLParser* CBSImportXMLParser::NewL( CBSImportStorageManager* aStorage )
    {
    CBSImportXMLParser* self = NewLC( aStorage );
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// CBSImportXMLParser::NewLC
// ---------------------------------------------------------------------------
//
CBSImportXMLParser* CBSImportXMLParser::NewLC( CBSImportStorageManager* aStorage )
    {
    CBSImportXMLParser* self = new (ELeave) CBSImportXMLParser( aStorage );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CBSImportXMLParser::ConstructL
// ---------------------------------------------------------------------------
//
void CBSImportXMLParser::ConstructL()
    {
    }
    
// ---------------------------------------------------------------------------
// CBSImportXMLParser::~CBSImportXMLParser
// ---------------------------------------------------------------------------
//
CBSImportXMLParser::~CBSImportXMLParser()
    {
    }

// ---------------------------------------------------------------------------
// CBSImportXMLParser::CBSImportXMLParser
// ---------------------------------------------------------------------------
//
CBSImportXMLParser::CBSImportXMLParser( CBSImportStorageManager* aStorage ) :
    iStorage( aStorage )
    {
    }
    
// ---------------------------------------------------------------------------
// CBSImportXMLParser::ParseDocumentL
// ---------------------------------------------------------------------------
//
void CBSImportXMLParser::ParseDocumentL( CMDXMLDocument& aDocument )
    {
    if( !iStorage )
        {
        // No storage --> can't continue
        User::Leave( KErrNotReady );
        }

    CMDXMLElement* element = aDocument.DocumentElement();
    ParseDocumentBaseL( element );
    }

// ---------------------------------------------------------------------------
// CBSImportXMLParser::ParseDocumentBaseL
// ---------------------------------------------------------------------------
//
void CBSImportXMLParser::ParseDocumentBaseL( CMDXMLNode* aElement )
    {
    if( aElement->HasChildNodes() )
        {
        // Verify that <branding> element is the first one
        CMDXMLNode* brandingNode = aElement->FirstChild();
        if( !brandingNode->HasChildNodes() )
            {
            IMPORT_DP_TXT("ERROR: empty <branding> tag found" );
            User::Leave( KErrCorrupt ); 
            }

        // Parse through all <brand> elements
        CMDXMLNode* brandNode = brandingNode->FirstChild();
        while( brandNode )
            {
            if( 0 == brandNode->NodeName().CompareC( KBrandTag ) )
                {
                if( !brandNode->HasChildNodes() )
                    {
                    IMPORT_DP_TXT("ERROR: empty <brand> tag found" );
                    User::Leave( KErrCorrupt ); 
                    }

                // Create a new storage for each <brand> element
                iStorage->CreateStorageL();

                // Scan through all elements inside <brand> tags
                // and store them to storage
                CMDXMLNode* kidNode = brandNode->FirstChild();
                while( kidNode )
                    {
                    TRAPD( err, AnalyzeNodeL( kidNode ) );
                    if( err )
                        {
                        IMPORT_DP( D_IMPORT_LIT("ERROR: Error parsing content of <brand> (%d)"), err );
                        User::Leave( err ); 
                        }
                    kidNode = kidNode->NextSibling();
                    }
                }
            
            // move to next brand package
            brandNode = brandNode->NextSibling();
            }
        }
    else
        {
        // no elements in file --> corrupt
        IMPORT_DP_TXT("ERROR: Empty XML content" );
        User::Leave( KErrCorrupt ); 
        }
    }

// ---------------------------------------------------------------------------
// CBSImportXMLParser::AnalyzeNodeL
// ---------------------------------------------------------------------------
//
void CBSImportXMLParser::AnalyzeNodeL( CMDXMLNode* aNode )
    {
    switch( SolveTagTypeL( aNode ) )
        {
        case EBrandID:
            {
            if( aNode->HasChildNodes() )
                {
                HBufC* id = NodeTextLC( aNode );
                iStorage->Storage()->SetStorageIdL( *id );
                CleanupStack::PopAndDestroy();
                }
            else
                {
                IMPORT_DP_TXT("WARNING: BrandID has no value" );
                User::Leave( KErrNotFound ); 
                }
            break;
            }
        case EBrandAppID:
            {
            if( aNode->HasChildNodes() )
                {
                HBufC* id = NodeTextLC( aNode );
                iStorage->Storage()->SetApplicationIdL( *id );
                CleanupStack::PopAndDestroy();
                }
            else
                {
                IMPORT_DP_TXT("WARNING: ApplicationID has no value" );
                User::Leave( KErrNotFound ); 
                }
            break;
            }
        case EBrandLanguage:
            {
            if( aNode->HasChildNodes() )
                {
                TInt lang = ELangTest;
                TRAPD( err, lang = NodeIntL( aNode ) );
                if( err )
                    {
                    // not a number. Check if language is "SC"
                    HBufC* langBuf = NodeTextLC( aNode );
                    TPtr langText( langBuf->Des() );
                    langText.UpperCase();
                    if( 0 != langText.CompareC( KDefaultLanguage ) ) 
                        {
                        IMPORT_DP_TXT("ERROR: Language field has invalid value (has to be a number or 'SC')" );
                        User::Leave( KErrArgument );
                        }
                    CleanupStack::PopAndDestroy( langBuf );
                    }
                iStorage->Storage()->SetLanguageL( (TLanguage)lang );
                }
            else
                {
                IMPORT_DP_TXT("WARNING: Language field has no value" );
                User::Leave( KErrNotFound ); 
                }
            break;
            }
        case EBrandVersion:
            {
            if( aNode->HasChildNodes() )
                {
                iStorage->Storage()->SetVersion( NodeIntL( aNode ) );
                }
            else
                {
                IMPORT_DP_TXT("WARNING: Version field has no value" );
                User::Leave( KErrNotFound ); 
                }
            break;
            }
        case EBrandElement:
            {
            if( aNode->HasChildNodes() )
                {
                iStorage->Storage()->AppendElement( ParseBrandElementL( aNode ) );
                }
            else
                {
                IMPORT_DP_TXT("WARNING: Empty <element> tag found" );
                User::Leave( KErrNotFound ); 
                }
            break;
            }
        case EBrandUnknown: // flowthrough
        default:
            {
            TPtrC ptr( aNode->NodeName() );
            IMPORT_DP( D_IMPORT_LIT("WARNING: Unknown XML tag encountered: <%S>"), &ptr );
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// CBSImportXMLParser::SolveTagTypeL
// ---------------------------------------------------------------------------
//
CBSImportXMLParser::TXMLBaseTagTypes CBSImportXMLParser::SolveTagTypeL( CMDXMLNode* aNode )
    {
    TPtrC node( aNode->NodeName() );
    if( 0 == node.CompareF( KBrandElementTag ) )
        return EBrandElement;
    
    if( 0 == node.CompareF( KBrandIDTag ) )
        return EBrandID;

    if( 0 == node.CompareF( KBrandLanguageTag ) )
        return EBrandLanguage;

    if( 0 == node.CompareF( KBrandVersionTag ) )
        return EBrandVersion;
    
    if( 0 == node.CompareF( KBrandAppIdTag ) )
        return EBrandAppID;
    
    return EBrandUnknown;
    }

// ---------------------------------------------------------------------------
// CBSImportXMLParser::ParseBrandElementL
// ---------------------------------------------------------------------------
//
MBSElement* CBSImportXMLParser::ParseBrandElementL( CMDXMLNode* aNode )
    {
    if( !aNode->HasChildNodes() || 
        0 != aNode->NodeName().CompareC( KBrandElementTag ))
        {
        // invalid element block
        User::Leave( KErrCorrupt );
        }
    
    // Check <element> tag attributes
    CMDXMLElement* elNode = (CMDXMLElement*)(aNode);
    TInt elType( KErrNotFound );
    HBufC8* elID = NULL;
    TInt attrNum = elNode->NumAttributes();
    for( TInt i = 0; i < attrNum; i++ )
        {
        TPtrC name;
        TPtrC value;
        elNode->AttributeDetails( i, name, value );
        
        // <element ... type="xxx" ...>
        if( 0 == name.CompareC( KElementTypeName ) ) 
            {
            elType = SolveElementType( value );
            }
        // <element ... id="xxx" ...>
        else if( 0 == name.CompareC( KElementTypeID ) && !elID )
            {
            elID = HBufC8::NewL( value.Length() );
            CleanupStack::PushL( elID );
            TPtr8 ptrID( elID->Des() );
            CnvUtfConverter::ConvertFromUnicodeToUtf8( ptrID, value );
            }
        }
    
    // Verify type
    if( elType == KErrNotFound )
        {
        // Element type missing!
        IMPORT_DP_TXT("ERROR: Element's 'type' attribute is missing!" );
        User::Leave( KErrCorrupt );
        }
    
    // Create empty ID if element didn't have one
    if( !elID )
        {
        elID = KNullDesC8().AllocL();
        CleanupStack::PushL( elID );
        }
    
    // Parse element according to type
    MBSElement* elem = NULL;
    switch( elType )
        {
        case EBSInt:
            {
            TInt intValue;
            TRAPD( err, DoParseIntegerL( elNode, intValue ) )
            if( !err )
                {
                TPtr8 ptr( elID->Des() );
                // Codescanner warning: neglected to put variable on cleanup stack (id:35)
                // This method cannot leave after this line
                elem = BSElementFactory::CreateBSElementL( ptr, EBSInt, intValue ); // CSI: 35 # See above
                }
            else
                {
                IMPORT_DP_TXT("ERROR: Invalid value in integer element!" );
                User::Leave( err );
                }
            break;
            }
        case EBSFile:
            {
            HBufC* text = NULL;
            TRAPD( err, DoParseFileL( elNode, text ) )
            if( !err )
                {
                TPtr8 ptr( elID->Des() );
                CleanupStack::PushL( text );
                elem = BSElementFactory::CreateBSElementL( ptr, EBSFile, *text );
                CleanupStack::PopAndDestroy( text );
                }
            else
                {
                IMPORT_DP_TXT("ERROR: Invalid value in file element!" );
                User::Leave( err );
                }

            break;
            }
        case EBSBuffer:
        case EBSText:
            {
            HBufC* text = NULL;
            TRAPD( err, DoParseTextL( elNode, text ) )
            if( !err )
                {
                TPtr8 ptr( elID->Des() );
                CleanupStack::PushL( text );
                
                if( EBSText == elType ) // 16-bit descriptor
                    {
                    // Element takes ownership of text
                    elem = BSElementFactory::CreateBSElementL( ptr, EBSText, *text );
                    }
                else // EBuffer: 8-bit descriptor
                    {
                    // convert to 8-bit
                    HBufC8* conv = HBufC8::NewLC( text->Length() );
                    TPtr8 ptrconv( conv->Des() );
                    CnvUtfConverter::ConvertFromUnicodeToUtf8( ptrconv, *text );
                    elem = BSElementFactory::CreateBSElementL( ptr, EBSBuffer, *conv );
                    CleanupStack::PopAndDestroy( conv );
                    }
                CleanupStack::PopAndDestroy( text );
                }
            else
                {
                // Error parsing text element
                IMPORT_DP_TXT("ERROR: Invalid value in text element!" );
                User::Leave( err );
                }
            break;
            }
        case EBSList:
            {
            RBSObjOwningPtrArray<MBSElement> elementArray;
            CleanupClosePushL( elementArray );
            TRAPD( err, DoParseListL( aNode, elementArray ) );
            if( !err )
                {
                // Create list element
                TPtr8 ptr( elID->Des() );
                // Codescanner warning: neglected to put variable on cleanup stack (id:35)
                // This method cannot leave after this line
                elem = BSElementFactory::CreateBSElementL( ptr, EBSList, elementArray ); // CSI: 35 # See above
                CleanupStack::Pop(); // elementArray
                }
            else
                {
                IMPORT_DP_TXT("ERROR: List element parse failed!" );
                User::Leave( err );
                }
            break;
            }
        case EBSBitmap:
            {
            CBSBitmap* bitmap = NULL;
            TRAPD( err, DoParseBitmapL( elNode, bitmap ) )
            if( !err )
                {
                CleanupStack::PushL( bitmap );
                // Create list element
                TPtr8 ptr( elID->Des() );
                elem = BSElementFactory::CreateBSElementL( ptr, EBSBitmap, bitmap );
                CleanupStack::Pop( bitmap );
                }
            break;
            }
        default:
            {
            // Invalid element type
            break;
            }
        }

    // return parsed element
    CleanupStack::PopAndDestroy( elID );
    return elem;
    }

// ---------------------------------------------------------------------------
// CBSImportXMLParser::SolveElementType
// ---------------------------------------------------------------------------
//
TInt CBSImportXMLParser::SolveElementType( const TDesC& aType )
    {
    if( 0 == aType.CompareC( KElementTypeList ) )
        return EBSList;

    if( 0 == aType.CompareC( KElementTypeBitmap ) )
        return EBSBitmap;

    if( 0 == aType.CompareC( KElementTypeBuffer ) )
        return EBSBuffer;

    if( 0 == aType.CompareC( KElementTypeInt ) )
        return EBSInt;
    
    if( 0 == aType.CompareC( KElementTypeText ) )
        return EBSText;
    
    if( 0 == aType.CompareC( KElementTypeFile ) )
        return EBSFile;
    
    return KErrNotFound;
    }

// ---------------------------------------------------------------------------
// CBSImportXMLParser::DoParseIntegerL
// ---------------------------------------------------------------------------
//
void CBSImportXMLParser::DoParseIntegerL( CMDXMLNode* aNode, TInt& aValue )
    {
    // Integer element has only one child element. e.g:
    // <element_value>
    //  12345
    // </element_value>
    //
    CMDXMLNode* kidNode = aNode->FirstChild();
    while( kidNode )
        {
        if( 0 == KElementValueTag().CompareC( kidNode->NodeName() ) )
            {
            aValue = NodeIntL( kidNode );
            return;
            }
        kidNode = kidNode->NextSibling();
        }
    User::Leave( KErrNotFound );
    }

// ---------------------------------------------------------------------------
// CBSImportXMLParser::DoParseTextL
// ---------------------------------------------------------------------------
//
void CBSImportXMLParser::DoParseTextL( CMDXMLNode* aNode, HBufC*& aText )
    {
    // Text element has only one valuable child element. e.g:
    // <element_value>
    //  some important text
    // </element_value>
    //
    CMDXMLNode* kidNode = SearchChild( aNode, KElementValueTag );
    if( kidNode )
        {
        aText = NodeTextLC( kidNode );
        CleanupStack::Pop( aText );
        return;
        }
    
    // didn't find a valid value
    IMPORT_DP_TXT( "ERROR: Text element is missing it's value" );
    User::Leave( KErrNotFound );
    }

// ---------------------------------------------------------------------------
// CBSImportXMLParser::DoParseFileL
// ---------------------------------------------------------------------------
//
void CBSImportXMLParser::DoParseFileL( CMDXMLNode* aNode, HBufC*& aText )
    {
    // File element has only one valuable text value element. e.g:
    // <element_value>
    //   <file_name> c:\data\file.txt </file_name>
    // </element_value>
    //
    CMDXMLNode* kidNode = SearchChild( aNode, KElementValueTag );
    if( kidNode )
        {
        // check <file_name> tag
        CMDXMLNode* subKidNode = SearchChild( kidNode, KFileFileNameTag );
        if( subKidNode )
            {
            // Get filename from tag
            aText = NodeTextLC( subKidNode );
            CleanupStack::Pop( aText );
            return;
            }
        }

    // didn't find a valid value
    IMPORT_DP_TXT( "ERROR: File element is missing it's value" );
    User::Leave( KErrNotFound );
    }

// ---------------------------------------------------------------------------
// CBSImportXMLParser::DoParseListL
// ---------------------------------------------------------------------------
//
void CBSImportXMLParser::DoParseListL( 
    CMDXMLNode* aNode, 
    RBSObjOwningPtrArray<MBSElement>& aElementArray )
    {
    // Scan trough elements inside this list element
    // <element_value>
    //  <element ...>
    //   ...
    //  </element>
    //  <element ...>
    //   ...
    //  </element>
    // </element_value>
    //
    CMDXMLNode* kidNode = SearchChild( aNode, KElementValueTag );
    if( kidNode )
        {
        CMDXMLNode* subKidNode = kidNode->FirstChild();
        while( subKidNode )
            {
            // verify that this really is <element> node.
            if( 0 == subKidNode->NodeName().CompareC( KBrandElementTag ) )
                {
                MBSElement* el = ParseBrandElementL( subKidNode );
                if( el )
                    {
                    aElementArray.Append( el );
                    }
                }
            subKidNode = subKidNode->NextSibling();
            }
        }
    else
        {
        // No elements found inside list!
        IMPORT_DP_TXT( "ERROR: List element is missing it's value" );
        User::Leave( KErrNotFound );
        }
    }

// ---------------------------------------------------------------------------
// CBSImportXMLParser::DoParseBitmapL
// ---------------------------------------------------------------------------
//
void CBSImportXMLParser::DoParseBitmapL( 
    CMDXMLNode* aNode, 
    CBSBitmap*& aBitmap )
    {
/// XSP MOD: added support for <bitmap_file_id>
    // Bitmap element has 5 child elements. e.g:
    // <element_value>
    //  <bitmap_file_id>
    //  <bitmap_id> 65 </bitmap_id>
    //  <mask_id> 66 </mask_id>
    //  <skin_id_major> 190 </skin_id_major>
    //  <skin_id_minor> 191 </skin_id_minor>
    // </element_value>
    //
    TInt bmpId = KErrNotFound;
    TInt bmpMaskId = KErrNotFound;
    TInt skinId = KErrNotFound;
    TInt skinMaskId = KErrNotFound;
    HBufC8* fileId = NULL;
    
    CMDXMLNode* kidNode = SearchChild( aNode, KElementValueTag );
    if( kidNode )
        {
        CMDXMLNode* subKidNode = kidNode->FirstChild();
        TInt readCheck = 0;
        while( subKidNode )
            {
            TPtrC ptr = subKidNode->NodeName();
            if( 0 == KBitmapIdTag().CompareC( subKidNode->NodeName() ) )
                {
                bmpId = NodeIntL( subKidNode );
                readCheck |= 0x1;
                }
            else if( 0 == KBitmapMaskIdTag().CompareC( subKidNode->NodeName() ) )
                {
                bmpMaskId = NodeIntL( subKidNode );
                readCheck |= 0x2;
                }
            else if( 0 == KBitmapSkinIdTag().CompareC( subKidNode->NodeName() ) )
                {
                skinId = NodeIntL( subKidNode );
                readCheck |= 0x4;
                }
            else if( 0 == KBitmapSkinMaskIdTag().CompareC( subKidNode->NodeName() ) )
                {
                skinMaskId = NodeIntL( subKidNode );
                readCheck |= 0x8;
                }
/// XSP MOD: added support for madatory KBitmapFileIdTag field                
            else if( 0 == KBitmapFileIdTag().CompareC( subKidNode->NodeName() ) )
                {
                // Ignore following file id elements if 
                // bitmap element has more than one of them.
                if ( !fileId )
                    {
                    HBufC* tmp = NodeTextLC( subKidNode );
                	fileId = CnvUtfConverter::ConvertFromUnicodeToUtf8L( *tmp );
                	CleanupStack::PopAndDestroy( tmp );
                	tmp = NULL;
                    CleanupStack::PushL( fileId );
                    readCheck |= 0x10;
                    }
                }

            subKidNode = subKidNode->NextSibling();
            }
        
        if( readCheck != 0x1F )/// XSP MOD:check that all 5 values were found
            {
            // Some bitmap elements are missing!
            IMPORT_DP_TXT( "ERROR: Bitmap element is missing some mandatory values" );
            User::Leave( KErrNotFound );
            }
        
        // create bitmap
        TPtrC8 pFileId( KNullDesC8 );
        if ( fileId )
            {
            pFileId.Set( *fileId );
            }
        aBitmap = CBSBitmap::NewL( bmpId, bmpMaskId, skinId, skinMaskId, pFileId );
        if ( fileId )
            {
            CleanupStack::PopAndDestroy( fileId );
            fileId = NULL;
            }
        }
    else
        {
        IMPORT_DP_TXT( "ERROR: Bitmap element is missing it's value" );
        User::Leave( KErrNotFound );
        }
    }

// ---------------------------------------------------------------------------
// CBSImportXMLParser::NodeTextLC
// ---------------------------------------------------------------------------
//
HBufC* CBSImportXMLParser::NodeTextLC( CMDXMLNode* aNode )
    {
    CMDXMLNode* kidNode = aNode->FirstChild();
    if( !kidNode )
        {
        // empty content
        return KNullDesC().AllocLC();
        }
        
    if( kidNode->HasChildNodes() )
        {
        // not a valid text element!
        User::Leave( KErrCorrupt );        
        }

    HBufC* text = static_cast<CMDXMLText*>(kidNode)->Data().AllocL();
    CleanupStack::PushL( text );
    
    // Remove extra characters from text
    TPtr ptr( text->Des() );
    AknTextUtils::StripCharacters( ptr, KCommonControlCharacters );
    ptr.TrimAll();
    
    return text;
    }

// ---------------------------------------------------------------------------
// CBSImportXMLParser::NodeIntL
// ---------------------------------------------------------------------------
//
TInt CBSImportXMLParser::NodeIntL( CMDXMLNode* aNode )
    {
    CMDXMLNode* kidNode = aNode->FirstChild();
    if( !kidNode || kidNode->HasChildNodes() )
        {
        // not a valid text element!
        User::Leave( KErrCorrupt );        
        }

    TLex lex( static_cast<CMDXMLText*>(kidNode)->Data() );
    TInt value;
    User::LeaveIfError( lex.Val( value ) );
    return value;
    }

// ---------------------------------------------------------------------------
// CBSImportXMLParser::FindTag
// ---------------------------------------------------------------------------
//
CMDXMLNode* CBSImportXMLParser::SearchChild( CMDXMLNode* aNode, const TDesC& aChildName )
    {
    if( aNode->HasChildNodes() )
        {
        CMDXMLNode* kidNode = aNode->FirstChild();
        while( kidNode )
            {
            if( 0 == aChildName.CompareC( kidNode->NodeName() ) )
                {
                // found it
                return kidNode;
                }
            kidNode = kidNode->NextSibling();
            }
        }
    // not found
    return NULL;
    }

// End of file
