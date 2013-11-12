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

#include "PRNSTD.H"
#include <s32strm.h>
#include <flddef.h>
#include <fldbltin.h>
#include <txtrich.h>
#include <txtetext.h>
#include <txtfmlyr.h>
#include <prninf.h>
#include "PRNRES.H"

//
// CHeaderFooter
//

EXPORT_C CHeaderFooter* CHeaderFooter::NewL()
/** Allocates and constructs an uninitialised CHeaderFooter object.

@return The newly constructed CHeaderFooter object. */
	{
	CHeaderFooter* self=new(ELeave) CHeaderFooter();
	return self;
	}


CHeaderFooter::CHeaderFooter()
	{}

EXPORT_C CHeaderFooter::~CHeaderFooter()
/** Destructor.

This frees all resources owned by the CHeaderFooter object, prior to its destruction. */
	{
	delete iText;
	delete iParaLayer;
	delete iCharLayer;
	}

EXPORT_C TStreamId CHeaderFooter::StoreL(CStreamStore& aStore)const
/** Stores a CHeaderFooter object and all associated components to a stream store.

Components include rich text-specific formatting, fields and pictures.

@param aStore The store to which the CHeaderFooter should be stored. 
@leave KErrNoMemory Out of memory.
@return ID of the stream containing the external representation of the CHeaderFooter 
object */
	{
	CStoreMap* map=CStoreMap::NewLC(aStore);
	StoreComponentsL(aStore,*map);
//
	RStoreWriteStream stream(*map);
	TStreamId id=stream.CreateLC(aStore);
	stream<< *this;
	stream.CommitL();
//
	map->Reset();
	CleanupStack::PopAndDestroy(2);
	return id;
	}

EXPORT_C void CHeaderFooter::StoreComponentsL(CStreamStore& aStore,CStoreMap& aMap)const
/** Stores this object's components to a stream store, specifying a stream store 
and store map. 

Components include fields and pictures.

This function does not store the text content or global formatting of the 
header and footer, so it may need to be accompanied by a call to ExternalizeL().

@param aStore The store to which the components are stored 
@param aMap Table of swizzles. Supports the deferred loading of the rich text 
components of the header or footer. */
	{
	if (iText)
		iText->StoreComponentsL(aStore,aMap);
	}

EXPORT_C void CHeaderFooter::ExternalizeL(RWriteStream& aStream) const
/** Externalises this object's text content and global format layers to a write 
stream. 

The object's components are not externalised. The presence of this function 
means that the standard templated operator<<() (defined in s32strm.h) is available 
to externalise objects of this class.

This function does not store any fields, pictures or rich text-specific formatting, 
so may need to be accompanied by a call to StoreComponentsL().

@param aStream Stream to which the object should be externalised. */
	{
	TUint flags=0x00;
	if (iText) 
		flags |= ETextExists;
	aStream.WriteInt8L(flags);
	aStream.WriteInt32L(iFirstPageToPrintTo);
	// stream the text if it exists
	if (iText)
		{
		aStream<< *iParaLayer;
		aStream<< *iCharLayer;
		aStream<< *iText;
		}
	}

EXPORT_C void CHeaderFooter::RestoreL(const CStreamStore& aStore,TStreamId aStreamId,MPictureFactory* aFactory)
/** Restores this object and all associated components from a stream store. 

Components include fields and pictures.

@param aStore Stream store from which to restore the CHeaderFooter. 
@param aStreamId ID of the stream containing the external representation of 
the CHeaderFooter object. 
@param aFactory Picture factory. Must be supplied if the header or footer contains 
pictures which should be restored. */
	{
	// retrieve the headstream from the store
	RStoreReadStream stream;
	stream.OpenLC(aStore,aStreamId);
	// internalize the headstream, then fetch the components from their own streams
	InternalizeL(stream);
	CleanupStack::PopAndDestroy(); // stream
	RestoreComponentsL(aStore,aFactory);
	}

EXPORT_C void CHeaderFooter::RestoreComponentsL(const CStreamStore& aStore,MPictureFactory* aFactory)
/** Restores this object's components from a stream store.

Components include fields and pictures.

This function does not restore the text content or global formatting of the 
header and footer, so it may need to be accompanied by a call to InternalizeL().

@param aStore Stream store from which the components are restored. 
@param aFactory Picture factory. Must be supplied if the header or footer contains 
pictures which should be restored. */
	{
	if (iText)
		{
		TStoreResolver resolver(aStore);
		if (aFactory)
			iText->SetPictureFactory(aFactory,&resolver);
		iText->RestoreComponentsL(aStore);
		if (aFactory)
			{
			iText->DetachFromStoreL(CPicture::EDetachFull);
			iText->SetPictureFactory(aFactory,NULL); // panics if I pass in a null factory...
			}
		}
	}

