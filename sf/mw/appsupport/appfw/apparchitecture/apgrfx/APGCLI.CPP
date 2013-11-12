// Copyright (c) 1997-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// apgcli.cpp
//

#include "../apserv/APSCLSV.H"
#include "../apserv/apsserv.h"

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
#include "../apgrfx/apgcommonutils.h"
#endif

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#if !defined(__APA_INTERNAL_H__)
#include "apainternal.h"
#endif
#if !defined(__APGICNFL_PARTNER_H__)
#include "apgicnflpartner.h"
#endif
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS
#include "APGCLI.H"
#include "APGSTD.H"
#include "APGICNFL.H"
#include "APGPRIV.H"
#include "apprivate.h"

const TUint KInitialIconBufSize = sizeof(TSize)*6 + sizeof(TInt);
const TUint KInitialViewDataBufSize = sizeof(TApaAppViewInfo)*6 + sizeof(TInt);
const TUint KInitialOwnedFilesBufSize = sizeof(TFileName)*6 + sizeof(TInt);
const TUint KDefaultBufSize = 0x400;
const TUint KAsyncMessageSlots = 4; // One slot each for, RApaLsSession::SetNotify, RApaLsSession::
                                    // RegisterListPopulationCompleteObserver and RApaLsSession::NotifyOnDataMappingChange
                                    // CAsyncFileRecognition::Start makes an asynch request of apparc server.

extern void CleanupAppServiceArray(TAny* aServiceArray);

IMPORT_C extern const TInt KMinApplicationStackSize;

/** 
@internalComponent
*/
class MArrayFiller
	{
public:
	virtual void AppendItemFromStreamL(RReadStream& aReadStream) = 0;
	};

/** 
@internalComponent
*/
NONSHARABLE_CLASS(TSizeArrayFiller) : public MArrayFiller
	{
public:
	inline TSizeArrayFiller(CArrayFixFlat<TSize>& aArray) : iArray(aArray) {}
public: // from MArrayFiller
	void AppendItemFromStreamL(RReadStream& aReadStream);
private:
	CArrayFixFlat<TSize>& iArray;
	};

/** 
@internalComponent
*/
NONSHARABLE_CLASS(TViewDataArrayFiller) : public MArrayFiller
	{
public:
	inline TViewDataArrayFiller(CApaAppViewArray& aArray) : iArray(aArray) {}
public: // from MArrayFiller
	void AppendItemFromStreamL(RReadStream& aReadStream);
private:
	CApaAppViewArray& iArray;
	};

/** 
@internalComponent
*/
NONSHARABLE_CLASS(TDesCArrayFiller) : public MArrayFiller
	{
public:
	inline TDesCArrayFiller(CDesCArray& aArray) : iArray(aArray) {}
public: // from MArrayFiller
	void AppendItemFromStreamL(RReadStream& aReadStream);
private:
	CDesCArray& iArray;
	};

// TSizeArrayFiller

void TSizeArrayFiller::AppendItemFromStreamL(RReadStream& aReadStream)
	{
	TSize size;
	size.iWidth = aReadStream.ReadUint32L();
	size.iHeight= aReadStream.ReadUint32L();
	iArray.AppendL(size);
	}

// TViewDataArrayFiller

void TViewDataArrayFiller::AppendItemFromStreamL(RReadStream& aReadStream)
	{
	TApaAppViewInfo info;
	aReadStream >> info;
	iArray.AppendL(info);
	}

// TDesCArrayFiller

void TDesCArrayFiller::AppendItemFromStreamL(RReadStream& aReadStream)
	{
	TFileName info;
	aReadStream >> info;
	iArray.AppendL(info);
	}

/** 
@internalComponent
*/
EXPORT_C TUint MinApplicationStackSize()
	{
	return KMinApplicationStackSize;
	}


//
// class RApaLsSession
//

EXPORT_C RApaLsSession::RApaLsSession()
	: iExtension(NULL)
	{}



/** Connects a client to the application architecture server, creating a session 
with it.

@return KErrNone if successful; otherwise, one of the system-wide error codes. 
*/
EXPORT_C TInt RApaLsSession::Connect()
	{
	TInt r=CreateSession(KAppListServerName,Version(),KAsyncMessageSlots);
	return(r); 
	}

EXPORT_C void RApaLsSession::Close()
/** 
Closes the session. Needs to be called to avoid memory leaks.
@publishedAll
@released
*/
	{
	CancelRecognizeFiles();
	RHandleBase::Close();
	}


/** Gets the version of the application architecture server.

@return The version number. 
*/
EXPORT_C TVersion RApaLsSession::Version(void) const
	{
	return(TVersion(KAppListServMajorVersionNumber,KAppListServMinorVersionNumber,KAppListServBuildVersionNumber));
	}


/** Gets the total number of applications.

Control panel applications are excluded.

@param aCount On return, the total number of applications.
@return KErrNone if successful, otherwise one of the system-wide error codes. 
*/
EXPORT_C TInt RApaLsSession::AppCount(TInt& aCount) const
	{
	return DoAppCount(aCount, EAppListServAppCount);
	}


/** Gets the total number of embeddable applications.

Control panel applications are excluded.

@param aCount On return, the total number of embeddable applications.
@return KErrNone, if successful, otherwise one of the system-wide error codes. 
*/
EXPORT_C TInt RApaLsSession::EmbeddableAppCount(TInt& aCount) const
	{
	return DoAppCount(aCount, EAppListServEmbedCount);
	}


TInt RApaLsSession::DoAppCount(TInt& aCount,TInt aCommand) const
// returns the number of embeddable apps in the server side list
	{
	__ASSERT_DEBUG(aCommand==EAppListServEmbedCount || aCommand==EAppListServAppCount,Panic(EDPanicWrongCommand));
	const TInt returnValue = SendReceiveWithReconnect(aCommand,TIpcArgs());
	if (returnValue < 0)
		return returnValue;
	
	aCount = returnValue;
	return KErrNone;
	}



/** Initialises the process of getting all applications in the cached list.

Control panel applications are excluded.

A call to this function is followed by subsequent and repeated calls to GetNextApp() 
to retrieve all applications in the cached list.

@return KErrNone, if successful, otherwise one of the system-wide error codes. 
*/
EXPORT_C TInt RApaLsSession::GetAllApps() const
	{
	return GetAllApps(0);
	}


/** Initialises the process of getting all applications in the cached list.

Control panel applications are excluded.

A call to this function is followed by subsequent and repeated calls to GetNextApp() 
to retrieve all applications supporting aScreenMode in the cached list.

@param aScreenMode Only applications which define a view supporting aScreenMode
will be returned by subsequent calls to GetNextApp(). If an application does not
define views in it's application information file, only screen mode 0 is supported.
@return KErrNone, if successful, otherwise one of the system-wide error codes. 
*/
EXPORT_C TInt RApaLsSession::GetAllApps(TInt aScreenMode) const
	{
	return GetFilteredApps(TApaAppCapability::EControlPanelItem, 0, aScreenMode); // exclude control panel apps
	}



/** Initialises the process of getting all embeddable applications from the cached list.

Control panel applications are excluded.

A call to this function is followed by subsequent and repeated calls to GetNextApp() 
to retrieve embeddable applications in the cached list.

Only applications which specify KAppEmbeddable or KAppEmbeddableOnly in their
application information file will be returned by subsequent calls to GetNextApp().

@return KErrNone, if successful, otherwise one of the system-wide error codes. 
*/
EXPORT_C TInt RApaLsSession::GetEmbeddableApps() const
	{
	return GetEmbeddableApps(0);
	}



/** Initialises the process of getting embeddable applications from the cached list
that support the specified screen mode.

Control panel applications are excluded.

A call to this function is followed by subsequent and repeated calls to GetNextApp() 
to retrieve embeddable applications in the cached list.

Only applications which specify KAppEmbeddable or KAppEmbeddableOnly in their
application information file will be returned by subsequent calls to GetNextApp().

@param aScreenMode Only embeddable applications which define a view supporting
aScreenMode will be returned by subsequent calls to GetNextApp(). If an application
does not define views in it's application information file, only screen mode 0 is supported.
@return KErrNone, if successful, otherwise one of the system-wide error codes. 
*/
EXPORT_C TInt RApaLsSession::GetEmbeddableApps(TInt aScreenMode) const
	{
	TApaEmbeddabilityFilter filter;
	filter.AddEmbeddability(TApaAppCapability::EEmbeddable);
	filter.AddEmbeddability(TApaAppCapability::EEmbeddableOnly);
	return GetFilteredApps(filter, aScreenMode);
	}



/** Initialises the process of getting all applications matching aFilter in the
cached list.

Control panel applications are excluded.

A call to this function is followed by subsequent and repeated calls to GetNextApp() 
to retrieve all applications matching aFilter in the cached list.

@param aFilter Defines the filter to be applied to the cached list.
@return KErrNone, if successful, otherwise one of the system-wide error codes. 
*/
EXPORT_C TInt RApaLsSession::GetFilteredApps(const TApaEmbeddabilityFilter& aFilter) const
	{
	return GetFilteredApps(aFilter, 0);
	}



/** Initialises the process of getting all applications matching aFilter in the
cached list.

Control panel applications are excluded.

A call to this function is followed by subsequent and repeated calls to GetNextApp() 
to retrieve all applications matching aFilter in the cached list.

@param aFilter Defines the filter to be applied to the cached list.
@param aScreenMode Only applications which define a view supporting aScreenMode
will be returned by subsequent calls to GetNextApp(). If an application does not
define views in it's application information file, only screen mode 0 is supported.
@return KErrNone, if successful, otherwise one of the system-wide error codes. 
*/
EXPORT_C TInt RApaLsSession::GetFilteredApps(const TApaEmbeddabilityFilter& aFilter, TInt aScreenMode) const
	{
	const TPckgC<TApaEmbeddabilityFilter> filter(aFilter);
	return SendReceiveWithReconnect(EAppListServInitFilteredEmbedList,TIpcArgs(aScreenMode,&filter));
	}



