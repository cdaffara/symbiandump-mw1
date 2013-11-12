// Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// aplapplistitem.cpp
//

#include "aplapplistitem.h"
#include "APGAPLST.H"				// KApaIconIndexSmall, KApaIconIndexMedium, KApaIconIndexLarge
#include "../apgrfx/APGSTD.H" 
#include "APFDEF.H"					// KAppResourceFileExtension
#include "../apparc/TRACE.H"
#include <bautils.h>
#include "../apgrfx/APGPRIV.H"		// KLitPathForNonNativeResourceAndIconFiles
#include "../apgrfx/apprivate.h"	// KLitPathForNonNativeResourceAndIconFiles
#include "aplappinforeader.h"
#include <e32uid.h>

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
#include <usif/scr/appregentries.h>
#endif

// Delays in the pseudo idle object that builds the application list
//

//
// Local functions
//

extern void CleanupServiceArray(TAny* aServiceArray);	// Implemented in AplAppList.cpp


//
// class TApaAppEntry
//

/** Constructs an empty application entry object.

The full path name is empty, and the triplet of UIDs forming the UID type 
are set to null UIDs. */
EXPORT_C TApaAppEntry::TApaAppEntry()
	: iUidType(TUidType()),
	iFullName(KNullDesC)
	{}


/** Constructs an application entry object from the specified application 
DLL full path name and UID type. 
@param aAppUidType UID type.
@param aDllName Application DLL full path name. */
TApaAppEntry::TApaAppEntry(const TUidType& aAppUidType,const TFileName& aDllName)
	: iUidType(aAppUidType),
	iFullName(aDllName)
	{}


/** Externalises the application entry to a write stream.

@param aStream The write stream. */
void TApaAppEntry::ExternalizeL(RWriteStream& aStream)const
	{
	aStream<< iUidType[0];
	aStream<< iUidType[1];
	aStream<< iUidType[2];
	aStream<< iFullName;
	}


/** Internalises the application entry from a read stream.

@param aStream The read stream. */
void TApaAppEntry::InternalizeL(RReadStream& aStream)
	{
	TUid uid1;
	TUid uid2;
	TUid uid3;
	aStream>> uid1;
	aStream>> uid2;
	aStream>> uid3;
	iUidType = TUidType(uid1,uid2,uid3);
	aStream>> iFullName;
	}

//
// Class CApaAppViewData
//

CApaAppViewData::~CApaAppViewData()
	{
	delete iIcons;
	delete iCaption;
	delete iIconFileName;
	}

CApaAppViewData::CApaAppViewData()
	: iNonMbmIconFile(EFalse)
	{
	}

void CApaAppViewData::ConstructL()
	{
	iIcons=CApaAppIconArray::NewL();
	}

CApaAppViewData* CApaAppViewData::NewLC()
	{
	CApaAppViewData* self=new(ELeave) CApaAppViewData();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;	
	}

void CApaAppViewData::SetUid(TUid aUid)
	{
	iUid=aUid;
	}

void CApaAppViewData::SetScreenMode(TInt aScreenMode)
	{
	iScreenMode=aScreenMode;
	}

EXPORT_C TInt CApaAppViewData::ScreenMode() const
	{
	return iScreenMode;
	}

void CApaAppViewData::SetCaptionL(const TDesC& aCaption)
	{
	HBufC* newCaption=aCaption.AllocL();
	delete(iCaption); // after the AllocL succeeds
	iCaption=newCaption;
	}

void CApaAppViewData::SetIconArray(CApaAppIconArray* aIcons)
	{
	delete iIcons;
	iIcons = aIcons;
	}

void CApaAppViewData::SetIconFileNameL(const TDesC& aFileName)
	{
	HBufC* fileName = aFileName.AllocL();
	delete iIconFileName; // after the AllocL succeeds
	iIconFileName = fileName;
	}

void CApaAppViewData::SetNumOfViewIcons(TInt aNumOfViewIcons)
	{
	iNumOfViewIcons = aNumOfViewIcons;
	}

void CApaAppViewData::SetNonMbmIconFile(TBool aNonMbmIconFile)
	{
	iNonMbmIconFile = aNonMbmIconFile;
	}

EXPORT_C TUid CApaAppViewData::Uid() const
	{
	return iUid;
	}

EXPORT_C CApaMaskedBitmap* CApaAppViewData::Icon(const TSize& aSize) const
	{
	return iIcons->IconBySize(aSize);
	}

EXPORT_C CArrayFixFlat<TSize>* CApaAppViewData::IconSizesL() const
	{
	return iIcons->IconSizesL();
	}

EXPORT_C TPtrC CApaAppViewData::IconFileName() const
	{
	if (iIconFileName)
		{
		return *iIconFileName;
		}
	else
		{
		return TPtrC(KNullDesC);
		}
	}

EXPORT_C TBool CApaAppViewData::NonMbmIconFile() const
	{
	return iNonMbmIconFile;
	}


//
// class CApaAppEntry
//

CApaAppEntry* CApaAppEntry::NewL(const TApaAppEntry& aAppEntry)
	{ // static
	CApaAppEntry* self=new(ELeave) CApaAppEntry(aAppEntry.iUidType);
	CleanupStack::PushL(self);
	self->ConstructL(aAppEntry.iFullName);
	CleanupStack::Pop(self);
	return self;
	}

CApaAppEntry::~CApaAppEntry()
	{
	delete iFullName;
	}

void CApaAppEntry::Get(TApaAppEntry& aAppEntry) const
	{
	aAppEntry.iFullName=*iFullName;
	aAppEntry.iUidType=iUidType;
	}

CApaAppEntry::CApaAppEntry(const TUidType& aUidType)
	: iUidType(aUidType)
	{
	}

void CApaAppEntry::ConstructL(const TDesC& aFileName)
	{
	iFullName=aFileName.AllocL();
	}


