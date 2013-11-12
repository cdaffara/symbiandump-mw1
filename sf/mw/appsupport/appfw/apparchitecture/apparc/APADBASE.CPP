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
// apadbase.cpp
//

#include <apadbase.h>
#include "APASTD.H" // Panics etc.

const TUid KUidApaDoorBaseStream = {0x10003A36};

//
// CApaDoorBase
//

EXPORT_C CApaDoorBase::CApaDoorBase()
	{
	}

EXPORT_C void CApaDoorBase::ExternalizeBaseStreamL(CStreamStore& aStore,CStreamDictionary& aStreamDict)const
/** Externalises the information contained in this base class to a stream in the 
specified store.

The resulting stream ID is associated with an internal UID, and placed in 
the specified stream dictionary.

@publishedAll
@released
@param aStore The store to contain the stream.
@param aStreamDict The stream dictionary to contain the stream ID (and an associated 
internal UID). */
	{
	RStoreWriteStream stream;
	TStreamId id=stream.CreateLC(aStore);
	//
	// write the format
	if (iFormat==ETemporarilyIconic)
		stream.WriteInt8L(EGlassDoor);
	else
		stream.WriteInt8L(iFormat);
	//
	// write the size
	TSize size;
	if (iFormat==ETemporarilyIconic)
		size = GlassDoorSize();
	else
		GetSizeInTwips(size);
	stream<< size;
	// 
	// write the source and close
	stream<< iSource;
	stream.CommitL();
	CleanupStack::PopAndDestroy(); // stream
	aStreamDict.AssignL(KUidApaDoorBaseStream,id);
	}



EXPORT_C TSize CApaDoorBase::InternalizeBaseStreamL(const CStreamStore& aStore,const CStreamDictionary& aStreamDict)
/** Internalises the information for this base class from a stream in the specified 
store, and returns the size of the icon or glass door.

The stream ID is extracted from the specified stream dictionary. The UID associated 
with the stream ID is the same as that used when externalising.

@publishedAll 
@released
@param aStore The store containing the stream.
@param aStreamDict The steam dictionary containing the stream ID.
@return The size of the icon or glass door, in twips. */
	{
	TStreamId id=aStreamDict.At(KUidApaDoorBaseStream);
	if (id==KNullStreamId)
		User::Leave(KErrCorrupt); //  there is no base stream - the file is not valid (used to panic EPanicNoBaseDoorStream)
	RStoreReadStream stream;
	stream.OpenLC(aStore,id);
	iFormat = (TFormat)stream.ReadInt8L();
	__ASSERT_DEBUG(iFormat==EIconic || iFormat==EGlassDoor,Panic(EDPanicIllegalDoorFormat));
	TSize size;
	stream>> size;
	stream>> iSource;
	CleanupStack::PopAndDestroy(); // stream
	return size;
	}

// Virtual functions from CPicture

EXPORT_C TStreamId CApaDoorBase::StoreL(CStreamStore& aStore) const
/** Stores the Embedded Document to the specified store.

@publishedAll 
@released
@param aStore The store containing the stream.
@return The ID of the (head) stream used to store the Embedded Document */
	{
	return CPicture::StoreL(aStore);
	}

EXPORT_C void CApaDoorBase::SetScaleFactor(TInt aScaleFactorWidth,TInt aScaleFactorHeight)
/** Sets the Embedded Document's scale factors

@publishedAll 
@released
@param aScaleFactorWidth The width scale factor, in percent
@param aScaleFactorHeight The height scale factor, in percent
*/
	{
	CPicture::SetScaleFactor(aScaleFactorWidth, aScaleFactorHeight);
	}

EXPORT_C void CApaDoorBase::SetCropInTwips(const TMargins& aMargins)
/** Sets the cropping margins of a Embedded Document in twips.
These are relative to the original unscaled size of the Embedded Document.

@publishedAll 
@released
@param aMargins The cropping margins of the Embedded Document, in twips
*/	
	{
	CPicture::SetCropInTwips(aMargins);
	}
	
EXPORT_C TPictureCapability CApaDoorBase::Capability() const
/** Gets the Embedded Document's capabilities.
These include whether it is scalable and croppable.

@publishedAll 
@released
@return The capabilities of the Embedded Document
*/
	{
	return CPicture::Capability();
	}
	
EXPORT_C void CApaDoorBase::GetCropInTwips(TMargins& aMargins) const 
/** Gets the cropping margins of a Embedded Document in twips.
These margins are relative to the original unscaled size of the Embedded Document.

@publishedAll 
@released
@param aMargins The cropping margins of the Embedded Document, in twips 
*/
	{
	CPicture::GetCropInTwips(aMargins);
	}

EXPORT_C TInt CApaDoorBase::ScaleFactorWidth() const
/** Gets the Embedded Document's width scale factor.

@publishedAll 
@released
@return The width scale factor, in percent
*/	
	{
	return CPicture::ScaleFactorWidth();
	}

EXPORT_C TInt CApaDoorBase::ScaleFactorHeight() const
/** Gets the Embedded Document height scale factor.

@publishedAll 
@released
@return The height scale factor, in percent
*/
	{
	return CPicture::ScaleFactorHeight();
	}

EXPORT_C TBool CApaDoorBase::LineBreakPossible(TUint aClass,TBool aBeforePicture,TBool aHaveSpaces) const
/** States whether a line break is possible, either before or after an Embedded Document.
The default implementation returns ETrue, implying that there is a break opportunity both before and after the Embedded Document, whether or not a space is present.
This may be overridden for special types of Embedded Documents.

@publishedAll 
@released
@param aClass The line breaking class of the adjacent character
@param aBeforePicture ETrue, if the adjacent character is before the Embedded Document; EFalse, if the adjacent character is afterwards
@param aHaveSpaces ETrue, if spaces occur between the adjacent character and the Embedded Document; EFalse, otherwise
@return ETrue, if a line break is possible; EFalse, otherwise. 
*/	
	{
	return CPicture::LineBreakPossible(aClass, aBeforePicture, aHaveSpaces);
	}

EXPORT_C TBool CApaDoorBase::NativePixelSize(TSize& aPixelSize)
/** Returns the native pixel size of the bitmap. 

Derived classes might be implemented as bitmaps, in that case it might be interesting to know this.
@publishedAll 
@released
@param aPixelSize The pixel size
@return TBool ETrue, if the derived classes are implemented as bitmaps; EFalse, otherwise.
*/	
	{
	return const_cast<CApaDoorBase*>(this)->CPicture::NativePixelSize(aPixelSize);	// CPicture::NativePixelSize() is logical const
	}

/** Reserved for future use */
EXPORT_C void CApaDoorBase::CApaDoorBase_Reserved1()
	{
	}

/** Reserved for future use */
EXPORT_C void CApaDoorBase::CApaDoorBase_Reserved2()
	{
	}



