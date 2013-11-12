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
// aplappinforeader.cpp
//

#include "aplapplist.h"
#include "aplapplistitem.h"
#include "aplappinforeader.h"
#include <apgaplst.h>
#include <bautils.h>
#include "APGICNFL.H" 
#include "APFDEF.H"
#include "../apgrfx/apprivate.h"		// KUidPrefixedNonNativeRegistrationResourceFile
#include <barsc2.h>
#include <barsread2.h>
#include <e32uid.h>
#include "../apgrfx/APGSTD.H"			// EPanicNullPointer
#include "../apgrfx/apsecutils.h"		// CApaSecurityUtils


#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
const TUint KResourceOffsetMask = 0xFFFFF000;

_LIT(KAppBinaryPathAndExtension, "\\sys\\bin\\.exe");
const TInt KAppRegistrationInfoResourceId = 1;

// The 2nd UID that defines a resource file as being an application registration resource file.
const TUid KUidAppRegistrationFile = {0x101F8021};
#endif



//
// Local functions
//

extern void CleanupServiceArray(TAny* aServiceArray);	// Implemented in AplAppList.cpp


#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
// ApaUtils

TBool ApaUtils::TypeUidIsForRegistrationFile(const TUidType& aUidType)
    { // static
    return (aUidType[1].iUid==KUidAppRegistrationFile.iUid ||
           aUidType[0].iUid==KUidPrefixedNonNativeRegistrationResourceFile);
    }
#endif

//
// CApaAppInfoReader
//

// The behaviour of the Take-methods of this class is a little non-standard, as it
// transfers ownership of the pointer owned by a CApaAppInfoReader derived object
// to the caller. This means that this function is only designed to be called once.
// Doing things this way provides a small performance optimisation by enabling the caller
// to delete it's stored pointer, and replace it with one returned by this function,
// instead of having to copy the object (copying could be expensive for the methods
// of this class that need to return arrays).

void CApaAppInfoReader::ConstructL()
	{
	iIconLoader = CApaIconLoader::NewL(iFs);
	}

CApaAppInfoReader::~CApaAppInfoReader()
	{
	delete iAppBinaryFullName;
	delete iCaption;
	delete iShortCaption;
	delete iIcons;
	if (iViewDataArray)
		iViewDataArray->ResetAndDestroy();
	
	delete iViewDataArray;
	delete iOwnedFileArray;
	delete iIconFileName;
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
	delete iLocalisableResourceFileName;
#endif
	
	if (iServiceArray)
 		{
 		CleanupServiceArray(iServiceArray);
 		iServiceArray = NULL;
  		}
  		
	delete iOpaqueData;
	delete iIconLoader;
	}

HBufC* CApaAppInfoReader::AppBinaryFullName()
	{
	HBufC* fileName = iAppBinaryFullName;
	iAppBinaryFullName = NULL; // ownership transferred to caller
	return fileName;
	}

TUidType CApaAppInfoReader::AppBinaryUidType() const
	{
	return iAppBinaryUidType;
	}

void CApaAppInfoReader::Capability(TDes8& aCapabilityBuf) const
	{
	TApaAppCapabilityBuf buf(iCapability);
	TApaAppCapability::CopyCapability(aCapabilityBuf, buf);
	}

TUint CApaAppInfoReader::DefaultScreenNumber() const
	{
	return iDefaultScreenNumber;
	}

HBufC* CApaAppInfoReader::Caption()
	{
	HBufC* caption = iCaption;
	iCaption = NULL; // ownership transferred to caller
	return caption;
	}

HBufC* CApaAppInfoReader::ShortCaption()
	{
	HBufC* shortCaption = iShortCaption;
	iShortCaption = NULL; // ownership transferred to caller
	return shortCaption;
	}

CApaAppIconArray* CApaAppInfoReader::Icons()
	{
	CApaAppIconArray* icons = iIcons;
	iIcons = NULL; // ownership transferred to caller
	return icons;
	}

TInt CApaAppInfoReader::NumOfAppIcons() const
	{
	return iNumOfAppIcons;
	}

CArrayPtrFlat<CApaAppViewData>* CApaAppInfoReader::Views()
	{
	CArrayPtrFlat<CApaAppViewData>* viewDataArray = iViewDataArray;
	iViewDataArray = NULL; // ownership transferred to caller
	return viewDataArray;
	}
	
CDesCArray* CApaAppInfoReader::OwnedFiles()
	{
	CDesCArray* ownedFileArray = iOwnedFileArray;
	iOwnedFileArray = NULL; // ownership transferred to caller
	return ownedFileArray;
	}

HBufC* CApaAppInfoReader::IconFileName()
	{
	HBufC* iconFileName = iIconFileName;
	iIconFileName = NULL; // ownership transferred to caller
	return iconFileName;
	}

TBool CApaAppInfoReader::NonMbmIconFile() const
	{
	return iNonMbmIconFile;
	}


TLanguage CApaAppInfoReader::AppLanguage() const
	{
	return iApplicationLanguage;
	}

CArrayFixFlat<TApaAppServiceInfo>* CApaAppInfoReader::ServiceArray(TInt& aIndexOfFirstOpenService)
	{
	CArrayFixFlat<TApaAppServiceInfo>* serviceArray = iServiceArray;
	iServiceArray = NULL;
	aIndexOfFirstOpenService = iIndexOfFirstOpenService;
	return serviceArray;
	}

HBufC8* CApaAppInfoReader::OpaqueData()
	{
	HBufC8* opaqueData = iOpaqueData;
	iOpaqueData = NULL;
	return opaqueData;
	}

CApaIconLoader* CApaAppInfoReader::IconLoader()
	{
	CApaIconLoader* iconLoader = iIconLoader;
	iIconLoader = NULL; // ownership transferred to caller
	return iconLoader;
	}

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK

CApaAppInfoReader::CApaAppInfoReader(RFs& aFs, const Usif::CApplicationRegistrationData& aAppInfo, const Usif::RSoftwareComponentRegistry& aScr) :
    iFs(aFs),
    iDefaultScreenNumber(0),
    iNonMbmIconFile(EFalse),
    iApplicationLanguage(ELangNone),
    iIndexOfFirstOpenService(KErrNotFound),
    iAppInfo(aAppInfo),
    iScr(aScr)
    {
    }