//
// Class CApaAppData
//

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
CApaAppData::CApaAppData(RFs& aFs)
    :iCaption(NULL), iShortCaption(NULL), 
    iFs(aFs),
    iNonMbmIconFile(EFalse),
    iApplicationLanguage(ELangNone), iIndexOfFirstOpenService(-1),
    iShortCaptionFromResourceFile(NULL)
    {
    }
EXPORT_C CApaAppData* CApaAppData::NewL(const Usif::CApplicationRegistrationData& aAppInfo, RFs& aFs, const Usif::RSoftwareComponentRegistry& aScrCon)
    {
    CApaAppData* self=new(ELeave) CApaAppData(aFs);
    CleanupStack::PushL(self);
    self->ConstructL(aAppInfo, aScrCon);
    CleanupStack::Pop(self); // self
    return self;
    }

void CApaAppData::ConstructL(const Usif::CApplicationRegistrationData& aAppInfo, const Usif::RSoftwareComponentRegistry& aScrCon)
    {
    iCapabilityBuf.FillZ(iCapabilityBuf.MaxLength());
    iIcons = CApaAppIconArray::NewL();
    iViewDataArray=new(ELeave) CArrayPtrFlat<CApaAppViewData>(1);
    iOwnedFileArray=new(ELeave) CDesCArraySeg(1);
    User::LeaveIfError(ReadApplicationInformationFromSCRL(aAppInfo, aScrCon));   
    }

//Initializes the CApaAppData object with information read from SCR. Leaves if any error occurs during initialization.

TInt CApaAppData::ReadApplicationInformationFromSCRL(const Usif::CApplicationRegistrationData& aAppInfo, const Usif::RSoftwareComponentRegistry& aScrCon)
    {
    HBufC* caption = NULL;
    HBufC* shortCaption = NULL;

    CApaAppInfoReader* appInfoReader = NULL;
    appInfoReader = CApaAppInfoReader::NewL(iFs, aAppInfo, aScrCon); 
    CleanupStack::PushL(appInfoReader);
    TBool readSuccessful=EFalse;
    readSuccessful= appInfoReader->ReadL();
    
    iFullName=appInfoReader->AppBinaryFullName();
    
    iUidType = appInfoReader->AppBinaryUidType();

    caption = appInfoReader->Caption();
    shortCaption = appInfoReader->ShortCaption();

    CApaAppIconArray* icons = appInfoReader->Icons();
    if(icons)
        {
        delete iIcons;
        iIcons = icons;
        iIconLoader = appInfoReader->IconLoader();
        }
    else
        {           
        TRAPD(err, icons = CApaAppIconArray::NewL());
        if(err == KErrNone)
            {
            delete iIcons;
            iIcons = icons;
            }
        }
        
               
    iOpaqueData = appInfoReader->OpaqueData();

    if (readSuccessful)
        {
        appInfoReader->Capability(iCapabilityBuf);

        iDefaultScreenNumber = appInfoReader->DefaultScreenNumber();

        delete iIconFileName;
        iIconFileName = appInfoReader->IconFileName();
        iNonMbmIconFile = appInfoReader->NonMbmIconFile();
        iNumOfAppIcons = appInfoReader->NumOfAppIcons();
        iApplicationLanguage = appInfoReader->AppLanguage();
                
        // views
        iViewDataArray->ResetAndDestroy();
        CArrayPtrFlat<CApaAppViewData>* viewDataArray = appInfoReader->Views();
        if (viewDataArray)
            {
            delete iViewDataArray;
            iViewDataArray = viewDataArray;
            
            if(!iIconLoader && ViewMbmIconsRequireLoading())
                {
                //if VIEW_DATA contains a MBM icon we need to initialize iIconLoader
                iIconLoader = appInfoReader->IconLoader();
                }
            }

            // owned files
            iOwnedFileArray->Reset();
            CDesCArray* const ownedFileArray = appInfoReader->OwnedFiles();
            if (ownedFileArray)
                {
                delete iOwnedFileArray;
                iOwnedFileArray = ownedFileArray;
                }
            
            UpdateServiceArray(appInfoReader->ServiceArray(iIndexOfFirstOpenService));
            }

    CleanupStack::PopAndDestroy(appInfoReader);

    if (!caption)
        {
        TParsePtrC parse (*iFullName);
        caption = parse.Name().Alloc();
        }

    // Put the captions into place
    if (caption)
        {
        if (!shortCaption)
            {
            shortCaption = caption->Alloc();
            if (!shortCaption)
                {
                delete caption;
                caption = NULL;
                }
            }

        delete iCaption;
        iCaption = caption;
        delete iShortCaption;
        iShortCaption = shortCaption;
        }
    
    return caption ? KErrNone : KErrNoMemory;
    }


EXPORT_C TUid CApaAppData::NonNativeApplicationType() const
/** @internalComponent */
    {
    if (iCapabilityBuf().iAttributes & TApaAppCapability::ENonNative)
        return iUidType[1];
    else
        return TUid::Null();
    }

EXPORT_C TBool CApaAppData::IsLangChangePending()
{
    return iIsLangChangePending;
}

#else
CApaAppData::CApaAppData(RFs& aFs)
    :iCaption(NULL), iShortCaption(NULL), 
    iIsPresent(CApaAppData::EIsPresent), iFs(aFs),
    iNonMbmIconFile(EFalse),
    iApplicationLanguage(ELangNone), iIndexOfFirstOpenService(-1),
    iNonNativeApplicationType(TUid::Null()),
    iShortCaptionFromResourceFile(NULL)
    {
    }

EXPORT_C CApaAppData* CApaAppData::NewL(const TApaAppEntry& aAppEntry, RFs& aFs)
    {
    CApaAppData* self=new(ELeave) CApaAppData(aFs);
    CleanupStack::PushL(self);
    self->ConstructL(aAppEntry);
    CleanupStack::Pop(); // self
    return self;
    }

