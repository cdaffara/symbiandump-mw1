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
* Description:  OOM Monitor plug-in interface.
*
*/


#ifndef OOMMONITORPLUGIN_H
#define OOMMONITORPLUGIN_H

#include <e32base.h>

class CMemoryMonitor;
class RFs;
class RWsSession;


/**
* Panic codes with category "OomMonitorPlugin"
*/
enum TOomMonitorPluginPanic
    {
    /**
    * The plugin has been constructed by a thread other than the OOM monitor.
    */
    EOomMonitorPluginPanic_PluginConstructedOutsideOomMonitorThread
    };

// Message type used to signal an OOM plug-in to free some memory.
const TInt KAppOomMonitor_FreeRam = 0x10282304;

// Message type used to signal an OOM plug-in that there is free memory again.
const TInt KAppOomMonitor_MemoryGood = 0x10282305;

class COomMonitorPlugin : public CBase
	{
public:
	/**
	* Constructor
	* Plugins should assume that the system memory is good when
	* they are created.
	*/
	IMPORT_C COomMonitorPlugin();
	/**
	* Destructor
	*/
	IMPORT_C ~COomMonitorPlugin();
	/**
	* 2nd stage constructor
	*/
	IMPORT_C void ConstructL();

public:
	/**
	* FreeRam is called when the system RAM level becomes
	* low. This plugin is requested to help free some RAM.
	*/
	virtual void FreeRam() = 0;
	
	/**
	* MemoryGood is called when the system RAM level becomes
	* good after being low.The plugin may take this opportunity
	* to start using RAM again.
	* Nb It is assumed that the plugin will not immediately
	* cause a large increase in memory use, but that memory may be
	* used over time, otherwise the plugin may cause oscillation
	* between low and good memory states.
	*/
	virtual void MemoryGood() = 0;

public:
    /**
    * Get a handle to the file server.
    * @return a connected file server session.
    */
    IMPORT_C RFs& FsSession();
    /**
    * Get a handle to the window server.
    * @return a connected window server session.
    */
    IMPORT_C RWsSession& WsSession();

protected:
	IMPORT_C virtual void ExtensionInterface(TUid aInterfaceId, TAny*& aImplementaion);

private:
	CMemoryMonitor* iMemoryMonitor;
	};


class COomMonitorPluginV2 : public COomMonitorPlugin
	{
public:	// From COomMonitorPlugin
	/**
	* FreeRam is called when the system RAM level becomes
	* low. This plugin is requested to help free some RAM.
	* 
	* Note that OomMonitorV2 will not call this version of the function so it does not need to be implemented.
	*/
	IMPORT_C virtual void FreeRam();

	/**
	* MemoryGood is called when the system RAM level becomes
	* good after being low.The plugin may take this opportunity
	* to start using RAM again.
	* Nb It is assumed that the plugin will not immediately
	* cause a large increase in memory use, but that memory may be
	* used over time, otherwise the plugin may cause oscillation
	* between low and good memory states.
	*/
	virtual void MemoryGood() = 0;

public:	
	/**
	* FreeRam is called when the system RAM level becomes
	* low. This plugin is requested to help free some RAM.
	* Note that it is desirable to delete all memory associated with this plugin if
	* not using a disconnected chunk. The reason for this is that by only releasing a small amount
	* of the memory you reduce the chances of that memory being at the end of the chunk and therefore
	* reduce the likelihood of it being released to the system.
	* @param aBytesToFree The minimum number of bytes of free memory that the plugin should try to free.
	*/
	virtual void FreeRam(TInt aBytesToFree) = 0;

	};



/**
* CAppOomMonitorPlugin is a specialised OOM monitor plugin
* that sends messages to applications when the memory state
* changes.
* The target application is specified by the appUid parameter.
* the message will be sent to all running instances of this app.
* The messages are:
* KAppOomMonitor_FreeRam when memory should be released.
* KAppOomMonitor_MemoryGood when memory is good again.
*
* These messages can be picked up in the app UI like this:
* void CMyAppUi::HandleApplicationSpecificEventL(TInt aType, const TWsEvent& aEvent)
*     {
*     CAknAppUi::HandleApplicationSpecificEventL(aType, aEvent);
*     if (aType == KAppOomMonitor_FreeRam)
*         {
*         // release memory and disable memory use
*         }
*     else if (aType == KAppOomMonitor_MemoryGood)
*         {
*         // re-enable memory use
*         }
*     }
*
* The plugin can be instantiated in an ECom factory function.
* The application does not need to derive from this class.
* For example:
* TAny* CreateAppPlugin()
*     {
*     return CAppOomMonitorPlugin::NewL(KMyAppUid);
*     }
*/
NONSHARABLE_CLASS(CAppOomMonitorPlugin) : public COomMonitorPlugin
    {
public:
    IMPORT_C static CAppOomMonitorPlugin* NewL(TUid aAppUid);
    
private:
	CAppOomMonitorPlugin(TUid aAppUid);

	void FreeRam();
	void MemoryGood();

    void SendMessageToApp(TInt aMessage);
	
private:
    TUid iAppUid;
    };


/**
* Sets the CMemoryMonitor object for this DLL.
* Only for use by CMemoryMonitor.
* @internal
*/
IMPORT_C void SetMemoryMonitorTls(CMemoryMonitor* aMonitor);


#endif // OOMMONITORPLUGIN_H
