// Copyright (c) 1997-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// aplapplistitem.h
//

#ifndef __APLAPPLISTITEM_H__
#define __APLAPPLISTITEM_H__

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#if !defined(__APAID_PARTNER_H__)
#include "apaidpartner.h"
#endif
#include <apmfndr.h>
#else
#include <apmstd.h>
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS
#include <apaid.h>
#include <badesca.h>
#include <s32file.h>

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
#include<usif/scr/appregentries.h>
#include<usif/scr/scr.h>
#endif


 
// classes defined:
class CApaAppData;
class CApaAppList;
class CApaAppViewData;
class CApaMaskedBitmap;
class TEntry;
class RFs;
class CApaAppRegFinder;
class CApaAppIconArray;
class CApaIconLoader;



/** An application entry.

An object of this type contains two pieces of information:

the full path name of the application DLL

the UID type (or compound identifier) of the application DLL. 

@internalComponent
@released */
class TApaAppEntry
	{
public:
	IMPORT_C TApaAppEntry();
	TApaAppEntry(const TUidType& aAppUidType,const TFileName& aDllName);
	void ExternalizeL(RWriteStream& aStream)const;
	void InternalizeL(RReadStream& aStream);
public:
	/** The UID type (or compound identifier) of the application DLL. */
	TUidType iUidType;
	/** The full path name of the application DLL. */
	TFileName iFullName;
private:
	TInt iTApaAppEntry_Reserved1;
	};

/**
Utility class with smaller RAM footprint than TApaAppEntry

@internalComponent
*/
class CApaAppEntry : public CBase
	{
public:
	static CApaAppEntry* NewL(const TApaAppEntry& aAppEntry);
	~CApaAppEntry();
	void Get(TApaAppEntry& aAppEntry) const;
private:
	CApaAppEntry(const TUidType& aUidType);
	void ConstructL(const TDesC& aFileName);
private:
	TUidType iUidType;
	HBufC* iFullName;
	};

/** Represents the data associated with a CApaApplication, such as its 
caption, capabilities and icons. This should be accessed through the Apparc Server.

@internalComponent
*/
class CApaAppData : public CBase
	{
public:    
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
    IMPORT_C static CApaAppData* NewL(const Usif::CApplicationRegistrationData& aAppInfo, RFs& aFs, const Usif::RSoftwareComponentRegistry& aScrCon);
    IMPORT_C TBool IsLangChangePending();
#else
    IMPORT_C static CApaAppData* NewL(const TApaAppEntry& aAppEntry, RFs& aFs);
    inline TBool IsPresent() const;  
    IMPORT_C TBool RegistrationFileUsed() const;  
    IMPORT_C TPtrC RegistrationFileName() const;   
    IMPORT_C TBool IsPending()const;
    IMPORT_C TPtrC LocalisableResourceFileName() const;    
#endif

	IMPORT_C ~CApaAppData();
	IMPORT_C TApaAppEntry AppEntry() const;
	inline TPtrC Caption() const;
	inline TPtrC ShortCaption() const;
	IMPORT_C CApaMaskedBitmap* Icon(TInt aIconIndex) const;
	IMPORT_C void Capability(TDes8& aCapabilityBuf)const;
	// ER5
	IMPORT_C TDataTypePriority DataType(const TDataType& aDataType) const;
	// ER6
	IMPORT_C CApaMaskedBitmap* Icon(TSize aSize) const;
	IMPORT_C CArrayFixFlat<TSize>* IconSizesL() const;
	// ER6.1
	IMPORT_C CArrayPtrFlat<CApaAppViewData>* Views() const;
	IMPORT_C CDesCArray* OwnedFiles() const;
	IMPORT_C TBool CanUseScreenMode(TInt aScreenMode);
	// 7.0s
	IMPORT_C void GetIconInfo(TInt& aIconCount, TInt& aDefaultIconsUsed) const;
	// 8.1
	IMPORT_C TUint DefaultScreenNumber() const;
	IMPORT_C TPtrC IconFileName() const;
	IMPORT_C TBool NonMbmIconFile() const;
	// 9.0
	IMPORT_C TBool ImplementsService(TUid aServiceUid) const;
	TInt ImplementsServiceWithDataType(TUid aServiceUid, const TDataType& aDataType) const;

	// 9.1
	IMPORT_C TLanguage ApplicationLanguage() const;
	IMPORT_C TPtrC8 OpaqueData() const;
    IMPORT_C TUid NonNativeApplicationType() const;
	IMPORT_C void SetShortCaptionL(const TDesC& aShortCaption);
	// 9.5
	IMPORT_C void SetCaptionL(const TDesC& aCaption);
	IMPORT_C void SetIconsL(const TDesC& aFileName, TInt aNumIcons);

	void ExternalizeL(RWriteStream& aWriteStream) const;	
	void LoadIconsL();
	TBool MbmIconsRequireLoading() const;
	inline CApaAppData* Next() const;
private:
	CApaAppData(RFs& aFs);
	void UpdateServiceArray(CArrayFixFlat<TApaAppServiceInfo>* aNewServiceArray);
	TDataTypePriority DataType(const TDataType& aDataType, const CArrayFixFlat<TDataTypeWithPriority>& aDataTypeArray) const;
	void InternalizeL(RReadStream& aReadStream);
	TBool ViewMbmIconsRequireLoading() const;

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
    void ConstructL(const Usif::CApplicationRegistrationData& aAppInfo, const Usif::RSoftwareComponentRegistry& aScrCon);	
    TInt ReadApplicationInformationFromSCRL(const Usif::CApplicationRegistrationData& aAppInfo, const Usif::RSoftwareComponentRegistry& aScrCon);
#else
    TBool Update();
    void SetAppPending();
    void ConstructL(const TApaAppEntry& aAppEntry);	
    TInt ReadApplicationInformationFromResourceFiles();
#endif
	
private:
	enum { ENotPresent, ENotPresentPendingUpdate, EPresentPendingUpdate, EIsPresent, ESuperseded };
private:
	CApaAppIconArray* iIcons;
	HBufC* iCaption;
	HBufC* iShortCaption;
	HBufC* iFullName; // filename of application binary
	TUid iUid;
    TUidType iUidType;  	
	CApaAppData* iNext;
	TApaAppCapabilityBuf iCapabilityBuf;
	CApaAppEntry* iSuccessor;
	CArrayPtrFlat<CApaAppViewData>* iViewDataArray;
	CDesCArray* iOwnedFileArray;
 	RFs& iFs;
 	TUint iDefaultScreenNumber;
 	HBufC* iIconFileName;
 	TBool iNonMbmIconFile;
 	TLanguage iApplicationLanguage;
 	CArrayFixFlat<TApaAppServiceInfo>* iServiceArray;
 	TInt iIndexOfFirstOpenService;
	HBufC8* iOpaqueData;
 	TInt iNumOfAppIcons;
	TInt iNumOfAppIconsFromResourceFile;
 	HBufC* iIconFileNameFromResourceFile; // Icon file name as found in the localisable resource file
 	TBool iNonMbmIconFileFromResourceFile; // A Flag that tells whether the icon in resource file is non MBM file format
	HBufC* iShortCaptionFromResourceFile;	// Short Caption as found in the localisable resource file
	HBufC* iCaptionFromResourceFile;		// Caption as found in the localisable resource file
	CApaIconLoader* iIconLoader;
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
    TInt iIsPresent; // uses enum	
    TTime iTimeStamp;  
    HBufC* iRegistrationFile; 
    HBufC* iLocalisableResourceFileName;
    TTime iLocalisableResourceFileTimeStamp;
    TTime iIconFileTimeStamp;    
    TUid iNonNativeApplicationType;  
    TTime iIconFileTimeStampFromResourceFile;
#endif

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
    TBool iIsLangChangePending;
#endif
    
private:
	friend class CApaAppList;
	};