void CApaAppData::ConstructL(const TApaAppEntry& aAppEntry)
    {
    iUidType = aAppEntry.iUidType; // if the 2nd UID is KUidAppRegistrationFile, iUidType will be updated in ReadApplicationInformationFromResourceFiles() to reflect the TUidType for the application binary

    if (ApaUtils::TypeUidIsForRegistrationFile(aAppEntry.iUidType))
        {
        iRegistrationFile = aAppEntry.iFullName.AllocL();
        }
    else
        {
        iFullName = aAppEntry.iFullName.AllocL();
        }
    
    iCapabilityBuf.FillZ(iCapabilityBuf.MaxLength());
    iIcons = CApaAppIconArray::NewL();
    iViewDataArray=new(ELeave) CArrayPtrFlat<CApaAppViewData>(1);
    iOwnedFileArray=new(ELeave) CDesCArraySeg(1);
    User::LeaveIfError(ReadApplicationInformationFromResourceFiles());    
    }

// Return a standard error code
// The value returned only reflect the caption status
// If there is errors setting up captions the old values are retained
// All other errors are silently ignored
// General notes:
// 1. This method is deliberately very similar to the old CApaAppData::GetAifData
//    in order to maintain behavioural compatibility for V1 apps
// 2. Be very careful in this method, because it can be called on a newly constructed object,
//    or on an existing object, so don't assume member data pointers will be NULL
TInt CApaAppData::ReadApplicationInformationFromResourceFiles()
    {
    HBufC* caption = NULL;
    HBufC* shortCaption = NULL;

    iTimeStamp = TTime(0); // cannot init in constructor because this function can be called on an existing CApaAppData object

    if(iRegistrationFile)
        {
        CApaAppInfoReader* appInfoReader = NULL;
        TRAP_IGNORE(appInfoReader = CApaAppInfoReader::NewL(iFs, *iRegistrationFile, iUidType[2])); 
        if (!appInfoReader)
            {
            if (!iFullName)
                {
                // assume that if iFullName is NULL, this method has been called as part
                // of constructing a new app data object. The CApaAppInfoReader derived object
                // could not be created, therefore we have no way to determine the full filename
                // of the app binary, so give up
                return KErrNoMemory;
                }
            }
        else
            {
            TBool readSuccessful=EFalse;
            TRAP_IGNORE(readSuccessful= appInfoReader->ReadL());

            HBufC* const appBinaryFullName = appInfoReader->AppBinaryFullName();
            if (appBinaryFullName)
                {
                delete iFullName;
                iFullName = appBinaryFullName;
                }
                
            if (!iFullName)
                {
                delete appInfoReader;
                return KErrNoMemory;
                }
                
            // if this object has just been constructed, iUidType is currently the TUidType
            // of the registration file, it should be the TUidType of the app binary file
            TUidType uidType = appInfoReader->AppBinaryUidType();
            if (uidType[1].iUid != KNullUid.iUid)
                iUidType = uidType;

            // must get captions regardless of value of readSuccessful,
            // because the V1 reader might have read captions
            // this is done to maintain behavioural compatibility with V1
            caption = appInfoReader->Caption();
            shortCaption = appInfoReader->ShortCaption();

            CApaAppIconArray* icons = appInfoReader->Icons();
            if(icons)
                {
                delete iIcons;
                iIcons = icons;
                iIconLoader = appInfoReader->IconLoader();
                }
            else
                {           
                TRAPD(err, icons = CApaAppIconArray::NewL());
                if(err == KErrNone)
                    {
                    delete iIcons;
                    iIcons = icons;
                    }
                }
                
            iTimeStamp = appInfoReader->TimeStamp();
            delete iLocalisableResourceFileName;
            iLocalisableResourceFileName = appInfoReader->LocalisableResourceFileName();
            iLocalisableResourceFileTimeStamp = appInfoReader->LocalisableResourceFileTimeStamp();

            const TBool isNonNativeApp = 
                (TParsePtrC(*iRegistrationFile).Path().CompareF(KLitPathForNonNativeResourceAndIconFiles) == 0);
                
            if (isNonNativeApp)
                {
                // In the case of a non-native app, the resource file has been prefixed with a
                // TCheckedUid, the second of whose UIDs is the non-native application type uid.
                TEntry entry;
                const TInt error=iFs.Entry(*iRegistrationFile, entry);
                if (error!=KErrNone)
                    {
                    delete appInfoReader;
                    return error;
                    }
                    
                __ASSERT_DEBUG(entry.iType[0].iUid==KUidPrefixedNonNativeRegistrationResourceFile, Panic(EPanicUnexpectedUid));
                iNonNativeApplicationType=entry.iType[1];
                }

            delete iOpaqueData;
            iOpaqueData = appInfoReader->OpaqueData();

            if (readSuccessful)
                {
                appInfoReader->Capability(iCapabilityBuf);

                iDefaultScreenNumber = appInfoReader->DefaultScreenNumber();

                delete iIconFileName;
                iIconFileName = appInfoReader->IconFileName();
                iIconFileTimeStamp = appInfoReader->IconFileTimeStamp();
                iNonMbmIconFile = appInfoReader->NonMbmIconFile();
                iNumOfAppIcons = appInfoReader->NumOfAppIcons();
                iApplicationLanguage = appInfoReader->AppLanguage();
                        
                // views
                iViewDataArray->ResetAndDestroy();
                CArrayPtrFlat<CApaAppViewData>* viewDataArray = appInfoReader->Views();
                if (viewDataArray)
                    {
                    delete iViewDataArray;
                    iViewDataArray = viewDataArray;
                    
                    if(!iIconLoader && ViewMbmIconsRequireLoading())
                        {
                        //if VIEW_DATA contains a MBM icon we need to initialize iIconLoader
                        iIconLoader = appInfoReader->IconLoader();
                        }
                    }

                // owned files
                iOwnedFileArray->Reset();
                CDesCArray* const ownedFileArray = appInfoReader->OwnedFiles();
                if (ownedFileArray)
                    {
                    delete iOwnedFileArray;
                    iOwnedFileArray = ownedFileArray;
                    }
                
                UpdateServiceArray(appInfoReader->ServiceArray(iIndexOfFirstOpenService));
                }

            delete appInfoReader;
            }
        }
        
    if (!caption)
        {
        TParsePtrC parse (*iFullName);
        caption = parse.Name().Alloc();
        }

    // Put the captions into place
    if (caption)
        {
        if (!shortCaption)
            {
            shortCaption = caption->Alloc();
            if (!shortCaption)
                {
                delete caption;
                caption = NULL;
                }
            }

        delete iCaption;
        iCaption = caption;
        delete iShortCaption;
        iShortCaption = shortCaption;
        }

    return caption ? KErrNone : KErrNoMemory;
    }


