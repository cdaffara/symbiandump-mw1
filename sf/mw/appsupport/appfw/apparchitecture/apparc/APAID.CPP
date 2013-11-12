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
// apaid.cpp
//
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#if !defined(__APAID_PARTNER_H__)
#include "apaidpartner.h"
#endif
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS
#include <apaid.h>
#include <s32strm.h>
#include "APASTD.H"

const TInt KMaxOpaqueDataLength = 0x1000; // maximum length of opaque data that can be passed between client and apparc server via a TApaAppServiceInfo object - this can be increased in future if needed

//
// TApaAppIdentifier
//

/** Constructs an empty application identifier.

The data is not initialised. */
EXPORT_C TApaAppIdentifier::TApaAppIdentifier()
	{}

/** Constructs an application identifier from the specified application 
DLL file name and extension, and the specified application UID.
@param aAppUid The application specific UID.
@param aDllName The filename and extension of the application DLL. */
EXPORT_C TApaAppIdentifier::TApaAppIdentifier(TUid aAppUid,const TFileName& aDllName)
	: iAppUid(aAppUid),
	iFullName(aDllName)
	{}

/** Externalises the application identifier to a write stream.

@param aStream The write stream. */
EXPORT_C void TApaAppIdentifier::ExternalizeL(RWriteStream& aStream)const
	{
	aStream<< iAppUid;
	aStream<< iFullName;
	}

/** Internalises the application identifier from a read stream.

@param aStream The read stream. */
EXPORT_C void TApaAppIdentifier::InternalizeL(RReadStream& aStream)
	{
	aStream>> iAppUid;
	aStream>> iFullName;
	}

//
// TApaAppInfo
//

/** Constructs an empty application information object.

The full path name is empty, the captions are empty and the application specific 
UID is set to the null UID. */
EXPORT_C TApaAppInfo::TApaAppInfo()
	: iUid(TUid()),
	iFullName(KNullDesC),
	iCaption(KNullDesC),
	iShortCaption(KNullDesC)
	{}

/** Constructs an application information object from the specified full DLL path 
name, UID and full length caption.

@param aAppUid The application specific UID. 
@param aDllName The full path name of the application DLL. 
@param aCaption The application caption. */
EXPORT_C TApaAppInfo::TApaAppInfo(TUid aAppUid,const TFileName& aDllName,const TApaAppCaption& aCaption)
	: iUid(aAppUid),
	iFullName(aDllName),
	iCaption(aCaption),
	iShortCaption(aCaption)
	{}

/** Constructs an application information object from the specified full DLL path 
name, UID, caption and short caption.

@param aAppUid The application specific UID. 
@param aDllName The full path name of the application DLL. 
@param aCaption The application caption. 
@param aShortCaption The application short caption. */
EXPORT_C TApaAppInfo::TApaAppInfo(TUid aAppUid,const TFileName& aDllName,const TApaAppCaption& aCaption,const TApaAppCaption& aShortCaption)
	: iUid(aAppUid),
	iFullName(aDllName),
	iCaption(aCaption),
	iShortCaption(aShortCaption)
	{}

/** Externalises the application information to a write stream.

@param aStream The write stream. */
EXPORT_C void TApaAppInfo::ExternalizeL(RWriteStream& aStream)const
	{
	aStream<< iUid;
	aStream<< iFullName;
	aStream<< iCaption;
	aStream<< iShortCaption;
	}

/** Internalises the application information from a read stream.

@param aStream The read stream. */
EXPORT_C void TApaAppInfo::InternalizeL(RReadStream& aStream)
	{
	aStream>> iUid;
	aStream>> iFullName;
	aStream>> iCaption;
	aStream>> iShortCaption;
	}


//
// TApaAppViewInfo
//

/** Constructs an empty object.

Specifically, it sets the view UID to KNullUid and empties the application 
caption, i.e. sets it to KNullDesC. */
EXPORT_C TApaAppViewInfo::TApaAppViewInfo()
	: iUid(KNullUid),
	iViewCaption(KNullDesC)
	{}


EXPORT_C TApaAppViewInfo::TApaAppViewInfo(TUid aAppUid,const TApaAppCaption& aViewCaption, TInt aScreenMode)
	: iUid(aAppUid),
	iViewCaption(aViewCaption),
	iScreenMode(aScreenMode)
	{}


