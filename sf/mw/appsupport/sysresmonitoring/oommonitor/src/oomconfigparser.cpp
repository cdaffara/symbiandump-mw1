/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The parser for the OOM configuration file.
*
*/
#include <e32base.h>
#include <xml/parser.h>

#include "oomconfigparser.h"
#include "oompanic.h"
#include "OomTraces.h"
#include "oomidletimerule.h"
#include "oomforegroundrule.h"
#include "oomconstants.hrh"
#include "oomapplicationconfig.h"
#include "oomcloseappconfig.h"
#include "oomconfig.h"
#include "oomrunpluginconfig.h"

enum TOomConfigParserPanic
{
KOomErrMoreThanOneOomConfig = 0,
KOomErrGlobalSettingsMustComeAfterRoot,
KOomErrAppSettingsMustComeAfterGlobalSettings,
KOomErrCloseAppSettingsMustComeAfterAppSettings,
KOomErrAppIdleSettingsMustComeAfterAppCloseSettings,
KOomErrLowRamErrorInGlobalSettings,
KOomErrGoodRamErrorInGlobalSettings,
KOomErrSwapUsageMonitoredErrorInGlobalSettings,
KOomErrLowSwapErrorInGlobalSettings,
KOomErrGoodSwapErrorInGlobalSettings,
KOomErrMaxCloseErrorInGlobalSettings,
KOomErrDefaultPriorityErrorInGlobalSettings,
KOomErrMissingUidFromAppCloseConfig,
KOomErrMissingPriorityFromAppCloseConfig,
KOomErrMissingSyncModeFromAppCloseConfig,
KOomErrMissingEstimateFromAppCloseConfig,
KOomErrInvalidSyncMode,
KOomErrMissingSyncModeInAppCloseConfig,
KOomErrBadOrMissingPriorityInAppIdleRule,
KOomErrBadOrMissingIdleTimeInAppIdleRule,
KOomErrBadOrMissingUidInAppIdleRule,
KOomErrBadNeverCloseValue,
KOomErrBadOrMissingUidInAppConfig,
KOomErrBadOrMissingPriorityInAppCloseConfig,
KOomErrBadLowThresholdValueForAppConfig,
KOomErrBadGoodThresholdValueForAppConfig,
KOomErrSystemPluginSettingsMustComeAfterAppCloseSettings,
KOomErrAppPluginSettingsMustComeAfterSystemPluginSettings,
KOomErrAppPluginIdleTimeRulesMustComeAfterAppPluginSettings,
KOomErrBadOrMissingUidInAppCloseConfig,
KOomErrBadOrMissingUidInSystemPluginConfig,
KOomErrBadOrMissingPriorityInSystemPluginConfig,
KOomErrBadOrMissingTargetAppIdInAppPluginConfig,
KOomErrBadOrMissingUidInAppPluginConfig,
KOomErrBadOrMissingPriorityInAppPluginConfig,
KOomErrBadOrMissingPriorityInPluginIdleRule,
KOomErrBadOrMissingIdleTimeInPluginIdleRule,
KOomErrBadOrMissingUidInPluginIdleRule,
KOomErrBadOrMissingUidInForegroundAppRule,
KOomErrBadOrMissingPriorityInForegroundAppRule,
KOomErrBadOrMissingTargetAppIdInForegroundAppRule,
KOomErrDefaultWaitAfterPluginInGlobalSettings,
KOomErrBadOrMissingPriorityInForceCheck,
KOomErrOomRulesMustComeLast,
KOomErrBadPluginWaitTime,
KOomErrBadXml,
KOomErrAppCloseIdleRuleOutsideAppCloseElement,
KOomErrForegroundAppRuleOutsideAppCloseElement,
KOomErrPluginIdleRuleOutsideAppPluginElement,
KOomErrPluginForegroundRuleOutsidePluginElement,
KOomErrBadCallIfTargetAppNotRunning
};

const TInt KOomXmlFileBufferSize = 1024;
const TInt KOomMaxAppExitTime = 2000;
const TInt KBytesInMegabyte = 1024;
#ifdef __WINS__
const TInt KEmulatorTickDivisor = 5; // The tick is 5 times slower on the emulator than on the phone
#endif
using namespace Xml;

// Mime type of the parsed document
_LIT8(KXmlMimeType, "text/xml");

_LIT(KOomConfigFilePath, ":\\private\\10207218\\oomconfig.xml");
_LIT(KRomDrive, "z");

// Element strings
// Root
_LIT8(KOomConfigOomConfig, "oom_config");

// Global settings
_LIT8(KOomConfigGlobalSettings, "global_settings");
_LIT8(KOomConfigForceCheckAtPriority, "force_check");

// App settings
_LIT8(KOomConfigAppSettings, "app_specific_thresholds");
_LIT8(KOomConfigApp, "app");

// App close settings
_LIT8(KOomConfigAppCloseSettings, "app_close_settings");
_LIT8(KOomConfigCloseApp, "close_app");

// App close idle time
_LIT8(KOomConfigAppCloseIdlePriority, "app_close_idle_priority");

