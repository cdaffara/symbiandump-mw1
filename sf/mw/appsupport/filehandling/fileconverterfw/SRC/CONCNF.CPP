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

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include "coninternal.h"
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS
#include <concnf.h>
#include <conarc.h>
#include "CONSTD.H"

#include <f32file.h>
#include <s32file.h>

const TInt KConverterGranularity=2;
const TInt KMimeTypeGranularity=3;
const TInt KLanguageInfoGranularity=3;
const TInt KConarcPluginResourceId = 1;

TLanguageInfo::TLanguageInfo()
	:iLanguage(ELangEnglish), iTranslation(KNullDesC)
	{}

EXPORT_C TLanguageInfo::TLanguageInfo(TLanguage aLanguage,TTranslation aTranslation)
/** Constructor specifying language ID and localised name.

@param aLanguage Language ID
@param aTranslation Localised name in the specified language */
	{
	iLanguage=aLanguage;
	iTranslation=aTranslation;
	}

void TLanguageInfo::InternalizeL(RReadStream& aStream)
	{
	iLanguage=(TLanguage)aStream.ReadInt8L();
	aStream >> iTranslation;
	}

void TLanguageInfo::ExternalizeL(RWriteStream& aStream) const
	{
	aStream.WriteInt8L(iLanguage);
	aStream << iTranslation;
	}

TLanguage TLanguageInfo::Language()
	{
	return iLanguage;
	}

TTranslation TLanguageInfo::Translation()
	{
	return iTranslation;
	}

void TLanguageInfo::ReadFromResource(TResourceReader& aReader)
	{
	// Skip over extension
	aReader.ReadUint32();
	TInt enumInt=aReader.ReadInt16();
	iLanguage=static_cast<TLanguage>(enumInt);
    iTranslation=aReader.ReadTPtrC(); 
	}

EXPORT_C CMimeInfo::~CMimeInfo()
/** Destructor. */
	{
	delete iLanguageInfo;
	}

void CMimeInfo::ConstructL()
	{
	iLanguageInfo = new(ELeave) CArrayFixFlat<TLanguageInfo>(KLanguageInfoGranularity);
	}

CMimeInfo* CMimeInfo::NewLC(const CMimeInfo* aMimeInfo)
	{
	CMimeInfo* self= new(ELeave) CMimeInfo();
	self->iMime=aMimeInfo->iMime;
	
	CleanupStack::PushL(self);
	self->ConstructL();
	TInt count = aMimeInfo->iLanguageInfo->Count();
	for (TInt i=0; i<count;i++ )
		{
		TLanguageInfo info(aMimeInfo->iLanguageInfo->At(i).Language(),aMimeInfo->iLanguageInfo->At(i).Translation());
		self->iLanguageInfo->AppendL(info);
		}
	return self;
	}

