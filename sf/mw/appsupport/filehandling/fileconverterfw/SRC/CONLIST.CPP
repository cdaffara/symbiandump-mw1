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

#include <f32file.h>
#include <e32uid.h>
#include <s32file.h>
#include "B64CONV.H"
#include "TXCONV.H"
#include "QPCONV.H"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include "coninternal.h"
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS
#include "CONLIST.H"
#include "CONSTD.H"

const TInt KConverterListGranularity=5;

// 
// class CCnaConverterList
//

EXPORT_C CCnaConverterList* CCnaConverterList::NewL()
/** Allocates and constructs a CCnaConverterList.

@return New converter list */
	{
	CCnaConverterList* self=CCnaConverterList::NewLC();
	CleanupStack::Pop(); // self
	return self;
	}

EXPORT_C CCnaConverterList* CCnaConverterList::NewLC()
/** Allocates and constructs a CCnaConverterList, leaving the object on the cleanup 
stack.

@return New converter list */
 	{
	CCnaConverterList* self=new(ELeave) CCnaConverterList();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CCnaConverterList::CCnaConverterList()
	{
	__DECLARE_NAME(_S("CCnaConverterList"));
	}

void CCnaConverterList::ConstructL()
	{
	iConverters=new(ELeave) CArrayPtrFlat<CCnaConverter>(KConverterListGranularity); 
	}

EXPORT_C CCnaConverterList::~CCnaConverterList()
/** Destructor.

This releases all loaded converter DLLs. */
	{
	if (iConverters)
		{
		iConverters->ResetAndDestroy();
		delete iConverters;
		}
	iImplementationArray.ResetAndDestroy();
	}

EXPORT_C void CCnaConverterList::Release()
/** Legacy function, do not use. */
	{
	}

EXPORT_C TInt CCnaConverterList::CountL()
/** Gets the total number of converters.

It can leave because it needs to rescan the disk to refresh 
the list of converters.

@return Total number of converters */
	{
	if (!iDoneScan)
		UpdateL();
	TInt count=0;
	return count+iImplementationArray.Count();
	}

EXPORT_C CConverterBase* CCnaConverterList::NewConverterL(TUid aUid)
/** Locates the converter with the specified UID, loads its associated DLL
and returns a pointer to it.

@param aUid Converter UID
@return Converter with the specified UID, or NULL if there is no suitable converter */
	{
	CConverterBase* base=ConverterListUtil::UtilityConverterL(aUid);
	if (base)
		return base;
	if (!iDoneScan)
		UpdateL();
	TInt i;
	TInt count;
	count = iImplementationArray.Count();
	for (i = 0; i < count; i++)
		{
		if (iImplementationArray[i]->ImplementationUid()==aUid)
			{
			return LoadConverterL(iImplementationArray[i]->ImplementationUid());
			}
		}
	return NULL;
	}

EXPORT_C void CCnaConverterList::UpdateL()
/** Requests a rescan of the disk to refresh the list of converters. */
	{
	// delete old array and populate it with new values
	iImplementationArray.ResetAndDestroy();
	REComSession::ListImplementationsL(KUidConverterDll16,iImplementationArray);
	for (TInt i=0; i<iImplementationArray.Count(); i++)
		{// load the conarc-plugin resource file. This should be set to 1 in
		 // the ecom resource file to show there is extra file to load
		if (iImplementationArray[i]->OpaqueData().Length()!=0 )
			{ 
			_LIT(KPath,"z:\\resource\\convert\\");
			_LIT(KExtension,".rsc");
			TFileName resourceFile;
			resourceFile.Copy(KPath);
			resourceFile.AppendNum((TInt)iImplementationArray[i]->ImplementationUid().iUid, EHex);
			resourceFile.Append(KExtension);
			CCnaConvInfoFileReader2* readConverter=CCnaConvInfoFileReader2::NewL(resourceFile);
			CleanupStack::PushL(readConverter);
			readConverter->RestoreL();
			const TInt converterCount = readConverter->Count();
			for (TInt j = 0 ; j < converterCount ; j++ )
				{
				CCnaConverter* tempConv= readConverter->AtL(j);
				CleanupStack::PushL(tempConv);
				iConverters->AppendL(tempConv);
				CleanupStack::Pop(tempConv);
				}
			CleanupStack::PopAndDestroy(readConverter);
			}	
		}
	iDoneScan=ETrue;
	}

EXPORT_C TUid CCnaConverterList::ConvFromL(const TDataType& aMimeType)
// searches for a particular converter that converts from the specified Mime Type
// 
// To be called iteratively to find all available converters
// for the specified mime type
// 
// returns the Uid of the converter or NULL when no converters remain
/** Gets the UID of the converter that converts from the specified data type.

@param aMimeType Source data type
@return Converter UID */
	{
	TInt i;
	TInt count=iConverters->Count();
	for (i=0; i < count; i++)
		{
		CCnaConverter* converter = iConverters->At(i);
		if (converter->MimeFrom(aMimeType))
			{
			return converter->Uid();
			}
		}
	return KNullUid;
	}

EXPORT_C TUid CCnaConverterList::ConvToL(const TDataType& aMimeType)
// searches for a particular converter that converts to the specified Mime Type
// as for GetConvFromL() but specifing a target mime type
/** Gets the UID of the converter that converts to a specified data type.

@param aMimeType Target data type
@return Converter UID */
	{
	TInt i;
	TInt count=iConverters->Count();
	for (i=0; i < count; i++)
		{
		CCnaConverter* converter = iConverters->At(i);
		if (converter->MimeTo(aMimeType))
			{
			return converter->Uid();
			}
		}
	return KNullUid;
	}

EXPORT_C TUid CCnaConverterList::ConverterL(const TDataType& aFrom,const TDataType& aTo)
/** Gets the UID of the converter that converts to and from the specified data types.

@param aFrom Source data type
@param aTo Target data type
@return Converter UID */
	{
	if (!iDoneScan)
		UpdateL();
	TInt ii;
	TInt count=iConverters->Count();
	for (ii=0;ii<count;ii++)
		{
		const CCnaConverter* conv=iConverters->At(ii);
		if (conv->MimeFrom(aFrom) && conv->MimeTo(aTo))
			{
			return conv->Uid();
			}
		}
	return KNullUid;
	}


// Compiles a list of converters that convert FROM a mime type.
// Produces an array of the translations of the types they convert FROM 
// and a corresponding array of uids.
// The data type to convert TO is supplied

EXPORT_C void CCnaConverterList::ConvFromListL(const TDataType& aMimeType,CArrayFix<SConverterInfo>* aSConverterInfoArray)
/** Gets a list of converters that convert from a specified data type.

@param aMimeType Source data type
@param aSConverterInfoArray On return, array of converter information objects 
for all suitable converters */
	{
	ASSERT(aSConverterInfoArray->Count()==0);
	if (!iDoneScan)
		UpdateL();
	
	TInt i;
	TInt count = iConverters->Count();
	for (i = 0; i < count; i++)
		{
		CCnaConverter* conv = iConverters->At(i);
		//does converter support this type
		if (conv->MimeFrom(aMimeType) )
			{
			const TInt countTo = conv->CountTo();
			//add the types that it converts to
			for( TInt j=0;j<countTo;j++)
				{
				SConverterInfo info;
				info.iTranslation=conv->MimeToText(j);
				info.iUid=conv->Uid();
				info.iIndex=j;
				aSConverterInfoArray->AppendL(info);
				}
			}
		}

	}

// Compiles a list of converters that convert TO a mime type.
// Produces an array of the translations of the types they convert TO
// and a corresponding array of uids.
// The data type to convert FROM is supplied

EXPORT_C void CCnaConverterList::ConvToListL(const TDataType& aMimeType,CArrayFix<SConverterInfo>* aSConverterInfoArray)
/** Gets a list of converters that convert to a specified data type.

@param aMimeType Target data type
@param aSConverterInfoArray On return, an array of converter information objects 
for all suitable converters */
	{
	ASSERT(aSConverterInfoArray->Count()==0);
	if (!iDoneScan)
		UpdateL();
	TInt i;
	TInt count = iConverters->Count();
	for (i = 0; i < count; i++)
		{
		CCnaConverter* conv = iConverters->At(i);
		//does converter support this type
		if (conv->MimeTo(aMimeType) )
			{
			const TInt countFrom=conv->CountFrom();
			for( TInt j=0;j<countFrom;j++)
				{
				SConverterInfo info;
				info.iTranslation=conv->MimeFromText(j);
				info.iUid=conv->Uid();
				info.iIndex=j;
				aSConverterInfoArray->AppendL(info);
				}
			}
		}
	}

EXPORT_C TInt CCnaConverterList::MimeTypeFrom(TDataType& aDataType,const SConverterInfo& aInfo) const
/** Gets converter information for a specified source data type.

@param aDataType Source data type
@param aConverterInfo On return, converter information for a suitable converter
@return KErrNone if a suitable converter was found, else KErrNotFound */	
	{
	TInt i;
	TInt count = iConverters->Count();
	for (i = 0; i < count; i++)
		{
		const CCnaConverter* conv = iConverters->At(i);
		if (conv->Uid()==aInfo.iUid)
			{
			if((aInfo.iIndex>=0) && (aInfo.iIndex<conv->CountFrom()))
				{
				aDataType=conv->MimeFrom(aInfo.iIndex);
				return KErrNone;
				}
			break;
			}
		}
	return KErrNotFound;
	}

EXPORT_C TInt CCnaConverterList::MimeTypeTo(TDataType& aDataType,const SConverterInfo& aInfo) const
/** Gets converter information for a specified target data type.

@param aDataType Target data type
@param aConverterInfo On return, converter information for a suitable converter
@return KErrNone if a suitable converter was found, else KErrNotFound */
	{
	TInt i;
	TInt count = iConverters->Count();
	for (i = 0; i < count; i++)
		{
		const CCnaConverter* conv = iConverters->At(i);
		if (conv->Uid()==aInfo.iUid)
			{
			if(aInfo.iIndex>=0 && aInfo.iIndex<conv->CountTo())
				{
				aDataType=conv->MimeTo(aInfo.iIndex);
				return KErrNone;
				}
			break;
			}
		}
	return KErrNotFound;
	}

CConverterBase* CCnaConverterList::LoadConverterL(TUid aImplUid)
	{
	return CConverterBase2::CreateConverterL(aImplUid);
	}

CConverterBase* ConverterListUtil::UtilityConverterL(TUid aUid)
	{
	switch (aUid.iUid)
		{
	case KBase64Decoder:
		return new(ELeave) CBase64Decoder();
	case KBase64Encoder:
		return new(ELeave) CBase64Encoder();
	case KQuotedPrintableToText:
		return new(ELeave) CQpToTxtCnv();
	case KTextToQuotedPrintable:
		return new(ELeave) CTxtToQpCnv();
	case KEtextToText:
		return new(ELeave) CEtToTxtCnv();
	case KTextToEtext:
		return new(ELeave) CTxtToEtCnv();
	case KTextToEtextNoTrim:
		return new(ELeave) CTxtToEtCnv(ETrue);	
	default:
		return NULL;
		}
	}

TInt ConverterListUtil::CountUtilityConverters()
	{
	return 6;
	}