/** Initialises the process of getting all applications matching the specified
application attributes.

A call to this function is followed by subsequent and repeated calls to GetNextApp() 
to retrieve all applications matching the filter in the cached list.

Attributes are defined by TApaAppCapability::TCapabilityAttribute

@param aCapabilityAttributeMask Specifies the attributes whose values will be
used to filter the cached list. If the mask specifies more than one attribute,
all associated attribute values must match.
@param aCapabilityAttributeValue Specifies the attribute values for each attribute
identified by the mask.
@return KErrNone, if successful, otherwise one of the system-wide error codes.
@see TCapabilityAttribute 
*/
EXPORT_C TInt RApaLsSession::GetFilteredApps(TUint aCapabilityAttributeMask, TUint aCapabilityAttributeValue) const
	{
	return GetFilteredApps(aCapabilityAttributeMask, aCapabilityAttributeValue, 0);
	}



/** Initialises the process of getting all applications matching the specified
application attributes.

A call to this function is followed by subsequent and repeated calls to GetNextApp() 
to retrieve all applications matching the filter in the cached list.

Attributes are defined by TApaAppCapability::TCapabilityAttribute

@param aCapabilityAttributeMask Specifies the attributes whose values will be
used to filter the cached list. If the mask specifies more than one attribute,
all associated attribute values must match.
@param aCapabilityAttributeValue Specifies the attribute values for each attribute
identified by the mask.
@param aScreenMode Only applications which define a view supporting aScreenMode
will be returned by subsequent calls to GetNextApp(). If an application does not
define views in it's application information file, only screen mode 0 is supported.
@return KErrNone, if successful, otherwise one of the system-wide error codes.
@see TCapabilityAttribute 
*/
EXPORT_C TInt RApaLsSession::GetFilteredApps(TUint aCapabilityAttributeMask, TUint aCapabilityAttributeValue, TInt aScreenMode) const
	{
	return SendReceiveWithReconnect(EAppListServInitAttrFilteredList,TIpcArgs(aScreenMode, aCapabilityAttributeMask, aCapabilityAttributeValue));
	}



/** Initialises the process of getting server applications in the cached list.

Control panel applications are excluded.

A call to this function is followed by subsequent and repeated calls to GetNextApp() 
to retrieve server applications in the cached list.

Only applications which specify one or more services in their application
information file will be returned by subsequent calls to GetNextApp().

@return KErrNone, if successful, otherwise one of the system-wide error codes.
@publishedPartner
@released
*/
EXPORT_C TInt RApaLsSession::GetServerApps(TUid aServiceUid) const
	{
	return GetServerApps(aServiceUid, 0);
	}

/** Initialises the process of getting server applications in the cached list.

Control panel applications are excluded.

A call to this function is followed by subsequent and repeated calls to GetNextApp() 
to retrieve server applications in the cached list.

Only applications which specify one or more services in their application
information file will be returned by subsequent calls to GetNextApp().

@param aScreenMode Only server applications which define a view supporting
aScreenMode will be returned by subsequent calls to GetNextApp(). If an application
does not define views in its application information file, only screen mode 0 is supported.
@return KErrNone, if successful, otherwise one of the system-wide error codes.
@publishedPartner
@released
*/
EXPORT_C TInt RApaLsSession::GetServerApps(TUid aServiceUid, TInt aScreenMode) const
	{
	return SendReceiveWithReconnect(EAppListServInitServerAppList,TIpcArgs(aScreenMode, aServiceUid.iUid));
	}

/** Gets information about the next application or embeddable application from 
the cached list.

A sequence of calls to this function must always be preceded by a call to 
one of GetAllApps(), GetEmbeddableApps() or GetFilteredApps().

@param aInfo On return, contains application information.
@return KErrNone if successful; ENoMoreAppsInList, if there are no more applications 
in the list; EAppListInvalid if the server's initial population of the list has
not completed; otherwise one of the other system wide error codes. 
*/
EXPORT_C TInt RApaLsSession::GetNextApp(TApaAppInfo& aInfo) const
	{
	return GetNextApp(aInfo,0);
	}

/** Gets information about the next application or embeddable application from 
the cached list.

A sequence of calls to this function must always be preceded by a call to 
one of GetAllApps(), GetEmbeddableApps() or GetFilteredApps().

@param aInfo On return, contains application information.
@param aScreenMode This parameter is ignored.
@return KErrNone if successful; ENoMoreAppsInList, if there are no more applications 
in the list; EAppListInvalid if the server's initial population of the list has
not completed; otherwise one of the other system wide error codes. 
*/
EXPORT_C TInt RApaLsSession::GetNextApp(TApaAppInfo& aInfo,TInt aScreenMode) const
	{
	TPckg<TApaAppInfo> info(aInfo);
	TInt err = SendReceiveWithReconnect(EAppListServGetNextApp, TIpcArgs(aScreenMode,&info));
	if (!err)
		aInfo = info();
	else if (err==KErrNotFound)
		err = ENoMoreAppsInList;
	else if (err==KErrCorrupt)
		err = EAppListInvalid;
	
	return err;
	}



/** Gets information about the application with the specified UID.

@param aInfo On return, contains the application information, if an application 
with the specified UID is found. If no matching application is found, then 
this object is not changed.
@param aAppUid The application specific UID.
@return KErrNone if successful; KErrNotFound if a matching entry could not be found; 
otherwise one of the other system wide error codes. 
*/
EXPORT_C TInt RApaLsSession::GetAppInfo(TApaAppInfo& aInfo, TUid aAppUid) const
	{
	TPckg<TApaAppInfo> info(aInfo);
	return SendReceiveWithReconnect(EAppListServGetAppInfo, TIpcArgs(aAppUid.iUid,&info));
	} //lint !e1764 Suppress reference parameter 'aInfo' could be declared const ref
	
/** Sets the short caption of the application.

Overrides the short caption specified in the localizable resource file for this application.
Short captions set using this API will only take effect until the next device reset.

@param aShortCaption The short caption of the application. The maximum length allowed is KApaMaxAppCaption.
@param aLanguage The language corresponding to the caption. If this is ELangNone the caption is used
for all languages for which a language specific short caption has not been set.
@param aAppUid The uid of the application.
@return KErrNone if successful, otherwise one of the system wide error codes.
*/ 
EXPORT_C TInt RApaLsSession::SetAppShortCaption(const TDesC& aShortCaption, TLanguage aLanguage, TUid aAppUid)
	{
	if (aShortCaption.Length() > KApaMaxAppCaption || aShortCaption.Length() == 0)
		return KErrArgument;

	return SendReceiveWithReconnect(EAppListServSetAppShortCaption, TIpcArgs(aAppUid.iUid, &aShortCaption, aLanguage));
	} //lint !e1762 Suppress member function could be made const


/** Gets the default screen-number of the application with the specified UID.

@param aDefaultScreenNumber On return, contains the default screen-number, if an application 
with the specified UID is found. If no matching application is found, then 
this object is not changed.
@param aAppUid The application specific UID.
@return KErrNone if successful; KErrNotFound if a matching entry could not be found; 
otherwise one of the other system wide error codes. 
*/
EXPORT_C TInt RApaLsSession::GetDefaultScreenNumber(TInt& aDefaultScreenNumber, TUid aAppUid) const
	{
	const TInt result = SendReceiveWithReconnect(EAppListServGetDefaultScreenNumber, TIpcArgs(aAppUid.iUid));
	if (result < 0)
		return result;

	aDefaultScreenNumber = result;
	return KErrNone;
	}


/** Gets the capabilities of the application with the specified UID.

@param aCapabilityBuf A modifiable descriptor that, on return, contains the 
application's capability information. The data returned in the descriptor 
is mapped by the TApaAppCapability class. If no matching application is found, 
then the content of this descriptor is not changed.
@param aAppUid The application specific UID.
@return KErrNone, if successful; KErrNotFound, if no matching entry can be found; otherwise 
one of the other system wide error codes. 
*/
EXPORT_C TInt RApaLsSession::GetAppCapability(TDes8& aCapabilityBuf,TUid aAppUid) const
	{
	return SendReceiveWithReconnect(EAppListServGetAppCapability,TIpcArgs(&aCapabilityBuf,aAppUid.iUid));
	}


/** Gets the UID of an application that can handle the specified data (MIME) type.

If no application can be found, the function returns the UID of the preferred 
default handler. If none of the default handlers can handle the data type, 
then a NULL UID is returned.

@param aDataType The data (MIME) type.
@param aAppUid On return, the UID of the application that can handle the data 
(MIME) type; this may be NULL.
@return KErrNone, if successful; otherwise one of the other system-wide error 
codes. 
*/
EXPORT_C TInt RApaLsSession::AppForDataType(const TDataType& aDataType, TUid& aAppUid) const
	{
	const TPckgC<TDataType> dataType(aDataType);
	TPckg<TUid> uid(aAppUid);
	return SendReceiveWithReconnect(EAppListServAppForDataType,TIpcArgs(&dataType,&uid));
	} //lint !e1764 Suppress reference parameter 'aAppUid' could be declared const ref


/** Gets the available icon sizes for the application with the specified UID.

@param aAppUid The application specific UID.
@param aArrayToFill On return, the array contains all of the available icon 
sizes.
@return KErrNone, if successful; KErrNotFound, if no matching application can 
be found; KErrNotSupported, if the application provides icons in non-MBM format;
otherwise one of the other system wide error codes. 
*/
EXPORT_C TInt RApaLsSession::GetAppIconSizes(TUid aAppUid, CArrayFixFlat<TSize>& aArrayToFill) const
	{
	TRAPD(error, DoGetAppIconSizesL(aAppUid,aArrayToFill));
	return error;
	}

void RApaLsSession::DoGetAppIconSizesL(TUid aAppUid, CArrayFixFlat<TSize>& aArrayToFill) const
	{
	TSizeArrayFiller arrayFiller(aArrayToFill);
	FetchArrayL(arrayFiller, aAppUid, EAppListServGetAppIconSizes, KInitialIconBufSize);
	}

