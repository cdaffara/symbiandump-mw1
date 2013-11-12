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
// apsses.h
//

#ifndef __APSSES_H__
#define __APSSES_H__

#include <e32base.h>
#include "apsserv.h"
#include <f32file.h>
#include <apaid.h>
#include "APSCLSV.H"
#include "APGAPLST.H"
#include "APSRECUTIL.H"

// classes referenced
class CApaAppList;
class CApaAppData;
class TDataType;
class CApsNonNativeApplicationsManager;
class MArrayItemWriter;
class CFileRecognitionUtility;

class CApaAppListServSession : public CBase
	{
public:
	static CApaAppListServSession* NewL(RFs& aFs, CApaAppArcServer& aAppArcSrv, CApaAppList& aAppList);
	~CApaAppListServSession();

	CApaAppList& AppList();

	void NotifyScanComplete();
	void SetNotify(const RMessage2& aMessage);
	void CancelNotify();
	
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 	
	void NotifyClients(TInt aReason, CArrayFixFlat<TApaAppUpdateInfo>* aUpdatedAppsInfo);
#else
    void NotifyClients(TInt aReason);
#endif	
	
	void NotifyClientForCompletionOfListPopulation();
	void CancelListPopulationCompleteObserver();
public:
	CApaAppData& FindAppInListL(TUid aUid);
	const CApaAppData& FindAppInListL(TUid aUid) const { return const_cast<CApaAppListServSession*>(this)->FindAppInListL(aUid); }
	TBool FindAppInList(CApaAppData*& aApp, TApaAppEntry& aEntry, TUid aAppUid);
	TBool FindAppInList(const CApaAppData*& aApp, TApaAppEntry& aEntry, TUid aAppUid) const 
		{ return const_cast<CApaAppListServSession*>(this)->FindAppInList(const_cast<CApaAppData*&>(aApp), aEntry, aAppUid); }	
	TUid AppUidFromFullFileNameL(const TDesC& aFullFileName) const;
	CApaAppData* FindSpecificAppL(TUid aAppUid);
public:
	void DoServiceL(const RMessage2& aMessage);
private:
	enum TAppListType {
		ENoList,
		EListFilteredEmbeddedApps,
		EListCapabilityAttrFilteredApps,
		EListServerApps
		};
private:
	CApaAppListServSession(RFs& aFs, CApaAppArcServer& aAppArcSrv, CApaAppList& aAppList);
	void ConstructL();
private:	
	void GetAppTypeL(const RMessage2& aMessage);
	void ForceRegistrationL(const RMessage2& aMessage);	
	void RegisterListPopulationCompleteObserver(const RMessage2& aMessage);
	void MatchesSecurityPolicyL(const RMessage2& aMessage);
	
	void AppCount(const RMessage2& aMessage) const;
	void EmbedCount(const RMessage2& aMessage) const;
	void GetNextAppL(const RMessage2& aMessage);
	void InitListL(const RMessage2& aMessage, TAppListType aType);
	void GetAppInfoL(const RMessage2& aMessage);
	void GetAppInfoL(TUid aUid, TApaAppInfo& aInfo);
	void GetAppCapabilityL(const RMessage2& aMessage);
	void GetDefaultScreenNumberL(const RMessage2& aMessage);
	
	void IconForAppL(const RMessage2& aMessage);
	void IconForAppBySizeL(const RMessage2& aMessage);
	void AppIconSizesL(const RMessage2& aMessage);
	void IconFileHandleForAppL(const RMessage2& aMessage);
	void AppViewsL(const RMessage2& aMessage);
	void IconForViewBySizeL(const RMessage2& aMessage);
	void AppFileOwnershipInfoL(const RMessage2& aMessage);

	void NumberOfOwnDefinedIconsL(const RMessage2& aMessage);

	void AppInfoProvidedByRegistrationFileL(const RMessage2& aMessage);
	void IconFileNameL(const RMessage2& aMessage);
	void ViewIconFileNameL(const RMessage2& aMessage);
	
	void ApplicationLanguageL(const RMessage2& aMessage);
	void SetAppShortCaptionL(const RMessage2& aMessage);
	
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
	void UpdateAppListL(const RMessage2& aMessage);
	void UpdatedAppsInfoL(const RMessage2& aMessage);	
#endif
private:		
	static TInt NextDriveToScan(TInt aCurrentDrive);	
	static TBool AppIsControlPanelItem(const CApaAppData& aAppData);
	TBool AppMatchesEmbeddabilityFilter(const CApaAppData& aAppData, const TApaEmbeddabilityFilter& aEmbeddabilityFilter) const;
	TBool AppMatchesCapabilityAttrFilter(const CApaAppData& aAppData) const;
	void SendArrayL(const MArrayItemWriter& aArrayItemWriter,const RMessage2& aMessage) const;	
private:
	class CApaAppInfo;
private:
	enum TFlags
		{
		ENotifyPresent=0x01,
		EAppListPopulationPending=0x02
		};
private:
	RFs& iFs;
	TInt iFlags;
	CApaAppArcServer& iAppArcSrv;
	CApaAppList& iAppList;
	RMessage2 iNotifyMessage;
	RMessage2 iNotifyOnScanCompleteMsg;
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 
	CApsNonNativeApplicationsManager* iNonNativeApplicationsManager;
#endif
	RMessage2 iCompletionOfListPopulationObserverMsg;
		
	TAppListType iAppListType;
	TInt iAppListScreenMode;
	TApaEmbeddabilityFilter iEmbeddabilityFilter;
	TUint iCapabilityAttrFilterMask; // contains bit flags from TCapabilityAttribute
	TUint iCapabilityAttrFilterValue; // contains bit flags from TCapabilityAttribute
	TUid iServiceUid;
	RPointerArray<CApaAppInfo> iApaAppInfoArray;	//contains the most recent "snapshot" of the applist taken by GetNextAppL.
	
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
	RArray<TApaAppUpdateInfo> iAppsUpdated;
	TBool iNotificationRequested; //If its true updated application information is maintained in session object.
#endif
	};
	
	
