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
// apgstart.cpp
//

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#if !defined(__APA_INTERNAL_H__)
#include "apainternal.h"
#endif
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS
#include "../apserv/APSCLSV.H"
#include "APGCLI.H"
#include "APACMDLN.H"
#include "APGSTD.H"

#if defined(USE_IH_RAISE_EVENT)		
// For performance system test 
// see /common/testtools/systemmonitor/instrumentationhandler/inc/raiseevent.h
// and /common/generic/plattest/Group/SetEnv.bat
#include <systemmonitor/raiseevent.h>
#include <test/testinstrumentation.h>
#endif

/** Starts an application defined by the specified command line information.
    This is an asynchronous method which doesn't wait for the process creation to complete.
    To be informed of the process creation success, 
    then appropriate overloaded method taking a TRequestStatus parameter should be used.
    
This is only recommended for non document based applications.

View based applications are usually started by activating a specific view 
using CCoeAppUi::ActivateViewL. Alternatively, using StartApp() to start a 
view based application will activate the application's default view.

@param aCommandLine The command line.
@return KErrNone, if successful; KErrNotFound, if the application cannot be 
found; otherwise one of the other system-wide error codes. 
@see CCoeAppUi::ActivateViewL() 
*/
EXPORT_C TInt RApaLsSession::StartApp(const CApaCommandLine& aCommandLine)
	{
	return DoStartApp(aCommandLine, NULL,NULL);
	}
	
/** Starts an application defined by the specified command line information.
    This is an asynchronous method which doesn't wait for the process creation to complete.
    To be informed of the process creation success, 
    then appropriate overloaded method taking a TRequestStatus parameter should be used.

This is only recommended for non document based applications.

View based applications are usually started by activating a specific view 
using CCoeAppUi::ActivateViewL. Alternatively, using StartApp() to start a 
view based application will activate the application's default view.

@param aCommandLine The command line.
@param aThreadId On return, the id of the main thread started.
@return KErrNone, if successful; KErrNotFound, if the application cannot be 
found; otherwise one of the other system-wide error codes. 
@see CCoeAppUi::ActivateViewL() 
*/	
EXPORT_C TInt RApaLsSession::StartApp(const CApaCommandLine& aCommandLine,TThreadId& aThreadId)
	{
	return DoStartApp(aCommandLine, &aThreadId,NULL);
	}

	
/** Starts an application defined by the specified command line information.

This is only recommended for non document based applications.

View based applications are usually started by activating a specific view 
using CCoeAppUi::ActivateViewL. Alternatively, using StartApp() to start a 
view based application will activate the application's default view.

@param aCommandLine The command line.
@param aThreadId On return, the id of the main thread started.
@param aRequestStatusForRendezvous If not NULL, the asynchronous RProcess::Rendezvous() 
function is called (being passed this TRequestStatus object) before RProcess::Resume() is called on 
the new application process. If this function does not return KErrNone, RProcess::Rendezvous() will
not be called passing aRequestStatusForRendezvous, so in this case the caller must not wait
on aRequestStatusForRendezvous.
@return KErrNone, if successful; KErrNotFound, if the application cannot be 
found; otherwise one of the other system-wide error codes. 
@see CCoeAppUi::ActivateViewL() 
*/
EXPORT_C TInt RApaLsSession::StartApp(const CApaCommandLine& aCommandLine,TThreadId& aThreadId,TRequestStatus* aRequestStatusForRendezvous )
	{
	return DoStartApp(aCommandLine, &aThreadId,aRequestStatusForRendezvous);
	}


TInt RApaLsSession::DoStartApp(const CApaCommandLine& aCommandLine, TThreadId* aThreadId,TRequestStatus* aRequestStatusForRendezvous)
	{
	TRAPD(error, DoStartAppL(aCommandLine, aThreadId, aRequestStatusForRendezvous));
	return error;
	}