/**
This method implementes a generic mechanism for retrieving data items from the AppArc server.
The data items can be either TSize, TApaAppViewInfo, or TDesC (see MArrayFiller).
@internalTechnology
*/
void RApaLsSession::FetchArrayL(MArrayFiller& aArray, TUid aAppUid, TInt aOpcode, TInt aInitialBufSize) const
	{
	// Create a buffer to recieve the data in
	CBufFlat* buffer = CBufFlat::NewL(aInitialBufSize);
	CleanupStack::PushL(buffer);
	
	// Set its size and create a pointer for writing to it
	buffer->ExpandL(0,aInitialBufSize);
	TPtr8 bufPtr = buffer->Ptr(0);
	
	// Get the data from the AppArc server
	
	const TInt sizeRequired = SendReceiveWithReconnect(aOpcode,TIpcArgs(aAppUid.iUid,buffer->Size(),&bufPtr));
	User::LeaveIfError(sizeRequired); 	// Negative values are error codes
	if (sizeRequired > 0)	// If the initial buffer was too small...
		{
		// (This should really never happen - the code below is a backup for release builds only
		ASSERT(0); // so panic any debug builds)
		
		// ...create a bigger one and try again
		CleanupStack::PopAndDestroy(buffer);
		buffer = CBufFlat::NewL(sizeRequired);
		CleanupStack::PushL(buffer);
		buffer->ExpandL(0, sizeRequired);
		bufPtr.Set(buffer->Ptr(0));
		User::LeaveIfError(SendReceiveWithReconnect(aOpcode, TIpcArgs(aAppUid.iUid, buffer->Size(), &bufPtr)));
		}
	
	// Create a read stream for reading from the buffer	
	RBufReadStream readStream;
	CleanupClosePushL(readStream);
	readStream.Open(*buffer);
	
	// Get the item count from the stream and popoulate the array
	const TInt count = readStream.ReadUint32L();
	for (TInt i = 0; i < count; ++i)
		aArray.AppendItemFromStreamL(readStream);

	CleanupStack::PopAndDestroy(&readStream);
	CleanupStack::PopAndDestroy(buffer);
	}


/** Gets the nearest matching application icon for the application with the specified 
UID.

The function gets the icon whose size matches the specified size. If there 
is no exact match, then the function gets the closest smaller icon.

This function should be used in preference to the TInt GetAppIcon(TUid,TInt,CApaMaskedBitmap&); 
overload.

@param aAppUid The application specific UID.
@param aSize The required size of the icon.
@param aAppBitmap On return, the application icon.
@return KErrNone, if successful; KErrNotFound, if a matching entry could not be found, or no 
icon equal to or smaller than the specified size can be found; KErrNotSupported, if the
application provides icons in non-MBM format; otherwise one of the other system wide
error codes. 
*/
EXPORT_C TInt RApaLsSession::GetAppIcon(TUid aAppUid, TSize aSize, CApaMaskedBitmap& aAppBitmap) const
	{
	SReturnData_AppIconByUidAndSize returnData = {0,0};
	TPckg<SReturnData_AppIconByUidAndSize> returnData_asDescriptor(returnData);
	TInt error = SendReceiveWithReconnect(EAppListServAppIconByUidAndSize, TIpcArgs(aAppUid.iUid,aSize.iWidth,aSize.iHeight,&returnData_asDescriptor));
	if (!error)
		{
		error = aAppBitmap.Duplicate(returnData.iIcon);
		if (!error)
			error = aAppBitmap.Mask()->Duplicate(returnData.iIconMask);
		}

	return error;
	}


/** 
 gets the bitmap handles from the Server, forms these up into a CApaMaskedBitmap

Sets aAppBitmap to be the small, medium or large app icon of the app with uid 
aAppUid, when aSize=0, 1 or 2 respectively.
Panics the caller if a different index is specified.
The overload which takes a TSize should be used instead.

@deprecated 
@param aAppUid The application specific UID.
@param aSize The required size of the icon.
@param aAppBitmap On return, the application icon.
@return KErrNone, if successful; KErrNotFound, if a matching entry could not be found, or no 
icon equal to or smaller than the specified size can be found; KErrNotSupported, if the 
application provides icons in non-MBM format, otherwise one of the other system wide 
error codes.
*/
EXPORT_C TInt RApaLsSession::GetAppIcon(TUid aAppUid, TInt aSize, CApaMaskedBitmap& aAppBitmap) const
	{
	__ASSERT_ALWAYS((aSize>=0) && (aSize<3), Panic(EDPanicBadIconSize));
	SReturnData_AppIconByUid returnData = {0,0};
	TPckg<SReturnData_AppIconByUid> returnData_asDescriptor(returnData);
	TInt error = SendReceiveWithReconnect(EAppListServAppIconByUid,TIpcArgs(aAppUid.iUid,aSize,&returnData_asDescriptor));
	if (!error)
		{
		error = aAppBitmap.Duplicate(returnData.iIcon);
		if (!error)
			error = aAppBitmap.Mask()->Duplicate(returnData.iIconMask);
		}
		
	return error;
	}


/** Gets an open shareable read only file handle to the application icon file for the 
application with the specified UID. 

An icon file can only be defined by applications providing an application registration file.

An icon file may be in any graphics format and contain one or more icons.

On entering this function, aFile must be non-open. It is recommended that aFile is 
pushed onto the cleanup-stack (via CleanupClosePushL()) before this function is called.

@param aAppUid The application specific UID.
@param aFile On return, a read only open file handle to the icon file.
@return KErrNone, if successful; KErrNotFound, if a matching application could not be found, 
or an icon filename was not defined; otherwise one of the other system wide error codes. 
@see GetAppIcon 
*/ 
EXPORT_C TInt RApaLsSession::GetAppIcon(TUid aAppUid, RFile& aFile) const 
	{
	__ASSERT_ALWAYS(aFile.SubSessionHandle() == KNullHandle, Panic(EDPanicHandleAlreadySet));
	TPckgBuf<TInt> fileHandle;
	TInt sessionHandleOrErrorCode = SendReceiveWithReconnect(EAppListServAppIconFileHandle, TIpcArgs(aAppUid.iUid, &fileHandle));
	if (sessionHandleOrErrorCode >= KErrNone)
	    {
	    sessionHandleOrErrorCode = aFile.AdoptFromServer(sessionHandleOrErrorCode, fileHandle());
	    }
	return sessionHandleOrErrorCode;
	}   


/** Gets the data (MIME) type of the data in the specified file and gets the UID 
of an application that can handle this type.

@param aFileName The name of the file containing the data.
@param aAppUid On return, the UID of the application that can handle the data 
(MIME) type; this may be NULL.
@param aDataType On return, the data (MIME) type.
@return KErrNone, if successful; otherwise one of the other system-wide error 
codes. */
EXPORT_C TInt RApaLsSession::AppForDocument(const TDesC& aFileName, TUid& aAppUid, TDataType& aDataType) const
	{
	return DoAppForDocumentOptionallySpecifyingService(aFileName, TUid::Null(), aAppUid, aDataType, EAppListServAppForDocument);
	}


/** Gets the data (MIME) type of the data in the specified file and gets the UID 
of an application that can handle this type.

@param aFile The file containing the data. Before this function can be called,
the file server session which owns this file handle must first be marked as shareable by 
calling RFs::ShareProtected().
@param aAppUid On return, the UID of the application that can handle the data 
(MIME) type; this may be NULL.
@param aDataType On return, the data (MIME) type.
@return KErrNone, if successful; otherwise one of the other system-wide error 
codes. 
*/
EXPORT_C TInt RApaLsSession::AppForDocument(const RFile& aFile, TUid& aAppUid, TDataType& aDataType) const
	{
	return DoAppForDocumentOptionallySpecifyingService(aFile, TUid::Null(), aAppUid, aDataType, EAppListServAppForDocumentPassedByFileHandle);
	}


/** Tests whether the file is a native executable (DLL or EXE).

@param aFileName The name of the file containing the data.
@param aProgram On return, true, if the file contains application code; false, 
otherwise.
@return KErrNone, if successful; otherwise one of the other system-wide error 
codes. 
*/
EXPORT_C TInt RApaLsSession::IsProgram(const TDesC& aFileName, TBool& aProgram) const
	{
	_LIT(KLitSysBin, "\\sys\\bin\\");
	aProgram=(TParsePtrC(aFileName).Path().CompareF(KLitSysBin)==0);
	return KErrNone;
	}


/** Gets the preferred number of bytes of data to read from a file for the purpose 
of recognizing the data type.

This should be used to determine the size of buffer to pass to the 3-parameter 
overload of RecognizeData() or to the 4-parameter overload of RecognizeSpecificData().

@param aPreferredBufSize On return, contains either the largest buffer size required 
by any of the currently installed data-recognizers, or the value that would be 
returned by GetMaxDataBufSize(), whichever is less.
@return KErrNone, if successful; otherwise one of the other system-wide error 
codes.
@see GetMaxDataBufSize() */
EXPORT_C TInt RApaLsSession::GetPreferredBufSize(TInt& aPreferredBufSize) const
	{
	const TInt preferredBufSize = SendReceiveWithReconnect(EAppListServPreferredBufSize, TIpcArgs());
	if (preferredBufSize < KErrNone)
		return preferredBufSize; // it's an error

	aPreferredBufSize = preferredBufSize;
	return KErrNone;
	}


/** Gets the maximum size of the data that can be read from a file for the purpose 
of recognizing the data type.

To determine the size of buffer to pass to the 3-parameter overload of RecognizeData() 
or to the 4-parameter overload of RecognizeSpecificData(), use GetPreferredBufSize() 
rather than this function.

@param aBufSize On return, contains the maximum size.
@return KErrNone, if successful; otherwise one of the other system-wide error 
codes.
@see SetMaxDataBufSize()
@see GetPreferredBufSize() */
EXPORT_C TInt RApaLsSession::GetMaxDataBufSize(TInt& aBufSize) const
	{
	const TInt returnValue = SendReceiveWithReconnect(EAppListServGetBufSize, TIpcArgs());
	if (returnValue < 0)
		return returnValue;

	aBufSize = returnValue;
	return KErrNone;
	}


/** Sets the maximum size of the data that can be read from a file for the purpose 
of recognizing the data type.

The value is not used when the client explicitly supplies a buffer, for example 
in calls to RecognizeData() and RecognizeSpecificData(), but is used in the 
implementation of functions such as StartDocument() and CreateDocument().

Unless explicitly set, a default value of KApaAppListServMaxBuffer is used.

@param aBufSize The maximum size of data to be read.
@return KErrNone, if successful; otherwise one of the other system-wide error 
codes. 
@see CreateDocument()
@see StartDocument()
@see RecognizeData()
@see RecognizeSpecificData() 
*/
EXPORT_C TInt RApaLsSession::SetMaxDataBufSize(TInt aBufSize)
	{
	return SendReceiveWithReconnect(EAppListServSetBufSize,TIpcArgs(aBufSize));
	} //lint !e1762 Suppress member function could be made const


