/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
 *
*/


#ifndef C_WIDGETSCANNERPARSER_H
#define C_WIDGETSCANNERPARSER_H

//  INCLUDES
#include <e32base.h>
#include <xml/dom/xmlengdomimplementation.h>
#include <badesca.h>
#include <f32file.h>
#include "cawidgetdescription.h"
#include "castorage_global.h"

// CONSTANTS

// FORWARD DECLARATIONS

class CDir;
CA_STORAGE_TEST_CLASS( T_CaWidgetScanner )
class TXmlEngElement;
/**
 *  Widget Parser
 *
 *  @since Series Series60 5.x
 */
class CCaWidgetScannerParser : public CBase
    {

public:
    // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static CCaWidgetScannerParser* NewL( RFs& aFs );
    static CCaWidgetScannerParser* NewLC( RFs& aFs );

    /**
     * Destructor.
     */
    virtual ~CCaWidgetScannerParser();

    /**
     * WidgetsL
     * @return array containing widgets description
     */
    RWidgetArray& WidgetsScanL( const RWidgetArray& aWidgets  );
    
private:

    /**
     * C++ default constructor.
     */
    CCaWidgetScannerParser( RFs& aFs );

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL( );


    /**
     * Looks for manifests on a drive
     * @param aDrive drive to use
     */
    void ScanOnDriveL( TChar& aDrive );

    /**
     * Parses all manifest files in a directory
     * @param aDirectoryName directory name
     * @param aDrive drive to use
     */
    void ParseDirectoryL( const TDesC& aDirectoryName, TChar& aDrive );

    /**
    * Parse manifest file
    * @param aFilePath full file path to manifest file
    * @param aPackageUid descriptor containing package uid
    * @param aDrive drive to use
    */
    void ParseManifestFileL( const TDesC& aFilePath,
        const TDesC& aPackageUid, TChar& aDrive );

    /**
    * Parse a widget, store parsed object in an internal array
    * @param aFilePath full file path to manifest file
    * @param aElement widget element
    * @param aPackageUid descriptor containing package uid
    * @param aDrive drive to use
    */
    void ParseWidgetL( const TDesC& aFilePath,
            TXmlEngElement& aElement, const TDesC& aPackageUid,
            TChar& aDrive );

    /**
     * Gets all directories from import folder
     * @param aDriveLetter drive letter
     * @return directories
     */
    CDir* GetDirectoriesLC( TChar& aDrive );

    /**
     * Returns import path
     * @param aDriveLetter drive letter
     * @return import path
     */
    HBufC* FullPathLC( TChar& aDrive ) const;

    /**
     * Returns manifest directory path
     * @param aDriveLetter drive letter
     * @param aDirectoryName directory name
     * @param aDrive drive to use
     * @return manifest directory path
     */
    HBufC* GetManifestDirectoryPathLC( const TDesC& aDirectoryName, TChar& aDrive ) const;

    /**
     * Sets widgets URI
     * @param aElement xml element
     * @param aWidget widget to set value
     */
    void SetUriL( TXmlEngElement& aElement, CCaWidgetDescription* aWidget );
    
    /**
     * Sets translation filename
     * @param aElement xml element
     * @param aWidget widget to set value
     */
    void SetTranslationFileNameL( TXmlEngElement& aElement, CCaWidgetDescription* aWidget );

    /**
     * Sets widgets library and path created from library
     * @param aElement xml element
     * @param aPackageUid package uid
     * @param aWidget widget to set value
     */
    void SetLibraryL( TXmlEngElement& aElement, const TDesC& aPackageUid,
            CCaWidgetDescription* aWidget, TChar& aDrive );

    /**
     * Sets widgets title
     * @param aElement xml element
     * @param aWidget widget to set value
     */
    void SetTitleL( TXmlEngElement& aElement, 
            CCaWidgetDescription* aWidget );

    /**
     * Sets widgets description
     * @param aElement xml element
     * @param aWidget widget to set value
     */
    void SetDescriptionL( TXmlEngElement& aElement,
            CCaWidgetDescription* aWidget );

    /**
     * Sets widgets visibility
     * @param aElement xml element
     * @param aWidget widget to set value
     */
    void SetVisibilityL( TXmlEngElement& aElement,
            CCaWidgetDescription* aWidget );

    /**
     * Sets icons URI
     * @param aElement xml element
     * @param aPackageUid package uid
     * @param aWidget widget to set value
     */
    void SetIconUriL( TXmlEngElement& aElement, const TDesC& aPackageUid,
            CCaWidgetDescription* aWidget, TChar& aDrive );

    /**
     * Sets widgets mmc id
     * @param aWidget widget to set value
     */
    void SetMmcIdL( CCaWidgetDescription* aWidget, TChar& aDrive );
    
    void ParseUriL( TXmlEngElement& aElement,
            CCaWidgetDescription* aWidgetDescriptor );
    void ParseTitleL( TXmlEngElement& aElement,
            CCaWidgetDescription* aWidgetDescriptor );
    void ParseIconL( TXmlEngElement& aElement,  const TDesC& aPackageUid,
            CCaWidgetDescription* aWidgetDescriptor, TChar& aDrive );
    void ParseDescriptionL( TXmlEngElement& aElement,
            CCaWidgetDescription* aWidgetDescriptor );
    void ParseHiddenL( TXmlEngElement& aElement,
            CCaWidgetDescription* aWidgetDescriptor );
    void ParseServiceXmlL(TXmlEngElement & aElement,
            CCaWidgetDescription* aWidgetDescriptor );
    void SetModificationTimeL( const TDesC& aFilePath,
            CCaWidgetDescription* aWidgetDescriptor );
    
    void ParsePreviewImageNameL( TXmlEngElement& aElement, 
        const TDesC& aPackageUid,
        CCaWidgetDescription* aWidgetDescriptor,
        TChar& aDrive );
    
    void ParseTranslationFileNameL( TXmlEngElement& aElement,
            CCaWidgetDescription* aWidgetDescriptor );

    /**
     * @param aElement xml document element.
     * @return created descriptor with element's text value. Ownership
     * of the descriptor is passed to the caller.
     */
    HBufC *CCaWidgetScannerParser::GetElementTextLC( 
        const TXmlEngElement& aElement ) const;
    
    /**
     * Parses graphics name value and if the name has a file extension
     * adds widget root directory path to it.
     * @param aElement xml document element.
     * @return created descriptor with graphics name value. Ownership
     * of the descriptor is passed to the caller.
     */
    HBufC* GetThemableGraphicsNameLC(
            const TXmlEngElement & aElement,
            const TDesC & aPackageUid,
            TChar& aDrive ) const;

private:
    // Data  

    /*
     * File server session.Not Own
     */
    RFs iFs;

    /*
     * Path to the private directory (wihout drive).own
     */
    RBuf iImportPath;

    /*
     * Widgets. Own.
     */
    RWidgetArray iWidgets;
    
    /*
     * Widgets. Not own.
     */
    RWidgetArray iFetchedWidgets; 
    /*
     * Own.
     */
    RXmlEngDOMImplementation iDomImpl;
    
    
    CA_STORAGE_TEST_FRIEND_CLASS( T_CaWidgetScanner )
    
    };

#endif      // C_WIDGETSCANNERPARSER_H
// End of File
