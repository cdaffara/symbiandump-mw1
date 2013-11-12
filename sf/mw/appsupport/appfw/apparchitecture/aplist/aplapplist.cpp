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
// aplapplist.cpp
//

#include "aplapplist.h"
#include "APGAPLST.H"			// KDefaultAppIconMbm
#include "aplapplistitem.h"
#include "../apgrfx/APGSTD.H"	// EPanicNullPointer
#include "APFDEF.H"
#include "../apparc/TRACE.H"
#include "apgnotif.h"			// MApaAppListServObserver
#include <bautils.h>			// BaflUtils::NearestLanguageFile()
#include <s32mem.h>				// RBufWriteStream
#include "aplappinforeader.h"
#include "apsiconcaptionoverride.h"
#ifdef SYMBIAN_BAFL_SYSUTIL
#include <bafl/sysutil.h>
#endif

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
#include<usif/scr/scr.h>
#include<usif/scr/appregentries.h>
#else
#include "aplappregfinder.h"    // CApaAppRegFinder
#endif


// Delays in the pseudo idle object that builds the application list
//

const TInt KIdleStartDelay=0;
const TInt KBufferExpansionGranularity = 0x100;
const TInt KNumberOfIconsInDefaultMbm = 3;
const TInt KAppListToFileStartDelay = 60000000;
#ifdef SYMBIAN_BAFL_SYSUTIL
const TInt KInfoBufLength=KSysUtilVersionTextLength;
//This file is used for storing the rom version. If actual rom version differs from the one stored here the applist is not restored.
//This file is versioned to avoid a data compatability break on adding further information to this file.
//On adding further info to this file, KROMVersionCacheFileMajorVersion or KROMVersionCacheFileMinorVersion or KROMVersionCacheFileBuildVersion needs to be incremented appropriately.
_LIT(KROMVersionStringCacheFileName, "ROMVersionCache.bin");
const TInt8 KROMVersionCacheFileMajorVersion=1;
const TInt8 KROMVersionCacheFileMinorVersion=0;
const TInt16 KROMVersionCacheFileBuildVersion=0;
#endif

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
const TInt KNumAppEntriesFromSCR=10;

const TInt KSCRConnectionWaitTime=20000; //Time to wait if SCR is busy
const TUid KUidSisLaunchServer={0x1020473f};
#endif


GLDEF_C void Panic(TApgPanic aPanic)
	{
	_LIT(KApGrfxPanic,"APGRFX");
	User::Panic(KApGrfxPanic,aPanic);
	}


//
// Class CApaLangChangeMonitor
//

/**
Utility class used to monitor locale/language change event.
@internalComponent
*/
NONSHARABLE_CLASS(CApaAppList::CApaLangChangeMonitor) : public CActive
	{
public:
	static CApaLangChangeMonitor* NewL(CApaAppList& aAppList);
	~CApaLangChangeMonitor();
	void Start();
private:
	CApaLangChangeMonitor(CApaAppList& aAppList);
	void ConstructL();
private:	//from CActive
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);
private:
	RChangeNotifier iLangNotifier;
	CApaAppList& iAppList;
	TLanguage iPrevLanguage;
	};


#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK

enum TApaSCRFetchAction
    {
    EGetAllAppsInfo, //Fetch all application the information from SCR
    EGetSpecificAppsInfo //Fetch only provided application uids information
    };

/*
 * Contain information about appliations to be fetched from SCR.
 */

NONSHARABLE_CLASS(CApaAppSCRFetchInfo : public CBase)
    { 
public:
    static CApaAppSCRFetchInfo* NewL(TApaSCRFetchAction aSCRFetchAction, RArray<TApaAppUpdateInfo>* aAppUpdateInfo);
    ~CApaAppSCRFetchInfo();
    RArray<TApaAppUpdateInfo>* AppUpdateInfo();
    TApaSCRFetchAction SCRFetchAction();
    
private:
    CApaAppSCRFetchInfo(TApaSCRFetchAction aSCRFetchAction, RArray<TApaAppUpdateInfo>* aAppUpdateInfo);
    
private:
    TApaSCRFetchAction iSCRFetchAction;
    RArray<TApaAppUpdateInfo>* iAppUpdateInfo;
    };


/*
 * Reads multiple application information from SCR and caches it. When requested provides one application
 * information at a time.
 */
NONSHARABLE_CLASS(CApaAppList::CApaScrAppInfo)
    {
public:
    static CApaScrAppInfo* NewL(const Usif::RSoftwareComponentRegistry& aScrCon, TInt aNumEntries);    
    void GetAllAppsInfoL();
    void GetSpecificAppsInfoL(RArray<TApaAppUpdateInfo>* aAppUpdateInfo);    
    TUid GetNextApplicationInfo(TApaAppUpdateInfo::TApaAppAction& aAppAction, Usif::CApplicationRegistrationData*& aAppData);
    TApaSCRFetchAction GetSCRFetchAction();    
    ~CApaScrAppInfo();
    
private:
    void ConstructL();
    CApaScrAppInfo(const Usif::RSoftwareComponentRegistry& aScr, TInt aNumEntries);
    void GetAppUidListL(RArray<TApaAppUpdateInfo>& aAppUpdateInfoArr, RArray<TUid>& aAppUids);
    TUid GetAllAppsNextApplicationInfoL(TApaAppUpdateInfo::TApaAppAction& aAppAction, Usif::CApplicationRegistrationData*& aAppData); 
    TUid GetSpecificAppsNextApplicationInfoL(TApaAppUpdateInfo::TApaAppAction& aAppAction, Usif::CApplicationRegistrationData*& aAppData);  
private:
    Usif::RApplicationRegistryView iScrAppView;
    RPointerArray<Usif::CApplicationRegistrationData> iAppInfo;
    RPointerArray<CApaAppSCRFetchInfo> iSCRFetchInfoQueue;
    const Usif::RSoftwareComponentRegistry& iSCR;   
    TBool iIsSCRRegViewOpen;
    TInt iSpecificAppsIndex;
    CApaAppSCRFetchInfo* iAppSCRFetchInfo;
    TBool iMoreAppInfo;
    TInt iNumEntriesToFetch;
    };



CApaAppSCRFetchInfo* CApaAppSCRFetchInfo::NewL(TApaSCRFetchAction aSCRFetchAction, RArray<TApaAppUpdateInfo>* aAppUpdateInfo)
            {
            //Ownership of aAppUpdateInfo is transfered to this object.
            CApaAppSCRFetchInfo* self=new (ELeave) CApaAppSCRFetchInfo(aSCRFetchAction, aAppUpdateInfo);
            return(self);
            }


CApaAppSCRFetchInfo::CApaAppSCRFetchInfo(TApaSCRFetchAction aSCRFetchAction, RArray<TApaAppUpdateInfo>* aAppUpdateInfo):
        iSCRFetchAction(aSCRFetchAction),        
        iAppUpdateInfo(aAppUpdateInfo)
        {
        }

CApaAppSCRFetchInfo::~CApaAppSCRFetchInfo()
    {
    delete iAppUpdateInfo;
    }

RArray<TApaAppUpdateInfo>* CApaAppSCRFetchInfo::AppUpdateInfo()
            {
            return iAppUpdateInfo;
            }


TApaSCRFetchAction CApaAppSCRFetchInfo::SCRFetchAction()
    {
    return iSCRFetchAction;
    }


//CApaAppList::CApaScrAppInfo

CApaAppList::CApaScrAppInfo* CApaAppList::CApaScrAppInfo::NewL(const Usif::RSoftwareComponentRegistry& aScrCon, TInt aNumEntries)
    { 
    CApaScrAppInfo* self=new(ELeave) CApaScrAppInfo(aScrCon, aNumEntries);
    CleanupStack::PushL(self);    
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }


CApaAppList::CApaScrAppInfo::CApaScrAppInfo(const Usif::RSoftwareComponentRegistry& aScr, TInt aNumEntries):
        iSCR(aScr),
        iIsSCRRegViewOpen(EFalse),
        iSpecificAppsIndex(-1),
        iAppSCRFetchInfo(NULL),
        iMoreAppInfo(EFalse),
        iNumEntriesToFetch(aNumEntries)
       
    {
    }

void CApaAppList::CApaScrAppInfo::ConstructL()
    {
    }


CApaAppList::CApaScrAppInfo::~CApaScrAppInfo()
    {
    if(iAppSCRFetchInfo)
        {
        delete iAppSCRFetchInfo; 
        iAppSCRFetchInfo=NULL;
        }
    
    iAppInfo.ResetAndDestroy();
    iSCRFetchInfoQueue.ResetAndDestroy();
    iScrAppView.Close();
    }

/*
 * Gets all the application information available in the SCR. It adds SCR fetch info with action EGetAllAppsInfo to a queue 
 * to get all the application information. The information can be obtained one at a time by calling GetNextApplicationInfoL 
 * function.
 */
void CApaAppList::CApaScrAppInfo::GetAllAppsInfoL()
    {
    CApaAppSCRFetchInfo* appSCRFetchInfo = CApaAppSCRFetchInfo::NewL(EGetAllAppsInfo, NULL);
    CleanupStack::PushL(appSCRFetchInfo);      
    iSCRFetchInfoQueue.AppendL(appSCRFetchInfo);
    CleanupStack::Pop();
    }


/*
 * Gets specific application information from the SCR. It adds SCR fetch info request with action EGetSpecificAppsInfo 
 * along with the required uid list to the queue. The information can be obtained one at a time by calling GetNextApplicationInfoL 
 * function.
 */
void CApaAppList::CApaScrAppInfo::GetSpecificAppsInfoL(RArray<TApaAppUpdateInfo>* aAppUpdateInfo)
    {
    CApaAppSCRFetchInfo* appSCRFetchInfo=CApaAppSCRFetchInfo::NewL(EGetSpecificAppsInfo, aAppUpdateInfo);
    CleanupStack::PushL(appSCRFetchInfo);
    iSCRFetchInfoQueue.AppendL(appSCRFetchInfo);
    CleanupStack::Pop();   
    }

/*
 * Create array of uids from TApaAppUpdateInfo array.
 */
void CApaAppList::CApaScrAppInfo::GetAppUidListL(RArray<TApaAppUpdateInfo>& aAppUpdateInfoArr, RArray<TUid>& aAppUids)
    {
    TInt count=aAppUpdateInfoArr.Count();
    
    for(TInt index=0;index<count;index++)
        {
        TApaAppUpdateInfo appUpdateInfo=aAppUpdateInfoArr[index];
        aAppUids.AppendL(appUpdateInfo.iAppUid);
        }
    }


TApaSCRFetchAction CApaAppList::CApaScrAppInfo::GetSCRFetchAction()
{
    return iAppSCRFetchInfo->SCRFetchAction();
}


/*
 * Provides one application information at a time. Returns Null UID if no more application information available.
 * Ownership of aAppData is transfered to calling function.
 */