void RApaLsSession::DoStartAppL(const CApaCommandLine& aCommandLine, TThreadId* aThreadId,TRequestStatus* aRequestStatusForRendezvous)
	{
	// This function does not require "this" object to be connected to the Apparc server,
	// but if not, it works with some limitations (see the following document for a list
	// of these limitations: 
	// generic/app-framework/Documentation/PREQ967_solution_constraints.doc).

#if defined(USE_IH_RAISE_EVENT)
	const TInt appStartupInstrumentationEventIdBase=aCommandLine.AppStartupInstrumentationEventIdBase();
	if (appStartupInstrumentationEventIdBase!=0)
		{
		IH_DECLARE( lInstrumentationHandler );
		IH_CREATE( lInstrumentationHandler );
		IH_RAISE_EVENT( lInstrumentationHandler, appStartupInstrumentationEventIdBase+MTestInstrumentation::TIDOffsetBeginApplicationFirstRedraw );
		IH_RAISE_EVENT( lInstrumentationHandler, appStartupInstrumentationEventIdBase+MTestInstrumentation::TIDOffsetBeginApplicationReadyForInput );
		IH_DELETE( lInstrumentationHandler );
		}
#endif

 	// Retrieve the executable name from the CApaCommandLine object passed in.
	const TPtrC logicalExecutableName(aCommandLine.ExecutableName());
	// Rule-based app launching is not allowed unless there is a connected RApaLsSession object.
	if(Handle() != KNullHandle)
		{
		// requesting from rule-based plug-ins if we can run an application 
		// if server fails while requested rule-based plug-ins it returns a negative value - proceed with launching the application in this case
		const TBool okayToRun = SendReceiveWithReconnect(EAppListServRuleBasedLaunching, TIpcArgs(&logicalExecutableName));
		User::LeaveIfError(!okayToRun ? KErrCancel : okayToRun);	// May leave with KErrNotFound if exe not found
		}

	TFileName nativeExecutableNameOfNonNativeApplication;
	RProcess process;
	HBufC8* opaqueData = NULL;
	CleanupStack::PushL(TCleanupItem(DeletePointerToPointerToTAny, &opaqueData));

	// if we're connected to the Apparc server, try to get the opaque-data and native-executable name
	// (the latter is only if it's a non-native application that we're launching)
	if (Handle()!=KNullHandle)
		{
		const TInt lengthOfOpaqueData=User::LeaveIfError(SendReceiveWithReconnect(EAppListServGetNativeExecutableNameIfNonNative, TIpcArgs(&nativeExecutableNameOfNonNativeApplication, &logicalExecutableName)));
		User::LeaveIfError(GetNewOpaqueData(opaqueData, lengthOfOpaqueData));
		}

	// try first to create the application process without interacting with the Apparc server at all - 
	// assumes "logicalExecutableName" is itself a native executable
	TUidType uidType(KNullUid, KNullUid, KNullUid);
	TInt err = process.CreateWithStackOverride(logicalExecutableName, KNullDesC, uidType, MinApplicationStackSize(), EOwnerProcess);
	 
	// If we haven't been able to create the process using the native executable name from the command line
	// object, instead try to create it using the native executable name of the non-native application.
	// Can only do this if apparc is connected and thus this name has been retrieved above and 
	// nativeExecutableNameOfNonNativeApplication populated.
	if (err && nativeExecutableNameOfNonNativeApplication.Length() > 0)
		err = process.CreateWithStackOverride(nativeExecutableNameOfNonNativeApplication, KNullDesC, uidType, MinApplicationStackSize(), EOwnerProcess);

	// if we managed to create the process via either of the two methods attempted above (with the native 
	// name or the native name of the non-native app), finish setting it up and "resume" it
	if (!err)
		{
		CleanupStack::PushL(TCleanupItem(CleanupOperation, &process));

		if(opaqueData)
			const_cast<CApaCommandLine&>(aCommandLine).SetOpaqueDataL(*opaqueData);
		
		aCommandLine.SetProcessEnvironmentL(process);

		if (aThreadId)
			GetMainThreadIdL(*aThreadId, process);

		if (aRequestStatusForRendezvous)
			process.Rendezvous(*aRequestStatusForRendezvous);

		// Note - must not leave between here and the end of this method because we only expect
		// the caller to wait on aRequestStatusForRendezvous if this method does not leave.
		if(aRequestStatusForRendezvous != NULL && *aRequestStatusForRendezvous != KRequestPending)
			{
			User::WaitForRequest(*aRequestStatusForRendezvous);
			User::Leave(aRequestStatusForRendezvous->Int()); // item on cleanupstack terminates and closes the process
			}
		else
			{
			process.Resume();	
			}
		CleanupStack::Pop(&process);	
		process.Close();
		}

	CleanupStack::PopAndDestroy(&opaqueData);
	User::LeaveIfError(err);
	} //lint !e1762 Suppress member function could be made const
	
