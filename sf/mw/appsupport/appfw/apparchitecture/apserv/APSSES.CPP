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
// AppArc server session
// 
// apsses.cpp
//

#include <e32svr.h>
#include <apacmdln.h>

#include <apaflrec.h>
#include "apsserv.h"
#include "APSSES.H"
#ifdef _DEBUG
#include "APSSTD.H"
#endif //_DEBUG
#include "APSCLSV.H"
#include <apsscan.h>
#include <apgaplst.h>
#include <apgicnfl.h>
#include <apmrec.h>
#include <apmstd.h>
#include <apmfndr.h>
#include <datastor.h>
#include <s32mem.h>
#include <s32strm.h>
#include <s32file.h>
#include "../apgrfx/apprivate.h"
#include "apgnotif.h"
#include "ApLaunchChecker.h"
#include "../aplist/aplapplistitem.h"

#include "apsecutils.h"

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
#include "../aplist/aplappregfinder.h"
#include "apsnnapps.h"
#else
#include "usif/scr/scr.h"
#endif

const TInt KApaAppListServMaxBuffer=256;
#include "APSRECCACHE.h"
const TInt KApaAppInfoArrayGranularity = 4;
const TInt KApaAppInfoDesMaxLength = sizeof(TApaAppInfo);

#if defined(__PROFILE)
_LIT(KProfileAppForDocumentL, "AppForDocumentL - %d.%06d seconds");
_LIT(KProfileAppForDocumentPassedByFileHandleL, "AppForDocumentPassedByFileHandleL - %d.%06d seconds");
#endif
_LIT(KApaPanicCli,"APSERV-CLI");
const TInt KFinishedScanning=-2;


class MArrayItemWriter
	{
public:
	virtual TInt ArrayItemCount() const=0;
	virtual TInt ArrayItemSize() const=0;
	virtual void WriteArrayItemL(RWriteStream& aWriteStream,TInt aIndex) const=0;
	};

NONSHARABLE_CLASS(TSizeArrayItemWriter) : public MArrayItemWriter
	{
public:
	inline TSizeArrayItemWriter(const CArrayFix<TSize>& aArray) : iArray(aArray) {}
public: // from MArrayItemWriter
	TInt ArrayItemCount() const;
	TInt ArrayItemSize() const;
	void WriteArrayItemL(RWriteStream& aWriteStream,TInt aIndex) const;
private:
	const CArrayFix<TSize>& iArray;
	};

NONSHARABLE_CLASS(TViewDataArrayItemWriter) : public MArrayItemWriter
	{
public:
	inline TViewDataArrayItemWriter(const CArrayPtr<CApaAppViewData>& aArray) : iArray(aArray) {}
public:	// from MArrayItemWriter
	TInt ArrayItemCount() const;
	TInt ArrayItemSize() const;
	void WriteArrayItemL(RWriteStream& aWriteStream,TInt aIndex) const;
private:
	const CArrayPtr<CApaAppViewData>& iArray;
	};

NONSHARABLE_CLASS(TDesCArrayItemWriter) : public MArrayItemWriter
	{
public:
	inline TDesCArrayItemWriter(const CDesCArray& aArray) : iArray(aArray) {}
public:	// from MArrayItemWriter
	TInt ArrayItemCount() const;
	TInt ArrayItemSize() const;
	void WriteArrayItemL(RWriteStream& aWriteStream,TInt aIndex) const;
private:
	const CDesCArray& iArray;
	};

NONSHARABLE_CLASS(CApaAppListServSession::CApaAppInfo) : public CBase
	{
public:
	CApaAppInfo();
	~CApaAppInfo();
	void SetUid(const TUid aUid);
	void SetCaptionL(const TDesC& aCaption);
	void SetShortCaptionL(const TDesC& aShortCaption);
	void SetFullNameL(const TDesC& aFullName);
	inline TPtrC Caption() const;
	inline TPtrC ShortCaption() const;
	inline TPtrC FullName() const;
	inline TUid Uid() const;
private:
	TUid iUid;
	HBufC* iCaption;
	HBufC* iShortCaption;
	HBufC* iFullName;
	};

inline TPtrC CApaAppListServSession::CApaAppInfo::Caption() const
	{ return *iCaption; }

inline TPtrC CApaAppListServSession::CApaAppInfo::ShortCaption() const
	{ return *iShortCaption; }

inline TPtrC CApaAppListServSession::CApaAppInfo::FullName() const
	{ return *iFullName; }

inline TUid CApaAppListServSession::CApaAppInfo::Uid() const
	{ return iUid; }

//
// CApaAppListServSession
//