_LIT8(KOomConfigForegroundAppPriority, "foreground_app_priority");

// Global settings attribute names
_LIT8(KOomAttributeLowRamThreshold, "low_ram_threshold");
_LIT8(KOomAttributeGoodRamThreshold, "good_ram_threshold");
_LIT8(KOomAttributeSwapUsageMonitored, "swap_usage_monitored");
_LIT8(KOomAttributeLowSwapThreshold, "low_swap_threshold");
_LIT8(KOomAttributeGoodSwapThreshold, "good_swap_threshold");
_LIT8(KOomAttributeMaxAppCloseBatch, "max_app_close_batch");
_LIT8(KOomAttributeDefaultWaitAfterPlugin, "default_wait_after_plugin");
_LIT8(KOomAttributeMaxAppExitTime , "max_app_exit_time");

// System plugins 

_LIT8(KOomAttributeSystemPluginSettings, "system_plugin_settings");
_LIT8(KOomAttributeSystemPlugin, "system_plugin");

// Application plugins

_LIT8(KOomAttributeAppPluginSettings, "app_plugin_settings");
_LIT8(KOomAttributeAppPlugin, "app_plugin");

// Plugin idle time rules

_LIT8(KOomAttributePluginIdlePriority, "plugin_idle_priority");

// Plugin foreground app rules
_LIT8(KOomAttributePluginForegroundAppPriority, "plugin_foreground_app_priority");

// Atribute names
_LIT8(KOomAttibuteUid, "uid");
_LIT8(KOomAttibuteSyncMode, "sync_mode");
_LIT8(KOomAttibutePriority, "priority");
_LIT8(KOomAttibuteRamEstimate, "ram_estimate");

_LIT8(KOomConfigSyncModeContinue, "continue");
_LIT8(KOomConfigSyncModeCheck, "check");
_LIT8(KOomConfigSyncModeEstimate, "estimate");

_LIT8(KOomAttibuteIdleTime, "idle_time");
_LIT8(KOomAttibuteIdlePriority, "priority");

_LIT8(KOomAttibuteNeverClose, "NEVER_CLOSE");

_LIT8(KOomAttributeTargetAppId, "target_app_id");

_LIT8(KOomAttributeWait, "wait");

_LIT8(KOomAttributeIfForegroundAppId, "if_foreground_app_id");

_LIT8(KOomAttributeCallIfTargetAppNotRunning, "call_if_target_app_not_running");
_LIT8(KOomAttributeTrue, "true");
_LIT8(KOomAttributeFalse, "false");
_LIT8(KOomAttribute0, "0");
_LIT8(KOomAttribute1, "1");


_LIT8(KOomConfigDefaultAppUid, "DEFAULT_APP");
_LIT8(KOomConfigDefaultPluginUid, "DEFAULT_PLUGIN");
_LIT8(KOomConfigTargetAppValue, "TARGET_APP");

_LIT8(KOomConfigBusyAppUid, "BUSY_APP");
_LIT8(KOomConfigHighPriorityAppUid, "HIGH_PRIORITY_APP");

COomConfigParser::COomConfigParser(COomConfig& aConfig, RFs& aFs) : iConfig(aConfig), iFs(aFs), iState(EOomParsingStateNone)
    {
    }

void COomConfigParser::ParseL()
    {
    FUNC_LOG;

    TRACES("COomConfigParser::ParseL: Parsing Config File");
    
    CParser* parser = CParser::NewLC(KXmlMimeType, *this);
    
    RFile configFile;
    TFileName configFileName;
    TChar driveChar = iFs.GetSystemDriveChar();
    configFileName.Append(driveChar);
    configFileName.Append(KOomConfigFilePath);
    if (configFile.Open(iFs, configFileName, EFileShareExclusive) != KErrNone)
        {
        configFileName.Replace(0,1,KRomDrive); //replace 'c' with 'z'
        User::LeaveIfError(configFile.Open(iFs, configFileName, EFileShareExclusive));
        }
    CleanupClosePushL(configFile);
    
    TBuf8<KOomXmlFileBufferSize> fileBuffer;
    TInt bytesRead;
    do
        {
        User::LeaveIfError(configFile.Read(fileBuffer));
        bytesRead = fileBuffer.Size();
        
        parser->ParseL(fileBuffer);
        
        } while (bytesRead != 0);
    
    CleanupStack::PopAndDestroy(2, parser); // config file - automatically closes it
                                            // parser
            
    TRACES("COomConfigParser::ParseL: Finished Parsing Config File");    
    }

void COomConfigParser::OnStartDocumentL(const RDocumentParameters&, TInt)
    {
    FUNC_LOG;
    }

void COomConfigParser::OnEndDocumentL(TInt)
    {
    FUNC_LOG;
    }


void COomConfigParser::OnEndElementL(const RTagInfo&, TInt)
    {
    }

void COomConfigParser::OnContentL(const TDesC8&, TInt)
    {
    }

void COomConfigParser::OnStartPrefixMappingL(const RString&, const RString&, 
                                   TInt)
    {
    }