void RApaLsSession::CleanupOperation(TAny* aAny)
	{
	RProcess* activeProcess = reinterpret_cast<RProcess*>(aAny);	
	activeProcess->Terminate(KErrGeneral);
	activeProcess->Close();
	}
	

/**
Get the ID of the process's main thread.
*/
void RApaLsSession::GetMainThreadIdL(TThreadId& aThreadId, const RProcess& aProcess)
	{ // static
	TFullName fullName(aProcess.Name());
	_LIT(KCCMain,"::Main");
	fullName.Append(KCCMain);
	RThread thread;
	User::LeaveIfError(thread.Open(fullName, EOwnerThread));
	aThreadId = thread.Id();
	thread.Close();
	}

void RApaLsSession::DeletePointerToPointerToTAny(TAny* aPointerToPointerToTAny)
	{ // static
	__ASSERT_ALWAYS(aPointerToPointerToTAny, Panic(EPanicNullPointer));
	delete *static_cast<TAny**>(aPointerToPointerToTAny); //lint !e613 Suppress possible use of null pointer
	} //lint !e818 Suppress pointer parameter could be declared as pointing to const

/**
Gets the executable's file name and a new buffer containing its opaque data.

Sets up slots 0 and 1 in aIpcArgs - assumes that slots 2 and 3 have already been set up; 
it then invokes aOpcode
*/
TInt RApaLsSession::GetExecutableNameAndNewOpaqueData(TDes& aNativeExecutableName, TDes& aLogicalExecutableName, HBufC8*& aOpaqueData, TIpcArgs& aIpcArgs, TInt aOpcode) const
	{
	aNativeExecutableName.SetLength(0); // if this comes out zero-length from EAppListServExecutableNameForXxx below then use the logicalExecutableName in RProcess::Create (in this case it's a native C++ application)
	aIpcArgs.Set(0, &aNativeExecutableName);
	aIpcArgs.Set(1, &aLogicalExecutableName);
	
	const TInt lengthOfOpaqueData = SendReceiveWithReconnect(aOpcode, aIpcArgs);
	if (lengthOfOpaqueData < 0)
		return lengthOfOpaqueData; // it's an error code

	return GetNewOpaqueData(aOpaqueData, lengthOfOpaqueData);
	}

/**
Allocated a new buffer for aOpaqueData containing data fetched fronm AppArc server.
Returns an error code if no opaque data could be allocated or fetched.
*/
TInt RApaLsSession::GetNewOpaqueData(HBufC8*& aOpaqueData, TInt aLengthOfOpaqueData) const
	{
	aOpaqueData = NULL;
	if(!aLengthOfOpaqueData)
		return KErrNone;	// Nothing to get
	
	// Make sure it's not a negative error code
	ASSERT(aLengthOfOpaqueData > 0);
	
	// Allocate a buffer for the opaque data
	HBufC8* const opaqueData = HBufC8::New(aLengthOfOpaqueData);
	if (!opaqueData)
		return KErrNoMemory;
	
	// Get the opaque data from the AppArc server
	TPtr8 opaqueData_asWritable(opaqueData->Des());
	const TInt error = SendReceiveWithReconnect(EAppListServGetOpaqueData, TIpcArgs(&opaqueData_asWritable));
	if(error)
		delete opaqueData;
	else
		aOpaqueData = opaqueData;
	
	return error;
	}

