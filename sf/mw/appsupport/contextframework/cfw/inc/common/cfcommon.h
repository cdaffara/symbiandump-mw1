/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  ?Description
*
*/


#ifndef CFCOMMON_H
#define CFCOMMON_H

// Panic categories
_LIT( KPanicCatClient, "CfClient" );

/**
* Reasons for Server Panic.
*/
enum TContextServPanic
    {
    EBadRequest,
    EBadDescriptor,
    EMainSchedulerError,
    ESvrCreateServer,
    ESvrStartServer,
    ECreateTrapCleanup,
    ENotImplementedYet,
    EReceiveReceiveAlreadyActive,
    ESrvCreateServer,
    ENoCurrentAction,
    EAlreadyReceivingScriptNotification
    };

// OpCodes used in message passing between client and server
enum TContextServRqst
    {
    // Context related operation codes
    EDefineContext,
    EPublishContext,
    EReceiveContextIndicationMessage,
    EGetContextIndication,
    ECancelReceiveContextIndicationMessage,
    ECreate,
    EClose,
    EAddSubscription,
    ERemoveSubscription,
    ERequestContext,
    ERequestContextSet,
    EPublishContextWithData,
    EGetContextSubscriptionError,
    EReceiveMessage,
    EGetMessage,
    ECancelReceiveMessage,

    // Action related operation codes
    EDefineAction,
    ESubscribeAction,
    EUnsubscribeAction,
    EListenActionSubscription,
    ECancelListenActionSubscription,
    EGetActionIndication,
    
    // Script related operation codes
    ERegisterScriptByName,
    EDeregisterScriptById,
    ERegisterScriptByNameAndDesc,
    ESaveScriptByName,
    ESaveScriptByNameAndDesc,
    EDeleteScriptByName,
    EDeleteScriptByUid,
    EUpgradeRomScriptByName,
    EUpgradeRomScriptByNameAndDesc,
    ERestoreRomScript,
    EScriptNotificationStart,
    EScriptNotificationCancel,
    EGetScriptNotification,

    // Context source related operation codes
    EInstallSourceSetting,
    EUninstallSourceSetting,
    EUninstallSourceSettings,
    
    // Keep this last
    ENumOfOpCodes
    };

// OpCodes used in message passing between client and server
enum TContextServCallbackMessages
    {
    // Context related operation codes
    ESubscribers,
    ENoSubscribers,
    EHandleCommand
    };    
    
// Server leave codes
enum TContextServLeave
    {
    ENonNumericString,
    EListenAlreadyListening
    };

// server name
_LIT( KContextServerName, "CFServer" );

// A version must be specifyed when creating a session with the server
const TUint KContextServMajorVersionNumber=1;
const TUint KContextServMinorVersionNumber=0;
const TUint KContextServBuildVersionNumber=1;

// Context data object default size if not defined in cenrep
const TUint KContextDataObjectDefaultSize = 0x2000; // 8kb

// IPC communication constants
const TInt KMaxSizeFixedArray = 5;
const TInt KMaxBufferSize = 1024;

const TUid KCFServerSid = {0x10282BC4};

// Special contextfw error codes.
// These must not cause leave if called User::LeaveIfError().
enum TCfwErrorCodes
    {
    // Base error code
    KErrCfwErrorBase = 1000,
    KErrDeregisterNotNeeded,
    };
    
// Format string to create persistent data file from:
// - Owner uid
// - Script name
// - Persistent data name
_LIT( KPersistenDataFormat, "c:\\private\\10282bc4\\rules\\%x\\%S-%S.dat" );

// Format string to create persistent data file from:
// - Owner uid
// - Persistent data name
_LIT( KPersistenDataFormat2, "c:\\private\\10282bc4\\rules\\%x\\%S.dat" );

// Format string for creating a file name for a saved script:
// - Owner uid
// - Script name
_LIT( KDefaultSystemRuleFileFormat, "!:\\private\\10282BC4\\Rules\\%x\\%S.rul" );

// Format string for creating a file name for rom upgrade:
// - Script name
_LIT( KDefaultRomRuleFileUpgradeFormat, "!:\\private\\10282BC4\\Rules\\%S.rul" );

// Script tag to identify the XML file as Contextfw script
_LIT8( KScriptStartTag, "<script" );

#endif // CFCOMMON_H

// End of File
