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
* Description:  Configuration representation classes for Out of Memory Monitor.
*
*/


#ifndef OOMCONFIG_
#define OOMCONFIG_

#include <e32base.h>

#include "oomglobalconfig.h"

class COomCloseAppConfig;
class COomRunPluginConfig;
class MOomRuleConfig;
class COomApplicationConfig;

/*
 * A class representing the entire configuration.
 *
 *  @lib oommonitor.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(COomConfig) : public CBase
    {
public:
    
    static COomConfig* NewL();
    
// Functions for setting configuration  
    
    // Add the configuration for an application closure.
    // This class takes ownership of this configuration object.
    void SetAppCloseConfigL(COomCloseAppConfig* aActionConfig);
    
    // Add the configuration for a plugin action.
    // This class takes ownership of the configuration object.
    void AddPluginConfigL(COomRunPluginConfig* aPluginConfig);
    
    // Add a rule for an application
    // This class takes ownership of the given rule
    // This rule applies to the specified application (and not a plugin associated with this application)
    // The rule would usually apply to an "application close" event
    void AddApplicationRuleL(TUint aTargetAppId, MOomRuleConfig* aRule);
    
    // Add a rule for a plugin
    // This class takes ownership of the given rule
    // This rule is applied to the plugin with the specified ID
    void AddPluginRuleL(TUint aPluginId, MOomRuleConfig* aRule);    
    
    // Add this application config - this class takes ownership of it
    // Application config includes settings for a particular application, e.g. whether or not it can be closed
    void AddApplicationConfigL(COomApplicationConfig* aApplicationConfig);
    
// Functions for getting configuration  
    
    // Get the application configuration for the given app id
    // If no specific actions have been configured for this application then the default application configuration is returned
    COomApplicationConfig& GetApplicationConfig(TInt32 aAppId);
    
    // Get the plugin configuration for the given plugin id
    // If no specific actions have been configured for this plugin then the default plugin configuration is returned
    COomRunPluginConfig& GetPluginConfig(TInt32 aPluginId);
        
// Functions for setting global configuration   
    
    ~COomConfig();
    
    inline COomGlobalConfig& GlobalConfig();
    
    inline void SetDefaultLowRamThreshold(TInt aLowRamThreshold);
    inline void SetDefaultGoodRamThreshold(TInt aGoodRamThreshold);
    inline void SetSwapUsageMonitored(TBool aMonitored);
    inline void SetDefaultLowSwapThreshold(TInt aLowSwapThreshold);
    inline void SetDefaultGoodSwapThreshold(TInt aGoodSwapThreshold);
    inline void SetMaxCloseAppBatch(TUint MaxCloseAppBatch);
    inline void SetDefaultWaitAfterPlugin(TInt aMilliseconds);
    inline void SetMaxAppExitTime(TInt aMilliseconds);
    
private:
    void ConstructL();
    
    RHashMap<TInt32, COomApplicationConfig*> iApplicationToConfigMapping; // A hash-map of application configs, keyed on the application ID
    
    RHashMap<TInt32, COomRunPluginConfig*> iPluginToConfigMapping; // A hash-map of plug-in configs, keyed on the plugin ID

    COomGlobalConfig iGlobalConfig;
    };

#include "oomconfig.inl"

#endif /*OOMCONFIG_*/