/** Externalises the application view information to a write stream.

@param aStream The write stream. */
EXPORT_C void TApaAppViewInfo::ExternalizeL(RWriteStream& aStream)const
	{
	aStream<< iUid;
	aStream<< iViewCaption;
	aStream.WriteInt32L(iScreenMode);
	}


/** Internalises the application view information from a read stream.

@param aStream The read stream. */
EXPORT_C void TApaAppViewInfo::InternalizeL(RReadStream& aStream)
	{
	aStream>> iUid;
	aStream>> iViewCaption;
	iScreenMode=aStream.ReadInt32L();
	}


//
// class TApaAppCapability
//

EXPORT_C void TApaAppCapability::InternalizeL(RReadStream& aStream)
	{
	DoInternalizeL(aStream, iLaunchInBackground, iGroupName);
	}
	
/** Internalises the application capabilities from a read stream.

@param aStream The read stream.
*/
void TApaAppCapability::DoInternalizeL(RReadStream& aStream, TBool& aLaunchInBackground, TApaAppGroupName& aGroupName)
	{
	TInt version = aStream.ReadInt32L();
	iEmbeddability = TEmbeddability(aStream.ReadInt32L());
	iSupportsNewFile = aStream.ReadInt32L();
	iAppIsHidden = aStream.ReadInt32L();

	// initialise values of members which may not be in the stream
	aLaunchInBackground = EFalse;
	aGroupName.Zero();

	if (version==1)
		return;

	// Calypso extension to allow apps to be launched in the background
	aLaunchInBackground = aStream.ReadInt32L();
	if (version==2)
		return;

	aStream >> aGroupName;
	if (version == 3)
		return;

	iAttributes = aStream.ReadUint32L();
	if (version == 4)
		return;
	
	Panic(EDPanicInvalidVersionNumber);
	}

/** Externalises the application capabilities to a write stream.

@param aStream The write stream. 
*/
EXPORT_C void TApaAppCapability::ExternalizeL(RWriteStream& aStream) const
	{
	aStream.WriteInt32L(EVersion);
	aStream.WriteInt32L(iEmbeddability);
	aStream.WriteInt32L(iSupportsNewFile);
	aStream.WriteInt32L(iAppIsHidden);
	aStream.WriteInt32L(iLaunchInBackground);
	aStream << iGroupName;
	aStream.WriteUint32L(iAttributes);
	}
			
/** A utility function that can copy capability information from one descriptor 
to another.

@param aDest Target descriptor.
@param aSource Source descriptor. 
*/
EXPORT_C void TApaAppCapability::CopyCapability(TDes8& aDest,const TDesC8& aSource)
	{
	TInt maxLen=aDest.MaxLength();
	aDest.FillZ(maxLen); // zero fill in case aSource is shorter
	aDest.Copy(aSource.Left(Min(aSource.Length(),maxLen)));
	aDest.SetLength(maxLen);
	}


//
// class TApaEmbeddabilityFilter
//

/** Constructs an empty embeddability filter. */
EXPORT_C TApaEmbeddabilityFilter::TApaEmbeddabilityFilter()
	: iEmbeddabilityFlags(0)
	{
	}

/** Adds aEmbeddability to the filter.

@param aEmbeddability TEmbeddability value to add to the filter. */
EXPORT_C void TApaEmbeddabilityFilter::AddEmbeddability(TApaAppCapability::TEmbeddability aEmbeddability)
	{
	__ASSERT_ALWAYS(aEmbeddability >= 0 && static_cast<TUint>(aEmbeddability) < (sizeof(TUint)*8), Panic(EPanicEmbeddabilityOutOfRange));
	iEmbeddabilityFlags |= (1 << aEmbeddability);
	}