class CApaAppArcServSession : public CSession2
	{
public:
	// construct/destruct
	static CApaAppArcServSession* NewL(CApaAppArcServer& aServer, RFs& aFs);
	~CApaAppArcServSession();
	//service request
	virtual void ServiceL(const RMessage2 &aMessage);

	void NotifyClientOfDataMappingChange();
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK  
    void NotifyClients(TInt aReason, CArrayFixFlat<TApaAppUpdateInfo>* aUpdatedAppsInfo);
#else
    void NotifyClients(TInt aReason);
#endif  
	void NotifyScanComplete();
	void NotifyClientForCompletionOfListPopulation();
private:
	CApaAppArcServSession(CApaAppArcServer& aServer, RFs& aFs);
	void ConstructL();
	void NotifyOnDataMappingChange(const RMessage2& aMessage);
	void CancelNotifyOnDataMappingChange();
private:
	// Data recognition stuff
	void RecognizeDataL(const RMessage2& aMessage);
	void RecognizeDataPassedByFileHandleL(const RMessage2& aMessage);
	void RecognizeSpecificDataL(const RMessage2& aMessage);
	void RecognizeSpecificDataPassedByFileHandleL(const RMessage2& aMessage);
    static TPtrC8 DoRecognizeUnpackLC(HBufC*& aName, RChunk& aLocalChunk, const RMessage2& aMessage);

	void AppForDataTypeL(const RMessage2& aMessage);
	TUid AppForDataTypeL(const TDataType& aDataType, const TUid* aServiceUid);
	void AppForDocumentL(const RMessage2& aMessage, const TUid* aServiceUid);

	void RecognizeFilesL(const RMessage2& aMessage);
	void TransferRecognitionResultL(const RMessage2& aMessage);
	void RecognizeFilesAsyncL(const RMessage2& aMessage);
	void AquirePermissionToLaunchAppL(const RMessage2& aMessage) const;
	void CancelRecognizeFiles();

	void InsertDataMappingL(const RMessage2& aMessage);
	void DeleteDataMappingL(const RMessage2& aMessage);
	void GetAppByDataTypeL(const RMessage2& aMessage) const;

	void GetAppServicesL(const RMessage2& aMessage);
	CBufBase* GetServiceBufferL(const RMessage2& aMessage) const;
	void AppForDataTypeAndServiceL(const RMessage2& aMessage);
	void AppForDocumentPassedByFileHandleL(const RMessage2& aMessage, const TUid* aServiceUid);

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
	void RegisterNonNativeApplicationTypeL(const RMessage2& aMessage);
	void DeregisterNonNativeApplicationTypeL(const RMessage2& aMessage);
#endif

	void GetExecutableNameGivenDocumentL(const RMessage2& aMessage);
	void GetExecutableNameGivenDocumentPassedByFileHandleL(const RMessage2& aMessage);
	void GetExecutableNameGivenDataTypeL(const RMessage2& aMessage);
	void GetExecutableNameGivenAppUidL(const RMessage2& aMessage);
	void GetExecutableNameL(const RMessage2& aMessage, TUid aAppUid);
	void GetNativeExecutableNameIfNonNativeL(const RMessage2& aMessage);
	void WriteNativeExecutableIfNonNativeAndPrepareForClientRetrievalOfOpaqueDataL(const RMessage2& aMessage, TInt aMessageSlotForNativeExecutable, const CApaAppData& aAppData);
	void GetOpaqueDataL(const RMessage2& aMessage);

	void GetConfidenceL(const RMessage2& aMessage);
	void SetConfidence(const RMessage2& aMessage);
	void GetBufSize(const RMessage2& aMessage);
	void SetBufSize(const RMessage2& aMessage);
	void GetDataTypesCountL(const RMessage2& aMessage);
	void GetDataTypesL(const RMessage2& aMessage);
	
	TInt PreferredBufSize() const;
private:
	CApaAppArcServer& iServ;
	CApaAppListServSession* iAppListSession;
	RFs& iFs;
	TInt iMaxBufSize;
	CBufBase* iBuffer;
	CDirectoryRecognitionResult* iRecognitionResult;
	CFileRecognitionUtility* iFileRecognitionUtility;
	TBool iAsyncRecognitionActive;
	HBufC8* iOpaqueData_pendingDispatchToClient;
	RMessagePtr2 iMessage_NotifyOnDataMappingChange;
	};


#endif	// __APSSES_H__
