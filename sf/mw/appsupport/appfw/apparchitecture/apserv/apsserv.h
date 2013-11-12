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
// apsserv.h
//

#ifndef __APSSERV_H__
#define __APSSERV_H__

#include "../aplist/aplapplist.h" 	// class MApaAppListObserver
#include <e32base.h>
#include <f32file.h>
#include <apmfndr.h>
#include <aprulebased.h>
#include <w32std.h>
#include <apgaplst.h>

class CCustomAppInfoData;
class CApaAppList;
class CApaFsMonitor;
class CTypeStoreManager;
class CApaEComMonitor;
class CBaBackupSessionWrapper;
class CApsRecognitionCache;
class CRecognitionResult;
class CApaAppInstallationMonitor;


/**
KAppListServerName 
@internalAll
*/
_LIT(KAppListServerName,"!AppListServer");


/**
CApaAppArcServer 
@internalAll 
*/
class CApaAppArcServer : public CPolicyServer, public MApaAppListObserver, public MBackupOperationObserver
	{
public:
	IMPORT_C static CApaAppArcServer* NewL();
	IMPORT_C static CApaAppArcServer* Self();

	inline RWsSession& WsSession();
	
	inline CApaScanningRuleBasedPlugIns* RuleBasedPlugIns();
		
	// Application list stuff
	inline CApaAppList& AppList();
	TPtrC NativeExecutableL(TUid aNonNativeApplicationType);

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
    void UpdateApps();
    IMPORT_C TCallBack RescanCallBack();
    void UpdateAppsByForceRegistration();   
    void RegisterNonNativeApplicationTypeL(TUid aApplicationType, const TDesC& aNativeExecutable);
    void DeregisterNonNativeApplicationTypeL(TUid aApplicationType);
    inline const CApaFsMonitor& AppFsMonitor() const {return *iAppFsMonitor;}
	IMPORT_C void HandleInstallationStartEvent();
	IMPORT_C void HandleInstallationEndEventL();
#endif
	
	// MIME-type recognition
	inline CApaDataRecognizer* MimeTypeRecognizer();
	CRecognitionResult* RecognizeDataAsCRecognitionResultL(RFile& aFile, TInt aPreferredBufSize);
	TDataRecognitionResult RecognizeDataL(const TDesC& aName, const TDesC8& aBuffer);
	TDataRecognitionResult RecognizeDataL(RFile& aFile, TInt aPreferredBufSize);
	TBool RecognizeDataL(const TDesC& aName, const TDesC8& aBuffer, const TDataType& aDataType);
	TBool RecognizeDataL(RFile& aFile, TInt aPreferredBufSize, const TDataType& aDataType);
	TInt DataRecognizerPreferredBufSizeL();
	void DataTypeL(CDataTypeArray& aArray);
	
	void GetAppForMimeType(const TDataType& aDataType, TUid& aUid) const;
	void GetAppForMimeType(const TDataType& aDataType, TUid aServiceUid, TUid& aUid) const;
	TBool InsertAndStoreIfHigherL(const TDataType& aDataType, TDataTypePriority aPriority, TUid aUid);
	void InsertAndStoreDataMappingL(const TDataType& aDataType, TDataTypePriority aPriority, TUid aUid, TUid aServiceUid);
	void DeleteAndStoreDataMappingL(const TDataType& aDataType, TUid aServiceUid);

	
    
#ifdef _DEBUG
	void FlushRecognitionCache();
	void SetLoadRecognizersOnDemandL(TBool aLoadRecognizersOnDemand);
	void PerformOutstandingRecognizerUnloading();
#endif //_DEBUG
public: // from MApaAppListObserver
	void NotifyUpdate(TInt aReason);
	void InitialListPopulationComplete();
public: // from MBackupOperationObserver
	void HandleBackupOperationEventL(const TBackupOperationAttributes& aBackupOperationAttributes);
public:	//
	IMPORT_C ~CApaAppArcServer();
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
	TBool NonNativeRecovery() const;
	void SetNonNativeRecovery(TBool aValue);
#else
    void UpdateAppListL(RArray<TApaAppUpdateInfo>* aAppUpdateInfo, TUid aSecureID);
#endif
	TBool LoadMbmIconsOnDemand() const;

private:
	CApaAppArcServer(TInt aPriority);
	void ConstructL();
	virtual CSession2* NewSessionL(const TVersion& aVersion,const RMessage2& aMessage) const;
	static TInt PlugInNotifyCallBack(TAny* aPtr);
	static TInt TypeStoreNotifyCallback(TAny* aPtr);
	void UpdatePlugIns();
	void UpdateTypeStore();
	void DoUpdateTypeStoreL();
	
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
    void InitNonNativeApplicationTypeArrayL();
    TPtrC FindAndAddNonNativeRuntimeMappingL(TUid aNonNativeApplicationType);
#else
    static TInt AppFsNotifyWithForcedRegistrationsResetCallBack(TAny* aPtr);
    static TInt AppFsNotifyCallBack(TAny* aPtr);
	void InternalizeNonNativeApplicationTypeArrayL();
    void ExternalizeNonNativeApplicationTypeArrayL(TInt aIndexToIgnore=-1) const;
    void ConstructPathsToMonitorL();    
#endif
	static void DeleteLastNonNativeApplicationType(TAny* aThis);
	void NotifyScanComplete();
	void DeleteCustomAppInfoList();
	void LoadRecognizersLC();
	void LoadRecognizersL();
	static void RecognizerCleanup(TAny* aAny);
	TInt UnloadRecognizers();
	static TInt DoUnloadRecognizersCallback(TAny* aAny);
	TInt DoUnloadRecognizers();
	TBool CachedRecognitionResult(const TParseBase& aParser, TDataRecognitionResult& aResult) const;
	CRecognitionResult* CachedRecognitionResult(const RFile& aFile, const TParseBase& aParser) const;
	void CacheRecognitionResultL(const TParseBase& aParser, const TDataRecognitionResult& aResult);
	void CacheRecognitionResultL(const RFile& aFile, const TParseBase& aParser, const TDataRecognitionResult& aResult);
private:
	enum
		{
		EOwnsFileRecognizer=0x01,
		EOwnsAppList=0x02
		};

	enum TForceRegistrationStatus
	    {
	    EForceRegistrationNone=0x00,
	    EForceRegistrationRequested=0x01,
	    EForceRegistrationAppListChanged=0x02
	    };
	
	struct SNonNativeApplicationType
		{
		TUid iTypeUid;
		HBufC* iNativeExecutable;
		};
private:
	RFs iFs;
	TInt iRecognizerUsageCount;
	CApaAppList* iAppList;
	CPeriodic* iRecognizerUnloadTimer;
	CApaScanningDataRecognizer* iMimeTypeRecognizer;
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
	CApaFsMonitor* iAppFsMonitor;
#endif
	CApaFsMonitor* iTypeStoreMonitor;
	CTypeStoreManager* iMimeTypeToAppMappingsManager;
	TTime iTypeStoreModified;
	CBaBackupSessionWrapper* iBaBackupSessionWrapper;
	CApaEComMonitor* iRecEcomMonitor;
	CApsRecognitionCache* iRecognitionCache;
	CApaScanningRuleBasedPlugIns* iRuleBasedPlugIns;
	RWsSession iWsSession;
	RArray<SNonNativeApplicationType> iNonNativeApplicationTypeArray;
	/**
	This is ETrue if there was a power failure during the non-native applications list
	update. It means that there is a list fo updated apps that needs to be rolled back.
	*/
	TBool iNonNativeRecovery;

	TBool iLoadRecognizersOnDemand;
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
	CApaAppInstallationMonitor* iAppInstallationMonitor; //CApaAppInstallationMonitor monitors installation and uninstallation of applications.
    RBuf iNonNativeApplicationTypeRegistry;	
    TInt iForceRegistrationStatus;
#endif
	TBool iLoadMbmIconsOnDemand;

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
    RArray<TApaAppUpdateInfo> iAppUpdateInfo;
#endif
	
	};