/** Returns true if app info was provided by a registration file

@return true if app info was provided by a registration file
*/
EXPORT_C TBool CApaAppData::RegistrationFileUsed() const
    {
    return iRegistrationFile != NULL;
    }

/** Returns the full filename of the registration resource file

@return The full path and filename of the registration resource file.
@internalTechnology
*/
EXPORT_C TPtrC CApaAppData::RegistrationFileName() const
    {
    if (iRegistrationFile)
        {
        return *iRegistrationFile;
        }
    else
        {
        return TPtrC(KNullDesC);
        }
    }


/** Returns the full filename of the localisable resource file

@return The full path and filename of the localisable resource file.
@internalTechnology
*/
EXPORT_C TPtrC CApaAppData::LocalisableResourceFileName() const
    {
    if (iLocalisableResourceFileName)
        {
        return *iLocalisableResourceFileName;
        }
    else
        {
        return TPtrC(KNullDesC);
        }
    }


TBool CApaAppData::Update()
// returns true if changes were made to the cached data
    {
    __APA_PROFILE_START(17);
    TBool changed=EFalse;

    // Get app info file entry
    TEntry entry;
    TInt ret;
    if (iRegistrationFile != NULL)
        {
        ret = iFs.Entry(*iRegistrationFile, entry);
        if (ret==KErrNone && entry.iModified!=iTimeStamp)
            {
            // assume registration file may have changed
            changed = ETrue;
            }
        else
            {
            if (iLocalisableResourceFileName)
                {
                // see if localisable resource information might have changed
                TParse parse;
                ret = parse.SetNoWild(KAppResourceFileExtension, iLocalisableResourceFileName, NULL);
                if (ret == KErrNone)
                    {
                    TFileName resourceFileName(parse.FullName());
                    TLanguage language;
                    BaflUtils::NearestLanguageFileV2(iFs, resourceFileName, language);
                    (void)language;
                    if (resourceFileName.CompareF(*iLocalisableResourceFileName)!=0)
                        {
                        changed = ETrue;
                        }
                    else
                        {
                        ret = iFs.Entry(*iLocalisableResourceFileName, entry);
                        if ((ret==KErrNotFound && iLocalisableResourceFileTimeStamp!=TTime(0)) ||
                            (ret==KErrNone && entry.iModified!=iLocalisableResourceFileTimeStamp))
                            {
                            changed = ETrue;
                            }
                        }
                    }
                }
            }
        }
    if (changed)
        {
        // re-read data
        // Ignore result, nothing we can do in case failure
        // and the old values should be preserved
        const TInt ignore = ReadApplicationInformationFromResourceFiles();
        } //lint !e529 Symbol 'ignore' not subsequently referenced
        
    else 
        {
        if (iIconFileName)
            {
            ret = iFs.Entry(*iIconFileName, entry);
            // See if the icon file has been "modified"
            // It could have been replaced with a differnt version, deleted or added 
            // if the icon file specified in the resource was missing
            if ((ret==KErrNotFound && iIconFileTimeStamp!=TTime(0)) ||
                    (ret==KErrNone && entry.iModified!=iIconFileTimeStamp))
                    {
                    // Assume the icon file has changed
                    iIconFileTimeStamp = entry.iModified;
                    changed = ETrue;
                    }
            }
        }

    __APA_PROFILE_END(17);
    return changed;
    }

EXPORT_C TBool CApaAppData::IsPending() const
/* Returns true if the app info is not yet updated by the current scan. */
    {
    return (iIsPresent==CApaAppData::EPresentPendingUpdate 
        || iIsPresent==CApaAppData::ENotPresentPendingUpdate);
    }

EXPORT_C TUid CApaAppData::NonNativeApplicationType() const
/** @internalComponent */
    {
    return iNonNativeApplicationType;
    }

void CApaAppData::SetAppPending()
    {
    if (iIsPresent == CApaAppData::ENotPresent 
        || iIsPresent == CApaAppData::ENotPresentPendingUpdate)
        {
        iIsPresent = CApaAppData::ENotPresentPendingUpdate;
        }
    else
        {
        iIsPresent = CApaAppData::EPresentPendingUpdate;
        }
    }
#endif

EXPORT_C TApaAppEntry CApaAppData::AppEntry() const
/** Constructs an application entry based on this object.

@return The application entry. */
    {
    return TApaAppEntry(iUidType,*iFullName);
    }

EXPORT_C CApaAppData::~CApaAppData()
// Just delete components, NOT iNext (next CApaAppData in the list).
	{
	delete iSuccessor;
	delete iCaption;
	delete iShortCaption;
	delete iFullName;
	delete iShortCaptionFromResourceFile;
	delete iIcons;
	delete iIconLoader;
	if(iViewDataArray)
		{
		iViewDataArray->ResetAndDestroy();
		delete iViewDataArray;
		}
	delete iOwnedFileArray;
	delete iIconFileName;
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
	delete iLocalisableResourceFileName;
    delete iRegistrationFile;	
#endif	
	if (iServiceArray)
		{
		CleanupServiceArray(iServiceArray);
		iServiceArray = NULL;
		}
	delete iOpaqueData;
	iNext = NULL;
	}


