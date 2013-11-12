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
// apaapp.cpp
//

#include <apaapp.h> // stuff everyone will want ie most things
#include <apacln.h> // CleanupStack protection for CApaDocument
#include "APASTD.H" // Panics etc.
#include <ecom/ecom.h>
#include <ecom/implementationinformation.h>

#include "../apparc/TRACE.H"

_LIT(KApplicationLocation,"\\sys\\bin\\");

//
// CApaApplication
//

/** Constructor for CApaApplication */
EXPORT_C CApaApplication::CApaApplication()
	{
	}

/** Returns the full name and path of the application.

The default implementation returns the full path name of the application DLL.

An application can provide its own implementation. 

@return Full path name of the application.
@see CApaApplication::DllName() */
EXPORT_C TFileName CApaApplication::AppFullName()const
	{
	return DllName();
	}


/** Returns the full name and path of the loaded application DLL.

@return Full path name of the application DLL. */
EXPORT_C TFileName CApaApplication::DllName()const
	{
	ASSERT(iAppFileNameRef);
	return *iAppFileNameRef;
	}


/** Generates a unique filename based on the file name contained within the specified 
full path name.

If necessary, the function creates the directory structure that is defined 
in the specified full path name.

If the file name does not yet exist, then this is the file name chosen. If 
this file name already exists, then a file name of the form: name(nn) is generated, 
where nn are decimal digits. The value of nn is incremented until a name is 
generated that is unique within the directory structure. A minimum of two 
decimal digits is generated.

The function is used by the UI framework.

@param aFs Handle to a file server session. 
@param aRootName The full path name.
@return KErrNone if successful, otherwise one of the other system-wide error 
codes. Specifically: KErrBadName if the file name portion of the specified 
full path name has invalid format; KErrArgument if the drive, path or file 
name parts are missing from the specified full path name; KErrOverflow if 
the generated filename becomes too long; KErrNoMemory if there is insufficient 
memory to perform the operation.
@see CEikAppUi */
EXPORT_C TInt CApaApplication::GenerateFileName(RFs& aFs, TFileName& aRootName)
	{
	// check that filename is valid
	if (!aFs.IsValidName(aRootName))
		return KErrBadName;
	//
	// check that a drive, path and root filename have been specified
	TParsePtr parsePtr(aRootName);
	if (!parsePtr.DrivePresent() || !parsePtr.PathPresent() || !parsePtr.NamePresent())
		return KErrArgument;
	//
	// create the path if necessary
	TInt ret=aFs.MkDirAll(parsePtr.DriveAndPath());
	if (ret!=KErrNone && ret!=KErrAlreadyExists)
		return ret;
	//
	// Create the Rbuf object to hold a filename (return if no mem available)
	RBuf newName;
	ret = newName.Create(aRootName, KMaxFileName+8);
	if (ret!=KErrNone)
		return KErrNoMemory;
	//	
	// generate a valid filename that doesn't already exist...
	TEntry entry;
	TInt i = 1;
	_LIT(KFormatStringOne,"%S%S(%02d)%S");
	_LIT(KFormatStringTwo,"%S%S(%d)%S");
	TBuf<16> format(KFormatStringOne);
	while (aFs.Entry(newName, entry) == KErrNone)		// Continue until DoesNotExist or PathDoesNotExist, etc
		{ 
		// If the file does already exist...
		// ... then create a "/path/filename(nn)" sting and try again.
		if (i == 100)
			format = KFormatStringTwo;
			
		TPtrC driveAndPath = parsePtr.DriveAndPath();
		TPtrC name = parsePtr.Name();
		TPtrC ext = parsePtr.Ext();
		newName.Format(format, &driveAndPath, &name, i++, &ext);
		if (newName.Length() > KMaxFileName)
			{
			newName.Close();
			return KErrOverflow;
			}
		}
	
	// Set the new filename and return
	aRootName = newName;
	newName.Close();
	return KErrNone;
	}