/**
The TApaAppViewIconSizeData class encapsulates the attributes that specify how to identify an icon for a particular
application and view in a certain size.

@internalComponent
*/
class TApaAppViewIconSizeData
	{
public:
	inline TApaAppViewIconSizeData();
	inline TApaAppViewIconSizeData(TUid aAppUid,TUid aViewUid, const TSize& aSize);
public:
	TUid iAppUid;
	TUid iViewUid;
	TSize iSize;
	};

typedef TPckgBuf<TApaAppViewIconSizeData> TApaAppViewIconSizeDataBuf;

//
// inlines
//

inline TApaAppViewIconSizeData::TApaAppViewIconSizeData()
	: iAppUid(KNullUid), iViewUid(KNullUid), iSize(TSize(0,0))
	{}

inline TApaAppViewIconSizeData::TApaAppViewIconSizeData(TUid aAppUid,TUid aViewUid, const TSize& aSize)
	: iAppUid(aAppUid), iViewUid(aViewUid), iSize(aSize)
	{}

inline CApaAppList& CApaAppArcServer::AppList()
	{ return *iAppList; }

inline CApaDataRecognizer* CApaAppArcServer::MimeTypeRecognizer()
	{ return iMimeTypeRecognizer; }

inline CApaScanningRuleBasedPlugIns* CApaAppArcServer::RuleBasedPlugIns()
	{ return iRuleBasedPlugIns; }

inline RWsSession& CApaAppArcServer::WsSession()
		{ return iWsSession; } //lint !e1536 Exposing low access member


	
#endif // __APSSERV_H__



