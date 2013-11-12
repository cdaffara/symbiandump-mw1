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
// aplapplist.h
//

#ifndef __APLAPPLIST_H__
#define __APLAPPLIST_H__

/*#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <apmfndr.h>
#else
#include <apmstd.h>
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS
*/
#include <apaid.h>
#include <badesca.h>
#include <s32file.h>

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
#include <usif/scr/scr.h>
#include <usif/scr/appregentries.h>
#include <apgcli.h>
#endif

// classes defined:
class CApaAppList;
class CApaAppViewData;
// classes referenced:
class CApaMaskedBitmap;
class TEntry;
class RFs;
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
class CApaAppRegFinder;
#endif
class CApaAppIconArray;
class CApaIconLoader;
class TApaAppEntry;
class CApaAppData;
class CApaIconCaptionOverrides;
class CApaIconCaptionCenrepObserver;

/**
KIgnoreScreenMode

@internalTechnology
*/
const TInt KIgnoreScreenMode=-1;


/**
The MApaAppListObserver interface allows a class to be informed when a CApaAppList is 
updated.

@internalComponent
*/
class MApaAppListObserver
	{
public:
	virtual void NotifyUpdate(TInt aReason)=0;
	virtual void InitialListPopulationComplete()=0;
	virtual void NotifyScanComplete()=0;
 	};