/** Gets the icon for the specified view published by the application that has 
the specified UID.

The icon returned is that which is closest in size to the specified size.

@param aAppUid The application specific UID.
@param aViewUid The UID identifying the view.
@param aSize The requested size of the icon.
@param aViewBitmap On return, the icon that is closest in size to the requested 
size.
@return KErrNone, if successful; KErrNotFound, if no matching application nor matching view 
can be found; KErrNotSupported, if the application provides icons in non-MBM format;
otherwise one of the other system wide error codes. 
*/
EXPORT_C TInt RApaLsSession::GetAppViewIcon(TUid aAppUid, TUid aViewUid, const TSize& aSize, CApaMaskedBitmap& aViewBitmap) const
	{
	const TApaAppViewIconSizeData appViewIconSizeData(aAppUid, aViewUid, aSize);
	const TPckgC<TApaAppViewIconSizeData> inputData(appViewIconSizeData);
	SReturnData_ViewIconByUidAndSize returnData = {0,0};
	TPckg<SReturnData_ViewIconByUidAndSize> returnData_asDescriptor(returnData);
	TInt error = SendReceiveWithReconnect(EAppListServViewIconByUidAndSize,TIpcArgs(&inputData,&returnData_asDescriptor));
	if (!error)
		{
		error = aViewBitmap.Duplicate(returnData.iIcon);
		if (!error)
			error = aViewBitmap.Mask()->Duplicate(returnData.iIconMask);
		}
		
	return error;
	}


/** Gets the views published by the application that has the specified UID.

Information on each view is contained in a TApaAppViewInfo object, and this 
set of objects is put into the array supplied by the caller.

@param aAppViews On return, the array contains information on all of the views 
published by the specified application.
@param aAppUid The application specific UID.
@return KErrNone, if successful; KErrNotFound, if no matching application can 
be found; otherwise one of the other system wide error codes.
@see TApaAppViewInfo 
*/
EXPORT_C TInt RApaLsSession::GetAppViews(CApaAppViewArray& aAppViews, TUid aAppUid) const
	{
	TRAPD(error,DoGetAppViewsL(aAppViews,aAppUid));
	return error;
	}

void RApaLsSession::DoGetAppViewsL(CApaAppViewArray& aArrayToFill, TUid aAppUid) const
	{
	TViewDataArrayFiller arrayFiller(aArrayToFill);
	FetchArrayL(arrayFiller, aAppUid, EAppListServGetAppViews, KInitialViewDataBufSize);
	}


/** Gets the list of file names for which the application with the specified 
UID claims ownership.

The list is written to a descriptor array supplied by the caller.

Note that if the function fails due to lack of memory, the array is left in 
an undefined state.

@param aAppOwnedFiles On return, the descriptor array contains the file names.
@param aAppUid The application specific UID.
@return KErrNone, if successful; KErrNotFound, if no matching application can 
be found; otherwise one of the other system wide error codes. 
*/
EXPORT_C TInt RApaLsSession::GetAppOwnedFiles(CDesCArray& aAppOwnedFiles, TUid aAppUid) const
	{
	TRAPD(error,DoGetAppOwnedFilesL(aAppOwnedFiles,aAppUid));
	return error;
	}


/** Gets the number of icons defined by the app that has the specified UID

Applications that don't define icons in their application information file will
return an aCount value of zero when this function is called.

@param aAppUid The application specific UID
@param aCount On return, contains the number of icons defined by the application
@return KErrNone, if successful; KErrNotFound, if a matching application could not be found;
KErrNotSupported, if the application provides icons in non-MBM format; otherwise one of 
the other system-wide error codes. 
*/
EXPORT_C TInt RApaLsSession::NumberOfOwnDefinedIcons(TUid aAppUid, TInt& aCount) const
	{
	TPckgBuf<TInt> pckg;
	const TInt err = SendReceiveWithReconnect(EAppListServNumberOfOwnDefinedIcons,TIpcArgs(aAppUid.iUid,&pckg));
	if (!err)
		aCount = pckg();

	return err;
	}


/** Gets the full filename of a file containing application icons for the
application with the specified UID.

An icon file can only be defined by applications providing an application registration file.

An icon file may be in any graphics format and contain one or more icons.

@param aAppUid The application specific UID.
@param aFullFileName On return, the full filename of a file containing one or more
application icons. Returns a pointer to the filename and transfers ownership to the caller.
@return KErrNone, if successful; KErrNotFound, if a matching application could not be found,
or an icon filename was not defined; KErrNotSupported, if the application does not
provide an application registration file; otherwise one of the other system wide error codes. 
*/
EXPORT_C TInt RApaLsSession::GetAppIcon(TUid aAppUid, HBufC*& aFullFileName) const
	{
	TFileName fileName;
	TPckg<TFileName> filenamePckg(fileName);
	const TInt err = SendReceiveWithReconnect(EAppListServAppIconFileName, TIpcArgs(aAppUid.iUid, &filenamePckg));
	if (!err)
		{
		HBufC* fullFileName = HBufC::New(fileName.Length());
		if (fullFileName == NULL)
			return KErrNoMemory;
		else
			{
			*fullFileName = fileName;
			aFullFileName = fullFileName; // ownership transferred to caller
			}
		}
		
	return err;
	}


/** Gets the full filename of a file containing view-specific icons for the application
with the specified UID and view.

A file containing view-specific icons can only be defined by applications providing
an application registration file.

A view icon file may be in any graphics format and contain one or more view icons.

@param aAppUid The application specific UID.
@param aViewUid The UID identifying the view.
@param aFullFileName On return, the full filename of a file containing one or more
view icons. Returns a pointer to the filename and transfers ownership to the caller.
@return KErrNone, if successful; KErrNotFound, if no matching application nor matching view
could be found, or a view icon filename was not defined; KErrNotSupported, if the application
does not provide an application registration file; otherwise one of the other system wide error codes. 
*/
EXPORT_C TInt RApaLsSession::GetAppViewIcon(TUid aAppUid, TUid aViewUid, HBufC*& aFullFileName) const
	{
	TFileName fileName;
	TPckg<TFileName> filenamePckg(fileName);
	const TInt err = SendReceiveWithReconnect(EAppListServAppViewIconFileName, TIpcArgs(aAppUid.iUid, aViewUid.iUid, &filenamePckg));
	if (!err)
		{
		HBufC* fullFileName = HBufC::New(fileName.Length());
		if (fullFileName == NULL)
			return KErrNoMemory;
		else
			{
			*fullFileName = fileName;
			aFullFileName = fullFileName; // ownership transferred to caller
			}
		}

	return err;
	}


/** Changes an existing data type mapping, or adds a new one.

If the data type is not currently mapped, a new mapping is added. 
If the data type is mapped, its mapping is replaced.

@capability WriteDeviceData Prevent addition of data type mappings by malicious programs.
@param aDataType A new or existing data type.
@param aPriority The priority with which the application handles the data type.
@param aUid The UID of the application to associate with the data type.
@return KErrNone on success, or a system-wide error code. 
*/
EXPORT_C TInt RApaLsSession::InsertDataMapping(const TDataType& aDataType, TDataTypePriority aPriority, TUid aUid)
	{
	return InsertDataMapping(aDataType, aPriority, aUid, KOpenServiceUid);
	}


/** Changes an existing data type mapping, or adds a new one.

If the data type is not currently mapped, a new mapping is added. 
If the data type is mapped, its mapping is replaced.

@capability WriteDeviceData Prevent addition of data type mappings by malicious programs.
@param aDataType A new or existing data type.
@param aPriority The priority with which the application handles the data type.
@param aUid The UID of the application to associate with the data type.
@param aServiceUid The UID of the service.
@return KErrNone on success, or a system-wide error code. 
@internalComponent
@released
*/
EXPORT_C TInt RApaLsSession::InsertDataMapping(const TDataType& aDataType, TDataTypePriority aPriority, 
	TUid aUid, TUid aServiceUid)
	{
	const TPckgC<TDataType> dataType(aDataType);
	return SendReceiveWithReconnect(EAppListInsertDataMapping, 
		TIpcArgs(&dataType, TInt(aPriority), aUid.iUid, aServiceUid.iUid));
	} //lint !e1762 Suppress member function could be made const


/** Changes an existing data type mapping, or adds a new one.
If the data type is not currently mapped, it is added.
If the data type is mapped with a priority lower than aPriority, the new mapping replaces the existing one. 
Otherwise, no change is made.

@capability WriteDeviceData Prevent addition of data type mappings by malicious programs.
@param aDataType A new or existing data type.
@param aPriority The priority with which the application handles the data type.
@param aUid The UID of the application to associate with the data type.
@param aInserted Non-zero if the new mapping was added or an existing mapping replaced, zero otherwise.
@return KErrNone on success, or a system-wide error code. 
*/
EXPORT_C TInt RApaLsSession::InsertDataMappingIfHigher(const TDataType& aDataType, TDataTypePriority aPriority, TUid aUid, TBool& aInserted)
	{
	TPckgBuf<TBool> inserted(EFalse);
	const TPckgC<TDataType> dataType(aDataType);
	const TInt err = SendReceiveWithReconnect(EAppListInsertDataMappingIfHigher, TIpcArgs(&dataType, TInt(aPriority), aUid.iUid, &inserted));
	if(!err)
		aInserted = inserted();
		
	return err;
	} //lint !e1762 Suppress member function could be made const



/** Removes an existing user mapping between an application and data-type made through InsertDataMapping() or InsertDataMappingIfHigher().

@capability WriteDeviceData Prevent removal of data type mappings by malicious programs.
@param aDataType Data type whose mapping should be removed.
@panic USER 0 The specified data type cannot be found. Debug builds only.
@return KErrNone on success, or a system-wide error code. 
@see InsertDataMapping()
@see InsertDataMappingIfHigher()
*/
EXPORT_C TInt RApaLsSession::DeleteDataMapping(const TDataType& aDataType)
	{
	return DeleteDataMapping(aDataType, KOpenServiceUid);
	}


/** Removes an existing data type mapping.

@capability WriteDeviceData Prevent removal of data type mappings by malicious programs.
@param aDataType Data type whose mapping should be removed.
@param aServiceUid The UID of the service.
@panic USER 0 The specified data type cannot be found. Debug builds only.
@return KErrNone on success, or a system-wide error code. 
@internalComponent
@released
*/
EXPORT_C TInt RApaLsSession::DeleteDataMapping(const TDataType& aDataType, TUid aServiceUid)
	{
	const TPckgC<TDataType> dataType(aDataType);
	return SendReceiveWithReconnect(EAppListDeleteDataMapping, TIpcArgs(&dataType, aServiceUid.iUid));
	} //lint !e1762 Suppress member function could be made const

	
