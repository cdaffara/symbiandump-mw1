// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#ifndef __APA_INTERNAL_H__
#define __APA_INTERNAL_H__

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#if !defined(__F32FILE_H__)
#include <f32file.h>
#endif

// classes referenced
class MApaAppStarter;

/** @internalComponent
*/
IMPORT_C TUint MinApplicationStackSize();

// comand line tokens


/**
@internalComponent
*/
const TInt KApaMaxCommandLine=0x100;	

/**
@internalComponent
*/
typedef TBuf<KApaMaxCommandLine> TApaCommandLine;

/**
@internalComponent
*/
const TInt KApaMaxAppFileName=0x10;	// Length of App's filename without path or extension (not it's caption)

/**
@internalComponent
*/
typedef TBuf<KApaMaxAppFileName> TApaAppFileName;

/**
@internalAll
*/
const TUid KUidApaDoorDocStream={0x10003A35};


/**
@internalComponent
@deprecated
*/
//const TUid KUidApaDoorDocStream8={0x10000144};

/**
@internalComponent
*/
//const TUid KUidApaDoorDocStream16={0x10003A35};

//gmahe from APASVST.H 
/**
@internalTechnology
@released
*/
IMPORT_C TPtrC NameApaServStartSemaphore();

/**
@internalTechnology
@released
*/
IMPORT_C TInt StartupApaServer(MApaAppStarter& aAppStarter);

/**
@internalTechnology
@released
*/
IMPORT_C TInt ApaServThreadStart(TAny* aUnused = NULL);


//gmahe..from ApaServerApp.h

/** Panic codes that the server application framework can generate.
@internalComponent */
enum TApaAppServerPanic
	{
	/** The IPC message ID used by the client is not recognised. */
	EApaAppServerPanicIllegalFunction,
	/** The client already has an active notification of server exit. */
	EApaAppServerPanicNotifyExitActive,
	/** The client has tried to connect an already connected session. */
	EApaAppServerPanicClientAlreadyConnected
	};

#if 0
class MApaFsChangeObserver
/** An interface for notifying a file system observer when
a change has been made to the file system.
This interface is intended for use only by CApaFsMonitor.

@internalComponent
*/
	{
public:
	virtual void FsChanged()=0;
	};
#endif

#if !defined(__WINC__)

class REComSession;

/** 
CApaEComMonitor

A low priority (EPriorityIdle) active object which monitors changes 
in the Ecom plugin.

@see REComSession::NotifyOnChange()

@internalAll 
*/ 

class CApaEComMonitor : public CActive
	{
public:
	 ~CApaEComMonitor();
	 static CApaEComMonitor* NewL(TCallBack aCallBack);
	 void Start();
private:
	CApaEComMonitor(TCallBack aCallBack);
	void ConstructL ();
	static TInt TimerCallBack(TAny* aObject);
	void DoStart();
	void DoCancel();
	void RunL();
private:
	TCallBack iCallBack;
	CPeriodic* iEComTimer;
	REComSession* iEComSession;
	TBool iEComHasChanged;
	};
#endif //__WINC_

#endif