void COomConfigParser::OnEndPrefixMappingL(const RString&, TInt)
    {
    }

void COomConfigParser::OnIgnorableWhiteSpaceL(const TDesC8&, TInt)
    {
    }

void COomConfigParser::OnSkippedEntityL(const RString&, TInt)
    {
    }

void COomConfigParser::OnProcessingInstructionL(const TDesC8&, const TDesC8&, 
                                      TInt)
    {
    }

void COomConfigParser::OnError(TInt)
    {
    }

TAny* COomConfigParser::GetExtendedInterface(const TInt32)
    {
    return 0;
    }

void COomConfigParser::OnStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes, 
                             TInt aErrorCode)
    {
    if (aErrorCode != KErrNone)
        ConfigError(KOomErrBadXml);
    
    StartElementL(aElement.LocalName().DesC(), aAttributes);
    }

void COomConfigParser::StartElementL(const TDesC8& aLocalName,
                                        const RAttributeArray& aAttributes)
    {
    // Root
    if (aLocalName == KOomConfigOomConfig)
        {
        if (iState != EOomParsingStateNone)
            ConfigError(KOomErrMoreThanOneOomConfig);
        
        ChangeState(EOomParsingStateRoot);
        }
    // Parse main elements
    else if (aLocalName == KOomConfigGlobalSettings)
        {
        if (iState != EOomParsingStateRoot)
            ConfigError(KOomErrGlobalSettingsMustComeAfterRoot);
        
        SetGlobalSettings(aAttributes);
        
        ChangeState(EOomParsingStateGlobalSettings);
        }
    else if (aLocalName == KOomConfigAppSettings)
        {
        ChangeState(EOomParsingStateAppSettings);
        }
    else if (aLocalName == KOomConfigAppCloseSettings)
        {
        ChangeState(EOomParsingStateAppCloseSettings);
        }
    else if (aLocalName == KOomAttributeSystemPluginSettings)
        {
        ChangeState(EOomParsingStateSystemPluginSettings);
        }
    else if (aLocalName == KOomAttributeAppPluginSettings)
        {
        ChangeState(EOomParsingStateAppPluginSettings);
        }
    // Parse actual configuration elements
    else if (aLocalName == KOomConfigForceCheckAtPriority)
        {
        SetForceCheckConfigL(aAttributes);
        }
    else if (aLocalName == KOomConfigApp)
        {
        SetAppConfigL(aAttributes);
        }
    else if (aLocalName == KOomConfigCloseApp)
        {
        SetCloseAppConfigL(aAttributes);
        }
    else if (aLocalName == KOomConfigAppCloseIdlePriority)
        {
        CheckState(EOomParsingStateAppCloseSettings, KOomErrAppCloseIdleRuleOutsideAppCloseElement);
        SetAppCloseIdlePriorityConfigL(aAttributes);
        }
    else if (aLocalName == KOomConfigForegroundAppPriority)
        {
        CheckState(EOomParsingStateAppCloseSettings, KOomErrForegroundAppRuleOutsideAppCloseElement);
        SetForegroundAppPriorityL(aAttributes);
        }
    else if (aLocalName == KOomAttributeSystemPlugin)
        {
        SetSystemPluginConfigL(aAttributes);
        }
    else if (aLocalName == KOomAttributeAppPlugin)
        {
        SetAppPluginConfigL(aAttributes);
        }
    else if (aLocalName == KOomAttributePluginIdlePriority)
        {
        CheckState(EOomParsingStateAppPluginSettings, KOomErrPluginIdleRuleOutsideAppPluginElement);
        SetPluginIdlePriorityL(aAttributes);
        }
    else if (aLocalName == KOomAttributePluginForegroundAppPriority)
        {
        CheckState(EOomParsingStateAppPluginSettings, EOomParsingStateSystemPluginSettings, KOomErrPluginForegroundRuleOutsidePluginElement);
        SetPluginForegroundAppPriorityL(aAttributes);
        }
    
    }

void COomConfigParser::ConfigError(TInt aError)
    {
    OomConfigParserPanic(aError);
    }