/** Gets the application associated with the data type and the service uid from
the datatype store. 

The function will only look into the datatype store and will not use the 
default type associations. This is different from the AppForDataTypeAndService() function.

@param aDataType The data (MIME) type.
@param aAppUid On return, the UID of the application that can handle the data 
(MIME) type; this may be NULL.
@param aServiceUid The UID of the service.
@return KErrNone, if successful; otherwise one of the other system-wide error 
codes. 
@internalComponent
@released
*/
EXPORT_C TInt RApaLsSession::GetAppByDataType(const TDataType& aDataType, TUid aServiceUid, TUid& aAppUid) const
	{
	const TPckgC<TDataType> dataType(aDataType);
	TPckg<TUid> uid(aAppUid);
	const TInt returnValue = SendReceiveWithReconnect(EAppListServGetAppByDataType,TIpcArgs(&dataType, aServiceUid.iUid, &uid));
	if (returnValue < 0)
		return returnValue;
	
	return (aAppUid == KNullUid ? KErrNotFound : KErrNone);
	} //lint !e1764 Suppress reference parameter 'aAppUid' could be declared const ref


/** Determines the current language an application is using to display its
user interface.
@param aAppUid The application specific UID.
@param aLanguage On return, the application language.  
@return KErrNone, if successful; KErrNotFound, if a matching application could not be found; 
otherwise one of the other system wide error codes.
*/
EXPORT_C TInt RApaLsSession::ApplicationLanguage(TUid aAppUid, TLanguage& aLanguage) const
	{
	TPckgBuf<TLanguage> pckg;
	const TInt err = SendReceiveWithReconnect(EAppListServApplicationLanguage, TIpcArgs(aAppUid.iUid, &pckg));
	if (!err)
		aLanguage = pckg();

	return err;
	}


/** Gets the services implemented by the application that has the specified
application UID.

The returned CApaAppServiceInfoArray object contains an array of TApaAppServiceInfo objects.

Information on each service implementation is contained in a TApaAppServiceInfo object.

TApaAppServiceInfo::Uid() returns the service UID of the service implemented by the
specified application UID.

@param aAppUid The application specific UID.
@return A pointer to a CApaAppServiceInfoArray object left on the cleanup stack.
@leave KErrNotFound No matching application can be found, or a matching application
does not implement any services.
@leave KErrNotSupported The specified application does not provide an application
registration file.
@leave KErrNoMemory There is insufficient memory to perform the operation.
@see CApaAppServiceInfoArray::Array()
@see TApaAppServiceInfo
@publishedPartner
@released
*/
EXPORT_C CApaAppServiceInfoArray* RApaLsSession::GetAppServicesLC(TUid aAppUid) const
	{
	CArrayFixFlat<TApaAppServiceInfo>* serviceArray = new(ELeave) CArrayFixFlat<TApaAppServiceInfo>(4);
	CleanupStack::PushL(TCleanupItem(CleanupAppServiceArray, serviceArray));
	CBufBase* buffer = GetServiceBufferLC(EAppListServGetAppServices, aAppUid);
	RBufReadStream readStream(*buffer);
	readStream >> *serviceArray;
	CleanupStack::PopAndDestroy(buffer);
	CleanupStack::Pop(serviceArray);
	CApaAppServiceInfoArrayImpl* wrapper = CApaAppServiceInfoArrayImpl::NewL(serviceArray); // takes ownership of serviceArray
	CleanupStack::PushL(wrapper);
	return wrapper;
	}

CBufBase* RApaLsSession::GetServiceBufferLC(TInt aOpcode, TUid aUid1, TUid aUid2/*=KNullUid*/) const
	{
	CBufFlat* buffer=CBufFlat::NewL(KDefaultBufSize); // allocate buffer with a default size that should be large enough in most cases
	CleanupStack::PushL(buffer);
	buffer->ExpandL(0,KDefaultBufSize);
	TPtr8 bufPtr=buffer->Ptr(0);
	const TInt sizeRequired=User::LeaveIfError(SendReceiveWithReconnect(aOpcode,TIpcArgs(aUid1.iUid,aUid2.iUid,buffer->Size(),&bufPtr)));
	if (sizeRequired>0)
		{
		CleanupStack::PopAndDestroy(buffer);
		buffer=CBufFlat::NewL(sizeRequired);
		CleanupStack::PushL(buffer);
		buffer->ExpandL(0,sizeRequired);
		bufPtr.Set(buffer->Ptr(0));
#if defined(_DEBUG)
		const TInt check=
#endif
		User::LeaveIfError(SendReceiveWithReconnect(aOpcode,TIpcArgs(aUid1.iUid,aUid2.iUid,0,&bufPtr)));
		__ASSERT_DEBUG(check==0,User::Invariant());
		}
	return buffer;
	}
	
CBufBase* RApaLsSession::GetServiceBufferLC(TInt aOpcode, TUid aUid1, const TDataType& aDataType) const
	{
	const TPckgC<TDataType> dataType(aDataType);
	CBufFlat* buffer=CBufFlat::NewL(KDefaultBufSize); // allocate buffer with a default size that should be large enough in most cases
	CleanupStack::PushL(buffer);
	buffer->ExpandL(0,KDefaultBufSize);
	TPtr8 bufPtr=buffer->Ptr(0);
	const TInt sizeRequired=User::LeaveIfError(SendReceiveWithReconnect(aOpcode,TIpcArgs(aUid1.iUid,&dataType,buffer->Size(),&bufPtr)));
	if (sizeRequired>0)
		{
		CleanupStack::PopAndDestroy(buffer);
		buffer=CBufFlat::NewL(sizeRequired);
		CleanupStack::PushL(buffer);
		buffer->ExpandL(0,sizeRequired);
		bufPtr.Set(buffer->Ptr(0));
#if defined(_DEBUG)
		const TInt check=
#endif
		User::LeaveIfError(SendReceiveWithReconnect(aOpcode,TIpcArgs(aUid1.iUid,&dataType,0,&bufPtr)));
		__ASSERT_DEBUG(check==0,User::Invariant());
		}
	return buffer;	
	}



/** Gets the service implementations for the specified service UID.

The returned CApaAppServiceInfoArray object contains an array of TApaAppServiceInfo objects.

Information on each implementation is contained in a TApaAppServiceInfo object.

TApaAppServiceInfo::Uid() returns the UID of the application that implements the
specified service UID.

@param aServiceUid The service UID.
@return A pointer to a CApaAppServiceInfoArray object left on the cleanup stack.
@leave KErrNotFound No service implementations for the specified service UID can be found.
@leave EAppListInvalid The server's initial population of the list has not completed.
@leave KErrNoMemory There is insufficient memory to perform the operation.
@see CApaAppServiceInfoArray::Array()
@see TApaAppServiceInfo
@publishedPartner
@released
*/
EXPORT_C CApaAppServiceInfoArray* RApaLsSession::GetServiceImplementationsLC(TUid aServiceUid) const
	{
	CArrayFixFlat<TApaAppServiceInfo>* serviceArray = new(ELeave) CArrayFixFlat<TApaAppServiceInfo>(4);
	CleanupStack::PushL(TCleanupItem(CleanupAppServiceArray, serviceArray));
	CBufBase* buffer = GetServiceBufferLC(EAppListServGetServiceImplementations, aServiceUid);
	RBufReadStream readStream(*buffer);
	readStream >> *serviceArray;
	CleanupStack::PopAndDestroy(buffer);
	CleanupStack::Pop(serviceArray);
	CApaAppServiceInfoArrayImpl* wrapper = CApaAppServiceInfoArrayImpl::NewL(serviceArray); // takes ownership of serviceArray
	CleanupStack::PushL(wrapper);
	return wrapper;
	}
	
EXPORT_C CApaAppServiceInfoArray* RApaLsSession::GetServiceImplementationsLC(TUid aServiceUid, const TDataType& aDataType) const
/** Gets the service implementations for the specified service UID. The implementation must also be
able to handle the data type given as argument.

The returned CApaAppServiceInfoArray object contains an array of TApaAppServiceInfo objects.

Information on each implementation is contained in a TApaAppServiceInfo object.

TApaAppServiceInfo::Uid() returns the UID of the application that implements the
specified service UID.

@param aServiceUid The service UID.
@param aDataType The data type that must be supported by the implementation.
@return A pointer to a CApaAppServiceInfoArray object left on the cleanup stack.
@leave KErrNotFound No service implementations for the specified service UID can be found.
@leave EAppListInvalid The server's initial population of the list has not completed.
@leave KErrNoMemory There is insufficient memory to perform the operation.
@see CApaAppServiceInfoArray::Array()
@see TApaAppServiceInfo
@publishedPartner
@released
*/
	{
	CArrayFixFlat<TApaAppServiceInfo>* serviceArray = new(ELeave) CArrayFixFlat<TApaAppServiceInfo>(4);
	CleanupStack::PushL(TCleanupItem(CleanupAppServiceArray, serviceArray));
	CBufBase* buffer = GetServiceBufferLC(EAppListServGetServiceImplementationsDataType, aServiceUid, aDataType);
	RBufReadStream readStream(*buffer);
	readStream >> *serviceArray;
	CleanupStack::PopAndDestroy(buffer);
	CleanupStack::Pop(serviceArray);
	CApaAppServiceInfoArrayImpl* wrapper = CApaAppServiceInfoArrayImpl::NewL(serviceArray); // takes ownership of serviceArray
	CleanupStack::PushL(wrapper);
	return wrapper;
	}

/** Gets the service UIDs implemented by the application with the specified UID.

@param aAppUid The application specific UID.
@param aServiceUids On return, contains the service UIDs implemented by the specified
application UID.
@leave KErrNotFound No matching application can be found, or a matching application
does not implement any services.
@leave KErrNotSupported The specified application does not provide an application
registration file.
@leave KErrNoMemory There is insufficient memory to perform the operation.
@publishedPartner
@released
*/
EXPORT_C void RApaLsSession::GetAppServicesL(TUid aAppUid, CArrayFixFlat<TUid>& aServiceUids) const
	{
	CBufBase* buffer = GetServiceBufferLC(EAppListServGetAppServiceUids, aAppUid);
	RBufReadStream readStream(*buffer);
	readStream >> aServiceUids;
	CleanupStack::PopAndDestroy(buffer);
	}



