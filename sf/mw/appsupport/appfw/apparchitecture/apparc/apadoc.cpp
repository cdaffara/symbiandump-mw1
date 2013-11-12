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
// apadoc.cpp
//

#include <apadoc.h> // class CApaDocument and MApaEmbeddedDocObserver
#include <apacln.h> // CleanupStack protection for CApaDocument
#include "APASTD.H" // Panics etc.


//
// Doc cleanup method
//

EXPORT_C void TApaDocCleanupItem::DoCleanup(TAny* aPtr)
	{
	__ASSERT_ALWAYS(aPtr,Panic(EPanicNoCleanupItem));
	TApaDocCleanupItem* cleanup = reinterpret_cast<TApaDocCleanupItem*>(aPtr);
	__ASSERT_ALWAYS(cleanup->iApaProcess,Panic(EPanicNoCleanupItem));//lint !e613 Possible use of null pointer - Asserted above
	cleanup->iApaProcess->DestroyDocument(cleanup->iApaDoc); //lint !e613 Possible use of null pointer - Asserted above
	}


//
// CApaDocument
//

/** Constructor for CApaDocument */
EXPORT_C CApaDocument::CApaDocument()
	{
	}

EXPORT_C CApaDocument::CApaDocument(CApaApplication& aApp,CApaProcess& aProcess)
	: iApplication(&aApp),
	iApaProcess(&aProcess)
/** Constructs the document object with the specified application and process.

Derived classes must define and implement a constructor through which both 
the associated application and process can be specified. A typical implementation 
calls this constructor through a constructor initialization list.

@param aApp The application.
@param aProcess The process.
@see CEikDocument */
	{}


EXPORT_C CApaDocument::~CApaDocument()
/** Destructor.

The implementation is empty. */
	{
	iContainer = NULL;
	iApplication = NULL;
	iApaProcess = NULL;
	}


EXPORT_C CApaDocument::TCapability CApaDocument::Capability() const
/** Gets the document's capabilities.

Capabilities are encapsulated by an instance of a TCapability class, a public 
class defined inside this class.

The default implementation returns a default TCapability object, indicating 
that the document does not support any of the defined capabilities.

If a document does support one or more of the capabilities, it should override 
this function to return a suitably initialised object.

@return The document's capabilities */
	{
	return TCapability();
	}


EXPORT_C void CApaDocument::ValidatePasswordL() const
/** Checks the document password.

The default implementation is empty.

If a document is intended to be password protected, the UI application should 
provide an implementation that forces the user to enter the password and validate 
the input.

If the document is protected by a password and the password entered by the 
user is incorrect, the function should leave with KErrLocked, otherwise it 
should just return. */
	{}


EXPORT_C CPicture* CApaDocument::GlassPictureL()
// Return handle to glass picture, creating one if not already created.
// returns NULL as glass pictures are not supported by default
/** Gets an object that can draw a representation of the document's content.

If the document supports being embedded as a glass door, then the UI application 
must provide an implementation for this function.

The default implementation raises an APPARC 8 panic.

@return A pointer to a glass door. */
	{
	Panic(EPanicNoGlassDoorMethodSupplied);
	//
	return NULL;
	}


EXPORT_C void CApaDocument::ExternalizeL(RWriteStream& /*aStream*/)const
	{}


EXPORT_C void CApaDocument::OpenFileL(CFileStore*&, RFile&)
	{
	}

EXPORT_C void CApaDocument::Reserved_2()
	{}


EXPORT_C CApaDocument::TCapability::TCapability()
	:iCapability(0),TCapability_Reserved1(0)
/** Constructs a default capability object.

All capabilities are marked as "not supported". */
	{}

	
//
// MApaEmbeddedDocObserver
//

/** Constructor for MApaEmbeddedDocObserver */
EXPORT_C MApaEmbeddedDocObserver::MApaEmbeddedDocObserver()
	{
	}

/** Reserved for future use */
EXPORT_C void MApaEmbeddedDocObserver::MApaEmbeddedDocObserver_Reserved1()
	{
	}

/** Reserved for future use */
EXPORT_C void MApaEmbeddedDocObserver::MApaEmbeddedDocObserver_Reserved2()
	{
	}