void COomConfigParser::SetGlobalSettings(const RAttributeArray& aAttributes)
    {
    TInt defaultLowMemoryThreshold;    
    TInt err = GetValueFromDecimalAttributeList(aAttributes, KOomAttributeLowRamThreshold, defaultLowMemoryThreshold);

    if (err == KErrNone)
        iConfig.SetDefaultLowRamThreshold(defaultLowMemoryThreshold * KBytesInMegabyte);
    else
        ConfigError(KOomErrLowRamErrorInGlobalSettings);
    
    if (err == KErrNone)
        {
        TInt defaultGoodMemoryThreshold;    
        TInt err = GetValueFromDecimalAttributeList(aAttributes, KOomAttributeGoodRamThreshold, defaultGoodMemoryThreshold);

        if (err == KErrNone)
            iConfig.SetDefaultGoodRamThreshold(defaultGoodMemoryThreshold * KBytesInMegabyte);
        else
            ConfigError(KOomErrGoodRamErrorInGlobalSettings);
        }

    if (err == KErrNone)
        {
        TInt swapUsageMonitored;
        TInt err = GetValueFromBooleanAttributeList(aAttributes, KOomAttributeSwapUsageMonitored, swapUsageMonitored);
        
        if (err == KErrNone)
            iConfig.SetSwapUsageMonitored(swapUsageMonitored);
        else
            ConfigError(KOomErrSwapUsageMonitoredErrorInGlobalSettings);
        }
    
    if (err == KErrNone)
        {
        TInt defaultLowSwapThreshold;    
        TInt err = GetValueFromDecimalAttributeList(aAttributes, KOomAttributeLowSwapThreshold, defaultLowSwapThreshold);

        if (err == KErrNone)
            iConfig.SetDefaultLowSwapThreshold(defaultLowSwapThreshold * KBytesInMegabyte);
        else
            ConfigError(KOomErrLowSwapErrorInGlobalSettings);
        }

    if (err == KErrNone)
        {
        TInt defaultGoodSwapThreshold;    
        TInt err = GetValueFromDecimalAttributeList(aAttributes, KOomAttributeGoodSwapThreshold, defaultGoodSwapThreshold);

        if (err == KErrNone)
            iConfig.SetDefaultGoodSwapThreshold(defaultGoodSwapThreshold * KBytesInMegabyte);
        else
            ConfigError(KOomErrGoodSwapErrorInGlobalSettings);
        }
    
    if (err == KErrNone)
        {
        TInt defaultMaxCloseAppBatch;    
        TInt err = GetValueFromDecimalAttributeList(aAttributes, KOomAttributeMaxAppCloseBatch, defaultMaxCloseAppBatch);

        if (err == KErrNone)
            iConfig.SetMaxCloseAppBatch(defaultMaxCloseAppBatch);
        else
            ConfigError(KOomErrMaxCloseErrorInGlobalSettings);
        }
    
    if (err == KErrNone)
        {
        TInt defaultWaitAfterPlugin;    
        TInt err = GetValueFromDecimalAttributeList(aAttributes, KOomAttributeDefaultWaitAfterPlugin, defaultWaitAfterPlugin);

        if (err == KErrNone)
            iConfig.SetDefaultWaitAfterPlugin(defaultWaitAfterPlugin);
        else
            ConfigError(KOomErrDefaultWaitAfterPluginInGlobalSettings);
        }
    
    if (err == KErrNone)
        {
        TInt maxAppExitTime;    
        TInt err = GetValueFromDecimalAttributeList(aAttributes, KOomAttributeMaxAppExitTime, maxAppExitTime);

        if (err == KErrNone)
            iConfig.SetMaxAppExitTime(maxAppExitTime);
        else
            iConfig.SetMaxAppExitTime(KOomMaxAppExitTime);
        }
    }

void COomConfigParser::SetForceCheckConfigL(const RAttributeArray& aAttributes)
    {
    TUint priority;
    TInt err = GetValueFromDecimalAttributeList(aAttributes, KOomAttibutePriority, priority);
    if (err == KErrNone)
        {
        iConfig.GlobalConfig().AddForceCheckPriorityL(priority);
        }
    else
        {
        ConfigError(KOomErrBadOrMissingPriorityInForceCheck);    
        }
    }

void COomConfigParser::SetAppConfigL(const RAttributeArray& aAttributes)
    {
    TUint uid;
    COomApplicationConfig* appConfig = NULL;
    
    TInt err = GetValueFromHexAttributeList(aAttributes, KOomAttibuteUid, uid);
    
    if (err != KErrNone)
        {
        ConfigError(KOomErrBadOrMissingUidInAppConfig);
        }
    else
        iParentUid = uid;

    appConfig = COomApplicationConfig::NewL(uid);
    CleanupStack::PushL(appConfig);    
        
    // Set the app specific memory thresholds (if they exist)
    // Get the app specific low threshold
    if (err == KErrNone)
        {
        TUint lowThreshold;
        err = GetValueFromDecimalAttributeList(aAttributes, KOomAttributeLowRamThreshold, lowThreshold);
        if (err == KErrNone)
            {
            appConfig->iLowRamThreshold = lowThreshold * KBytesInMegabyte;
            }
        else if (err == KErrNotFound)
            err = KErrNone;
        
        if (err != KErrNone)
            ConfigError(KOomErrBadLowThresholdValueForAppConfig);
        }

    // Get the app specific good threshold
    if (err == KErrNone)
        {
        TUint goodThreshold;
        err = GetValueFromDecimalAttributeList(aAttributes, KOomAttributeGoodRamThreshold, goodThreshold);
        if (err == KErrNone)
            {
            appConfig->iGoodRamThreshold = goodThreshold * KBytesInMegabyte;
            }
        else if (err == KErrNotFound)
            err = KErrNone;
        
        if (err != KErrNone)
            ConfigError(KOomErrBadGoodThresholdValueForAppConfig);
        }
    
    // Set the app specific swap thresholds (if they exist)
    // Get the app specific low swap threshold
    if (err == KErrNone)
        {
        TUint lowThreshold;
        err = GetValueFromDecimalAttributeList(aAttributes, KOomAttributeLowSwapThreshold, lowThreshold);
        if (err == KErrNone)
            {
            appConfig->iLowSwapThreshold = lowThreshold * KBytesInMegabyte;
            }
        else if (err == KErrNotFound)
            err = KErrNone;
            
        if (err != KErrNone)
            ConfigError(KOomErrBadLowThresholdValueForAppConfig);
        }

    // Get the app specific good swapthreshold
    if (err == KErrNone)
        {
        TUint goodThreshold;
        err = GetValueFromDecimalAttributeList(aAttributes, KOomAttributeGoodSwapThreshold, goodThreshold);
        if (err == KErrNone)
            {
            appConfig->iGoodSwapThreshold = goodThreshold * KBytesInMegabyte;
            }
        else if (err == KErrNotFound)
            err = KErrNone;
            
        if (err != KErrNone)
            ConfigError(KOomErrBadGoodThresholdValueForAppConfig);
        }  
        
    // Add the applciation config to the main config
    if ((err == KErrNone) && (appConfig))
        {
        iConfig.AddApplicationConfigL(appConfig);
        }
    
    if (appConfig)
        CleanupStack::Pop(appConfig);
    }

