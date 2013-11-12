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
// apgicnfl.cpp
//

#include <e32std.h>
#include <s32file.h>
#include <s32ucmp.h>
#include <bautils.h>
#include <barsread.h>
#include <barsc.h>
#include <barsread2.h>
#include <barsc2.h>
#include "APFDEF.H"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#if !defined(__APGICNFL_PARTNER_H__)
#include "apgicnflpartner.h"
#endif
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS


#ifdef _DEBUG
#include "APGSTD.H" // panic codes
#endif // _DEBUG

#include "APGICNFL.H"

//
// Try and reduce the bitmap mask depth to 1bpp (2 colours)
//
LOCAL_D CFbsBitmap* TryCompressMaskL(const CFbsBitmap& aMask)
	{
	CFbsBitmap* newMask=NULL;
	if (aMask.DisplayMode()!=EGray2 && aMask.IsMonochrome())
		{
		newMask=new(ELeave) CFbsBitmap;
		CleanupStack::PushL(newMask);
		const TSize size=aMask.SizeInPixels();
		User::LeaveIfError(newMask->Create(size,EGray2));
		CFbsBitmapDevice* device=CFbsBitmapDevice::NewL(newMask);
		CleanupStack::PushL(device);
		CFbsBitGc* gc=NULL;
		User::LeaveIfError(device->CreateContext(gc));
		CleanupStack::PushL(gc);
		TPoint origin(0,0);
		gc->BitBlt(origin,&aMask);
		CleanupStack::PopAndDestroy(2); // gc, device
		CleanupStack::Pop(newMask);
		}
	return newMask;
	}

//
// Class CApaMaskedBitmap
//

CApaMaskedBitmap::CApaMaskedBitmap()
:CFbsBitmap()
	{}

EXPORT_C CApaMaskedBitmap::~CApaMaskedBitmap()
/** Destructor.

Frees resources owned by the object prior to its destruction. */
	{
	delete iMask;
	}

