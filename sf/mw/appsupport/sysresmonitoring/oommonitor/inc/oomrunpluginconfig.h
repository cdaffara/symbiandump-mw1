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


#ifndef OOMRUNPLUGINCONFIG_
#define OOMRUNPLUGINCONFIG_

#include "oomactionconfig.h"

enum TOomPluginType
    {
    EOomAppPlugin,
    EOomSystemPlugin
    };

/*
 * The OOM action of running an OOM plug-in to free up memory.
 */
NONSHARABLE_CLASS(COomRunPluginConfig) : public COomActionConfig
    {
public:
    static COomRunPluginConfig* NewL(TUint aPluginId, TOomPluginType aPluginType);
    
    TUint CalculatePluginPriority(const COomWindowGroupList& aWindowGroupList);
    
    inline TUint Id();
    
    inline void SetTargetApp(TUint aTargetAppId);
    
    inline TUint TargetApp() const;
    
    ~COomRunPluginConfig();
    
    // Returns ETrue if a wait period has been explicitly configured for this plugin
    inline TBool WaitAfterPluginDefined() const;
    
    // Returns the configured wait after the plugin has been called
    inline TInt WaitAfterPlugin() const;
    
    inline TBool CallIfTargetAppNotRunning() const;
    
    // Set the time to wait 
    inline void SetWaitAfterPlugin(TInt aMillisecondsToWait);
    
    inline void SetCallIfTargetAppNotRunning(TBool aCallIfTargetAppNotRunning);
    
    inline TOomPluginType PluginType();
    
private:
    COomRunPluginConfig(TUint aPluginId, TOomPluginType aPluginType);
    
    TUint iPluginId;
    
    TUint iTargetAppId;
        
    TBool iWaitAfterPluginDefined;
    
    TInt iWaitAfterPlugin; // The period to wait after a plugin has been called
    
    TBool iCallIfTargetAppNotRunning; 
    
    TOomPluginType iPluginType;
    };

#include "oomrunpluginconfig.inl"

#endif /*OOMRUNPLUGINCONFIG_*/