/** Gets the service implementation's opaque data for the specified application and service.

The specified application may provide more than one implementation of the specified service.

The returned CApaAppServiceInfoArray object contains an array of TApaAppServiceInfo objects,
each of which provides information on an implementation.

For each TApaAppServiceInfo object, TApaAppServiceInfo::Uid() returns the specified service UID.

@param aAppUid The application specific UID.
@param aServiceUid The service UID.
@return A pointer to a CApaAppServiceInfoArray object left on the cleanup stack.
@leave KErrNotFound No matching application can be found, or a matching application
does not implement the specified service.
@leave KErrNotSupported The specified application does not provide an application
registration file.
@leave KErrNoMemory There is insufficient memory to perform the operation.
@see CApaAppServiceInfoArray::Array()
@see TApaAppServiceInfo
@publishedPartner
@released
*/
EXPORT_C CApaAppServiceInfoArray* RApaLsSession::GetAppServiceOpaqueDataLC(TUid aAppUid, TUid aServiceUid) const
	{
	CArrayFixFlat<TApaAppServiceInfo>* serviceArray = new(ELeave) CArrayFixFlat<TApaAppServiceInfo>(4);
	CleanupStack::PushL(TCleanupItem(CleanupAppServiceArray, serviceArray));
	CBufBase* buffer = GetServiceBufferLC(EAppListServGetAppServiceOpaqueData, aAppUid, aServiceUid);
	RBufReadStream readStream(*buffer);
	readStream >> *serviceArray;
	CleanupStack::PopAndDestroy(buffer);
	CleanupStack::Pop(serviceArray);
	CApaAppServiceInfoArrayImpl* wrapper = CApaAppServiceInfoArrayImpl::NewL(serviceArray); // takes ownership of serviceArray
	CleanupStack::PushL(wrapper);
	return wrapper;
	}


/** Gets the UID of an application that can handle the specified data (MIME) type and service.

If no application can be found, the function returns the UID of the preferred 
default handler. If none of the default handlers can handle the combination
of data type and service, then a NULL UID is returned in aAppUid.

@param aDataType The data (MIME) type.
@param aServiceUid The service UID.
@param aAppUid On return, the UID of the application that can handle the data 
(MIME) type and service; this may be NULL.
@return KErrNone, if successful; otherwise one of the other system-wide error 
codes.
@publishedPartner
@released
*/
EXPORT_C TInt RApaLsSession::AppForDataTypeAndService(const TDataType& aDataType, TUid aServiceUid, TUid& aAppUid) const
	{
	const TPckgC<TDataType> dataType(aDataType);
	TPckg<TUid> uid(aAppUid);
	return SendReceiveWithReconnect(EAppListServAppForDataTypeAndService,TIpcArgs(&dataType, aServiceUid.iUid,&uid));
	} //lint !e1764 Suppress reference parameter 'aAppUid' could be declared const ref

EXPORT_C TInt RApaLsSession::AppForDocumentAndService(const TDesC& aFileName, TUid aServiceUid, TUid& aAppUid, TDataType& aDataType) const
/** Gets the data (MIME) type of the data in the specified file and gets the UID 
of an application that can handle this type and service.

@param aFileName The name of the file containing the data.
@param aServiceUid The service UID
@param aUid On return, the UID of the application that can handle the data 
(MIME) type and service; this may be NULL.
@param aDataType On return, the data (MIME) type.
@return KErrNone, if successful; otherwise one of the other system-wide error 
codes.
@publishedPartner
@released
*/
	{
	return DoAppForDocumentOptionallySpecifyingService(aFileName, aServiceUid, aAppUid, aDataType, EAppListServAppForDocumentAndService);
	}

EXPORT_C TInt RApaLsSession::AppForDocumentAndService(const RFile& aFile, TUid aServiceUid, TUid& aAppUid, TDataType& aDataType) const
/** Gets the data (MIME) type of the data in the specified file and gets the UID 
of an application that can handle this type and service.

@param aFile The file handle.  Before this function can be called,
the file server session which owns this file handle must first be marked as shareable by 
calling RFs::ShareProtected().
@param aServiceUid The service UID.
@param aUid On return, the UID of the application that can handle the data 
(MIME) type and service; this may be NULL.
@param aDataType On return, the data (MIME) type.
@return KErrNone, if successful; otherwise one of the other system-wide error 
codes.
@publishedPartner
@released
*/
	{
	return DoAppForDocumentOptionallySpecifyingService(aFile, aServiceUid, aAppUid, aDataType, EAppListServAppForDocumentAndServicePassedByFileHandle);
	}

TInt RApaLsSession::DoAppForDocumentOptionallySpecifyingService(const TDesC& aFileName, TUid aServiceUid, TUid& aAppUid, TDataType& aDataType, TInt aOpcode) const
	{
	if (!aFileName.Length())
		{
		aAppUid = KNullUid;
		return KErrNone;
		}
		
	HBufC8* buffer = NULL;
	TInt error = GetNewBufferFromFile(buffer, aFileName);
	if (error)
		return error;

	SReturnData_AppForDocument returnData;
	TPckg<SReturnData_AppForDocument> returnData_asDescriptor(returnData);
	error = SendReceiveWithReconnect(aOpcode, TIpcArgs(&returnData_asDescriptor, aServiceUid.iUid, &aFileName, buffer));
	delete buffer;
	buffer = NULL;
	
	if (!error)
		{
		aAppUid = returnData.iUid;
		aDataType = returnData.iDataType;
		}

	return error;
	}

/**
*/
TInt RApaLsSession::DoAppForDocumentOptionallySpecifyingService(const RFile& aFile, TUid aServiceUid, TUid& aAppUid, TDataType& aDataType, TInt aOpcode) const
	{
	SReturnData_AppForDocument returnData;
	TPckg<SReturnData_AppForDocument> returnData_asDescriptor(returnData);
	TIpcArgs ipcArgs(&returnData_asDescriptor, aServiceUid.iUid);
	TInt error = aFile.TransferToServer(ipcArgs, 2, 3);
	if (!error)
		error = SendReceiveWithReconnect(aOpcode, ipcArgs);

	if (!error)
		{
		aAppUid = returnData.iUid;
		aDataType = returnData.iDataType;
		}
		
	return error;
	}

/**
*/
TInt RApaLsSession::GetNewBufferFromFile(HBufC8*& aBuffer, const TDesC& aFileName) const
	{
	aBuffer = NULL;
	TInt preferredBufSize = 0;
	const TInt error = GetPreferredBufSize(preferredBufSize);
	if (error < KErrNone)
		return error;

	HBufC8* const buffer = HBufC8::New(Max(8, preferredBufSize)); // 8 is a sensible minimum
	if (!buffer)
		return KErrNoMemory;

	const RFs* fsSession = FsSession();
	RFs tempFsSession;
	if (!fsSession)
		{
		const TInt error = tempFsSession.Connect();
		if (error)
			{
			delete buffer;
			return error;
			}

		fsSession = &tempFsSession;
		}
		
	if (fsSession->IsValidName(aFileName))
		{
		TPtr8 buffer_asWritable(buffer->Des());
		const TInt error = fsSession->ReadFileSection(aFileName, 0, buffer_asWritable, preferredBufSize);
		if (error)
			{
			delete buffer;
			tempFsSession.Close();
			return error;
			}
		}
		
	if (fsSession == &tempFsSession)
		tempFsSession.Close();
		
	aBuffer = buffer;
	return KErrNone;
	}


/**
@internalTechnology
*/
EXPORT_C void RApaLsSession::SetFsSessionL(RFs& aFsSession)
	{ // static
	User::LeaveIfError(Dll::SetTls(&aFsSession));
	}

/**
@internalTechnology
*/
EXPORT_C void RApaLsSession::ClearFsSession()
	{ // static
	Dll::FreeTls();
	}

/**
@internalTechnology
*/
EXPORT_C RFs* RApaLsSession::FsSession()
	{ // static
	return static_cast<RFs*>(Dll::Tls());
	}

/** @publishedPartner */
EXPORT_C void RApaLsSession::RegisterNonNativeApplicationTypeL(TUid aApplicationType, const TDesC& aNativeExecutable)
	{
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK    
	User::LeaveIfError(SendReceiveWithReconnect(EAppListServRegisterNonNativeApplicationType, TIpcArgs(aApplicationType.iUid, &aNativeExecutable)));
#else
	(void)aApplicationType; //to make compiler happy
	(void)aNativeExecutable; 
    User::Leave(KErrNotSupported);	
#endif	
	} //lint !e1762 Suppress member function could be made const

/** @publishedPartner */
EXPORT_C void RApaLsSession::DeregisterNonNativeApplicationTypeL(TUid aApplicationType)
	{
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK    
	User::LeaveIfError(SendReceiveWithReconnect(EAppListServDeregisterNonNativeApplicationType, TIpcArgs(aApplicationType.iUid)));
#else
	(void)aApplicationType;	//to make compiler happy
    User::Leave(KErrNotSupported); 
#endif	
	} //lint !e1762 Suppress member function could be made const
	
/** @publishedPartner */
EXPORT_C void RApaLsSession::PrepareNonNativeApplicationsUpdatesL()
	{
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK    
	TIpcArgs ipcArgs(0, 0, 0, 0);
	User::LeaveIfError(SendReceiveWithReconnect(EAppListServPrepareNonNativeApplicationsUpdates, ipcArgs));
#else
    User::Leave(KErrNotSupported);
#endif
	} //lint !e1762 Suppress member function could be made const