void CApaAppData::UpdateServiceArray(CArrayFixFlat<TApaAppServiceInfo>* aNewServiceArray)
	{
	// clear out any existing service info
	if (iServiceArray)
		{
		CleanupServiceArray(iServiceArray);
		iServiceArray = NULL;
		}
	// store new service array
	iServiceArray = aNewServiceArray;
	}
	
TDataTypePriority CApaAppData::DataType(const TDataType& aDataType, const CArrayFixFlat<TDataTypeWithPriority>& aDataTypeArray) const
	{
	TInt count=aDataTypeArray.Count();
	for (TInt ii=0;ii<count;ii++)
		{
		const TDataTypeWithPriority& type=aDataTypeArray[ii];
		if (type.iDataType==aDataType)
			{
			return type.iPriority;
			}
		else
			{
			TPtrC8 src=type.iDataType.Des8();
			TPtrC8 trg=aDataType.Des8();
			if (src.Match(trg)==0 || trg.Match(src)==0)
				{
				if (type.iPriority == KDataTypePrioritySystem)
					{
					// This is more or less a magic number so don't decrement
					return KDataTypePrioritySystem;
					}
				else
					{
					return (TInt16)(type.iPriority-1);
					}
				}
			}
		}
	return KDataTypePriorityNotSupported;
	}

/**
 * Returns the CApaMaskedBitmap of size aSize for the application associated
 * with this CApaAppData. If the icons for the application are not yet loaded then it would be loaded first.
 * If there is not a bitmap of exact size aSize then 
 * the icon closest to but smaller than the one asked for is returned, or NULL if
 * none is smaller.
 * 
 * @since Uikon1.2
 */
EXPORT_C CApaMaskedBitmap* CApaAppData::Icon(TSize aSize) const
	{
	return iIcons->IconBySize(aSize);
	}

/**
 * Returns a pointer to the small, medium or large application icon for aIconIndex equal to 0, 1 or 2 respectively.
 * Panics if aIconIndex is not one of these three values.
 *
 * This method is superseded by an overload which returns the icon by finding the closest match to a specified size.
 *
 * @deprecated
 */
EXPORT_C CApaMaskedBitmap* CApaAppData::Icon(TInt aIconIndex) const
	{
	__ASSERT_DEBUG(aIconIndex>-1 && aIconIndex<3, Panic(EDPanicBadIconSize)); //only support old behaviour
	TSize sizeExpected;
	switch(aIconIndex)
		{
	case KApaIconIndexSmall:
		sizeExpected=TSize(24,24);
		break;
	case KApaIconIndexMedium:
		sizeExpected=TSize(32,32);
		break;
	case KApaIconIndexLarge:
		sizeExpected=TSize(48,48);
		break;
	default:
		break;
		}
	return Icon(sizeExpected);
	}

void CApaAppData::LoadIconsL()
	{
	iIconLoader->LoadAllIconsL();
	}

EXPORT_C CArrayFixFlat<TSize>* CApaAppData::IconSizesL()const
/** Gets the sizes of icons available for the application. 
* If the icons for the application are not yet loaded then it would be loaded first.

@return A pointer to an array of the icon sizes. The caller takes ownership. */
	{
	return iIcons->IconSizesL();
	}


EXPORT_C void CApaAppData::Capability(TDes8& aCapabilityBuf)const
/** Gets the application's capabilities.

@param aCapabilityBuf On return, contains the application's capabilities, 
specified as a TApaAppCapabilityBuf object. */
	{
	TApaAppCapability::CopyCapability(aCapabilityBuf,iCapabilityBuf);
	}

/**
 * Returns a pointer to the array of view data objects current for this application. Does not imply transfer of ownership.
 *
 * @since App-Framework_6.1
 */
EXPORT_C CArrayPtrFlat<CApaAppViewData>* CApaAppData::Views() const
	{
	return iViewDataArray;
	}

/**
 * Returns a pointer to the array of files for which this application claims ownership. Does not imply transfer of ownership.
 *
 * @since App-Framework_6.1
 */
EXPORT_C CDesCArray* CApaAppData::OwnedFiles() const
	{
	return iOwnedFileArray;
	}


EXPORT_C TDataTypePriority CApaAppData::DataType(const TDataType& aDataType) const
// returns the priority of the data type
/** If the application supports the specified data type, the function returns 
the priority with which it should be selected for handling it.

If the application does not support the specified data type, 
KDataTypePriorityNotSupported is returned.

Note that the function supports wildcard matching, using "*" and "?". In the case 
of a wildcard match, the priority value returned is reduced by 1, so that in this 
case, the application could never have the maximum priority 
(KDataTypePriorityUserSpecified).

@param aDataType The data type of interest.
@return The priority with which the application should be selected for handling 
the specified data type, or KDataTypePriorityNotSupported if the data type is 
not supported. */
	{
	if (iIndexOfFirstOpenService >= 0)
		{
		const CArrayFixFlat<TDataTypeWithPriority>& dataTypeArray = 
			(*iServiceArray)[iIndexOfFirstOpenService].DataTypes();
		return DataType(aDataType, dataTypeArray);
		}
	return KDataTypePriorityNotSupported;
	}



EXPORT_C TBool CApaAppData::CanUseScreenMode(TInt aScreenMode)
/** Tests whether the specified screen mode is valid for any of 
this application's views. If the app has no views, the function 
assumes that only the default screen mode (at screen mode index 
zero) is allowed. This function is used by CApaAppList to create 
a list of valid applications.

@param aScreenMode The index of the screen mode.
@return True if screen mode is valid, otherwise false. */
	{
	const TInt count=iViewDataArray->Count();
	// If there are no views, assume only the default screen mode is allowed
	TBool ret=(count==0 && aScreenMode==0);
	for(TInt ii=0;ii<count;ii++)
		{
		const CApaAppViewData* data=(*iViewDataArray)[ii];
		if(data->ScreenMode()==aScreenMode)
			{
			ret=ETrue;
			break;
			}
		}
	return ret;
	}

