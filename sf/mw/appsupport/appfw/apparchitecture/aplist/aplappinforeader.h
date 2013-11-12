// Copyright (c) 2004-2010 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// aplappinforeader.h
//

#ifndef __APLAPPINFOREADER_H__
#define __APLAPPINFOREADER_H__

#include <apaid.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <apaidpartner.h>
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS
#include <apmstd.h>
#include <barsc.h>
#include <barsread.h>
#include <apgicnfl.h>

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
#include <usif/scr/scr.h>
#include <usif/scr/appregentries.h>
#endif

class TEntry;
class RFs;
class CResourceFile;
class RResourceReader;
class CApaAppList;
class CApaAppIconArray;
class CApaAppViewData;

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
/**
@internalComponent
*/
NONSHARABLE_CLASS(ApaUtils)
	{
public:
	static TBool TypeUidIsForRegistrationFile(const TUidType& aUidType);
	};
#endif

/**
@internalComponent
*/
NONSHARABLE_CLASS(CApaIconLoader) : public CBase
	{
public:
	static CApaIconLoader* NewL(RFs& aFs);
	static CApaIconLoader* NewLC(RFs& aFs);
	~CApaIconLoader();

	void AddIconArrayL(const CApaAppIconArray& aIcons);
	void LoadAllIconsL();
	TBool LoadIconsL(TInt aNumOfIcons, const TDesC& aMbmFileName, CArrayPtr<CApaMaskedBitmap>& aIcons);
private:
	CApaIconLoader(RFs& aFs);
	void ConstructL();
	TInt IconIndexL(const TDesC& aFileName, TBool& aUseCache);
	static TInt CompareIcons(const CApaMaskedBitmap& aFirst, const CApaMaskedBitmap& aSecond);
private:
	struct TKeyValuePair
		{
	public:
		HBufC* iFileName; // key
		TInt iIndex; // value
		};
private:
	RFs& iFs;
	RArray<TKeyValuePair> iIconIndexArray;
	TInt iCachedArrayIndex;
	RPointerArray<CApaAppIconArray> iIconArrays;
	};

/**
@internalComponent
*/
NONSHARABLE_CLASS(CApaAppIconArray) : public CBase
	{
public:
	static CApaAppIconArray* NewL(); // creates a dummy icon array
	static CApaAppIconArray* NewAppIconsL(TInt aNumOfIcons, const TDesC& aMbmFileName, CApaIconLoader& aIconLoader);
	static CApaAppIconArray* NewViewIconsL(TInt aNumOfIcons, const TDesC& aMbmFileName, CApaIconLoader& aIconLoader);
	static CApaAppIconArray* NewDefaultIconsL(); // creates a CApaAppIconArray that uses the default icons
	static CApaAppIconArray* NewRealDefaultIconsLC(TInt aNumOfIcons, const TDesC& aMbmFileName, CApaIconLoader& aIconLoader); // creates the CApaAppIconArray that contains the default icons
	~CApaAppIconArray();

	TBool LoadIconsL();

	CApaMaskedBitmap* IconBySize(const TSize& aSize) const;
	CArrayFixFlat<TSize>* IconSizesL() const;
	TInt Count() const;
	TBool DefaultIconsUsed() const;
	CApaMaskedBitmap* operator[](TInt aIndex) const;

	TBool AreAppIconsLoaded() const;
	TBool AreViewIconsLoaded() const;
private:
	static CApaAppIconArray* NewL(TInt aNumOfIcons, const TDesC& aMbmFileName, CApaIconLoader& aIconLoader, TBool aFallbackToDefaultIcons);
	static CApaAppIconArray* NewLC(TInt aNumOfIcons, const TDesC& aMbmFileName, CApaIconLoader& aIconLoader, TBool aFallbackToDefaultIcons);
	CApaAppIconArray();
	CApaAppIconArray(TInt aNumOfIcons, CApaIconLoader& aIconLoader, TBool aFallbackToDefaultIcons);
	void ConstructL(const TDesC& aMbmFileName);
	void GetDefaultIconsL();
	const CApaAppIconArray& DefaultIcons() const;
private:
	TInt iNumOfIcons;
	CApaIconLoader* iIconLoader;
	TBool iFallbackToDefaultIcons;
	HBufC* iMbmFileName;
	TBool iDefaultIconsUsed;
	CArrayPtrFlat<CApaMaskedBitmap>* iIcons;
	CApaAppList* iDefaultIconsProvider;
	};

