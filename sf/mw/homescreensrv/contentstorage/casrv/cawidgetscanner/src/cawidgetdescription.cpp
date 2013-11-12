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
#include <hbtextresolversymbian.h>

#include "cadef.h"
#include "cawidgetdescription.h"
#include "cainnerentry.h"
#include "widgetscannerutils.h"
#include "cawidgetscannerdef.h"
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCaWidgetDescription::CCaWidgetDescription
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCaWidgetDescription::CCaWidgetDescription() :
    iEntryId( KNoId ), iValid ( EFalse )
    {
    }

// -----------------------------------------------------------------------------
// CCaWidgetDescription::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::ConstructL( )
    {
    }

// -----------------------------------------------------------------------------
// CCaWidgetDescription::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::ConstructL( CCaInnerEntry* aEntry )
    {
    iEntryId = aEntry->GetId();
    iDescription.CreateL( aEntry->GetDescription() );
    //package uid
    TBuf<KMaxUidName> uidDesc;
    aEntry->FindAttribute( KCaPackageUid, uidDesc );
    if ( uidDesc.Length() )
        {
        TLex uidLex(uidDesc);
        User::LeaveIfError( uidLex.Val( iPackageUid, EHex ));
        }
    iTitle.CreateL( aEntry->GetText() );
    
    iIconUri.CreateL( aEntry->Icon()->FileName() );
    
    TPtrC attribute;
    aEntry->FindAttribute( KPreviewImageAttrName, attribute );
    iPreviewImageName.CreateL( attribute.Length() );
    iPreviewImageName = attribute;

    //library
    iLibrary.CreateL( KCaMaxAttrValueLen );
    aEntry->FindAttribute( KAttrWidgetLibrary, iLibrary );
    //path
    iPath.CreateL( KCaMaxAttrValueLen );
    aEntry->FindAttribute( KAttrWidgetPath, iPath );
    //uri
    iUri.CreateL( KCaMaxAttrValueLen );
    aEntry->FindAttribute( KAttrWidgetUri, iUri );
    //translation filename
    iTranslationFileName.CreateL( KCaMaxAttrValueLen );
    aEntry->FindAttribute( KAttrWidgetTranslationFileName, iTranslationFileName );
    //mmc id
    iMmcId.CreateL(KMassStorageIdLength);
    aEntry->FindAttribute( KCaAttrMmcId, iMmcId );
    //service xml
    iServiceXml.CreateL( KCaMaxAttrValueLen );
    aEntry->FindAttribute( KAttrWidgetServiceXml,iServiceXml );
    
    iFlags = aEntry->GetFlags();
    iModificationTime.CreateL(KCaMaxAttrValueLen);
    aEntry->FindAttribute( KCaAttrInstallationTime, iModificationTime );
    }