CApaAppInfoReader* CApaAppInfoReader::NewL(RFs& aFs, const Usif::CApplicationRegistrationData& aAppInfo, const Usif::RSoftwareComponentRegistry& aScr)
    {
    CApaAppInfoReader* self = new(ELeave) CApaAppInfoReader(aFs, aAppInfo, aScr);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

/*
 * Reads the application information from SCR. 
 */
TBool CApaAppInfoReader::ReadL()
    {
    ReadAppRegistrationInfoL();
    ReadLocalisationInfoL();  
#ifdef APPARC_SHOW_TRACE    
    DisplayAppInfo();
#endif
    return ETrue;
    }

#ifdef APPARC_SHOW_TRACE
void CApaAppInfoReader::DisplayAppInfo()
    { 
    RDebug::Print(_L("[Apparc] Application UID: %X"), iAppUid.iUid);
    if(iAppBinaryFullName)
        RDebug::Print(_L("[Apparc] AppBinary Name: %S"), iAppBinaryFullName);
    
    RDebug::Print(_L("[Apparc] Embeddability: %d"), iCapability.iEmbeddability);
    RDebug::Print(_L("[Apparc] Hidden: %d"), iCapability.iAppIsHidden);
    RDebug::Print(_L("[Apparc] NewFile: %d"), iCapability.iSupportsNewFile);
    RDebug::Print(_L("[Apparc] Launch in Foreground: %d"), iCapability.iLaunchInBackground);
    RDebug::Print(_L("[Apparc] Attributes: %X"), iCapability.iAttributes);
    RDebug::Print(_L("[Apparc] Group Name: %S"), &iCapability.iGroupName);
    
    RDebug::Print(_L("[Apparc] Default Screen Number: %d"), iDefaultScreenNumber);
    RDebug::Print(_L("[Apparc] Application Language: %d"), iApplicationLanguage);
    
    if(iCaption)
        RDebug::Print(_L("[Apparc] Short Cpation: %S"), iCaption); 
		
    if(iShortCaption)
        RDebug::Print(_L("[Apparc] Caption: %S"), iShortCaption);
    
    if(iServiceArray)
        {
        for(TInt index=0;index<iServiceArray->Count();index++)
            {
            TApaAppServiceInfo serviceInfo=(*iServiceArray)[index];
            RDebug::Print(_L("[Apparc] Service Uid: %X"), serviceInfo.Uid().iUid);
            
            for(TInt j=0;j<serviceInfo.DataTypes().Count();j++)
            {
            TDataTypeWithPriority dataType=(serviceInfo.DataTypes())[j];
            RDebug::Print(_L("[Apparc] Data Type: %s   Priority:%d"), &dataType.iDataType, dataType.iPriority);
            }
            
            }
        }
    
    if(iIconFileName)
        {
        RDebug::Print(_L("[Apparc] Icon File: %S"), iIconFileName);   
        
        if(iNonMbmIconFile)
            RDebug::Print(_L("[Apparc] Its Non MBM icon file"));
        RDebug::Print(_L("[Apparc] Num Icons: %d"), iNumOfAppIcons);
        }

    if(iViewDataArray)
        {
        for(TInt index=0; index<iViewDataArray->Count();index++)
            {
            CApaAppViewData* view= (*iViewDataArray)[index];
            RDebug::Print(_L("[Apparc] ViewID: %X"), view->Uid().iUid);
            //RDebug::Print(_L("[Apparc] View Caption: %s"), view->Caption());
            //RDebug::Print(_L("[Apparc] View Icon File: %s"), view->IconFileName());   
            if(view->NonMbmIconFile())
                RDebug::Print(_L("[Apparc] Its Non MBM icon file"));
            RDebug::Print(_L("[Apparc] Screen Mode: %d"), view->ScreenMode());
            }
        }
    }

#endif


void CApaAppInfoReader::ReadAppRegistrationInfoL()
    {
    //Get 3rd UID of the application
    iAppUid=iAppInfo.AppUid();
    
    iCapability.iAttributes=iAppInfo.Attributes();
    TUid firstUid(KExecutableImageUid);
    TUid middleUid(KUidApp);

    //If the application is non-native, first UID should be Null UID and second uid is the non-native application type(i.e type ID of java, widget etc.). 
    if (iCapability.iAttributes & TApaAppCapability::ENonNative)
        {
            firstUid=KNullUid;
            middleUid.iUid=iAppInfo.TypeId();
        }
    else if (iCapability.iAttributes & TApaAppCapability::EBuiltAsDll)
        {
        User::Leave(KErrNotSupported); // legacy dll-style app
        }
    
    iAppBinaryUidType=TUidType(firstUid, middleUid, iAppUid);
   
    //If executable file name is not given then just leave. 
    if(iAppInfo.AppFile().Length() ==0 )
        User::Leave(KErrGeneral);

    //Absolute path of the executable file is stored in iAppBinaryFullName
    iAppBinaryFullName=iAppInfo.AppFile().AllocL();
    
//    //Check whether the binary exists.
    /*RLibrary::TInfoBuf infoBuf;
    TInt ret = RLibrary::GetInfo(*iAppBinaryFullName, infoBuf);
    User::LeaveIfError(ret);
    
    if(infoBuf().iUids[2]!=iAppUid && iAppBinaryUidType[1]==KUidApp)
        User::Leave(KErrNotFound);*/
    
    iCapability.iAppIsHidden=iAppInfo.Hidden();
    iCapability.iEmbeddability = static_cast<TApaAppCapability::TEmbeddability>(iAppInfo.Embeddability());
    iCapability.iLaunchInBackground=iAppInfo.Launch();
    iCapability.iSupportsNewFile=iAppInfo.NewFile();
    
    iDefaultScreenNumber=iAppInfo.DefaultScreenNumber();
    
    iCapability.iGroupName=iAppInfo.GroupName();
    
    RPointerArray<Usif::COpaqueData> appOpaqueData=iAppInfo.AppOpaqueData();
    ASSERT(!(appOpaqueData.Count()>1));
    
    if(appOpaqueData.Count()>0)
        {
        iOpaqueData=appOpaqueData[0]->OpaqueData().AllocL();
        }
    else
        {
        //If opaque data is not available, create an empty object and assign to iOpaqueData
        iOpaqueData=HBufC8::NewL(0);
        }  
    
    ReadServiceInfoL(iAppInfo.ServiceArray()); 
    ReadOwnedFilesInfoL(iAppInfo.OwnedFileArray());
    }


/*
 * Reads service information of the application.
 */
void CApaAppInfoReader::ReadServiceInfoL(const RPointerArray<Usif::CServiceInfo>& aServiceInfo)
    {
    TInt serviceCount=aServiceInfo.Count();
    
    if (serviceCount > 0)
        {
        iServiceArray = new(ELeave) CArrayFixFlat<TApaAppServiceInfo>(4);   
        }
    else
        {
        //if service information is not avaliable, just return.
        return;        
        }

    //Read application service info one at a time and store in iServiceArray.
    for (TInt index=0;index<serviceCount;index++)
        {
        TUid serviceUid=aServiceInfo[index]->Uid();
        
        CArrayFixFlat<TDataTypeWithPriority>* mimeTypesSupported = new(ELeave) CArrayFixFlat<TDataTypeWithPriority>(5);
        CleanupStack::PushL(mimeTypesSupported);
        
        //Read supported mime types of a service
        ReadMimeTypesSupportedL(aServiceInfo[index]->DataTypes(), *mimeTypesSupported); 
        
        RPointerArray<Usif::COpaqueData> serviceOpaqueData=aServiceInfo[index]->OpaqueData();
        //SCR schould give atmost only one opaque data for a service.
        ASSERT(!(serviceOpaqueData.Count()>1));
        
        HBufC8* opaqueData=NULL;
        if(serviceOpaqueData.Count()>0)
            {
            opaqueData= serviceOpaqueData[0]->OpaqueData().AllocL();
            }
        else
            {
            //If opaque data is not available, create an empty object and assign to opaqueData        
            opaqueData=HBufC8::NewL(0);
            }
        
        TApaAppServiceInfo serviceInfo(serviceUid, mimeTypesSupported,opaqueData); // takes ownership of mimeTypesSupported and opaqueData 
        CleanupStack::PushL(opaqueData);
        iServiceArray->AppendL(serviceInfo);
        CleanupStack::Pop(2, mimeTypesSupported);        
    
        //If service UID is KOpenServiceUid and it is first open service then initialize iIndexOfFirstOpenService
        if ((serviceUid == KOpenServiceUid) && (iIndexOfFirstOpenService < 0))
            iIndexOfFirstOpenService = iServiceArray->Count() - 1;        
        }
    } 


/*
 * Reads supported mime types and its handling priorities of a service
 */
void CApaAppInfoReader::ReadMimeTypesSupportedL(const RPointerArray<Usif::CDataType>& dataTypes, CArrayFixFlat<TDataTypeWithPriority>& aMimeTypesSupported) 
    {
    
    const TInt dataTypeArraySize = dataTypes.Count();
    //if there are no data types available, just return.
    if (dataTypeArraySize <= 0)
        return;
   
    for (TInt i=0; i < dataTypeArraySize; i++)
        {
        TDataTypePriority priority = static_cast<TDataTypePriority>(dataTypes[i]->Priority());
        
        //Check for data priority of UnTrusted apps however the trusted apps will not have any restrictions 
        //over the data priority.   
        //If an untrusted app has write device data capability (i.e. still has priority = KDataTypePrioritySystem),
        //do not restrict to KDataTypeUnTrustedPriorityThreshold
        if (priority > KDataTypeUnTrustedPriorityThreshold || priority == KDataTypePrioritySystem )
            {
            ReadAppSecurityInfo();

            if (priority == KDataTypePrioritySystem)
                {
                // Check that the app has capability WriteDeviceData
                if (!iHasWriteDeviceDataCap)
                    priority = KDataTypePriorityNormal;
                }
            else
                {
                //data priority for UnTrusted apps would be capped if it is greater than the threshold priority i.e, KMaxTInt16.
                //Component ID is zero if the application is shipped with phone.
                TBool isInstalledApp=(iScr.GetComponentIdForAppL(iAppBinaryUidType[2])!=0);
                if (!iIsSidTrusted && isInstalledApp) 
                    {
                    //if application sid is in unprotected range and the applciation is instaleld with  
                    //one of the installers after phone marketed, then priority needs to be downgraded.
                    priority = KDataTypeUnTrustedPriorityThreshold; 
                    }
                }
            }

        TBuf8<KMaxDataTypeLength> buf;
        //Convert 16-bit descriptor to 8-bit descriptor.
        buf.Copy(dataTypes[i]->Type());

        TDataType dataType(buf);
        TDataTypeWithPriority dataTypeWithPriority(dataType, priority); 
        aMimeTypesSupported.AppendL(dataTypeWithPriority);
        }
    }


/*
 * Reads owned files information.
 */
void CApaAppInfoReader::ReadOwnedFilesInfoL(const RPointerArray<HBufC>& aOwnedFiles)
    {
    const TInt fileOwnershipArraySize = aOwnedFiles.Count();
    
    //if owned files information is not avaliable, just return.
    if (fileOwnershipArraySize <= 0)
        return;
    
    iOwnedFileArray = new(ELeave) CDesCArraySeg(fileOwnershipArraySize);

    for (TInt index=0; index < fileOwnershipArraySize; index++)
        {
        HBufC *fileowned=aOwnedFiles[index]->Des().AllocL();
        CleanupStack::PushL(fileowned);
        iOwnedFileArray->AppendL(*fileowned); //takes the ownership of fileowned
        CleanupStack::Pop(fileowned);
        }
    }

void CApaAppInfoReader::ReadLocalisationInfoL()
    {
    RPointerArray<Usif::CLocalizableAppInfo> localisationInfo;
    localisationInfo=iAppInfo.LocalizableAppInfoList();
    ASSERT(localisationInfo.Count() <= 1);
    
    if(localisationInfo.Count()<=0)
        {
        //If localisable information is not avaialable then assign default icons.
        TRAP_IGNORE(iIcons = CApaAppIconArray::NewDefaultIconsL());     
        return;
        }

    //Group name provided in localisation file takes precedence over group name provided in registration file name.
    const TDesC& groupName=localisationInfo[0]->GroupName();

    if(groupName.Length()>0)
        {
        iCapability.iGroupName=groupName;
        }
    
    //Get application language for current phone language.
    iApplicationLanguage=localisationInfo[0]->ApplicationLanguage();
    
    const Usif::CCaptionAndIconInfo* captionIconInfo=localisationInfo[0]->CaptionAndIconInfo();
    
    TBool useDefaultIcons=ETrue;
    
    if(captionIconInfo!=NULL)
        {
        iShortCaption=localisationInfo[0]->ShortCaption().AllocL();
        if(iShortCaption && iShortCaption->Length() == 0)
            {
                delete iShortCaption;
                iShortCaption=NULL;
            }
    
        iCaption=captionIconInfo->Caption().AllocL();
        if(iCaption && iCaption->Length() == 0)
            {
                delete iCaption;
                iCaption=NULL;
            }        
    
        iNumOfAppIcons=captionIconInfo->NumOfAppIcons();

        if(captionIconInfo->IconFileName().Length()>0)
            iIconFileName=captionIconInfo->IconFileName().AllocL();    

        
        if (iIconFileName && iIconFileName->Length())
            {
            if (iFs.IsValidName(*iIconFileName))
                {
                RFile file;
                TInt fileSize( 0 );
                TInt err= file.Open(iFs, *iIconFileName, EFileShareReadersOnly );
                
                //If the icon file does not exist, use default icons.
                if(err==KErrNone)
                    {
                    User::LeaveIfError(err);
                    CleanupClosePushL( file );
                    User::LeaveIfError( file.Size( fileSize ) );
                    CleanupStack::PopAndDestroy(&file);//file
                    
                    if ( fileSize > 0 )
                        {
                        if(FileIsMbmWithGenericExtensionL(*iIconFileName))
                            {
                            if (iNumOfAppIcons > 0)
                                {
                                //Icon file is valid and contains mbm icons.
                                iIcons = CApaAppIconArray::NewAppIconsL(iNumOfAppIcons, *iIconFileName, *iIconLoader);
                                useDefaultIcons=EFalse;
                                }
                            }
                        else
                            {
                            //If the icon file is not a mbm icon file then the file is treated as a non-mbm file.                
                            iNonMbmIconFile = ETrue;
                            useDefaultIcons=EFalse;
                            }
                        
                        }
                    }
                }
            else
                {
                //If the filename is not a valid name then the file is treated as a non-mbm file.        
                iNonMbmIconFile = ETrue;
                useDefaultIcons=EFalse;                
                }
            }
        }
    
    if(useDefaultIcons)
        TRAP_IGNORE(iIcons = CApaAppIconArray::NewDefaultIconsL());        

    ReadViewInfoL(localisationInfo[0]->ViewDataList());
    }

/*
 * Read application view information.
 */

void CApaAppInfoReader::ReadViewInfoL(const RPointerArray<Usif::CAppViewData>& aViewData)
    {
     const TInt numOfViews = aViewData.Count();

     //if view information not avaliable, just return.     
     if(numOfViews <=0 )
         return;
     
     iViewDataArray = new(ELeave) CArrayPtrFlat<CApaAppViewData>(numOfViews);

     //Read one view information at time and add it iViewDataArray
     for(TInt view = 0; view < numOfViews; ++view)
         {
         CApaAppViewData* viewData = CApaAppViewData::NewLC();
         
         const TUid viewUid = aViewData[view]->Uid();
         viewData->SetUid(viewUid);
         
         const TInt screenMode = {aViewData[view]->ScreenMode()};
         viewData->SetScreenMode(screenMode);

         const Usif::CCaptionAndIconInfo* viewCaptionIconInfo=aViewData[view]->CaptionAndIconInfo();
         
         if(viewCaptionIconInfo!=NULL)
             {
             viewData->SetCaptionL(viewCaptionIconInfo->Caption());
    
             const TInt numOfViewIcons = viewCaptionIconInfo->NumOfAppIcons();
             viewData->SetNumOfViewIcons(numOfViewIcons);
    
             TPtrC viewIconFile = viewCaptionIconInfo->IconFileName();
             
             if (viewIconFile.Length())
                 {
                 viewData->SetIconFileNameL(viewIconFile);
                 
                 if (iFs.IsValidName(viewIconFile))
                     {
                     if(!FileIsMbmWithGenericExtensionL(viewIconFile))
                         viewData->SetNonMbmIconFile(ETrue);
                     }
                 else    //If the filename is not a valid name then the file is treated as a non-mbm file.
                     viewData->SetNonMbmIconFile(ETrue);
                 }
             else
                 {
                 viewIconFile.Set(KNullDesC);
                 if (numOfViewIcons > 0 && iIconFileName)
                     viewIconFile.Set(*iIconFileName); // default to app icon filename
                 }
             
             if (numOfViewIcons > 0 && iFs.IsValidName(viewIconFile) && FileIsMbmWithGenericExtensionL(viewIconFile))
                 {
                 CApaAppIconArray* iconArray = CApaAppIconArray::NewViewIconsL(numOfViewIcons, viewIconFile, *iIconLoader);
                 viewData->SetIconArray(iconArray);
                 iconArray = NULL;
                 }
             }

         iViewDataArray->AppendL(viewData);
         CleanupStack::Pop(viewData);
         }    
    }

#else

CApaAppInfoReader* CApaAppInfoReader::NewL(RFs& aFs, const TDesC& aRegistrationFileName, TUid aAppUid)
    {
    CApaAppInfoReader* self = new(ELeave) CApaAppInfoReader(aFs, aRegistrationFileName, aAppUid);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CApaAppInfoReader::CApaAppInfoReader(RFs& aFs, const TDesC& aRegistrationFileName, TUid aAppUid) :
    iFs(aFs),
    iAppUid(aAppUid),
    iTimeStamp(0),
    iDefaultScreenNumber(0),
    iNonMbmIconFile(EFalse),
    iLocalisableResourceFileTimeStamp(0),
    iApplicationLanguage(ELangNone),
    iIndexOfFirstOpenService(KErrNotFound),
    iRegistrationFileName(aRegistrationFileName)
    {
    }

TTime CApaAppInfoReader::TimeStamp() const
    {
    return iTimeStamp;
    }

TTime CApaAppInfoReader::IconFileTimeStamp() const
     {
     return iIconFileTimeStamp;
     }

HBufC* CApaAppInfoReader::LocalisableResourceFileName()
    {
    HBufC* localisableResourceFileName = iLocalisableResourceFileName;
    iLocalisableResourceFileName = NULL; // ownership transferred to caller
    return localisableResourceFileName;
    }

TTime CApaAppInfoReader::LocalisableResourceFileTimeStamp() const
    {
    return iLocalisableResourceFileTimeStamp;
    }

// reads as much info as it can
// at least captions and icons must be setup on return from this method (using defaults if necessary)
TBool CApaAppInfoReader::ReadL()
	{
	TEntry entry;
	User::LeaveIfError(iFs.Entry(iRegistrationFileName, entry));
	iTimeStamp = entry.iModified;
	
	TUint fileOffset = 0;
	TInt fileLength = 0;
	TUid firstUid(KExecutableImageUid);
	TUid middleUid(KUidApp);

	// in the case of a non-native application, the resource file
	// has been padded with the application type uid
	
	if (TParsePtrC(iRegistrationFileName).Path().CompareF(KLitPathForNonNativeResourceAndIconFiles)==0)
		{
		fileOffset = sizeof(TCheckedUid);
		fileLength = entry.iSize-fileOffset;
		firstUid = KNullUid;
		middleUid = entry[1];
		if (middleUid == KNullUid)
			User::Leave(KErrCorrupt);
		}
		
	// set the TUidType for the app binary
	// cannot read the TEntry info from the app binary because it's in \sys\bin
	iAppBinaryUidType = TUidType(firstUid, middleUid, iAppUid);	

	CResourceFile* registrationFile = CResourceFile::NewLC(iFs, iRegistrationFileName, fileOffset, fileLength);
	RResourceReader	resourceReader;
	resourceReader.OpenLC(registrationFile, KAppRegistrationInfoResourceId);	

	TRAPD(err, ReadMandatoryInfoL(resourceReader));
	if (err)
		{
		CleanupStack::PopAndDestroy();	// resourceReader
		CleanupStack::PopAndDestroy(registrationFile);
		return EFalse; // might have read something, but failed to setup enough info to make it worthwhile trying to read any more
		}

	CResourceFile* localisableFile = NULL;
	TUint localisableResourceId = 1; // only initialising this here to keep the compiler happy, as it's concerned that the variable might be used without having been initialised. The variable should be initialised later, before it's used
	TRAP(err, ReadNonLocalisableInfoL(resourceReader, localisableFile, localisableResourceId));

	if (!err)
		TRAP(err, ReadNonLocalisableOptionalInfoL(resourceReader, registrationFile, localisableFile));

	TBool useDefaultIcons = ETrue;
	if (!err && localisableFile)
		TRAP(err, ReadLocalisableInfoL(*localisableFile, localisableResourceId, useDefaultIcons));

	delete localisableFile;
	localisableFile = NULL;
	
	// if anything went wrong, we tell the caller that the read was unsuccessful. Some
	// of the members of this class may contain data which is not complete, but this doesn't matter
	// because the caller shouldn't try to access the data if the read was unsuccessful
	const TBool readSuccessful = (err == KErrNone);

	if (useDefaultIcons)
		{
		delete iIcons;
		iIcons = NULL;
		TRAP_IGNORE(iIcons = CApaAppIconArray::NewDefaultIconsL());
		}

	CleanupStack::PopAndDestroy();	// resourceReader
	CleanupStack::PopAndDestroy(registrationFile);
	return readSuccessful;
	}


// this method reads the minimum information required to register an app
// if this fails (Leaves), we say the read has been unsuccessful
void CApaAppInfoReader::ReadMandatoryInfoL(RResourceReader& aResourceReader)
	{
	aResourceReader.ReadUint32L(); // skip over LONG reserved_long
	aResourceReader.ReadUint32L(); // skip over LLINK reserved_llink

	// read LTEXT app_file
	const TPtrC appFile(aResourceReader.ReadTPtrCL());
	TParse parse; // this object gets used for 2 purposes: first to check that a TParsePtrC can be created over "appFile" without it panicking, and second to construct iAppBinaryFullName
	User::LeaveIfError(parse.SetNoWild(appFile, NULL, NULL)); // do this before creating a TParsePtrC, since TParsePtrC's constructor panics if it fails (which would provide an easy way for malware to kill the Apparc server)
	const TParsePtrC appFileParser(appFile);

	// read LONG attributes
	iCapability.iAttributes = aResourceReader.ReadUint32L();

	if (!appFileParser.NamePresent())
		User::Leave(KErrCorrupt);

	const TPtrC appNameWithoutExtension(appFileParser.Name());
	const TPtrC registrationFileDrive(TParsePtrC(iRegistrationFileName).Drive());
	
	if (iCapability.iAttributes & TApaAppCapability::ENonNative)
		{
		if (!appFileParser.PathPresent() || !appFileParser.ExtPresent())
			User::Leave(KErrCorrupt);

		const TPtrC appFilePath(appFileParser.Path());
		const TPtrC appFileNameAndExt(appFileParser.NameAndExt());
		TPtrC appFileDrive(registrationFileDrive);
		if (appFileParser.DrivePresent())
			appFileDrive.Set(appFileParser.Drive());

		User::LeaveIfError(parse.SetNoWild(appFileDrive, &appFilePath, &appFileNameAndExt));
		}
	else if (iCapability.iAttributes & TApaAppCapability::EBuiltAsDll)
		{
		User::Leave(KErrNotSupported); // legacy dll-style app
		}
	else
		{
		// exe-style app
		User::LeaveIfError(parse.SetNoWild(registrationFileDrive, &KAppBinaryPathAndExtension, &appNameWithoutExtension));
		}

	iAppBinaryFullName = parse.FullName().AllocL();
	}

void CApaAppInfoReader::ReadLocalisableInfoL(const CResourceFile& aResourceFile, TUint aResourceId, TBool& aUseDefaultIcons)
	{
	RResourceReader resourceReader;
	resourceReader.OpenLC(&aResourceFile, aResourceId);

	resourceReader.ReadUint32L(); // skip over LONG reserved_long
	resourceReader.ReadUint32L(); // skip over LLINK reserved_llink

	// read LTEXT short_caption
	iShortCaption = resourceReader.ReadHBufCL();

	resourceReader.ReadUint32L(); // skip over LONG reserved_long
	resourceReader.ReadUint32L(); // skip over LLINK reserved_llink

	// read LTEXT caption
	iCaption = resourceReader.ReadHBufCL();

	// read WORD number_of_icons
	const TInt numOfIcons = resourceReader.ReadInt16L();
	iNumOfAppIcons = numOfIcons;

	// read LTEXT icon_file
	TPtrC iconFile = resourceReader.ReadTPtrCL();
	
	ASSERT(!iIconFileName);
	iIconFileName = CreateFullIconFileNameL(iconFile);
	if (iIconFileName)
		{
		TEntry entry;
		TInt ret = iFs.Entry(*iIconFileName, entry);
		if (KErrNone == ret)
			iIconFileTimeStamp = entry.iModified;

			
		aUseDefaultIcons = EFalse;
		if (iFs.IsValidName(*iIconFileName))
			{
			RFile file;
            TInt fileSize( 0 );
            User::LeaveIfError( file.Open(iFs, *iIconFileName, EFileShareReadersOnly ));
            CleanupClosePushL( file );
            User::LeaveIfError( file.Size( fileSize ) );
            CleanupStack::PopAndDestroy();//file
            if ( fileSize > 0  )
                {
				if(FileIsMbmWithGenericExtensionL(*iIconFileName))
					{
					if (numOfIcons > 0)
						{
						CApaAppIconArray* iconArray = CApaAppIconArray::NewAppIconsL(numOfIcons, *iIconFileName, *iIconLoader);
						delete iIcons;
						iIcons = iconArray;
						}
					else
						{
						aUseDefaultIcons = ETrue;		
						}
					}
				else
					{
					iNonMbmIconFile = ETrue;
					}
				}
				            //File is of size 0 with Valid filename 
			else
                {
                aUseDefaultIcons = ETrue; 
                }
            }
			//If the filename is not a valid name then the file is treated as a non-mbm file.
		else
			{
			iNonMbmIconFile = ETrue;
			}
		}

	// read LEN WORD STRUCT view_list[]
	const TInt numOfViews = resourceReader.ReadInt16L();
	if (numOfViews > 0)
		iViewDataArray = new(ELeave) CArrayPtrFlat<CApaAppViewData>(1);

	for(TInt view = 0; view < numOfViews; ++view)
		{
		CApaAppViewData* viewData = CApaAppViewData::NewLC();
		resourceReader.ReadUint32L(); // skip over LONG reserved_long
		resourceReader.ReadUint32L(); // skip over LLINK reserved_llink

		// read LONG uid
		const TUid viewUid = {resourceReader.ReadInt32L()};
		viewData->SetUid(viewUid);
		// read LONG screen_mode
		const TInt screenMode = {resourceReader.ReadInt32L()};
		viewData->SetScreenMode(screenMode);

		resourceReader.ReadUint32L(); // skip over LONG reserved_long
		resourceReader.ReadUint32L(); // skip over LLINK reserved_llink

		// read LTEXT caption
		TPtrC viewCaption = resourceReader.ReadTPtrCL();
		viewData->SetCaptionL(viewCaption);
		// read WORD number_of_icons
		const TInt numOfViewIcons = resourceReader.ReadInt16L();
		viewData->SetNumOfViewIcons(numOfViewIcons);

		// read LTEXT icon_file
		TPtrC viewIconFile = resourceReader.ReadTPtrCL();
		HBufC* const fullViewIconFileName = CreateFullIconFileNameL(viewIconFile);
		if (fullViewIconFileName)
			{
			CleanupStack::PushL(fullViewIconFileName);
			viewIconFile.Set(*fullViewIconFileName);
			viewData->SetIconFileNameL(viewIconFile);
			
			if (iFs.IsValidName(viewIconFile))
				{
				if(!FileIsMbmWithGenericExtensionL(viewIconFile))
					viewData->SetNonMbmIconFile(ETrue);
				}
			else	//If the filename is not a valid name then the file is treated as a non-mbm file.
				viewData->SetNonMbmIconFile(ETrue);
			}
		else
			{
			viewIconFile.Set(KNullDesC);
			if (numOfViewIcons > 0 && iIconFileName)
				viewIconFile.Set(*iIconFileName); // default to app icon filename
			}
		if (numOfViewIcons > 0 && iFs.IsValidName(viewIconFile) && FileIsMbmWithGenericExtensionL(viewIconFile))
			{
			CApaAppIconArray* iconArray = CApaAppIconArray::NewViewIconsL(numOfViewIcons, viewIconFile, *iIconLoader);
			viewData->SetIconArray(iconArray);
			iconArray = NULL;
			}

		if (fullViewIconFileName)
			CleanupStack::PopAndDestroy(fullViewIconFileName);

		iViewDataArray->AppendL(viewData);
		CleanupStack::Pop(viewData);
		}

	// Read LTEXT group_name
	// If a localised group name has been specified, it overrides
	// The group name (if any), specified by APP_REGISTRATION_INFO

	TApaAppGroupName groupName;
	TRAPD(ret, (groupName = resourceReader.ReadTPtrCL()));
	if (ret != KErrNone)
		{
		 if (ret != KErrEof)
	         User::Leave(ret);
		}
	else
		{
	     if (groupName.Length() > 0)
			 iCapability.iGroupName = groupName;
		}

	CleanupStack::PopAndDestroy(&resourceReader);
	}


HBufC8* CApaAppInfoReader::ReadOpaqueDataL(TUint aResourceId, const CResourceFile* aRegistrationFile, CResourceFile* aLocalisableResourceFile)
	{ // static
	HBufC8* opaqueData = NULL;
	if (aResourceId == 0)
		opaqueData = HBufC8::NewL(0);
	else
		{
		if (aResourceId & KResourceOffsetMask)
			{
			// expecting opaque data to be in the localisable resource file
			if (aLocalisableResourceFile)
				{
				aLocalisableResourceFile->ConfirmSignatureL();
				opaqueData = aLocalisableResourceFile->AllocReadLC(aResourceId);
				CleanupStack::Pop(opaqueData);
				}
			else
				opaqueData = HBufC8::NewL(0);
			}
		else
			{
			// expecting opaque data to be in the registration file
			__ASSERT_ALWAYS(aRegistrationFile, Panic(EPanicNullPointer));
			opaqueData = aRegistrationFile->AllocReadLC(aResourceId); //lint !e613 Suppress ossible use of null pointer
			CleanupStack::Pop(opaqueData);
			}
		}

	return opaqueData;
	}

void CApaAppInfoReader::ReadNonLocalisableOptionalInfoL(RResourceReader& aResourceReader, const CResourceFile* aRegistrationFile, CResourceFile* aLocalisableResourceFile)
	{
	// read LEN WORD STRUCT service_list[]
	TInt serviceCount = 0;
	// service information was not present in the first release of the registration file
	// APP_REGISTRATION_INFO resource struct
	// this method must not leave if the registration file doesn't contain service information, so the
	// following call to ReadInt16L is trapped to ensure this method doesn't leave just because
	// there is no more information in the resource to read (KErrEof)
	TRAPD(err, serviceCount = aResourceReader.ReadInt16L());
	if (err)
		{
		if (err == KErrEof)
			return; // end of resource reached
		
		User::Leave(err);
		}
	
	if (!iServiceArray && serviceCount > 0)
		iServiceArray = new(ELeave) CArrayFixFlat<TApaAppServiceInfo>(4);

	while (serviceCount--)
		{
		const TUid serviceUid = {aResourceReader.ReadUint32L()};
		
		if ((serviceUid == KOpenServiceUid) && (iOpenServiceIsLegacy))
			{
			ASSERT(iIndexOfFirstOpenService == 0);
			// If we found an Open service in the SERVICE_INFO declaration
			// then we must ignore the legacy one
			(*iServiceArray)[0].Release();
			iServiceArray->Delete(0);
			iOpenServiceIsLegacy = EFalse;
			iIndexOfFirstOpenService = KErrNotFound;
			}
		
		CArrayFixFlat<TDataTypeWithPriority>* mimeTypesSupported = new(ELeave) CArrayFixFlat<TDataTypeWithPriority>(5);
		CleanupStack::PushL(mimeTypesSupported);
		ReadMimeTypesSupportedL(aResourceReader, *mimeTypesSupported);
		
		const TUint resourceId = aResourceReader.ReadUint32L();
		HBufC8* opaqueData = NULL;
		opaqueData = ReadOpaqueDataL(resourceId, aRegistrationFile, aLocalisableResourceFile);
		CleanupStack::PushL(opaqueData);
		
		TApaAppServiceInfo serviceInfo(serviceUid, mimeTypesSupported, opaqueData); // takes ownership of opaqueData
		iServiceArray->AppendL(serviceInfo);
		CleanupStack::Pop(opaqueData);
		CleanupStack::Pop(mimeTypesSupported);
		if ((serviceUid == KOpenServiceUid) && (iIndexOfFirstOpenService < 0))
			iIndexOfFirstOpenService = iServiceArray->Count() - 1;
		}

	// read LLINK opaque_data
	const TUint resourceId = aResourceReader.ReadUint32L();
	delete iOpaqueData;
	iOpaqueData = NULL;
	iOpaqueData = ReadOpaqueDataL(resourceId, aRegistrationFile, aLocalisableResourceFile);
	}

void CApaAppInfoReader::ReadNonLocalisableInfoL(RResourceReader& aResourceReader, CResourceFile*& aLocalisableResourceFile, TUint& aLocalisableResourceId)
	{
	ASSERT(aLocalisableResourceFile == NULL);

	// read LTEXT localisable_resource_file
	TPtrC localisableResourceFileName(aResourceReader.ReadTPtrCL());
	if (localisableResourceFileName.Length() > 0 && iFs.IsValidName(localisableResourceFileName))
		{
		// determine the language specific name of the localisable resource file
		TParse parse;
		TParsePtrC parsePtr(iRegistrationFileName);
		User::LeaveIfError(parse.SetNoWild(parsePtr.Drive(), &KAppResourceFileExtension, &localisableResourceFileName));
		TFileName resourceFileName(parse.FullName());
		BaflUtils::NearestLanguageFileV2(iFs, resourceFileName, iApplicationLanguage);
		iLocalisableResourceFileName = resourceFileName.AllocL();

		TEntry entry;
		User::LeaveIfError(iFs.Entry(*iLocalisableResourceFileName, entry));
		iLocalisableResourceFileTimeStamp = entry.iModified;
	

		// open the localisable resource file	
		aLocalisableResourceFile = CResourceFile::NewL(iFs, resourceFileName, 0, 0);
		}

	// read LONG localisable_resource_id
	aLocalisableResourceId = aResourceReader.ReadUint32L();
	if (aLocalisableResourceFile && (aLocalisableResourceId & KResourceOffsetMask))
		aLocalisableResourceFile->ConfirmSignatureL();

	iCapability.iAppIsHidden = aResourceReader.ReadInt8L();
	iCapability.iEmbeddability = static_cast<TApaAppCapability::TEmbeddability>(aResourceReader.ReadInt8L());
	iCapability.iSupportsNewFile = aResourceReader.ReadInt8L();
	iCapability.iLaunchInBackground = aResourceReader.ReadInt8L();
	iCapability.iGroupName = aResourceReader.ReadTPtrCL();

	// read BYTE default_screen_number
	iDefaultScreenNumber = aResourceReader.ReadUint8L();
 	
	//read the datatypes
	CArrayFixFlat<TDataTypeWithPriority>* datatypes = new(ELeave) CArrayFixFlat<TDataTypeWithPriority>(5);
	CleanupStack::PushL(datatypes);
	ReadMimeTypesSupportedL(aResourceReader, *datatypes);
	//dataTypes is deleted if 
	// A. There are no legacy datatypes
	// B. Control panel plugin apps are not allowed to register MIME types.If they happen to have any, these datatypes should be ignored.
	if ((iCapability.iAttributes & TApaAppCapability::EControlPanelItem) || (datatypes->Count() == 0))
		CleanupStack::PopAndDestroy(datatypes);
	else
		{
		ASSERT(!iServiceArray);
		iServiceArray = new(ELeave) CArrayFixFlat<TApaAppServiceInfo>(4);
		HBufC8* opaqueData = HBufC8::NewL(0);
		CleanupStack::PushL(opaqueData);
		TApaAppServiceInfo serviceInfo(KOpenServiceUid, datatypes, opaqueData);
		iServiceArray->AppendL(serviceInfo);
		CleanupStack::Pop(opaqueData);
		CleanupStack::Pop(datatypes);
		iIndexOfFirstOpenService = 0;
		iOpenServiceIsLegacy = ETrue;
		}

	// read LEN WORD STRUCT file_ownership_list[]
	const TInt fileOwnershipArraySize = aResourceReader.ReadInt16L();
	if (fileOwnershipArraySize > 0)
		iOwnedFileArray = new(ELeave) CDesCArraySeg(1);

	for (TInt i=0; i < fileOwnershipArraySize; i++)
		{
		TPtrC fileNamePtr = aResourceReader.ReadTPtrCL();
		iOwnedFileArray->AppendL(fileNamePtr);
		}
	}
	
void CApaAppInfoReader::ReadMimeTypesSupportedL(RResourceReader& aResourceReader,
	CArrayFixFlat<TDataTypeWithPriority>& aMimeTypesSupported)
	{
	// read LEN WORD STRUCT datatype_list[]
	const TInt dataTypeArraySize = aResourceReader.ReadInt16L();
	if (dataTypeArraySize <= 0)
		return;
	
	for (TInt i=0; i < dataTypeArraySize; i++)
		{
		TDataTypePriority priority = static_cast<TDataTypePriority>(aResourceReader.ReadInt32L());
        
		//Check for data priority of UnTrusted apps however the trusted apps will not have any restrictions 
		//over the data priority.	
		//If an untrusted app has write device data capability (i.e. still has priority = KDataTypePrioritySystem),
		//do not restrict to KDataTypeUnTrustedPriorityThreshold
		if (priority > KDataTypeUnTrustedPriorityThreshold || priority == KDataTypePrioritySystem )
		    {
		    ReadAppSecurityInfo();

            if (priority == KDataTypePrioritySystem)
                {
                // Check that the app has capability WriteDeviceData
                if (!iHasWriteDeviceDataCap)
                    priority = KDataTypePriorityNormal;
                }
            else
                {
                //data priority for UnTrusted apps would be capped if it is greater than the threshold priority i.e, KMaxTInt16.
                TInt match=iRegistrationFileName.MatchF(KLitPathForUntrustedRegistrationResourceFiles);
                if (match != KErrNotFound && !iIsSidTrusted) 
                    {
                    //if registration file is in import directory and 
                    //its sid is in unprotected range - downgrade the priority
                    priority = KDataTypeUnTrustedPriorityThreshold;	
                    }
                }
		    }

		TPtrC8 dataTypePtr = aResourceReader.ReadTPtrC8L();
		TDataType dataType(dataTypePtr);
		TDataTypeWithPriority dataTypeWithPriority(dataType, priority);
		aMimeTypesSupported.AppendL(dataTypeWithPriority);
		}
	}


HBufC* CApaAppInfoReader::CreateFullIconFileNameL(const TDesC& aIconFileName) const
    {
    HBufC* filename = NULL;
    if (aIconFileName.Length() == 0)
        return NULL;
    /*
     * aIconFileName may contain a valid string in some format (for eg. URI format) other than path to a regular file on disk
     * and that can be a mbm or non-mbm file. Such a filename will be reported as invalid filename by iFs.IsValidName() method. 
     * aIconFileName will be returned since it is a valid string. 
     */ 
    if(!iFs.IsValidName(aIconFileName))
        {
        filename = aIconFileName.AllocL();
        return filename;
        }
    
    TParsePtrC parsePtr(aIconFileName);
    if (parsePtr.IsWild() || !parsePtr.PathPresent() || !parsePtr.NamePresent())
        return NULL;

    // check for fully qualified icon filename
    if (parsePtr.DrivePresent() && BaflUtils::FileExists(iFs, aIconFileName))
        filename = aIconFileName.AllocL();
    else
        {
        // check for icon file on same drive as localisable resource file
        TParse parse;
        TPtrC localisableResourceFileDrive = TParsePtrC(*iLocalisableResourceFileName).Drive();
        TInt ret = parse.SetNoWild(localisableResourceFileDrive, &aIconFileName, NULL);
        if (ret == KErrNone && BaflUtils::FileExists(iFs, parse.FullName()))
            filename = parse.FullName().AllocL();
        else
            {
            TPtrC registrationFileDrive = TParsePtrC(iRegistrationFileName).Drive();
            if (TInt(TDriveUnit(registrationFileDrive)) != TInt(TDriveUnit(localisableResourceFileDrive)))
                {
                // check for icon file on same drive as registration file
                ret = parse.SetNoWild(registrationFileDrive, &aIconFileName, NULL);
                if (ret == KErrNone && BaflUtils::FileExists(iFs, parse.FullName()))
                    filename = parse.FullName().AllocL();
                }
            }
        }

    return filename;
    }

#endif

/*An MBM file may have a generic icon extension. In this case, as a way to check whether the file is an MBM one, 
it is necessary to read the content of the fist four 32bit words of it and find out whether these words correspond to 
KWriteonceFileStoreUid, KMultiBitmapFileImageUid, zero and KMultiBitmapFileImageChecksum respectively (defined in graphics/gditools/bmconv/bmconv.h).
So the file is opened and the first 4 32 bit words are extracted and compared with the header information of standard MBM file.
If they match, the function returns ETrue, else it returns EFalse */
TBool CApaAppInfoReader::FileIsMbmWithGenericExtensionL(const TDesC& aFileName)
      { 
      if (aFileName.Length() > 0) 
            { 
            //open a file in Share mode - this will allow other methods to access it too
            RFile file;
            RFs fs;
            User::LeaveIfError(fs.Connect());
            CleanupClosePushL(fs);
            User::LeaveIfError(file.Open(fs,aFileName,EFileShareReadersOnly));
            //this is done beacuse the file can also be accessed by applist at the same time
            //buffer stores the 16 bytes of the file
            CleanupClosePushL(file);
            TBuf8<16> buffer;
            User::LeaveIfError(file.Read(buffer,16));
            CleanupStack::PopAndDestroy();//file
            CleanupStack::PopAndDestroy(&fs);//file, fs
            //we use a constant pointer to the buffer to read header info
            TPtrC8 filePointer(buffer);
            
            /*The first 16 bytes of an MBM file are the same for any generic MBM file.
            These are :
            KWriteOnceFileStoreUid = 0x10000037(Emulator MBM file) 0x10000041(ROM image)    
            KMultiBitMapFileImageUid = 0x10000042(Emulator MBM file)    0x00000001(ROM image)
            Zero = 0x00000000(Emulator MBM file) 0x0000000C(ROM image)
            checksum = 0x47396439(Emulator MBM file) 0x10000040(ROM image)
            The first 16 bytes of the given file is compared with these standard values to ascertain it is MBM file*/
            if((filePointer[3]==0x10)&&(filePointer[2]==0x00)&&(filePointer[1]==0x00)&&(filePointer[0]==0x37))
                {//KWriteOnceFileStoreUid = 0x10000037
                if((filePointer[7]==0x10)&&(filePointer[6]==0x00)&&(filePointer[5]==0x00)&&(filePointer[4]==0x42))
                    {//KMultiBitMapFileImageUid = 0x10000042
                    if((filePointer[11]==0x00)&&(filePointer[10]==0x00)&&(filePointer[9]==0x00)&&(filePointer[8]==0x00))
                        {//Zero = 0x00000000)
                        if((filePointer[15]==0x47)&&(filePointer[14]==0x39)&&(filePointer[13]==0x64)&&(filePointer[12]==0x39))
                            {//checksum = 0x47396439
                            return ETrue;
                            }
                        }
                    }
                }
            //Else Check for ROM Image MBM file's header
            else if((filePointer[3]==0x10)&&(filePointer[2]==0x00)&&(filePointer[1]==0x00)&&(filePointer[0]==0x41))
                {//KWriteOnceFileStoreUid = 0x10000041
                if((filePointer[7]==0x00)&&(filePointer[6]==0x00)&&(filePointer[5]==0x00)&&(filePointer[4]==0x01))
                    {//KMultiBitMapFileImageUid = 0x00000001
                    if((filePointer[11]==0x00)&&(filePointer[10]==0x00)&&(filePointer[9]==0x00)&&(filePointer[8]==0x0C))
                        {//Zero = 0x0000000C)
                        if((filePointer[15]==0x10)&&(filePointer[14]==0x00)&&(filePointer[13]==0x00)&&(filePointer[12]==0x40))
                            {//checksum = 0x10000040
                            return ETrue;
                            }
                        }
                    }
                }
            }
      return EFalse;
      }

// This method can be used to check whether app has a WriteDeviceCap 
// and its sid is trusted
void CApaAppInfoReader::ReadAppSecurityInfo()
    {
    if (!iSecurityInfoHasBeenRead)
        {
        __ASSERT_DEBUG( iAppBinaryFullName, Panic(EPanicNullPointer) );
        const TInt err = CApaSecurityUtils::CheckAppSecurity( *iAppBinaryFullName, 
                                        iHasWriteDeviceDataCap, iIsSidTrusted);
        if ( KErrNone != err )
            {
            iHasWriteDeviceDataCap = EFalse;
            iIsSidTrusted = EFalse;
            }

        iSecurityInfoHasBeenRead = ETrue;   
        }
    }

//
// Class CApaIconLoader
//

CApaIconLoader* CApaIconLoader::NewL(RFs& aFs)
	{
	CApaIconLoader* self = NewLC(aFs);
	CleanupStack::Pop(self);
	return self;
	}

CApaIconLoader* CApaIconLoader::NewLC(RFs& aFs)
	{
	CApaIconLoader* self = new(ELeave) CApaIconLoader(aFs);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CApaIconLoader::CApaIconLoader(RFs& aFs) : iFs(aFs)
	{
	}

void CApaIconLoader::ConstructL()
	{
	} //lint !e1762 Suppress member function could be made const


CApaIconLoader::~CApaIconLoader()
	{
	TInt arrayCount = iIconIndexArray.Count();
	while (--arrayCount >= 0)
		delete (iIconIndexArray[arrayCount].iFileName);

	iIconIndexArray.Close();
	iIconArrays.Close();
	}

void CApaIconLoader::AddIconArrayL(const CApaAppIconArray& aIcons)
	{
	iIconArrays.AppendL(&aIcons);
	}

void CApaIconLoader::LoadAllIconsL()
	{
	const TInt iconArraysCount = iIconArrays.Count();
	CleanupClosePushL(iIconArrays);
	for (TInt ii = 0; ii <iconArraysCount; ++ii)
		iIconArrays[ii]->LoadIconsL();
	CleanupStack::PopAndDestroy(&iIconArrays);
	}

// given an mbm filename, returns the next index to read from the file
// always set aUseCache to EFalse on first call in a sequence of calls
TInt CApaIconLoader::IconIndexL(const TDesC& aFileName, TBool& aUseCache)
	{
	if (aUseCache)
		{
		TInt ret = iIconIndexArray[iCachedArrayIndex].iIndex++;
		return ret;
		}
	else
	    aUseCache = ETrue;	    

	// if filename in array, get the next index
	TInt ret = 0;
	const TInt arrayCount = iIconIndexArray.Count();
	TInt arrayIndex;
	for (arrayIndex = 0; arrayIndex < arrayCount; arrayIndex++)
		{
		__ASSERT_DEBUG(iIconIndexArray[arrayIndex].iFileName, Panic(EDPanicInvalidIconIndexArray));
		if (iIconIndexArray[arrayIndex].iFileName->CompareF(aFileName) == 0)
			{
			ret = iIconIndexArray[arrayIndex].iIndex++;
			iCachedArrayIndex = arrayIndex;
			break;
			}
		}

	if (arrayIndex >= arrayCount)
		{
		// filename not found, add it to array
		TKeyValuePair keyValuePair;
		keyValuePair.iFileName = aFileName.AllocL();
		keyValuePair.iIndex = 0;
		ret = keyValuePair.iIndex++;
		User::LeaveIfError(iIconIndexArray.Append(keyValuePair));
		iCachedArrayIndex = arrayCount;
		}

	return ret;
	}
	


// returns EFalse if there was an error obtaining aMbmFileName's entry information,
// otherwise returns ETrue.
// Leaves if an error occurs while trying to populate aIcons or sort icons
TBool CApaIconLoader::LoadIconsL(TInt aNumOfIcons, const TDesC& aMbmFileName, CArrayPtr<CApaMaskedBitmap>& aIcons)
	{
  
	TEntry entry;
	TInt error=iFs.Entry(aMbmFileName,entry);
	if (error!=KErrNone)
		return EFalse;

	TInt iconIndex;
	TInt fileIndex;
	TBool useCache = EFalse;
	
	// create link to CApaAppList which owns this object as it has a separate share protected RFs
	// which can be used to load bitmap icons 
	CApaAppList* const appList = CApaAppList::Self();
	
	RFile mbmFile;
	CleanupClosePushL(mbmFile);
	User::LeaveIfError(mbmFile.Open(appList->ShareProtectedFileServer(), aMbmFileName, EFileShareReadersOnly));

	for(iconIndex=0; iconIndex<aNumOfIcons; ++iconIndex)	
		{		
		CApaMaskedBitmap* bitmap = CApaMaskedBitmap::NewLC();
		fileIndex = IconIndexL(aMbmFileName, useCache);
		User::LeaveIfError(bitmap->Load(mbmFile, 2*fileIndex));
		User::LeaveIfError((bitmap->Mask())->Load(mbmFile,2*fileIndex+1));   
		
		aIcons.AppendL(bitmap);
		CleanupStack::Pop(bitmap);		
		}
	
	CleanupStack::PopAndDestroy(&mbmFile); // close mbmFile	

	// now sort them in ascending order of size
	TInt numberOfIcons = aIcons.Count();
	// create a new array that we can sort by area, populated from aIcons
	RPointerArray<CApaMaskedBitmap> ptrArray(numberOfIcons);
	CleanupClosePushL(ptrArray);
	TLinearOrder<CApaMaskedBitmap> order(CApaIconLoader::CompareIcons);
	for (iconIndex=0; iconIndex<numberOfIcons; iconIndex++)
		User::LeaveIfError(ptrArray.InsertInOrderAllowRepeats(aIcons[iconIndex], order));

	// copy the sorted icon pointers back into aIcons - must not Leave inside the loop below
	for (iconIndex=0; iconIndex<numberOfIcons; iconIndex++)
		aIcons[iconIndex]=ptrArray[iconIndex];

	CleanupStack::PopAndDestroy(&ptrArray);

	return ETrue;
	}

TInt CApaIconLoader::CompareIcons(const CApaMaskedBitmap& aFirst, const CApaMaskedBitmap& aSecond)
	{ // static
	TSize sizeFirst = aFirst.SizeInPixels();
	TInt areaFirst = sizeFirst.iWidth*sizeFirst.iHeight;
	TSize sizeSecond = aSecond.SizeInPixels();
	TInt areaSecond = sizeSecond.iWidth*sizeSecond.iHeight;
	return areaFirst - areaSecond;
	}

//
// Class CApaAppIconArray
//

CApaAppIconArray* CApaAppIconArray::NewL()
	{
	return new(ELeave) CApaAppIconArray();
	}

/* public factory functions */
CApaAppIconArray* CApaAppIconArray::NewAppIconsL(TInt aNumOfIcons, const TDesC& aMbmFileName, CApaIconLoader& aIconLoader)
	{
	return NewL(aNumOfIcons,aMbmFileName,aIconLoader,ETrue);
	}

CApaAppIconArray* CApaAppIconArray::NewViewIconsL(TInt aNumOfIcons, const TDesC& aMbmFileName, CApaIconLoader& aIconLoader)
	{
	return NewL(aNumOfIcons,aMbmFileName,aIconLoader,EFalse);
	}

CApaAppIconArray* CApaAppIconArray::NewDefaultIconsL()
	{
	CApaAppIconArray* self = NewL();
	CleanupStack::PushL(self);
	self->GetDefaultIconsL();
	CleanupStack::Pop(self);
	return self;
	}

CApaAppIconArray* CApaAppIconArray::NewRealDefaultIconsLC(TInt aNumOfIcons, const TDesC& aMbmFileName, CApaIconLoader& aIconLoader)
	{
	return NewLC(aNumOfIcons,aMbmFileName,aIconLoader,EFalse);
	}

/* real NewL and NewLC, private */
CApaAppIconArray* CApaAppIconArray::NewL(TInt aNumOfIcons, const TDesC& aMbmFileName, CApaIconLoader& aIconLoader, TBool aFallbackToDefaultIcons)
	{
	CApaAppIconArray* self = NewLC(aNumOfIcons,aMbmFileName,aIconLoader,aFallbackToDefaultIcons);
	CleanupStack::Pop(self);
	return self;
	}

CApaAppIconArray* CApaAppIconArray::NewLC(TInt aNumOfIcons, const TDesC& aMbmFileName, CApaIconLoader& aIconLoader, TBool aFallbackToDefaultIcons)
	{
	CApaAppIconArray* self = new(ELeave) CApaAppIconArray(aNumOfIcons,aIconLoader,aFallbackToDefaultIcons);
	CleanupStack::PushL(self);
	self->ConstructL(aMbmFileName);
	return self;
	}

CApaAppIconArray::CApaAppIconArray()
	{
	}

CApaAppIconArray::CApaAppIconArray(TInt aNumOfIcons, CApaIconLoader& aIconLoader, TBool aFallbackToDefaultIcons) :
		iNumOfIcons(aNumOfIcons),
		iIconLoader(&aIconLoader),
		iFallbackToDefaultIcons(aFallbackToDefaultIcons)
	{
	}

void CApaAppIconArray::ConstructL(const TDesC& aMbmFileName)
	{
	iMbmFileName = aMbmFileName.AllocL();
	ASSERT(iIconLoader);
	iIconLoader->AddIconArrayL(*this);
	}

CApaAppIconArray::~CApaAppIconArray()
	{
	if(iDefaultIconsUsed)
		{
		ASSERT(iDefaultIconsProvider);
		if(iDefaultIconsProvider)
			iDefaultIconsProvider->ReleaseDefaultIconArray();
		}

	delete iMbmFileName;
	if (iIcons)
		{
		iIcons->ResetAndDestroy();
		delete iIcons;
		}

	iDefaultIconsProvider = NULL;
	iIconLoader = NULL;
	}

// returns EFalse if there was an error obtaining iMbmFileName's entry information,
// otherwise returns ETrue or leaves with a system-wide error code
TBool CApaAppIconArray::LoadIconsL()
	{
	ASSERT(iIconLoader);
	ASSERT(!iDefaultIconsUsed);
	ASSERT(!iIcons);
	iIcons = new(ELeave) CArrayPtrFlat<CApaMaskedBitmap>(5);
	TBool badMbmEntryInfo = EFalse;
	TRAPD(err,badMbmEntryInfo = !iIconLoader->LoadIconsL(iNumOfIcons,*iMbmFileName,*iIcons));

	// We'll be called by the iconLoader at most once, and after we've been called,
	// it might be destroyed. So we shouldn't keep a pointer to it anymore.
	iIconLoader = NULL;

	if ((badMbmEntryInfo || err != KErrNone) && iFallbackToDefaultIcons)
		GetDefaultIconsL();
	else if (badMbmEntryInfo)
		return EFalse;
	else if (err != KErrNone)
		User::Leave(err);

	return ETrue;
	}

CApaMaskedBitmap* CApaAppIconArray::IconBySize(const TSize& aSize) const
	{
	if(iDefaultIconsUsed)
		return DefaultIcons().IconBySize(aSize);
	else if(iIcons == NULL && iIconLoader == NULL)
		{
		// Then we've been created with the default constructor, just pretend
		// there's no icon of the right size
		return NULL;
		}
	else if(iIcons == NULL)
		{
		// Arriving here means the the icons need loading but havn't been loaded yet.
		TRAPD(err, iIconLoader->LoadAllIconsL());
		if (err != KErrNone)
			return NULL;
		}

	const TInt count = iIcons->Count();
	CApaMaskedBitmap* bmp=NULL;
	for (TInt ii=0;ii<count;ii++)
		{
		if ((*iIcons)[ii]->SizeInPixels()==aSize)
			bmp=(*iIcons)[ii];
		}

	if (!bmp)
		{// if the exact size has not been found get one with nearest but smaller area
		TInt differenceOfClosest=KMaxTInt;
		TInt indexOfClosest=KMaxTInt;
		TInt reqArea=aSize.iHeight*aSize.iWidth;
		TSize bmpSize;
		TInt difference;
		for (TInt jj=0;jj<count;jj++)
			{
			bmpSize=(*iIcons)[jj]->SizeInPixels();
			difference=reqArea-bmpSize.iHeight*bmpSize.iWidth;
			if (difference<differenceOfClosest && difference>=0)
				{
				differenceOfClosest=difference;
				indexOfClosest=jj;
				}
			}

		if (indexOfClosest<KMaxTInt)
			bmp=(*iIcons)[indexOfClosest];
		}

	return bmp;
	}

CArrayFixFlat<TSize>* CApaAppIconArray::IconSizesL() const
	{
	if(iDefaultIconsUsed)
		return DefaultIcons().IconSizesL();
	else if(iIcons == NULL && iIconLoader == NULL)
		return new(ELeave)CArrayFixFlat<TSize>(1);
	else if(iIcons == NULL)
		{
		// Arriving here means the the icons need loading but havn't been loaded yet.
		TRAPD(err, iIconLoader->LoadAllIconsL());
		if (err != KErrNone)
			return new(ELeave)CArrayFixFlat<TSize>(1);
		}

	const TInt count=iIcons->Count();
	CArrayFixFlat<TSize>* arrayOfSizes=new(ELeave)CArrayFixFlat<TSize>(1);
	CleanupStack::PushL(arrayOfSizes);
	for(TInt ii=0;ii<count;ii++)
		arrayOfSizes->AppendL((*iIcons)[ii]->SizeInPixels());

	CleanupStack::Pop(arrayOfSizes);
	return arrayOfSizes;
	}

TInt CApaAppIconArray::Count() const
	{
	if(iDefaultIconsUsed)
		return DefaultIcons().Count();
	else if(iIcons == NULL && iIconLoader == NULL)
		return 0;
	else if(iIcons == NULL)
		{
		// Arriving here means the the icons need loading but havn't been loaded yet.
		TRAPD(err, iIconLoader->LoadAllIconsL());
		if (err != KErrNone)
			return 0;
		}

	return iIcons->Count();
	}

TBool CApaAppIconArray::DefaultIconsUsed() const
	{
	return iDefaultIconsUsed;
	}

CApaMaskedBitmap* CApaAppIconArray::operator[](TInt aIndex) const
	{
	if(iDefaultIconsUsed)
		return DefaultIcons()[aIndex];
	else if(iIcons == NULL && iIconLoader == NULL)
		{
		Panic(EPanicIndexOutOfRange);
		return NULL;
		}
	else if(iIcons == NULL)
		{
		// Arriving here means the the icons need loading but havn't been loaded yet.
		TRAPD(err, iIconLoader->LoadAllIconsL());
		if (err != KErrNone)
			return NULL;
		}

	return (*iIcons)[aIndex];
	}

TBool CApaAppIconArray::AreAppIconsLoaded() const
	{
	if (iDefaultIconsUsed)
		return ETrue;

	if (iNumOfIcons > 0 && !iIcons)
		return EFalse;	

	return ETrue;
	}

TBool CApaAppIconArray::AreViewIconsLoaded() const
	{
	if (iNumOfIcons > 0 && !iIcons )
		return EFalse;

	return ETrue;	
	}

void CApaAppIconArray::GetDefaultIconsL()
	{
	ASSERT(!iDefaultIconsUsed);
	iDefaultIconsProvider = CApaAppList::Self();
	ASSERT(iDefaultIconsProvider);
	iDefaultIconsProvider->AcquireDefaultIconArrayL();
	iDefaultIconsUsed = ETrue;
	delete iMbmFileName;
	iMbmFileName = NULL;
	if (iIcons)
		{
	 	iIcons->ResetAndDestroy();
		delete iIcons;
		iIcons = NULL;
		}
	}

const CApaAppIconArray& CApaAppIconArray::DefaultIcons() const
	{
	ASSERT(iDefaultIconsUsed && iDefaultIconsProvider);
	return iDefaultIconsProvider->DefaultIconArray();
	}