TInt RApaLsSession::StartApplicationPassingFileHandle(const TDesC& aNativeExecutableName, const TDesC& aLogicalExecutableName, const TDesC8* aOpaqueData, const RFile& aFile, TThreadId& aThreadId, TRequestStatus* aRequestStatusForRendezvous)
	{
	TFileName documentName;
	aFile.FullName(documentName);
	CApaCommandLine* commandLine=NULL;	
	TRAPD(error, 	commandLine=CApaCommandLine::NewLC();
					commandLine->SetCommandL(EApaCommandOpen);
					commandLine->SetExecutableNameL(aLogicalExecutableName);
					commandLine->SetFileByHandleL(aFile);
					commandLine->SetDocumentNameL(documentName);
					if (aOpaqueData)
						commandLine->SetOpaqueDataL(*aOpaqueData);

					DoStartApplicationL(aNativeExecutableName, *commandLine, aThreadId, aRequestStatusForRendezvous);
					CleanupStack::PopAndDestroy(commandLine));
					
	return error;
	}
	
TInt RApaLsSession::StartApplicationPassingDocumentName(const TDesC& aNativeExecutableName, const TDesC& aLogicalExecutableName, const TDesC8* aOpaqueData, const TDesC& aDocumentName, TThreadId& aThreadId,TApaCommand aCommand, TRequestStatus* aRequestStatusForRendezvous)
	{
	CApaCommandLine* commandLine=NULL;
	TRAPD(error, 	commandLine=CApaCommandLine::NewLC();
					commandLine->SetCommandL(aCommand);
					commandLine->SetExecutableNameL(aLogicalExecutableName);
					commandLine->SetDocumentNameL(aDocumentName);
					if (aOpaqueData)
						commandLine->SetOpaqueDataL(*aOpaqueData);

					DoStartApplicationL(aNativeExecutableName, *commandLine, aThreadId, aRequestStatusForRendezvous);
					CleanupStack::PopAndDestroy(commandLine));

	return error;
	}

/**
*/
void RApaLsSession::DoStartApplicationL(const TDesC& aNativeExecutableName, const CApaCommandLine& aCommandLine, TThreadId& aThreadId, TRequestStatus* aRequestStatusForRendezvous)
	{ 
	TPtrC actualNativeExecutableName(aNativeExecutableName);
	if(!actualNativeExecutableName.Length())
		actualNativeExecutableName.Set(aCommandLine.ExecutableName()); // it's a native C++ application, rather than a MIDlet, Python script, etc

	// Aquire permission to start the app from "application start rule" plug-ins
	if(Handle() != KNullHandle)	// Connected to AppArc server?
		{
		const TBool okayToRun = SendReceiveWithReconnect(EAppListServRuleBasedLaunching, TIpcArgs(&actualNativeExecutableName));
		// If server fails while requested rule-based plug-ins it returns a negative value. 
		// We shall proceed with launching an application in this case.
		User::LeaveIfError(!okayToRun ? KErrCancel : okayToRun);	// May leave with KErrNotFound if exe is not found or KErrNotSupported if embeddable only
		}

	// Start the application
	
	// Create a new process
	RProcess process;
	TUidType uidType(KNullUid, KNullUid, KNullUid);
	User::LeaveIfError(process.CreateWithStackOverride(actualNativeExecutableName, KNullDesC, uidType, MinApplicationStackSize(), EOwnerProcess)); // RProcess::CreateWithStackOveride - TESTED
	CleanupClosePushL(process);
	
	// Write the command line data to the process environment
	// This is used inside the new process to resume the execution
	aCommandLine.SetProcessEnvironmentL(process);
	
	//  Find the ID of the new process's main thread
	GetMainThreadIdL(aThreadId, process);
	
	// Rendezvous with the new process, if required
	if (aRequestStatusForRendezvous)
		process.Rendezvous(*aRequestStatusForRendezvous);

	process.Resume();
	CleanupStack::PopAndDestroy(&process);
	} //lint !e1762 Suppress member function could be made const