// -----------------------------------------------------------------------------
// CCaWidgetScannerParser::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCaWidgetDescription* CCaWidgetDescription::NewL( )
    {
    CCaWidgetDescription* self = NewLC( );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCaWidgetDescription::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCaWidgetDescription* CCaWidgetDescription::NewLC( )
    {
    CCaWidgetDescription* self = new( ELeave ) CCaWidgetDescription();
    CleanupStack::PushL( self );
    self->ConstructL( );
    return self;
    }

// -----------------------------------------------------------------------------
// CCaWidgetDescription::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCaWidgetDescription* CCaWidgetDescription::NewLC( CCaInnerEntry* aEntry )
    {
    CCaWidgetDescription* self = new( ELeave ) CCaWidgetDescription();
    CleanupStack::PushL( self );
    self->ConstructL( aEntry );
    return self;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CCaWidgetDescription::~CCaWidgetDescription()
    {
    iPath.Close();
    iModificationTime.Close();
    iServiceXml.Close();
    iMmcId.Close();
    iUri.Close();
    iTranslationFileName.Close();
    iLibrary.Close();
    iPreviewImageName.Close();
    iIconUri.Close();
    iTitle.Close();
    iDescription.Close();
    iStringIdTitle.Close();
    iStringIdDescription.Close();
    iManifestFilePathName.Close();
    
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
CCaWidgetDescription *CCaWidgetDescription::CloneL()
    {   
    CCaWidgetDescription* clone = CCaWidgetDescription::NewLC();
    
    clone->SetPathL( GetPath() );
    clone->SetMmcIdL( GetMmcId() );
    clone->SetStringidTitleL( GetStringIdTitle() );
    clone->SetLibraryL( GetLibrary() );
    clone->SetDescriptionL( GetDescription() );
    clone->SetStringIdDescriptionL( GetStringIdDescription() );
    clone->SetUriL( GetUri() );
    clone->SetIconUriL( GetIconUri() );
    clone->SetPreviewImageNameL( GetPreviewImageName() );
    clone->SetTitleL( GetTitle() );
    clone->SetTranslationFileNameL( GetTranslationFileName() );
    clone->SetLibraryL( GetLibrary() );
    clone->SetModificationTimeL( GetModificationTime() );
    clone->SetServiceXmlL( GetServiceXml() );
    clone->SetManifestFilePathNameL( GetManifestFilePathName() );
    
    CleanupStack::Pop( clone );
    return clone;
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
TBool CCaWidgetDescription::CompareUri(
        const CCaWidgetDescription& aFirst,const CCaWidgetDescription& aSecond)
    {
    TBool result = EFalse;
    if( aFirst.GetUri() == aSecond.GetUri() )
        {
        result = ETrue;
        }
    else
        {
        result = EFalse;
        }
    return result;
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
TBool CCaWidgetDescription::Compare( const CCaWidgetDescription& aToCompare )
    {
    TBool result = EFalse;
    if( aToCompare.GetLibrary() == GetLibrary() &&
            aToCompare.GetDescription() == GetDescription() &&
            aToCompare.GetUri() == GetUri() &&
            aToCompare.GetIconUri() == GetIconUri() &&
            aToCompare.GetPreviewImageName() == GetPreviewImageName() &&
            aToCompare.GetTitle() == GetTitle() &&
            aToCompare.GetTranslationFileName() == GetTranslationFileName() &&
            aToCompare.GetLibrary() != KNoLibrary &&
            aToCompare.GetModificationTime() == GetModificationTime() &&
            aToCompare.GetServiceXml() == GetServiceXml()
            )
        {
        result = ETrue;
        }
    else
        {
        result = EFalse;
        }
    return result;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::SetMmcIdL( const TDesC& aMmcId )
    {
    iMmcId.Close();
    iMmcId.CreateL( aMmcId );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::SetPackageUidL( const TDesC& aPackageUid )
    {
    TLex lexer( aPackageUid );
    User::LeaveIfError( lexer.Val( iPackageUid,EHex));
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TUint CCaWidgetDescription::GetPackageUidL()
    {
    return iPackageUid;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::SetTitleL( const TDesC& aTitle )
    {
    iTitle.Close();
    iTitle.CreateL(aTitle);
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::SetDescriptionL( const TDesC& aDescription )
    {
    iDescription.Close();
    iDescription.CreateL(aDescription);
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::SetUriL( const TDesC& aUri )
    {
    iUri.Close();
    iUri.CreateL(aUri);
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::SetTranslationFileNameL(
        const TDesC& aTranslationFileName )
    {
    iTranslationFileName.Close();
    iTranslationFileName.CreateL( aTranslationFileName );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::SetIconUriL( const TDesC& aIconUri )
    {
    iIconUri.Close();
    iIconUri.CreateL(aIconUri);
    }
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::SetPreviewImageNameL( const TDesC& aPreviewName )
    {
    iPreviewImageName.Close();
    iPreviewImageName.CreateL( aPreviewName );
    }
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::SetLibraryL( const TDesC& aLibrary )
    {
    iLibrary.Close();
    iLibrary.CreateL(aLibrary);
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::SetFlag( TInt aFlag, TBool aValue )
    {
    if( aValue )
        {
        iFlags = iFlags | aFlag;
        }
    else
        {
        iFlags = iFlags & ~aFlag;
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::SetModificationTimeL( const TDesC& aModificationTime )
    {
    iModificationTime.Close();
    iModificationTime.CreateL( aModificationTime );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::SetStringIdDescriptionL( const TDesC& aStringIdDescription )
    {
    iStringIdDescription.Close();
    iStringIdDescription.CreateL( aStringIdDescription );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::SetStringidTitleL( const TDesC& aStringIdTitle )
    {
    iStringIdTitle.Close();
    iStringIdTitle.CreateL( aStringIdTitle );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CCaWidgetDescription::GetEntryId( ) const
    {
    return iEntryId;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TPtrC CCaWidgetDescription::GetMmcId( ) const
    {
    return iMmcId;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TPtrC CCaWidgetDescription::GetStringIdDescription() const
    {
    return iStringIdDescription;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//

TPtrC CCaWidgetDescription::GetStringIdTitle() const
    {
    return iStringIdTitle;
    }
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TPtrC CCaWidgetDescription::GetLibrary( ) const
    {
    return iLibrary;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TPtrC CCaWidgetDescription::GetPath( ) const
    {
    return iPath;
    }
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::SetPathL( const TDesC& aPath )
    {
	iPath.Close();
	iPath.CreateL(aPath);
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TPtrC CCaWidgetDescription::GetDescription( ) const
    {
    return iDescription;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TPtrC CCaWidgetDescription::GetUri( ) const
    {
    return iUri;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TPtrC CCaWidgetDescription::GetTranslationFileName() const
    {
    return iTranslationFileName;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TPtrC CCaWidgetDescription::GetIconUri( ) const
    {
    return iIconUri;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TPtrC CCaWidgetDescription::GetPreviewImageName( ) const
    {
    return iPreviewImageName;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TPtrC CCaWidgetDescription::GetTitle( ) const
    {
    TPtrC result ;
    if ( iTitle == KNullDesC )
        {
        result.Set( GetLibraryName() );
        }
    else
        {
        result.Set( iTitle );
        }
    return result;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TPtrC CCaWidgetDescription::GetModificationTime( ) const
    {
    return iModificationTime;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CCaWidgetDescription::IsMissing( ) const
    {
    return iFlags & EMissing;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CCaWidgetDescription::IsUsed( ) const
    {
    return iFlags & EUsed;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CCaWidgetDescription::IsVisible( ) const
    {
    return iFlags & EVisible;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CCaInnerEntry* CCaWidgetDescription::GetEntryLC( ) const
    {
    CCaInnerEntry* entry = CCaInnerEntry::NewLC();
    entry->SetEntryTypeNameL( KCaTypeWidget );
    entry->SetRole( EItemEntryRole );
    entry->SetFlags( iFlags );
    if ( iPackageUid )
        {
        TBuf<KMaxUidName> uidDesc;
        uidDesc.AppendNum( iPackageUid,EHex );
        entry->AddAttributeL( KCaPackageUid, uidDesc );
        }
    if ( iEntryId != KNoId )
        {
        entry->SetId( iEntryId );
        }
    if ( iMmcId != KNullDesC )
        {
        entry->AddAttributeL( KCaAttrMmcId, iMmcId );
        }
    if ( iLibrary != KNullDesC )
        {
        entry->AddAttributeL(KAttrWidgetLibrary, iLibrary);
        }
    if ( iPath != KNullDesC )
        {
        entry->AddAttributeL(KAttrWidgetPath, iPath);
        }
    if ( iTitle != KNullDesC )
        {
        entry->SetTextL(iTitle);
        }
    else
        {
        TPtrC libraryName( GetLibraryName() );
        entry->SetTextL( libraryName );
        }

    if ( iDescription != KNullDesC )
        {
        entry->SetDescriptionL( iDescription );
        }
    if ( iUri != KNullDesC )
        {
        entry->AddAttributeL(KAttrWidgetUri, iUri);
        }
    if ( iIconUri != KNullDesC)
        {
        // aSkinId and AppId not used for widgets - KNullDesC
        entry->SetIconDataL( iIconUri, KNullDesC, KNullDesC );
        }
    if ( iPreviewImageName != KNullDesC )
        {
        entry->AddAttributeL( KPreviewImageAttrName, iPreviewImageName );
        }
    if( iModificationTime != KNullDesC )
        {
        entry->AddAttributeL( KCaAttrInstallationTime, iModificationTime );
        }
    if ( iServiceXml != KNullDesC )
        {
        entry->AddAttributeL( KAttrWidgetServiceXml, iServiceXml);
        }
    if( iTranslationFileName != KNullDesC )
        {
        entry->AddAttributeL( KAttrWidgetTranslationFileName,
                iTranslationFileName );
        }
    return entry;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TPtrC CCaWidgetDescription::GetLibraryName( ) const
    {
    TPtrC libraryName( iLibrary );
    TInt pos( 0 );
    while ( ( pos = libraryName.Find( KDoubleSlash ) ) != KErrNotFound )
        {
        libraryName.Set( libraryName.Mid( pos + 1 ) );
        }
    pos = libraryName.Find( KDllExt );
    if (pos == (libraryName.Length() - KDllExt().Length()))
        {
        libraryName.Set( libraryName.Mid( 0, pos ));
        }
    return libraryName;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::RemoveMmcId( )
    {
    iMmcId.Close();
    }
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::SetServiceXmlL(const TDesC& aServiceXml)
    {
    iServiceXml.Close();
    iServiceXml.CreateL( aServiceXml );
    }
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TPtrC CCaWidgetDescription::GetServiceXml() const
    {
    return iServiceXml;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::LocalizeTextsL()
    {
    if( iTranslationFileName.Length() )
        {
        RBuf translationFileName;
        CleanupClosePushL( translationFileName );
        translationFileName.CreateL( iTranslationFileName.Length() + 1 );
        translationFileName.Copy( iTranslationFileName );
        translationFileName.Append( KWidgetScannerUnderline );

        if( !HbTextResolverSymbian::Init( translationFileName, KLocalizationFilepathC ) )
            {
            if( !HbTextResolverSymbian::Init( translationFileName, KLocalizationFilepathZ ) )
                {
                // this should not be called too often 
                TChar currentDriveLetter;
                TDriveList driveList;
                RFs fs;
                User::LeaveIfError( fs.Connect() );
                User::LeaveIfError( fs.DriveList( driveList ) );

                RBuf path;
                CleanupClosePushL( path );
                path.CreateL( KLocalizationFilepath().Length() + 1 );

                for( TInt driveNr=EDriveY; driveNr >= EDriveA; driveNr-- )
                    {
                    if( driveList[driveNr] )
                        {
                        User::LeaveIfError( fs.DriveToChar( driveNr,
                            currentDriveLetter ) );
                        path.Append( currentDriveLetter );
                        path.Append( KLocalizationFilepath );
                        if( HbTextResolverSymbian::Init( translationFileName, path ) )
                            {
                            break;
                            }
                        }
                    path.Zero();
                    }
                CleanupStack::PopAndDestroy( &path );
                fs.Close();
                }
            }
        
        HBufC* tmp;
        
        if( iTitle.Length() )
            {
            SetStringidTitleL( iTitle );
            tmp = HbTextResolverSymbian::LoadLC( iTitle );
            SetTitleL( *tmp );
            CleanupStack::PopAndDestroy( tmp );
            }
        if( iDescription.Length() )
            {
            SetStringIdDescriptionL( iDescription );
            tmp = HbTextResolverSymbian::LoadLC( iDescription );
            SetDescriptionL( *tmp );
            CleanupStack::PopAndDestroy( tmp );
            }
        CleanupStack::PopAndDestroy( &translationFileName );
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
TBool CCaWidgetDescription::IsValid()
    {
    return iValid;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::SetValid(TBool aVal)
    {
    iValid = aVal;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CCaWidgetDescription::SetManifestFilePathNameL( 
    const TDesC& aManifestFilePathName )
    {
    iManifestFilePathName.Close();
    iManifestFilePathName.CreateL( aManifestFilePathName );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TPtrC CCaWidgetDescription::GetManifestFilePathName() const
    {
    return iManifestFilePathName;
    }
//  End of File
