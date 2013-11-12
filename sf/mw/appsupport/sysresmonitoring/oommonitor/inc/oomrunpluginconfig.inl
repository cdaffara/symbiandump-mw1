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


#ifndef OOMRUNPLUGINCONFIGINL_H_
#define OOMRUNPLUGINCONFIGINL_H_


// Returns ETrue if a wait period has been explicitly configured for this plugin
inline TBool COomRunPluginConfig::WaitAfterPluginDefined() const
    {
    return iWaitAfterPluginDefined;
    }

// Returns the configured wait after the plugin has been called
inline TInt COomRunPluginConfig::WaitAfterPlugin() const
    {
    return iWaitAfterPlugin;
    }

inline TBool COomRunPluginConfig::CallIfTargetAppNotRunning() const
    {
    return iCallIfTargetAppNotRunning;
    }

// Set the time to wait 
inline void COomRunPluginConfig::SetWaitAfterPlugin(TInt aMillisecondsToWait)
    {
    iWaitAfterPluginDefined = ETrue;
    iWaitAfterPlugin = aMillisecondsToWait;
    }

inline void COomRunPluginConfig::SetCallIfTargetAppNotRunning(TBool aCallIfTargetAppNotRunning)
    {
    iCallIfTargetAppNotRunning = aCallIfTargetAppNotRunning;
    }

inline void COomRunPluginConfig::SetTargetApp(TUint aTargetAppId)
    {
    iTargetAppId = aTargetAppId;
    }

inline TUint COomRunPluginConfig::Id()
    {
    return  iPluginId;
    }

inline TOomPluginType COomRunPluginConfig::PluginType()
    {
    return iPluginType;
    }

inline TUint COomRunPluginConfig::TargetApp() const
    {
    return iTargetAppId;
    }

#endif /*OOMRUNPLUGINCONFIGINL_H_*/