EXPORT_C CApaMaskedBitmap* CApaMaskedBitmap::NewLC()
/** Creates a default application icon object.

@return A pointer to the new application icon object. */
	{
	CApaMaskedBitmap* self=new(ELeave) CApaMaskedBitmap;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

EXPORT_C CApaMaskedBitmap* CApaMaskedBitmap::NewL(const CApaMaskedBitmap* aSourceIcon)
/** Creates a new application icon object, making a duplicate copy of an existing 
application icon.

@param aSourceIcon A pointer to an existing application icon.
@return A pointer to the new application icon object. */
	{
	CApaMaskedBitmap* self=new(ELeave) CApaMaskedBitmap;
	CleanupStack::PushL(self);
	self->ConstructL();
	User::LeaveIfError( self->iMask->Duplicate(aSourceIcon->Mask()->Handle()) );
	User::LeaveIfError( self->Duplicate(aSourceIcon->Handle()) );
	CleanupStack::Pop(self);
	return self;
	}

void CApaMaskedBitmap::ConstructL()
	{
	if (!iFbs)
		User::Leave(KErrCouldNotConnect);
	
	iMask=new(ELeave) CFbsBitmap;
	}

EXPORT_C CFbsBitmap* CApaMaskedBitmap::Mask() const
/** Gets the icon's mask.

@return A pointer to the mask bitmap. */
	{
	return iMask;
	}

EXPORT_C void CApaMaskedBitmap::InternalizeL(RReadStream& aStream)
/** Internalizes the application icon from the read stream.

@param aStream The read stream. */
	{
	__ASSERT_DEBUG(iMask, Panic(EPanicNullPointer));
	CFbsBitmap::InternalizeL(aStream);
	aStream >> *iMask;

	// Try to reduce the colour depth of the bitmap mask
	CFbsBitmap* tempMask;
	tempMask = TryCompressMaskL(*iMask);
	// tempMask = NULL if iMask could not be reduced
	if (tempMask != NULL)
		{
		delete iMask;
		iMask = tempMask;
		}
	}

void CApaMaskedBitmap::SetRomBitmapL(TUint8* aRomPointer)
	{
	__ASSERT_DEBUG(iMask, Panic(EPanicNullPointer));
	TInt bitmapSize = 0;
	CFbsBitmap::SetRomBitmapL(reinterpret_cast<CBitwiseBitmap*>(aRomPointer),bitmapSize);

	aRomPointer += bitmapSize;

	iMask->SetRomBitmapL(reinterpret_cast<CBitwiseBitmap*>(aRomPointer),bitmapSize);
	}

EXPORT_C void CApaMaskedBitmap::ExternalizeL(RWriteStream& aStream) const
/** Externalises the application icon to the specified stream.

@param aStream The write stream. */
	{
	__ASSERT_DEBUG(iMask, Panic(EPanicNullPointer));
	CFbsBitmap::ExternalizeL(aStream);
	aStream << *iMask;
	}


EXPORT_C void CApaMaskedBitmap::SetMaskBitmap(CFbsBitmap* aMask)
/**
Sets the icon's mask

@publishedAll
@released
@param     aMask A pointer to the mask bitmap       
*/
	{
	delete iMask;
	iMask = aMask;
	}

//
// CApaResourceFileWriterBase::MDataSink
//

void CApaResourceFileWriterBase::MDataSink::WriteBufferL(const TDesC8& aBuffer)
	{
	if (aBuffer.Length()>0)
		{
		if (iBufferSinkForCompressedUnicodeFormat!=NULL)
			{
			WriteInCompressedUnicodeFormatL(aBuffer.Length(), aBuffer, EFalse);
			}
		else
			{
			DoWriteBufferL(aBuffer);
			iNumberOfBytesWhenUncompressed+=aBuffer.Length();
			}
		}
	}

void CApaResourceFileWriterBase::MDataSink::WriteCompressedUnicodeRunL(TInt aNumberOfBytesWhenUncompressed, const TDesC8& aTextAsCompressedUnicode)
	{
	WriteInCompressedUnicodeFormatL(aNumberOfBytesWhenUncompressed, aTextAsCompressedUnicode, ETrue);
	}

void CApaResourceFileWriterBase::MDataSink::WriteInCompressedUnicodeFormatL(TInt aNumberOfBytesWhenUncompressed, const TDesC8& aData, TBool aCompressedUnicode)
	{
	__ASSERT_DEBUG(iBufferSinkForCompressedUnicodeFormat!=NULL, Panic(EPanicWrongResourceFormat1));
	const TInt dataLength=aData.Length();
	__ASSERT_DEBUG(((dataLength==0) && ((aNumberOfBytesWhenUncompressed==0) || (aNumberOfBytesWhenUncompressed==1))) ||
				   ((dataLength>0) && (aNumberOfBytesWhenUncompressed>0)), Panic(EPanicBadCompressedUnicodeRun));
	if (dataLength>0)
		{
		if ((iNumberOfBytesWhenUncompressed==0) && (iBufferSinkForCompressedUnicodeFormat->iNumberOfBytesWhenUncompressed==0))
			{
			if (aCompressedUnicode)
				{
				iFlags|=EFlag_InCompressedUnicodeRun;
				}
			else
				{
				iFlags&=~EFlag_InCompressedUnicodeRun;
				}
			}
		else if ((iFlags&EFlag_InCompressedUnicodeRun)^(aCompressedUnicode? EFlag_InCompressedUnicodeRun: 0)) // if we're changing the state of the EFlag_InCompressedUnicodeRun flag
			{
			FlushL(EFalse);
			iFlags^=EFlag_InCompressedUnicodeRun; // toggle the EFlag_InCompressedUnicodeRun flag
			}
		static_cast<MDataSink*>(iBufferSinkForCompressedUnicodeFormat)->DoWriteBufferL(aData);
		}
	iBufferSinkForCompressedUnicodeFormat->iNumberOfBytesWhenUncompressed+=aNumberOfBytesWhenUncompressed;
	}

TInt CApaResourceFileWriterBase::MDataSink::NumberOfBytesWhenUncompressed() const
	{
	TInt numberOfBytesWhenUncompressed=iNumberOfBytesWhenUncompressed;
	if (iBufferSinkForCompressedUnicodeFormat!=NULL)
		{
		numberOfBytesWhenUncompressed+=iBufferSinkForCompressedUnicodeFormat->iNumberOfBytesWhenUncompressed;
		}
	return numberOfBytesWhenUncompressed;
	}

CApaResourceFileWriterBase::MDataSink::MDataSink(RBufferSink* aBufferSinkForCompressedUnicodeFormat)
	:iNumberOfBytesWhenUncompressed(0),
	 iFlags(0),
	 iBufferSinkForCompressedUnicodeFormat(aBufferSinkForCompressedUnicodeFormat)
	{
	}

void CApaResourceFileWriterBase::MDataSink::FlushL(TBool aFinalFlush)
	{
	if (iBufferSinkForCompressedUnicodeFormat!=NULL)
		{
		RBuf8 run;
		CleanupClosePushL(run);
		TInt numberOfBytesInRunWhenUncompressed=0;
		iBufferSinkForCompressedUnicodeFormat->FlushAndGetAndResetL(numberOfBytesInRunWhenUncompressed, run);
		if (numberOfBytesInRunWhenUncompressed>0)
			{
			if ((iNumberOfBytesWhenUncompressed==0) && ((iFlags&EFlag_InCompressedUnicodeRun)==0))
				{
				WriteRunLengthL(0); // insert initial zero-length run-length as we're not starting with compressed Unicode
				}
			__ASSERT_DEBUG(run.Length()>0, Panic(EPanicBadRunLength));
			WriteRunLengthL(run.Length());
			DoWriteBufferL(run);
			iNumberOfBytesWhenUncompressed+=numberOfBytesInRunWhenUncompressed;
			}
		CleanupStack::PopAndDestroy(&run);
		if (aFinalFlush && (iNumberOfBytesWhenUncompressed==0))
			{
			WriteRunLengthL(0); // write a zero-length run-length as the resource is completely empty
			}
		}
	}

void CApaResourceFileWriterBase::MDataSink::Reset(TInt& aNumberOfBytesWhenUncompressed)
	{
	aNumberOfBytesWhenUncompressed=iNumberOfBytesWhenUncompressed;
	iNumberOfBytesWhenUncompressed=0;
	iFlags=0;
	}

void CApaResourceFileWriterBase::MDataSink::WriteRunLengthL(TInt aRunLength)
	{
	__ASSERT_DEBUG((aRunLength&~0x7fff)==0, Panic(EPanicBadRunLengthParameter));
	__ASSERT_DEBUG(CompressedUnicodeFormat(), Panic(EPanicWrongResourceFormat2));
	TBuf8<2> buffer;
	if (aRunLength&~0x7f)
		{
		buffer.Append((aRunLength>>8)|0x80);
		}
	buffer.Append(aRunLength&0xff);
	DoWriteBufferL(buffer); // must call DoWriteBufferL here (not WriteBufferL) so that iNumberOfBytesWhenUncompressed is not altered
	}

// CApaResourceFileWriterBase::RBufferSink

CApaResourceFileWriterBase::RBufferSink::RBufferSink(RBufferSink* aBufferSinkForCompressedUnicodeFormat)
	:MDataSink(aBufferSinkForCompressedUnicodeFormat)
	{
	}

void CApaResourceFileWriterBase::RBufferSink::ConstructLC()
	{
	iBuffer.CreateL(20);
	CleanupClosePushL(*this);
	}

void CApaResourceFileWriterBase::RBufferSink::Close()
	{
	iBuffer.Close();
	}

void CApaResourceFileWriterBase::RBufferSink::FlushAndGetAndResetL(TInt& aNumberOfBytesWhenUncompressed, RBuf8& aBuffer)
	{
	FlushL(ETrue);
	iBuffer.Swap(aBuffer);
	Reset(aNumberOfBytesWhenUncompressed);
	}

void CApaResourceFileWriterBase::RBufferSink::DoWriteBufferL(const TDesC8& aBuffer)
	{
	if (iBuffer.MaxLength()-iBuffer.Length()<aBuffer.Length())
		{
		iBuffer.ReAllocL(iBuffer.Length()+aBuffer.Length()+20);
		}
	iBuffer.Append(aBuffer);
	}

// CApaResourceFileWriterBase

CApaResourceFileWriterBase::CApaResourceFileWriterBase()
	{
	}

void CApaResourceFileWriterBase::DoGenerateFileContentsL(RBuf8& aBuffer, TUid aUid2, TUid aUid3) const
	{
	TInt mainResourceSizeInBytesWhenUncompressed=0;
	RBuf8 mainResourceInFormatContainingCompressedUnicode;
	MainResourceInCompiledFormatLC(mainResourceSizeInBytesWhenUncompressed, mainResourceInFormatContainingCompressedUnicode, ETrue);
	TInt temp=0;
	RBuf8 mainResourceInFormatNotContainingCompressedUnicode;
	MainResourceInCompiledFormatLC(temp, mainResourceInFormatNotContainingCompressedUnicode, EFalse);
	__ASSERT_DEBUG(temp==mainResourceSizeInBytesWhenUncompressed, Panic(EPanicInconsistentResourceSizeInBytes));
	TBool mainResourceInCompressedUnicodeFormat=ETrue;
	TPtrC8 mainResourceInSmallestFormat(mainResourceInFormatContainingCompressedUnicode);
	if (mainResourceInSmallestFormat.Length()>=mainResourceInFormatNotContainingCompressedUnicode.Length())
		{
		mainResourceInCompressedUnicodeFormat=EFalse;
		mainResourceInSmallestFormat.Set(mainResourceInFormatNotContainingCompressedUnicode);
		}
	TBool secondResourceInCompressedUnicodeFormat=EFalse;
	const TDesC8* const secondResource=SecondResourceL(secondResourceInCompressedUnicodeFormat);
	RBufferSink bufferSink(NULL);
	bufferSink.ConstructLC();

	WriteUidTypeL(bufferSink, aUid2, aUid3);
	WriteUint8L(bufferSink, 0); // flags
	WriteLittleEndianUint16L(bufferSink, (secondResource==NULL)? mainResourceSizeInBytesWhenUncompressed: Max(mainResourceSizeInBytesWhenUncompressed, secondResource->Length())); // the size in bytes of the largest resource in the file when uncompressed
	TUint bitArray=0; // bit-array (one per top-level resource) indicating whether each top-level resource contains compressed unicode or not
	if (mainResourceInCompressedUnicodeFormat)
		{
		bitArray|=0x01;
		}
	if (secondResourceInCompressedUnicodeFormat)
		{
		bitArray|=0x02;
		}
	WriteUint8L(bufferSink, bitArray);
	WriteBufferL(bufferSink, mainResourceInSmallestFormat);
	if (secondResource!=NULL)
		{
		WriteBufferL(bufferSink, *secondResource);
		}
	TInt filePosition=16+1+2+1;
	WriteLittleEndianUint16L(bufferSink, filePosition);
	filePosition+=mainResourceInSmallestFormat.Length();
	WriteLittleEndianUint16L(bufferSink, filePosition);
	if (secondResource!=NULL)
		{
		filePosition+=secondResource->Length();
		WriteLittleEndianUint16L(bufferSink, filePosition);
		}
	TInt notUsed;
	bufferSink.FlushAndGetAndResetL(notUsed, aBuffer);

	CleanupStack::PopAndDestroy(3, &mainResourceInFormatContainingCompressedUnicode);
	}

void CApaResourceFileWriterBase::WriteUidTypeL(MDataSink& aDataSink, TUid aUid2, TUid aUid3) const
	{
	aDataSink.WriteBufferL(TCheckedUid(TUidType(TUid::Uid(0x101f4a6b), aUid2, aUid3)).Des());
	}

void CApaResourceFileWriterBase::WriteTextL(MDataSink& aDataSink, const TDesC& aText) const
	{
	// LTEXT

	WriteUint8L(aDataSink, aText.Length());
	if (!aDataSink.CompressedUnicodeFormat())
		{
		if ((aDataSink.NumberOfBytesWhenUncompressed()%2)!=0)
			{
			WriteUint8L(aDataSink, 0xab);
			}
		const TInt lengthOfTextInBytes=aText.Size();
		WriteBufferL(aDataSink, TPtrC8(reinterpret_cast<const TUint8*>(aText.Ptr()), lengthOfTextInBytes));
		}
	else
		{
		TInt numberOfBytesWhenUncompressed=aText.Size();
		if ((aDataSink.NumberOfBytesWhenUncompressed()%2)!=0)
			{
			++numberOfBytesWhenUncompressed; // for the padding-byte 0xab when it's uncompressed
			}
		HBufC8* const textAsCompressedUnicode=AsCompressedUnicodeLC(aText);
		aDataSink.WriteCompressedUnicodeRunL(numberOfBytesWhenUncompressed, *textAsCompressedUnicode);
		CleanupStack::PopAndDestroy(textAsCompressedUnicode);
		}
	}

void CApaResourceFileWriterBase::WriteText8L(MDataSink& aDataSink, const TDesC8& aText8) const
	{
	// LTEXT8

	WriteUint8L(aDataSink, aText8.Length());
	WriteBufferL(aDataSink, aText8);
	}

void CApaResourceFileWriterBase::WriteUint8L(MDataSink& aDataSink, TUint aUint8) const
	{
	TBuf8<1> buffer;
	buffer.Append(aUint8&0xff);
	aDataSink.WriteBufferL(buffer);
	}

void CApaResourceFileWriterBase::WriteLittleEndianUint16L(MDataSink& aDataSink, TUint aUint16) const
	{
	TBuf8<2> buffer;
	buffer.Append(aUint16&0xff);
	buffer.Append((aUint16>>8)&0xff);
	aDataSink.WriteBufferL(buffer);
	}

void CApaResourceFileWriterBase::WriteLittleEndianUint32L(MDataSink& aDataSink, TUint aUint32) const
	{
	TBuf8<4> buffer;
	buffer.Append(aUint32&0xff);
	buffer.Append((aUint32>>8)&0xff);
	buffer.Append((aUint32>>16)&0xff);
	buffer.Append((aUint32>>24)&0xff);
	aDataSink.WriteBufferL(buffer);
	}

void CApaResourceFileWriterBase::WriteBufferL(MDataSink& aDataSink, const TDesC8& aBuffer) const
	{
	aDataSink.WriteBufferL(aBuffer);
	}

HBufC8* CApaResourceFileWriterBase::AsCompressedUnicodeLC(const TDesC& aUncompressedUnicode)
	{
	TUnicodeCompressor unicodeCompressor;
	TMemoryUnicodeSource decompressedUnicode1(aUncompressedUnicode.Ptr());
	TMemoryUnicodeSource decompressedUnicode2(aUncompressedUnicode.Ptr());
	
	// Create a buffer big enough to hold all the compressed output
	TInt compressedUnicodeSizeInBytes = TUnicodeCompressor::CompressedSizeL(decompressedUnicode1, aUncompressedUnicode.Length());
	HBufC8* const compressedUnicodeBuffer=HBufC8::NewLC(compressedUnicodeSizeInBytes);
	TUint8* const compressedUnicodeBuffer_asBytePointer=const_cast<TUint8*>(compressedUnicodeBuffer->Ptr());
	
	// Compress the Unicode string
	TInt numberOfInputElementsConsumed = 0;
	TInt numberOfOutputBytes = 0;
	unicodeCompressor.CompressL(compressedUnicodeBuffer_asBytePointer, decompressedUnicode2, compressedUnicodeSizeInBytes, aUncompressedUnicode.Length(), &numberOfOutputBytes, &numberOfInputElementsConsumed);
	TInt temp = 0;
	unicodeCompressor.FlushL(compressedUnicodeBuffer_asBytePointer, compressedUnicodeSizeInBytes, temp);
	numberOfOutputBytes+=temp;
	TPtr8 compressedUnicodeBuffer_asWritable(compressedUnicodeBuffer->Des());
	compressedUnicodeBuffer_asWritable.SetLength(numberOfOutputBytes);
	return compressedUnicodeBuffer;
	}

void CApaResourceFileWriterBase::MainResourceInCompiledFormatLC(TInt& aMainResourceSizeInBytesWhenUncompressed, RBuf8& aBuffer, TBool aCompressedUnicodeFormat) const
	{
	CleanupClosePushL(aBuffer);
	RBufferSink bufferSinkForCompressedUnicodeFormat(NULL);
	if (aCompressedUnicodeFormat)
		{
		bufferSinkForCompressedUnicodeFormat.ConstructLC();
		}
	RBufferSink bufferSink(aCompressedUnicodeFormat? &bufferSinkForCompressedUnicodeFormat: NULL);
	bufferSink.ConstructLC();
	MainResourceInCompiledFormatL(bufferSink);
	bufferSink.FlushAndGetAndResetL(aMainResourceSizeInBytesWhenUncompressed, aBuffer);
	CleanupStack::PopAndDestroy(&bufferSink);
	if (aCompressedUnicodeFormat)
		{
		CleanupStack::PopAndDestroy(&bufferSinkForCompressedUnicodeFormat);
		}
	}

// CApaRegistrationResourceFileWriter

/** 
Creates a new CApaRegistrationResourceFileWriter instance.

@param aAppUid The UID of the application.
@param aAppFile The name and extension of the file to generate.
@param aAttributes The attributes of the application. See the TApaAppCapability class for more details.
@return A pointer to the new CApaRegistrationResourceFileWriter instance.

@publishedPartner
@released
*/
EXPORT_C CApaRegistrationResourceFileWriter* CApaRegistrationResourceFileWriter::NewL(TUid aAppUid, const TDesC& aAppFile, TUint aAttributes)
// aAppFile does not need to have the drive set
	{ // static
	CApaRegistrationResourceFileWriter* const self=new(ELeave) CApaRegistrationResourceFileWriter(aAppUid, aAttributes);
	CleanupStack::PushL(self);
	self->ConstructL(aAppFile);
	CleanupStack::Pop(self);
	return self;
	}

/** 
The destructor for the CApaRegistrationResourceFileWriter class.

@publishedPartner
@released
*/
EXPORT_C CApaRegistrationResourceFileWriter::~CApaRegistrationResourceFileWriter()
	{
	delete iAppFile;
	delete iLocalisableResourceFile;
	delete iGroupName;
	delete iOpaqueData;

	TInt i;
	for (i=iDataTypeList.Count()-1; i>=0; --i)
		{
		delete iDataTypeList[i].iType;
		}
	iDataTypeList.Close();

	for (i=iFileOwnershipList.Count()-1; i>=0; --i)
		{
		delete iFileOwnershipList[i].iFileName;
		}
	iFileOwnershipList.Close();
	}

TUid CApaRegistrationResourceFileWriter::AppUid() const
/** @internalComponent */
	{
	return iAppUid;
	}

const TUid KUidAppRegistrationFile ={0x101F8021};

void CApaRegistrationResourceFileWriter::GenerateFileContentsL(RBuf8& aBuffer) const
/** @internalComponent */
	{
	DoGenerateFileContentsL(aBuffer, KUidAppRegistrationFile, iAppUid);
	}

void CApaRegistrationResourceFileWriter::SetLocalisableResourceFileL(const TDesC& aLocalisableResourceFile)
/** @internalComponent */
	{
	HBufC* const localisableResourceFile=aLocalisableResourceFile.AllocL();
	delete iLocalisableResourceFile;
	iLocalisableResourceFile=localisableResourceFile;
	}

/** 
Sets or clears the hidden attribute. The usual purpose of the hidden attribute is to 
decide if the application should appear in the task list or not but this could vary between
products.

@param aAppIsHidden The value of the hidden flag.

@publishedPartner
@released
*/
EXPORT_C void CApaRegistrationResourceFileWriter::SetAppIsHiddenL(TBool aAppIsHidden)
	{
	iAppIsHidden=aAppIsHidden? 1: 0;
	}

/** 
Sets the embeddability attribute. See the TApaAppCapability::TEmbeddability class for more details.

@param aEmbeddability The value of the embeddability flags.

@publishedPartner
@released
*/
EXPORT_C void CApaRegistrationResourceFileWriter::SetEmbeddabilityL(TApaAppCapability::TEmbeddability aEmbeddability)
	{
	iEmbeddability=aEmbeddability;
	}

/** 
Specifies if the application supports the creation of a new file or not.

@param aSupportsNewFile ETrue to specify that the application supports the creation of a new file.

@publishedPartner
@released
*/
EXPORT_C void CApaRegistrationResourceFileWriter::SetSupportsNewFileL(TBool aSupportsNewFile)
	{
	iSupportsNewFile=aSupportsNewFile? 1: 0;
	}

/** 
Specifies if the application must be launched in the background.

@param aLaunchInBackground ETrue if the application must be launched in the background, EFalse otherwise.

@publishedPartner
@released
*/
EXPORT_C void CApaRegistrationResourceFileWriter::SetLaunchInBackgroundL(TBool aLaunchInBackground)
	{
	iLaunchInBackground=aLaunchInBackground;
	}

/** 
Sets the name of the application group.

@param aGroupName The name of the application group.

@publishedPartner
@released
*/
EXPORT_C void CApaRegistrationResourceFileWriter::SetGroupNameL(const TDesC& aGroupName)
	{
	HBufC* const groupName=aGroupName.AllocL();
	delete iGroupName;
	iGroupName=groupName;
	}

/** 
Sets the default screen number. This can be used to specify the preferred screen on devices
that support more than one screen.

@param aDefaultScreenNumber The name of the default screen.

@publishedPartner
@released
*/
EXPORT_C void CApaRegistrationResourceFileWriter::SetDefaultScreenNumberL(TInt aDefaultScreenNumber)
	{
	iDefaultScreenNumber=aDefaultScreenNumber;
	}

/** 
Sets the opaque data. The opaque data is some data that is specific to the type of application.

@publishedPartner
@released
*/
EXPORT_C void CApaRegistrationResourceFileWriter::SetOpaqueDataL(const TDesC8& aOpaqueData)
	{
	HBufC8* const opaqueData=aOpaqueData.AllocL();
	delete iOpaqueData;
	iOpaqueData=opaqueData;
	}

/** 
Adds a datatype to the list of datatypes that the application can handle.

@param aPriority The priority.
@param aType The datatype.

@publishedPartner
@released
*/
EXPORT_C void CApaRegistrationResourceFileWriter::AddDataTypeL(TInt aPriority, const TDesC8& aType)
	{
	SDataType dataType;
	dataType.iPriority=aPriority;
	dataType.iType=aType.AllocLC();
	iDataTypeList.AppendL(dataType);
	CleanupStack::Pop(dataType.iType);
	}

/** 
Adds a file to the list of files owned by the CApaRegistrationResourceFileWriter instances. These files
are deleted if an error occurs whil registering the new applications.

@param aFileName The name of the file.

@publishedPartner
@released
*/
EXPORT_C void CApaRegistrationResourceFileWriter::AddFileOwnershipInfoL(const TDesC& aFileName)
	{
	SFileOwnershipInfo fileOwnershipInfo;
	fileOwnershipInfo.iFileName=aFileName.AllocLC();
	iFileOwnershipList.AppendL(fileOwnershipInfo);
	CleanupStack::Pop(fileOwnershipInfo.iFileName);
	}

CApaRegistrationResourceFileWriter::CApaRegistrationResourceFileWriter(TUid aAppUid, TUint aAttributes)
	:iAppUid(aAppUid),
	 iAppFile(NULL),
	 iAttributes(aAttributes),
	 iLocalisableResourceFile(NULL),
	 iAppIsHidden(0),
	 iEmbeddability(TApaAppCapability::ENotEmbeddable),
	 iSupportsNewFile(0),
	 iLaunchInBackground(0),
	 iGroupName(NULL),
	 iDefaultScreenNumber(0),
	 iOpaqueData(NULL)
	{
	}

void CApaRegistrationResourceFileWriter::ConstructL(const TDesC& aAppFile)
	{
	iAppFile=aAppFile.AllocL();
	iLocalisableResourceFile=NULL;
	}

void CApaRegistrationResourceFileWriter::WriteDataTypeL(MDataSink& aDataSink, const SDataType& aDataType) const
	{
	// DATATYPE

	// LONG priority
	WriteLittleEndianUint32L(aDataSink, aDataType.iPriority);

	// LTEXT8 type(KMaxDataTypeLength)
	WriteText8L(aDataSink, *aDataType.iType);
	}

void CApaRegistrationResourceFileWriter::WriteFileOwnershipInfoL(MDataSink& aDataSink, const SFileOwnershipInfo& aFileOwnershipInfo) const
	{
	// FILE_OWNERSHIP_INFO

	// LTEXT file_name(KMaxFileNameLength)
	WriteTextL(aDataSink, *aFileOwnershipInfo.iFileName);
	}

void CApaRegistrationResourceFileWriter::MainResourceInCompiledFormatL(MDataSink& aDataSink) const
	{
	// APP_REGISTRATION_INFO

	// LONG reserved_long = 0
	WriteLittleEndianUint32L(aDataSink, 0);

	// LLINK reserved_llink = 0
	WriteLittleEndianUint32L(aDataSink, 0);
	__ASSERT_DEBUG(iAppFile, Panic(EPanicNullPointer));
	// LTEXT app_file(KMaxFileNameLength) = ""
	WriteTextL(aDataSink, *iAppFile);

	// LONG attributes = 0
	WriteLittleEndianUint32L(aDataSink, iAttributes);

	// LTEXT localisable_resource_file(KMaxFileNameLength) = ""
	TPtrC localisableResourceFile(KNullDesC);
	if (iLocalisableResourceFile!=NULL)
		{
		localisableResourceFile.Set(*iLocalisableResourceFile);
		}
	WriteTextL(aDataSink, localisableResourceFile);

	// LONG localisable_resource_id = 1
	WriteLittleEndianUint32L(aDataSink, 1);

	// BYTE hidden = KAppNotHidden
	WriteUint8L(aDataSink, iAppIsHidden);

	// BYTE embeddability = KAppNotEmbeddable
	WriteUint8L(aDataSink, iEmbeddability);

	// BYTE newfile = KAppDoesNotSupportNewFile
	WriteUint8L(aDataSink, iSupportsNewFile);

	// BYTE launch = KAppLaunchInForeground
	WriteUint8L(aDataSink, iLaunchInBackground);

	// LTEXT group_name(KAppMaxGroupName) = ""
	TPtrC groupName(KNullDesC);
	if (iGroupName!=NULL)
		{
		groupName.Set(*iGroupName);
		}
	WriteTextL(aDataSink, groupName);

	// BYTE default_screen_number = 0
	WriteUint8L(aDataSink, iDefaultScreenNumber);

	// LEN WORD STRUCT datatype_list[]
	TInt i;
	const TInt numberOfDataTypes=iDataTypeList.Count();
	WriteLittleEndianUint16L(aDataSink, numberOfDataTypes);
	for (i=0; i<numberOfDataTypes; ++i)
		{
		WriteDataTypeL(aDataSink, iDataTypeList[i]);
		}

	// LEN WORD STRUCT file_ownership_list[]
	const TInt numberOfOwnershipItems=iFileOwnershipList.Count();
	WriteLittleEndianUint16L(aDataSink, numberOfOwnershipItems);
	for (i=0; i<numberOfOwnershipItems; ++i)
		{
		WriteFileOwnershipInfoL(aDataSink, iFileOwnershipList[i]);
		}

	// LEN WORD STRUCT service_list[]
	WriteLittleEndianUint16L(aDataSink, 0);

	// LLINK opaque_data = 0
	WriteLittleEndianUint32L(aDataSink, (iOpaqueData!=NULL)? 2: 0);
	}

const TDesC8* CApaRegistrationResourceFileWriter::SecondResourceL(TBool& aSecondResourceInCompressedUnicodeFormat) const
	{
	aSecondResourceInCompressedUnicodeFormat=EFalse;
	return iOpaqueData;
	}

// CApaLocalisableResourceFileWriter

EXPORT_C CApaLocalisableResourceFileWriter* CApaLocalisableResourceFileWriter::NewL(const TDesC& aShortCaption, const TDesC& aCaption, TInt aNumberOfIcons, const TDesC& aGroupName)
	{ // static
	CApaLocalisableResourceFileWriter* const self=new(ELeave) CApaLocalisableResourceFileWriter(aNumberOfIcons);
	CleanupStack::PushL(self);
	self->ConstructL(aShortCaption, aCaption, aGroupName);
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CApaLocalisableResourceFileWriter::~CApaLocalisableResourceFileWriter()
	{
	delete iShortCaption;
	delete iCaptionAndIcon.iCaption;
	delete iCaptionAndIcon.iIconFile;
	delete iGroupName;
	}

void CApaLocalisableResourceFileWriter::GenerateFileContentsL(RBuf8& aBuffer) const
/** @internalComponent */
	{
	DoGenerateFileContentsL(aBuffer, TUid::Null(), TUid::Null());
	}

void CApaLocalisableResourceFileWriter::SetIconFileL(const TDesC& aIconFile)
/** @internalComponent */
	{
	HBufC* const iconFile=aIconFile.AllocL();
	delete iCaptionAndIcon.iIconFile;
	iCaptionAndIcon.iIconFile=iconFile;
	}

CApaLocalisableResourceFileWriter::CApaLocalisableResourceFileWriter(TInt aNumberOfIcons)
	:iShortCaption(NULL),
	 iGroupName(NULL)
	{
	iCaptionAndIcon.iCaption=NULL;
	iCaptionAndIcon.iNumberOfIcons=aNumberOfIcons;
	iCaptionAndIcon.iIconFile=NULL;
	}

void CApaLocalisableResourceFileWriter::ConstructL(const TDesC& aShortCaption, const TDesC& aCaption, const TDesC& aGroupName)
	{
	iShortCaption=aShortCaption.AllocL();
	iCaptionAndIcon.iCaption=aCaption.AllocL();
	iCaptionAndIcon.iIconFile=NULL;
	iGroupName=aGroupName.AllocL();
	}

void CApaLocalisableResourceFileWriter::WriteCaptionAndIconInfoL(MDataSink& aDataSink, const SCaptionAndIconInfo& aCaptionAndIcon) const
	{
	// CAPTION_AND_ICON_INFO

	// LONG reserved_long = 0
	WriteLittleEndianUint32L(aDataSink, 0);

	// LLINK reserved_llink = 0
	WriteLittleEndianUint32L(aDataSink, 0);

	// LTEXT caption(KMaxCaption) = ""
	WriteTextL(aDataSink, *aCaptionAndIcon.iCaption);

	// WORD number_of_icons = 0
	WriteLittleEndianUint16L(aDataSink, aCaptionAndIcon.iNumberOfIcons);

	// LTEXT icon_file(KMaxFileNameLength) = ""
	TPtrC iconFile(KNullDesC);
	if (aCaptionAndIcon.iIconFile!=NULL)
		{
		iconFile.Set(*aCaptionAndIcon.iIconFile);
		}
	WriteTextL(aDataSink, iconFile);
	}

void CApaLocalisableResourceFileWriter::MainResourceInCompiledFormatL(MDataSink& aDataSink) const
	{
	// LOCALISABLE_APP_INFO

	// LONG reserved_long = 0
	WriteLittleEndianUint32L(aDataSink, 0);

	// LLINK reserved_llink = 0
	WriteLittleEndianUint32L(aDataSink, 0);
	__ASSERT_DEBUG(iShortCaption, Panic(EPanicNullPointer));
	// LTEXT short_caption(KMaxCaption) = ""
	WriteTextL(aDataSink, *iShortCaption);

	// STRUCT caption_and_icon
	WriteCaptionAndIconInfoL(aDataSink, iCaptionAndIcon);

	// LEN WORD STRUCT view_list[]
	WriteLittleEndianUint16L(aDataSink, 0);
	__ASSERT_DEBUG(iGroupName, Panic(EPanicNullPointer));
	//	LTEXT group_name(KAppMaxGroupName) = ""
	WriteTextL(aDataSink, *iGroupName);
	}

const TDesC8* CApaLocalisableResourceFileWriter::SecondResourceL(TBool& aSecondResourceInCompressedUnicodeFormat) const
	{
	aSecondResourceInCompressedUnicodeFormat=EFalse;
	return NULL;
	}
	