EXPORT_C void CApaAppData::GetIconInfo(TInt& aIconCount, TInt& aDefaultIconsUsed) const
/** Gets icon information for the app. If the icons for the application are not yet loaded then it would be loaded first.

@param aIconCount On return, this contains the number of app icons
@param aDefaultIconsUsed On return, this indicates whether the default icons have been used
*/
	{
	aIconCount = iIcons->Count();
	aDefaultIconsUsed = iIcons->DefaultIconsUsed();
	}

/** Gets the default screen number used by the application.

A device may have more than once screen. This function
returns the number associated with the screen which will
be the default screen used by the application.

@return The default screen number
*/
EXPORT_C TUint CApaAppData::DefaultScreenNumber() const
	{
	return iDefaultScreenNumber;
	}

/** Returns the non-native application opaque data

@return The non-native application opaque data.
@internalComponent
*/
EXPORT_C TPtrC8 CApaAppData::OpaqueData() const
	{
	if (iOpaqueData)
		{
		return *iOpaqueData;
		}
	else
		{
		return TPtrC8(KNullDesC8);
		}
	}


/** Returns the full filename of the file containing application icons

@return The full path and filename of the icon file.
*/
EXPORT_C TPtrC CApaAppData::IconFileName() const
	{
	if (iIconFileName)
		{
		return *iIconFileName;
		}
	else
		{
		return TPtrC(KNullDesC);
		}
	}

/** Returns true if the application provides a non-MBM icon filename.

If this function returns false, this does not necessarily mean
an MBM icon filename is provided.

@return true if the application provides a non-MBM icon filename.
*/
EXPORT_C TBool CApaAppData::NonMbmIconFile() const
	{
	return iNonMbmIconFile;
	}


/** Determines the current language the application is using to display its
user interface.
@return The current language.
*/	
EXPORT_C TLanguage CApaAppData::ApplicationLanguage() const
	{
	return iApplicationLanguage;
	}

/** Returns true if the application implements the specified service.
@param aServiceUid The service UID.
@return true if the application implements the specified service.
@internalComponent
*/
EXPORT_C TBool CApaAppData::ImplementsService(TUid aServiceUid) const
	{
	if (iServiceArray)
		{
		TInt count = iServiceArray->Count();
		for (TInt i = count-1; i >= 0; i--)
			{
			if ((*iServiceArray)[i].Uid() == aServiceUid)
				{
				return ETrue;
				}
			}
		}
	return EFalse;
	}
	
/** Checks if the application implements the specified service and if the 
service explicitly supports the datatype. Explicitly means that the datatype is
listed in the service's list of datatype in the registration file and is
not the general datatype associated with the application (aka the Open service).
@param aServiceUid The service UID.
@param aDataType The datattype
@return The priority. KDataTypePriorityNotSupported if the app doesn't support
this service with this datatype.
@internalComponent
*/
TInt CApaAppData::ImplementsServiceWithDataType(TUid aServiceUid, const TDataType& aDataType) const
	{
	TInt result = KDataTypePriorityNotSupported;
	if (iServiceArray)
		{
		TInt count = iServiceArray->Count();
		for (TInt i = count-1; i >= 0; i--)
			{
			// There can be more than one instance of a given service so we iterate
			// through the whole service list even if we have already found a suitable
			// service.
			if ((*iServiceArray)[i].Uid() == aServiceUid)
				{
				const CArrayFixFlat<TDataTypeWithPriority>& datatypes =
					(*iServiceArray)[i].DataTypes();
				TInt priority = DataType(aDataType, datatypes);
				if (priority > result)
					{
					result = priority;
					}
				}
			}
		}
	return result;
	}

EXPORT_C void CApaAppData::SetShortCaptionL(const TDesC& aShortCaption)
	{
	if(iShortCaption->Compare(aShortCaption) != 0)
		{
		HBufC* newShortCaption=aShortCaption.AllocL();
		if (!iShortCaptionFromResourceFile)
			{ // store the rsc file caption into iShortCaptionFromResourceFile so that it can be externalized.
			iShortCaptionFromResourceFile = iShortCaption;
			}
		else
			{
			delete iShortCaption;
			}
		iShortCaption = newShortCaption;
		}
	}

/** Sets the caption of the application. If the caption is from central repository,
 it overrides tha value from the resource file.  
*/
EXPORT_C void CApaAppData::SetCaptionL(const TDesC& aCaption)
	{
	if(iCaption->Compare(aCaption) != 0)
		{
		HBufC* newCaption=aCaption.AllocL();
		if (!iCaptionFromResourceFile)
			{ // store the rsc file caption into iCaptionFromResourceFile so that it can be externalized.
			iCaptionFromResourceFile = iCaption;
			}
		else
			{
			delete iCaption;
			}
		iCaption = newCaption;
		}
	}

/** Sets the icon details of an application. If these details are from the central repository,
 it overrides the value in the resource file and loads it.
*/
EXPORT_C void CApaAppData::SetIconsL(const TDesC& aFileName, TInt aNumIcons)
	{
	if (iIconFileName && 
		iIconFileName->Compare(aFileName) == 0 &&
		iNumOfAppIcons == aNumIcons)
		return;

	if (!iIconFileNameFromResourceFile)
		{
	 	iNumOfAppIconsFromResourceFile = iNumOfAppIcons;
	 	iIconFileNameFromResourceFile = iIconFileName;
	 	iIconFileName = NULL;
	 	iNonMbmIconFileFromResourceFile = iNonMbmIconFile;
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	 	
	 	iIconFileTimeStampFromResourceFile = iIconFileTimeStamp;
#endif	 	
		}
		
	iNonMbmIconFile = !CApaAppInfoReader::FileIsMbmWithGenericExtensionL(aFileName);
	iNumOfAppIcons = aNumIcons;
	
	if (aFileName != KNullDesC)
		{
		iIconFileName = aFileName.AllocL();
		if (!iNonMbmIconFile)
			{
			if (iNumOfAppIcons > 0)
				{
				// Creates an Application Icon Array
				CApaAppIconArray* icons = CApaAppIconArray::NewAppIconsL(iNumOfAppIcons, *iIconFileName, *iIconLoader);
				delete iIcons;
				iIcons = icons;
				}
			}
		else
			{	// Creates an Empty Icon Array if application has Non-Mbm Icons
			CApaAppIconArray* icons = CApaAppIconArray::NewL();
			delete iIcons;
			iIcons = icons;
			}
		}
	else
		{
		CApaAppIconArray* icons = CApaAppIconArray::NewDefaultIconsL();
		delete iIcons;
		iIcons = icons;
		}
	}