EXPORT_C void CHeaderFooter::InternalizeL(RReadStream& aStream)
/** Internalises this object's text content and global format layers from a read 
stream. 

The presence of this function means that the standard templated operator>>() 
(defined in s32strm.h) is available to internalise objects of this class.

This function does not restore any fields, pictures or rich text-specific 
formatting, so may need to be accompanied by a call to RestoreComponentsL().

@param aStream Stream from which the object should be internalised. */
	{
	TUint flags = aStream.ReadInt8L();
	iFirstPageToPrintTo = aStream.ReadInt32L();
	if (flags & ETextExists)
		{
		delete iText;
		iText = NULL;
		CreateFormatLayersL();
		aStream>> *iParaLayer;
		aStream>> *iCharLayer;
		iText = CRichText::NewL(iParaLayer,iCharLayer);
		iText->SetFieldFactory(this);
		aStream>> *iText;
		}
	}

EXPORT_C void CHeaderFooter::CreateTextL()
/** Allocates and constructs the rich text object owned by the header or footer.

The object's formatting is based on default values. If the header or footer's 
rich text object has already been allocated, the object is deleted and replaced 
by the one created by this function. So, calling this function causes a header 
or footer to lose all its existing text content, fields and formatting. */
	{
	delete iText;
	iText = NULL;
	CreateFormatLayersL();
	iText = CRichText::NewL(iParaLayer,iCharLayer,CEditableText::EFlatStorage);
	iText->SetFieldFactory(this);
	}


void CHeaderFooter::CreateFormatLayersL()
	{
	delete iParaLayer;
	delete iCharLayer;
	iParaLayer = NULL;
	iCharLayer = NULL;
	
	TParaFormatMask paraMask;
	iParaLayer=CParaFormatLayer::NewL((CParaFormat*)NULL,paraMask);

	TCharFormat charFormat;
	TCharFormatMask charMask;
	iCharLayer=CCharFormatLayer::NewL(charFormat,charMask);
	}


CTextField* CHeaderFooter::NewFieldL(TUid aFieldType)
// Creates a field  of the type specified
// Returns NULL if it doesnt understand the type
// 
	{
	CTextField* field=NULL;
	// cant switch Uid's, so...
	if (aFieldType==KDateTimeFieldUid)
		field = (CTextField*)new(ELeave) CDateTimeField();
	//
	else if ((aFieldType==KPageNumberFieldUid)&&(iPageNumInfo))
		{
		field = (CTextField*)new(ELeave) CPageNumField();
		((CPageNumField*)field)->SetPageNumInfo(iPageNumInfo);
		}
	//
	else if ((aFieldType==KNumPagesFieldUid)&&(iNumPagesInfo))
		{
		field = (CTextField*)new(ELeave) CNumPagesField();
		((CNumPagesField*)field)->SetNumPagesInfo(iNumPagesInfo);
		}
	//
	else if ((aFieldType==KFileNameFieldUid)&&(iFileNameInfo))
		{
		field = (CTextField*)new(ELeave) CFileNameField();
		((CFileNameField*)field)->SetFileNameInfo(iFileNameInfo);
		}
	//
	return field;
	}

EXPORT_C void CHeaderFooter::SetFileNameInfo(const MFieldFileNameInfo& aFileNameInfo)
/** Sets an object to update the document's filename.

If the header or footer supports filename fields, use this function to set 
an object that implements the MFieldFileNameInfo interface.

@param aFileNameInfo Provides a filename update function. */
	{
	iFileNameInfo = CONST_CAST(MFieldFileNameInfo*,&aFileNameInfo);
	}

/*
 *
 * Sets class to update page number information.
 *
 * If the header or footer is to support the insertion of page number
 * fields, use this function to set up a class deriving from
 * <code>MFieldPageNumInfo</code> which implements its
 * <code>UpdateFieldPageNum()</code> function. This implementation should
 * return the current page number.Note:Class <code>CPrintSetup</code>
 * derives from <code>MFieldPageNumInfo</code>, and implements its
 * <code>UpdateFieldPageNum()</code> function. On construction, a
 * <code>CPrintSetup</code> object automatically calls
 * <code>SetPageNumInfo()</code> for both its header and footer, removing
 * the need for the user of <code>CPrintSetup</code> to call
 * <code>SetPageNumInfo()</code>.
 *
 * @param     "const MFieldPageNumInfo& aPageNumInfo"
 *            Provides update page number function.
 */
void CHeaderFooter::SetPageNumInfo(const MFieldPageNumInfo& aPageNumInfo)
/** This function is used internally by the CPrintSetup class. */
	{
	iPageNumInfo = CONST_CAST(MFieldPageNumInfo*,&aPageNumInfo);
	}

