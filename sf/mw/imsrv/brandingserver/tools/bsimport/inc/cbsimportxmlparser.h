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


#ifndef CBSIMPORTXMLPARSER_H
#define CBSIMPORTXMLPARSER_H

// INCLUDE FILES
#include <e32base.h>
#include <f32file.h>
#include <gmxmldocument.h>
#include <rbsobjowningptrarray.h>
#include <mbselement.h>

class CMDXMLNode;
class CBSImportStorageManager;

// CLASS DEFINITION
class CBSImportXMLParser : public CBase
    {
private: // Definitions

    struct TImportBitmap
        {
        TInt iBitmapId;
        TInt iBitmapMaskId;
        TInt iSkinId;
        TInt iSkinMaskId;
        };

    enum TXMLBaseTagTypes
        {
        EBrandUnknown,
        EBrandAppID,
        EBrandID,
        EBrandLanguage,
        EBrandVersion,
        EBrandElement
        };
        
public:

    static CBSImportXMLParser* NewL( CBSImportStorageManager* aStorage );
    
    static CBSImportXMLParser* NewLC( CBSImportStorageManager* aStorage );

    ~CBSImportXMLParser();

    /**
     * Parse XML document and store data to storage
     *
     * @since S60 3.2
     * @param aDocument XML document to parse
     * @return none
     */
    void ParseDocumentL( CMDXMLDocument& aDocument );

private:

    CBSImportXMLParser( CBSImportStorageManager* aStorage );
    
    void ConstructL();
    
    /**
     * Parses full the base element <branding> and it's child elements.
     *
     * @since S60 v3.2
     * @param aElement to parse
     */
    void ParseDocumentBaseL( CMDXMLNode* aElement );
    
    /**
     * Analyzes a child node of <branding> and stores the value to storage.
     * Ignores undetected XML nodes.
     *
     * @since S60 v3.2
     * @param aElement to parse
     */
    void AnalyzeNodeL( CMDXMLNode* aNode );
    
    /**
     * Converts a given node to TXMLBaseTagTypes. This can handle all
     * child nodes of <branding>
     *
     * @since S60 v3.2
     * @param aNode node to check
     * @return corresponding TXMLBaseTagTypes value (or EBrandUnknown).
     */
    TXMLBaseTagTypes SolveTagTypeL( CMDXMLNode* aNode );

    /**
     * Parses full <element> node. Returns a newly constructed MBSElement
     * from the content of teh node. 
     * Ownership is transferred!
     *
     * @since S60 v3.2
     * @param aNode <element> node
     * @return Concrete element constructed from node. 
     */
    MBSElement* ParseBrandElementL( CMDXMLNode* aNode );
    
    /**
     * Converts elements type attribute value to TBSElementType 
     * (see MBSElement).
     *
     * @since S60 v3.2
     * @param aType value of type attribute
     * @return corresponding TBrandingElementType value or KErrNotFound.
     */
    TInt SolveElementType( const TDesC& aType );

    /**
     * Search a child node with name inside another node. Returns the first match
     *
     * @since S60 v3.2
     * @param aNode parent node to search 
     * @param aTagName Child nodes name
     * @return found node or NULL if not found;
     */
    CMDXMLNode* SearchChild( CMDXMLNode* aNode, const TDesC& aChildName );

    /**
     * Parse value from <element_value> tag. Leaves if parse fails.
     * Ownership is transferred to caller (NodeTextLC)
     *
     * @since S60 3.2
     * @param aNode XML node to parse. should be <element_value>.
     * @return value from <element_value>
     */
    HBufC* NodeTextLC( CMDXMLNode* elNode );
    TInt   NodeIntL  ( CMDXMLNode* elNode );
    
    /**
     * Parse <element> tag for given value type. Leaves if parsing fails.
     *
     * @since S60 3.2
     * @param aNode XML node to parse
     * @param aValue Parsed value
     * @return none
     */
    void DoParseIntegerL( CMDXMLNode* aNode, TInt& aValue );
    void DoParseTextL   ( CMDXMLNode* aNode, HBufC*& aText );
    void DoParseFileL   ( CMDXMLNode* aNode, HBufC*& aText );
    void DoParseListL   ( CMDXMLNode* aNode, RBSObjOwningPtrArray<MBSElement>& aElementArray );
    void DoParseBitmapL ( CMDXMLNode* aNode, CBSBitmap*& aBitmap );
    
private: // Data
    
    // Not owned. Storage where to put parsed elements
    CBSImportStorageManager* iStorage;
    };

#endif //CBSIMPORTXMLPARSER_H