void CApaAppData::InternalizeL(RReadStream& aReadStream)
/** Internalizes the appdata from the AppsList.bin file */
	{
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK    
	TUint highTime = aReadStream.ReadUint32L();
	TUint lowTime = aReadStream.ReadUint32L();
	iTimeStamp = TTime(MAKE_TINT64(highTime, lowTime));
	
	highTime = aReadStream.ReadUint32L();
	lowTime = aReadStream.ReadUint32L();
	iIconFileTimeStamp = TTime(MAKE_TINT64(highTime, lowTime));
#endif
	
	iCaption = HBufC::NewL(aReadStream, KMaxFileName);	// Caption
	iShortCaption = HBufC::NewL(aReadStream, KMaxFileName);	// Shortcaption
	iFullName = HBufC::NewL(aReadStream, KMaxFileName);		// Filename of application binary

	TUid uid1;
	uid1.iUid = aReadStream.ReadUint32L();
	TUid uid2;
	uid2.iUid = aReadStream.ReadUint32L();	
	TUid uid3;
	uid3.iUid = aReadStream.ReadUint32L();
	iUidType = TUidType(uid1, uid2, uid3);	// Application UID
	
	aReadStream >> iCapabilityBuf;
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 	
	iRegistrationFile = HBufC::NewL(aReadStream, KMaxFileName);	// Registration Filename
#endif	
	iDefaultScreenNumber = aReadStream.ReadUint32L();	// Default Screen number
	iNumOfAppIcons = aReadStream.ReadInt32L();	// No. of icons
	iNonMbmIconFile = aReadStream.ReadUint32L();

	HBufC* iconFileName = HBufC::NewL(aReadStream, KMaxFileName);	// Icon Filename
	if (*iconFileName != KNullDesC)
		{
		iIconFileName = iconFileName;
		if (!iNonMbmIconFile)
			{
			if (iNumOfAppIcons > 0)
				{ // Create IconLoader to load icons
				iIconLoader = CApaIconLoader::NewL(iFs);
				// Creates an Application Icon Array
				iIcons = CApaAppIconArray::NewAppIconsL(iNumOfAppIcons, *iIconFileName, *iIconLoader);
				}
			else
			    TRAP_IGNORE(iIcons = CApaAppIconArray::NewDefaultIconsL()); // Creates and Loads Default Icons.
			}
		else
			{	// Creates an Empty Icon Array if application has Non-Mbm Icons
			iIcons = CApaAppIconArray::NewL();
			}
		}
	else
		{
		delete iconFileName;
		TRAP_IGNORE(iIcons = CApaAppIconArray::NewDefaultIconsL()); // Creates and Loads Default Icons.
		}

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 
	HBufC* localisableResourceFileName = HBufC::NewL(aReadStream, KMaxFileName);	// Registration Filename
	if (*localisableResourceFileName != KNullDesC)
		iLocalisableResourceFileName = localisableResourceFileName;
	else
		delete localisableResourceFileName;

	highTime = aReadStream.ReadUint32L();
	lowTime = aReadStream.ReadUint32L();
	iLocalisableResourceFileTimeStamp = TTime(MAKE_TINT64(highTime, lowTime));	// Localisable file timestamp
#endif
	
	iApplicationLanguage = (TLanguage)aReadStream.ReadInt32L();	// Application Language
	iIndexOfFirstOpenService = aReadStream.ReadUint32L();		// Index of first open service
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 	
	iNonNativeApplicationType.iUid = aReadStream.ReadUint32L();
#endif

	HBufC8* opaqueData = HBufC8::NewL(aReadStream, KMaxFileName);	// Opaque Data
	if (*opaqueData != KNullDesC8)
		iOpaqueData = opaqueData;
	else
		delete opaqueData;

	iViewDataArray = new(ELeave) CArrayPtrFlat<CApaAppViewData>(1);	// ViewDataArray
	const TInt viewCount = aReadStream.ReadInt32L();
	TInt i;	
	for (i = 0; i < viewCount; ++i)
		{
		CApaAppViewData* pView = CApaAppViewData::NewLC();

		pView->iCaption = HBufC::NewL(aReadStream, KMaxFileName);
		pView->iNumOfViewIcons = aReadStream.ReadUint32L();
		pView->iNonMbmIconFile = aReadStream.ReadUint32L();
		HBufC* iconFileName = HBufC::NewL(aReadStream, KMaxFileName);	// Icon Filename		
		if (*iconFileName != KNullDesC)
			{
			pView->iIconFileName = iconFileName;
			if (!pView->iNonMbmIconFile)
				{
				if (pView->iNumOfViewIcons > 0)
					{
					if (!iIconLoader)
						{	// Create Icon Loader if it was not done for App or any of the previous views for the App.
						iIconLoader = CApaIconLoader::NewL(iFs);
						}
					// Creates an Application View Icon Array
					CApaAppIconArray* iconViewArray = CApaAppIconArray::NewViewIconsL(pView->iNumOfViewIcons, *pView->iIconFileName, *iIconLoader);
					pView->SetIconArray(iconViewArray);
					}
				}
			}
		else
			delete iconFileName;

		pView->iScreenMode = aReadStream.ReadUint32L();
		pView->iUid.iUid = aReadStream.ReadUint32L();

		iViewDataArray->AppendL(pView);
		CleanupStack::Pop(pView);
		}

	iServiceArray = new(ELeave) CArrayFixFlat<TApaAppServiceInfo>(1);
	const TInt serviceCount = aReadStream.ReadUint32L();

	for (i = 0; i < serviceCount; ++i)
		{
		TApaAppServiceInfo serviceInfo ;
		aReadStream >> serviceInfo;
		iServiceArray->AppendL(serviceInfo);
		}

	iOwnedFileArray = new(ELeave) CDesCArraySeg(1);
	const TInt fileCount = aReadStream.ReadUint32L();	
	for (i = 0; i < fileCount; ++i)
		{
		TFileName ownedFile;
		aReadStream >> ownedFile;
		iOwnedFileArray->AppendL(ownedFile);
		}
	}