/** Finds and launches an application to handle the document contained in the specified 
file.

@param aDocFileName The document name.
@param aThreadId On return, the id of the main thread started.
@param aLaunchType Not used. Deprecated.
@return KErrNone, if successful; EAppListInvalid, if the server's initial population of 
the list has not completed; KErrNotFound, if a matching entry could not be found; otherwise 
one of the other system-wide error codes. 
*/
EXPORT_C TInt RApaLsSession::StartDocument(const TDesC& aDocFileName, TThreadId& aThreadId, TLaunchType /*aLaunchType*/)
	{
	HBufC8* buffer = NULL;
	TInt error = GetNewBufferFromFile(buffer, aDocFileName);
	if (error)
		return error;

	TFileName nativeExecutableName; // the name of the EXE that we pass to RProcess::Create
	TFileName logicalExecutableName; // the name of the MIDlet, Python script, etc
	HBufC8* opaqueData = NULL;
	TIpcArgs ipcArgs;
	ipcArgs.Set(2, &aDocFileName);
	ipcArgs.Set(3, buffer);
	error = GetExecutableNameAndNewOpaqueData(nativeExecutableName, logicalExecutableName, opaqueData, ipcArgs, EAppListServGetExecutableNameGivenDocument);
	if (error)
		{
		delete buffer;
		return error;
		}
		
	error = StartApplicationPassingDocumentName(nativeExecutableName, logicalExecutableName, opaqueData, aDocFileName, aThreadId, EApaCommandOpen, NULL);
	delete opaqueData;

	delete buffer;
	return error;
	}

/** Finds and launches an application to handle the document contained in the specified file

@param aFile  The file handle. Before this function can be called,
the file server session which owns this file handle must first be marked as shareable by 
calling RFs::ShareProtected().
@param aThreadId On return, the id of the main thread started.
@param aRequestStatusForRendezvous If not NULL, the asynchronous RProcess::Rendezvous() 
function is called (being passed this TRequestStatus object) before RProcess::Resume() is called on 
the new application process.
@return KErrNone, if successful; KErrNotFound, if no suitable application can 
be found; otherwise one of the other system-wide error codes. 
*/
EXPORT_C TInt RApaLsSession::StartDocument(RFile& aDocFile, TThreadId& aThreadId, TRequestStatus* aRequestStatusForRendezvous/*=NULL*/)
	{
	TFileName nativeExecutableName; // the name of the EXE that we pass to RProcess::Create
	TFileName logicalExecutableName; // the name of the MIDlet, Python script, etc
	HBufC8* opaqueData = NULL;
	TIpcArgs ipcArgs;
	TInt error = aDocFile.TransferToServer(ipcArgs, 2, 3);
	if (!error)
		error = GetExecutableNameAndNewOpaqueData(nativeExecutableName, logicalExecutableName, opaqueData, ipcArgs, EAppListServGetExecutableNameGivenDocumentPassedByFileHandle);
	
	if (error)
		return error;

	error = StartApplicationPassingFileHandle(nativeExecutableName, logicalExecutableName, opaqueData, aDocFile, aThreadId, aRequestStatusForRendezvous);
	delete opaqueData;
	return error;
	} //lint !e1764 Suppress reference parameter could be declared const ref