void COomConfigParser::SetCloseAppConfigL(const RAttributeArray& aAttributes)
    {
    // Get and convert uid attribute to TInt
    TInt err = KErrNone;
    
    TUint uid;
    err = GetValueFromHexAttributeList(aAttributes, KOomAttibuteUid, uid);

    if (err != KErrNone)
        {
        ConfigError(KOomErrBadOrMissingUidInAppCloseConfig);    
        return;
        }
    else
        iParentUid = uid;
    
    COomCloseAppConfig* closeAppConfig = COomCloseAppConfig::NewL(uid); // Radio UID
    CleanupStack::PushL(closeAppConfig);

     if (err == KErrNone)
        {
        // Check that we have a priority for the added app_close event
        // Specifying a priority is mandatory
        TUint priority;
        err = GetValueFromDecimalAttributeList(aAttributes, KOomAttibutePriority, priority);
        if (err == KErrNone)
            {
            closeAppConfig->SetDefaultPriority(priority);
            }
        else
            {
            ConfigError(KOomErrBadOrMissingPriorityInAppCloseConfig);    
            }
        }
    
    if (err == KErrNone)
        {
        TPtrC8 syncModeString;
        err = GetValueFromAttributeList(aAttributes, KOomAttibuteSyncMode, syncModeString);
        
        if (err == KErrNone)
            {
            TOomSyncMode syncMode = EContinue;
            
            if (syncModeString == KOomConfigSyncModeContinue)
                syncMode = EContinue;
            else if (syncModeString == KOomConfigSyncModeCheck)
                syncMode = ECheckRam;
            else if (syncModeString == KOomConfigSyncModeEstimate)
                syncMode = EEstimate;
            else
                ConfigError(KOomErrInvalidSyncMode);
            
            if (err == KErrNone)
                {
                closeAppConfig->iSyncMode = syncMode;
                }
            }
        else
            {
            ConfigError(KOomErrMissingSyncModeInAppCloseConfig);
            }
        }
    
    
    if (err == KErrNone)
        {
        // If we have a default priority attribute then add it, otherwise use the global default priority
        TInt ramEstimate;
        err = GetValueFromDecimalAttributeList(aAttributes, KOomAttibuteRamEstimate, ramEstimate);
        if ((err == KErrNotFound) && (closeAppConfig->iSyncMode != EEstimate))
            {
            err = KErrNone;
            }
        
        if (err != KErrNone)
            ConfigError(KOomErrMissingEstimateFromAppCloseConfig);
        else
            closeAppConfig->iRamEstimate = ramEstimate * KBytesInMegabyte;
           }

    if (err == KErrNone)
        iConfig.SetAppCloseConfigL(closeAppConfig);
    
    CleanupStack::Pop(closeAppConfig);
    }

void COomConfigParser::SetAppCloseIdlePriorityConfigL(const RAttributeArray& aAttributes)
    {
    TUint uid;
    TInt idleTime;
    TUint priority;

    // Use the UID from the parent scope
    uid = iParentUid;

    TInt err = KErrNone;
    
    err = GetValueFromDecimalAttributeList(aAttributes, KOomAttibuteIdleTime, idleTime);
    
#ifdef __WINS__
    // The tick is 5 times slower on the emulator than on the phone
    idleTime = idleTime / KEmulatorTickDivisor;
#endif
    
    if (err != KErrNone)
        {
        ConfigError(KOomErrBadOrMissingIdleTimeInAppIdleRule);
        }
    
    if (err == KErrNone)
        {
        err = GetValueFromDecimalAttributeList(aAttributes, KOomAttibuteIdlePriority, priority);
        
        if (err != KErrNone)
            {
            ConfigError(KOomErrBadOrMissingPriorityInAppIdleRule);
            }
        }
    
    if (err == KErrNone)
        {
        COomIdleTimeRule* idleRule = COomIdleTimeRule::NewL(idleTime, priority);
        CleanupStack::PushL(idleRule);
        iConfig.AddApplicationRuleL(uid, idleRule);
        CleanupStack::Pop(idleRule);
        }
    }