EXPORT_C CMimeInfo* CMimeInfo::NewLC(TDataType& aMimeType)
/** Allocates and constructs a CMimeInfo for a specified data type, leaving the 
object on the cleanup stack.

@param aMime Data type
@return New CMimeInfo */
	{
	CMimeInfo* self= new(ELeave) CMimeInfo();
	self->iMime=aMimeType;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CMimeInfo* CMimeInfo::NewL()
	{
	CMimeInfo* self= new(ELeave) CMimeInfo();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CMimeInfo::InternalizeL(RReadStream& aStream)
	{
	aStream >>iMime;
	TInt count = aStream.ReadInt8L();
	for (TInt i=0; i<count;i++ )
		{
		TLanguageInfo info;
		info.InternalizeL(aStream);
		iLanguageInfo->AppendL(info);

		}
	}

void CMimeInfo::ExternalizeL(RWriteStream& aStream) const
	{
	aStream << iMime;
	TInt count = iLanguageInfo->Count();
	aStream.WriteInt8L(count);
	for (TInt i=0; i<count;i++ )
		{
		iLanguageInfo->At(i).ExternalizeL(aStream);;
		}
	}

TDataType CMimeInfo::Mime()
	{
	return iMime;
	}

/**
 * return the translation for the current language
 * if the current lanuage does not exist the use english or the first in the list
 */
TTranslation CMimeInfo::Text()
	{
	TInt count = iLanguageInfo->Count();
	// no translations => return the mime description
	if (count==0)
		{
		TPtrC ptr=iMime.Des();
		if (ptr.Length()<=KMaxTranslation)
			return ptr;
		return (ptr.Left(KMaxTranslation));
		}
	TLanguage currentlanguage=User::Language();
	TInt englishIndex=0;
	for (TInt ii=0;ii<count;ii++)
		{
		if (iLanguageInfo->At(ii).Language()==currentlanguage)
			return iLanguageInfo->At(ii).Translation();
		if (iLanguageInfo->At(ii).Language()==ELangEnglish)
			englishIndex=ii;
		}
	return iLanguageInfo->At(englishIndex).Translation();
	}

EXPORT_C void CMimeInfo::AddLanguageL(TLanguageInfo aLang)
/** Adds a localised name for the data type.

@param aLang Localised name */
	{
	iLanguageInfo->AppendL(aLang);
	}

CMimeInfo* CMimeInfo::NewLC(TResourceReader& aReader)
	{
	CMimeInfo* self= new(ELeave) CMimeInfo();
	CleanupStack::PushL(self);
	self->ConstructL(aReader);
	return self;
	}

void CMimeInfo::ConstructL(TResourceReader& aReader)
	{
    // read 
	// Skip over extension
	aReader.ReadUint32();
	iMime = aReader.ReadTPtrC8();
	iLanguageInfo = new(ELeave) CArrayFixFlat<TLanguageInfo>(KLanguageInfoGranularity);
    AddLanguageInfoL(aReader);
	}

void CMimeInfo::AddLanguageInfoL(TResourceReader& aReader)
	{
    const TInt numOfLangInfo=aReader.ReadInt16();
    for (TInt index = 0; index < numOfLangInfo ; index++)
        {
  		TLanguageInfo info;
		info.ReadFromResource(aReader);
		iLanguageInfo->AppendL(info);    
        }
	}

//
// class CCnaConverter
//

CCnaConverter* CCnaConverter::NewLC()
	{
	CCnaConverter* self=new(ELeave) CCnaConverter();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

EXPORT_C CCnaConverter* CCnaConverter::NewL(TUid aUid)
/** Allocates and constructs a CCnaConverter for a specified converter.

@param aUid Converter UID
@return New CCnaConverter */
	{
	CCnaConverter* self=CCnaConverter::NewLC(aUid);
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CCnaConverter* CCnaConverter::NewLC(TUid aUid)
/** Allocates and constructs a CCnaConverter for a specified converter, leaving 
the object on the cleanup stack.

@param aUid Converter UID
@return New CCnaConverter */
	{
	CCnaConverter* self=new(ELeave) CCnaConverter();
	self->iUid = aUid;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

EXPORT_C CCnaConverter* CCnaConverter::NewL(const CCnaConverter& aConverter)
/** Allocates and copies a new CCnaConverter.

@param aConverter Object to copy
@return New CCnaConverter */
	{
	CCnaConverter* self=CCnaConverter::NewLC(aConverter);
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CCnaConverter* CCnaConverter::NewLC(const CCnaConverter& aConverter)
/** Allocates and copies a new CCnaConverter, leaving the object on the cleanup 
stack.

@param aConverter Object to copy
@return New CCnaConverter */
	{
	CCnaConverter* self=CCnaConverter::NewLC(aConverter.iUid);
	TInt count=aConverter.iMimeFrom->Count();
	TInt i;
	CMimeInfo* mimeinfo;
	for (i=0; i<count; i++)
		{
		mimeinfo=CMimeInfo::NewLC(aConverter.iMimeFrom->At(i));
		self->iMimeFrom->AppendL(mimeinfo);
		CleanupStack::Pop(mimeinfo);
		}
	count=aConverter.iMimeTo->Count();
	for (i=0; i<count; i++)
		{
		mimeinfo=CMimeInfo::NewLC(aConverter.iMimeTo->At(i));
		self->iMimeTo->AppendL(mimeinfo);
		CleanupStack::Pop(mimeinfo);
		}
	return self;
	}


CCnaConverter::CCnaConverter()
	{
	__DECLARE_NAME(_S("CCnaConverter"));
	}

EXPORT_C CCnaConverter::~CCnaConverter()
/** Destructor. */
	{
	if (iMimeFrom)
		iMimeFrom->ResetAndDestroy();
	delete iMimeFrom;
	if (iMimeTo)
		iMimeTo->ResetAndDestroy();
	delete iMimeTo;
	}

void CCnaConverter::ConstructL()
	{
	iMimeFrom= new (ELeave)CArrayPtrFlat<CMimeInfo>(KMimeTypeGranularity);
	iMimeTo= new (ELeave)CArrayPtrFlat<CMimeInfo>(KMimeTypeGranularity);
	}

EXPORT_C TBool CCnaConverter::MimeFrom(const TDataType& aMimeType) const
/** Tests if the specified source data type is available.

@param aMimeType Source data type
@return True if the converter supports the source data type, else false */
	{
	return FindMime(aMimeType,iMimeFrom);
	}

EXPORT_C TBool CCnaConverter::MimeTo(const TDataType& aMimeType) const
/** Tests if the specified target data type is available.

@param aMimeType Target data type
@return True if the converter supports the target data type, else false */
	{
	return FindMime(aMimeType,iMimeTo);
	}


TBool CCnaConverter::FindMime(const TDataType& aMimeType, const CArrayPtrFlat<CMimeInfo>* aList) const
// do the search
	{
	// !! need to use array searching code here?
	TInt count=aList->Count();
	for (TInt i=0; i<count; i++)
		{
		if (aMimeType==(*aList)[i]->Mime())
			return ETrue;
		}
	return EFalse;
	}

EXPORT_C TTranslation CCnaConverter::MimeFromText(const TDataType& aMimeType) const
/** Gets the localised name of the specified source data type.

@param aMimeType Source data type
@return Localised name of the source data type */
	{
	return FindMimeText(aMimeType,iMimeFrom);
	}

EXPORT_C TTranslation CCnaConverter::MimeToText(const TDataType& aMimeType) const
/** Gets the localised name of the specified target data type.

@param aMimeType Target data type
@return Localised name of the target data type */
	{
	return FindMimeText(aMimeType,iMimeTo);
	}

TTranslation CCnaConverter::FindMimeText(const TDataType& aMimeType, const CArrayPtrFlat<CMimeInfo>* aList) const
// do the search
	{
	TInt count=aList->Count();
	for (TInt i=0; i<count; i++)
		{
		if (aMimeType==(*aList)[i]->Mime())
			return (*aList)[i]->Text();
		}
	return KNullDesC();
	}

EXPORT_C TInt CCnaConverter::CountFrom() const
/** Gets the number of source data types for the converter.

@return Number of source data types */
	{
	return iMimeFrom->Count();
	}

EXPORT_C TInt CCnaConverter::CountTo() const
/** Gets the number of target data types for the converter.

@return Number of target data types */
	{
	return iMimeTo->Count();
	}

EXPORT_C TDataType CCnaConverter::MimeFrom(TInt aIndex) const
/** Gets the n'th source data type of the converter.

@param aIndex Index of the source data type to get
@return Source data type */
	{
	return iMimeFrom->At(aIndex)->Mime();
	}

EXPORT_C TDataType CCnaConverter::MimeTo(TInt aIndex) const
/** Gets the n'th target data type of the converter.

@param aIndex Index of the target data type to get
@return Target data type */
	{
	return iMimeTo->At(aIndex)->Mime();
	}

EXPORT_C void CCnaConverter::SetUid(TUid aUid)
/** Sets the converter UID.

@param aUid Converter UID */
	{
	iUid = aUid;
	}

EXPORT_C TTranslation CCnaConverter::MimeFromText(TInt aIndex) const
/** Gets the localised name of the n'th source data type.

@param aIndex Index of the source data type
@return Localised name of the source data type */
	{
	return iMimeFrom->At(aIndex)->Text();
	}

EXPORT_C TTranslation CCnaConverter::MimeToText(TInt aIndex) const
/** Gets the localised name of the n'th target data type.

@param aIndex Index of the target data type
@return Localised name of the target data type */
	{
	return iMimeTo->At(aIndex)->Text();
	}

CCnaConverter* CCnaConverter::NewL(TUid aUid,TResourceReader& aReader)
	{
	CCnaConverter* self=new(ELeave) CCnaConverter();
	CleanupStack::PushL(self);
	self->ConstructL(aUid, aReader);
	CleanupStack::Pop(self);
	return self;
	}

// used when creating object from a resource file.
void CCnaConverter::ConstructL(TUid aUid,TResourceReader& aReader)
	{
	iUid=aUid;
	iMimeFrom= new (ELeave)CArrayPtrFlat<CMimeInfo>(KMimeTypeGranularity);
	AddMimeDataL(aReader,*iMimeFrom);
	iMimeTo= new (ELeave)CArrayPtrFlat<CMimeInfo>(KMimeTypeGranularity);
	AddMimeDataL(aReader,*iMimeTo);
	}

void CCnaConverter::AddMimeDataL(TResourceReader& aReader,CArrayPtrFlat<CMimeInfo>& aMimeInfo)
	{
    const TInt number=aReader.ReadInt16();
    for (TInt index = 0; index < number ; index++)
		{
        CMimeInfo* mimeInfo=CMimeInfo::NewLC(aReader);
		aMimeInfo.AppendL(mimeInfo);
		CleanupStack::Pop(mimeInfo); // now mimeInfo safely in an array   
        }
	}


//
// class CCnaConvInfoFile2
//

CCnaConvInfoFile2::~CCnaConvInfoFile2()
	{
	if (iConverters)
		{
		iConverters->ResetAndDestroy();
		}
	delete iConverters;
	iResourceFile.Close();
	iFs.Close();
	}

void CCnaConvInfoFile2::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());
	iConverters=new(ELeave) CArrayPtrFlat<CCnaConverter>(KConverterGranularity);
	}

//
// class CCnaConvInfoFileReader2
//

EXPORT_C CCnaConvInfoFileReader2* CCnaConvInfoFileReader2::NewLC(const TDesC& aFileName)
/** Constructs a CCnaConvInfoFileReader2 object for a given .rsc file.

@param aFileName The .rsc file name.
@return The new CCnaConvInfoFileReader2 object. */
	{
	CCnaConvInfoFileReader2* self=new(ELeave) CCnaConvInfoFileReader2();
	CleanupStack::PushL(self);
	self->ConstructL(aFileName);
	return self;
	}

EXPORT_C CCnaConvInfoFileReader2* CCnaConvInfoFileReader2::NewL(const TDesC& aFileName)
/** Constructs a CCnaConvInfoFileReader2 object for a given .rsc file.

@param aFileName The .rsc file name.
@return The new CCnaConvInfoFileReader2 object. */
	{
	CCnaConvInfoFileReader2* self=CCnaConvInfoFileReader2::NewLC(aFileName);
	CleanupStack::Pop(self); 
	return self;
	}

CCnaConvInfoFileReader2::CCnaConvInfoFileReader2()
	{
	}

void CCnaConvInfoFileReader2::RestoreConverterL(TResourceReader& aReader)
	{
	// Skip over extension
	aReader.ReadUint32();
	TUid converterUid;
	converterUid.iUid=aReader.ReadInt32();
	CCnaConverter* tempConv= CCnaConverter::NewL(converterUid,aReader);
	CleanupStack::PushL(tempConv);
	iConverters->AppendL(tempConv);  // takes the ownership
	CleanupStack::Pop(tempConv); 
	}

EXPORT_C void CCnaConvInfoFileReader2::RestoreL()
/** Restores all the converters from the .rsc file that was specified in the NewL() or NewLC(). */
	{
	TResourceReader reader;
	HBufC8* resource=iResourceFile.AllocReadLC(KConarcPluginResourceId); //pushes onto the cleanup stack
	reader.SetBuffer(resource);
	// Skip over extension
	reader.ReadUint32();
	const TInt numOfConverters = reader.ReadUint16();
	for (TInt i = 0; i < numOfConverters; i++)
		{
		RestoreConverterL(reader);
		}
	CleanupStack::PopAndDestroy(resource);
	}

EXPORT_C TInt CCnaConvInfoFileReader2::Count()
/** Gets the number of converters that were restored from the .rsc file. 

This function returns zero if RestoreL() has not been called.

@return The number of converters. */
	{
	if (!iConverters)
		{
		return 0;
		}
	return iConverters->Count();
	}

EXPORT_C CCnaConverter* CCnaConvInfoFileReader2::AtL(TInt anIndex)
/** Constructs and returns a new CCnaConverter object that is a copy of the 
converter at the specified index.

@param anIndex An index into the object's array of converters. The array is created 
by calling RestoreL(). The index must be valid, or a panic occurs.
@return A copy of the converter at the specified index. The caller takes ownership. */
	{
	CCnaConverter* converter=CCnaConverter::NewL(*(iConverters->At(anIndex)));
	return converter;
	}

void CCnaConvInfoFileReader2::ConstructL(const TDesC& aFileName)
	{
	CCnaConvInfoFile2::ConstructL();
	iResourceFile.OpenL(iFs,aFileName);
	}