/** Launches an application that can handle the specified data (MIME) type.

The application handles the document contained in the specified file.

@param aDocFileName The document name.
@param aDataType The data (MIME) type.
@param aThreadId On return, the id of the main thread started.
@param aLaunchType Not used. Deprecated.
@return KErrNone, if successful; EAppListInvalid if the server's initial population of 
the list has not completed; KErrNotFound, if no suitable application can 
be found; otherwise one of the other system-wide error codes. 
*/
EXPORT_C TInt RApaLsSession::StartDocument(const TDesC& aDocFileName, const TDataType& aDataType, TThreadId& aThreadId, TLaunchType /*aLaunchType*/)
	{
	TFileName nativeExecutableName; // the name of the EXE that we pass to RProcess::Create
	TFileName logicalExecutableName; // the name of the MIDlet, Python script, etc
	HBufC8* opaqueData=NULL;
	const TPckgC<TDataType> dataType(aDataType);
	TIpcArgs ipcArgs;
	ipcArgs.Set(2, &dataType);
	TInt error=GetExecutableNameAndNewOpaqueData(nativeExecutableName, logicalExecutableName, opaqueData, ipcArgs, EAppListServGetExecutableNameGivenDataType);
	if (error)
		return error;

	error = StartApplicationPassingDocumentName(nativeExecutableName, logicalExecutableName, opaqueData, aDocFileName, aThreadId,EApaCommandOpen,NULL);
	delete opaqueData;

	return error;
	}

/** Finds and launches an application to handle the document contained in the specified file

@param aDocFile The file handle.
@param aDataType The data (MIME) type.
@param aThreadId On return, the id of the main thread started.
@param aRequestStatusForRendezvous If not NULL, the asynchronous RProcess::Rendezvous() 
function is called (being passed this TRequestStatus object) before RProcess::Resume() is called on 
the new application process.
@return KErrNone, if successful; KErrNotFound, if no suitable application can 
be found; otherwise one of the other system-wide error codes. 
*/
EXPORT_C TInt RApaLsSession::StartDocument(RFile& aDocFile, const TDataType& aDataType, TThreadId& aThreadId, TRequestStatus* aRequestStatusForRendezvous/*=NULL*/)
	{
	TFileName nativeExecutableName; // the name of the EXE that we pass to RProcess::Create
	TFileName logicalExecutableName; // the name of the MIDlet, Python script, etc
	HBufC8* opaqueData = NULL;
	const TPckgC<TDataType> dataType(aDataType);
	TIpcArgs ipcArgs;
	ipcArgs.Set(2, &dataType);
	TInt error = GetExecutableNameAndNewOpaqueData(nativeExecutableName, logicalExecutableName, opaqueData, ipcArgs, EAppListServGetExecutableNameGivenDataType);
	if (error)
		return error;

	error = StartApplicationPassingFileHandle(nativeExecutableName, logicalExecutableName, opaqueData, aDocFile, aThreadId, aRequestStatusForRendezvous);
	delete opaqueData;
	return error;
	} //lint !e1764 Suppress reference parameter could be declared const ref


/** Launches the application identified by the specified UID.

The application handles the document contained in the specified file.

@param aDocFileName The document name.
@param aAppUid The application specific UID.
@param aThreadId On return, the id of the main thread started.
@param aLaunchType Not used. Deorecated.
@return KErrNone, if successful; EAppListInvalid if the server's initial population of 
the list has not completed; KErrNotFound, if no suitable application can 
be found; otherwise one of the other system-wide error codes. */
EXPORT_C TInt RApaLsSession::StartDocument(const TDesC& aDocFileName, TUid aAppUid, TThreadId& aThreadId, TLaunchType /*aLaunchType*/)
	{
	// Get the executable file name and "opaque" app meta-data from AppArc server
	
	TFileName nativeExecutableName; // the name of the EXE that we pass to RProcess::Create
	TFileName logicalExecutableName; // the name of the MIDlet, Python script, etc
	HBufC8* opaqueData = NULL;
	
	TIpcArgs ipcArgs;
	ipcArgs.Set(2, aAppUid.iUid);
	
	TInt error = GetExecutableNameAndNewOpaqueData(nativeExecutableName, logicalExecutableName, opaqueData, ipcArgs, EAppListServGetExecutableNameGivenAppUid);
	if (error)
		return error;
	
	// Start the application, passing it the document file name

	error = StartApplicationPassingDocumentName(nativeExecutableName, logicalExecutableName, opaqueData, aDocFileName, aThreadId, EApaCommandOpen, NULL);
	delete opaqueData;
	
	return error;
	}