void COomConfigParser::SetForegroundAppPriorityL(const RAttributeArray& aAttributes)
    {
    TUint appUid;
    TUint targetAppId;
    TUint priority;

    TInt err = KErrNone;
    
    // Use the UID from the parent scope
    appUid = iParentUid;

    // Check that we have a priority for the added system plugin action
    // Specifying a priority is mandatory
    err = GetValueFromHexAttributeList(aAttributes, KOomAttributeIfForegroundAppId, targetAppId);
    if (err != KErrNone)
        {
        ConfigError(KOomErrBadOrMissingTargetAppIdInForegroundAppRule);    
        }
    
    if (err == KErrNone)
        {
        err = GetValueFromDecimalAttributeList(aAttributes, KOomAttibuteIdlePriority, priority);
        
        if (err != KErrNone)
            {
            ConfigError(KOomErrBadOrMissingPriorityInForegroundAppRule);
            }
        }
    
    if (err == KErrNone)
        {
        COomForegroundRule* foregroundRule = new (ELeave) COomForegroundRule(targetAppId, priority);
        CleanupStack::PushL(foregroundRule);
        iConfig.AddApplicationRuleL(appUid, foregroundRule);
        CleanupStack::Pop(foregroundRule);
        }

    }

void COomConfigParser::SetSystemPluginConfigL(const RAttributeArray& aAttributes)
    {
    // Get and convert uid attribute to TInt
    TInt err = KErrNone;
    
    TUint uid;
    err = GetValueFromHexAttributeList(aAttributes, KOomAttibuteUid, uid);

    if (err != KErrNone)
        {
        ConfigError(KOomErrBadOrMissingUidInSystemPluginConfig);    
        return;
        }
    else
        iParentUid = uid;
    
    COomRunPluginConfig* pluginConfig = COomRunPluginConfig::NewL(uid, EOomSystemPlugin);
    CleanupStack::PushL(pluginConfig);

     if (err == KErrNone)
        {
        // Check that we have a priority for the added system plugin action
        // Specifying a priority is mandatory
        TUint priority;
        err = GetValueFromDecimalAttributeList(aAttributes, KOomAttibutePriority, priority);
        if (err == KErrNone)
            {
            pluginConfig->SetDefaultPriority(priority);
            }
        else
            {
            ConfigError(KOomErrBadOrMissingPriorityInSystemPluginConfig);    
            }
        }
     
     if (err == KErrNone)
        {
        TInt wait;
        err = GetValueFromDecimalAttributeList(aAttributes, KOomAttributeWait, wait);
        if (err == KErrNone)
            {
            pluginConfig->SetWaitAfterPlugin(wait);
            }
        else if (err == KErrNotFound)
            {
            // If this attribute isn't present then just don't set it, and clear the error
            err = KErrNone;
            }
        else
            ConfigError(KOomErrBadPluginWaitTime);
        }
     
     if (err == KErrNone)
         {
         // Get the config for the sync mode for this plugin (if one is specified) and set it
         SetPluginSyncMode(aAttributes, *pluginConfig);
         }

     iConfig.AddPluginConfigL(pluginConfig);
     
     CleanupStack::Pop(pluginConfig);
    }

void COomConfigParser::SetAppPluginConfigL(const RAttributeArray& aAttributes)
    {
    // Get and convert uid attribute to TInt
    TInt err = KErrNone;
    
    TUint uid;
    err = GetValueFromHexAttributeList(aAttributes, KOomAttibuteUid, uid);

    if (err != KErrNone)
        {
        ConfigError(KOomErrBadOrMissingUidInAppPluginConfig);    
        return;
        }
    else
        iParentUid = uid;
    
    COomRunPluginConfig* pluginConfig = COomRunPluginConfig::NewL(uid, EOomAppPlugin);
    CleanupStack::PushL(pluginConfig);

    if (err == KErrNone)
       {
       // Check that we have a priority for the added system plugin action
       // Specifying a priority is mandatory
        TUint priority;
        err = GetValueFromDecimalAttributeList(aAttributes, KOomAttibutePriority, priority);
        if (err == KErrNone)
            {
            pluginConfig->SetDefaultPriority(priority);
            }
        else
            {
            ConfigError(KOomErrBadOrMissingPriorityInAppPluginConfig);    
            }
       }

    if (err == KErrNone)
       {
       // Check that we have a priority for the added system plugin action
       // Specifying a priority is mandatory
        TUint targetAppId;
        err = GetValueFromHexAttributeList(aAttributes, KOomAttributeTargetAppId, targetAppId);
        if (err == KErrNone)
            {
            pluginConfig->SetTargetApp(targetAppId);
            iParentTargetApp = targetAppId;
            }
        else
            {
            ConfigError(KOomErrBadOrMissingTargetAppIdInAppPluginConfig);    
            }
       }
    
    if (err == KErrNone)
       {
       TInt wait;
        err = GetValueFromDecimalAttributeList(aAttributes, KOomAttributeWait, wait);
        if (err == KErrNone)
            {
            pluginConfig->SetWaitAfterPlugin(wait);
            }
        else if (err == KErrNotFound)
            {
            // If this attribute isn't present then just don't set it, and clear the error
            err = KErrNone;
            }
        else
            ConfigError(KOomErrBadPluginWaitTime);
       }    
    
    if (err == KErrNone)
        {
        // Get the config for the sync mode for this plugin (if one is specified) and set it
        SetPluginSyncMode(aAttributes, *pluginConfig);
        
        TBool targetAppNotRunning;
        err = GetValueFromBooleanAttributeList(aAttributes, KOomAttributeCallIfTargetAppNotRunning, targetAppNotRunning);
        if (err == KErrNone)
            {
            pluginConfig->SetCallIfTargetAppNotRunning(targetAppNotRunning);
            }
        else if (err == KErrNotFound)
            {
            // If this attribute isn't present then just don't set it, and clear the error
            err = KErrNone;
            }
        else
            {
            ConfigError(KOomErrBadCallIfTargetAppNotRunning);
            }
        }

     iConfig.AddPluginConfigL(pluginConfig);
     
     CleanupStack::Pop(pluginConfig);

    }

