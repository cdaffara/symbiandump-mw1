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

#include <exbuf.h>

//
// class CExportBufferBase
//

EXPORT_C CExportBufferBase::CExportBufferBase(TInt aSize)
	:iMaxSize(aSize)
	{}

EXPORT_C CExportBufferBase::~CExportBufferBase()
	{
	delete iExportBuffer;
	}

EXPORT_C void CExportBufferBase::ConstructL()
	{
	iExportBuffer=HBufC::NewL(iMaxSize);
	ResetExportBuffer();
	}

EXPORT_C void CExportBufferBase::ResetExportBuffer()
//
	{
	iWritePtr=CONST_CAST(TText*,iExportBuffer->Ptr());
	iTBase=iWritePtr;
	}

EXPORT_C void CExportBufferBase::ResetWritePtr()
	{
	iWritePtr=iTBase;
	}

EXPORT_C void CExportBufferBase::FinalizeExport()
	{
	iExportBufferLength=iWritePtr-iTBase;
	}

EXPORT_C void CExportBufferBase::InsertL(TText aText)
	{
	if (iWritePtr>=iTBase+iMaxSize)
		CommitExportBufferL();
	*(iWritePtr++)=aText;
	}

//
// class CExportDynamicBuffer
//

CExportDynamicBuffer::CExportDynamicBuffer(TInt aSize, CBufBase& aDynamicBuffer)
	:CExportBufferBase(aSize),
	iOutput(aDynamicBuffer)
	{}

EXPORT_C CExportDynamicBuffer* CExportDynamicBuffer::NewL(TInt aSize, CBufBase& aDynamicBuffer)
	{
	CExportDynamicBuffer* self=new(ELeave) CExportDynamicBuffer(aSize, aDynamicBuffer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

EXPORT_C void CExportDynamicBuffer::ConstructL()
	{
	CExportBufferBase::ConstructL();
	}

EXPORT_C void CExportDynamicBuffer::CommitExportBufferL()
//
	{
	FinalizeExport();
	iOutput.InsertL(iOutput.Size(),iExportBuffer->Ptr(),iExportBufferLength);
	ResetWritePtr();
	}