/**
The CApaAppViewData class represents the data associated with an application view.

@internalComponent
*/
class CApaAppViewData : public CBase
	{
public:
	IMPORT_C TUid Uid() const;
	inline TPtrC Caption() const;
	IMPORT_C CApaMaskedBitmap* Icon(const TSize& aSize) const;
	IMPORT_C CArrayFixFlat<TSize>* IconSizesL() const;
	IMPORT_C TInt ScreenMode() const;
	IMPORT_C TPtrC IconFileName() const;
	IMPORT_C TBool NonMbmIconFile() const;
public:
	~CApaAppViewData();
	static CApaAppViewData* NewLC();
	void SetUid(TUid aUid);
	void SetCaptionL(const TDesC& aCaption);
	void SetIconArray(CApaAppIconArray* aIcons);
	void SetScreenMode(TInt aScreenMode);
	void SetIconFileNameL(const TDesC& aFileName);
	void SetNonMbmIconFile(TBool aNonMbmIconFile);
	void SetNumOfViewIcons(TInt aNumOfViewIcons);
private:
	CApaAppViewData();
	void ConstructL();
private:
	CApaAppIconArray* iIcons;
	HBufC* iCaption;
	TUid iUid;
	TInt iScreenMode;
	HBufC* iIconFileName;
	TBool iNonMbmIconFile;
	TInt iNumOfViewIcons;
	friend class CApaAppData;
	};

//
// inlines
//

inline TPtrC CApaAppViewData::Caption() const
	{ return *iCaption; }

/** Gets the application's caption.

@return The application's caption. */
inline TPtrC CApaAppData::Caption() const
	{ return *iCaption; }

/** Gets the application's short caption.

@return The application's short caption. */
inline TPtrC CApaAppData::ShortCaption() const
	{ return *iShortCaption; }

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
/** Tests whether the application is present or not on the device.

@return True if application exists, else false. */
inline TBool CApaAppData::IsPresent() const
	{ return iIsPresent; }
#endif

/** Gets the Next Appdata in the list

@return iNext */
inline CApaAppData* CApaAppData::Next() const
	{ return iNext; }

#endif //__APLAPPLISTITEM_H__