/** @publishedPartner */
EXPORT_C void RApaLsSession::RegisterNonNativeApplicationL(TUid aApplicationType, const TDriveUnit& aDrive, CApaRegistrationResourceFileWriter& aRegistrationResourceFile, CApaLocalisableResourceFileWriter* aLocalisableResourceFile, const RFile* aIconFile)
	{
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK      
	TIpcArgs ipcArgs(0, 0, 0, 0);
	RBuf8 ipcParameter0;
	CleanupClosePushL(ipcParameter0);
	RBuf8 ipcParameter1;
	CleanupClosePushL(ipcParameter1);
	if (aLocalisableResourceFile==NULL)
		{
		__ASSERT_ALWAYS(aIconFile==NULL, Panic(EPanicIconFileWithoutLocalisableResourceFile));
		ipcArgs.Set(1, NULL);
		}
	else
		{
		TParse* const parser=new(ELeave) TParse;
		CleanupStack::PushL(parser);
		const TDriveName driveName(aDrive.Name()); // TDriveName is a TBuf<2>

		if (aIconFile!=NULL)
			{
			User::LeaveIfError(aIconFile->TransferToServer(ipcArgs, 2, 3));

			TFileName* const fileName=new(ELeave) TFileName;
			CleanupStack::PushL(fileName);
			User::LeaveIfError(aIconFile->Name(*fileName));
			parser->SetNoWild(*fileName, &KLitPathForNonNativeResourceAndIconFiles, &driveName);
			aLocalisableResourceFile->SetIconFileL(parser->FullName());
			CleanupStack::PopAndDestroy(fileName);
			}
		aLocalisableResourceFile->GenerateFileContentsL(ipcParameter1); // must be done after the aLocalisableResourceFile->SetIconFileL call (if there is one)
		const TDesC8& ipcParameter1_asConst=ipcParameter1;
		ipcArgs.Set(1, &ipcParameter1_asConst);

		TBuf<30> fileName;
		fileName.Format(KLitFormatForLocalisableResourceFile, aRegistrationResourceFile.AppUid().iUid);
		parser->SetNoWild(fileName, &KLitPathForNonNativeResourceAndIconFiles, &driveName);
		aRegistrationResourceFile.SetLocalisableResourceFileL(parser->FullName());

		CleanupStack::PopAndDestroy(parser);
		}
		
	aRegistrationResourceFile.GenerateFileContentsL(ipcParameter0); // must be done after the aRegistrationResourceFile.SetLocalisableResourceFileL call (if there is one)
	SNonNativeApplicationInfo nonNativeApplicationInfo;
	nonNativeApplicationInfo.iApplicationType=aApplicationType;
	nonNativeApplicationInfo.iDrive=aDrive;
	ipcParameter0.ReAllocL(sizeof(SNonNativeApplicationInfo)+ipcParameter0.Length());
	ipcParameter0.Insert(0, TPckgC<SNonNativeApplicationInfo>(nonNativeApplicationInfo));
	const TDesC8& ipcParameter0_asConst=ipcParameter0;
	ipcArgs.Set(0, &ipcParameter0_asConst);

	User::LeaveIfError(SendReceiveWithReconnect(EAppListServRegisterNonNativeApplication, ipcArgs));
	CleanupStack::PopAndDestroy(2, &ipcParameter0);
#else
	(void) aApplicationType; //to make compiler happy
	(void) aDrive;
	(void) aRegistrationResourceFile;
	(void) aRegistrationResourceFile;
	(void) aLocalisableResourceFile;
	(void) aIconFile;	
    User::Leave(KErrNotSupported);  
#endif	
	} //lint !e1762 Suppress member function could be made const

/** @publishedPartner */
EXPORT_C void RApaLsSession::DeregisterNonNativeApplicationL(TUid aApplication)
	{
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK  
	User::LeaveIfError(SendReceiveWithReconnect(EAppListServDeregisterNonNativeApplication, TIpcArgs(aApplication.iUid)));
#else
	(void) aApplication;
    User::Leave(KErrNotSupported);  	
#endif
	} //lint !e1762 Suppress member function could be made const
	
/**
Commits the non-native application updates. This is a synchronous method which waits 
until the application list is updated.
	 
@see ForceCommitNonNativeApplicationsUpdatesL
@publishedPartner
@released
*/

EXPORT_C void RApaLsSession::CommitNonNativeApplicationsUpdatesL()
	{
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK      
	TIpcArgs ipcArgs(EFalse, 0, 0, 0);
	User::LeaveIfError(SendReceiveWithReconnect(EAppListServCommitNonNativeApplications, ipcArgs));
#else
    User::Leave(KErrNotSupported);
#endif	
	} //lint !e1762 Suppress member function could be made const
	

/**
Commits the non-native application updates. This is an asynchronous method which will not wait until 
the application list is updated. CApaAppListNotifier class should be used to synchronize the completion 
of updating the application list. 
 
@see CommitNonNativeApplicationsUpdatesL
@see CApaAppListNotifier
@publishedPartner
@released
*/

EXPORT_C void RApaLsSession::ForceCommitNonNativeApplicationsUpdatesL()
	{
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK      
	TIpcArgs ipcArgs(ETrue, 0, 0, 0);
	User::LeaveIfError(SendReceiveWithReconnect(EAppListServCommitNonNativeApplications, ipcArgs));
#else
    User::Leave(KErrNotSupported);  	
#endif
	}

/** 
Rolls back all changes made to the list of installed non-native applications since the last call to
PrepareNonNativeApplicationsUpdatesL().

This function can be called even if PrepareNonNativeApplicationsUpdatesL() hasn't been called before (in which
case it does nothing).

@publishedPartner
*/
EXPORT_C TInt RApaLsSession::RollbackNonNativeApplicationsUpdates()
	{
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK      
	TIpcArgs ipcArgs(0, 0, 0, 0);
	return SendReceiveWithReconnect(EAppListServRollbackNonNativeApplications, ipcArgs);
#else
    return KErrNotSupported;    
#endif
	} //lint !e1762 Suppress member function could be made const

/**
@internalTechnology 
*/
EXPORT_C void RApaLsSession::SetNotify(TBool aCompleteImmediatelyIfNoScanImpendingOrInProgress, TRequestStatus& aStatus)
	{
	SendReceive(ESetNotify,TIpcArgs(aCompleteImmediatelyIfNoScanImpendingOrInProgress),aStatus);
	} //lint !e1762 Suppress member function could be made const

/**
@internalTechnology 
*/
EXPORT_C void RApaLsSession::CancelNotify()
	{
	SendReceive(ECancelNotify,TIpcArgs());
	} //lint !e1762 Suppress member function could be made const
	
/**
Gets the application type of the application. For a native application the type is KNullUid.
@return A standard error code.
@publishedPartner
@released
@param aTypeUid On return contains the application's type
@param aAppUid The application's UID passed into TIpcArgs	
*/
EXPORT_C TInt RApaLsSession::GetAppType(TUid& aTypeUid, TUid aAppUid) const
	{
	TPckg<TUid> uid(aTypeUid);
	return SendReceiveWithReconnect(EAppListServGetAppType,TIpcArgs(aAppUid.iUid,&uid));
	} //lint !e1764 Suppress reference parameter 'aTypeUid' could be declared const ref
	
/**
This function is only for use by Software Install.

As part of the fix for defect INC069526, we added a check in apparc. We check if the application has 
been installed before adding it to the apparc db. A side-effect of this fix is that it is not possible 
to launch applications that are being installed from the installation process itself. This is a regresssion.

To fix this regression we added this function. It allows Software Install to specify a list of registration
files that need to be included in apparc's list even if they have not been marked as installed in the
SISRegistry or JavaRegistry. The list of registration files is cleared once Software Install notifies
(via P&S) the end of the installation (whether successful or not).
The function also forces a rescan and only returns when this rescan is complete. This is because 
Software Install needs to be sure the registration files have been added to apparc's list before 
trying to launch the recently installed applications.

@param aRegFiles The list of registration files for which the SISRegistry check must be ignored.
@return A standard error code.
@internalAll
@released
*/
EXPORT_C TInt RApaLsSession::ForceRegistration(const RPointerArray<TDesC>& aRegFiles)
	{
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK     
	CBufFlat* buffer = 0;
	TRAPD(err, buffer = CreateRegFilesBufferL(aRegFiles));
	if (err)
		return err;
	
	TPtr8 ptr = buffer->Ptr(0);
	const TInt returnValue=SendReceiveWithReconnect(EAppListServForceRegistration,TIpcArgs(&ptr));
	delete buffer;
	return returnValue;
#else
	(void) aRegFiles;
    return KErrNotSupported;	
#endif	
	} //lint !e1762 Suppress member function could be made const
	
	
/**
Make a call to AppArc server's aFunction, passing it aIpcArgs.
*/
TInt RApaLsSession::SendReceiveWithReconnect(TInt aFunction, const TIpcArgs& aIpcArgs) const
	{
	TInt ret = SendReceive(aFunction, aIpcArgs);
	if(ret != KErrServerTerminated)
		{
		return ret;
		}

	RApaLsSession ls;
	TInt err=ls.Connect();
	if (err==KErrNone)
		{
		RApaLsSession* const localThis = const_cast<RApaLsSession*>(this);
		localThis->Close();
		localThis->iHandle=ls.iHandle;
		ret = SendReceive(aFunction, aIpcArgs);
		}
	return ret;
	}

void RApaLsSession::DoGetAppOwnedFilesL(CDesCArray& aArrayToFill, TUid aAppUid) const
	{
	TDesCArrayFiller arrayFiller(aArrayToFill);
	FetchArrayL(arrayFiller, aAppUid, EAppListServGetFileOwnershipInfo, KInitialOwnedFilesBufSize);
	}


/** Registers an observer with apparc server to notify when the initial population of applist is completed

@param aStatus Request status object. On successful completion contains KErrNone, otherwise one of the 
system-wide error codes. 
@see CancelListPopulationCompleteObserver()
*/
EXPORT_C void RApaLsSession::RegisterListPopulationCompleteObserver(TRequestStatus& aStatus) const
	{
	SendReceive(ERegisterListPopulationCompleteObserver,TIpcArgs(TIpcArgs::ENothing),aStatus);
	}


/** Cancels the observer registered with apparc server to notify when the initial population of applist is completed
 
@return KErrNone, if successful; otherwise one of the system-wide error codes. 
*/
EXPORT_C TInt RApaLsSession::CancelListPopulationCompleteObserver() const
	{
	return SendReceiveWithReconnect(ECancelListPopulationCompleteObserver,TIpcArgs(TIpcArgs::ENothing));
	}


/** Tests whether the given TSecurityPolicy matches with the application TSecurityPolicy.

@param aMatches On return, contains the result. ETrue if the application TSecurityPolicy matches the given TSecurityPolicy or else EFalse
@param aAppUid Uid of the application for which the security policy has to be matched
@param aSecurityPolicy TSecurityPolicy to test whether the application with given uid matches with its TSecurityPolicy or not.
@return KErrNone, if successful; otherwise one of the other system-wide error codes.
@see TSecurityPolicy
*/
EXPORT_C TInt RApaLsSession::MatchesSecurityPolicy(TBool& aMatches, TUid aAppUid, const TSecurityPolicy& aSecurityPolicy) const
	{
	const TPckgC<TSecurityPolicy> securityPolicy(aSecurityPolicy);
	const TInt result = SendReceiveWithReconnect(EMatchesSecurityPolicy,TIpcArgs(aAppUid.iUid, &securityPolicy));

	if (result>=0)
		aMatches = result;

	return result;
	}