/**
Reads application information from a combination of registration file,
optional localisable resource file and optional icon file.

@internalComponent
*/
class CApaAppInfoReader : public CBase
	{
public:
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
    static CApaAppInfoReader* NewL(RFs& aFs, const Usif::CApplicationRegistrationData& aAppInfo, const Usif::RSoftwareComponentRegistry& aScr); 
#else
    static CApaAppInfoReader* NewL(RFs& aFs, const TDesC& aRegistrationFileName, TUid aAppUid);
    HBufC* LocalisableResourceFileName();
    TTime LocalisableResourceFileTimeStamp() const; 
    TTime TimeStamp() const;
    TTime IconFileTimeStamp() const;   
#endif
    TBool ReadL();    
    static TBool FileIsMbmWithGenericExtensionL(const TDesC& aFileName);
	~CApaAppInfoReader();
public:
	HBufC* AppBinaryFullName();
	TUidType AppBinaryUidType() const;
	void Capability(TDes8& aCapabilityBuf) const;
	TUint DefaultScreenNumber() const;
	CArrayPtrFlat<CApaAppViewData>* Views();
	CDesCArray* OwnedFiles();
	
	HBufC* Caption();
	HBufC* ShortCaption();
	TInt NumOfAppIcons() const;
	CApaAppIconArray* Icons();
	HBufC* IconFileName();
	TBool NonMbmIconFile() const;
	CApaIconLoader* IconLoader();
	TLanguage AppLanguage() const;
	CArrayFixFlat<TApaAppServiceInfo>* ServiceArray(TInt& aIndexOfFirstOpenService);
	HBufC8* OpaqueData();
	
private:
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
	CApaAppInfoReader(RFs& aFs, const Usif::CApplicationRegistrationData& aAppInfo, const Usif::RSoftwareComponentRegistry& aScr);
    void ReadAppRegistrationInfoL();
    void ReadServiceInfoL(const RPointerArray<Usif::CServiceInfo>& aServiceInfo);    
    void ReadOwnedFilesInfoL(const RPointerArray<HBufC>& aOwnedFiles);
    void ReadMimeTypesSupportedL(const RPointerArray<Usif::CDataType>& dataTypes, CArrayFixFlat<TDataTypeWithPriority>& aMimeTypesSupported);    
    void ReadLocalisationInfoL();
    void ReadViewInfoL(const RPointerArray<Usif::CAppViewData>& aViewData);    
#ifdef APPARC_SHOW_TRACE   
    void DisplayAppInfo();
#endif
    
#else
    CApaAppInfoReader(RFs& aFs, const TDesC& aRegistrationFileName, TUid aAppUid);
    void ReadMandatoryInfoL(RResourceReader& aResourceReader);
    void ReadNonLocalisableInfoL(RResourceReader& aResourceReader, CResourceFile*& aLocalisableResourceFile, TUint& aLocalisableResourceId);
    void ReadNonLocalisableOptionalInfoL(RResourceReader& aResourceReader, const CResourceFile* aRegistrationFile, CResourceFile* aLocalisableResourceFile);
    void ReadMimeTypesSupportedL(RResourceReader& aResourceReader, CArrayFixFlat<TDataTypeWithPriority>& aMimeTypesSupported);       
    void ReadLocalisableInfoL(const CResourceFile& aResourceFile, TUint aResourceId, TBool& aUseDefaultIcons);
    HBufC* CreateFullIconFileNameL(const TDesC& aIconFileName) const;    
#endif
	void ConstructL();
	TBool HasWriteDeviceDataCap();
    void ReadAppSecurityInfo();
	static HBufC8* ReadOpaqueDataL(TUint aResourceId, const CResourceFile* aRegistrationFile, CResourceFile* aLocalisableResourceFile);
private:
	RFs& iFs;
	TUid iAppUid;
	HBufC* iAppBinaryFullName;
	TUidType iAppBinaryUidType;
	TApaAppCapability iCapability;
	TUint iDefaultScreenNumber;
	HBufC* iCaption;
	HBufC* iShortCaption;
	CApaAppIconArray* iIcons;
	TInt iNumOfAppIcons;
	CArrayPtrFlat<CApaAppViewData>* iViewDataArray;
	CDesCArray* iOwnedFileArray;
	HBufC* iIconFileName;
	TBool iNonMbmIconFile; // ETrue if icon filename is not an MBM file, however, EFalse does not necessarily mean it is an MBM file
	TLanguage iApplicationLanguage;
	CArrayFixFlat<TApaAppServiceInfo>* iServiceArray;
    TInt iIndexOfFirstOpenService; 	
	HBufC8* iOpaqueData;
private:
	TBool iHasWriteDeviceDataCap;
    TBool iIsSidTrusted;
    // This flag is used to determine if app security info was allready read
    TBool iSecurityInfoHasBeenRead;
	CApaIconLoader* iIconLoader;
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
	const Usif::CApplicationRegistrationData& iAppInfo; //The ownership is not taken	
	const Usif::RSoftwareComponentRegistry& iScr; //The ownership is not taken	
#else
    const TDesC& iRegistrationFileName;
    TTime iTimeStamp;
    TTime iIconFileTimeStamp;   
    HBufC* iLocalisableResourceFileName;
    TTime iLocalisableResourceFileTimeStamp; 
    TBool iOpenServiceIsLegacy;   
#endif
	
	};


#endif	// __APLAPPINFOREADER_H__
