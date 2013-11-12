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
// apaproc.cpp
//

#include <apaproc.h> // stuff everyone will want ie most things
#include <apacln.h> // CleanupStack protection for CApaDocument
#include "APADLL.H" // class RApaApplication
#include "APASTD.H" // Panics etc.
#include <s32file.h>
#include "../apparc/TRACE.H"
#include <e32def_private.h> // MattR addition for __PROFILE_END error

const TInt KAppProcessArrayGranularity(1);


//
// CApaParentProcessMonitor
//

class CApaParentProcessMonitor : public CActive
	{
public: // Construction / destruction
	static CApaParentProcessMonitor* NewL(TProcessId aProcessId);
	~CApaParentProcessMonitor();
	void ConstructL();
private:
	CApaParentProcessMonitor(TProcessId aProcessId);
public: // From CActive
	void RunL();
	void DoCancel();
private:
	TProcessId iProcessId;
	RProcess iProcess;
	};

CApaParentProcessMonitor* CApaParentProcessMonitor::NewL(TProcessId aProcessId)
	{
	CApaParentProcessMonitor* self=new (ELeave) CApaParentProcessMonitor(aProcessId);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CApaParentProcessMonitor::CApaParentProcessMonitor(TProcessId aProcessId)
	: CActive(EPriorityLow)
	{
	iProcessId=aProcessId;
	}

CApaParentProcessMonitor::~CApaParentProcessMonitor()
	{
	Cancel();
	}

void CApaParentProcessMonitor::ConstructL()
	{
	User::LeaveIfError(iProcess.Open(iProcessId));
	iProcess.Logon(iStatus);
	if(iStatus==KErrNoMemory)
		{
		User::WaitForRequest(iStatus);
		User::Leave(KErrNoMemory);
		}
	CActiveScheduler::Add(this);
	SetActive();
	}

void CApaParentProcessMonitor::RunL()
	{
	// Do something that will kill the child when the parent process terminates
	if(iStatus==KErrNone)
		{
		RProcess proc;
		proc.Terminate(KErrNone);
		}
	}

void CApaParentProcessMonitor::DoCancel()
	{
	iProcess.LogonCancel(iStatus);
	}

//
// CApaProcess
//

/** Constructor for CApaProcess */
EXPORT_C CApaProcess::CApaProcess()
	{
	}

EXPORT_C CApaProcess* CApaProcess::NewL(const RFs& aFs)
/** Creates and returns a pointer to a new application process.

This function is not used by UI applications.

@param aFs Handle to a file server session.
@return Pointer to the new application process. */
	{
	CApaProcess* self=new(ELeave) CApaProcess(aFs);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

EXPORT_C CApaProcess::CApaProcess(const RFs& aFs)
	:iFsSession(aFs)
/** Constructs the application process object with the specified file session handle.

Derived classes must define and implement a constructor through which 
the file session handle can be specified. A 
typical implementation calls this constructor through a constructor initialization 
list.

This constructor is used by the UI framework.

@deprecated
@param aFs Handle to a file server session */
	{}

const TInt KPriorityGreaterThanShutter=102;

EXPORT_C void CApaProcess::ConstructL()
/** Completes construction of the application process object.

Implementers of derived classes must call this function as part of the second 
stage construction of an object. Typically, derived classes implement their 
own NewL() function and call ConstructL() as part of that implementation. */
	{
	//
	iAppList = new(ELeave) CArrayPtrFlat<RApaApplication>(KAppProcessArrayGranularity);
	iDocList = new(ELeave) CArrayPtrFlat<CApaDocument>(KAppProcessArrayGranularity);
	iMainDocFileName = HBufC::NewL(KMaxFileName);
	iAsyncAppRemover = CIdle::NewL(KPriorityGreaterThanShutter);	// Use an idle object so that app has chance to clear its call stack
	}

EXPORT_C void CApaProcess::ConstructL(TProcessId aParentProcessId)
/** Completes construction of the application process object, passing in a Parent Process Identifier.

Implementers of derived classes must call this function as part of the second 
stage construction of an object. Typically, derived classes implement their 
own NewL() function and call ConstructL() as part of that implementation.

@param aParentProcessId Id of the parent process. This process will terminate when the parent does. */
	{
	ConstructL();
	if(KNullProcessId!=aParentProcessId)
		{
		iMonitor=CApaParentProcessMonitor::NewL(aParentProcessId);
		}
	}

/** Frees resources prior to destruction.

Documents must be saved before the application process is deleted, otherwise 
data may be lost.

In debug mode, the destructor raises an APPARC 6 panic if documents still 
exist, and an APPARC 5 panic if applications still exist. */
EXPORT_C CApaProcess::~CApaProcess()
// If this is called without calling ResetL() or CApaDocument::SaveL() first, data may be lost
//
	{
	if (iMainDoc)
		{
		DestroyDocument(iMainDoc);
		iMainDoc = NULL;
		}

	if (iDocList)
		{
		__ASSERT_DEBUG(iDocList->Count()==0,Panic(EPanicDocListNotEmpty));
		for (TInt i=iDocList->Count()-1 ; i>=0 ; i--)
			delete (*iDocList)[i]; // delete stray doc's in release mode, just to be tidy
		}

	if (iAppList)
		{
		for (TInt i = iAppList->Count()-1 ; i >= 0 ; i--)
			(*iAppList)[i]->Close();
		}

	delete iAppList;
	delete iDocList;
	delete iMainDocFileName;
	delete iAsyncAppRemover;
	delete iMonitor;
	}


/** Resets the the application process to its initial state.

Specifically, it saves the main document, deletes the main and all embedded 
documents from memory, resets the main document filename and deletes all applications 
except the main application.

The function can leave if saving the main document fails. */
EXPORT_C void CApaProcess::ResetL()
	{
	if (iMainDoc)
		{
		iMainDoc->SaveL();
		DeleteAllDocs(); // sets iMainDoc to NULL, deletes all apps except main
		}
		
	__ASSERT_DEBUG(iMainDocFileName, Panic(EPanicNoDocument));
	*iMainDocFileName = KNullDesC;
	}


void CApaProcess::DeleteAllDocs()
// deletes all docs
// deletes all apps except main app
// sets iMainDoc* to NULL
//
	{
	CApaApplication* mainApp = NULL;
	
	// If the main document has been constructed...
	if (iMainDoc) // then iDocList must also exist
		{
		__ASSERT_DEBUG(iMainDoc->Application(), Panic(EDPanicNoApp));
		mainApp = iMainDoc->Application();
		
		// ...find the main document in the list of documents and delete it.
		for (TInt i = iDocList->Count()-1; i >= 0; i--)
			{
			if ((*iDocList)[i] == iMainDoc)
				{
				iDocList->Delete(i); // removes from array, but doesnt destroy
				delete iMainDoc;
				iMainDoc = NULL;
				break;
				}
			}
		}
	
	// Remove all documents from the list of documents, without deleting them.
	if(iDocList)	
		{
		__ASSERT_ALWAYS(iDocList->Count()==0, Panic(EPanicDocListNotEmpty));
		iDocList->Reset();
		}

	// Delete all applications except the main one.
	if(iAppList)
		{
		for (TInt ii = iAppList->Count()-1; ii >= 0; ii--) // need to iterate backwards as the array changes size during the loop
			{
			if ((*iAppList)[ii]->Application() != mainApp)
				{
				(*iAppList)[ii]->Close();
				iAppList->Delete(ii);
				}
			}
	
		iAppList->Compress();
		}
	}


EXPORT_C void CApaProcess::SetMainDocFileName(const TDesC& aMainDocFileName)
/** Sets the filename of the main document.

@param aMainDocFileName The filename to be set.
@panic APPARC 7 If the length of aMainDocFileName is greater than KMaxFileName or the
length of the last filename set by SetMainDocFileNameL if greater
@see KMaxFileName */
	{
	__ASSERT_DEBUG( iMainDocFileName, Panic(EPanicNullPointer));
	__ASSERT_ALWAYS( aMainDocFileName.Length()<=iMainDocFileName->Des().MaxLength() ,Panic(EPanicFileNameTooLong));
	*iMainDocFileName = aMainDocFileName;
	}

EXPORT_C void CApaProcess::SetMainDocFileNameL(const TDesC& aMainDocFileName)
/** Sets the filename of the main document.

@param aMainDocFileName The filename to be set. There is no restriction on the
length of this descriptor. */
	{
	__ASSERT_ALWAYS( iMainDocFileName, Panic(EPanicNullPointer));
	const TInt newLength = aMainDocFileName.Length() < KMaxFileName ? KMaxFileName : aMainDocFileName.Length();
	if (newLength != iMainDocFileName->Des().MaxLength())
		{
		HBufC* const newMainDocFileName = HBufC::NewL(newLength);
		delete iMainDocFileName;
		iMainDocFileName = newMainDocFileName;
		}
	SetMainDocFileName(aMainDocFileName);
	}

/** Sets the main document.

@param aDocument A pointer to the document to be set as the main document 
of the application process. This must be a an object created by the AddNewDocumentL() 
or OpenNewDocumentL() functions 
@see CApaProcess::AddNewDocumentL()
@see CApaProcess::OpenNewDocumentL() */
EXPORT_C void CApaProcess::SetMainDocument(CApaDocument* aDocument)
	{
	__ASSERT_ALWAYS( iDocList, Panic(EPanicNullPointer));
	// check that the prospective main doc has actually been added to the array
	for (TInt i = iDocList->Count()-1 ; i >= 0 ; i--)
		{
		if ((*iDocList)[i] == aDocument)
			break;
		
		if (i==0)
			Panic(EPanicNoDocument);
		}
	
	// assign it once it has checked out
	iMainDoc = aDocument;
	}

/** Creates and adds a new document using the specified application factory.

The document may be a main document or an embedded document.

Any document created with this function must be destroyed using DestroyDocument().

@param aApplicationFactory Should be created implicitly by passing a pointer to
a factory function, an ECOM plugin UID, or a CImplementationInformation reference.
@return A pointer to the new document.
@see CApaProcess::DestroyDocument()
@see CApaApplication */
EXPORT_C CApaDocument* CApaProcess::AddNewDocumentL(TApaApplicationFactory aApplicationFactory)
	{
	__SHOW_TRACE(_L("Starting CApaProcess::AddNewDocumentL"));
	__APA_PROFILE_START(0);

	RApaApplication* app = AddAppL(aApplicationFactory);	

	// use the app to create a doc
	CApaDocument* doc = NULL;
	TRAPD(err, doc = CreateDocL(app->Application()));
	if (err)
		RemoveApp(app);	// remove app as it has been orphaned
	
	User::LeaveIfError(err);
	__PROFILE_END(0);
	return doc;
	} //lint !e1762 Member function could be made const - Not true



void CApaProcess::RemoveApp(RApaApplication* aApp)
// removes app exe from the list if it exists, panics otherwise
	{
	__ASSERT_ALWAYS(iAppList, Panic(EPanicNullPointer));
	TInt i = 0;
	for (i = iAppList->Count()-1; i >= 0; i--)
		{
		if ((*iAppList)[i] == aApp) // the main app may be alive on its own if Reset() has just been called
			{
			aApp->Close(); // the main app may be alive on its own if Reset() has just been called
			iAppList->Delete(i);
			break;
			}
		}
		
	if (i < 0)
		Panic(EPanicAppNotInList);
	}


/** Opens the specified file and restores the content as a document.

The created document can be merged into or embedded in another document.

Any document created with this function must be destroyed using DestroyDocument().

@param aStore On return, this contains a pointer to the store object created 
during the restore.
@param aStreamDic On return, this contains a pointer to the stream dictionary 
object created during the restore. 
@param aDocFullFileName The name of the file containing the document. 
@param aFileMode The mode in which to open the file. 
@return A pointer to the restored document.
@see TFileMode
@see CApaProcess::DestroyDocument() */
EXPORT_C CApaDocument* CApaProcess::OpenNewDocumentL(CFileStore*& aStore,CStreamDictionary*& aStreamDic,const TDesC& aDocFullFileName,TUint aFileMode)
	{
	__SHOW_TRACE(_L("Starting CApaProcess::OpenNewDocumentL"));
	__APA_PROFILE_START(1);
	TParse parser;
	User::LeaveIfError(iFsSession.Parse(aDocFullFileName,parser)); 
	// open doc as a file store & read in the header
	CFileStore* docStore;
	CStreamDictionary* streamDic = ReadRootStreamLC(FsSession(),docStore,parser.FullName(),aFileMode);
	CleanupStack::PushL(docStore);
	// read in the app id info
	TApaAppIdentifier appId=ReadAppIdentifierL(*docStore,*streamDic);
	// create the doc
	CApaDocument* importedDoc =	AddNewDocumentL(appId.iAppUid);
	// restore the document
	TApaDocCleanupItem cleanup(this,importedDoc);
	CleanupStack::PushL(cleanup);
	importedDoc->RestoreL(*docStore,*streamDic);
	CleanupStack::Pop(3); //docStore,importedDoc,streamDic
	aStore = docStore;
	aStreamDic = streamDic;
	__PROFILE_END(1);
	return importedDoc;
	}

/** Reads the application identifier from its stream in the specified store and 
returns it.

The location of the stream is found in the specified stream dictionary.

@param aStore The store from which the application identifier should be read. 
@param aStreamDic The stream dictionary containing the stream ID of the application 
identifier stream. The stream dictionary can be found in the root stream of 
the store.
@return The application identifier. */
EXPORT_C TApaAppIdentifier CApaProcess::ReadAppIdentifierL(const CStreamStore& aStore,const CStreamDictionary& aStreamDic)
	{
	// this is a static method
	__SHOW_TRACE(_L("Starting CApaProcess::ReadAppIdentifierL"));
	
	TStreamId infoStreamId = aStreamDic.At(KUidAppIdentifierStream);
	TApaAppIdentifier appId;
	
	// create a stream and read in the data
	RStoreReadStream stream;
	stream.OpenLC(aStore,infoStreamId);

	stream >> appId;
	stream.Close();

	CleanupStack::PopAndDestroy(); // stream
	return appId;	
	}


/** Writes the application identifier to a new stream in the specified store and 
records the location of this stream in the specified stream dictionary.

@param aStore The store to which the application identifier should be written. 

@param aStreamDic The stream dictionary. 
@param aAppId The application identifier to be externalised to a stream. */
EXPORT_C void CApaProcess::WriteAppIdentifierL(CStreamStore& aStore,CStreamDictionary& aStreamDic,const TApaAppIdentifier& aAppId)
	{
	// this is a static method
	__SHOW_TRACE(_L("Starting CApaProcess::WriteAppIdentifierL"));
	
	// create a stream
	RStoreWriteStream stream;
	TStreamId streamId = stream.CreateLC(aStore);
	
	// stream the header
	stream << aAppId;
	stream.CommitL();
	
	CleanupStack::PopAndDestroy(); // id stream
	// enter the stream in the dictionary
	aStreamDic.AssignL(KUidAppIdentifierStream, streamId);
	}


/** Reads the stream dictionary contained as the root stream in the specified document 
file.

The function constructs, and returns a pointer to the stream dictionary object 
and puts the pointer to the stream dictionary object onto the cleanup stack. 
It also returns a pointer to the created file store object through an argument 
reference. 

The file must be a valid document file; otherwise the function leaves with one of 
the system-wide error codes.

@param aFs Handle to a file server session.
@param aStore On return, a pointer to the newly created file store object. 
@param aDocFullFileName The full path name of the document file. 
@param aFileMode The mode in which to open the file.
@return A pointer to the stream dictionary object read from the root stream 
of the store. 
@see TFileMode */
EXPORT_C CStreamDictionary* CApaProcess::ReadRootStreamLC(RFs& aFs,CFileStore*& aStore,const TDesC& aDocFullFileName,TUint aFileMode)
	{ // static
	__SHOW_TRACE(_L("Starting CApaProcess::ReadRootStreamLC (file-name overload)"));
	CStreamDictionary* const streamDictionary=CStreamDictionary::NewLC();
	CFileStore* const store=CFileStore::OpenLC(aFs,aDocFullFileName,aFileMode);
	DoReadRootStreamL(*streamDictionary, *store);
	aStore=store; // delay assignment until nothing can go wrong to avoid destroying the store twice if a leave occurs
	CleanupStack::Pop(store);
	return streamDictionary;
	}


/**
@internalTechnology
*/
EXPORT_C CStreamDictionary* CApaProcess::ReadRootStreamLC(CFileStore*& aStore, const RFile& aFile)
	{ // static
	__SHOW_TRACE(_L("Starting CApaProcess::ReadRootStreamLC (file-handle overload)"));

	CStreamDictionary* const streamDictionary = CStreamDictionary::NewLC();
	
	RFile duplicateFile;
	CleanupClosePushL(duplicateFile);
	User::LeaveIfError(duplicateFile.Duplicate(aFile)); // this is because CFileStore::FromLC closes the file its passed (and stores its own duplicate)
	
	CFileStore* const store = CFileStore::FromL(duplicateFile);
	CleanupStack::PopAndDestroy(&duplicateFile);
	
	CleanupStack::PushL(store);
	DoReadRootStreamL(*streamDictionary, *store);
	aStore = store; // delay assignment until nothing can go wrong to avoid destroying the store twice if a leave occurs
	CleanupStack::Pop(store);
	
	return streamDictionary;
	}


void CApaProcess::DoReadRootStreamL(CStreamDictionary& aStreamDictionary, const CFileStore& aStore)
	{ // static
	const TStreamId rootStreamId=aStore.Root();
	if ((aStore.Type()[1] != KUidAppDllDoc) || (rootStreamId == KNullStreamId))
		User::Leave(KErrCorrupt);
	
	RStoreReadStream rootStream;
	rootStream.OpenLC(aStore, rootStreamId);
	rootStream >> aStreamDictionary;
	CleanupStack::PopAndDestroy(&rootStream);
	}


/** Writes the application identifier (derived from the application object CApaApplication) 
followed by the stream dictionary to the store and makes the stream dictionary the root stream of the
store.

Typically, the function is called by the application when it 
implements a file create or file save type operation. It is called after all 
model and UI data has been persisted. The IDs of the streams containing the 
model and UI data should have been lodged in the stream dictionary.

In effect, the function finishes off the file save or file
create type operation, leaving the file containing the store in a valid state
with the standard interface.

@param aStore  The store to which the root stream is to be written. Before
calling this function, a reference to the store must be saved by putting a
pointer onto the cleanup stack or by making it member data of a class. This
ensures that it is not orphaned in the event of this function leaving.
@param aStreamDic The stream dictionary containing the locations and associated 
UIDs of other streams in the store.
@param aApp  The application used to create the main document in the file
being written. The application identifier to be written is constructed from
this application object. */
EXPORT_C void CApaProcess::WriteRootStreamL(CPersistentStore& aStore,CStreamDictionary& aStreamDic,const CApaApplication& aApp)
	{ // this is a static method
	__SHOW_TRACE(_L("Starting CApaProcess::WriteRootStreamL(app)"));
	// get the app dll name
	TParse dllPath;
	dllPath.SetNoWild(aApp.DllName(),NULL,NULL);
	// set up an app identifier
	TApaAppIdentifier appId(aApp.AppDllUid(),dllPath.NameAndExt());
	// Write the root stream
	WriteRootStreamL(aStore,aStreamDic,appId);
	}


/** Writes the application identifier followed by the stream dictionary 
to the store and makes the stream dictionary the root stream of the store.

Typically, the function is called by the application when it 
implements a file create or file save type operation. It is called after all 
model and UI data has been persisted. The IDs of the streams containing the 
model and UI data should have been lodged in the stream dictionary.

In effect, the function finishes off the file save or file
create type operation, leaving the file containing the store in a valid state
with the standard interface.

@param aStore  The store to which the root stream is to be written. Before
calling this function, a reference to the store must be saved by putting a
pointer onto the cleanup stack or by making it member data of a class. This
ensures that it is not orphaned in the event of this function leaving.
@param aStreamDic The stream dictionary containing the locations and associated 
UIDs of other streams in the store.
@param aAppId  The application identifier to be written into the application
identifier stream. */
EXPORT_C void CApaProcess::WriteRootStreamL(CPersistentStore& aStore,CStreamDictionary& aStreamDic,const TApaAppIdentifier& aAppId)
	{ // this is a static method
	__SHOW_TRACE(_L("Starting CApaProcess::WriteRootStreamL(id)"));
	// create a stream
	WriteAppIdentifierL(aStore,aStreamDic,aAppId);
	
	// externalize the dictionary
	RStoreWriteStream stream;
	TStreamId streamId = stream.CreateLC(aStore);
	stream << aStreamDic;
	stream.CommitL();
	CleanupStack::PopAndDestroy(); // dictionary stream
	
	// set the dictionary stream as the root stream
	aStore.SetRootL(streamId);
	}


/** Destroys the specified document.

All references to the document are removed, and associated resources are freed. 
Specifically, the function deletes any associated application and unloads 
the application DLL, provided that no other documents of that application 
type are still open.

All document objects created through CApaProcess must be deleted using this 
function.

@param aDoc A pointer to the document to be destroyed. 
@see CApaApplication
@see CApaProcess */
EXPORT_C void CApaProcess::DestroyDocument(CApaDocument* aDoc)
	{
	__SHOW_TRACE(_L("Starting CApaProcess::DestroyDocument(app)"));

	if(!aDoc)
		return;
	
	// delete the doc, keeping a handle to its app
	CApaApplication* const app = aDoc->Application();
	__ASSERT_DEBUG(app, Panic(EDPanicDocWithNoApp));
	
	// remove the doc from the list, keeping a handle to the doc
	TBool appStillRequired = EFalse;
	__ASSERT_ALWAYS(iDocList, Panic(EPanicNullPointer));
	for (TInt i = iDocList->Count()-1; i >= 0; i--)
		{ // check through the list, remove the right doc, and see if the app is used by any other docs
		if((*iDocList)[i] == aDoc)
			{
			iDocList->Delete(i); // removes from array, but doesnt destroy
			iDocList->Compress();
			}
		else if ((*iDocList)[i]->Application() == app)
			appStillRequired = ETrue;
		}
	
	// Null the main doc handle if we are deleting the main doc
	if (aDoc == iMainDoc)
		iMainDoc = NULL;
	
	// Now delete the document
	delete aDoc;
	
	// Remove app if no other doc's use it and it's not the main app
	if (!appStillRequired && iMainDoc && app!=iMainDoc->Application())
		{
		MarkApplicationForAsyncRemoval(app);
		__ASSERT_DEBUG(iAsyncAppRemover, Panic(EDPanicNoAppRemover));
		
		if (!iAsyncAppRemover->IsActive())
			iAsyncAppRemover->Start(TCallBack(CApaProcess::IdleRemoveApplications, this));
		}
	}

void CApaProcess::MarkApplicationForAsyncRemoval(const CApaApplication* aApp)
// Mark the application in the app list for removal by idle object
//
	{
	__ASSERT_DEBUG(aApp!=NULL,Panic(EDPanicRemovingNullApp));
	__ASSERT_DEBUG(iAppList, Panic(EPanicNullPointer));
	// remove the app from the list, keeping a handle to it
	for (TInt i = iAppList->Count()-1 ; i >= 0 ; i--)
		{
		__ASSERT_DEBUG((*iAppList)[i], Panic(EDPanicNoAppHolder));
		if ((*iAppList)[i]->Application() == aApp)
			{
			(*iAppList)[i]->ScheduleForAsyncDeletion();
			return;
			}
		}
	}

TInt CApaProcess::IdleRemoveApplications(TAny* aApaProcess)
// Remove applications on callback of idle object. Using an idle object gives an embedded application a chance to clear
// its call stack before its dll is closed
//
	{
	CApaProcess* process = reinterpret_cast<CApaProcess*>(aApaProcess);
	__ASSERT_DEBUG(process, Panic(EDPanicNoProcess));
	process->RemoveMarkedApplications();
	
	return KErrNone;
	}


void CApaProcess::RemoveMarkedApplications()
// Remove any applications that have been marked for removal, closing their dlls also
//
	{
	__ASSERT_DEBUG(iAppList, Panic(EPanicNullPointer));
	for (TInt i = iAppList->Count()-1; i >= 0; i--)
		{
		if ((*iAppList)[i]->IsScheduleForAsyncDeletion())
			{
			(*iAppList)[i]->Close();
			iAppList->Delete(i); // remove from array
			}
		}
	
	iAppList->Compress();
	}


CApaDocument* CApaProcess::CreateDocL(CApaApplication* aApp)
// creates a new doc with aApp and adds it to the list before returning a handle to it
//
	{
	__SHOW_TRACE(_L("Starting CApaProcess::CreateDocL"));
	__ASSERT_DEBUG(aApp, Panic(EDPanicNoApp));

	// create a new doc with the app
	CApaDocument* doc = aApp->CreateDocumentL(this); //lint !e613 Possible use of null pointer - Asserted above
	__ASSERT_ALWAYS(doc, Panic(EPanicDocumentNotCreated));

	// add the doc to the list
	CleanupStack::PushL(doc);
	iDocList->AppendL(doc);
	CleanupStack::Pop(); // doc

	// return a	handle to the doc
	return doc;
	}


RApaApplication* CApaProcess::FindAppInListL(const TDesC& aAppFileName, TUid aUid) const
// returns pointer to a matching app, or NULL if not in list
//
	{
	__ASSERT_DEBUG(iAppList, Panic(EPanicNullPointer));

	TInt index = iAppList->Count();
	if (aUid!=KNullUid)
		{// search by UID
		while(--index >= 0)
			{
			__ASSERT_DEBUG((*iAppList)[index], Panic(EDPanicNoAppHolder));
			if ((*iAppList)[index]->AppFileUid() == aUid)
				{
				(*iAppList)[index]->ScheduleForAsyncDeletion(EFalse);
				return (*iAppList)[index]; // match found
				}
			}
		}
	else
		{// search by name as no UID has been supplied
		TParse app; 
		TParse suspect;
		User::LeaveIfError(app.Set(aAppFileName,NULL,NULL));
		while (--index>=0)
			{
			__ASSERT_DEBUG((*iAppList)[index], Panic(EDPanicNoAppHolder));
			suspect.SetNoWild((*iAppList)[index]->AppFileName(), NULL, NULL);
			if (!app.Name().CompareF(suspect.Name()))
				{
				(*iAppList)[index]->ScheduleForAsyncDeletion(EFalse);
				return (*iAppList)[index]; // match found
				}
			}
		}
		
	return NULL; // no match found
	}

RApaApplication* CApaProcess::AddAppL(TApaApplicationFactory aApplicationFactory)
	{
	RApaApplication* app = new (ELeave) RApaApplication;
	CleanupClosePushL(*app);
	
	// create the app
	app->CreateApplicationL(aApplicationFactory);
	__ASSERT_DEBUG(app->Application(), Panic(EPanicNullPointer));
	app->Application()->PreDocConstructL();
	
	// add the application to the list and return a pointer to it
	__ASSERT_DEBUG(iAppList, Panic(EPanicNullPointer));
	iAppList->AppendL(app);
	
	CleanupStack::Pop(app);
	return app;
	}

EXPORT_C TPtrC CApaProcess::MainDocFileName()const
/** Returns the filename of the main document.

@return A non-modifiable pointer descriptor to the main document filename. 
For non file-based applications, the length of this descriptor is zero. */
	{
	if (iMainDocFileName)
		return *iMainDocFileName;
	
	return KNullDesC();
	}

/** Reserved for future use */
EXPORT_C void CApaProcess::CApaProcess_Reserved1()
	{
	}

/** Reserved for future use */
EXPORT_C void CApaProcess::CApaProcess_Reserved2()
	{
	}