TBool CApaAppData::ViewMbmIconsRequireLoading() const
	{
	const TInt count = iViewDataArray->Count();
	for (TInt i = 0; i < count; ++i)
		{
		const CApaAppViewData* const viewData = iViewDataArray->At(i);
		if ((!viewData->iNonMbmIconFile) && (!viewData->iIcons->AreViewIconsLoaded()))
			{
			return ETrue;
			}
		}
	return EFalse;
	}
	
TBool CApaAppData::MbmIconsRequireLoading() const
	{
	if (!iNonMbmIconFile)
		{
		if (!iIcons->AreAppIconsLoaded())
			{
			return ETrue;
			}
		}

	if (ViewMbmIconsRequireLoading())
		{// if a view has mbm icons, and its not yet loaded we should load its icons.
		return ETrue;
		}
	return EFalse; // icons were loaded already so no need to load them again.
	}

void CApaAppData::ExternalizeL(RWriteStream& aWriteStream) const
	{
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK     
	aWriteStream.WriteUint32L(I64HIGH(iTimeStamp.Int64()));
	aWriteStream.WriteUint32L(I64LOW(iTimeStamp.Int64()));

	if (iIconFileNameFromResourceFile)
		{
		aWriteStream.WriteUint32L(I64HIGH(iIconFileTimeStampFromResourceFile.Int64()));
	    aWriteStream.WriteUint32L(I64LOW(iIconFileTimeStampFromResourceFile.Int64()));
		}
	else
		{
		aWriteStream.WriteUint32L(I64HIGH(iIconFileTimeStamp.Int64()));
		aWriteStream.WriteUint32L(I64LOW(iIconFileTimeStamp.Int64()));
		}
#endif	
	
	if (iCaptionFromResourceFile) // Caption present in the resource file would be externalized if the one in applist has dynamically changed
		{
		aWriteStream << *iCaptionFromResourceFile;
		}
	else
		{
		aWriteStream << *iCaption;	// Caption
		}

	if (iShortCaptionFromResourceFile)	// Short caption present in the resource file would be externalized if the one in applist has dynamically changed
		aWriteStream << *iShortCaptionFromResourceFile;
	else
		aWriteStream << *iShortCaption;

	aWriteStream << *iFullName;	// FullName

	TInt i = 0;
	for (i = 0; i < 3; ++i)
		aWriteStream << iUidType[i];	// Uid Type

	aWriteStream << iCapabilityBuf;
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 	
	aWriteStream << RegistrationFileName();	// Registration filename
#endif	
	aWriteStream.WriteUint32L(iDefaultScreenNumber);	// Default screen number

	if (iIconFileNameFromResourceFile)
		{
		aWriteStream.WriteUint32L(iNumOfAppIconsFromResourceFile);	// number of icons
		aWriteStream.WriteUint32L(iNonMbmIconFileFromResourceFile);
		aWriteStream << *iIconFileNameFromResourceFile;
		}
	else
		{
		aWriteStream.WriteUint32L(iNumOfAppIcons);	// number of icons
		aWriteStream.WriteUint32L(iNonMbmIconFile);
		aWriteStream << IconFileName();
		}
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 
	aWriteStream << LocalisableResourceFileName();

	aWriteStream.WriteUint32L(I64HIGH(iLocalisableResourceFileTimeStamp.Int64()));
	aWriteStream.WriteUint32L(I64LOW(iLocalisableResourceFileTimeStamp.Int64()));
#endif	

	aWriteStream.WriteInt32L(iApplicationLanguage);

	aWriteStream.WriteUint32L(iIndexOfFirstOpenService);

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 
	aWriteStream.WriteUint32L(iNonNativeApplicationType.iUid);
#endif

	aWriteStream << OpaqueData();
	
	TInt count = iViewDataArray->Count();
	aWriteStream.WriteUint32L(count);

	for (i = 0; i < count; ++i)
		{
		const CApaAppViewData* const viewData = iViewDataArray->At(i);
		aWriteStream << *(viewData->iCaption);
		aWriteStream.WriteUint32L(viewData->iNumOfViewIcons);
		aWriteStream.WriteUint32L(viewData->iNonMbmIconFile);
		aWriteStream << viewData->IconFileName();		
		aWriteStream.WriteUint32L(viewData->iScreenMode);
		aWriteStream.WriteUint32L(viewData->iUid.iUid);
		}

	// TApaAppServiceInfo service array
	if (iServiceArray)
		{
		count = iServiceArray->Count();
		aWriteStream.WriteUint32L(count);
		for (i = 0; i < count; ++i)
			aWriteStream << iServiceArray->At(i);
		}
	else
		aWriteStream.WriteUint32L(NULL);

	if (iOwnedFileArray)
		{
		count = iOwnedFileArray->MdcaCount();
		aWriteStream.WriteUint32L(count);
		for (i = 0; i < count; ++i)
			aWriteStream << (*iOwnedFileArray)[i];
		}
	else
		aWriteStream.WriteUint32L(0);
	}