/** Compares aEmbeddability with the filter.

@param aEmbeddability TEmbeddability value to compare.
@return True, if aEmbeddability is included in the filter; false, otherwise. */
EXPORT_C TBool TApaEmbeddabilityFilter::MatchesEmbeddability(TApaAppCapability::TEmbeddability aEmbeddability) const
	{
	__ASSERT_ALWAYS(aEmbeddability >= 0 && static_cast<TUint>(aEmbeddability) < (sizeof(TUint)*8), Panic(EPanicEmbeddabilityOutOfRange));
	TUint embeddabilityFlag = (1 << aEmbeddability);
	return (embeddabilityFlag & iEmbeddabilityFlags);
	}


//
// class TApaAppServiceInfo
//

EXPORT_C TApaAppServiceInfo::TApaAppServiceInfo()
	: iUid(KNullUid),
	  iDataTypes(0),
	  iOpaqueData(NULL)
	{
	}

EXPORT_C TApaAppServiceInfo::TApaAppServiceInfo(TUid aUid, 
	CArrayFixFlat<TDataTypeWithPriority>* aDataTypes, HBufC8* aOpaqueData)
	: iUid(aUid),
	  iDataTypes(aDataTypes),
	  iOpaqueData(aOpaqueData)
	{
	__ASSERT_DEBUG(iDataTypes, Panic(EPanicNullPointer));
	__ASSERT_DEBUG(iOpaqueData, Panic(EPanicNullPointer));
	}

EXPORT_C void TApaAppServiceInfo::ExternalizeL(RWriteStream& aStream) const
	{
	__ASSERT_DEBUG(iDataTypes, Panic(EPanicNullPointer));
	__ASSERT_DEBUG(iOpaqueData, Panic(EPanicNullPointer));
	aStream << iUid;
	aStream << *iDataTypes; //lint !e613 Possible use of null pointer - Asserted above
	aStream << *iOpaqueData;//lint !e613 Possible use of null pointer - Asserted above
	}

EXPORT_C void TApaAppServiceInfo::InternalizeL(RReadStream& aStream)
	{
	aStream >> iUid;
	iDataTypes = new(ELeave) CArrayFixFlat<TDataTypeWithPriority>(1);
	aStream >> *iDataTypes;
	iOpaqueData = HBufC8::NewL(aStream, KMaxOpaqueDataLength);
	}

EXPORT_C void TApaAppServiceInfo::Release()
	{
	if (iDataTypes)
		{
		iDataTypes->Reset();
		delete iDataTypes;		
		}
	if (iOpaqueData)
		{
		delete iOpaqueData;
		iOpaqueData = NULL;
		}
	}

EXPORT_C CArrayFixFlat<TDataTypeWithPriority>& TApaAppServiceInfo::DataTypes()
	{
	__ASSERT_DEBUG(iDataTypes, Panic(EPanicNullPointer));
	return *iDataTypes; //lint !e613 Possible use of null pointer - Asserted above
	}

/** Returns the service UID.

Note that some APIs may store a UID other than a service UID
in a TApaAppServiceInfo object. Such APIs clearly state what
the UID represents.

@return the service UID.
*/
EXPORT_C TUid TApaAppServiceInfo::Uid() const
	{
	return iUid;
	}
	
EXPORT_C const CArrayFixFlat<TDataTypeWithPriority>& TApaAppServiceInfo::DataTypes() const
	{
	__ASSERT_DEBUG(iDataTypes, Panic(EPanicNullPointer));	
	return *iDataTypes; //lint !e613 Possible use of null pointer - Asserted above
	}

/** Returns the service implementation's opaque data.

For each service UID registered by an application, the associated
opaque data indicates how the service is implemented by that application.

The meaning of the opaque data is not known to the framework, it will vary
according to the service.

For some services the opaque data may be a name intended for user display,
for others it may be structured data that the service's client-side code can interpret.

@return the service implementation's opaque data.
*/
EXPORT_C const TDesC8& TApaAppServiceInfo::OpaqueData() const
	{
	if (iOpaqueData)
		return *iOpaqueData;

	return KNullDesC8;
	}

	
//
// class CApaAppServiceInfoArray
//

EXPORT_C CApaAppServiceInfoArray::CApaAppServiceInfoArray()
	{
	}
	
EXPORT_C void CApaAppServiceInfoArray::CApaAppServiceInfoArray_Reserved1()
	{
	}

EXPORT_C void CApaAppServiceInfoArray::CApaAppServiceInfoArray_Reserved2()
	{
	}
