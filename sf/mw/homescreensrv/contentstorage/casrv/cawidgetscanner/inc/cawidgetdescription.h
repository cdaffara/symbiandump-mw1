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
#ifndef C_WIDGETDESCRIPTION_H
#define C_WIDGETDESCRIPTION_H

//  INCLUDES
#include <e32base.h>
// CONSTANTS

// FORWARD DECLARATIONS
class CCaInnerEntry;

/**
 *  Widget Parser
 *
 *  @since Series Series60 5.x
 */
class CCaWidgetDescription : public CBase
    {

public:
    // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static CCaWidgetDescription* NewL( );
    static CCaWidgetDescription* NewLC( );
    static CCaWidgetDescription* NewLC( CCaInnerEntry* aEntry );

    /**
     * Destructor.
     */
    virtual ~CCaWidgetDescription();

    /**
     * @return new heap allocated object being a copy of the object.
     * Ownership is passed to the caller.
     */
    CCaWidgetDescription *CloneL();
    
    /**
     * Static method to compare two CCaWidgetDescription objects
     * used in Find() method
     * @param aFirst first item to compare
     * @param aSecond second item to compare
     * @return ETrue if widget uri value is the same in both objects,
     * otherwise returns EFalse
     */
    static TBool CompareUri( const CCaWidgetDescription& aFirst,
            const CCaWidgetDescription& aSecond );

    /**
     * Method to compare two CCaWidgetDescription objects
     * @param aToCompare first item to compare
     * @return ETrue if all value is the same in both objects,
     * otherwise returns EFalse
     */
    TBool Compare( const CCaWidgetDescription& aToCompare );

    /*
     * MMC id setter
     * @param aMmcId MMC id
     */
    void SetMmcIdL( const TDesC& aMmcId );

    /*
     * PackageUid setter
     * @param aPackageUid package UID
     */
    void SetPackageUidL( const TDesC& aPackageUid );

    /*
     * PackageUid getter
     * @return iPackageUid - package UID
     */
    TUint GetPackageUidL();
    
    /*
     * Title setter
     * @param aTitle widget title
     */
    void SetTitleL( const TDesC& aTitle );

    /*
     * Description setter
     * @param aTitle widget title
     */
    void SetDescriptionL( const TDesC& aDescription );

    /*
     * Uri setter
     * @param aUri widget uri
     */
    void SetUriL( const TDesC& aUri );
    
    /*
     * Translation file setter.
     * @param aTranslationFileName widget translation file.
     */
    void SetTranslationFileNameL( const TDesC& aTranslationFileName );

    /*
     * Icon Uri setter
     * @param aIconUri widget icon uri
     */
    void SetIconUriL( const TDesC& aIconUri );

    /*
     * Preview image name setter.
     * @param aPreview preview image name identifier.
     */
    void SetPreviewImageNameL( const TDesC& aPreviewName );
    
    /*
     * Library setter
     * @param aLibrary widget library
     */
    void SetLibraryL( const TDesC& aLibrary );

    /*
     * Path setter
     * @param aLibrary widget library
     */
    void SetPathL( const TDesC& aPath );

    /*
     * Path getter
     * @return widget library
     */
    TPtrC GetPath( ) const;
    
    /*
     * Content storage entry id getter
     * @return CS entry id
     */
    TInt GetEntryId() const;

    /*
     * MMC id getter
     * @return CA entry id
     */
    TPtrC GetMmcId() const;

    /*
     * Library getter
     * @return widget library
     */
    TPtrC GetLibrary() const;

    /*
     * Description getter
     * @return widget Description
     */
    TPtrC GetDescription() const;

    /*
     * Uri getter
     * @return widget Uri
     */
    TPtrC GetUri() const;

    /*
     * Uri translation filename
     * @return widget translation filename
     */
    TPtrC GetTranslationFileName() const;

    /*
     * IconUri getter
     * @return widget IconUri
     */
    TPtrC GetIconUri() const;

    /*
     * Preview image name getter
     * @return widget preview image name
     */
    TPtrC GetPreviewImageName( ) const;
    
    /*
     * Title getter
     * @return widget title
     */
    TPtrC GetTitle() const;

    /*
     * Missing flag
     * @return ETrue if flag missing is set
     */
    TBool IsMissing() const;

    /*
     * Used flag
     * @return ETrue if flag used is set
     */
    TBool IsUsed() const;

    /*
     * Used flag
     * @return ETrue if flag visible is set
     */
    TBool IsVisible() const;

    /*
     * Entry getter
     * @return entry representing widget
     */
    CCaInnerEntry* GetEntryLC() const;

    /*
     * Modification time setter
     * @param aModificationTime
     */
    void SetModificationTimeL( const TDesC& aModificationTime );

    /*
     * Modification time getter
     * @return widget Modification time
     */
    TPtrC GetModificationTime() const;

    /*
     * Remove MMC id.
     */
    void RemoveMmcId();
    /*
     * Set service fw xml filename
     * @param aServiceXml Service xml filename
     */
    void SetServiceXmlL( const TDesC& aServiceXml );
    /*
     * Get service fw xml filename
     * @return Service xml filename
     */
    TPtrC GetServiceXml() const;

    /*
     /*
     * Localizes title and description, fill string id fields
     */
    void LocalizeTextsL();

    /*
     * StringIdDescription time getter
     * @return StringIdDescription
     */
    TPtrC GetStringIdDescription() const;

    /*
     * StringIdTitle time getter
     * @return widget StringIdTitle
     */
    TPtrC GetStringIdTitle() const;

    /*
     * Set manifest file path name.
     * @param aServiceXml Manifest file path name.
     */
    void SetManifestFilePathNameL( const TDesC& aManifestFilePathName );

    /*
     * Get manifest file path name.
     * @return Manifest file path name.
     */
    TPtrC GetManifestFilePathName() const;

    /*
     * Set logical string for description.
     * @param aStringIdDescription logical string for description.
     */
    void SetStringIdDescriptionL( const TDesC& aStringIdDescription );

    /*
     * Set logical string for title.
     * @param aStringIdTitle logical string for title.
     */
    void SetStringidTitleL( const TDesC& aStringIdTitle );

    /*
     * Flag setter
     * @param aFlag flag
     * @param aValue flag value
     */
    void SetFlag( TInt aFlag, TBool aValue );

    /*
     * Valid setter
     * @param aVal value
     */ 
    void SetValid(TBool aVal);
    
    /*
     * Valid getter
     * @return valid value
     */ 
    TBool CCaWidgetDescription::IsValid();

    /*
     * Library name getter
     * @return libraru name
     */
    TPtrC GetLibraryName() const;


    /**
     * C++ default constructor.
     */
    CCaWidgetDescription();

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();
    void ConstructL( CCaInnerEntry* aEntry );

    // Data

    /*
     * Content Storage entry id
     */
    TInt iEntryId;
    
    /*
     * Marks vidget as valid, used to remove widgets
     */
    TBool iValid;

    /*
     * Package UID
     */
    TUint iPackageUid;

    /*
     * MMC id
     */
    RBuf iMmcId;

    /*
     * Widget title. Own
     */
    RBuf iTitle;

    /*
     * Widget description. Own
     */
    RBuf iDescription;

    /*
     * Widget uri. Own
     */
    RBuf iUri;
    
    /*
     * Widget translation file name. Own
     */
    RBuf iTranslationFileName;

    /*
     * Widget icon uri. Own
     */
    RBuf iIconUri;
    
    /*
     * Preview name. Own
     */
    RBuf iPreviewImageName;
    
    /*
     * Widget library. Own
     */
    RBuf iPath;
    
    /*
     * Widget library. Own
     */
    RBuf iLibrary;

    /*
     * Widgets flags.
     */
    TInt iFlags;

    /*
     * Modification time.
     */
    RBuf iModificationTime;
    /*
     * Service fw xml file
     */
    RBuf iServiceXml;
    /*
     * Manifest file path name.
     */
    RBuf iManifestFilePathName;
    /*
     * Strign id of title
     */
    RBuf iStringIdTitle;
    /*
     * Strign id of description
     */
    RBuf iStringIdDescription;
    };

typedef RPointerArray<CCaWidgetDescription> RWidgetArray;

#endif      // C_WIDGETDESCRIPTION_H
// End of File
