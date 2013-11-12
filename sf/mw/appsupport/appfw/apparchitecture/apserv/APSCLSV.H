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
// apsclsv.h
//

#ifndef __APGCLSV_H__
#define __APGCLSV_H__

#include <e32base.h>
#include <apaid.h>
#include <apmstd.h>

// A version must be specifyed when creating a session with the server
const TUint KAppListServMajorVersionNumber=7;
const TUint KAppListServMinorVersionNumber=1;
const TUint KAppListServBuildVersionNumber=117;

enum TAppListServRanges
	{
	EFirstUnrestrictedOpcodeInAppListServ,
	EFirstOpcodeNeedingWriteDeviceDataInAppListServ=100,
	};

// opcodes used in message passing between client and server
enum TCountServRqst
	{
	//No Capability requirement
	EAppListServFirst = EFirstUnrestrictedOpcodeInAppListServ,	// = 0
	EAppListServInitFullList,
	EAppListServInitEmbedList,
	EAppListServGetNextApp,
	EAppListServEmbedCount,
	EAppListServAppCount,
	EAppListServGetAppInfo,
	EAppListServGetAppCapability,
	EAppListServStartAppWithoutReturningThreadId,	// deprecated
	EAppListServStartAppReturningThreadId,			// deprecated
	EAppListServRecognizeData,	// = 10
	EAppListServRecognizeDataPassedByFileHandle,
	EAppListServRecognizeSpecificData,
	EAppListServRecognizeSpecificDataPassedByFileHandle,
	EAppListServAppForDataType,
	EAppListServStartDocument,						// deprecated
	EAppListServStartDocumentByDataType,			// deprecated
	EAppListServStartDocumentByUid,					// deprecated
	EAppListServCreateDocumentByUid,				// deprecated
	EAppListServGetExecutableNameGivenDocument,
	EAppListServGetExecutableNameGivenDocumentPassedByFileHandle,	// = 20
	EAppListServGetExecutableNameGivenDataType,
	EAppListServGetExecutableNameGivenAppUid,
	EAppListServGetOpaqueData,
	EAppListServGetNativeExecutableNameIfNonNative,
	EAppListServAppIconByUid,
	EAppListServAppForDocument,
	EAppListServAppForDocumentPassedByFileHandle,
	EAppListServGetConfidence,
	EAppListServGetBufSize,
	EAppListServSetBufSize,	// = 30
	EAppListServGetDataTypesPhase1,
	EAppListServGetDataTypesPhase2,
	ESetNotify,
	ECancelNotify,
	EDCloseServer,
	EAppListServAppIconByUidAndSize,
	EAppListServGetAppIconSizes,
	EAppListServGetAppViews,
	EAppListServViewIconByUidAndSize,
	EAppListServGetFileOwnershipInfo,	// = 40
	EAppListServNumberOfOwnDefinedIcons,
	EAppListServInitFilteredEmbedList,
	EAppListServInitAttrFilteredList,
	EAppListServAppIconFileName,
	EAppListServAppViewIconFileName,
	EAppListServInitServerAppList,
	EAppListServGetAppServices,
	EAppListServGetServiceImplementations,
	EAppListServGetServiceImplementationsDataType,
	EAppListServGetAppServiceUids,	// = 50
	EAppListServGetAppServiceOpaqueData,
	EAppListServAppForDataTypeAndService,
	EAppListServAppForDocumentAndService,
	EAppListServAppForDocumentAndServicePassedByFileHandle,
	EAppListServApplicationLanguage,
	ERegisterListPopulationCompleteObserver,
	ECancelListPopulationCompleteObserver,
	EAppListServPreferredBufSize,
	EAppListServRecognizeFiles,
	EAppListServRecognizeFilesAsync,	// = 60
	ECancelRecognizeFiles,
	EAppListServTransferRecognitionResult,
	EAppListServGetAppByDataType,
	EAppListServGetDefaultScreenNumber,
	EAppListServRuleBasedLaunching,
	EAppListServRegisterNonNativeApplicationType,
	EAppListServDeregisterNonNativeApplicationType,
	EAppListServPrepareNonNativeApplicationsUpdates,
	EAppListServRegisterNonNativeApplication,
	EAppListServDeregisterNonNativeApplication,	// = 70
	EAppListServCommitNonNativeApplications,
	EAppListServRollbackNonNativeApplications,
	EAppListServGetAppType,
	ENotifyOnDataMappingChange,
	ECancelNotifyOnDataMappingChange,
	EMatchesSecurityPolicy,
	EAppListServSetAppShortCaption,
	EAppListServForceRegistration,
	EDebugHeapMark,
	EDebugHeapMarkEnd,	// = 80
	EDebugHeapFailNext,
	EDebugClearAppInfoArray,
	EDebugFlushRecognitionCache,
	EDebugSetLoadRecognizersOnDemand,
	EDebugPerformOutstandingRecognizerUnloading,
	EAppListServAppIconFileHandle,
	EDebugAddFailingNonNativeApplicationsUpdate,
	EDebugAddPanicingNonNativeApplicationsUpdate,
	EDebugAddRollbackPanicingNonNativeApplicationsUpdate, // = 89
	EAppListServUpdateAppList,
	EAppListUpdatedAppsInfo,
	EAppListServAppInfoProvidedByRegistrationFile = 99,	// = 99
	//WriteDeviceData Capability requirement
	// ER5
	EAppListServSetConfidence=EFirstOpcodeNeedingWriteDeviceDataInAppListServ,	// = 100
	// 8.1
	EAppListInsertDataMapping,
	EAppListInsertDataMappingIfHigher,
	EAppListDeleteDataMapping,
	// End Marker no Capability
	EAppListFirstUnusedOpcode,
 	};


enum TApaClientPanic
	{
	EClientBadRequest,
	EClientBadDescriptor,
	ENotifierAlreadyPresent,
	ENoSupportedDataTypes,
	EObserverAlreadyPresent,
	ENotifyOnDataMappingChangeRequestOutstanding
#if defined(UI_FRAMEWORKS_V1_REMNANT_FOR_JAVA_MIDLET_INSTALLER)
	,EInvalidFileName
#endif
	};


struct SReturnData_AppForDocument
	{
	TUid iUid;
	TDataType iDataType;
	};

struct SReturnData_AppIconByUid
	{
	TInt iIcon;
	TInt iIconMask;
	};

struct SReturnData_AppIconByUidAndSize
	{
	TInt iIcon;
	TInt iIconMask;
	};

struct SReturnData_ViewIconByUidAndSize
	{
	TInt iIcon;
	TInt iIconMask;
	};

struct SNonNativeApplicationInfo
	{
	TUid iApplicationType;
	TInt iDrive;
	};

#endif	// __APGCLSV_H__