/** Opens the .ini file associated with the application, constructs the dictionary 
store object and returns a pointer to it.

The implementation of this function is provided by the OpenIniFileLC() function. 
The function pops the pointer returned by OpenIniFileLC() from the cleanup 
stack.

@param aFs Handle to a file server session. 
@return A pointer to the dictionary store object representing the application's 
.ini file. 
@see CApaApplication::OpenIniFileLC() */
EXPORT_C CDictionaryStore* CApaApplication::OpenIniFileL(RFs& aFs)const
	{
	CDictionaryStore* store = OpenIniFileLC(aFs);
	CleanupStack::Pop(); // store
	return store;
	}

EXPORT_C CApaApplication::~CApaApplication()
	{
	if (iEComDtorKey != TUid::Null()) 
		REComSession::DestroyedImplementation(iEComDtorKey);

	iAppFileNameRef = NULL;
	}

/** Virtual function called by the framework when the application
has been launched as a server application.
Applications that wish to be used as server applications must
override this function to return their implemetation of the server.
@param aAppServer The server pointer to be set. */
EXPORT_C void CApaApplication::NewAppServerL(CApaAppServer*& /*aAppServer*/)
	{
	User::Leave(KErrNotSupported);
	}

/** Reserved for future use */
EXPORT_C void CApaApplication::CApaApplication_Reserved1()
	{
	}

/** Reserved for future use */
EXPORT_C void CApaApplication::CApaApplication_Reserved2()
	{
	}

void CApaApplication::SetAppFileNameRef(const RBuf& aFileName)
	{
	iAppFileNameRef = &aFileName;
	}



//
// TApaApplicationFactory
//

/**
Default constructor
*/

/** Constructor for TApaApplicationFactory */
EXPORT_C TApaApplicationFactory::TApaApplicationFactory()
	: iType(ETypeFunction), iData(0), iApplication(NULL)
	{
	}

/** 
Constructor.
@publishedAll
@released
@param aFunction The function from which the application is to be created.
*/
EXPORT_C TApaApplicationFactory::TApaApplicationFactory(TFunction aFunction)
	: iType(ETypeFunction), iData(reinterpret_cast<TUint>(aFunction)), iApplication(NULL)
	{
	}

/** 
Constructor. Use this constructor in preference to the constructor taking a "TUid" parameter 
if at all possible as it is much more efficient.
@publishedAll
@released
@param aEmbeddedApplicationInformation The ECOM implementation-information of the embedded application to be created.
*/
EXPORT_C TApaApplicationFactory::TApaApplicationFactory(const CImplementationInformation& aEmbeddedApplicationInformation)
	: iType(ETypeEmbeddedApplicationInformation),
	 iData(reinterpret_cast<TUint>(&aEmbeddedApplicationInformation)),
	 iApplication(NULL)
	{
	}

/** 
Constructor. Use the constructor taking a "const CImplementationInformation&" parameter in preference 
to this constructor if at all possible as it is much more efficient.
@publishedAll
@released
@param aEmbeddedApplicationUid The ECOM implementation-UID of the embedded application to be created.
*/
EXPORT_C TApaApplicationFactory::TApaApplicationFactory(TUid aEmbeddedApplicationUid)
	: iType(ETypeEmbeddedApplicationUid), iData(aEmbeddedApplicationUid.iUid), iApplication(NULL)
	{
	}

CApaApplication* TApaApplicationFactory::CreateApplicationL() const
	{
	CApaApplication* application = NULL;

	switch (iType)
		{
		case ETypeFunction:
			{
			__ASSERT_DEBUG(iData, Panic(EPanicNullPointer));
			application = (*reinterpret_cast<TFunction>(iData))();
			break;
			}
		case ETypeEmbeddedApplicationInformation:
			{
			__ASSERT_DEBUG(iData, Panic(EPanicNullPointer));
			const CImplementationInformation& embeddedApplicationInformation = *reinterpret_cast<const CImplementationInformation*>(iData);
			TUid uid = embeddedApplicationInformation.ImplementationUid();
			application = CreateEmbeddedApplicationL(uid);
			break;
			}
		case ETypeEmbeddedApplicationUid:
			{
			TUid uid = TUid::Uid(iData);
			application = CreateEmbeddedApplicationL(uid);
			break;
			}
		default:
			Panic(EPanicBadApplicationFactoryType);
		}

	return application;
	}

