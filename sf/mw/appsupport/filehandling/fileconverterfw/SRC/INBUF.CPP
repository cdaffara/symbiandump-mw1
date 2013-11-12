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

#include <e32base.h>
#include <e32def.h>

#include <f32file.h> // for CFileImportBuffer
#include <s32file.h>
#include <s32stor.h>

#include <inbuf.h>

//
// class CImportBufferBase
//

EXPORT_C CImportBufferBase::CImportBufferBase(TInt aSize)
	:iMaxSize(aSize)
	{}

EXPORT_C CImportBufferBase::~CImportBufferBase()
	{
	delete iImportBuffer;
	}

EXPORT_C void CImportBufferBase::ConstructL()
	{
	iImportBuffer=HBufC8::NewL(iMaxSize);
	ResetImportBuffer();
	}

EXPORT_C void CImportBufferBase::ResetImportBuffer()
//
	{
	iReadPtr=CONST_CAST(TText8*,iImportBuffer->Ptr());
	iTBase=iReadPtr;
	iEndBuf=iReadPtr;
	}

EXPORT_C void CImportBufferBase::ResetReadPtr()
	{
	iReadPtr=iTBase;
	}

EXPORT_C TInt CImportBufferBase::ReadL(TText8& aByte)
	{
 	if (iReadPtr == iEndBuf)
		{
		TInt err = FillImportBuf();
		if (err)
			return err;
		}
	aByte = *(iReadPtr++);
	return KErrNone;
	}

EXPORT_C TInt CImportBufferBase::SeekL(TSeek /*aSeekMode*/, TInt /*anOffset*/)
	{
	// not implemented
	return KErrNotSupported;
	}

EXPORT_C TInt CImportBufferBase::MaxSize()
	{
	return iMaxSize;
	}

EXPORT_C void CImportBufferBase::SetLength(TInt aLength)
	{
	iEndBuf=iTBase+aLength;
	}

//
// class CFileImportBuffer
//

EXPORT_C CFileImportBuffer::CFileImportBuffer(RFile aSource, TInt aMaxSize)
	: CImportBufferBase(aMaxSize), iSource(aSource)
	{}

EXPORT_C TInt CFileImportBuffer::SeekL(TSeek aSeekMode, TInt anOffset)
	{
	TInt err=iSource.Seek(aSeekMode, anOffset);
	if (err==KErrNone)
		return FillImportBuf();
	else
		return err;
	}

EXPORT_C TInt CFileImportBuffer::FillImportBuf()
	{
  	TPtr8 buf=iImportBuffer->Des();
	iSource.Read(buf,MaxSize());
	SetLength(buf.Length());
	ResetReadPtr();
	if (buf.Length() == 0)
		return KErrEndOfInput;
	else
		return KErrNone;
	}