void COomConfigParser::SetPluginIdlePriorityL(const RAttributeArray& aAttributes)
    {
    TUint uid;
    TInt idleTime;
    TUint priority;

    TInt err = KErrNone;
    
    // Use the UID from the parent scope
    uid = iParentUid;

    err = GetValueFromDecimalAttributeList(aAttributes, KOomAttibuteIdleTime, idleTime);
    
#ifdef __WINS__
    // The tick is 5 times slower on the emulator than on the phone
    idleTime = idleTime / KEmulatorTickDivisor;
#endif
    
    if (err != KErrNone)
        {
        ConfigError(KOomErrBadOrMissingIdleTimeInPluginIdleRule);
        }
    
    if (err == KErrNone)
        {
        err = GetValueFromDecimalAttributeList(aAttributes, KOomAttibuteIdlePriority, priority);
        
        if (err != KErrNone)
            {
            ConfigError(KOomErrBadOrMissingPriorityInPluginIdleRule);
            }
        }
    
    if (err == KErrNone)
        {
        COomIdleTimeRule* idleRule = COomIdleTimeRule::NewL(idleTime, priority);
        CleanupStack::PushL(idleRule);
        iConfig.AddPluginRuleL(uid, idleRule);
        CleanupStack::Pop(idleRule);
        }
    }

void COomConfigParser::SetPluginForegroundAppPriorityL(const RAttributeArray& aAttributes)
    {
    TUint uid;
    TUint targetAppId;
    TUint priority;

    TInt err = KErrNone;
    
    // Use the UID from the parent scope
    uid = iParentUid;

        // Check that we have a priority for the added system plugin action
        // Specifying a priority is mandatory
        
    TPtrC8 targetAppString;
    err = GetValueFromAttributeList(aAttributes, KOomAttributeTargetAppId, targetAppString);
    if ((err == KErrNone)
            && (targetAppString == KOomConfigTargetAppValue)
            && (iState == EOomParsingStateAppPluginSettings))
        // If the target app is specified as "TARGET_APP" then we use the target app from the parent entry
        {
        targetAppId = iParentTargetApp;
        }
    else
        {
        err = GetValueFromHexAttributeList(aAttributes, KOomAttributeTargetAppId, targetAppId);
        if (err != KErrNone)
            {
            ConfigError(KOomErrBadOrMissingTargetAppIdInForegroundAppRule);    
            }
        }
     
    if (err == KErrNone)
        {
        err = GetValueFromDecimalAttributeList(aAttributes, KOomAttibuteIdlePriority, priority);
        
        if (err != KErrNone)
            {
            ConfigError(KOomErrBadOrMissingPriorityInForegroundAppRule);
            }
        }
    
    if (err == KErrNone)
        {
        COomForegroundRule* foregroundRule = new (ELeave) COomForegroundRule(targetAppId, priority);
        CleanupStack::PushL(foregroundRule);
        iConfig.AddPluginRuleL(uid, foregroundRule);
        CleanupStack::Pop(foregroundRule);
        }
    }

// Finds an attribute of the given name and gets its value
// A value is only valid as long as AAtrributes is valid (and unmodified)
// Returns KErrNone if the attribute is present in the list, KErrNotFound otherwise
TInt COomConfigParser::GetValueFromAttributeList(const RAttributeArray& aAttributes, const TDesC8& aName, TPtrC8& aValue)
    {
    TInt index = aAttributes.Count();
    TBool attributeFound = EFalse;
    while ((index--) && (!attributeFound))
        {
        if (aAttributes[index].Attribute().LocalName().DesC() == aName)
            {
            attributeFound = ETrue;
            aValue.Set(aAttributes[index].Value().DesC());
            }
        }
    
    TInt err = KErrNone;
    
    if (!attributeFound)
        err = KErrNotFound;
    
    return err;
    }

