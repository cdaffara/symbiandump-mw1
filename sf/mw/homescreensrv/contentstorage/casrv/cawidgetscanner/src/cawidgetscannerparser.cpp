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

// INCLUDE FILES

#include <driveinfo.h>
#include <xml/dom/xmlengnodelist.h>
#include <xml/dom/xmlengdomparser.h>
#include <xml/dom/xmlengdocument.h>
#include <xml/dom/xmlengelement.h>
#include <utf.h>
#include <e32base.h>

#include "cadef.h"
#include "cawidgetscannerparser.h"
#include "widgetscannerutils.h"
#include "cawidgetscannerdef.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CCaWidgetScannerParser::CCaWidgetScannerParser( RFs& aFs ) :
    iFs( aFs )
    {
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::ConstructL()
    {
    iImportPath.CreateL( KMaxPath );
    iImportPath.Append( KImportDir );
    iDomImpl.OpenL();
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCaWidgetScannerParser* CCaWidgetScannerParser::NewL( RFs& aFs )
    {
    CCaWidgetScannerParser* self = NewLC( aFs );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCaWidgetScannerParser* CCaWidgetScannerParser::NewLC( RFs& aFs )
    {
    CCaWidgetScannerParser* self = new ( ELeave ) CCaWidgetScannerParser( aFs );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCaWidgetScannerParser::~CCaWidgetScannerParser()
    {
    iImportPath.Close();
    iWidgets.ResetAndDestroy();
    iDomImpl.Close();
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
RWidgetArray& CCaWidgetScannerParser::WidgetsScanL(
        const RWidgetArray& aWidgets )
    {
    TDriveList driveList;
    TChar currentDriveLetter;
    User::LeaveIfError( iFs.DriveList( driveList ) );
    iWidgets.ResetAndDestroy();

    iFetchedWidgets = aWidgets;

    for ( TInt driveNumber = EDriveZ; driveNumber >= EDriveA; driveNumber-- )
        {
        if ( driveList[driveNumber] )
            {
            User::LeaveIfError( iFs.DriveToChar( driveNumber,
                    currentDriveLetter ) );
            ScanOnDriveL( currentDriveLetter );
            }
        }
    return iWidgets;
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::ScanOnDriveL( TChar& aDrive )
    {
    CDir* directories = GetDirectoriesLC( aDrive );
    if ( directories )
        {
        for ( TInt i( 0 ); i < directories->Count(); i++ )
            {
            if ( ( *directories )[i].IsDir() )
                {
                ParseDirectoryL( ( *directories )[i].iName, aDrive );
                }
            }
        }
    CleanupStack::PopAndDestroy( directories );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::ParseDirectoryL( const TDesC& aDirectoryName,
        TChar& aDrive )
    {
    HBufC* manifestDirectoryPath = GetManifestDirectoryPathLC( aDirectoryName,
            aDrive );

    CDir* fileList = NULL;

    User::LeaveIfError( iFs.GetDir( *manifestDirectoryPath,
            KEntryAttMatchExclude | KEntryAttDir, ESortByDate, fileList ) );
    CleanupStack::PushL( fileList );

    for ( TInt i = 0; i < fileList->Count(); i++ )
        {
        if ( ( *fileList )[i].iName.Find( KManifest ) != KErrNotFound )
            {
            RBuf manifestFilePathName;
            CleanupClosePushL( manifestFilePathName );
            manifestFilePathName.CreateL( manifestDirectoryPath->Length()
                    + ( *fileList )[i].iName.Length() );
            manifestFilePathName.Append( *manifestDirectoryPath );
            manifestFilePathName.Append( ( *fileList )[i].iName );
          
            TPtrC packageUidPtr = manifestDirectoryPath->Mid(
                    manifestDirectoryPath->Length() - KPackageUidPosition,
                    KPackageUidLength); 
            TUint packageUid;            
            TLex lexer( packageUidPtr );
            User::LeaveIfError( lexer.Val( packageUid, EHex ) );

            CCaWidgetDescription* currentWidget = NULL;
            for ( TInt j = 0; j < iFetchedWidgets.Count(); j++ )
                {
                if ( iFetchedWidgets[j]->GetPackageUidL() ==  packageUid )
                    {
                    currentWidget = iFetchedWidgets[j];
                    currentWidget->SetValid( ETrue ); //do not remove from db
                    break; // once found we dont iterate anymore
                    }
                }

            if ( currentWidget )
                {
                TTime modificationTime = ( *fileList )[i].iModified;
                TInt64 modificationIntTime = modificationTime.Int64();
                TLex lex( currentWidget->GetModificationTime() );
                TInt64 uintTimeDB( 0 );
                User::LeaveIfError( lex.Val( uintTimeDB ) );

                if ( uintTimeDB != modificationIntTime )
                    {
                    TRAP_IGNORE( ParseManifestFileL( manifestFilePathName,
                                    aDirectoryName, aDrive ) );
                    }
                else if ( currentWidget->IsMissing() ) 
                    {
                    // manifest of missing widget found
                    CCaWidgetDescription* clone = currentWidget->CloneL();
                    CleanupStack::PushL( clone );
                    clone->SetManifestFilePathNameL( manifestFilePathName );
                    iWidgets.AppendL( clone );
                    CleanupStack::Pop( clone );
                    }
                }
            else
                {
                TRAP_IGNORE( ParseManifestFileL(
                    manifestFilePathName, aDirectoryName, aDrive ) );
                }
            CleanupStack::PopAndDestroy( &manifestFilePathName );
            }
        }
    CleanupStack::PopAndDestroy( fileList );
    CleanupStack::PopAndDestroy( manifestDirectoryPath );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::ParseManifestFileL( const TDesC& aFilePath,
        const TDesC& aPackageUid, TChar& aDrive )
    {
    RXmlEngDOMParser parser;
    CleanupClosePushL( parser );
    User::LeaveIfError( parser.Open( iDomImpl ) );
    RXmlEngDocument doc = parser.ParseFileL( aFilePath, KChunkSize );
    CleanupClosePushL( doc );
    TXmlEngElement docElement = doc.DocumentElement();

    if ( docElement.Name() == KWidgetProvider )
        {
        TXmlEngElement element;
        RXmlEngNodeList<TXmlEngElement> elementList;
        CleanupClosePushL( elementList );
        docElement.GetElementsByTagNameL( elementList, KWidget );

        while ( elementList.HasNext() )
            {
            element = elementList.Next();
            if ( element.HasAttributes() )
                {
                ParseWidgetL( aFilePath, element, aPackageUid, aDrive );
                }
            }
        CleanupStack::PopAndDestroy( &elementList );
        }
    else if ( docElement.Name() == KWidgetManifest )
        {
        CCaWidgetDescription* widgetDescriptor = CCaWidgetDescription::NewL();
        widgetDescriptor->SetPackageUidL( aPackageUid );
        widgetDescriptor->SetFlag( EVisible, ETrue );
        widgetDescriptor->SetManifestFilePathNameL( aFilePath );

        SetMmcIdL( widgetDescriptor, aDrive );

        RXmlEngNodeList<TXmlEngElement> childElementList;
        CleanupClosePushL( childElementList );
        docElement.GetChildElements( childElementList );
        TXmlEngElement element;
        
        while ( childElementList.HasNext() )
            {
            element = childElementList.Next();
            if ( element.Name() == KUri )
                {
                ParseUriL( element, widgetDescriptor );
                }
            else if ( element.Name() == KTitle )
                {
                ParseTitleL( element, widgetDescriptor );
                }
            else if ( element.Name() == KIcon )
                {
                ParseIconL( element, aPackageUid, widgetDescriptor, aDrive );
                }
            else if ( element.Name() == KDescription )
                {
                ParseDescriptionL( element, widgetDescriptor );
                }
            else if ( element.Name() == KHidden )
                {
                ParseHiddenL( element, widgetDescriptor );
                }
            else if ( element.Name() == KServiceXml )
                {
                ParseServiceXmlL( element, widgetDescriptor );
                }
            else if ( element.Name() == KPreviewImageElementName )
                {
                ParsePreviewImageNameL( element, aPackageUid, widgetDescriptor, aDrive );
                }
            else if ( element.Name() == KTranslationFileName )
                {
                ParseTranslationFileNameL( element, widgetDescriptor );
                }
            }
        CleanupStack::PopAndDestroy( &childElementList );

        // Set path for hs to use, trim the last character (backslash).
        HBufC *libraryPath = GetManifestDirectoryPathLC( aPackageUid, aDrive );
        widgetDescriptor->SetPathL(libraryPath->Mid(0,libraryPath->Length()-1));
        CleanupStack::PopAndDestroy(libraryPath);

        
        HBufC *libraryPath2 = GetManifestDirectoryPathLC( aPackageUid, aDrive );
        libraryPath2 = libraryPath2->ReAllocL( libraryPath2->Length()
                + widgetDescriptor->GetUri().Length() );
        libraryPath2->Des().Append( widgetDescriptor->GetUri() );
        widgetDescriptor->SetLibraryL( *libraryPath2 );
        CleanupStack::PopAndDestroy( libraryPath2 );

        SetModificationTimeL( aFilePath, widgetDescriptor );

        TInt index = iWidgets.Find( widgetDescriptor,
                CCaWidgetDescription::CompareUri );
        if ( index != KErrNotFound )
            {
            delete iWidgets[index];
            iWidgets.Remove( index );
            }
        iWidgets.AppendL( widgetDescriptor );//ownership transfer
        widgetDescriptor = NULL;
        }

    CleanupStack::PopAndDestroy( &doc );
    CleanupStack::PopAndDestroy( &parser );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::ParseWidgetL( const TDesC& aFilePath,
        TXmlEngElement& aElement, const TDesC& aPackageUid, TChar& aDrive )
    {
    CCaWidgetDescription* widget = CCaWidgetDescription::NewLC();

    SetUriL( aElement, widget );
    SetLibraryL( aElement, aPackageUid, widget, aDrive );
    SetTitleL( aElement, widget );
    SetDescriptionL( aElement, widget );
    SetVisibilityL( aElement, widget );
    SetIconUriL( aElement, aPackageUid, widget, aDrive );
    widget->SetPackageUidL( aPackageUid );
    widget->SetManifestFilePathNameL( aFilePath );
    SetMmcIdL( widget, aDrive );
    SetTranslationFileNameL( aElement, widget );

    SetModificationTimeL( aFilePath, widget );

    TInt index = iWidgets.Find( widget, CCaWidgetDescription::CompareUri );
    if ( index != KErrNotFound )
        {
        delete iWidgets[index];
        iWidgets.Remove( index );
        }
    iWidgets.AppendL( widget );//ownership transfer
    CleanupStack::Pop( widget );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::SetUriL( TXmlEngElement& aElement,
        CCaWidgetDescription* aWidget )
    {
    HBufC *attributeValue = CnvUtfConverter::ConvertToUnicodeFromUtf8L(
            aElement.AttributeValueL( KUri ) );
    CleanupStack::PushL( attributeValue );

    if ( attributeValue->Compare( KNullDesC ) != 0 )
        {
        aWidget->SetUriL( *attributeValue );
        aWidget->SetTranslationFileNameL( *attributeValue );
        }
    CleanupStack::PopAndDestroy( attributeValue );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::SetTranslationFileNameL( TXmlEngElement& aElement,
        CCaWidgetDescription* aWidget )
    {
    HBufC *attributeValue = CnvUtfConverter::ConvertToUnicodeFromUtf8L(
            aElement.AttributeValueL( KTranslationFileName ) );
    CleanupStack::PushL( attributeValue );

    if ( attributeValue->Compare( KNullDesC ) != 0 )
        {
        aWidget->SetTranslationFileNameL( *attributeValue );
        }
    CleanupStack::PopAndDestroy( attributeValue );
    }

// ----------------------------------------------------------------------------
//   
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::SetLibraryL( TXmlEngElement& aElement,
        const TDesC& aPackageUid, CCaWidgetDescription* aWidget, TChar& aDrive )
    {
    HBufC *attributeValue = CnvUtfConverter::ConvertToUnicodeFromUtf8L(
            aElement.AttributeValueL( KLibrary ) );
    CleanupStack::PushL( attributeValue );

    if ( attributeValue->Compare( KNullDesC ) != 0 )
        {
        HBufC *libraryPath = GetManifestDirectoryPathLC( aPackageUid, aDrive );

        //set path for hs to use, trim last 2 chars (doubleslash)
        aWidget->SetPathL(libraryPath->Mid(0,libraryPath->Length()-1));

        libraryPath->ReAllocL( libraryPath->Length() + attributeValue->Length() );

        TPtr libraryPathModifier( libraryPath->Des() );
        libraryPathModifier.Append( *attributeValue );
        aWidget->SetLibraryL( *libraryPath );

        CleanupStack::PopAndDestroy( libraryPath );
        }
    else
        {
        aWidget->SetLibraryL( KNoLibrary );
        }
    CleanupStack::PopAndDestroy( attributeValue );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::SetTitleL( TXmlEngElement& aElement,
        CCaWidgetDescription* aWidget )
    {
    HBufC *attributeValue = CnvUtfConverter::ConvertToUnicodeFromUtf8L(
            aElement.AttributeValueL( KTitle ) );
    CleanupStack::PushL( attributeValue );

    if ( attributeValue->Compare( KNullDesC ) != 0 )
        {
        aWidget->SetTitleL( *attributeValue );
        }
    CleanupStack::PopAndDestroy( attributeValue );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::SetDescriptionL( TXmlEngElement& aElement,
        CCaWidgetDescription* aWidget )
    {
    HBufC *attributeValue = CnvUtfConverter::ConvertToUnicodeFromUtf8L(
            aElement.AttributeValueL( KDescription ) );
    CleanupStack::PushL( attributeValue );

    if ( attributeValue->Compare( KNullDesC ) != 0 )
        {
        aWidget->SetDescriptionL( *attributeValue );
        }
    CleanupStack::PopAndDestroy( attributeValue );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::SetVisibilityL( TXmlEngElement& aElement,
        CCaWidgetDescription* aWidget )
    {
    HBufC *hidden = CnvUtfConverter::ConvertToUnicodeFromUtf8L(
            aElement.AttributeValueL( KHidden ) );
    CleanupStack::PushL( hidden );

    if ( hidden->Compare( KTrue ) == 0 )
        {
        aWidget->SetFlag( EVisible, EFalse );
        }
    else
        {
        aWidget->SetFlag( EVisible, ETrue );
        }
    CleanupStack::PopAndDestroy( hidden );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::SetIconUriL( TXmlEngElement& aElement,
        const TDesC& aPackageUid, CCaWidgetDescription* aWidget, TChar& aDrive )
    {
    HBufC *attributeValue = CnvUtfConverter::ConvertToUnicodeFromUtf8L(
            aElement.AttributeValueL( KIconUri ) );
    CleanupStack::PushL( attributeValue );

    if ( attributeValue->Compare( KNullDesC ) != 0 )
        {
        HBufC* iconUriPath = GetManifestDirectoryPathLC( aPackageUid, aDrive );
        iconUriPath->ReAllocL( iconUriPath->Length() + attributeValue->Length() );

        TPtr iconUriPathModifier( iconUriPath->Des() );
        iconUriPathModifier.Append( *attributeValue );
        aWidget->SetIconUriL( *iconUriPath );

        CleanupStack::PopAndDestroy( iconUriPath );
        }
    CleanupStack::PopAndDestroy( attributeValue );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::SetMmcIdL( CCaWidgetDescription* aWidget,
        TChar& aDrive )
    {
    TChar removableDrive;
    User::LeaveIfError( DriveInfo::GetDefaultDrive(
            DriveInfo::EDefaultRemovableMassStorage, removableDrive ) );

    TChar massStorageDrive;
    User::LeaveIfError( DriveInfo::GetDefaultDrive(
            DriveInfo::EDefaultMassStorage, massStorageDrive ) );
    if ( aDrive == removableDrive )
        {
        RBuf mmcId;
        mmcId.CleanupClosePushL();
        mmcId.CreateL( KMassStorageIdLength );
        WidgetScannerUtils::CurrentMmcId( iFs, mmcId );
        aWidget->SetMmcIdL( mmcId );
        CleanupStack::PopAndDestroy( &mmcId );
        }
    else if ( aDrive == massStorageDrive )
        {
        aWidget->SetMmcIdL( KCaMassStorage );
        }
    }
// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::SetModificationTimeL( const TDesC& aFilePath,
        CCaWidgetDescription* aWidgetDescriptor )
    {
    TTime modificationTime;
    iFs.Modified( aFilePath, modificationTime );
    TInt64 modificationIntTime = modificationTime.Int64();
    RBuf16 rBuf;
    rBuf.CleanupClosePushL();
    rBuf.CreateL( KModificationTimeLength );
    rBuf.AppendNum( modificationIntTime );
    aWidgetDescriptor->SetModificationTimeL( rBuf );
    CleanupStack::PopAndDestroy( &rBuf );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
CDir* CCaWidgetScannerParser::GetDirectoriesLC( TChar& aDrive )
    {
    CDir* result = NULL;
    HBufC* path = FullPathLC( aDrive );
    iFs.GetDir( *path, KEntryAttDir, ESortByName, result );
    CleanupStack::PopAndDestroy( path );
    CleanupStack::PushL( result );
    return result;
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
HBufC* CCaWidgetScannerParser::FullPathLC( TChar& aDrive ) const
    {
    HBufC* result = HBufC16::NewLC( iImportPath.Length() + KDriveLetterLength );
    TPtr modifier( result->Des() );
    modifier.Append( aDrive );
    modifier.Append( KColen );
    modifier.Append( iImportPath );
    return result;
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
HBufC* CCaWidgetScannerParser::GetManifestDirectoryPathLC(
        const TDesC& aDirectoryName, TChar& aDrive ) const
    {
    HBufC* result = HBufC16::NewLC( KMaxPath );
    TPtr modifier( result->Des() );
    HBufC* path = FullPathLC( aDrive );
    modifier.Append( *path );
    CleanupStack::PopAndDestroy( path );
    modifier.Append( aDirectoryName );
    modifier.Append( KDoubleSlash );
    return result;
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::ParseUriL( TXmlEngElement& aElement,
        CCaWidgetDescription* aWidgetDescriptor )
    {
    aWidgetDescriptor->SetUriL( *GetElementTextLC( aElement )  );
    CleanupStack::PopAndDestroy();
    
    if( aWidgetDescriptor->GetTranslationFileName().Compare( KNullDesC ) == 0 )
        {
        ParseTranslationFileNameL( aElement, aWidgetDescriptor );
        }
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::ParseTitleL( TXmlEngElement& aElement,
        CCaWidgetDescription* aWidgetDescriptor )
    {
    aWidgetDescriptor->SetTitleL( *GetElementTextLC( aElement ) );
    CleanupStack::PopAndDestroy( );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::ParseIconL( TXmlEngElement& aElement,
        const TDesC& aPackageUid, CCaWidgetDescription* aWidgetDescriptor,
        TChar& aDrive )
    {
    HBufC *const graphicsName = 
        GetThemableGraphicsNameLC(aElement, aPackageUid, aDrive );   
    
    aWidgetDescriptor->SetIconUriL( *graphicsName );
   
    CleanupStack::PopAndDestroy( graphicsName );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::ParsePreviewImageNameL(
        TXmlEngElement & aElement,
        const TDesC & aPackageUid,
        CCaWidgetDescription* aWidgetDescriptor,
        TChar& aDrive )
    {
    HBufC *const graphicsName = 
        GetThemableGraphicsNameLC(aElement, aPackageUid, aDrive );
    
    aWidgetDescriptor->SetPreviewImageNameL( *graphicsName );
    
    CleanupStack::PopAndDestroy( graphicsName );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::ParseTranslationFileNameL(
        TXmlEngElement& aElement, CCaWidgetDescription* aWidgetDescriptor )
    {
    aWidgetDescriptor->SetTranslationFileNameL( *GetElementTextLC( aElement ) );
    CleanupStack::PopAndDestroy();
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::ParseDescriptionL( TXmlEngElement& aElement,
        CCaWidgetDescription* aWidgetDescriptor )
    {
    aWidgetDescriptor->SetDescriptionL( *GetElementTextLC( aElement ) );
    CleanupStack::PopAndDestroy();
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::ParseHiddenL( TXmlEngElement& aElement,
        CCaWidgetDescription* aWidgetDescriptor )
    {
    if( aElement.Text().Compare( _L8("true") ) == 0 )
        {
        aWidgetDescriptor->SetFlag( EVisible, EFalse );
        }
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerParser::ParseServiceXmlL( TXmlEngElement& aElement,
        CCaWidgetDescription* aWidgetDescriptor )
    {
    aWidgetDescriptor->SetServiceXmlL( *GetElementTextLC( aElement ) );
    CleanupStack::PopAndDestroy( );
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
HBufC* CCaWidgetScannerParser::GetElementTextLC( 
    const TXmlEngElement& aElement ) const
    {
    HBufC* const elementText =
        CnvUtfConverter::ConvertToUnicodeFromUtf8L( aElement.Text() );
    CleanupStack::PushL( elementText );
    return elementText;
    }


// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
HBufC* CCaWidgetScannerParser::GetThemableGraphicsNameLC(
        const TXmlEngElement& aElement,
        const TDesC& aPackageUid,
        TChar& aDrive ) const
    {
    RBuf graphicsName;
    graphicsName.CleanupClosePushL();

    RBuf elementText;
    elementText.CleanupClosePushL();
    
    elementText.Assign( 
        CnvUtfConverter::ConvertToUnicodeFromUtf8L( aElement.Text() ) );
    
    if ( TParsePtrC(elementText).Ext() != KNullDesC )
        {
        HBufC* const manifestDirectoryPath( 
            GetManifestDirectoryPathLC( aPackageUid, aDrive ) );
        
        graphicsName.CreateL( manifestDirectoryPath->Length() + 
            elementText.Length() );
        graphicsName.Append( *manifestDirectoryPath );
        
        CleanupStack::PopAndDestroy( manifestDirectoryPath );
        }
    else
        {   
        graphicsName.CreateL( elementText.Length() );
        }
    
    graphicsName.Append( elementText );

    
    CleanupStack::PopAndDestroy( &elementText );
    
    HBufC *result = HBufC::NewL( graphicsName.Length() ) ;
    *result = graphicsName;
    
    CleanupStack::PopAndDestroy( &graphicsName );
    
    CleanupStack::PushL( result );
    
    return result;
    }

//  End of File