CApaAppListServSession* CApaAppListServSession::NewL(RFs& aFs, CApaAppArcServer& aAppArcSrv, CApaAppList& aAppList)
	{
	CApaAppListServSession* self = new (ELeave) CApaAppListServSession(aFs, aAppArcSrv, aAppList);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK  
CApaAppListServSession::CApaAppListServSession(RFs& aFs, CApaAppArcServer& aAppArcSrv, CApaAppList& aAppList)
 : iFs(aFs), iAppArcSrv(aAppArcSrv), iAppList(aAppList), iApaAppInfoArray(KApaAppInfoArrayGranularity)
	{

	}
#else
CApaAppListServSession::CApaAppListServSession(RFs& aFs, CApaAppArcServer& aAppArcSrv, CApaAppList& aAppList)
 : iFs(aFs), iAppArcSrv(aAppArcSrv), iAppList(aAppList), iApaAppInfoArray(KApaAppInfoArrayGranularity), 
   iNotificationRequested(EFalse)
	{

	}
#endif

void CApaAppListServSession::ConstructL()
	{
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK    
	iNonNativeApplicationsManager = CApsNonNativeApplicationsManager::NewL(iAppArcSrv,iFs);
#endif
	}
	

CApaAppListServSession::~CApaAppListServSession()
	{
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK    
	delete iNonNativeApplicationsManager;
#endif
	iApaAppInfoArray.ResetAndDestroy();
	iApaAppInfoArray.Close();
	}

CApaAppList& CApaAppListServSession::AppList()
	{
	return iAppList;
	}

	
void CApaAppListServSession::DoServiceL(const RMessage2& aMessage)
	{
	TBool completeMessage = ETrue;
	const TInt opcode = aMessage.Function();
	switch (opcode)
		{
	case ESetNotify:
		SetNotify(aMessage);
		completeMessage=EFalse;
		break;
	case ERegisterListPopulationCompleteObserver:
		RegisterListPopulationCompleteObserver(aMessage);
		completeMessage=EFalse;
		break;
	case ECancelListPopulationCompleteObserver:
		CancelListPopulationCompleteObserver();
		break;
	case EAppListServInitFullList:
		User::Leave(KErrNotSupported);
	case EAppListServInitEmbedList:
		User::Leave(KErrNotSupported);
	case EAppListServInitFilteredEmbedList:
		InitListL(aMessage,EListFilteredEmbeddedApps);
		break;
	case EAppListServInitAttrFilteredList:
		InitListL(aMessage,EListCapabilityAttrFilteredApps);
		break;
	case EAppListServInitServerAppList:
		InitListL(aMessage,EListServerApps);
		break;
	case EAppListServGetNextApp:
		GetNextAppL(aMessage);
		break;
	case EAppListServEmbedCount:
		EmbedCount(aMessage);
		break;
	case EAppListServAppCount:
		AppCount(aMessage);
		break;
	case EAppListServGetAppInfo:
		GetAppInfoL(aMessage);
		break;
	case EAppListServGetAppCapability:
		GetAppCapabilityL(aMessage);
		break;
	case EAppListServGetDefaultScreenNumber:
		GetDefaultScreenNumberL(aMessage);
		break;
	case EAppListServAppIconByUid:
		IconForAppL(aMessage);
		break;
	case ECancelNotify:
		CancelNotify();
		break;
	case EAppListServAppIconByUidAndSize:
		IconForAppBySizeL(aMessage);
		break;
	case EAppListServAppIconFileHandle:
		IconFileHandleForAppL(aMessage);
		break;	
	case EAppListServGetAppIconSizes:
		AppIconSizesL(aMessage);
		break;
	case EAppListServViewIconByUidAndSize:
		IconForViewBySizeL(aMessage);
		break;
	case EAppListServGetAppViews:
		AppViewsL(aMessage);
		break;
	case EAppListServGetFileOwnershipInfo:
		AppFileOwnershipInfoL(aMessage);
		break;
	case EAppListServNumberOfOwnDefinedIcons:
		NumberOfOwnDefinedIconsL(aMessage);
		break;
	case EAppListServApplicationLanguage:
		ApplicationLanguageL(aMessage);
		break;
	case EAppListServAppInfoProvidedByRegistrationFile: // private OpCode for CEikApplication's use only
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
		AppInfoProvidedByRegistrationFileL(aMessage);
#else
		ASSERT(0);  // panic debug only
        User::Leave(KErrNotSupported);
#endif		
		break;
	case EAppListServAppIconFileName:
		IconFileNameL(aMessage);
		break;
	case EAppListServAppViewIconFileName:
		ViewIconFileNameL(aMessage);
		break;
	case EAppListServPrepareNonNativeApplicationsUpdates:
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 	
		iNonNativeApplicationsManager->PrepareNonNativeApplicationsUpdatesL();
#else
        ASSERT(0);  // panic debug only
        User::Leave(KErrNotSupported);
#endif		
		break;
	case EAppListServRegisterNonNativeApplication:
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 	
		iNonNativeApplicationsManager->RegisterNonNativeApplicationL(aMessage);
#else
        ASSERT(0);  // panic debug only
        User::Leave(KErrNotSupported);
#endif		
		break;
	case EAppListServDeregisterNonNativeApplication:
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 	
		iNonNativeApplicationsManager->DeregisterNonNativeApplicationL(aMessage);
#else
        ASSERT(0);  // panic debug only
        User::Leave(KErrNotSupported);
#endif
		break;
	case EAppListServCommitNonNativeApplications:
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 	
		iNonNativeApplicationsManager->CommitNonNativeApplicationsUpdatesL(aMessage);
		completeMessage=EFalse;
#else
        ASSERT(0);  // panic debug only
        User::Leave(KErrNotSupported);
#endif		
		break;
	case EAppListServRollbackNonNativeApplications:
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 	
		iNonNativeApplicationsManager->RollbackNonNativeApplicationsUpdates();
#else
        ASSERT(0);  // panic debug only
        User::Leave(KErrNotSupported);
#endif		
		break;
	case EAppListServGetAppType:
		GetAppTypeL(aMessage);
		break;
	case EAppListServForceRegistration:
		ForceRegistrationL(aMessage);
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 		
		completeMessage=EFalse;
#endif		
		break;
	case EMatchesSecurityPolicy:
		MatchesSecurityPolicyL(aMessage);
		break;
	case EAppListServSetAppShortCaption:
		SetAppShortCaptionL(aMessage);
		break;
	case EDebugClearAppInfoArray:
	#ifdef _DEBUG
		iApaAppInfoArray.ResetAndDestroy();
		iApaAppInfoArray.Compress();
	#endif
		break;
	case EDebugAddFailingNonNativeApplicationsUpdate:
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 	
	#ifdef _DEBUG
		iNonNativeApplicationsManager->ForceFailInNonNativeApplicationsUpdatesL();
	#endif
#endif
		break;
	case EDebugAddPanicingNonNativeApplicationsUpdate:
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 	
	#ifdef _DEBUG
		iNonNativeApplicationsManager->ForcePanicInNonNativeApplicationsUpdatesL();
	#endif
#endif	
		break;
	case EDebugAddRollbackPanicingNonNativeApplicationsUpdate:
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 	
	#ifdef _DEBUG
		iNonNativeApplicationsManager->ForcePanicInNonNativeApplicationsRollbackL();
	#endif
#endif	
		break;
		
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 		
	case EAppListServUpdateAppList:
	    UpdateAppListL(aMessage);
	    break;
    case EAppListUpdatedAppsInfo:
        UpdatedAppsInfoL(aMessage);
        break;
#endif	    
	default:
		aMessage.Panic(KApaPanicCli,EClientBadRequest);
		break;
	
		}
	
	if (completeMessage && !aMessage.IsNull())
		aMessage.Complete(KErrNone);
	}


// CApaAppArcServSession

CApaAppArcServSession* CApaAppArcServSession::NewL(CApaAppArcServer& aServer, RFs& aFs)
	{
	CApaAppArcServSession* self=new(ELeave) CApaAppArcServSession(aServer, aFs);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self
	return self;
	}

CApaAppArcServSession::CApaAppArcServSession(CApaAppArcServer& aServer, RFs& aFs)
	: CSession2(),
	iServ(aServer),
	iFs(aFs),
	iMaxBufSize(KApaAppListServMaxBuffer),
	iOpaqueData_pendingDispatchToClient(NULL)
	{}

void CApaAppArcServSession::ConstructL()
	{
	iAppListSession = CApaAppListServSession::NewL(iFs, iServ, iServ.AppList());
	iFileRecognitionUtility = new (ELeave) CFileRecognitionUtility(iServ, iMaxBufSize, iFs);
	}

CApaAppArcServSession::~CApaAppArcServSession()
	{
	delete iAppListSession;
	delete iBuffer;
	delete iFileRecognitionUtility;
	delete iRecognitionResult;
	delete iOpaqueData_pendingDispatchToClient;
	}

void CApaAppArcServSession::ServiceL(const RMessage2& aMessage)
	{
#ifdef APPARC_SHOW_TRACE    
    TFullName* name = new(ELeave) TFullName();
    RThread client;
    if ( aMessage.Client( client ) == KErrNone )
        {
        client.FullName( *name );
        client.Close();
        }
    RDebug::Print( _L("[Apparc] CApaAppListServSession::ServiceL(0x%08x) - START - op code: %04d, client: %S"), this, aMessage.Function(), name );
    delete name; 
#endif
    
	TBool completeMessage = ETrue;
	switch (aMessage.Function())
		{
	case EAppListServStartAppWithoutReturningThreadId:
		ASSERT(0);	// panic debug only
		User::Leave(KErrNotSupported);
		break;
	case EAppListServStartAppReturningThreadId:
		ASSERT(0);	// panic debug only
		User::Leave(KErrNotSupported);
		break;
	case EAppListServRecognizeData:
		RecognizeDataL(aMessage);
		break;
	case EAppListServRecognizeDataPassedByFileHandle:
		RecognizeDataPassedByFileHandleL(aMessage);
		break;
	case EAppListServRecognizeSpecificData:
		RecognizeSpecificDataL(aMessage);
		break;
	case EAppListServRecognizeSpecificDataPassedByFileHandle:
		RecognizeSpecificDataPassedByFileHandleL(aMessage);
		break;
	case EAppListServAppForDataType:
		AppForDataTypeL(aMessage);
		break;
	case EAppListServStartDocument:
		ASSERT(0);	// panic debug only
		User::Leave(KErrNotSupported);
		break;
	case EAppListServStartDocumentByDataType:
		ASSERT(0);	// panic debug only
		User::Leave(KErrNotSupported);
		break;
	case EAppListServStartDocumentByUid:
		ASSERT(0);	// panic debug only
		User::Leave(KErrNotSupported);
		break;
	case EAppListServCreateDocumentByUid:
		ASSERT(0);	// panic debug only
		User::Leave(KErrNotSupported);
		break;
	case EAppListServGetExecutableNameGivenDocument:
		GetExecutableNameGivenDocumentL(aMessage);
		break;
	case EAppListServGetExecutableNameGivenDocumentPassedByFileHandle:
		GetExecutableNameGivenDocumentPassedByFileHandleL(aMessage);
		break;
	case EAppListServGetExecutableNameGivenDataType:
		GetExecutableNameGivenDataTypeL(aMessage);
		break;
	case EAppListServGetExecutableNameGivenAppUid:
		GetExecutableNameGivenAppUidL(aMessage);
		break;
	case EAppListServGetOpaqueData:
		GetOpaqueDataL(aMessage);
		break;
	case EAppListServGetNativeExecutableNameIfNonNative:
		GetNativeExecutableNameIfNonNativeL(aMessage);
		break;
	case EAppListServAppForDocument:
		AppForDocumentL(aMessage, NULL);
		break;
	case EAppListServAppForDocumentPassedByFileHandle:
		AppForDocumentPassedByFileHandleL(aMessage, NULL);
		break;
	case EAppListServGetConfidence:
		GetConfidenceL(aMessage);
		break;
	case EAppListServSetConfidence:
		SetConfidence(aMessage);
		break;
	case EAppListServGetBufSize:
		GetBufSize(aMessage);
		break;
	case EAppListServSetBufSize:
		SetBufSize(aMessage);
		break;
	case EAppListServGetDataTypesPhase1:
		GetDataTypesCountL(aMessage);
		break;
	case EAppListServGetDataTypesPhase2:
		GetDataTypesL(aMessage);
		break;
	case EAppListInsertDataMapping:
	case EAppListInsertDataMappingIfHigher:
		InsertDataMappingL(aMessage);
		break;
	case EAppListDeleteDataMapping:
		DeleteDataMappingL(aMessage);
		break;
	case EAppListServGetAppByDataType:
		GetAppByDataTypeL(aMessage);
		break;		
	case EAppListServGetAppServices:
	case EAppListServGetServiceImplementations:
	case EAppListServGetServiceImplementationsDataType:
	case EAppListServGetAppServiceUids:
	case EAppListServGetAppServiceOpaqueData:
		GetAppServicesL(aMessage);
		break;
	case EAppListServAppForDataTypeAndService:
		AppForDataTypeAndServiceL(aMessage);
		break;
	case EAppListServAppForDocumentAndService:
		{
		const TUid serviceUid=TUid::Uid(aMessage.Int1());
		AppForDocumentL(aMessage, &serviceUid);
		}
		break;
	case EAppListServAppForDocumentAndServicePassedByFileHandle:
		{
		const TUid serviceUid(TUid::Uid(aMessage.Int1()));
		AppForDocumentPassedByFileHandleL(aMessage, &serviceUid);
		}
		break;
	case EAppListServRegisterNonNativeApplicationType:
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
		RegisterNonNativeApplicationTypeL(aMessage);
#else
        ASSERT(0);  // panic debug only
        User::Leave(KErrNotSupported);
#endif
		break;
	case EAppListServDeregisterNonNativeApplicationType:
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
		DeregisterNonNativeApplicationTypeL(aMessage);
#else
        ASSERT(0);  // panic debug only
        User::Leave(KErrNotSupported);
#endif		
		break;
	case EAppListServPreferredBufSize:
		aMessage.Complete(PreferredBufSize());
		break;
	case EAppListServRecognizeFiles:
		RecognizeFilesL(aMessage);
		break;
	case EAppListServTransferRecognitionResult:
		TransferRecognitionResultL(aMessage);
		break;
	case EAppListServRecognizeFilesAsync:
		RecognizeFilesAsyncL(aMessage);
		completeMessage=EFalse;
		break;
	case ECancelRecognizeFiles:
		CancelRecognizeFiles();
		break;
	case EAppListServRuleBasedLaunching:
		AquirePermissionToLaunchAppL(aMessage);
		break;
	case ENotifyOnDataMappingChange:
		NotifyOnDataMappingChange(aMessage);
		completeMessage=EFalse;
		break;
	case ECancelNotifyOnDataMappingChange:
		CancelNotifyOnDataMappingChange();
		break;
	case EDebugHeapMark:
	#ifdef _DEBUG
		__UHEAP_MARK;
	#endif
		break;
	case EDebugHeapMarkEnd:
	#ifdef _DEBUG
		__UHEAP_MARKENDC(aMessage.Int0());
	#endif
		break;
	case EDebugHeapFailNext:
	#ifdef _DEBUG
		__UHEAP_FAILNEXT(aMessage.Int0());
	#endif
		break;
	case EDebugFlushRecognitionCache:
	#ifdef _DEBUG
		iServ.FlushRecognitionCache();
	#endif
		break;
	case EDebugSetLoadRecognizersOnDemand:
	#ifdef _DEBUG
		iServ.SetLoadRecognizersOnDemandL(aMessage.Int0());
	#endif
		break;
	case EDebugPerformOutstandingRecognizerUnloading:
	#ifdef _DEBUG
		iServ.PerformOutstandingRecognizerUnloading();
		REComSession::FinalClose();
	#endif
		break;
	default:
		iAppListSession->DoServiceL(aMessage);
#ifdef APPARC_SHOW_TRACE   
		RDebug::Print( _L("[Apparc] CApaAppListServSession::ServiceL(0x%08x) - END - op code: %04d, completeMessage: %d"), this, aMessage.Function(), completeMessage );
#endif		
		return;
		}
	
#ifdef APPARC_SHOW_TRACE 	
    RDebug::Print( _L("[Apparc] CApaAppListServSession::ServiceL(0x%08x) - END - op code: %04d, completeMessage: %d"), this, aMessage.Function(), completeMessage );
#endif	
	if (completeMessage && !aMessage.IsNull())
		aMessage.Complete(KErrNone);
	}


void CApaAppArcServSession::NotifyOnDataMappingChange(const RMessage2& aMessage)
	{ 
	if (!iMessage_NotifyOnDataMappingChange.IsNull())
		aMessage.Panic(KApaPanicCli,ENotifyOnDataMappingChangeRequestOutstanding);
	else
		iMessage_NotifyOnDataMappingChange=aMessage;
	}

void CApaAppArcServSession::CancelNotifyOnDataMappingChange()
	{
	if (!iMessage_NotifyOnDataMappingChange.IsNull())
		iMessage_NotifyOnDataMappingChange.Complete(KErrCancel);
	} //lint !e1762 Suppress member function could be made const


TInt CApaAppArcServSession::PreferredBufSize() const
	{
	TInt preferredBufferSize = 0;
	TRAPD(err, preferredBufferSize = iServ.DataRecognizerPreferredBufSizeL());
	return (err==KErrNone) ? Min(iMaxBufSize, preferredBufferSize) : iMaxBufSize;
	}

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
void CApaAppArcServSession::RegisterNonNativeApplicationTypeL(const RMessage2& aMessage)
	{
	const TUid applicationType(TUid::Uid(aMessage.Int0()));
	HBufC* const nativeExecutable=HBufC::NewLC(User::LeaveIfError(aMessage.GetDesLength(1)));
	{TPtr nativeExecutable_asWritable(nativeExecutable->Des());
	aMessage.ReadL(1, nativeExecutable_asWritable);}
	iServ.RegisterNonNativeApplicationTypeL(applicationType, *nativeExecutable);
	CleanupStack::PopAndDestroy(nativeExecutable);
	}

void CApaAppArcServSession::DeregisterNonNativeApplicationTypeL(const RMessage2& aMessage)
	{
	const TUid applicationType(TUid::Uid(aMessage.Int0()));
	iServ.DeregisterNonNativeApplicationTypeL(applicationType);
	}
#endif

void CApaAppListServSession::GetAppTypeL(const RMessage2& aMessage)
	{
    const TUid KTypeIDToNonNativeUidMapping[2][2]={ {TUid::Uid(0xB031C52A), TUid::Uid(0x10210E26)},  //Java
                                                    {TUid::Uid(0x7BDB6DA1), TUid::Uid(0x10282821)}}; //Widget 
   
	TInt uid = aMessage.Int0();
	CApaAppData* appData = iAppList.AppDataByUid(TUid::Uid(uid));
	if (!appData)
	    {
		aMessage.Complete(KErrNotFound);
	    }
	else
		{
		TUid typeId(appData->NonNativeApplicationType());
		
		//Check if non-native type to non-native UID mapping available. Otherwise
		//return whatever returned by NonNativeApplicationType.
		TUid nonNativeUid=typeId;
		TInt numMappings= (sizeof(KTypeIDToNonNativeUidMapping)/ (2*sizeof(TUid)));
		
		for(TInt index=0; index<numMappings; index++)
		    {
            if(typeId == KTypeIDToNonNativeUidMapping[index][0])
                {
                nonNativeUid = KTypeIDToNonNativeUidMapping[index][1];
                break;
                }
		    }
		
		TPckgBuf<TUid> nonNativeUidBuf(nonNativeUid);
		aMessage.WriteL(1,nonNativeUidBuf);
		aMessage.Complete(KErrNone);
		}
	}

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK  
void CleanupAndDestroyAppInfoArray(TAny* aRPArray)
    {
    RPointerArray<Usif::CApplicationRegistrationData>* rpArray = (static_cast<RPointerArray<Usif::CApplicationRegistrationData>*>(aRPArray));
    rpArray->ResetAndDestroy();
    rpArray->Close();
    }
#endif

void CApaAppListServSession::ForceRegistrationL(const RMessage2& aMessage)
	{
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK    
	TInt bufferSize = aMessage.GetDesLength(0);
	User::LeaveIfError(bufferSize);
	HBufC8* const buffer=HBufC8::NewLC(bufferSize);
	TPtr8 buffer_asWritable(buffer->Des());
	aMessage.ReadL(0,buffer_asWritable);
		
	RDesReadStream readStream(*buffer);
	CleanupClosePushL(readStream);
	const TUint count=readStream.ReadUint32L();
	for (TUint i = 0; i < count; ++i)
		{
		TUint length = readStream.ReadUint32L();
		HBufC* regFile = HBufC::NewLC(length);
  		TPtr ptr(regFile->Des());
  		readStream.ReadL(ptr, length);
		iAppList.AddForcedRegistrationL(*regFile);
		CleanupStack::PopAndDestroy(regFile);
		}
	
	CleanupStack::PopAndDestroy(&readStream);
	CleanupStack::PopAndDestroy(buffer);
	
	// Trigger a rescan, when rescan completes it will complete iNotifyOnScanCompleteMsg
	iNotifyOnScanCompleteMsg=aMessage;
	iAppArcSrv.UpdateAppsByForceRegistration();
#else
	const TUid KUidSisLaunchServer={0x1020473f};
	
	if(aMessage.SecureId().iId != KUidSisLaunchServer.iUid)
	    User::Leave(KErrNotSupported);
	
	//Get the size of the updated apps info buffer
    TInt bufferSize = aMessage.GetDesLength(0);

    //Allocate the buffer of bufferSize and read.
    HBufC8* const buffer=HBufC8::NewLC(bufferSize);
    TPtr8 buffer_asWritable(buffer->Des());
    aMessage.ReadL(0,buffer_asWritable);
        
    RDesReadStream readStream(*buffer);
    CleanupClosePushL(readStream);
    
    //Read the number of application information available in the buffer.
    const TUint count=readStream.ReadUint32L();

    RPointerArray<Usif::CApplicationRegistrationData> appsInfo;
    TCleanupItem cleanup(CleanupAndDestroyAppInfoArray, &appsInfo); 
    CleanupStack::PushL(cleanup); 
    
    //Read one applciation information at a time and create list of application information.
    for(TUint index=0; index<count; index++)
        {
        Usif::CApplicationRegistrationData* appData= Usif::CApplicationRegistrationData::NewL();
        CleanupStack::PushL(appData);
        readStream>>*appData;
        appsInfo.AppendL(appData);
        CleanupStack::Pop(appData);
        }
    CleanupStack::Pop(); //Remove cleanup
    CleanupStack::PopAndDestroy(2, buffer); //delete readStream, buffer	

    iAppList.UpdateApplistByForceRegAppsL(appsInfo);
#endif	
	}

void CApaAppArcServSession::AppForDocumentPassedByFileHandleL(const RMessage2& aMessage, const TUid* aServiceUid)
	{
#if defined(__PROFILE)
	TProfile profile;
	RDebug::ProfileReset(5,1);
	RDebug::ProfileStart(5);
#endif
	RFile file;
	CleanupClosePushL(file);
	User::LeaveIfError(file.AdoptFromClient(aMessage, 2, 3));
	SReturnData_AppForDocument returnData;
	returnData.iDataType=iServ.RecognizeDataL(file, PreferredBufSize()).iDataType;
	returnData.iUid=(returnData.iDataType!=TDataType())? AppForDataTypeL(returnData.iDataType, aServiceUid): TUid::Null();
	CleanupStack::PopAndDestroy(&file);
	aMessage.WriteL(0,TPckgC<SReturnData_AppForDocument>(returnData));
#if defined(__PROFILE)
	RDebug::ProfileEnd(5);
	RDebug::ProfileResult(&profile,5,1);
	RDebug::Print(KProfileAppForDocumentPassedByFileHandleL,profile.iTime/1000000,profile.iTime%1000000);
	RDebug::ProfileStart(5);
#endif
	}

/**
Call the recognizer framework to find the MIME-type of the data buffer,
then find and return the UID of the "best" application to handle that data.
*/
void CApaAppArcServSession::AppForDocumentL(const RMessage2& aMessage, const TUid* aServiceUid)
	{
#if defined(__PROFILE)
	TProfile profile;
	RDebug::ProfileReset(5,1);
	RDebug::ProfileStart(5);
#endif

	// Get the document file name from the IPC message
	HBufC* const docFileName = HBufC::NewLC(User::LeaveIfError(aMessage.GetDesLength(2)));
	{TPtr docFileName_asWritable(docFileName->Des());
	aMessage.ReadL(2, docFileName_asWritable);}
	
	// Get the data read from the document that will be used for recognition from the IPC message
	HBufC8* const buffer = HBufC8::NewLC(User::LeaveIfError(aMessage.GetDesLength(3)));
	{TPtr8 buffer_asWritable(buffer->Des());
	aMessage.ReadL(3,buffer_asWritable);}
	
	// Call the recognizer framework to get the MIME-type
	SReturnData_AppForDocument returnData;
	returnData.iDataType = iServ.RecognizeDataL(*docFileName, *buffer).iDataType;
	// If a MIME-type was found, get the UID of the "best" app registered for that type
	returnData.iUid = (returnData.iDataType!=TDataType() ? AppForDataTypeL(returnData.iDataType, aServiceUid) : TUid::Null());
	
#if defined(__PROFILE)
	RDebug::ProfileEnd(5);
	RDebug::ProfileResult(&profile,5,1);
	RDebug::Print(KProfileAppForDocumentL,profile.iTime/1000000,profile.iTime%1000000);
	RDebug::ProfileStart(5);
#endif
	
	// Send back the answer
	CleanupStack::PopAndDestroy(2, docFileName);
	aMessage.WriteL(0,TPckgC<SReturnData_AppForDocument>(returnData));
	}

void CApaAppArcServSession::GetConfidenceL(const RMessage2& aMessage)
// void GetAcceptedConfidence(TInt& aConfidence);
	{
	aMessage.WriteL(0,TPckgBuf<TInt>(iServ.MimeTypeRecognizer()->AcceptedConfidence()));
	}

void CApaAppArcServSession::SetConfidence(const RMessage2& aMessage)
// SetAcceptedConfidence(TInt aConfidence);
	{
	__ASSERT_DEBUG(iServ.MimeTypeRecognizer(), Panic(EPanicNullPointer));
	iServ.MimeTypeRecognizer()->SetAcceptedConfidence(aMessage.Int0());
	}

void CApaAppArcServSession::GetBufSize(const RMessage2& aMessage)
// GetMaxDataBufSize(TInt& aBufSize);
	{
	aMessage.Complete(iMaxBufSize);
	}

void CApaAppArcServSession::SetBufSize(const RMessage2& aMessage)
// SetMaxDataBufSize(TInt aBufSize);
	{
	iMaxBufSize=aMessage.Int0();
	}

void CApaAppArcServSession::GetDataTypesCountL(const RMessage2& aMessage)
	{
	delete iBuffer;
	iBuffer = NULL;
	CDataTypeArray* const dataTypes = new(ELeave) CDataTypeArray(5);
	CleanupStack::PushL(dataTypes);
	iServ.DataTypeL(*dataTypes);
	
	TInt completionCode=0; // not KErrNone, as completion code of zero tells the client that zero data types were found
	if (dataTypes->Count()>0)
		{
		CBufBase* const buffer = CBufFlat::NewL(sizeof(TDataType)); 
		CleanupStack::PushL(buffer); 
		RBufWriteStream writeStream(*buffer); 
		writeStream << *dataTypes; 
		CleanupStack::Pop(buffer); 
		iBuffer = buffer;
		completionCode = iBuffer->Ptr(0).Size(); // number of bytes in buffer (not number of data types)
		}
	
	CleanupStack::PopAndDestroy(dataTypes);
	aMessage.Complete(completionCode);
	}

void CApaAppArcServSession::GetDataTypesL(const RMessage2& aMessage)
// GetSupportedDataTypes(CDataTypeArray& aDataTypes);
	{
	if(!iBuffer)
		aMessage.Panic(KApaPanicCli,ENoSupportedDataTypes);
	else
		{
		aMessage.WriteL(0,iBuffer->Ptr(0));
		delete iBuffer;
		iBuffer=NULL;
		}
	}

CApaAppData& CApaAppListServSession::FindAppInListL(TUid aUid)
	{
	TApaAppEntry dummy;
	CApaAppData* app = NULL;
	if (!FindAppInList(app, dummy, aUid))
		User::Leave(KErrNotFound);

    return *app;
    }

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
/**
locate app in list, return EFalse if it isn't present
search is regardless of screen mode.
@internalComponent
*/
TBool CApaAppListServSession::FindAppInList(CApaAppData*& aApp, TApaAppEntry& aEntry, TUid aAppUid)
    {
    // Look for the app with aAppUid in the applist we keep
    const CApaAppList& list = iAppList;
    aApp = list.AppDataByUid(aAppUid);
    if (aApp)
        aEntry = aApp->AppEntry();
    
    // If the app list is currently in flux, try to nail down the app by looking for it specifically
    const TBool appPendingOnLangChange = (aApp && list.IsLanguageChangePending() && aApp->IsPending());

    if ((!aApp || appPendingOnLangChange) && !list.IsIdleUpdateComplete())
        {
        // 1. App wasn't found, but an app scan is currently in progress,
        // so try to find and add the specific app we're looking for to the list
        
        // 2. On language change event, current app scan could not yet update the found app, 
        // so try to update the specific app we're looking for, in the list.
        if(aAppUid != KNullUid)
            {
            CApaAppData* app = NULL;
            TRAPD(err, app = FindSpecificAppL(aAppUid));
            if (!err && app)
                {
                // app has been found and added to the app list
                aApp = app;
                aEntry = aApp->AppEntry();
                }
            }
        }

    return (aApp != NULL);
    }

#else
/**
locate app in list, return EFalse if it isn't present
search is regardless of screen mode.
@internalComponent
*/
TBool CApaAppListServSession::FindAppInList(CApaAppData*& aApp, TApaAppEntry& aEntry, TUid aAppUid)
    {
    // Look for the application with aAppUid in the applist
    const CApaAppList& list = iAppList;
    aApp = list.AppDataByUid(aAppUid);
   
    TBool appListChanging= (list.IsLanguageChangePending()||!list.IsIdleUpdateComplete());
   //If the application is not in the applist and applist is still getting updated then find the 
   //requested application specifically and add to applist.
    TInt err=KErrNone;
    if( (!aApp && appListChanging) || (aApp && aApp->IsLangChangePending()))
        TRAP(err, aApp=FindSpecificAppL(aAppUid));

    if (!err && aApp)
        aEntry = aApp->AppEntry(); 
    
    return (aApp != NULL);
    }

#endif

void CApaAppListServSession::SendArrayL(const MArrayItemWriter& aArrayItemWriter,const RMessage2& aMessage) const
	{
	const TInt sizeOfBuffer=aMessage.Int1();
	const TInt arrayItemCount=aArrayItemWriter.ArrayItemCount();
	const TInt sizeRequired=sizeof(TInt)+(arrayItemCount*aArrayItemWriter.ArrayItemSize());
	ASSERT(sizeRequired > 0);
	if (sizeRequired>sizeOfBuffer)
		User::Leave(sizeRequired); // causes aMessage to complete with sizeRequired

	CBufFlat* const buf=CBufFlat::NewL(sizeRequired);
	CleanupStack::PushL(buf);
	buf->ExpandL(0,sizeRequired);
	RBufWriteStream writeStream;
	writeStream.Open(*buf);
	writeStream.WriteUint32L(arrayItemCount);
	for (TInt i=0; i<arrayItemCount; ++i)
		aArrayItemWriter.WriteArrayItemL(writeStream,i);

	writeStream.CommitL();
	aMessage.WriteL(2,buf->Ptr(0));
	CleanupStack::PopAndDestroy(buf);
	}

void CApaAppListServSession::AppIconSizesL(const RMessage2& aMessage)
	{
	const TUid uid=TUid::Uid(aMessage.Int0());
	const CApaAppData& app = FindAppInListL(uid);
	if (app.NonMbmIconFile())
		User::Leave(KErrNotSupported);
		
	CArrayFixFlat<TSize>* array = app.IconSizesL();
	CleanupStack::PushL(array);
	TSizeArrayItemWriter arrayItemWriter(*array);		
	SendArrayL(arrayItemWriter, aMessage);
	CleanupStack::PopAndDestroy(array);
	}

void CApaAppListServSession::AppViewsL(const RMessage2& aMessage)
	{
	const TUid uid=TUid::Uid(aMessage.Int0());
	const CApaAppData& app = FindAppInListL(uid);
	TViewDataArrayItemWriter arrayItemWriter(*app.Views());
	SendArrayL(arrayItemWriter,aMessage);
	}

void CApaAppListServSession::AppFileOwnershipInfoL(const RMessage2& aMessage)
	{
	const TUid uid=TUid::Uid(aMessage.Int0());
	const CApaAppData& app = FindAppInListL(uid);
	TDesCArrayItemWriter arrayItemWriter(*app.OwnedFiles());
	SendArrayL(arrayItemWriter,aMessage);
	}

void CApaAppListServSession::NumberOfOwnDefinedIconsL(const RMessage2& aMessage)
	{
	const TUid uid = TUid::Uid(aMessage.Int0());
	const CApaAppData& app = FindAppInListL(uid);
	if (app.NonMbmIconFile())
		User::Leave(KErrNotSupported);

	TInt count, defaultIconsUsed;
	app.GetIconInfo(count, defaultIconsUsed);
	if (defaultIconsUsed)
		count=0;

	TPckgC<TInt> pckg(count); 
	aMessage.Write(1,pckg);
	}
	
void CApaAppListServSession::ApplicationLanguageL(const RMessage2& aMessage)
	{
	const TUid appUid = TUid::Uid(aMessage.Int0());
	const CApaAppData& appData = FindAppInListL(appUid);

	const TLanguage appLanguage = appData.ApplicationLanguage();
	TPckgC<TLanguage> pckg(appLanguage); 
	aMessage.Write(1,pckg);
	}
	

void CApaAppListServSession::IconForViewBySizeL(const RMessage2& aMessage)
	// Passes back handles to the icon and mask bitmaps for bitmap sharing 
	{
	TApaAppViewIconSizeData appViewIconSizeData;
	{TPckg<TApaAppViewIconSizeData> appViewIconSizeData_asDescriptor(appViewIconSizeData);
	aMessage.ReadL(0,appViewIconSizeData_asDescriptor);}
	const CApaAppData& app = FindAppInListL(appViewIconSizeData.iAppUid);
	
	ASSERT(app.Views());
	const CArrayPtr<CApaAppViewData>& viewDataArray = *app.Views();
	CApaMaskedBitmap* icon = NULL;
	const TInt count = viewDataArray.Count();
	for (TInt ii=0; ii<count; ii++)
		{
		const CApaAppViewData& appViewData = *viewDataArray[ii];
		if (appViewData.Uid() == appViewIconSizeData.iViewUid)
			{
			if (appViewData.NonMbmIconFile())
				User::Leave(KErrNotSupported);

			icon=appViewData.Icon(appViewIconSizeData.iSize);
			break;
			}
		}
		
	if (!icon)
		User::Leave(KErrNotFound);

	SReturnData_ViewIconByUidAndSize returnData;
	returnData.iIcon = icon->Handle();
	returnData.iIconMask = icon->Mask()->Handle();
	aMessage.WriteL(1,TPckgC<SReturnData_ViewIconByUidAndSize>(returnData));
	}

void CApaAppListServSession::IconForAppBySizeL(const RMessage2& aMessage)
	{
	const TUid uid=TUid::Uid(aMessage.Int0());
	const TSize size(aMessage.Int1(),aMessage.Int2());
	const CApaAppData& app=FindAppInListL(uid);

	if (app.NonMbmIconFile())
		User::Leave(KErrNotSupported);

	CApaMaskedBitmap* const icon=app.Icon(size);
	if (!icon)
		User::Leave(KErrNotFound);

	SReturnData_AppIconByUidAndSize returnData;
	returnData.iIcon=icon->Handle();
	returnData.iIconMask=icon->Mask()->Handle();
	aMessage.WriteL(3,TPckgC<SReturnData_AppIconByUidAndSize>(returnData));
	}

void CApaAppListServSession::IconFileHandleForAppL(const RMessage2& aMessage)
	{
	const TUid uid=TUid::Uid(aMessage.Int0());
	TApaAppEntry entry;
	CApaAppData* app=NULL;
	if (!FindAppInList(app,entry,uid))
		User::Leave(KErrNotFound);

	TPtrC iconFileName = app->IconFileName();
	if (iconFileName.Length()==0)
		User::Leave(KErrNotFound);

	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	User::LeaveIfError(fs.ShareProtected());

	RFile file;
	CleanupClosePushL(file);
	User::LeaveIfError(file.Open(fs, iconFileName, EFileShareReadersOnly));
	User::LeaveIfError(file.TransferToClient(aMessage, 1));
	CleanupStack::PopAndDestroy(2, &fs); //file and fs
	}

void CApaAppListServSession::IconForAppL(const RMessage2& aMessage)
// from GetAppIcon(TUid aAppUid, TInt aSideInPixels, CApaMaskedBitmap& aAppBitmap);
// BUT!  It's interface is uid, side, icon handle, mask handle for bitmap sharing 
// and avoiding IPC overhead
	{
	const TUid uid = TUid::Uid(aMessage.Int0());
	const CApaAppData& app = FindAppInListL(uid);

	if (app.NonMbmIconFile())
		User::Leave(KErrNotSupported);
	
	const TInt side = aMessage.Int1();
	CApaMaskedBitmap* const icon = app.Icon(side);
	if (!icon)
		User::Leave(KErrNotFound);

	SReturnData_AppIconByUid returnData;
	returnData.iIcon = icon->Handle();
	returnData.iIconMask = icon->Mask()->Handle();
	aMessage.WriteL(2,TPckgC<SReturnData_AppIconByUid>(returnData));
	}

void CApaAppArcServSession::AppForDataTypeL(const RMessage2& aMessage)
// from AppForDataType(const TDataType& aDataType, TUid& aAppUid);
	{
	if (sizeof(TDataType) != aMessage.GetDesLengthL(0))
		{
		//Leave with KErrArgument if client passes other than TDataType
		User::Leave(KErrArgument);
		}
	TDataType dataType;
	{TPckg<TDataType> dataType_asDescriptor(dataType);
	aMessage.ReadL(0,dataType_asDescriptor);}
	TPckgBuf<TUid> uid_asDescriptor(AppForDataTypeL(dataType, NULL));
	aMessage.WriteL(1,uid_asDescriptor);
	aMessage.Complete(KErrNone);
	}

void CApaAppArcServSession::AppForDataTypeAndServiceL(const RMessage2& aMessage)
	{
	TDataType dataType;
	{TPckg<TDataType> dataType_asDescriptor(dataType);
	aMessage.ReadL(0,dataType_asDescriptor);}
	const TUid serviceUid=TUid::Uid(aMessage.Int1());
	TPckgBuf<TUid> uid(AppForDataTypeL(dataType, &serviceUid));
	aMessage.WriteL(2,uid);
	aMessage.Complete(KErrNone);
	}

TUid CApaAppArcServSession::AppForDataTypeL(const TDataType& aDataType, const TUid* aServiceUid)
	{
	// It is possible to register apps as datatype handlers with system priority,
	// which means that they are not overridable by user mappings.
	// So search the list of apps for a datahandler and get the associated
	// priority
	 
	TInt priority = KDataTypePriorityNormal;
	TUid uid = iAppListSession->AppList().PreferredDataHandlerL(aDataType, aServiceUid, priority);
	if (priority == KDataTypePrioritySystem)
		return uid;	// We have found a handler with system priority
		
	// No handler with system priority so see if there is a user mapping
	TUid userUid = KNullUid;
	if (aServiceUid)
		iServ.GetAppForMimeType(aDataType,*aServiceUid,userUid);
	else
		iServ.GetAppForMimeType(aDataType,userUid);

	TApaAppEntry entry;
	const CApaAppData* app = NULL;
	if (userUid.iUid && iAppListSession->FindAppInList(app,entry,userUid))
		return userUid;	// The user mapping is valid
		
	// A user mapping was not found or is invalid so try to use
	// the uid returned by PreferredDataHandlerL.
	if (!uid.iUid && aDataType.IsNative())
		uid = aDataType.Uid();

	return uid;
	}

void CApaAppArcServSession::InsertDataMappingL(const RMessage2& aMessage)
	{
	TPckgBuf<TDataType> dataType;
	aMessage.ReadL(0, dataType);
	TDataTypePriority priority = aMessage.Int1();
	const TUid appUid = { aMessage.Int2() };

	if(priority>KDataTypeUnTrustedPriorityThreshold || priority == KDataTypePrioritySystem ) 
	   {   
	   CApaAppData* appData = iAppListSession->AppList().AppDataByUid(appUid);
	   if( appData )	
		  {
          TBool hasWriteDeviceDataCap( EFalse );
          TBool isSidTrusted( EFalse );
          
          CApaSecurityUtils::CheckAppSecurity( appData->AppEntry().iFullName, 
                                               hasWriteDeviceDataCap,
                                               isSidTrusted);
          
          if (priority == KDataTypePrioritySystem )
              {
              // Check if the app has capability WriteDeviceData
              if ( !hasWriteDeviceDataCap )
                  {
                  priority = KDataTypePriorityNormal;
                  }
              }
          else
              {
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK          
              Usif::RSoftwareComponentRegistry scrCon;
              User::LeaveIfError(scrCon.Connect());
              CleanupClosePushL(scrCon);
              TApaAppEntry appEntry=appData->AppEntry();
              
			  //If component id of an application is non-zero then it is installed by installer
			  //after phone marketed.
              TBool isInstalledApp=(scrCon.GetComponentIdForAppL(appEntry.iUidType[2])!=0);

              //data priority for UnTrusted apps would be capped if it is greater than the threshold priority i.e, KMaxTInt16.
              if (!isSidTrusted && isInstalledApp) 
                  {
                  priority = KDataTypeUnTrustedPriorityThreshold; 
                  }
              CleanupStack::PopAndDestroy(); //scrCon
#else
              TPtrC registrationFilePath = appData->RegistrationFileName();
              TInt match = registrationFilePath.MatchF (
                                          KLitPathForUntrustedRegistrationResourceFiles );
              //Check if registration file is in path for untrusted apps 
              //and its SID is untrusted
              if (match != KErrNotFound && !isSidTrusted )
                  {
                  // "cap" the priority if UnTrusted apps claim for priority higher 
                  // than UnTrusted apps Threshold priority
                  priority = KDataTypeUnTrustedPriorityThreshold;
                  }
#endif
              }
          }
	   else
		  {
              //if the application is not present in the applist 
              //then the priority will be reduced to Threshold
              priority = KDataTypeUnTrustedPriorityThreshold;
		  }
	   }

	const TUid serviceUid = { aMessage.Int3() };
	if(aMessage.Function() == EAppListInsertDataMappingIfHigher)
		{
		const TBool response = iServ.InsertAndStoreIfHigherL(dataType(), priority, appUid);
		aMessage.WriteL(3, TPckgC<TBool>(response));
		}
	else
		iServ.InsertAndStoreDataMappingL(dataType(), priority, appUid, serviceUid);
	}

void CApaAppArcServSession::DeleteDataMappingL(const RMessage2& aMessage)
	{
	TPckgBuf<TDataType> dataType;
	aMessage.ReadL(0, dataType);
	const TUid serviceUid = { aMessage.Int1() };
	TUid uid;
	iServ.GetAppForMimeType(dataType(),serviceUid,uid);
	if (uid != KNullUid)
		{
		// TypeStore doesn't support deletion of an inexistent mapping
		iServ.DeleteAndStoreDataMappingL(dataType(), serviceUid);
		aMessage.Complete(KErrNone);
		}
	else
		aMessage.Complete(KErrNotFound);
	}
	
void CApaAppArcServSession::GetAppByDataTypeL(const RMessage2& aMessage) const
	{
	TPckgBuf<TDataType> dataType;
	aMessage.ReadL(0,dataType);
	const TUid serviceUid = { aMessage.Int1() };
	TUid uid;
	iServ.GetAppForMimeType(dataType(),serviceUid,uid);
	TPckgC<TUid> uidpckg(uid);
	aMessage.WriteL(2,uidpckg);
	aMessage.Complete(KErrNone);
	}

void CApaAppArcServSession::GetExecutableNameGivenDocumentL(const RMessage2& aMessage)
	{
	HBufC* const name=HBufC::NewLC(User::LeaveIfError(aMessage.GetDesLength(2)));
	{TPtr name_asWritable(name->Des());
	aMessage.ReadL(2, name_asWritable);}
	HBufC8* const buffer=HBufC8::NewLC(User::LeaveIfError(aMessage.GetDesLength(3)));
	{TPtr8 buffer_asWritable(buffer->Des());
	aMessage.ReadL(3, buffer_asWritable);}
	const TDataType dataType(iServ.RecognizeDataL(*name, *buffer).iDataType);
	CleanupStack::PopAndDestroy(2, name);

	const TUid appUid(AppForDataTypeL(dataType, NULL));
	GetExecutableNameL(aMessage, appUid);
	}

void CApaAppArcServSession::GetExecutableNameGivenDocumentPassedByFileHandleL(const RMessage2& aMessage)
	{
	RFile file;
	CleanupClosePushL(file);
	User::LeaveIfError(file.AdoptFromClient(aMessage, 2, 3));
	const TDataType dataType(iServ.RecognizeDataL(file, PreferredBufSize()).iDataType);
	CleanupStack::PopAndDestroy(&file);

	const TUid appUid(AppForDataTypeL(dataType, NULL));
	GetExecutableNameL(aMessage, appUid);
	}

void CApaAppArcServSession::GetExecutableNameGivenDataTypeL(const RMessage2& aMessage)
	{
	if (sizeof(TDataType) != aMessage.GetDesLengthL(2))
		{
		//Leave with KErrArgument if client passes other than TDataType
		User::Leave(KErrArgument);
		}
	TDataType dataType;
	{TPckg<TDataType> dataType_asDescriptor(dataType);
	aMessage.ReadL(2, dataType_asDescriptor);}
	const TUid appUid(AppForDataTypeL(dataType, NULL));
	GetExecutableNameL(aMessage, appUid);
	}

void CApaAppArcServSession::GetExecutableNameGivenAppUidL(const RMessage2& aMessage)
	{
	const TUid appUid(TUid::Uid(aMessage.Int2()));
	GetExecutableNameL(aMessage, appUid);
	}

void CApaAppArcServSession::GetExecutableNameL(const RMessage2& aMessage, TUid aAppUid)
	{
	const CApaAppData* appData=NULL;
	TApaAppEntry entry;
	if (!iAppListSession->FindAppInList(appData, entry, aAppUid))
		User::Leave(iAppListSession->AppList().IsFirstScanComplete() ? KErrNotFound : RApaLsSession::EAppListInvalid);

	const TDesC& executableName(entry.iFullName);
	if (!executableName.Length())
		User::Leave(KErrNotFound);

	aMessage.WriteL(1, executableName); // the "logical" executable name - for non-native applications this is the name of the MIDlet, Python script, etc
	WriteNativeExecutableIfNonNativeAndPrepareForClientRetrievalOfOpaqueDataL(aMessage, 0, *appData);
	}

void CApaAppArcServSession::GetNativeExecutableNameIfNonNativeL(const RMessage2& aMessage)
	{
	RBuf logicalExecutableName;
	logicalExecutableName.CreateL(User::LeaveIfError(aMessage.GetDesLength(1)));
	CleanupClosePushL(logicalExecutableName);
	aMessage.ReadL(1, logicalExecutableName);
	CApaAppData* const appData=iAppListSession->AppList().AppDataByFileName(logicalExecutableName);
	if (appData!=NULL)
		WriteNativeExecutableIfNonNativeAndPrepareForClientRetrievalOfOpaqueDataL(aMessage, 0, *appData);

	CleanupStack::PopAndDestroy(&logicalExecutableName);
	}

void CApaAppArcServSession::WriteNativeExecutableIfNonNativeAndPrepareForClientRetrievalOfOpaqueDataL(const RMessage2& aMessage, TInt aMessageSlotForNativeExecutable, const CApaAppData& aAppData)
	{
	HBufC8* opaqueData=NULL;
	const TPtrC8 opaqueData_asTPtrC8(aAppData.OpaqueData());
	const TInt lengthOfOpaqueData(opaqueData_asTPtrC8.Length());
	if (lengthOfOpaqueData>0)
		opaqueData=opaqueData_asTPtrC8.AllocLC();

	const TUid nonNativeApplicationType(aAppData.NonNativeApplicationType());
	if (nonNativeApplicationType!=TUid::Null())
		aMessage.WriteL(aMessageSlotForNativeExecutable, iServ.NativeExecutableL(nonNativeApplicationType));

	delete iOpaqueData_pendingDispatchToClient; // only done when the potentially leaving stuff has all succeeded
	iOpaqueData_pendingDispatchToClient=opaqueData; // want to do this, even if opaqueData is NULL
	if (opaqueData)
		{
		CleanupStack::Pop(opaqueData);
		aMessage.Complete(lengthOfOpaqueData);
		}
	}

void CApaAppArcServSession::GetOpaqueDataL(const RMessage2& aMessage)
	{
	if (iOpaqueData_pendingDispatchToClient==NULL)
		User::Leave(KErrGeneral); // the protocol was broken: EAppListServGetOpaqueData can only be called immediately after one of the EAppListServGetExecutableNameGivenXxx or EAppListServGetNativeExecutableNameGivenXxx opcodes - see the client-side implementation of this protocol in RApaLsSession::GetOpaqueData (and the places that call it)

	aMessage.WriteL(0, *iOpaqueData_pendingDispatchToClient);
	delete iOpaqueData_pendingDispatchToClient;
	iOpaqueData_pendingDispatchToClient=NULL;
	}

void CApaAppListServSession::GetAppInfoL(TUid aUid, TApaAppInfo& aInfo)
	{
	const CApaAppData* app = NULL;
	TApaAppEntry entry;
	if (!FindAppInList(app, entry, aUid))
		User::Leave(KErrNotFound);

	aInfo.iUid = entry.iUidType[2];
	aInfo.iFullName = entry.iFullName;
	aInfo.iCaption = app->Caption();
	aInfo.iShortCaption = app->ShortCaption();
	}
	
TPtrC8 CApaAppArcServSession::DoRecognizeUnpackLC(HBufC*& aName, RChunk& aLocalChunk, const RMessage2& aMessage)
    {
    ASSERT(aName==NULL);
    
    aName=HBufC::NewLC(User::LeaveIfError(aMessage.GetDesLength(1)));
    TPtr name(aName->Des());
    aMessage.ReadL(1, name);
    
    HBufC8* buffer=NULL;
    TInt error=KErrNone;
    TInt bufferSize= aMessage.GetDesLength(2);
    User::LeaveIfError(bufferSize);
    //Allocate memory in apparc's process heap.
    TRAP(error, buffer=HBufC8::NewL(bufferSize));

    if(error==KErrNone)
        {
        CleanupStack::PushL(buffer);
        TPtr8 bufPtr(buffer->Des());
        aMessage.ReadL(2, bufPtr);   
        return bufPtr;
        }
    else if(error==KErrNoMemory)
        {
        //If memory is not available in apparc's process heap, then allocate in kernel heap
        User::LeaveIfError(aLocalChunk.CreateLocal(bufferSize,bufferSize));
        CleanupClosePushL(aLocalChunk);
        TPtr8 bufPtr(aLocalChunk.Base(), bufferSize);
        aMessage.ReadL(2, bufPtr); 
        return bufPtr;        
        }
    else
        {
        User::Leave(error);
        }
    
    TPtrC8 bufPtr(NULL,0); //Never executed. To make compiler happy
    return bufPtr;
    }

void CApaAppArcServSession::RecognizeDataL(const RMessage2& aMessage)
// Recognize the data type of an object
    {
    HBufC* name=NULL;
    RChunk localChunk;
    
    TPtrC8 bufPtr=DoRecognizeUnpackLC(name, localChunk, aMessage);

    const TDataRecognitionResult result = iServ.RecognizeDataL(*name, bufPtr);

    CleanupStack::PopAndDestroy(2); // name & buffer or localChunk
    aMessage.WriteL(0,TPckgC<TDataRecognitionResult>(result));
    }


void CApaAppArcServSession::RecognizeFilesL(const RMessage2& aMessage)
	{
	// if there is an outstanding async. request, we even don't allow 
	// a synchronous request at the same time (due to the two required
	// server messages)
	if (iAsyncRecognitionActive)
		User::Leave(KErrInUse);

	_LIT8(KAllDataTypes,"*");

	// retrieve pathname
	HBufC* const path=HBufC::NewLC(User::LeaveIfError(aMessage.GetDesLength(0)));
	TPtr pathPtr(path->Des());
	aMessage.ReadL(0,pathPtr);

	// retrieve data type filter
	HBufC8* const dataType = HBufC8::NewLC(User::LeaveIfError(aMessage.GetDesLength(2)));
	TPtr8 dataTypePtr(dataType->Des());
	aMessage.ReadL(2,dataTypePtr);

	delete iRecognitionResult;
	iRecognitionResult = NULL;

	if(dataType->Compare(KAllDataTypes) == 0)
		{
		iRecognitionResult = new (ELeave) CDirectoryRecognitionResult(path, NULL);
		CleanupStack::PopAndDestroy(dataType);
		}
	else
		{
		iRecognitionResult = new (ELeave) CDirectoryRecognitionResult(path,dataType);	
		CleanupStack::Pop(dataType);
		}

	CleanupStack::Pop(path);		// ownership transferred to CDirectoryRecognitionResult
	ASSERT(iFileRecognitionUtility);
	iFileRecognitionUtility->RecognizeSynchronouslyL(*iRecognitionResult);
	aMessage.WriteL(1,TPckgBuf<TUint>(iRecognitionResult->RequiredBufferSize()));
	}

void CApaAppArcServSession::TransferRecognitionResultL(const RMessage2& aMessage)
	{
	if(iRecognitionResult == NULL)
		User::Leave(KErrNotReady);

	iAsyncRecognitionActive = EFalse;

	// if data is too big for buffer, tell client
	const TInt sizeOfBuffer=aMessage.Int2();	
	if(sizeOfBuffer < iRecognitionResult->RequiredBufferSize())
		User::Leave(KErrTooBig);

	// buffer is big enough -> write result to buffer
	CBufFlat* const buf=CBufFlat::NewL(iRecognitionResult->RequiredBufferSize());
	CleanupStack::PushL(buf);
	buf->ExpandL(0,iRecognitionResult->RequiredBufferSize());

	RBufWriteStream writeStream;
	writeStream.Open(*buf);
	iRecognitionResult->WriteToStreamL(writeStream);
	aMessage.WriteL(1,buf->Ptr(0));

	delete iRecognitionResult;
	iRecognitionResult = NULL;

	CleanupStack::PopAndDestroy(buf);
	}

void CApaAppArcServSession::RecognizeFilesAsyncL(const RMessage2& aMessage)
	{
	if (iAsyncRecognitionActive)
		User::Leave(KErrInUse);
	else
		{
		_LIT8(KAllDataTypes,"*");

		HBufC* path = HBufC::NewLC(User::LeaveIfError(aMessage.GetDesLength(0)));
		TPtr pathPtr(path->Des());
		aMessage.ReadL(0,pathPtr);

		// retrieve data type filter
		HBufC8* dataType = HBufC8::NewLC(User::LeaveIfError(aMessage.GetDesLength(2)));
		TPtr8 dataTypePtr(dataType->Des());
		aMessage.ReadL(2,dataTypePtr);

		delete iRecognitionResult;
		iRecognitionResult = NULL;

		if(dataType->Compare(KAllDataTypes) == 0)
			{
			iRecognitionResult = new (ELeave) CDirectoryRecognitionResult(path,NULL);
			CleanupStack::PopAndDestroy(dataType);
			}
		else
			{
			iRecognitionResult = new (ELeave) CDirectoryRecognitionResult(path,dataType);
			CleanupStack::Pop(dataType);
			}

		CleanupStack::Pop(path);	// ownership transferred to CDirectoryRecognitionResult
		ASSERT(iFileRecognitionUtility);
		iFileRecognitionUtility->RecognizeAsynchronously(*iRecognitionResult,aMessage);
		iAsyncRecognitionActive = ETrue;
		}
	}

/** The function interrogates all available rule based plug-ins if apparc can launch application with 
given full name. It loops through all plug-ins until gets value different from 
CAppLaunchChecker::EAppLaunchIndifferent. 
The application will be launched if the return code is not equal to
CAppLaunchChecker::EAppLaunchDecline 
*/
void CApaAppArcServSession::AquirePermissionToLaunchAppL(const RMessage2& aMessage) const
	{
	const CApaScanningRuleBasedPlugIns* theRuleBasedPlugIns = iServ.RuleBasedPlugIns();
	const TInt theNumImp = (theRuleBasedPlugIns ? theRuleBasedPlugIns->ImplementationCount() : 0);
	if(!theNumImp || !iServ.WsSession().Handle())
		{
		// Proceed with launch if rule based plug-in framework was not initialized
		aMessage.Complete(ETrue);
		return;
		}

	// Get the name of the app to start from the IPC message object
	HBufC* theFullFileName = HBufC::NewLC(User::LeaveIfError(aMessage.GetDesLength(0)));
	TPtr theFullFileNamePtr(theFullFileName->Des());
	aMessage.ReadL(0, theFullFileNamePtr);
	
	// Ge the application's UID from its name
	const TUid theUid = iAppListSession->AppUidFromFullFileNameL(theFullFileNamePtr);
	CleanupStack::PopAndDestroy(theFullFileName); 

	// Iterate through all plug-ins and look for one that's not indifferent
	CAppLaunchChecker::TAppLaunchCode theLaunchCode = CAppLaunchChecker::EAppLaunchIndifferent;	
	TApaTaskList theTaskList(iServ.WsSession());
	for(TInt ii = 0; ii < theNumImp; ii++)
		{
		CAppLaunchChecker* const theLauncherChecker = (*theRuleBasedPlugIns)[ii];
		TRAP_IGNORE(theLaunchCode = theLauncherChecker->OkayToLaunchL(theUid, theTaskList));
		if(theLaunchCode != CAppLaunchChecker::EAppLaunchIndifferent)
			break;
		}
		
	// Return the result
	const TBool okayToLaunch = (theLaunchCode != CAppLaunchChecker::EAppLaunchDecline);
	aMessage.Complete(okayToLaunch);
	}

/** 
@param aFullFileName This filename is parsed and the path is replaced with "\\sys\\bin\\". 
		It uses ".exe" if no other is provided in the filename passed.  If drive name is 
		present in the filename then it scans through the \\sys\\bin of that particular drive,
		otherwise it scans through the \\sys\\bin folders in all the avaliable drives.
@return Returns the Application Uid for the aFullFileName application.
*/
TUid CApaAppListServSession::AppUidFromFullFileNameL(const TDesC& aFullFileName) const
	{
	
	// If the appliation still wasn't found, use AppArc's appliation list
	// Since we cannot get the Uid of NonNative apps by passing filename to RFs::Entry
	CApaAppData* appData = iAppList.AppDataByFileName(aFullFileName);
	if (appData)
		{
		TApaAppEntry appEntry = appData->AppEntry();
		return appEntry.iUidType[2];
		}
	
	
//mm: Why does this code not use AppArc's application list only?	
//mm: The order in which different methods to locate the app is used seems inefficient.
	_LIT(KSysBin, "\\sys\\bin\\");
	_LIT(KFileExtension, ".EXE");
	
	RLoader loader;
	User::LeaveIfError(loader.Connect());
	CleanupClosePushL(loader);
	TPckgBuf<RLibrary::TInfo> dllInfo;

	TParse parse;
	parse.Set(aFullFileName,NULL,NULL);

	// If the drive letter has been specified, look on that drive only...
	TInt error = KErrNotFound;
	if (parse.DrivePresent())
		{
		const TPtrC appDrive = parse.Drive();
		TBuf<KMaxFileName>fileName(appDrive);
		fileName.Append(KSysBin);
		User::LeaveIfError(parse.SetNoWild(fileName, &aFullFileName, &KFileExtension));
		error = loader.GetInfo(parse.FullName(), dllInfo);
		}
	else	// ...otherwise scan all drives.
		{
		User::LeaveIfError(parse.SetNoWild(KSysBin, &aFullFileName, &KFileExtension)); 
		TFileName tempFileName(parse.FullName());
		TDriveList driveList;
		User::LeaveIfError(iFs.DriveList(driveList));
		for (TInt driveNumber = EDriveY; driveNumber != KFinishedScanning; driveNumber = NextDriveToScan(driveNumber))
			{
			if (driveList[driveNumber]!=0)
				{
				User::LeaveIfError(parse.SetNoWild(TDriveUnit(driveNumber).Name(), &tempFileName, NULL));
				error = loader.GetInfo(parse.FullName(), dllInfo);
				if (error == KErrNone)
					break;
				}
			}
		}

	CleanupStack::PopAndDestroy(&loader);
	// Return the UID if found and no errors occured
	if(!error)
		return dllInfo().iUids[2];
	
	// If the application wasn't found
	// Try looking for the application using normal F32. This won't work in protected directories

	// Since we can't use RFs::Entry if the path refers the protected system directory (i.e. \sys\bin)...
	User::LeaveIfError(parse.SetNoWild(aFullFileName, NULL, NULL));
	if(parse.Path().FindF(KSysBin) == 0)
		User::Leave(KErrNotFound);	// ... then return not-found.

//mm: Surely we should still be allowed to use AppArc below?

	// The following is valid for non-native applications only
	TEntry entry;
	error = iFs.Entry(aFullFileName, entry);
	if(!error)
		return entry.iType[2];

	User::Leave(KErrNotFound);
	return TUid::Null();	// Won't be called
	}

/**
Applies the scanning order Y: through A: then Z: last.
*/
TInt CApaAppListServSession::NextDriveToScan(TInt aCurrentDrive)
	{
	if (aCurrentDrive == EDriveZ)
		return KFinishedScanning;
	else if (aCurrentDrive == 0)
		return EDriveZ; 	// finally scan the last one
	else if (aCurrentDrive > 0 && aCurrentDrive < KMaxDrives)
		return aCurrentDrive - 1;
	else
		return KErrGeneral; // never gets here, but it wont compile otherwise
	}

void CApaAppArcServSession::CancelRecognizeFiles()
	{
	if (iAsyncRecognitionActive)
		{
		ASSERT(iFileRecognitionUtility);
		iFileRecognitionUtility->CancelRecognitionRequest();
		iAsyncRecognitionActive = EFalse;
		}
	}

void CApaAppArcServSession::RecognizeDataPassedByFileHandleL(const RMessage2& aMessage)
// Recognize the data type of an object
	{
	RFile file;
	CleanupClosePushL(file);
	User::LeaveIfError(file.AdoptFromClient(aMessage, 1, 2));
	const TDataRecognitionResult result(iServ.RecognizeDataL(file, PreferredBufSize()));
	CleanupStack::PopAndDestroy(&file);
	aMessage.WriteL(0, TPckgC<TDataRecognitionResult>(result));
	}
	
void CApaAppArcServSession::RecognizeSpecificDataL(const RMessage2& aMessage)
// Determine whether an object is of a specific data type
    {
    HBufC* name=NULL;
    RChunk localChunk;
    
    TPtrC8 bufPtr=DoRecognizeUnpackLC(name, localChunk, aMessage);
    TDataType dataType;
    {TPckg<TDataType> dataType_asDescriptor(dataType);
    aMessage.ReadL(0, dataType_asDescriptor);}
    aMessage.Complete(iServ.RecognizeDataL(*name,bufPtr,dataType));
    CleanupStack::PopAndDestroy(2); // name & buffer or localChunk
    }

void CApaAppArcServSession::RecognizeSpecificDataPassedByFileHandleL(const RMessage2& aMessage)
	{
	RFile file;
	CleanupClosePushL(file);
	User::LeaveIfError(file.AdoptFromClient(aMessage, 1, 2));
	TDataType dataType;
	{TPckg<TDataType> dataType_asDescriptor(dataType);
	aMessage.ReadL(0,dataType_asDescriptor);}
	aMessage.Complete(iServ.RecognizeDataL(file, PreferredBufSize(), dataType));
	CleanupStack::PopAndDestroy(&file);
	}

void CApaAppListServSession::InitListL(const RMessage2& aMessage, TAppListType aType)
// carries out initialisation prior to starting to pass a new list across
	{
	iAppListType = aType;
	iAppListScreenMode = aMessage.Int0();
	if (aType == EListFilteredEmbeddedApps)
		{
		TApaEmbeddabilityFilter filter;
		{TPckg<TApaEmbeddabilityFilter> filter_asDescriptor(filter);
		aMessage.ReadL(1,filter_asDescriptor);}
		iEmbeddabilityFilter = filter;
		}

	if (aType == EListCapabilityAttrFilteredApps)
		{
		iCapabilityAttrFilterMask = aMessage.Int1();
		iCapabilityAttrFilterValue = aMessage.Int2();
		}

	if (aType == EListServerApps)
		iServiceUid = TUid::Uid(aMessage.Int1());

	iApaAppInfoArray.ResetAndDestroy();
	iFlags|=EAppListPopulationPending;
	}

void CApaAppListServSession::EmbedCount(const RMessage2& aMessage) const
// writes back the number of embedded apps in the list
	{
	TInt count=0;
	const CApaAppList& list = iAppList;
	CApaAppData* app = list.FirstApp();
	TApaEmbeddabilityFilter filter;
	filter.AddEmbeddability(TApaAppCapability::EEmbeddable);
	filter.AddEmbeddability(TApaAppCapability::EEmbeddableOnly);
	while (app)
		{
		if (!AppIsControlPanelItem(*app) && AppMatchesEmbeddabilityFilter(*app, filter))
			count++;

		app = list.NextApp(app);
		}

	aMessage.Complete(count);
	}

void CApaAppListServSession::AppCount(const RMessage2& aMessage) const
// writes back the number of apps in the list
	{
	TInt count = 0;
	const CApaAppList& list = iAppList;
	CApaAppData* app = list.FirstApp();
	while (app)
		{
		if (!AppIsControlPanelItem(*app))
			count++;

		app = list.NextApp(app);
		}

	aMessage.Complete(count);
	}

void CApaAppListServSession::GetNextAppL(const RMessage2& aMessage)
	{
	if (iFlags&EAppListPopulationPending)
		{
		const CApaAppList& list=iAppList;
		if (!(list.IsFirstScanComplete()))
			User::Leave(KErrCorrupt);
		
		iApaAppInfoArray.ResetAndDestroy();
		for (CApaAppData* appData = list.FirstApp(iAppListScreenMode); appData != NULL; appData = list.NextApp(appData, iAppListScreenMode))
			{
			if (iAppListType==EListFilteredEmbeddedApps && (AppIsControlPanelItem(*appData) || !AppMatchesEmbeddabilityFilter(*appData, iEmbeddabilityFilter)))
				continue;

			if (iAppListType==EListCapabilityAttrFilteredApps && !AppMatchesCapabilityAttrFilter(*appData))
				continue;

			if (iAppListType==EListServerApps && !appData->ImplementsService(iServiceUid))
				continue;

			CApaAppInfo* apaAppInfo= new (ELeave)CApaAppInfo();
			CleanupStack::PushL(apaAppInfo);
			apaAppInfo->SetCaptionL(appData->Caption());
			apaAppInfo->SetShortCaptionL(appData->ShortCaption());
			apaAppInfo->SetFullNameL(appData->AppEntry().iFullName);
			apaAppInfo->SetUid(appData->AppEntry().iUidType[2]);
			User::LeaveIfError(iApaAppInfoArray.Append(apaAppInfo));
			CleanupStack::Pop(apaAppInfo);
			}
		
		iFlags &= ~EAppListPopulationPending;
		}

	if (!iApaAppInfoArray.Count())
		User::Leave(KErrNotFound);

	TApaAppInfo* info=new(ELeave)TApaAppInfo; 
	CleanupStack::PushL(info);
	CApaAppInfo* apaAppInfo = iApaAppInfoArray[0];
	info->iUid = apaAppInfo->Uid();
	info->iFullName = apaAppInfo->FullName();
	info->iCaption = apaAppInfo->Caption();
	// Get the length of the target descriptor
	TInt targetLen=aMessage.GetDesMaxLength(1);
	if (targetLen==KApaAppInfoDesMaxLength)
		info->iShortCaption = apaAppInfo->ShortCaption();

	iApaAppInfoArray.Remove(0);
	delete apaAppInfo;
	TPckgC<TApaAppInfo> infoPk(*info);
	if (targetLen<KApaAppInfoDesMaxLength)
		infoPk.Set(infoPk.Left(_FOFF(TApaAppInfo,iShortCaption))); // reduce the length of infoPk to the 7.0 size of TApaAppInfo

	aMessage.WriteL(1,infoPk);
	CleanupStack::PopAndDestroy(info);
	}

TBool CApaAppListServSession::AppMatchesEmbeddabilityFilter(const CApaAppData& aAppData, const TApaEmbeddabilityFilter& aEmbeddabilityFilter) const
// returns True if aAppData's embeddability matches the filter set by InitListL
	{
	TApaAppCapabilityBuf capabilityBuf;
	aAppData.Capability(capabilityBuf);

	return (aEmbeddabilityFilter.MatchesEmbeddability(capabilityBuf().iEmbeddability));
	}

TBool CApaAppListServSession::AppMatchesCapabilityAttrFilter(const CApaAppData& aAppData) const
// returns True if aAppData's capability attributes match the filter set by InitListL
	{
	TApaAppCapabilityBuf capabilityBuf;
	aAppData.Capability(capabilityBuf);
	return ((capabilityBuf().iAttributes & iCapabilityAttrFilterMask) == (iCapabilityAttrFilterValue & iCapabilityAttrFilterMask));
	}

TBool CApaAppListServSession::AppIsControlPanelItem(const CApaAppData& aAppData)
// returns True if aAppData represents a control panel app
	{
	TApaAppCapabilityBuf capabilityBuf;
	aAppData.Capability(capabilityBuf);
	return (capabilityBuf().iAttributes & TApaAppCapability::EControlPanelItem);
	}


CApaAppData* CApaAppListServSession::FindSpecificAppL(TUid aAppUid)
	{
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
    CApaAppData* app = iAppList.FindAndAddSpecificAppL(aAppUid);	
#else
    //Scans the drives and folder lists for the specific app
    CApaAppRegFinder* regFinder = CApaAppRegFinder::NewLC(iFs);    
    CApaAppData* app = iAppList.FindAndAddSpecificAppL(regFinder, aAppUid);
    CleanupStack::PopAndDestroy(regFinder);
#endif
	return app;
	}

void CApaAppListServSession::GetAppInfoL(const RMessage2& aMessage)
	{
	// get UID of required app
	const TUid uid = TUid::Uid(aMessage.Int0());
	TApaAppInfo* info = new(ELeave) TApaAppInfo; // on heap to avoid running out of stack
	CleanupStack::PushL(info);
	GetAppInfoL(uid, *info);
	TPckgC<TApaAppInfo> infoPk(*info);	
	aMessage.WriteL(1, infoPk);
	CleanupStack::PopAndDestroy(info);
	}

void CApaAppListServSession::GetAppCapabilityL(const RMessage2& aMessage)
	{	  								  
	const TUid uid = TUid::Uid(aMessage.Int1());
	const CApaAppData& app = FindAppInListL(uid);

	TInt targetLen = aMessage.GetDesMaxLength(0);
	HBufC8* buf = HBufC8::NewLC(User::LeaveIfError(targetLen));
	TPtr8 ptr = buf->Des();
	app.Capability(ptr);
	ptr.SetLength(targetLen);
	aMessage.WriteL(0, *buf);
	CleanupStack::PopAndDestroy(buf); 
	}

void CApaAppListServSession::GetDefaultScreenNumberL(const RMessage2& aMessage)
	{
	const TUid uid=TUid::Uid(aMessage.Int0());
	const CApaAppData& app = FindAppInListL(uid);

	aMessage.Complete(app.DefaultScreenNumber());
	}


void CApaAppListServSession::SetNotify(const RMessage2& aMessage)
	{
	if (!iNotifyMessage.IsNull())
		aMessage.Panic(KApaPanicCli,ENotifierAlreadyPresent);
	else
		{
		const TBool completeImmediatelyIfNoScanImpendingOrInProgress=aMessage.Int0();
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
        if ((!completeImmediatelyIfNoScanImpendingOrInProgress) ||
            iAppList.AppScanInProgress())   
#else
        if ((!completeImmediatelyIfNoScanImpendingOrInProgress) ||
            iAppArcSrv.AppFsMonitor().AnyNotificationImpending() ||
            iAppList.AppScanInProgress())		
#endif
			iNotifyMessage=aMessage;
		else
			aMessage.Complete(KErrNone);
		}
	
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 	
	iNotificationRequested=ETrue;
#endif	
	}

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 
void CApaAppArcServSession::NotifyClients(TInt aReason, CArrayFixFlat<TApaAppUpdateInfo>* aUpdatedAppsInfo)
#else
void CApaAppArcServSession::NotifyClients(TInt aReason)
#endif

	{
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
	iAppListSession->NotifyClients(aReason, aUpdatedAppsInfo);
#else
	iAppListSession->NotifyClients(aReason);	
#endif
	}


void CApaAppListServSession::CancelNotify()
	{
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK     
	NotifyClients(KErrCancel, NULL);
#else
	NotifyClients(KErrCancel);
#endif
	}

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 
void CApaAppListServSession::NotifyClients(TInt aReason, CArrayFixFlat<TApaAppUpdateInfo>* aUpdatedAppsInfo)
#else
void CApaAppListServSession::NotifyClients(TInt aReason)
#endif
	{
	if (!iNotifyMessage.IsNull())
		iNotifyMessage.Complete(aReason);
	
	//Notify client for scan complete.
	NotifyScanComplete();
	
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 
	if(aUpdatedAppsInfo && iNotificationRequested)
	    {
        //Append the updated applications information to iAppsUpdated
        TInt count=aUpdatedAppsInfo->Count();
        for(TInt index=0; index<count; index++)
            {
            TRAPD(err, iAppsUpdated.AppendL((*aUpdatedAppsInfo)[index]));
            if(err != KErrNone)
                {
                iAppsUpdated.Reset();
                break;
                }
            }
	    }
#endif	
	}

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
void CApaAppListServSession::AppInfoProvidedByRegistrationFileL(const RMessage2& aMessage)
	{
	// get UID of required app
	const TUid uid = TUid::Uid(aMessage.Int0());

	// locate app in list
	const CApaAppData& app = FindAppInListL(uid);
	
	const TBool registrationFileUsed = app.RegistrationFileUsed();
	TPckgC<TBool> pckg(registrationFileUsed);
	aMessage.WriteL(1, pckg);
	}
#endif


void CApaAppListServSession::IconFileNameL(const RMessage2& aMessage)
	{
	// get UID of required app
	const TUid uid=TUid::Uid(aMessage.Int0());

	// locate app in list
	const CApaAppData& app = FindAppInListL(uid);

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
	if (!app.RegistrationFileUsed())     
	    User::Leave(KErrNotSupported);     
	 else     
	 { 
#endif
	 
        TPtrC iconFileName(app.IconFileName());
        if (iconFileName.Length() == 0)
            User::Leave(KErrNotFound);
        else
            {
            TFileName fileName = iconFileName;
            TPckgC<TFileName> pckg(fileName);
            aMessage.WriteL(1, pckg);
            }
        
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
	 }
#endif
	
	}

void CApaAppListServSession::ViewIconFileNameL(const RMessage2& aMessage)
	{
	// get UID of required app
	const TUid uid=TUid::Uid(aMessage.Int0());
	// get UID of required view
	const TUid viewUid=TUid::Uid(aMessage.Int1());

	TPtrC viewIconFileName;

	// locate app in list
	const CApaAppData& app = FindAppInListL(uid);

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 
    if (!app.RegistrationFileUsed())     
        User::Leave(KErrNotSupported);     
     else     
     { 
#endif
	
        const CArrayPtr<CApaAppViewData>& viewDataArray = *app.Views();
        const TInt count = viewDataArray.Count();
        for (TInt ii=0; ii<count; ii++)
            {
            const CApaAppViewData& appViewData = *viewDataArray[ii];
            if (appViewData.Uid() == viewUid)
                {
                viewIconFileName.Set(appViewData.IconFileName());
                break;
                }
            }
    
        if (viewIconFileName.Length() == 0)
            User::Leave(KErrNotFound);
        else
            {
            TFileName fileName = viewIconFileName;
            TPckgC<TFileName> pckg(fileName);
            aMessage.WriteL(2, pckg);
            }

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
     }
#endif
    
	}

void CApaAppArcServSession::GetAppServicesL(const RMessage2& aMessage)
	{
	const TInt initialBufSize = aMessage.Int2();
	if (initialBufSize)
		{
		delete iBuffer;
		iBuffer = NULL;
		iBuffer = GetServiceBufferL(aMessage);
		if (iBuffer->Size() > initialBufSize)
			User::Leave(iBuffer->Ptr(0).Size());	// default buffer provided by client is too small, ask client to provide buffer of correct size
		}
	__ASSERT_ALWAYS(iBuffer, aMessage.Panic(KApaPanicCli,EClientBadRequest));
	aMessage.WriteL(3, iBuffer->Ptr(0));
	delete iBuffer;
	iBuffer = NULL;
	}

CBufBase* CApaAppArcServSession::GetServiceBufferL(const RMessage2& aMessage) const
	{
	CBufBase* buffer = NULL;
	const TUid uid = TUid::Uid(aMessage.Int0());
	switch (aMessage.Function())
		{
	case EAppListServGetAppServices:
		buffer = iAppListSession->AppList().ServiceArrayBufferL(uid);
		break;
	case EAppListServGetServiceImplementations:
		buffer = iAppListSession->AppList().ServiceImplArrayBufferL(uid);
		break;
	case EAppListServGetServiceImplementationsDataType:
		{
		TDataType dataType;
		TPckg<TDataType> dataType_asDescriptor(dataType);
		aMessage.ReadL(1,dataType_asDescriptor);
		buffer = iAppListSession->AppList().ServiceImplArrayBufferL(uid, dataType);
		}
		break;
	case EAppListServGetAppServiceUids:
		buffer = iAppListSession->AppList().ServiceUidBufferL(uid);
		break;
	case EAppListServGetAppServiceOpaqueData:
		buffer = iAppListSession->AppList().ServiceOpaqueDataBufferL(uid, TUid::Uid(aMessage.Int1()));
		break;
	default:
		aMessage.Panic(KApaPanicCli,EClientBadRequest);
		User::Leave(KErrNotSupported);
		break;
		}

	return buffer;
	}

CApaAppListServSession::CApaAppInfo::CApaAppInfo()
	:iUid(KNullUid), iCaption(NULL), iShortCaption(NULL), iFullName(NULL)
	{
	}

CApaAppListServSession::CApaAppInfo::~CApaAppInfo()
	{
	delete iCaption;
	delete iShortCaption;
	delete iFullName;
	}

void CApaAppListServSession::CApaAppInfo::SetUid(const TUid aUid)
	{
	iUid=aUid;
	}

void CApaAppListServSession::CApaAppInfo::SetCaptionL(const TDesC& aCaption)
	{
	HBufC* caption = aCaption.AllocL();
	delete iCaption;
	iCaption = caption;
	}

void CApaAppListServSession::CApaAppInfo::SetShortCaptionL(const TDesC& aShortCaption)
	{
	HBufC* shortCaption = aShortCaption.AllocL();
	delete iShortCaption;
	iShortCaption = shortCaption;
	}

void CApaAppListServSession::CApaAppInfo::SetFullNameL(const TDesC& aFullName)
	{
	HBufC* fullName = aFullName.AllocL();
	delete iFullName;
	iFullName = fullName;
	}

void CApaAppListServSession::RegisterListPopulationCompleteObserver(const RMessage2& aMessage)
	{
	if (!iCompletionOfListPopulationObserverMsg.IsNull())
		aMessage.Panic(KApaPanicCli,EObserverAlreadyPresent);
	else
		{
		if(iAppList.IsFirstScanComplete())
			aMessage.Complete(KErrNone);
		else
			iCompletionOfListPopulationObserverMsg=aMessage;
		}
	}

void CApaAppArcServSession::NotifyClientForCompletionOfListPopulation()
	{
	iAppListSession->NotifyClientForCompletionOfListPopulation();
	}

void CApaAppListServSession::NotifyClientForCompletionOfListPopulation()
	{
	if (!iCompletionOfListPopulationObserverMsg.IsNull())
		iCompletionOfListPopulationObserverMsg.Complete(KErrNone);
	} //lint !e1762 Suppress member function could be made const


void CApaAppListServSession::CancelListPopulationCompleteObserver()
	{
	if (!iCompletionOfListPopulationObserverMsg.IsNull())
		iCompletionOfListPopulationObserverMsg.Complete(KErrCancel);
	} //lint !e1762 Suppress member function could be made const
	
void CApaAppArcServSession::NotifyClientOfDataMappingChange()
	{
	if (!iMessage_NotifyOnDataMappingChange.IsNull())
		iMessage_NotifyOnDataMappingChange.Complete(KErrNone);
	} //lint !e1762 Suppress member function could be made const

void CApaAppListServSession::MatchesSecurityPolicyL(const RMessage2& aMessage)
	{
	const TUid appUid=TUid::Uid(aMessage.Int0());
	TApaAppInfo* const appInfo=new(ELeave) TApaAppInfo;
	CleanupStack::PushL(appInfo);
	//Get the app info for the given App uid.
	GetAppInfoL(appUid, *appInfo);
	const TPtrC executableName(appInfo->iFullName);
	//Create a process only if the executable name is of non zero length
	if (executableName.Length() != 0)
		{
		RProcess process;
		TInt result = process.Create(executableName, KNullDesC);
		//Proceed with checking the security policy if the process is created normally
		if (result == KErrNone)
			{
			TPckgBuf<TSecurityPolicy> securityPolicy;
			aMessage.ReadL(1,securityPolicy);
			aMessage.Complete(securityPolicy().CheckPolicy(process));
			process.Close();
			}
		else
			aMessage.Complete(result);
		}
	else
		aMessage.Complete(KErrNotFound);

	CleanupStack::PopAndDestroy(appInfo);
	}
	
void CApaAppListServSession::SetAppShortCaptionL(const RMessage2& aMessage)
	{
	const TUid uid=TUid::Uid(aMessage.Int0());
	CApaAppData* app=NULL;
	TApaAppEntry entry;
	if (!FindAppInList(app,entry,uid))
		User::Leave(KErrNotFound);

	const TInt length = aMessage.GetDesLength(1);
	if(length < 0)
		User::Leave(length);

	HBufC* const shortCaption = HBufC::NewLC(length);
	TPtr captionPtr(shortCaption->Des());
	aMessage.ReadL(1, captionPtr);

	TLanguage appLanguage = TLanguage(aMessage.Int2());
	iAppList.AddCustomAppInfoInListL(uid, appLanguage, *shortCaption);
	if(app->ApplicationLanguage() == appLanguage)
		app->SetShortCaptionL(*shortCaption);
	else if(appLanguage==ELangNone)
		iAppList.UpdateAppListByShortCaptionL(); 

	CleanupStack::PopAndDestroy(shortCaption);
	}


void CApaAppArcServSession::NotifyScanComplete()
	{
	iAppListSession->NotifyScanComplete();
	}

void CApaAppListServSession::NotifyScanComplete()
	{
	//See if the session is intrested in scan complete notification.
	if (!iNotifyOnScanCompleteMsg.IsNull())
		{
		iNotifyOnScanCompleteMsg.Complete(KErrNone);
		}
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
	iNonNativeApplicationsManager->NotifyScanComplete();
#endif	
	} //lint !e1762 Suppress member function could be made const
	

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK

//Update applist based on the information provided by the installers.
void CApaAppListServSession::UpdateAppListL(const RMessage2& aMessage)
{
    //Create the update info array from the buffer.
    TInt bufferSize = aMessage.GetDesLength(0);
    User::LeaveIfError(bufferSize);
    HBufC8* const buffer=HBufC8::NewLC(bufferSize);
    TPtr8 buffer_asWritable(buffer->Des());
    aMessage.ReadL(0,buffer_asWritable);
        
    RDesReadStream readStream(*buffer);
    CleanupClosePushL(readStream);
    const TUint count=readStream.ReadUint32L();

    RArray<TApaAppUpdateInfo> *appUpdateInfo=new (ELeave) RArray<TApaAppUpdateInfo>(5);
    CleanupStack::PushL(appUpdateInfo);
    
    for(TUint index=0; index<count; index++)
        {
        TApaAppUpdateInfo appInfo;
        readStream>>appInfo;
        appUpdateInfo->AppendL(appInfo);
        }
    CleanupStack::Pop(appUpdateInfo);
    CleanupStack::PopAndDestroy(2, buffer); //delete readStream, buffer
    
    iAppArcSrv.UpdateAppListL(appUpdateInfo, TUid::Uid(aMessage.SecureId()));
}


void CApaAppListServSession::UpdatedAppsInfoL(const RMessage2& aMessage)
    {
    //Read the buffer size
    TInt sizeOfBuffer=aMessage.GetDesMaxLengthL(0);
    TInt count=iAppsUpdated.Count();
    TInt sizeRequired= sizeof(TInt)+(sizeof(TApaAppUpdateInfo) * count);

    TPckgBuf<TInt> pckg(sizeRequired);
    
    //If size of the buffer is not enough write the required size and leave.
    if(sizeOfBuffer<sizeRequired)
        {
        aMessage.WriteL(1, pckg);    
        User::Leave(KErrOverflow);
        }
    
    //If the passed buffer size is enough, create a buffer to write updates application information.
    CBufFlat* buffer=CBufFlat::NewL(sizeRequired);
    CleanupStack::PushL(buffer);
    buffer->ExpandL(0, sizeRequired);
    
    RBufWriteStream writeStream;
    writeStream.Open(*buffer);
    
    //Write count to stream.
    writeStream.WriteUint32L(count);
    
    //Write updated applications information to stream.
    for(TInt index=0; index<count; index++)
        {
        writeStream<<iAppsUpdated[index];
        }
    //Write the buffer into passed buffer.
    aMessage.WriteL(0, buffer->Ptr(0));
    //Write size of the buffer
    aMessage.WriteL(1, pckg);
    
    CleanupStack::PopAndDestroy(buffer); 
    iAppsUpdated.Reset();
    iNotificationRequested=EFalse;
    }
#endif

// TSizeArrayItemWriter

TInt TSizeArrayItemWriter::ArrayItemCount() const
	{
	return iArray.Count();
	}

TInt TSizeArrayItemWriter::ArrayItemSize() const
	{
	return sizeof(TSize);
	}

void TSizeArrayItemWriter::WriteArrayItemL(RWriteStream& aWriteStream,TInt aIndex) const
	{
	const TSize& size = iArray[aIndex];
	aWriteStream.WriteUint32L(size.iWidth);
	aWriteStream.WriteUint32L(size.iHeight);
	}

// TViewDataArrayItemWriter

TInt TViewDataArrayItemWriter::ArrayItemCount() const
	{
	return iArray.Count();
	}

TInt TViewDataArrayItemWriter::ArrayItemSize() const
	{
	return sizeof(TApaAppViewInfo);
	}

void TViewDataArrayItemWriter::WriteArrayItemL(RWriteStream& aWriteStream,TInt aIndex) const
	{
	const CApaAppViewData& appViewData=*(iArray[aIndex]);
	aWriteStream << TApaAppViewInfo(appViewData.Uid(),appViewData.Caption(),appViewData.ScreenMode());
	}

// TDesCArrayItemWriter

TInt TDesCArrayItemWriter::ArrayItemCount() const
	{
	return iArray.Count();
	}

TInt TDesCArrayItemWriter::ArrayItemSize() const
	{
	return sizeof(TFileName);
	}

void TDesCArrayItemWriter::WriteArrayItemL(RWriteStream& aWriteStream,TInt aIndex) const
	{
	aWriteStream << iArray[aIndex];
	}