/**
The CApaAppList class provides a list of all available applications present on a device.
Its functionality should be accessed through the Apparc Server.

@internalComponent
*/
class CApaAppList : public CBase
	{
public:
    IMPORT_C static CApaAppList* NewL(RFs& aFs, TBool aLoadMbmIconsOnDemand, TInt aTimeoutDelay = 50000); 
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
	IMPORT_C void PurgeL();
    IMPORT_C CApaAppData* FindAndAddSpecificAppL(CApaAppRegFinder* aFinder, TUid aAppUid);	
    IMPORT_C void StopScan(TBool aNNAInstall = EFalse);
    IMPORT_C void RestartScanL(); 
    IMPORT_C TBool AppListUpdatePending();    
    IMPORT_C void AddForcedRegistrationL(const TDesC& aRegistrationFile);
    IMPORT_C void ResetForcedRegistrations();
#else
    IMPORT_C CApaAppData* FindAndAddSpecificAppL(TUid aAppUid);    
#endif
    
	IMPORT_C TInt Count() const;
	IMPORT_C CApaAppData* FirstApp() const;
	IMPORT_C CApaAppData* FirstApp(TInt aScreenMode) const; 
	IMPORT_C CApaAppData* NextApp(const CApaAppData* aApp) const;
	IMPORT_C CApaAppData* NextApp(const CApaAppData* aApp, TInt aScreenMode) const;
	IMPORT_C CApaAppData* AppDataByUid(TUid aAppUid) const;
	// ER5
	IMPORT_C TUid PreferredDataHandlerL(const TDataType& aDataType) const;
	IMPORT_C void StartIdleUpdateL();
	IMPORT_C void StartIdleUpdateL(MApaAppListObserver* aObserver);
	IMPORT_C void InitListL(MApaAppListObserver* aObserver);
	IMPORT_C TBool IsIdleUpdateComplete() const;
	//
	IMPORT_C TBool IsFirstScanComplete() const;
	IMPORT_C TBool AppScanInProgress() const;
	IMPORT_C CBufFlat* ServiceArrayBufferL(TUid aAppUid) const;
	IMPORT_C CBufFlat* ServiceImplArrayBufferL(TUid aServiceUid) const;
	IMPORT_C CBufFlat* ServiceImplArrayBufferL(TUid aServiceUid, const TDataType& aDataType) const;	
	IMPORT_C CBufFlat* ServiceUidBufferL(TUid aAppUid) const;
	IMPORT_C CBufFlat* ServiceOpaqueDataBufferL(TUid aAppUid, TUid aServiceUid) const;
	IMPORT_C TUid PreferredDataHandlerL(const TDataType& aDataType, const TUid* aServiceUid, 
		TInt& aPriority) const;
	IMPORT_C ~CApaAppList();
	// 9.1
	IMPORT_C CApaAppData* AppDataByFileName(const TDesC& aFullFileName) const;
	/*IMPORT_C*/ RFs& ShareProtectedFileServer();
	IMPORT_C TBool IsLanguageChangePending() const;
	IMPORT_C static CApaAppList* Self();
    IMPORT_C CArrayFixFlat<TUid>* UninstalledAppArray();
    
	void AcquireDefaultIconArrayL() const;
	const CApaAppIconArray& DefaultIconArray() const;
	void ReleaseDefaultIconArray() const;

	void StoreL();
	void NotifyObserver();
	void DeleteAppListStorer();
	void DeleteAppIconLoader();
	void InitiateStoringOfAppList();
	
	IMPORT_C void AddCustomAppInfoInListL(TUid aAppUid, TLanguage aLanguage, const TDesC& aShortCaption);
	IMPORT_C void UpdateAppListByShortCaptionL();
	IMPORT_C void UpdateAppListByIconCaptionOverridesL();

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
	IMPORT_C void InitializeApplistL(MApaAppListObserver* aObserver);
	void InitializeLangAppListL();
	IMPORT_C void UpdateApplistL(MApaAppListObserver* aObserver, RArray<TApaAppUpdateInfo>* aAppUpdateInfo, TUid aSecureID);
	IMPORT_C void UpdateApplistByForceRegAppsL(RPointerArray<Usif::CApplicationRegistrationData>& aForceRegAppsInfo);
	IMPORT_C CArrayFixFlat<TApaAppUpdateInfo>* UpdatedAppsInfo();
#endif
	
private:
	enum
		{
		EFirstScanComplete = 0x01,
		EAppListHasChanged = 0x02,	// This flag is used to check if the applist has really changed after a re-scan/update scan.
		ENotifyUpdateOnFirstScanComplete = 0x04,// This flag is used to notify clients for applist update on first boot when AppsList.Bin is used.
		ELangChangePending = 0x08 // This flag is used to check if applist update is in progress on language change event.
		};
private:
    CApaAppList(RFs& aFs, TBool aLoadMbmIconsOnDemand, TInt aIdlePeriodicDelay); 
	void AddToList( CApaAppData* aAppData );
	static TInt IdleUpdateCallbackL(TAny* aObject);
	TInt IdleUpdateL();
	void ScanComplete();

	void StopIdler();
	void DeleteAppData();
	CArrayFixFlat<TDataTypeWithPriority>* DataTypeArrayDeepCopyLC(const CArrayFixFlat<TDataTypeWithPriority>& aOriginal) const;
	CApaAppIconArray* LoadDefaultIconsL() const;
	void UpdateDefaultIconsL();

	void StartIconLoadingL();
	void DeleteAppsListBackUpAndTempFiles();
	void ScanRemovableDrivesAndUpdateL();
	void CreateDefaultAppIconFileNameL();
	
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
    void UndoSetPending(CApaAppData* aAppData);
    static void SetPending(CApaAppData* aAppData);
    static void SetNotFound(CApaAppData* aAppData, TBool& aHasChanged);
    void UpdateNextAppL(const TApaAppEntry& aAppEntry,TBool& aHasChanged);
#else
    TInt FindAndDeleteApp(TUid aAppUid);
#endif
    
private: 
	// Persistence Layer
	void RestoreL();
	void ConstructL();

	/**
	Utility class used to Load Icons once applist is populated
	@internalComponent
	*/	
	NONSHARABLE_CLASS(CApaIdleIconLoader) : public CActive
		{
	public:
		CApaIdleIconLoader(CApaAppData* aFirstAppData, RFs& aFs, CApaAppList& aAppList);
		~CApaIdleIconLoader();
		void Start();
	private:	// from CActive
		void RunL();
		void DoCancel();
		TInt RunError(TInt aError);
	private:
		CApaAppData* iCurrentAppData;
		RFs& iFs;
		CApaAppList& iAppList;
		};

	/**
	Utility class used to externalize applist to file
	@internalComponent
	*/	
	NONSHARABLE_CLASS(CApaAppListStorer) : public CActive
		{
	public:
		static CApaAppListStorer* NewL(CApaAppData* aFirstAppData, RFs& aFs, CApaAppList& aAppList);
		~CApaAppListStorer();
		void StartL(const TTimeIntervalMicroSeconds32& aDelay);
	private:
		CApaAppListStorer(CApaAppData* aFirstAppData, RFs& aFs, CApaAppList& aAppList);
		static void StoreEntryL(RWriteStream& aWriteStream, const CApaAppData& aApp);
		void ConstructL();
	private:	// from CActive
		void RunL();
		void DoCancel();
		TInt RunError(TInt aError);
	private:
		CApaAppData* iCurrentAppData;
		TFileName iTempFilename;
		RFileWriteStream iWriteStream;
		RTimer iTimer;
		RFs& iFs;
		CApaAppList& iAppList;
		};
	
	/**
	The CCustomAppInfoData class encapsulate the attributes of customised application information
	which will be used to modify short caption dynamically.

	@internalComponent
	*/
	NONSHARABLE_CLASS(CCustomAppInfoData) : public CBase
		{
	public:
		static CCustomAppInfoData* NewL(TUid aAppUid, TLanguage aLanguage, const TDesC& aShortCaption);
		~CCustomAppInfoData();
		inline TUid Uid() const;
		inline TLanguage Language() const;
		inline HBufC* ShortCaption() const;
	private:
		CCustomAppInfoData(TUid aAppUid, TLanguage aLanguage);
		void ConstructL(const TDesC& aShortCaption);
	private:
		TUid iUid;
		TLanguage iLanguage;
		HBufC* iShortCaption;
		};

private:
	RFs& iFs;
	CApaAppData* iAppData; // linked list of apps
	CPeriodic* iAppIdler;
	MApaAppListObserver* iObserver;
	CApaAppData* iValidFirstAppData; //First valid app data in linked list!
	TInt iFlags;
	TInt iIdlePeriodicDelay;    // idle timeout periodic delay	
	RFs iFsShareProtected;
	mutable CApaAppIconArray* iDefaultIconArray;
	mutable TInt iDefaultIconUsageCount;
	class CApaLangChangeMonitor; //inner class of CApaAppList.
	
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
	class CApaScrAppInfo;
#else
   CDesCArray* iForcedRegistrations;
#endif
	
	CApaLangChangeMonitor* iAppLangMonitor; // Active Object used for language change monitoring.		

	RBuf iAppsListCacheFileName;
	RBuf iAppsListCacheBackUpFileName;
	RBuf iAppsListCachePath;

	HBufC* iDefaultAppIconMbmFileName; // This member data lives only during the scan
	CApaAppListStorer* iAppListStorer;	//Active Object used for writing applist to file.
	CApaIdleIconLoader* iAppIconLoader;	//Active Object used for icon handling
	TBool iLoadMbmIconsOnDemand;
	RPointerArray<CCustomAppInfoData> iCustomAppList;
	CApaIconCaptionOverrides* iIconCaptionOverrides;
	CApaIconCaptionCenrepObserver* iIconCaptionObserver;
    CArrayFixFlat<TUid>* iUninstalledApps;
    
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK    
    Usif::RSoftwareComponentRegistry iScr;
    CApaScrAppInfo *iScrAppInfo;
    RArray<TUid> iForceRegAppUids;
    CArrayFixFlat<TApaAppUpdateInfo>* iAppsUpdated;
#else
    CApaAppRegFinder* iAppRegFinder;
    TBool iNNAInstallation; 
#endif
    
private:
	friend class CApaLangChangeMonitor;
	};

#endif //__APLAPPLIST_H__
