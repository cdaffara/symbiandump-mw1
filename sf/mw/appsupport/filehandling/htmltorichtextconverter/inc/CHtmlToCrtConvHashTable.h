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

#ifndef __CHTMLTOCRTCONVHASHTABLE_H__
#define __CHTMLTOCRTCONVHASHTABLE_H__

#include <e32std.h>
#include <e32base.h>
#include "CHtmlToCrtConvHash.h"
#include "CHtmlToCrtConvLookupTable.h"

class CHtmlToCrtConvHashTable : public CBase
	{
	public:
		static CHtmlToCrtConvHashTable* NewL();
		~CHtmlToCrtConvHashTable();
/**
	@fn				THtmlToCrtConvTagType LookupTag(const TDesC8& aTag) const
	Intended Usage	: Searches for aTag in KTagHashTable[] and returns
					  corresponding THtmlToCrtConvTagType<br>
	@param			aTag The text to be searched for
	@return			THtmlToCrtConvTagType The corresponding type
					 - EHtmlTagUnknown if not found
 */
		THtmlToCrtConvTagType LookupTag(const TDesC16& aTag) const;
/**
	@fn				THtmlToCrtConvAttributeType LookupAttribute(const TDesC8& aAttribute) const
	Intended Usage	: Searches for aAttribute in KAttributeHashTable[] and returns
					  corresponding THtmlToCrtConvAttributeType<br>
	@param			aAttribute The text to be searched for
	@return			THtmlToCrtConvAttributeType The corresponding type
					 - EHtmlAttributeUnknown if not found
 */
		THtmlToCrtConvAttributeType LookupAttribute(const TDesC16& aAttribute) const;

/**
	@fn				const TDesC16&	LookupEntity(const TDesC16& aEntityName) const
	Intended Usage	: Searches for aEntityName in KEntityHashTable[] and returns the 
					  corresponding entity
	@param			aEntityName The entity name to be searched for
	@return			const TDesC16& The corresponding entity
	                - KHtmlEntityUnknown if not found
*/
		const TDesC16&	LookupEntity(const TDesC16& aEntityName) const;
	
	private:
		CHtmlToCrtConvHashTable();
		void ConstructL();
/**
	@fn				void CreateTagHashTableL()
	Intended Usage	: Used by LookupTag()
					  Populates iTagHashValues with values
					  generated from KTagHashTable[]<br>
	@return			void
 */
		void CreateTagHashTableL();
/**
	@fn				void CreateAttributeHashTableL()
	Intended Usage	: Used by LookupAttribute()
					  Populates iAttributeHashValues with values
					  generated from KAttributeHashTable[]<br>
	@return			void
 */
		void CreateAttributeHashTableL();
/**
	@fn				CreateEntityHashTableL()
	Intended Usage	: Populates iEntityHashValues with values
					  generated from KEntityHashTable[]. iEnitityHashValues 
					  is used by LookupEntity()
	@return			void
  */

		void CreateEntityHashTableL();
/**
	@fn				TInt Hash(const TDesC8& aText) const
	Intended Usage	: Used by CreateTagHashTableL(), CreateAttributeHashTableL(),
					  LookupTag() and LookupAttribute()
					  Takes a descriptor, and returns a value determined
					  by the descriptor<br>
	@param			aText the text to be translated into a hash value
	@return			TInt a value determined by aText
 */
		TInt Hash(const TDesC16& aText) const;
	
	private:
		RArray<TInt> iTagHashValues;
		RArray<TInt> iAttributeHashValues;
	};

#endif