HBufC* TApaApplicationFactory::AppFileNameL() const
	{
	HBufC* appFileName = NULL;
	switch (iType)
		{
		case ETypeFunction:
			{
			// Assume that if the type is a function pointer then the app is not embedded (so
			// the filename is the filename of this process).
			appFileName = RProcess().FileName().AllocL();
			break;
			}
		case ETypeEmbeddedApplicationInformation:
			{
			const CImplementationInformation& embeddedApplicationInformation = *reinterpret_cast<const CImplementationInformation*>(iData);
			appFileName = FullAppFileNameL(embeddedApplicationInformation.DisplayName());
			break;
			}
		case ETypeEmbeddedApplicationUid:
			{
			const TUid uid = TUid::Uid(iData);
			HBufC* displayName = EmbeddedApplicationDisplayNameLC(uid);
			appFileName = FullAppFileNameL(*displayName);
			CleanupStack::PopAndDestroy(displayName);
			break;
			}
		default:
			Panic(EPanicBadApplicationFactoryType);
		}

	return appFileName;
	}

TUid TApaApplicationFactory::AppFileUid() const
	{
	TUid uid=KNullUid;
	switch (iType)
		{
		case ETypeFunction:
			{
			uid = RProcess().Type()[2];
			break;
			}
		case ETypeEmbeddedApplicationInformation:
			{
			const CImplementationInformation& embeddedApplicationInformation=*REINTERPRET_CAST(const CImplementationInformation*,iData);
			uid = embeddedApplicationInformation.ImplementationUid();
			break;
			}
		case ETypeEmbeddedApplicationUid:
			{
			uid = TUid::Uid(iData);
			break;
			}
		default:
			Panic(EPanicBadApplicationFactoryType);
		}
		
	return uid;
	}

HBufC* TApaApplicationFactory::FullAppFileNameL(const TDesC& aAppName)
	{
	// This was appropriately changed for data caging (binaries placed in \sys\bin\)
	TFileName fileName;
	Dll::FileName(fileName);

	TParse parse;
	parse.SetNoWild(aAppName, &KApplicationLocation, &fileName);
	return parse.FullName().AllocL();
	}

CApaApplication* TApaApplicationFactory::CreateEmbeddedApplicationL(TUid aUid)
	{ // static
	CApaApplication* const application=static_cast<CApaApplication*>(REComSession::CreateImplementationL(aUid,_FOFF(CApaApplication,iEComDtorKey)));
	const TUid appUid = application->AppDllUid();
	__ASSERT_ALWAYS(appUid==aUid, Panic(EPanicUidsDoNotMatch));
	return application;
	}


HBufC* TApaApplicationFactory::EmbeddedApplicationDisplayNameLC(TUid aUid)
	{ // static
	HBufC* displayName = NULL;

	RImplInfoPtrArray implementationArray;
	CleanupStack::PushL(TCleanupItem(CleanupImplementationArray,&implementationArray));
	REComSession::ListImplementationsL(KUidFileEmbeddedApplicationInterfaceUid,implementationArray);
	
	for (TInt i = implementationArray.Count()-1; i >= 0; --i)
		{
		const CImplementationInformation& implementationInformation=*implementationArray[i];
		if (implementationInformation.ImplementationUid().iUid==aUid.iUid)
			{
			displayName=implementationInformation.DisplayName().AllocL();
			break;
			}
		}
		
	CleanupStack::PopAndDestroy(&implementationArray);
	if (!displayName)
		User::Leave(KErrNotFound);

	CleanupStack::PushL(displayName);

	return displayName;
	}

void TApaApplicationFactory::CleanupImplementationArray(TAny* aImplementationArray)
	{ // static
	__ASSERT_DEBUG(aImplementationArray, Panic(EPanicNullPointer));
	RImplInfoPtrArray& implementationArray=*static_cast<RImplInfoPtrArray*>(aImplementationArray);
	implementationArray.ResetAndDestroy();
	implementationArray.Close();
	}
	


