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
* Description:  
*
*/




#ifndef DUMMYPLUGIN_H_
#define DUMMYPLUGIN_H_

#include <oommonitorplugin.h>
#include "t_oomclient.h"

class CDummyOomPlugin : public COomMonitorPlugin
	{
public:
	static CDummyOomPlugin* NewL(TInt aInstance);
	~CDummyOomPlugin();
private:
	CDummyOomPlugin(TInt aInstance);
	void ConstructL();
	//from COomMonitorPlugin
	virtual void FreeRam();
	virtual void MemoryGood();

private:
	TInt iInstance;
	TInt iLowMemoryCallCount;
	TInt iGoodMemoryCallCount;
	ROOMAllocSession iAllocServer;
	};


/*
class CDummyOomPluginV2 : public COomMonitorPluginV2
	{
public:
	static CDummyOomPluginV2* NewL(TInt aInstance);
	~CDummyOomPluginV2();
private:
	CDummyOomPluginV2(TInt aInstance);
	void ConstructL();
	//from COomMonitorPlugin
	virtual void FreeRam(TInt aBytesRequested);
	virtual void MemoryGood();

private:
	TInt iInstance;
	TInt iLowMemoryCallCount;
	TInt iGoodMemoryCallCount;
	ROOMAllocSession iAllocServer;
	};
*/


#endif
