// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include "CHtmlToCrtConvHashTable.h"
#include "CHtmlToCrtConvHash.h"
#include "CHtmlToCrtConvLookupTable.h"
#include "CHtmlToCrtConvAssertDefines.h"

const TInt KTagHashTableNumberOfEntries = sizeof(KTagHashTable)/sizeof(TTagHashTableEntry);
const TInt KAttributeHashTableNumberOfEntries = sizeof(KAttributeHashTable)/sizeof(TAttributeHashTableEntry);

CHtmlToCrtConvHashTable::CHtmlToCrtConvHashTable()
	{
	}

CHtmlToCrtConvHashTable* CHtmlToCrtConvHashTable::NewL()
	{
	CHtmlToCrtConvHashTable* self=new(ELeave) CHtmlToCrtConvHashTable();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CHtmlToCrtConvHashTable::ConstructL()
	{
	CreateTagHashTableL();
	CreateAttributeHashTableL();
	}

CHtmlToCrtConvHashTable::~CHtmlToCrtConvHashTable()
	{
	iTagHashValues.Close();
	iAttributeHashValues.Close();
	}

void CHtmlToCrtConvHashTable::CreateTagHashTableL()
	{
	const TTagHashTableEntry* item=&KTagHashTable[0];
	TPtrC16 text;

	for(TInt ii=0; ii<KTagHashTableNumberOfEntries; ii++)
		{
		text.Set(item->iText);
		iTagHashValues.Append(Hash(text));
		item++;
		}
	iTagHashValues.Compress();
	}

void CHtmlToCrtConvHashTable::CreateAttributeHashTableL()
	{
	const TAttributeHashTableEntry* item=&KAttributeHashTable[0];
	TPtrC16 text;

	for(TInt ii=0; ii<KAttributeHashTableNumberOfEntries; ii++)
		{
		text.Set(item->iText);
		iAttributeHashValues.Append(Hash(text));
		item++;
		}
	iAttributeHashValues.Compress();
	}

THtmlToCrtConvTagType CHtmlToCrtConvHashTable::LookupTag(const TDesC16& aTag) const
	{
	TInt hashValue=Hash(aTag);
	TInt length=aTag.Length();
	const TInt count=iTagHashValues.Count();

	for(TInt ii=0; ii<count; ii++)
		{
		const TTagHashTableEntry* item=&KTagHashTable[ii];

		if ((iTagHashValues)[ii]==hashValue)
			{
			TPtrC16 text=TPtrC16(item->iText);
			if (text.Length()==length)
				{
				if (aTag.CompareF(text)==0)
					{
					return item->iTagType;
					}
				}
			}	
		}
	return EHtmlTagUnknown;
	}

const TDesC16& CHtmlToCrtConvHashTable::LookupEntity(const TDesC16& aEntityName) const
	{
	TChar firstChar(aEntityName.Ptr()[0]);
	firstChar=firstChar.GetUpperCase();
	switch (firstChar)
		{
	case 'L':
		if (!aEntityName.CompareF(KHtmlLt))
			return KHtmlEntityLt;
		break;
	case 'G':
		if (!aEntityName.CompareF(KHtmlGt))
			return KHtmlEntityGt;
		break;
	case 'N':
		if (!aEntityName.CompareF(KHtmlNbsp))
			return KHtmlEntitySpace;
		break;
		}

	return KHtmlEntityUnknown;
	}

THtmlToCrtConvAttributeType CHtmlToCrtConvHashTable::LookupAttribute(const TDesC16& aAttribute) const
	{
	TInt hashValue=Hash(aAttribute);
	TInt length=aAttribute.Length();
	const TInt count=iAttributeHashValues.Count();

	for(TInt ii=0; ii<count; ii++)
		{
		const TAttributeHashTableEntry* item=&KAttributeHashTable[ii];

		if ((iAttributeHashValues)[ii]==hashValue)
			{
			TPtrC16 text=TPtrC16(item->iText);
			if (text.Length()==length)
				{
				if (aAttribute.CompareF(text)==0)
					{
					return item->iAttributeType;
					}
				}
			}	
		}
	return EHtmlAttributeUnknown;
	}
		
TInt CHtmlToCrtConvHashTable::Hash(const TDesC16& aText) const
	{
	TInt hashValue=0;
	const TInt length=aText.Length();

	for(TInt ii=0; ii<length; ii++)
		{
		TCharUC temp(aText[ii]);
		hashValue += temp;
		}
	return hashValue;
	}