TUid CApaAppList::CApaScrAppInfo::GetNextApplicationInfo(TApaAppUpdateInfo::TApaAppAction& aAppAction, Usif::CApplicationRegistrationData*& aAppData)
    {
    aAppData=NULL;
    TUid appUid=KNullUid;

    while(appUid==KNullUid)
        {
        //If there is no valid current SCR fetch information, get it from SCR fetch info queue
        if(!iAppSCRFetchInfo)
            {
            if(iSCRFetchInfoQueue.Count()>0)
                {
                //Get next SCR fetch info
                iAppSCRFetchInfo=iSCRFetchInfoQueue[0]; 
                iSCRFetchInfoQueue.Remove(0); 
                iMoreAppInfo=ETrue;               
                }
            else
                {
                //No more SCR fetch information avaialable.
                break;
                }
            }
        
        //Get next application information        
        if(iAppSCRFetchInfo->SCRFetchAction()==EGetAllAppsInfo)
            {
            //If there is a leave with current SCR fetch info, ignore and proceed with next SCR fetch info
            TRAP_IGNORE(appUid=GetAllAppsNextApplicationInfoL(aAppAction, aAppData));
            }
        else
            {
            //If there is a leave with current SCR fetch info, ignore and proceed with next SCR fetch info        
            TRAP_IGNORE(appUid=GetSpecificAppsNextApplicationInfoL(aAppAction, aAppData));       
            }
            
        if(appUid==KNullUid)
            {
            //If no application information avaialble with current fetch action reset the values for next SCR fetch action.
            delete iAppSCRFetchInfo;
            iAppSCRFetchInfo=NULL;
            iScrAppView.Close();
            iIsSCRRegViewOpen=EFalse;            
            }
        }
    
    return(appUid);
    }

TUid CApaAppList::CApaScrAppInfo::GetAllAppsNextApplicationInfoL(TApaAppUpdateInfo::TApaAppAction& aAppAction, Usif::CApplicationRegistrationData*& aAppData)
    {
    TUid appUid=KNullUid;

    if(iAppInfo.Count()==0 && iMoreAppInfo)
        {
        //Open registry view if its not open.
        if(!iIsSCRRegViewOpen)
            {
            TInt err=KErrNone;
            TInt timeOut=KSCRConnectionWaitTime;

            //Retry if an error occurs while opening a SCR view. 
            while(timeOut < KSCRConnectionWaitTime*8)
            {
            TRAP(err, iScrAppView.OpenViewL(iSCR));
            if(err != KErrNone)
                {
                User::After(timeOut);
                timeOut= (2*timeOut); 
                }
            else
                {
                break;
                }
            }
            User::LeaveIfError(err);
            iIsSCRRegViewOpen=ETrue;
            }
        
        //Get next available applications information.
        iScrAppView.GetNextApplicationRegistrationInfoL(iNumEntriesToFetch, iAppInfo);
        if(iAppInfo.Count()<KNumAppEntriesFromSCR)
            iMoreAppInfo=EFalse;
        }

    //If no application information avaialble, return Null UID.
    if(iAppInfo.Count()==0)
        return KNullUid;

    aAppData=iAppInfo[0];
    aAppAction=TApaAppUpdateInfo::EAppPresent;
    iAppInfo.Remove(0);
    appUid=aAppData->AppUid();
    return appUid;
    }


/*
 * Gets next application information when specific applications information requested.
 */
TUid CApaAppList::CApaScrAppInfo::GetSpecificAppsNextApplicationInfoL(TApaAppUpdateInfo::TApaAppAction& aAppAction, Usif::CApplicationRegistrationData*& aAppData)
    {
    TUid appUid=KNullUid;
    TApaAppUpdateInfo::TApaAppAction action = TApaAppUpdateInfo::EAppNotPresent; //To make compiler happy
    Usif::CApplicationRegistrationData* appData=NULL;
    
    while(appUid==KNullUid)
        {
        if(iAppInfo.Count()==0 && iMoreAppInfo)
            {
            //Open registry view if its not open and also provides application uid list for which applist needs to be updated.
            if(!iIsSCRRegViewOpen)
                {
                RArray<TUid> appUids;
                CleanupClosePushL(appUids);
                //Get application uids list.
                GetAppUidListL(*iAppSCRFetchInfo->AppUpdateInfo(), appUids);
                
                TInt err=KErrNone;
                do
                    {
                    TRAP(err, iScrAppView.OpenViewL(iSCR, appUids));
                    if(err)
                        User::After(KSCRConnectionWaitTime);
                    }
                while(err!=KErrNone);

                CleanupStack::PopAndDestroy();
                iIsSCRRegViewOpen=ETrue;
                iSpecificAppsIndex=0;
                }
            
            //Get next available applications information.
            iScrAppView.GetNextApplicationRegistrationInfoL(iNumEntriesToFetch,iAppInfo);
            if(iAppInfo.Count()<KNumAppEntriesFromSCR)
                iMoreAppInfo=EFalse;
            }
    
        RArray<TApaAppUpdateInfo>& appUpdateInfo=*iAppSCRFetchInfo->AppUpdateInfo();
        
        
        if(iSpecificAppsIndex<appUpdateInfo.Count())
            {
            appUid=appUpdateInfo[iSpecificAppsIndex].iAppUid;
            action=appUpdateInfo[iSpecificAppsIndex].iAction;
    
            //If application information avaialable, and if application action is not uninstalled or
            //If application action is uninstalled and the uninstalled application exists in SCR,
            //then get the info and assign to aAppData.
            if(iAppInfo.Count()>0)
                {
                if(iAppInfo[0]->AppUid()==appUid)
                    {
                    appData=iAppInfo[0];
                    iAppInfo.Remove(0);
                    }
                }
            
            iSpecificAppsIndex++;
            
            //If action is not uninstalled, there should be application data in SCR. Otherwise skip the application action.
            if((action!=TApaAppUpdateInfo::EAppNotPresent) && appData==NULL)
                {
                appUid=KNullUid;
                }
            
            }
            //If there are no more applications in the current update applist, break the loop;     
            if(appUpdateInfo.Count()==iSpecificAppsIndex)
                break;        
        }
    
    aAppData=appData;
    aAppAction=action;
    return appUid;
    }

#endif

//
// Local functions
//

void CleanupServiceArray(TAny* aServiceArray)
	{
	__ASSERT_DEBUG(aServiceArray, Panic(EPanicNullPointer));
	CArrayFixFlat<TApaAppServiceInfo>* serviceArray = static_cast<CArrayFixFlat<TApaAppServiceInfo>*>(aServiceArray);
	const TInt serviceCount = serviceArray->Count();
	
	for (TInt i = serviceCount - 1; i >= 0; i--)
		(*serviceArray)[i].Release();

	delete serviceArray;
	serviceArray = 0;
	}


//
// Class CApaAppList
//

