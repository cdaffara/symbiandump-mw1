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
// Converter Architecture wrappers for base64 conversion
// 
//

#include "BASE64.H"
#include <s32buf.h>
#include "B64CONV.H"

void CBase64Encoder::ConvertObjectAL(RReadStream& aReadStream, RWriteStream& aWriteStream, MConverterUiObserver* aObserver)
	{
	if (!iCodec)
		iCodec=new(ELeave) CBase64CodecBase;
	else
		iCodec->Reset();
	iReadStream=aReadStream;
	iWriteStream=aWriteStream;
	iObserver=aObserver;
	}

CBase64Encoder::~CBase64Encoder()
	{
	delete iCodec;
	}

TBool CBase64Encoder::DoConvertL()
	{
	__ASSERT_DEBUG(iCodec,User::Invariant());
	const TInt inLen=384;
	const TInt outLen=512;
	TBuf8<inLen> inbuf;
	TBuf8<outLen> outbuf;
	TRAPD(err,iReadStream.ReadL(inbuf,inLen));
	if (!(err==KErrNone || err==KErrEof))
		User::Leave(err);
	else
		{
		if (err==KErrEof)
			inbuf.SetLength(iReadStream.Source()->SizeL()%inLen);
		iCodec->Encode(inbuf,outbuf);
		iWriteStream.WriteL(outbuf);
		}
	return err!=KErrEof;
	}

TUid CBase64Encoder::Uid()
	{
	return KBase64EncoderUid;
	}

TInt CBase64Encoder::Capabilities()
	{
	return EConvertsObjects;
	}

void CBase64Encoder::CancelConvert()
	{
	__ASSERT_DEBUG(iCodec,User::Invariant());
	iCodec->Reset();
	}

//
// class CBase64Decoder
//

CBase64Decoder::~CBase64Decoder()
	{
	delete iCodec;
	}

void CBase64Decoder::ConvertObjectAL(RReadStream& aReadStream, RWriteStream& aWriteStream, MConverterUiObserver* aObserver)
	{
	if (!iCodec)
		iCodec=new(ELeave) CBase64CodecBase;
	else
		iCodec->Reset();
	iReadStream=aReadStream;
	iWriteStream=aWriteStream;
	iObserver=aObserver;
	}

TBool CBase64Decoder::DoConvertL()
	{
	__ASSERT_DEBUG(iCodec,User::Invariant());
	const TInt inLen=512;
	const TInt outLen=384;
	TBuf8<inLen> inbuf;
	TBuf8<outLen> outbuf;
	TRAPD(err,iReadStream.ReadL(inbuf,inLen));
	if (!(err==KErrNone || err==KErrEof))
		User::Leave(err);
	else
		{
		if (err==KErrEof)
			inbuf.SetLength(iReadStream.Source()->SizeL()%inLen);
		iCodec->Decode(inbuf,outbuf);
		iWriteStream.WriteL(outbuf);
		}
	return err!=KErrEof;
	}

TUid CBase64Decoder::Uid()
	{
	TUid uid=KBase64DecoderUid;
	return uid;
	}

TInt CBase64Decoder::Capabilities()
	{
	return EConvertsObjects;
	}

void CBase64Decoder::CancelConvert()
	{
	__ASSERT_DEBUG(iCodec,User::Invariant());
	iCodec->Reset();
	}