// Finds an attribute of the given name and gets its value (coverting the string hex value to a UInt)
// Returns KErrNone if the attribute is present in the list, KErrNotFound otherwise
// Returns KErrCorrupt if the string is not a valid hex number
TInt COomConfigParser::GetValueFromHexAttributeList(const RAttributeArray& aAttributes, const TDesC8& aName, TUint& aValue)
    {
    TPtrC8 hexString;
    TInt err = GetValueFromAttributeList(aAttributes, aName, hexString);

    if (hexString == KOomConfigDefaultAppUid)
        {
        // This is a special case
        // When we hit this value in a hex field then we return the default app UID
        aValue = KOomDefaultAppId;
        }
    else if (hexString == KOomConfigDefaultPluginUid)
        {
        // This is a special case
        // When we hit this value in a hex field then we return the default app UID
        aValue = KOomDefaultPluginId;
        }
    else if (hexString == KOomConfigBusyAppUid)
        {
        aValue = KOomBusyAppId;
        }
    else if (hexString == KOomConfigHighPriorityAppUid)
        {
        aValue = KOomHighPriorityAppId;
        }
    else if (err == KErrNone)
        {
        TLex8 hexLex(hexString);
        err = hexLex.Val(aValue, EHex);
        if (err != KErrNone)
            err = KErrCorrupt;
        }
    
    return err;
    }

// Finds an attribute of the given name and gets its value (coverting the string decimal value to a UInt)
// Returns KErrNone if the attribute is present in the list, KErrNotFound otherwise
// Returns KErrCorrupt if the string is not a valid decimal number
TInt COomConfigParser::GetValueFromDecimalAttributeList(const RAttributeArray& aAttributes, const TDesC8& aName, TUint& aValue)
    {
    TPtrC8 decimalString;
    TInt err = GetValueFromAttributeList(aAttributes, aName, decimalString);

    if (err == KErrNone)
        {
        if (decimalString == KOomAttibuteNeverClose)
            aValue = KOomPriorityInfinate;
        else
            {
            TLex8 decimalLex(decimalString);
            err = decimalLex.Val(aValue, EDecimal);
            if (err != KErrNone)
                err = KErrCorrupt;
            }
        }
    
    return err;
    }

TInt COomConfigParser::GetValueFromDecimalAttributeList(const RAttributeArray& aAttributes, const TDesC8& aName, TInt& aValue)
    {
    TUint uintValue;
    TInt err = GetValueFromDecimalAttributeList(aAttributes, aName, uintValue);
    aValue = uintValue;
    return err;
    }

TInt COomConfigParser::GetValueFromBooleanAttributeList(const RAttributeArray& aAttributes, const TDesC8& aName, TBool& aValue)
    {
    TPtrC8 ptrValue;
    TInt err = GetValueFromAttributeList(aAttributes, aName, ptrValue);
    if (err == KErrNone)
        {
        if (ptrValue == KOomAttributeTrue || ptrValue == KOomAttribute1)
            {
            aValue = ETrue;
            }
        else if (ptrValue == KOomAttributeFalse || ptrValue == KOomAttribute0)
            {
            aValue = EFalse;
            }
        else
            {
            err = KErrCorrupt;
            }
        }
    return err;
    }

void COomConfigParser::SetPluginSyncMode(const RAttributeArray& aAttributes, COomRunPluginConfig& aRunPluginConfig)
    {
    TPtrC8 syncModeString;
    TInt err = GetValueFromAttributeList(aAttributes, KOomAttibuteSyncMode, syncModeString);
    
    if (err == KErrNone)
        // If there is no specified sync mode then leave it as the default
        {
        TOomSyncMode syncMode = EContinue;
        
        if (syncModeString == KOomConfigSyncModeContinue)
            syncMode = EContinueIgnoreMaxBatchSize;
        else if (syncModeString == KOomConfigSyncModeCheck)
            syncMode = ECheckRam;
        else if (syncModeString == KOomConfigSyncModeEstimate)
            syncMode = EEstimate;
        else
            ConfigError(KOomErrInvalidSyncMode);
        
        if (err == KErrNone)
            {
            aRunPluginConfig.iSyncMode = syncMode;
            }
        }
    }

// Check that the current state is as expected
// If not then the specified config error is generated
void COomConfigParser::CheckState(TOomParsingState aExpectedState, TInt aError)
    {
    if (iState != aExpectedState)
        ConfigError(aError);
    }

// Check that the current state is as expected
// If not then the specified config error is generated
// This version checks to ensure that the current state matches either of the passed in states
void COomConfigParser::CheckState(TOomParsingState aExpectedState1, TOomParsingState aExpectedState2, TInt aError)
    {
    if ((iState != aExpectedState1)
            && (iState != aExpectedState2))
        ConfigError(aError);
    }

void COomConfigParser::ChangeState(TOomParsingState aState)
    {
    iState = aState;
    }
