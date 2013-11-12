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
* Description:  Classes for executing OOM actions (e.g. closing applications and running plugins).
*
*/


#ifndef OOMRUNPLUGIN_H_
#define OOMRUNPLUGIN_H_

#include "oomaction.h"

class COomRunPlugin;
class COomRunPluginConfig;
class MOomActionObserver;
class COomPluginWaiter;
class COomMonitorPlugin;
class COomMonitorPluginV2;
class COomActionConfig;

/*
 * The OOM action of running an OOM plug-in to free up memory.
 * 
 *  @lib oommonitor.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(COomRunPlugin) : public COomAction
    {
public:
    static COomRunPlugin* NewL(TUint aPluginId, COomRunPluginConfig& aConfig, MOomActionObserver& aStateChangeObserver, COomMonitorPlugin& aPlugin, COomMonitorPluginV2* aV2Plugin = NULL);

    ~COomRunPlugin();   
    
    // Run the OOM plugin in order to free memory
    // Call the COomAction::MemoryFreed when it is done
    virtual void FreeMemory(TInt aBytesRequested, TBool aIsDataPaged);
    
    // Call the memory good function on the plugin but...
    // only if there is an outstanding FreeMemory request
    void MemoryGood();
        
    inline TBool IsRunning();
    
    // To be called by the COomPluginWaiter
    inline void WaitCompleted();
    
protected:
    
    void ConstructL(COomRunPluginConfig& aPluginConfig);    
    
    inline COomActionConfig& GetConfig();   
    
private:
    
    COomRunPlugin(TUint aPluginId, COomRunPluginConfig& aConfig, MOomActionObserver& aStateChangeObserver, COomMonitorPlugin& aPlugin, COomMonitorPluginV2* aV2Plugin);
    
    TUint iPluginId;
    
    COomMonitorPlugin& iPlugin;
    
    COomRunPluginConfig& iConfig;
    
    COomPluginWaiter* iPluginWaiter;
    
    // Note that this shouldn't be deleted
    // If it is pointing to a V2 plugin then it is a cast to the same instance as iPlugin
    COomMonitorPluginV2* iV2Plugin;
    
    TBool iFreeMemoryCalled; // True if FreeMemory has been called since the last call to MemoryGood
    };

#include "oomrunplugin.inl"

#endif /*OOMRUNPLUGIN_H_*/