/** Finds and launches an application to handle the document contained in the specified file

@param aDocFile The file handle.
@param aAppUid The application specific UID.
@param aThreadId On return, the id of the main thread started.
@param aRequestStatusForRendezvous If not NULL, the asynchronous RProcess::Rendezvous() 
function is called (being passed this TRequestStatus object) before RProcess::Resume() is called on 
the new application process.
@return KErrNone, if successful; KErrNotFound, if no suitable application can 
be found; otherwise one of the other system-wide error codes. */
EXPORT_C TInt RApaLsSession::StartDocument(RFile& aDocFile, TUid aAppUid, TThreadId& aThreadId, TRequestStatus* aRequestStatusForRendezvous/*=NULL*/)
	{
	TFileName nativeExecutableName; // the name of the EXE that we pass to RProcess::Create
	TFileName logicalExecutableName; // the name of the MIDlet, Python script, etc
	HBufC8* opaqueData=NULL;
	TIpcArgs ipcArgs;
	ipcArgs.Set(2, aAppUid.iUid);
	TInt error = GetExecutableNameAndNewOpaqueData(nativeExecutableName, logicalExecutableName, opaqueData, ipcArgs, EAppListServGetExecutableNameGivenAppUid);
	if (error)
		return error;

	error = StartApplicationPassingFileHandle(nativeExecutableName, logicalExecutableName, opaqueData, aDocFile, aThreadId, aRequestStatusForRendezvous);
	delete opaqueData;
	return error;
	} //lint !e1764 Suppress reference parameter could be declared const ref


/** Launches the application identified by the specified UID and creates a new document.

To create a document file with the passed document name, the application needs to override the 3-parameter
overload of ProcessCommandParametersL() to call the 2-parameter overload.

Otherwise, a document will be created with the default document name present in the application resource file.
If default document name is empty or not provided, no document is created.

If the application resource file is not present, a document with application caption name is created.

@param aDocFileName The document name.
@param aAppUid The application specific UID. 
@param aThreadId On return, the id of the main thread started.
@param aLaunchType Not used. Deprecated.
@return KErrNone, if successful; EAppListInvalid if the server's initial population of 
the list has not completed; KErrNotFound, if no suitable application can 
be found; otherwise one of the other system-wide error codes. 
@see CEikAppUi::ProcessCommandParametersL().
*/
EXPORT_C TInt RApaLsSession::CreateDocument(const TDesC& aDocFileName, TUid aAppUid, TThreadId& aThreadId, TLaunchType /*aLaunchType*/)
	{
	TFileName nativeExecutableName; // the name of the EXE that we pass to RProcess::Create
	TFileName logicalExecutableName; // the name of the MIDlet, Python script, etc
	HBufC8* opaqueData=NULL;
	TIpcArgs ipcArgs;
	ipcArgs.Set(2, aAppUid.iUid);
	TInt error = GetExecutableNameAndNewOpaqueData(nativeExecutableName, logicalExecutableName, opaqueData, ipcArgs, EAppListServGetExecutableNameGivenAppUid);
	if (error)
		return error;

	error = StartApplicationPassingDocumentName(nativeExecutableName, logicalExecutableName, opaqueData, aDocFileName, aThreadId,EApaCommandCreate,NULL);
	delete opaqueData;

	return error;
	}
	