EXPORT_C void RApaLsSession::RApaLsSession_Reserved1()
	{
	}
	
EXPORT_C void RApaLsSession::RApaLsSession_Reserved2()
	{
	}
	

/** Notification of changes in data-type mapping

This asynchronous function (whose corresponding "cancel" operation is CancelNotifyOnDataTypeMappingChange) completes when any data-type / application-UID association changes, i.e. when the default application handling a particular MIME-type changes.

@param aRequestStatus As is normal for an asynchronous operation, this object is set to something other than KRequestPending when the asynchronous operation that has been triggered by this function completes.
@see CancelNotifyOnDataTypeMappingChange
*/
EXPORT_C void RApaLsSession::NotifyOnDataMappingChange(TRequestStatus& aRequestStatus)
	{
	SendReceive(ENotifyOnDataMappingChange,TIpcArgs(), aRequestStatus);
 	} //lint !e1762 Suppress member function could be made const

 	
/** Cancellation of notification of changes in data-type mapping

This cancels the outstanding the NotifyOnDataTypeMappingChange issued by this client, if there is one outstanding. Otherwise it does nothing.
@see NotifyOnDataTypeMappingChange
*/
EXPORT_C void RApaLsSession::CancelNotifyOnDataMappingChange()
	{
	SendReceive(ECancelNotifyOnDataMappingChange,TIpcArgs());
	} //lint !e1762 Suppress member function could be made const

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 
CBufFlat* RApaLsSession::CreateRegFilesBufferL(const RPointerArray<TDesC>& aRegFiles)
	{
	// Serialize the array
	// Format of the buffer is :
	// 4 bytes for array item count
	// for each item : 4 bytes for length and then the string contents
	const TInt count = aRegFiles.Count();
	TInt requiredBufferSize = 4;	// For the array item count
	for (TInt index = 0; index < count; ++index)
		{
		requiredBufferSize += 4;	// For the string length
		requiredBufferSize += aRegFiles[index]->Size();
		}
	
	CBufFlat* const buffer = CBufFlat::NewL(requiredBufferSize);
	CleanupStack::PushL(buffer);
	buffer->ExpandL(0,requiredBufferSize);
	RBufWriteStream writeStream;
	writeStream.Open(*buffer);
	CleanupClosePushL(writeStream);
	writeStream.WriteUint32L(count);
	for (TInt index = 0; index < count; ++index)
		{
		writeStream.WriteUint32L(aRegFiles[index]->Length());
		writeStream.WriteL(*aRegFiles[index]);
		}

	writeStream.CommitL();
	CleanupStack::PopAndDestroy(&writeStream);
	CleanupStack::Pop(buffer);
	return buffer;
	}
#endif

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK

/* This function is only for use by Installers.

Installers can provide the information about installed, uninstaleld and upgraded applications using this function.
The function takes list of TApaAppUpdateInfo objects. TApaAppUpdateInfo object contains the application uid and
corresponding action done on that application like installed, uninstalled and upgraded.

Apparc updates the application list based on the information provided in the list. 

UpdateAppListL initiates applist update. It will not wait till the applist update completed.

@param aAppUpdateInfo List of TApaAppUpdateInfo objects, which contains application uid and corresponding action information 
					  like installed, uninstalled and changed.
@return A standard error code.
@publishedAll
@released
*/

EXPORT_C TInt RApaLsSession::UpdateAppListL(RArray<TApaAppUpdateInfo>& aAppUpdateInfo)
    {
    //Create a buffer with the application UID and corresponding action information.
    CBufFlat* buffer = 0;
    TRAPD(err, buffer = CreateAppUpdateInfoBufferL(aAppUpdateInfo));
    if (err)
        return err;
    
    TPtr8 ptr = buffer->Ptr(0);
    const TInt returnValue=SendReceiveWithReconnect(EAppListServUpdateAppList,TIpcArgs(&ptr));
    delete buffer;
    return returnValue;
    }


/**
This function is only for use by Software Install.

ForceRegistration allows Software Install to provide a list of application information that need to be 
included in apparc's application list even if they have not been marked as installed in the SISRegistry. 
The force registered applications will be removed from application list once Software Install notifies
the end of the installation by calling UpdateApplist.


@param aAppsData The list of application information needs to be added to application list. Apparc don't take the 
				 ownership of this array.
@return A standard error code.
@publishedAll
@released
*/

EXPORT_C TInt RApaLsSession::ForceRegistration(const RPointerArray<Usif::CApplicationRegistrationData>& aForceRegAppsInfo)
{
    //If there are no applications to update, just return.
    if(aForceRegAppsInfo.Count()==0)
        return(KErrNone);
    
    //Create a buffer with the application uid and corresponding action information.
    CBufFlat* buffer = 0;
    TRAPD(err, buffer = CreateForceRegAppInfoBufferL(aForceRegAppsInfo));
    if (err)
        return err;
    
    TPtr8 ptr = buffer->Ptr(0);
    const TInt returnValue=SendReceiveWithReconnect(EAppListServForceRegistration,TIpcArgs(&ptr));
    delete buffer;
    return returnValue;
}


/*
 * Creates a buffer for applications uids and action information. 
 */
CBufFlat* RApaLsSession::CreateAppUpdateInfoBufferL(RArray<TApaAppUpdateInfo>& aAppUpdateInfo)
    {
    TInt count=aAppUpdateInfo.Count();
    TInt requiredBufferSize=sizeof(TInt32)+(count*sizeof(TApaAppUpdateInfo)); //Size of count + size of TApaAppUpdateInfo objects

    CBufFlat* const buffer = CBufFlat::NewL(requiredBufferSize);
    CleanupStack::PushL(buffer);
    buffer->ExpandL(0,requiredBufferSize);
    RBufWriteStream writeStream;
    writeStream.Open(*buffer);
    CleanupClosePushL(writeStream);
    
	//Write number of TApaAppUpdateInfo objects to stream.
    writeStream.WriteUint32L(count);

    for(TInt index=0;index<count;index++)
        {
		//Write one application information at a time
        writeStream<<aAppUpdateInfo[index];
        }

    writeStream.CommitL();
    CleanupStack::PopAndDestroy(&writeStream);
    CleanupStack::Pop(buffer);
    
    return buffer;
    }
	

/* Creates buffer for force registered application information array*/

CBufFlat* RApaLsSession::CreateForceRegAppInfoBufferL(const RPointerArray<Usif::CApplicationRegistrationData>& aForceRegAppsInfo)
    {
    TInt count=aForceRegAppsInfo.Count();
    TInt requiredBufferSize=sizeof(TInt32); //For count
    
    for(TInt index=0; index<count; index++)
        {
        //Get each application information size and add it to required size.
        requiredBufferSize += GetObjectSizeL(aForceRegAppsInfo[index]);
        }

    CBufFlat* const buffer = CBufFlat::NewL(requiredBufferSize);
    CleanupStack::PushL(buffer);
    buffer->ExpandL(0,requiredBufferSize);
    
    RBufWriteStream writeStream;
    writeStream.Open(*buffer);
    CleanupClosePushL(writeStream);
    
    //Write count to stream.
    writeStream.WriteUint32L(count);

    for(TInt index=0;index<count;index++)
        {
        //Write one applciation information at a time to stream.
        writeStream<<*aForceRegAppsInfo[index];
        }

    writeStream.CommitL();
    CleanupStack::PopAndDestroy(&writeStream);
    CleanupStack::Pop(buffer);
    
    return buffer;    
    }



/* 
Provides updated application information after apparc notifies application list change. It provides list of TApaAppUpdateInfo
objects which contains the information about updated application UID and the action, i.e installed, uninstalled and changed.

The function returns empty list if the application list is changed due to initial application list creation or because of phone 
language change. During that time the list of changed applications can be large.

This function should be called only after the client receives the applist notification from AppArc registered through SetNotify(). Otherwise the 
return values are unpredictable.

@param aUpdatedAppsInfo On return, provides the list of TApaAppUpdateInfo objects, which contains changed application uids and 
                        corresponding action information like installed, uninstalled and changed. If this list is empty, then 
                        the applist is updated because of initial applist creation or because of phone language change. In that 
                        case use GetAllApps and GetNextApp APIs to retrieve the complete applist information.
                           
@return A standard error code.
@publishedAll
@released
*/

EXPORT_C TInt RApaLsSession::UpdatedAppsInfoL(RArray<TApaAppUpdateInfo>& aUpdatedAppsInfo)
    {
    const TInt KDefaultUpdateAppEntries=10;
  
    //Create a buffer with default size
    TInt sizeRequired=(KDefaultUpdateAppEntries * sizeof(TApaAppUpdateInfo)) + 2;
    CBufFlat* buffer=CBufFlat::NewL(sizeRequired);
    CleanupStack::PushL(buffer);
    buffer->ExpandL(0, sizeRequired);
    TPtr8 ptr = buffer->Ptr(0); 

    TPckgBuf<TInt> pckg(sizeRequired);
    
    //pass the buffer and size of the buffer.
    TInt returnValue=SendReceiveWithReconnect(EAppListUpdatedAppsInfo,TIpcArgs(&ptr, &pckg));
    
    //If the size of the buffer is not enough expand it to required size and pass it again.
    if(returnValue==KErrOverflow)
        {
        sizeRequired=pckg();
        buffer->ResizeL(sizeRequired);
        TPtr8 newPtr=buffer->Ptr(0);
        returnValue=SendReceiveWithReconnect(EAppListUpdatedAppsInfo,TIpcArgs(&newPtr, &pckg));
        }
    
    if(returnValue==KErrNone)
        {
        RBufReadStream readStream;
        readStream.Open(*buffer);
        
        //Read count from the stream
        TInt count=readStream.ReadUint32L();
    
        //Read updated applications information and add it to array 
        for(TInt index=0; index<count; index++)
            {
            TApaAppUpdateInfo appUpdateInfo;
            readStream>>appUpdateInfo;
            aUpdatedAppsInfo.AppendL(appUpdateInfo);
            }
        }
    
    CleanupStack::PopAndDestroy(buffer);
    return returnValue;
    }

#endif
    