EXPORT_C CApaAppList* CApaAppList::NewL(RFs& aFs, TBool aLoadMbmIconsOnDemand, TInt aIdlePeriodicDelay)
    {
    CApaAppList* self=new (ELeave) CApaAppList(aFs, aLoadMbmIconsOnDemand, aIdlePeriodicDelay);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CApaAppList::CApaAppList(RFs& aFs, TBool aLoadMbmIconsOnDemand, TInt aIdlePeriodicDelay)
    :iFs(aFs),
    iFlags(0),
    iIdlePeriodicDelay(aIdlePeriodicDelay),
    iLoadMbmIconsOnDemand(aLoadMbmIconsOnDemand),
    iUninstalledApps(NULL)  
    {
    }


void CApaAppList::ConstructL()
	{
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK    
	iAppRegFinder = CApaAppRegFinder::NewL(iFs);
#endif
	
	User::LeaveIfError(iFsShareProtected.Connect());
	User::LeaveIfError(iFsShareProtected.ShareProtected());
	User::LeaveIfError(Dll::SetTls(this));
	
	//Start language change monitor.
	iAppLangMonitor = CApaLangChangeMonitor::NewL(*this);

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK  	
	const TInt KArrayGranularity = 128;
	iForcedRegistrations = new (ELeave) CDesCArraySeg(KArrayGranularity);
#endif	
	
	// Init the AppsList cache paths
	_LIT(KAppsListCacheFileName, ":\\private\\10003a3f\\AppsListCache\\AppsList.bin");
	_LIT(KAppsListCacheBackUpFileName, ":\\private\\10003a3f\\AppsListCache\\AppsList_Backup.bin");
	_LIT(KAppsListCachePath, ":\\private\\10003a3f\\AppsListCache\\");

	const TChar sysDrive = RFs::GetSystemDriveChar();
	TInt maxSizeofFileName = KAppsListCacheFileName().Length() + 1;
	iAppsListCacheFileName.CreateL(maxSizeofFileName);
	iAppsListCacheFileName.Append(sysDrive);
	iAppsListCacheFileName.Append(KAppsListCacheFileName());
	
	maxSizeofFileName = KAppsListCacheBackUpFileName().Length() + 1;
	iAppsListCacheBackUpFileName.CreateL(maxSizeofFileName);
	iAppsListCacheBackUpFileName.Append(sysDrive);
	iAppsListCacheBackUpFileName.Append(KAppsListCacheBackUpFileName());
	
	maxSizeofFileName = KAppsListCachePath().Length() + 1;
	iAppsListCachePath.CreateL(maxSizeofFileName);
	iAppsListCachePath.Append(sysDrive);
	iAppsListCachePath.Append(KAppsListCachePath());
	
	iIconCaptionOverrides = new(ELeave) CApaIconCaptionOverrides();
	iIconCaptionObserver = CApaIconCaptionCenrepObserver::NewL(*iIconCaptionOverrides);
	iIconCaptionObserver->LoadOverridesL();
	}
	
EXPORT_C CApaAppList::~CApaAppList()
/** Frees all resources prior to destruction, including the application data list. */
	{
	iValidFirstAppData = NULL;
	DeleteAppData();

	iAppData = NULL;
	iObserver = NULL;
	iFsShareProtected.Close();

	delete iDefaultIconArray;
	delete iDefaultAppIconMbmFileName;
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
	delete iAppRegFinder;
    delete iForcedRegistrations;
#endif
	delete iAppIdler;
	delete iAppListStorer;
	delete iAppIconLoader;
	delete iAppLangMonitor;
	delete iIconCaptionObserver;
	delete iIconCaptionOverrides;
	iAppsListCacheFileName.Close();
	iAppsListCacheBackUpFileName.Close();
	iAppsListCachePath.Close();
	
	iCustomAppList.ResetAndDestroy();
	iCustomAppList.Close();

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
	iForceRegAppUids.Close();	
	delete iScrAppInfo;
	iScr.Close();
#endif	
	}


#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK   
// Stop scanning applications if installation or uninstallation has started	
EXPORT_C void CApaAppList::StopScan(TBool aNNAInstall)
	{
	if (aNNAInstall)
		{
		iNNAInstallation = ETrue;
		}
	if (iAppIdler)
		{
		delete iAppIdler;
		iAppIdler=NULL;
		}
	UndoSetPending(iAppData);
	}

// Allow scanning when installation or uninstallation is complete
EXPORT_C void CApaAppList::RestartScanL()
	{
	TRAP_IGNORE(PurgeL());
	StartIdleUpdateL();
	}

EXPORT_C TBool CApaAppList::AppListUpdatePending()
	{
	return iNNAInstallation;
	}
#endif

EXPORT_C void CApaAppList::StartIdleUpdateL()
/** Updates the list asynchronously, using an idle time active object, 
and no observer. When the update is finished, the resulting app 
list is stored. */
	{
	// find the default icons (.mbm file) for applications, wrt current locale
	CreateDefaultAppIconFileNameL();

	// File operation is abandoned if it is in progress.
	if (iAppListStorer && iAppListStorer->IsActive())
		{
		delete iAppListStorer;
		iAppListStorer = NULL;
		}

	// Stops icon loading if it is in progress.
	if (iAppIconLoader && iAppIconLoader->IsActive())
		{
		delete iAppIconLoader;
		iAppIconLoader = NULL;
		}

	// DEF076594 - if changing locale, need to update the default icons here
	// If updating the default icons array fails, the array is left in the state
	// it was in before the call.
	if(iDefaultIconArray)
		{
		TRAP_IGNORE(UpdateDefaultIconsL());
		}

	// start to scan.
	if (iAppIdler)
		{
		delete iAppIdler;
		iAppIdler=NULL;
		}

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
	// DEF072701
	// When performing the update scan let the idle object have lower priority.
	if (IsFirstScanComplete())
		{
		iAppIdler=CPeriodic::NewL(CActive::EPriorityLow);
		}
	else
		{
		iAppIdler=CPeriodic::NewL(CActive::EPriorityStandard);
		}
	SetPending(iAppData);
	iAppRegFinder->FindAllAppsL(CApaAppRegFinder::EScanAllDrives);
    // DEF072701
    // If this is the first scan i.e the boot scan then it may take some time. Thus
    // the periodic delay value should be used so that this process will stop periodically 
    // to allow time for other processes.
    // If this is just a re-scan it should take much less time. Therefore it should just
    // be completed in one go rather than periodically delayed. Thus the delay value
    // should be set to 0.
    iAppIdler->Start(KIdleStartDelay, IsFirstScanComplete()? 0 : iIdlePeriodicDelay, TCallBack(IdleUpdateCallbackL, this));
#else	
	
    iAppIdler=CPeriodic::NewL(CActive::EPriorityStandard);
    iAppIdler->Start(KIdleStartDelay, IsFirstScanComplete()? 0 : iIdlePeriodicDelay, TCallBack(IdleUpdateCallbackL, this));

#endif    
 	}

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 
EXPORT_C void CApaAppList::InitializeApplistL(MApaAppListObserver* aObserver)
    {
    if(!iScr.Handle())
        User::LeaveIfError(iScr.Connect());
    
    if(iScrAppInfo==NULL)
        iScrAppInfo=CApaScrAppInfo::NewL(iScr, KNumAppEntriesFromSCR);
    
    iScrAppInfo->GetAllAppsInfoL();
    
    StartIdleUpdateL(aObserver);   
    }


void CApaAppList::InitializeLangAppListL()
    {
    if(!iScr.Handle())
        User::LeaveIfError(iScr.Connect());
    
    if(iScrAppInfo==NULL) 
        iScrAppInfo=CApaScrAppInfo::NewL(iScr, KNumAppEntriesFromSCR);
    
    iScrAppInfo->GetAllAppsInfoL();
    
    // set iIsLangChangePending=ETrue, to all the application present in the applist    
    CApaAppData* appData=iAppData;    
    while(appData)
        {
        appData->iIsLangChangePending=ETrue;
        appData=appData->iNext;
        }
    }
	
	
EXPORT_C void CApaAppList::UpdateApplistL(MApaAppListObserver* aObserver, RArray<TApaAppUpdateInfo>* aAppUpdateInfo, TUid aSecureID)
    {
    //If update applist is called by SWI, clear force registrations from applist.
    if(aSecureID == KUidSisLaunchServer)
        {
        TInt count=iForceRegAppUids.Count();
        for(TInt index=0; index<count; index++)
            FindAndDeleteApp(iForceRegAppUids[index]);
        iForceRegAppUids.Reset();
        }
    
    if(aAppUpdateInfo->Count() == 0)
        return;
    
    //If SCR connection is not valid then connect.
    if(!iScr.Handle())
        User::LeaveIfError(iScr.Connect());
    
    if(iScrAppInfo==NULL)
        iScrAppInfo=CApaScrAppInfo::NewL(iScr, KNumAppEntriesFromSCR);
    
    iScrAppInfo->GetSpecificAppsInfoL(aAppUpdateInfo);
    
    if(IsIdleUpdateComplete())
        StartIdleUpdateL(aObserver);   
    }

void CleanupAndDestroyAppInfoArray(TAny* aRPArray)
    {
    RPointerArray<Usif::CApplicationRegistrationData>* rpArray = (static_cast<RPointerArray<Usif::CApplicationRegistrationData>*>(aRPArray));
    rpArray->ResetAndDestroy();
    rpArray->Close();
    }


EXPORT_C void CApaAppList::UpdateApplistByForceRegAppsL(RPointerArray<Usif::CApplicationRegistrationData>& aForceRegAppsInfo)
    {
    //Get number of force registered application information.
    TInt count=aForceRegAppsInfo.Count();
    Usif::RSoftwareComponentRegistry scr;
    User::LeaveIfError(scr.Connect());
    CleanupClosePushL(scr);
    
    //As this function takes the ownership of aForceRegAppsInfo, this needs to be destroyed if any leave occurs.
    TCleanupItem cleanup(CleanupAndDestroyAppInfoArray, &aForceRegAppsInfo); 
    CleanupStack::PushL(cleanup);
    
    //Get each force registered application information and add it to applist.
    for(TInt index=0; index<count; index++)
        {
        Usif::CApplicationRegistrationData* appInfo=aForceRegAppsInfo[index];
        CApaAppData* appData = CApaAppData::NewL(*appInfo, iFs, scr);
        TUid appUid=appInfo->AppUid();
        
        //Delete if the application already exist in the applist.
        FindAndDeleteApp(appUid);
        AddToList(appData);
        //Maintain added force registered application uids so that it can be cleared from applist
        //once installation complete.
        iForceRegAppUids.AppendL(appUid);
        }
    
    CleanupStack::PopAndDestroy(2); //cleanup, scr
    }


// The function transfers ownership of the pointer owned by a CApaAppList to the caller
// to avoid copying the array.
EXPORT_C  CArrayFixFlat<TApaAppUpdateInfo>* CApaAppList::UpdatedAppsInfo()
{
    CArrayFixFlat<TApaAppUpdateInfo>* updatedAppsInfo=iAppsUpdated;
    iAppsUpdated=NULL;
    return updatedAppsInfo;
}

#endif

EXPORT_C void CApaAppList::StartIdleUpdateL(MApaAppListObserver* aObserver)
/** Updates the list asynchronously, using an idle time active object 
and an observer. When the update is finished, the resulting app list 
is stored and the observer is notified with an MApaAppListServObserver::EAppListChanged 
message, if the list changed.
@param aObserver Observer to be notified when the update has finished. */
	{
	iObserver=aObserver;

	// Rename Appslist.bin file to AppsList_Backup.bin, so that it can be renamed back, if rescan/update scan does not change applist.
	TInt replaceError = iFs.Replace(iAppsListCacheFileName, iAppsListCacheBackUpFileName);
	if (replaceError != KErrNone)
		{
		iFs.Delete(iAppsListCacheFileName);
		}
	StartIdleUpdateL();
	}

void CApaAppList::CreateDefaultAppIconFileNameL()
	{
	TFileName tempFileName(KDefaultAppIconMbm);
	TLanguage language;
	BaflUtils::NearestLanguageFileV2(iFs, tempFileName, language);
	(void)language;
	delete iDefaultAppIconMbmFileName;
	iDefaultAppIconMbmFileName = NULL;
	iDefaultAppIconMbmFileName = tempFileName.AllocL();
	}

EXPORT_C void CApaAppList::InitListL(MApaAppListObserver* aObserver)
/** It Restores the data present in the AppsList.bin file and updates the applist. 
 * If Restore operation fails then it 
 * starts updating the list asynchronously, by calling StartIdleUpdateL().

@param aObserver Observer to be notified when the update has finished. */
	{
    
#ifdef APPARC_SHOW_TRACE    
    RDebug::Printf("[Apparc] *****************START CREATING APPLIST ****************************");
#endif    
    
	DeleteAppsListBackUpAndTempFiles();
	
	TInt ret = KErrGeneral;
#ifndef __WINS__ // on the emulator, don't read app list from file, as doing so means apps
                 // built while the emulator isn't running won't appear in the list
	TRAP(ret, RestoreL());
#endif
	if (ret != KErrNone)
		{
		// There was an error during restore, so update the list asynchronously.
		DeleteAppData();
		iFs.Delete(iAppsListCacheFileName);
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
		InitializeApplistL(aObserver);		
#else
        StartIdleUpdateL(aObserver);		
#endif
		
		}
	else
		{
		iObserver = aObserver;
		if (iLoadMbmIconsOnDemand)
			{
			InitiateStoringOfAppList();
			}
		else
			{
			StartIconLoadingL();
			}
		}
	}

void CApaAppList::StartIconLoadingL()
	{
	iAppIconLoader = new(ELeave) CApaIdleIconLoader(iAppData, iFs, *this);
	iAppIconLoader->Start();
	}


void CApaAppList::DeleteAppsListBackUpAndTempFiles()
/** Deletes all files inside AppsListCache folder except AppsList.bin */
	{
	_LIT(KTemp, "AppsList.bin");
	CDir* cache = NULL;
	TInt ret = iFs.GetDir(iAppsListCachePath, KEntryAttNormal, ESortNone, cache);
	if (ret == KErrNone)
		{
		const TInt count = cache->Count();
		TBufC<KMaxFileName> cacheFileName;
		TEntry entry;
		for (TInt i = 0; i < count; ++i)
			{
			entry=(*cache)[i];
			cacheFileName=entry.iName;
	#if defined (SYMBIAN_BAFL_SYSUTIL)
			if ((cacheFileName.Match(KTemp) == KErrNotFound) && (cacheFileName.Match(KROMVersionStringCacheFileName) == KErrNotFound)) 
	#else
			if ((cacheFileName.Match(KTemp) == KErrNotFound)) 
	#endif
				{
				TFileName fileNameToDelete;
				fileNameToDelete.Append(iAppsListCachePath);
				fileNameToDelete.Append(cacheFileName);
				iFs.Delete(fileNameToDelete);
				}
			}
		delete cache;
		}
	}

void CApaAppList::RestoreL()
/** It restores the data present in the AppsList.bin file */
	{
	RFileReadStream theReadStream;
	User::LeaveIfError(theReadStream.Open(iFs, iAppsListCacheFileName, EFileRead));
	CleanupClosePushL(theReadStream);
	
	TLanguage appListFileLanguage = (TLanguage)theReadStream.ReadInt32L();
	if (appListFileLanguage != User::Language())
		{
		User::Leave(KErrNotSupported);
		}
	
#if defined (SYMBIAN_BAFL_SYSUTIL)
	//Build the filename for the cache file
	TInt maxSizeofFileName = iAppsListCachePath.Length() + KROMVersionStringCacheFileName().Length();
	RBuf romVersionCacheFileName;
	romVersionCacheFileName.CreateL(maxSizeofFileName);
	romVersionCacheFileName.CleanupClosePushL();
	romVersionCacheFileName.Append(iAppsListCachePath);
	romVersionCacheFileName.Append(KROMVersionStringCacheFileName());
	
	RFileReadStream romVerStream;
	User::LeaveIfError(romVerStream.Open(iFs,romVersionCacheFileName,EFileRead));
	CleanupClosePushL(romVerStream);
	
	TVersion actualROMVersionCacheFileVersion(KROMVersionCacheFileMajorVersion, KROMVersionCacheFileMinorVersion, KROMVersionCacheFileBuildVersion);
	TVersionName actualROMVersionCacheFileVersionName = actualROMVersionCacheFileVersion.Name();
	
	//read the rom file version
	TInt8 romVersionCacheFileMajorVersion = romVerStream.ReadInt8L();
	TInt8 romVersionCacheFileMinorVersion = romVerStream.ReadInt8L();
	TInt16 romVersionCacheFileBuildVersion = romVerStream.ReadInt16L();
	TVersion romVersionCacheFileVersion(romVersionCacheFileMajorVersion, romVersionCacheFileMinorVersion, romVersionCacheFileBuildVersion);
	TVersionName romVersionCacheFileVersionName = romVersionCacheFileVersion.Name();
	
	//If persisted file version differs from what apparc can handle, recreate rom version file and applist.bin
	if (romVersionCacheFileVersionName.Compare(actualROMVersionCacheFileVersionName) != 0)
		{
		User::Leave(KErrGeneral);
		}
		
	//Read the length & value of software version from it.
	TBuf<KInfoBufLength> softwareVersion;
	TUint32 length = romVerStream.ReadUint32L();
	if (length>KInfoBufLength)
		{
		//File must be corrupt, an attempt to read will panic
		User::Leave(KErrCorrupt);
		}	
	romVerStream.ReadL(softwareVersion, length);

	//the persisted version has been successfully read
	//read the actual current version string
	TBuf<KInfoBufLength> actualSoftwareVersion;
	TInt err = SysUtil::GetSWVersion(actualSoftwareVersion); //use the SysUtil implementation
	if(err == KErrNone)
		{
		if (softwareVersion.Compare(actualSoftwareVersion) != 0)
			{
			//Leave if the current version is different from the previous stored version and recreate applist.
#ifdef _DEBUG
			RDebug::Print(_L("[Apparc] !!Firmware update detected!! Rebuilding AppList"));
#endif
			User::Leave(KErrGeneral);
			}
		}
	else
		{
		//Leave if any error reading the version information, except if file is not present
		if (err != KErrPathNotFound && err != KErrNotFound)
			{
#ifdef _DEBUG
			RDebug::Print(_L("[Apparc] !!Error %d reading Firmware version.  Rebuilding AppList"),err);
#endif
			User::Leave(err);
			}
		}
	CleanupStack::PopAndDestroy(2); //romVerStream, romVersionCacheFileName
#endif
		
	// Create Default Icon File Name
	CreateDefaultAppIconFileNameL();
	
	TInt ret = KErrNone;
	while (ret == KErrNone)
		{
		CApaAppData* const pApp = new (ELeave) CApaAppData(iFs);
		CleanupStack::PushL(pApp);

		// Restore entries till we leave
		TRAP(ret, pApp->InternalizeL(theReadStream));
		
		if (ret != KErrNone && ret != KErrEof)
			{
			User::Leave(ret);
			}
		// Check that the app has not been added to the list twice
		if (ret == KErrNone && !AppDataByUid(pApp->iUidType[2]))
			{
			AddToList(pApp);
			CleanupStack::Pop(pApp);
			}
		else
			{ // Delete pApp if an End of File condition is reached or the app has been added to the list twice
			CleanupStack::PopAndDestroy(pApp);
			}
		}	
	// Close the stream;
	CleanupStack::PopAndDestroy(&theReadStream);

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
	iFlags |= ENotifyUpdateOnFirstScanComplete;
#else
	iFs.Rename(iAppsListCacheFileName, iAppsListCacheBackUpFileName);
	iAppRegFinder->FindAllAppsL(CApaAppRegFinder::EScanRemovableDrives);	// Builds the Removable Media Drive List

	iFlags |= ENotifyUpdateOnFirstScanComplete;

	// It runs an update scan on removable media apps.
	ScanRemovableDrivesAndUpdateL();
#endif
	}

EXPORT_C TBool CApaAppList::IsLanguageChangePending() const
/** Returns ETrue if a language change event is received and a re-scan is in progress otherwise EFalse. */
	{
	return (iFlags & ELangChangePending);
	}


TInt CApaAppList::IdleUpdateCallbackL(TAny* aObject)
	{
	CApaAppList* self=REINTERPRET_CAST(CApaAppList*,aObject);
	const TBool moreToCome=self->IdleUpdateL();
	if (moreToCome==EFalse)
		{
		//Reset language change flag if scanning is over.
		if (self->IsLanguageChangePending())
			self->iFlags &= ~ELangChangePending;

		self->StopIdler();
		if (self->iLoadMbmIconsOnDemand)
			{
			self->InitiateStoringOfAppList();
			}
		else
			{
			self->StartIconLoadingL();
			}
		}
	return moreToCome;
	}

void CApaAppList::StoreL()
	{
	iAppListStorer = CApaAppListStorer::NewL(iAppData, iFs, *this);
	iAppListStorer->StartL(KAppListToFileStartDelay);
	}

void CApaAppList::NotifyObserver()
	{
	if (iObserver)
		{
		if (iFlags & EAppListHasChanged || iFlags & ENotifyUpdateOnFirstScanComplete)
			iObserver->NotifyUpdate(MApaAppListServObserver::EAppListChanged);	// NotifyUpdate will notify clients for both list update and scan complete.
		else
			iObserver->NotifyScanComplete();	// NotifyScanComplete will notify clients for scan complete.

		//iObserver=NULL;
		}
	}

void CApaAppList::StopIdler()
	{
 	delete iAppIdler;
	iAppIdler=NULL;
	}

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK

// returns ETrue if there more application information avaialable.
TInt CApaAppList::IdleUpdateL()
    {

    Usif::CApplicationRegistrationData* appInfo=NULL;
    TApaAppUpdateInfo::TApaAppAction action=TApaAppUpdateInfo::EAppPresent; //to make compiler happy. Actual value is assigned by GetNextApplicationInfo
    TUid appUid;
    
    //Ownership of appInfo is transfered to this function.
    appUid=iScrAppInfo->GetNextApplicationInfo(action,appInfo);
    
    if(appUid==KNullUid) 
        return EFalse;
    
    CleanupStack::PushL(appInfo);
    CApaAppData *appData=NULL;    
    CApaAppData* app = NULL;
    
    switch(action)
        {
        case TApaAppUpdateInfo::EAppNotPresent:
            if(appInfo==NULL)
                {
                TInt ret=FindAndDeleteApp(appUid);
                if(ret==KErrNone)
                    iFlags |= EAppListHasChanged;

                //Add uninstalled application UID to a list
                if(iUninstalledApps==NULL)
                    iUninstalledApps=new(ELeave) CArrayFixFlat<TUid>(1);
                
                iUninstalledApps->AppendL(appUid);
                }
            break;
            
        case TApaAppUpdateInfo::EAppPresent:
            // holds the application information from Applist
            app = AppDataByUid(appInfo->AppUid());
            
            if( app && IsLanguageChangePending() && (iScrAppInfo->GetSCRFetchAction() == EGetAllAppsInfo))
                {
                //Application needs to be updated because of language change
                RPointerArray<Usif::CLocalizableAppInfo> localisationInfo;
                localisationInfo=appInfo->LocalizableAppInfoList();
                ASSERT(!(localisationInfo.Count()>1));
                
                if((localisationInfo.Count()>0) && (app->ApplicationLanguage() != localisationInfo[0]->ApplicationLanguage()))
                    {
                    // holds the application information read from SCR db
                    appData=CApaAppData::NewL(*appInfo, iFs, iScr);    
                    FindAndDeleteApp(appUid);
                    AddToList(appData);  
                    iFlags |= EAppListHasChanged;
                    }
                else
                    {
                    app->iIsLangChangePending=EFalse;                
                    }
                }
            else
                {
                // holds the application information read from SCR db
                appData=CApaAppData::NewL(*appInfo, iFs, iScr);
                if(app)
                    {
                    //Application found in applist. Delete existing application information from applist and create new
                    //application information object and add to the applist.
                    FindAndDeleteApp(appUid);
                    AddToList( appData );
                    }
                else
                    {
                    AddToList( appData );                   
                    }
                iFlags |= EAppListHasChanged;
                }
            
            break;
            
        case TApaAppUpdateInfo::EAppInfoChanged:
            appData=CApaAppData::NewL(*appInfo, iFs, iScr);
            //Delete existing application information from applist and create new application information object and 
            //add to the applist.
            FindAndDeleteApp(appUid);
            AddToList( appData );
            iFlags |= EAppListHasChanged;
            break;
        }

    //If first scan not complete or if phone language is changed then clear the updated application list
    //Otherwise add application updated apps list
    if(!(iFlags&EFirstScanComplete) || (iFlags&ELangChangePending))
        {
        if(!iAppsUpdated)
            delete iAppsUpdated;
        iAppsUpdated=NULL;
        }
    else
        {
        if(!iAppsUpdated)
            iAppsUpdated= new(ELeave) CArrayFixFlat<TApaAppUpdateInfo>(1);
    
        TApaAppUpdateInfo appUpdateInfo(appUid, action);
        iAppsUpdated->AppendL(appUpdateInfo);
        }
    
    CleanupStack::PopAndDestroy(appInfo); 
    return ETrue;
    }



/*
 * Finds and delete an application from applist.
 */
TInt CApaAppList::FindAndDeleteApp(TUid aAppUid)
    {
    CApaAppData* appData=iAppData;
    CApaAppData* prevAppData=NULL;
    
    while(appData && appData->iUidType[2] != aAppUid)
        {
        prevAppData=appData;
        appData=appData->iNext;
        }

    if(appData)
        {
        if(prevAppData)
            {
            //If the application position is not the first application in the list
            prevAppData->iNext=appData->iNext;
            }
        else
            {
            //If the application position is first in the list
            iAppData=appData->iNext;
            }

#ifdef APPARC_SHOW_TRACE  
        if(appData)
            {
            RDebug::Print(_L("[Apparc] Application with UID: %X is deleted from applist"), appData->iUidType[2]);
            }
#endif

        delete appData;
        return(KErrNone);
        }

    //if application not found, return KErrNotFound
    return(KErrNotFound);
    }

/**
@internalComponent
*/
EXPORT_C CApaAppData* CApaAppList::FindAndAddSpecificAppL(TUid aAppUid)
    {
    Usif::RSoftwareComponentRegistry scrCon;
	//If SCR connection not avaialable then connect to SCR. Otherwise use the 
	//existing connection.
    if(!iScr.Handle())
        {
        User::LeaveIfError(scrCon.Connect());
        CleanupClosePushL(scrCon);
        }
    else
        scrCon=iScr;
  
    
    //Pass 1 as number of entries to fetch from SCR as only specific application information is required.
    CApaScrAppInfo *scrAppInfo=CApaScrAppInfo::NewL(scrCon, 1); 
    CleanupStack::PushL(scrAppInfo);

    RArray<TApaAppUpdateInfo>* appUpdateInfoList=new (ELeave) RArray<TApaAppUpdateInfo>(1);
    CleanupStack::PushL(appUpdateInfoList);
    TApaAppUpdateInfo appUpdateInfo(aAppUid, TApaAppUpdateInfo::EAppPresent) ;
    appUpdateInfoList->AppendL(appUpdateInfo);

    scrAppInfo->GetSpecificAppsInfoL(appUpdateInfoList);
    CleanupStack::Pop(appUpdateInfoList);
    
    Usif::CApplicationRegistrationData* appInfo=NULL;
    TApaAppUpdateInfo::TApaAppAction action;
    TUid uid;
    uid=scrAppInfo->GetNextApplicationInfo(action, appInfo);
    CleanupStack::PushL(appInfo);

    CApaAppData *appData=NULL;
    if(appInfo)
        {
        appData=CApaAppData::NewL(*appInfo, iFs, scrCon);
        FindAndDeleteApp(uid);
        AddToList(appData);
        iFlags |= EAppListHasChanged;
        }
    CleanupStack::PopAndDestroy(2, scrAppInfo);
    
	//If SCR session established in this function, then close it.
    if(!iScr.Handle())
        CleanupStack::PopAndDestroy();
        
    return appData;
    }

#else
TInt CApaAppList::IdleUpdateL()
// returns ETrue if there is more scanning to be done.
	{
    
	TBool more=EFalse;
	TApaAppEntry currentApp = TApaAppEntry();
	TRAPD(err, more = iAppRegFinder->NextL(currentApp, *iForcedRegistrations));
	if (err!=KErrNone)
		{
		if (iFlags & ENotifyUpdateOnFirstScanComplete)
			User::Leave(err);

		return more;
		}
	TBool hasChanged=EFalse;
	if (more)
		{
		TRAPD(err,UpdateNextAppL(currentApp,hasChanged));
		if (err!=KErrNone)
			{
			SetNotFound(iAppData,hasChanged);
			if (iFlags & ENotifyUpdateOnFirstScanComplete)
				User::Leave(err);

			more=EFalse; // abandon ship
			}
		}
	else
		{
		SetNotFound(iAppData,hasChanged);
		PurgeL();
		}

	if (hasChanged)
		iFlags |= EAppListHasChanged;

	return more;
	}

void CApaAppList::UpdateNextAppL(const TApaAppEntry& aAppEntry,TBool& aHasChanged)
    {
    CApaAppData* appData=AppDataByUid(aAppEntry.iUidType[2]);
    if (appData==NULL)
        {// not in list, so add it at the start
        TRAPD(err,appData=CApaAppData::NewL(aAppEntry, iFs));
        if (err==KErrNone)
            {
            AddToList( appData );
            aHasChanged=ETrue;
            }
        }
    else if (appData->IsPending())
        { // not found yet during current scan - we may need to override this one
        
        // On a system which scans for registration .RSC files (V2 apps) first, followed by
        // .APP files (V1 apps), it's valid for a V1 app to override a V2 app (if the V2 app
        // has just been removed). If this is the case, assume it's ok to compare the V1 .APP filename,
        // with the V2 .RSC filename as their filenames will never match (which is what we want in this case).
        TPtrC currentFileName;
        if (appData->RegistrationFileUsed())
            currentFileName.Set(*appData->iRegistrationFile);
        else
            currentFileName.Set(*appData->iFullName);
    
        if (aAppEntry.iFullName.CompareF(currentFileName)!=0)
            {
            delete appData->iSuccessor;
            appData->iSuccessor = NULL;
            appData->iSuccessor = CApaAppEntry::NewL(aAppEntry);

            appData->iIsPresent = CApaAppData::ESuperseded;
            aHasChanged=ETrue;
            }
        else
            {
            if (appData->Update() || appData->iIsPresent==CApaAppData::ENotPresentPendingUpdate) 
                aHasChanged=ETrue; 

            appData->iIsPresent = CApaAppData::EIsPresent;
            }
        }
    }

void CApaAppList::SetPending(CApaAppData* aAppData)
    // set all apps to pending update - we'll find them again as we scan
    {
    for (; aAppData; aAppData = aAppData->iNext)
        aAppData->SetAppPending();
    }

void CApaAppList::SetNotFound(CApaAppData* aAppData, TBool& aHasChanged)
    // mark any unfound apps not present
    {
    while (aAppData)
        {
        if (aAppData->IsPending())
            {
            aAppData->iIsPresent = CApaAppData::ENotPresent;
            aHasChanged = ETrue;
            }
        aAppData = aAppData->iNext;
        }
    }

EXPORT_C void CApaAppList::PurgeL()
/** Removes any applications from the list if they are no longer present 
on the phone. It updates applications that have been 
superceded. */
    {
    CApaAppData* appData=iAppData;
    CApaAppData* prev=NULL;
    while (appData)
        {
        CApaAppData* next=appData->iNext;
        if (appData->iIsPresent==CApaAppData::ENotPresent)
            {
            if (prev)
                prev->iNext=next;
            else
                iAppData=next;
            
            //Add uninstalled application UID to a list
            if(iUninstalledApps==NULL)
                iUninstalledApps=new(ELeave) CArrayFixFlat<TUid>(1);
            
            iUninstalledApps->AppendL(appData->AppEntry().iUidType[2]);
            
            delete appData;
            }
        else if (appData->iIsPresent==CApaAppData::ESuperseded)
            {
            CApaAppData* newApp=NULL;
            TApaAppEntry appEntry;
            appData->iSuccessor->Get(appEntry);
            TRAPD(err,newApp=CApaAppData::NewL(appEntry, iFs));
            if (err==KErrNone)
                {
                // remove the old one and add the new one in its place
                if (prev)
                    prev->iNext=newApp;
                else
                    iAppData=newApp;
        
                newApp->iNext = appData->iNext;
                delete appData;
                // increment the iterator
                prev = newApp;
                }
            }
        else
            prev=appData;
        
        appData=next;
        }
    }

void CApaAppList::ScanRemovableDrivesAndUpdateL()
/** Rename Appslist.bin file to AppsList_Backup.bin, so that it can be renamed back, 
     if the update scan on removable media drives does not change applist. */
    {
    const TArray<const TDriveUnitInfo> listOfRemovableMediaDrives = iAppRegFinder->DriveList();
    const TInt count = listOfRemovableMediaDrives.Count();

    // Removable media scan would take place only if removable drives are present.
    if (count)
        {
        CApaAppData* appData = iAppData;
        while (appData)
            {
            for (TInt driveIndex = 0; driveIndex < count; ++driveIndex)
                {
                if (TParsePtrC(*appData->iRegistrationFile).Drive() == listOfRemovableMediaDrives[driveIndex].iUnit.Name())
                    {
                    appData->SetAppPending();
                    break;
                    }
                }
            appData = appData->iNext;
            }

        while (IdleUpdateL())
            { // It updates the removable media apps present in AppList if it has changed.

            };
        }
    }

void CApaAppList::UndoSetPending(CApaAppData* aAppData)
    // Reset all apps to pevious pending state so they don't get purged
    {
    for (; aAppData; aAppData = aAppData->iNext)
        {
        if (aAppData->iIsPresent == CApaAppData::EPresentPendingUpdate)
            {
            aAppData->iIsPresent = CApaAppData::EIsPresent;
            }
        }
    }

/**
@internalComponent
*/
EXPORT_C CApaAppData* CApaAppList::FindAndAddSpecificAppL(CApaAppRegFinder* aFinder, TUid aAppUid)
    {
//Scans and adds the specified application to the app list if found
  __ASSERT_DEBUG(aFinder, Panic(EPanicNullPointer));
  TBool found = EFalse;
  TApaAppEntry appEntry;
  aFinder->FindAllAppsL(CApaAppRegFinder::EScanAllDrives);
  while (aFinder->NextL(appEntry, *iForcedRegistrations))
      {
      if (appEntry.iUidType[2] == aAppUid)
          {
          found = ETrue;
          break;
          }
      }
  
  CApaAppData* app = NULL;
  if (found)
      {
      // add the app to the list
      TBool hasChanged = EFalse;
      CApaAppData* prevFirstAppInList = iAppData;
      UpdateNextAppL(appEntry, hasChanged);
      if (iAppData != prevFirstAppInList)
          app = iAppData;     // assume the new app was added to the list

      if (hasChanged)
          iFlags |= EAppListHasChanged;
      }

  return app;
    return NULL;
    }

#endif

EXPORT_C TBool CApaAppList::IsIdleUpdateComplete() const
/** Tests whether an asynchronous update of the list is currently in progress.

@return True if no asynchronous update of the list is currently in progress, 
otherwise false. */
	{
	return iAppIdler == NULL;
	}


void CApaAppList::AddToList( CApaAppData* aAppData )
	{
	__ASSERT_DEBUG(aAppData, Panic(EPanicNullPointer));
	aAppData->iNext=iAppData;
	iAppData=aAppData;
	
#ifdef APPARC_SHOW_TRACE	
    RDebug::Print(_L("[Apparc] Application with UID: %X is added to applist"), aAppData->iUidType[2]);
#endif
    
	}


EXPORT_C TInt CApaAppList::Count() const
/** Gets the count of applications present in the app list.

@return The number of applications in the list. */

	{
	TInt count=0;
	CApaAppData* appData=iAppData;
	while (appData)
		{
		count++;
		appData=appData->iNext;
		}
	
	return count;
	}

EXPORT_C CApaAppData* CApaAppList::FirstApp() const
/** Gets a pointer to the first application in the list 
that can use the default screen mode.

@return A pointer to the first application. */
	{
	return FirstApp(0);
	}

EXPORT_C CApaAppData* CApaAppList::FirstApp(TInt aScreenMode) const
/** Gets a pointer to the first application in the list 
that can use the specified screen mode.

@param aScreenMode The index of the screen mode. Specifying 
KIgnoreScreenMode returns the first application in the list, 
regardless of screen mode.
@return A pointer to the first application that can use the 
specified screen mode. */
	{

	CApaAppData* appData=iAppData;

	if(aScreenMode!=KIgnoreScreenMode)
		{
		while (appData && !appData->CanUseScreenMode(aScreenMode))
			appData = appData->iNext;
		}

	return appData;
	}

EXPORT_C CApaAppData* CApaAppList::NextApp(const CApaAppData* aApp) const
/** Gets a pointer to the next application after aApp in the list 
that can use the default screen mode.

@param aApp A pointer to an application in the list.
@return A pointer to the next application after aApp in the list 
that can use the default screen mode.
@panic APGRFX 12 aApp is NULL. */
	{
	return NextApp(aApp,0);
	}

EXPORT_C CApaAppData* CApaAppList::NextApp(const CApaAppData* aApp, TInt aScreenMode) const
/** Gets a pointer to the next application after aApp in the list 
that can use the specified screen mode.

@param aApp A pointer to an application in the list. 
@param aScreenMode The index of the screen mode. Specifying 
KIgnoreScreenMode returns the next application in the list, 
regardless of screen mode.
@return A pointer to the next application after aApp in the list 
that can use the specified screen mode.
@panic APGRFX 12 aApp is NULL. */

	{
	__ASSERT_ALWAYS(aApp,Panic(EPanicNoAppDataSupplied));
	//
	CApaAppData* iApp=aApp->iNext; //lint !e613 Suppress possible use of null pointer - asserted above

	if(aScreenMode!=KIgnoreScreenMode)
		while (iApp && !iApp->CanUseScreenMode(aScreenMode))
			iApp = iApp->iNext;

	return iApp;
	}

EXPORT_C CApaAppData* CApaAppList::AppDataByUid(TUid aAppUid) const
/** Gets a pointer to the application in the list whose third 
UID matches the specified UID.

@param aAppUid An application's third UID. 
@return A pointer to the application, if successful. Null, 
if no match is found or if KNullUid was specified. */
	{
	if (aAppUid==KNullUid)
		return NULL; // never match null UID as it represents an un-UIDed file
	
	CApaAppData* appData=iAppData;
	while (appData)
		{
		if (appData->AppEntry().iUidType[2]==aAppUid)
			return appData;
		appData=appData->iNext;
		}
	
	return NULL;
	}

EXPORT_C CApaAppData* CApaAppList::AppDataByFileName(const TDesC& aFullFileName) const
/** Gets a pointer to the application in the list whose application
file name matches the one specified

@param aFullFileName a file name. 
@return A pointer to the application, if successful. Null, 
if no match is found or if KNullDesC was specified.
@internalTechnology
*/
	{
	if (aFullFileName.Length()==0)
		return NULL; // never match null UID as it represents an un-UIDed file

	CApaAppData* appData=iAppData;
	while (appData)
		{
		if (appData->AppEntry().iFullName.CompareF(aFullFileName)==0)
			return appData;

		appData=appData->iNext;
		}

	return NULL;
	}
	
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
/**
Adds a registration file to the iForcedRegistrations array.

@param aRegistrationFile The function takes ownership of the HBufC.
@internalComponent
*/
EXPORT_C void CApaAppList::AddForcedRegistrationL(const TDesC& aRegistrationFile)
	{
	TInt err = iForcedRegistrations->InsertIsqL(aRegistrationFile, ECmpFolded);
	if (err != KErrAlreadyExists) // We silently ignore attempts to insert duplicates
		User::LeaveIfError(err);
	
	} //lint !e818 Suppress pointer parameter 'aRegistrationFile' could be declared as pointing to const
	
EXPORT_C void CApaAppList::ResetForcedRegistrations()
	{
	if(iForcedRegistrations)
		iForcedRegistrations->Reset();
	}
#endif

/** Finds the preferred application to handle the specified data type.

@param aDataType The data type of interest.
@return The third UID of the application in the list which has the 
highest priority for handling the specified data type. A null UID is 
returned if no application in the list can handle the specified data type. */
EXPORT_C TUid CApaAppList::PreferredDataHandlerL(const TDataType& aDataType) const
	{
	TInt dummy;
	return PreferredDataHandlerL(aDataType, NULL, dummy);
	}
	
/** Finds the preferred application to handle the specified data type.

@param aDataType The data type of interest.
@param aServiceUid The optional service UID.
@param aPriority The priority associated with the returned app.
@return The third UID of the application in the list which has the 
highest priority for handling a combination of the specified data type
and service. A null UID is returned if no application in the list can
handle the combination of specified data type and service.
@internalComponent
*/
EXPORT_C TUid CApaAppList::PreferredDataHandlerL(const TDataType& aDataType, const TUid* aServiceUid, TInt& aPriority) const
	{
	// If there is a service UID then first try to use the service specific list
	// of datatypes
	if (aServiceUid)
		{
		CApaAppData* appData = iAppData;
		aPriority = KDataTypePriorityNotSupported;
		TUid uid = KNullUid; 

		while (appData)
			{
			TInt priority = appData->ImplementsServiceWithDataType(*aServiceUid, aDataType);
			if (priority > aPriority)
				{
				aPriority=priority;
				uid=appData->AppEntry().iUidType[2];
				}

			appData=appData->iNext;
			}

		if (aPriority != KDataTypePriorityNotSupported)
			return uid;
		}
	
	CApaAppData* appData = iAppData;
	aPriority = KDataTypePriorityNotSupported;
	TUid uid = KNullUid;

	// Run through all of the linked list of app-data to locate the application 
	// with the highest priority that can handle the given data type
	while (appData)
		{
		const TInt priority = appData->DataType(aDataType);
		if ((priority > aPriority) && (!aServiceUid || (aServiceUid && appData->ImplementsService(*aServiceUid))))
			{
			aPriority = priority;
			uid = appData->AppEntry().iUidType[2];
			}
			
		appData=appData->iNext;
		}
		
	return uid;
	}
  
void CApaAppList::DeleteAppData()
	{
	iValidFirstAppData = NULL;
	iFlags &= ~EFirstScanComplete;
	iFlags &= ~EAppListHasChanged;
	iFlags &= ~ELangChangePending;
	
	CApaAppData* next = NULL;
	for (CApaAppData* appData=iAppData; appData; appData = next)
		{
		next = appData->iNext;
		delete appData;
		}
	iAppData = NULL;
	}

void CApaAppList::ScanComplete()
	{
	if (!(iFlags & EFirstScanComplete) && iObserver)
		iObserver->InitialListPopulationComplete();
	iValidFirstAppData = iAppData;
	iFlags|=EFirstScanComplete;

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK  	
	iNNAInstallation = EFalse;
#endif
	
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
	delete iScrAppInfo;
	iScrAppInfo=NULL;
    iScr.Close();
#endif
#ifdef APPARC_SHOW_TRACE    
    RDebug::Printf("[Apparc] *****************END CREATING APPLIST ****************************");
#endif    
	}

/**
 *
 * Tests whether the first scan for list of Apps has completed.
 *
 * @return   "TBool"
 *            True, if the first scan for list of Apps has completed; false, otherwise.
 * @internalComponent
 */
EXPORT_C TBool CApaAppList::IsFirstScanComplete() const
	{
	return iFlags&EFirstScanComplete;
	}

EXPORT_C TBool CApaAppList::AppScanInProgress() const
/** @internalComponent */
	{
	return (iAppIdler!=NULL) && iAppIdler->IsActive();
	}

/**
@internalComponent
*/
EXPORT_C CBufFlat* CApaAppList::ServiceArrayBufferL(TUid aAppUid) const
	{
	CApaAppData* app = FirstApp(KIgnoreScreenMode);
	while (app && app->AppEntry().iUidType[2] != aAppUid)
		app = NextApp(app, KIgnoreScreenMode);

	if (app)
		{
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
		if (!app->RegistrationFileUsed())
			User::Leave(KErrNotSupported);
#endif		

		if (app->iServiceArray)
			{
			CBufFlat* const buf = CBufFlat::NewL(KBufferExpansionGranularity);
			CleanupStack::PushL(buf);
			RBufWriteStream writeStream(*buf);
			writeStream << *(app->iServiceArray);
			writeStream.CommitL();
			writeStream.Release();
			CleanupStack::Pop(buf);
			return buf;
			}
		}

	User::Leave(KErrNotFound);
	return NULL; // to keep compiler happy
	}

/**
@internalComponent
*/
EXPORT_C CBufFlat* CApaAppList::ServiceUidBufferL(TUid aAppUid) const
	{
	CApaAppData* app = FirstApp(KIgnoreScreenMode);
	while (app && app->AppEntry().iUidType[2] != aAppUid)
		app = NextApp(app, KIgnoreScreenMode);

	if (app)
		{
	
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
		if (!app->RegistrationFileUsed())
			User::Leave(KErrNotSupported);
#endif
		
		if (app->iServiceArray)
			{
			CArrayFixFlat<TApaAppServiceInfo>& serviceArray = *(app->iServiceArray);
			CArrayFixFlat<TUid>* uidArray = new(ELeave) CArrayFixFlat<TUid>(4);
			CleanupStack::PushL(uidArray);
			for (TInt i = serviceArray.Count()-1; i >= 0; i--)
				uidArray->AppendL(serviceArray[i].Uid());

			CBufFlat* const buf = CBufFlat::NewL(KBufferExpansionGranularity);
			CleanupStack::PushL(buf);
			RBufWriteStream writeStream(*buf);
			writeStream << *uidArray;
			writeStream.CommitL();
			writeStream.Release();
			CleanupStack::Pop(buf);
			CleanupStack::PopAndDestroy(uidArray);
			return buf;
			}
		}

	User::Leave(KErrNotFound);
	return NULL; // to keep compiler happy
	}

/**
@internalComponent
*/
EXPORT_C CBufFlat* CApaAppList::ServiceOpaqueDataBufferL(TUid aAppUid, TUid aServiceUid) const
	{
	CApaAppData* app = FirstApp(KIgnoreScreenMode);
	while (app && app->AppEntry().iUidType[2] != aAppUid)
		app = NextApp(app, KIgnoreScreenMode);

	if (app)
		{
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
		if (!app->RegistrationFileUsed())
			User::Leave(KErrNotSupported);
#endif		

		if (app->iServiceArray)
			{
			CArrayFixFlat<TApaAppServiceInfo>* implArray = NULL;
			CArrayFixFlat<TApaAppServiceInfo>& serviceArray = *(app->iServiceArray);
			for (TInt i = serviceArray.Count()-1; i >= 0; i--)
				{
				const TApaAppServiceInfo& infoRef = serviceArray[i];
				if (infoRef.Uid() == aServiceUid)
					{
					if (!implArray)
						{
						implArray = new(ELeave) CArrayFixFlat<TApaAppServiceInfo>(4);
						CleanupStack::PushL(TCleanupItem(CleanupServiceArray, implArray));
						}

					CArrayFixFlat<TDataTypeWithPriority>* dummy = new(ELeave) CArrayFixFlat<TDataTypeWithPriority>(1);
					CleanupStack::PushL(dummy);					
					TApaAppServiceInfo info(aServiceUid, dummy, infoRef.OpaqueData().AllocLC());
					implArray->AppendL(info);
					CleanupStack::Pop(CONST_CAST(TDesC8*,&info.OpaqueData()));
					CleanupStack::Pop(dummy);
					}
				}
				
			if (implArray)
				{
				CBufFlat* const buf = CBufFlat::NewL(KBufferExpansionGranularity);
				CleanupStack::PushL(buf);
				RBufWriteStream writeStream(*buf);
				writeStream << *implArray;
				writeStream.CommitL();
				writeStream.Release();
				CleanupStack::Pop(buf);
				CleanupStack::PopAndDestroy(implArray);
				return buf;
				}
			}
		}
	User::Leave(KErrNotFound);
	return NULL; // to keep compiler happy
	}

/**
@internalComponent
*/
EXPORT_C CBufFlat* CApaAppList::ServiceImplArrayBufferL(TUid aServiceUid) const
	{
	CApaAppData* app = FirstApp(KIgnoreScreenMode);
	// build an array containing all implementations of the service identified by aServiceUid
	CArrayFixFlat<TApaAppServiceInfo>* implArray = NULL;
	while (app)
		{
		if (app->iServiceArray)
			{
			for (TInt i = app->iServiceArray->Count()-1; i >= 0; i--)
				{
				const TApaAppServiceInfo& infoRef = (*app->iServiceArray)[i];
				if (infoRef.Uid() == aServiceUid)
					{
					if (!implArray)
						{
						implArray = new(ELeave) CArrayFixFlat<TApaAppServiceInfo>(4);
						CleanupStack::PushL(TCleanupItem(CleanupServiceArray, implArray));
						}
				
					CArrayFixFlat<TDataTypeWithPriority>* datatypes = DataTypeArrayDeepCopyLC(infoRef.DataTypes());
					HBufC8* data = infoRef.OpaqueData().AllocLC();
					TApaAppServiceInfo info(app->AppEntry().iUidType[2], datatypes, data);
					implArray->AppendL(info);
					CleanupStack::Pop(data);
					CleanupStack::Pop(datatypes);
					}
				}
			}
		app = NextApp(app, KIgnoreScreenMode);
		}

	if (implArray)
		{
		CBufFlat* const buf = CBufFlat::NewL(KBufferExpansionGranularity);
		CleanupStack::PushL(buf);
		RBufWriteStream writeStream(*buf);
		writeStream << *(implArray);
		writeStream.CommitL();
		writeStream.Release();
		CleanupStack::Pop(buf);
		CleanupStack::PopAndDestroy(implArray);
		return buf;
		}
		
	User::Leave(KErrNotFound);
	return NULL; // to keep compiler happy
	}

EXPORT_C CBufFlat* CApaAppList::ServiceImplArrayBufferL(TUid aServiceUid, const TDataType& aDataType) const
	{
	CApaAppData* app = FirstApp(KIgnoreScreenMode);
	// build an array containing all implementations of the service identified by aServiceUid
	CArrayFixFlat<TApaAppServiceInfo>* implArray = NULL;
	while (app)
		{
		if (app->iServiceArray)
			{
			for (TInt i = app->iServiceArray->Count()-1; i >= 0; i--)
				{
				const TApaAppServiceInfo& infoRef = (*app->iServiceArray)[i];
				if (infoRef.Uid() == aServiceUid)
					{
				 	if (KDataTypePriorityNotSupported != app->DataType(aDataType, infoRef.DataTypes()))
						{
						if (!implArray)
							{
							implArray = new(ELeave) CArrayFixFlat<TApaAppServiceInfo>(4);
							CleanupStack::PushL(TCleanupItem(CleanupServiceArray, implArray));
							}
						CArrayFixFlat<TDataTypeWithPriority>* datatypes = DataTypeArrayDeepCopyLC(infoRef.DataTypes());
						HBufC8* data = infoRef.OpaqueData().AllocLC();
						TApaAppServiceInfo info(app->AppEntry().iUidType[2], datatypes, data);
						implArray->AppendL(info);
						CleanupStack::Pop(data);
						CleanupStack::Pop(datatypes);
						}
					}
				}
			}
		app = NextApp(app, KIgnoreScreenMode);
		}

	if (implArray)
		{
		CBufFlat* const buf = CBufFlat::NewL(KBufferExpansionGranularity);
		CleanupStack::PushL(buf);
		RBufWriteStream writeStream(*buf);
		writeStream << *(implArray);
		writeStream.CommitL();
		writeStream.Release();
		CleanupStack::Pop(buf);
		CleanupStack::PopAndDestroy(implArray);
		return buf;
		}
		
	User::Leave(KErrNotFound);
	return NULL; // to keep compiler happy
	}
	
CArrayFixFlat<TDataTypeWithPriority>* CApaAppList::DataTypeArrayDeepCopyLC(const CArrayFixFlat<TDataTypeWithPriority>& aOriginal) const
	{
	CArrayFixFlat<TDataTypeWithPriority>* result = new(ELeave) CArrayFixFlat<TDataTypeWithPriority>(1);
	CleanupStack::PushL(result);
	TInt ii = 0;
	TInt end = aOriginal.Count();
	while (ii < end)
		{
		result->AppendL(aOriginal[ii]);
		ii++;
		}
		
	return result;
	}
	
EXPORT_C CApaAppList* CApaAppList::Self()
	{ // static
	return STATIC_CAST(CApaAppList*,Dll::Tls());
	}
	
/*EXPORT_C*/ RFs& CApaAppList::ShareProtectedFileServer()
	{
	return iFsShareProtected; //lint !e1536 Exposing low access member - need to consider a redesign or design clarification here(iFsShareProtected should be owned be the one that needs it) but postpone for now as that may require large changes
	}
	
CApaAppIconArray* CApaAppList::LoadDefaultIconsL() const
	{
	CApaIconLoader* iconLoader = CApaIconLoader::NewLC(iFs);
	CApaAppIconArray* icons = CApaAppIconArray::NewRealDefaultIconsLC(KNumberOfIconsInDefaultMbm,*iDefaultAppIconMbmFileName, *iconLoader);
	const TBool badMbmEntryInfo = !icons->LoadIconsL();
	if(badMbmEntryInfo)
		{
		CleanupStack::PopAndDestroy(2,iconLoader);
		return CApaAppIconArray::NewL();
		}
	else
		{
		CleanupStack::Pop(icons);
		CleanupStack::PopAndDestroy(iconLoader);
		}
		
	return icons;
	}

void CApaAppList::AcquireDefaultIconArrayL() const
	{
	ASSERT(iDefaultIconUsageCount >= 0);
	if (iDefaultIconUsageCount == 0)
		iDefaultIconArray = LoadDefaultIconsL();

	++iDefaultIconUsageCount;
	}

// Should NEVER be called by an object that does not call AcquireDefaultIconArrayL and
// ReleaseDefaultIconArray at the beginning and end of its lifetime respectively
const CApaAppIconArray& CApaAppList::DefaultIconArray() const
	{
	__ASSERT_ALWAYS(iDefaultIconArray, Panic(EPanicNullPointer));
	return *iDefaultIconArray;
	}

void CApaAppList::ReleaseDefaultIconArray() const
	{
	ASSERT(iDefaultIconUsageCount > 0);
	if(0 == --iDefaultIconUsageCount)
		{
		delete iDefaultIconArray;
		iDefaultIconArray = NULL;		
		}
	}
	
// DEF077478 - Required to update the default icons that the app list holds.
void CApaAppList::UpdateDefaultIconsL()
	{
	CApaAppIconArray* iconArray = LoadDefaultIconsL();
	if (iconArray)
		{
		delete iDefaultIconArray;
		iDefaultIconArray = iconArray;
		}
	}

void CApaAppList::DeleteAppListStorer()
	{
	delete iAppListStorer;
	iAppListStorer = NULL;
	}

void CApaAppList::DeleteAppIconLoader()
	{
	delete iAppIconLoader;
	iAppIconLoader = NULL;
	}

void CApaAppList::InitiateStoringOfAppList()
	{
	ScanComplete();	
	NotifyObserver();
	iFlags &= ~ENotifyUpdateOnFirstScanComplete;
	// now that the scan is finished, iDefaultAppIconMbmFileName is deleted
	delete iDefaultAppIconMbmFileName;
	iDefaultAppIconMbmFileName=NULL;
	// if applist has not changed and AppsList_Backup.bin file exists then it is replaced back to AppsList.bin
	if (!(iFlags & EAppListHasChanged) && BaflUtils::FileExists(iFs, iAppsListCacheBackUpFileName))
		{
		TInt replaceError = iFs.Replace(iAppsListCacheBackUpFileName, iAppsListCacheFileName);
		if (replaceError == KErrNone)
			return;
		}

    iFlags &= ~EAppListHasChanged;
	iFs.Delete(iAppsListCacheBackUpFileName);
	TInt err = iFs.MkDir(iAppsListCachePath);
	if (err == KErrNone || err == KErrAlreadyExists)
		{
		TRAP(err, StoreL());
		if (err)
			DeleteAppListStorer();
		}
	}


EXPORT_C void CApaAppList::AddCustomAppInfoInListL(TUid aAppUid, TLanguage aLanguage, const TDesC& aShortCaption)
	{
	TBool replaced=EFalse;
	for (TInt i=iCustomAppList.Count()-1; i>=0; --i)
		{
		if (aAppUid == iCustomAppList[i]->Uid() && aLanguage == iCustomAppList[i]->Language())
			{
			CCustomAppInfoData* oldAppInfo = iCustomAppList[i];
			iCustomAppList[i] = CCustomAppInfoData::NewL(aAppUid, aLanguage, aShortCaption);;
			delete oldAppInfo;
			replaced=ETrue;
			break;
			}
		}
	//Add ELangNone items at the end and others at the beginning
	if(!replaced)
		{
		CCustomAppInfoData* appInfo = CCustomAppInfoData::NewL(aAppUid, aLanguage, aShortCaption);
		CleanupStack::PushL(appInfo);
		
		if(aLanguage == ELangNone)
			iCustomAppList.AppendL(appInfo);
		else
			iCustomAppList.InsertL(appInfo,0);
		
		CleanupStack::Pop(appInfo);
		}
	
	} //lint !e818 Suppress pointer parameter could be declared as pointing to const - this method takes ownership


EXPORT_C void CApaAppList::UpdateAppListByShortCaptionL()
	{
	//Items with ELangNone are always found at the end and other languages at the beginning
	//While iterating from end to beginning, we overwrite short caption set by ELangNone if there is any provided for the application language
	for (TInt i=iCustomAppList.Count()-1; i>=0; --i)
		{
		const CCustomAppInfoData* const customAppInfo=iCustomAppList[i];
		CApaAppData* app = AppDataByUid(customAppInfo->Uid());
		
		if ( app && (app->ApplicationLanguage()==customAppInfo->Language() || customAppInfo->Language()==ELangNone))
			app->SetShortCaptionL(*customAppInfo->ShortCaption());
		}
	}

EXPORT_C void CApaAppList::UpdateAppListByIconCaptionOverridesL()
	{
	// get the language downgrade path, so that most appropriate language can be used
	RArray<TLanguage> languageDowngradePath;
	CleanupClosePushL(languageDowngradePath);
	BaflUtils::GetDowngradePathL(iFs, User::Language(), languageDowngradePath);
	for (CApaAppData* app = FirstApp(); app; app = NextApp(app))
		{
		CApaIconCaptionOverridesForApp* appOverride = iIconCaptionOverrides->OverrideForApp(app->AppEntry().iUidType[2]);
		if (appOverride)
			{
			TApaIconCaptionOverrideReader overrideReader = appOverride->Reader(languageDowngradePath);
			const TDesC* shortCaption = overrideReader.ShortCaption();
			if (shortCaption)
				app->SetShortCaptionL(*shortCaption);
			const TDesC* caption = overrideReader.Caption();
			if (caption)
				app->SetCaptionL(*caption);
			const TDesC* iconFileName = overrideReader.IconFileName();
			if (overrideReader.NumIconsSet() && iconFileName)
				app->SetIconsL(*iconFileName, overrideReader.NumIcons());
			}
		}
	CleanupStack::PopAndDestroy(&languageDowngradePath);
	}

//
// Class CApaAppListStorer
//

CApaAppList::CApaAppListStorer::CApaAppListStorer(CApaAppData* aFirstAppData, RFs& aFs, CApaAppList& aAppList) : CActive(EPriorityIdle), iCurrentAppData(aFirstAppData), iFs(aFs), iAppList(aAppList)
	{
	}

CApaAppList::CApaAppListStorer::~CApaAppListStorer()
	{
	Cancel();
	iWriteStream.Close();
	iFs.Delete(iTempFilename);
	iTimer.Close();
	iCurrentAppData = NULL;
	}

CApaAppList::CApaAppListStorer* CApaAppList::CApaAppListStorer::NewL(CApaAppData* aFirstAppData, RFs& aFs, CApaAppList& aAppList)
	{
	CApaAppListStorer* self = new(ELeave) CApaAppListStorer(aFirstAppData, aFs, aAppList);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CApaAppList::CApaAppListStorer::ConstructL()
	{
	User::LeaveIfError(iTimer.CreateLocal());
	CActiveScheduler::Add(this);
	}

void CApaAppList::CApaAppListStorer::StartL(const TTimeIntervalMicroSeconds32& aDelay)
	{
	User::LeaveIfError(iWriteStream.Temp(iFs, iAppList.iAppsListCachePath, iTempFilename, EFileWrite));
	iWriteStream.WriteInt32L(User::Language());
	
#if defined (SYMBIAN_BAFL_SYSUTIL)
	//Write a cache of the ROM version to a separate stream
	//Build the filename for the cache file
	TInt maxSizeofFileName = iAppList.iAppsListCachePath.Length() + KROMVersionStringCacheFileName().Length();
	RBuf romVersionCacheFileName;
	romVersionCacheFileName.CreateL(maxSizeofFileName);
	romVersionCacheFileName.CleanupClosePushL();
	romVersionCacheFileName.Append(iAppList.iAppsListCachePath);
	romVersionCacheFileName.Append(KROMVersionStringCacheFileName());
	
	RFileWriteStream romVerStream;
	User::LeaveIfError(romVerStream.Replace(iFs,romVersionCacheFileName,EFileWrite));
	CleanupClosePushL(romVerStream);
	
	// Write the file version that apparc can handle.
	romVerStream.WriteInt8L(KROMVersionCacheFileMajorVersion);
	romVerStream.WriteInt8L(KROMVersionCacheFileMinorVersion);
	romVerStream.WriteInt16L(KROMVersionCacheFileBuildVersion);
		
	TBuf<KInfoBufLength> version;
	SysUtil::GetSWVersion(version);
	
	// Write the software version even if SysUtil returns err since all conditions are taken care during restore.
	romVerStream.WriteUint32L(version.Length());
	romVerStream.WriteL(version, version.Length());
	CleanupStack::PopAndDestroy(2); //romVerStream, romVersionCacheFileName
#endif //(SYMBIAN_BAFL_SYSUTIL)
	
	iTimer.After(iStatus, aDelay);
	SetActive();
	}

void CApaAppList::CApaAppListStorer::RunL()
	{ 
	// iStatus could be KErrNone or negative when timer completes, irrespective of its status we need to re-queue the request.
	if (iCurrentAppData)
		{
		StoreEntryL(iWriteStream, *iCurrentAppData);
		iCurrentAppData = iCurrentAppData->Next();
		SetActive();
		TRequestStatus* status = &iStatus;
		User::RequestComplete(status, KErrNone);
		}
	else
		{
		iWriteStream.CommitL();
		iWriteStream.Close();

		TInt err = iFs.Replace(iTempFilename, iAppList.iAppsListCacheFileName);
		if (err != KErrNone)
			iFs.Delete(iTempFilename);

		iAppList.DeleteAppListStorer();
		}
	}

void CApaAppList::CApaAppListStorer::StoreEntryL(RWriteStream& aWriteStream, const CApaAppData& aApp)
	{
	aWriteStream << aApp;
	}

void CApaAppList::CApaAppListStorer::DoCancel()
	{
	iTimer.Cancel();
	}

TInt CApaAppList::CApaAppListStorer::RunError(TInt /*aError*/)
	{
	iAppList.DeleteAppListStorer();
	return KErrNone;
	}

//
// Class CApaIdleIconLoader
//

CApaAppList::CApaIdleIconLoader::CApaIdleIconLoader(CApaAppData* aFirstAppData, RFs& aFs, CApaAppList& aAppList) : CActive(EPriorityLow), iCurrentAppData(aFirstAppData), iFs(aFs), iAppList(aAppList)
	{ // Priority is less than KAppListServerPriority, to maintain server responsiveness.
	CActiveScheduler::Add(this);
	}

CApaAppList::CApaIdleIconLoader::~CApaIdleIconLoader()
	{
	Cancel();
	iCurrentAppData = NULL;
	}

void CApaAppList::CApaIdleIconLoader::Start()
	{
	SetActive();
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	}

void CApaAppList::CApaIdleIconLoader::RunL()
/** if the icons are not already loaded on demand then it would be loaded here. */
	{
	while (iCurrentAppData && !iCurrentAppData->MbmIconsRequireLoading())
		iCurrentAppData = iCurrentAppData->Next();

	if(iCurrentAppData)
		{
		Start();
		CApaAppData* const appData = iCurrentAppData;
		iCurrentAppData = iCurrentAppData->Next();
		appData->LoadIconsL();
		}
	else
		{
		iAppList.InitiateStoringOfAppList();
		iAppList.DeleteAppIconLoader();
		}
	}

void CApaAppList::CApaIdleIconLoader::DoCancel()
	{
	}

TInt CApaAppList::CApaIdleIconLoader::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}


//
// Class CApaLangChangeMonitor
//

CApaAppList::CApaLangChangeMonitor::~CApaLangChangeMonitor()
	{	
	Cancel();
	iLangNotifier.Close();
	}

CApaAppList::CApaLangChangeMonitor* CApaAppList::CApaLangChangeMonitor::NewL(CApaAppList& aAppList)
	{ // static	
	CApaLangChangeMonitor* self=new(ELeave) CApaLangChangeMonitor(aAppList);
	self->ConstructL();
	return self;
	}

CApaAppList::CApaLangChangeMonitor::CApaLangChangeMonitor(CApaAppList& aAppList)
	: CActive(EPriorityNormal),
	iAppList(aAppList)
	{	
	iPrevLanguage = User::Language();
	CActiveScheduler::Add(this);
	}

void CApaAppList::CApaLangChangeMonitor::ConstructL()
	{
	User::LeaveIfError(iLangNotifier.Create());
	Start();
	}
 
void CApaAppList::CApaLangChangeMonitor::Start()
	{
	iLangNotifier.Logon(iStatus);
	SetActive();
	}

void CApaAppList::CApaLangChangeMonitor::DoCancel()
	{
	iLangNotifier.LogonCancel();
	}

void CApaAppList::CApaLangChangeMonitor::RunL()
	{
	// Logon to get further events before handling current request.
	TRequestStatus status = iStatus;
	Start();
	
	// if it is a language change event, start a rescan on app-list.
	if (status.Int() == EChangesLocale && iPrevLanguage != User::Language())
		{		
		iPrevLanguage = User::Language();
		iAppList.iFlags |= CApaAppList::ELangChangePending;
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK		
		iAppList.InitializeLangAppListL();
#endif
		iAppList.StartIdleUpdateL(iAppList.iObserver);
		}
	}

TInt CApaAppList::CApaLangChangeMonitor::RunError(TInt /*aError*/)
	{
	// Reset ELangChangePending flag if RunL leaves.
	iAppList.iFlags &= ~CApaAppList::ELangChangePending;
	// Reset iPrevLanguage to ELangNone if RunL leaves.
	iPrevLanguage = ELangNone;
	return KErrNone;
	}

//
// CCustomAppInfoData
//

CApaAppList::CCustomAppInfoData* CApaAppList::CCustomAppInfoData::NewL(TUid aAppUid, TLanguage aLanguage, const TDesC& aShortCaption)
	{
	CCustomAppInfoData* self=new(ELeave) CCustomAppInfoData(aAppUid, aLanguage);
    CleanupStack::PushL(self);
	self->ConstructL(aShortCaption);
	CleanupStack::Pop(self);
	return self;
	}

CApaAppList::CCustomAppInfoData::~CCustomAppInfoData()
	{
	delete iShortCaption;
	}

CApaAppList::CCustomAppInfoData::CCustomAppInfoData(TUid aAppUid, TLanguage aLanguage)
	:iUid(aAppUid),
	iLanguage(aLanguage)
	{
	}

void CApaAppList::CCustomAppInfoData::ConstructL(const TDesC& aShortCaption)
	{
	iShortCaption=aShortCaption.AllocL();
	}

TUid CApaAppList::CCustomAppInfoData::Uid() const
	{ return iUid; }
	
TLanguage CApaAppList::CCustomAppInfoData::Language() const
	{ return iLanguage; }
	
HBufC* CApaAppList::CCustomAppInfoData::ShortCaption() const
	{ return iShortCaption; }

// The function transfers ownership of the pointer owned by a CApaAppList to the caller
// to avoid copying the array.

EXPORT_C CArrayFixFlat<TUid>* CApaAppList::UninstalledAppArray()
    {
    CArrayFixFlat<TUid>* uninstalledApps=iUninstalledApps;
    iUninstalledApps=NULL;
    return uninstalledApps;
    }