EXPORT_C void CHeaderFooter::SetNumPagesInfo(const MFieldNumPagesInfo& aInfo)
/** Sets an object to update the total number of pages.

If the header or footer supports total number of pages fields, use this function 
to set up an object that implements the MFieldNumPagesInfo interface.

@param aNumPagesInfo Provides a function that updates the total number of 
pages in the document. */
	{
	iNumPagesInfo = CONST_CAST(MFieldNumPagesInfo*,&aInfo);
	}

EXPORT_C void CHeaderFooter::SetFirstPageToPrintTo(TInt aPageNum)
/** Sets the first page on which the header or footer is to be printed, allowing 
headers and footers to be omitted from the beginning of a document.

Page numbering begins at zero.

@param aPageNum The number of the first page on which the header or footer 
is to be printed. The first page is numbered zero. If less than zero, a panic 
occurs. */
	{ 
	__ASSERT_ALWAYS(aPageNum>=0,Panic(EPanicNonPositivePageNumber));
	iFirstPageToPrintTo = aPageNum; 
	}

EXPORT_C CRichText* CHeaderFooter::Text()const
/** Gets the rich text object owned by this header or footer. 

Through this object, rich text, fields and pictures can be inserted. This 
function can also be used to find out whether the rich text component is NULL, 
in which case a call to CreateTextL() or SetText() may be necessary.

@return The rich text object owned by the header or footer. */
	{
	return iText;
	}

EXPORT_C CParaFormatLayer* CHeaderFooter::ParaFormat()const
/** Gets the paragraph format layer which is owned by the header or footer.

The header or footer's paragraph formatting is based upon this layer.

@return The format layer upon which the header or footer's paragraph formatting 
is based. */
	{
	return iParaLayer;
	}

EXPORT_C CCharFormatLayer* CHeaderFooter::CharFormat()const
/** Gets the character format layer upon which the header or footer's character 
formatting is based.

@return The format layer upon which the header or footer's character formatting 
is based. */
	{
	return iCharLayer;
	}

EXPORT_C void CHeaderFooter::SetText(CRichText* aText)
/** Sets the rich text object which is owned by the header or footer.

The character and paragraph format layers upon which the header or footer's 
formatting is based are not updated by this function, so you may also need 
to call SetParaFormat() and SetCharFormat(), specifying the paragraph and 
character format layers referenced by aText.

Ownership of aText is transferred to this object.

Any existing rich text object is replaced, but is not deleted by this function.

@param aText The rich text object to set as the header or footer's rich text 
object. */
	{
	iText = aText;
	}

EXPORT_C void CHeaderFooter::SetParaFormat(CParaFormatLayer* aParaFormat)
/** Sets the format layer upon which the header or footer's paragraph formatting 
is based.

Ownership of aParaFormat is transferred to this object.

Any existing paragraph format layer is replaced, but not deleted by this function.

@param aParaFormat The format layer upon which the header or footer's paragraph 
formatting is based. */
	{
	iParaLayer = aParaFormat;
	}

EXPORT_C void CHeaderFooter::SetCharFormat(CCharFormatLayer* aCharFormat)
/** Sets the format layer upon which the header or footer's character formatting 
is based.

Ownership of aCharFormat is transferred to this object.

Any existing character format layer is replaced, but not deleted by this function.

@param aCharFormat The format layer upon which the header or footer's character 
formatting is based. */
	{
	iCharLayer = aCharFormat;
	}


//
// TPrintParameters
//

EXPORT_C TPrintParameters::TPrintParameters()
	// C'tor - init variables
	: iNumCopies(1),
		iFirstPage(0),
		iLastPage(0)
/** Trivial default C++ constructor.

The number of copies is initialised to one, and the number of the first and 
last pages to zero. */
	{}


//
// TPageMargins
//

EXPORT_C void TPageMargins::InternalizeL(RReadStream& aStream)
/** Internalises a TPageMargins object from a read stream.

The presence of this function means that the standard templated operator>>() 
(defined in s32strm.h) is available to internalise objects of this class.

@param aStream Stream from which the object should be internalised. */
	{
	iHeaderOffset = aStream.ReadInt32L();
	iFooterOffset = aStream.ReadInt32L();
	iMargins.InternalizeL(aStream);
	}

EXPORT_C void TPageMargins::ExternalizeL(RWriteStream& aStream)const
/** Externalises a TPageMargins object to a write stream.

The presence of this function means that the standard templated operator<<() 
(defined in s32strm.h) is available to externalise objects of this class.

@param aStream Stream to which the object should be externalised. */
	{
	aStream.WriteInt32L(iHeaderOffset);
	aStream.WriteInt32L(iFooterOffset);
	iMargins.ExternalizeL(aStream);
	}


//
// TStoreResolver
//

TStoreResolver::TStoreResolver(const CStreamStore& aStore)
:iStore(aStore)
	{}


const CStreamStore& TStoreResolver::StreamStoreL(TInt /*aPos*/)const
	{ return iStore; }
