/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  cbselement.h
*
*/



#ifndef __CBSELEMENT_H__
#define __CBSELEMENT_H__
#include <e32std.h>

#include "mbselement.h"
#include "rbsobjowningptrarray.h"

class RWriteStream;

/**
 *
 */
NONSHARABLE_CLASS( CBSElement ): public CBase,
								 public MBSElement
    {
	public:
		/**
		* Two-phased constructor.
		*/
		static CBSElement* NewL(
						const TDesC8& aElementId,
						TInt aData );

		static CBSElement* NewL(
						const TDesC8& aElementId,
						const TDesC& aData,
						TBSElementType aType = EBSText );

		static CBSElement* NewL(
						const TDesC8& aElementId,
						const TDesC8& aData );

		// Transfers ownership
		static CBSElement* NewL(
						const TDesC8& aElementId,
						CBSBitmap* aBitmap );

		static CBSElement* NewL(
						const TDesC8& aElementId,
						RBSObjOwningPtrArray<MBSElement>& aStructure );

		void Close();

		virtual ~CBSElement();

	private:
		CBSElement();


    	void ConstructL( TBSElementType aElementType,
					 	 const TDesC8& aElementId,
					 	 TInt aIntData,
					 	 TBool aIntDataInitialized,
					 	 CBSBitmap* aBitmap,
					 	 const TDesC& aTextData,
					 	 const TDesC8& aBufferData,
					 	 RBSObjOwningPtrArray<MBSElement>* aStructure );

	private:
		TBSElementType ElementType();
		const TDesC8& ElementId();

		// Leaves with KErrArgument if trying to
		// get wrong type
		TInt IntDataL();
		const TDesC& TextDataL();
		const TDesC8& BufferDataL();
        const CBSBitmap& BitmapDataL();
		TArray<MBSElement*> GetStructureL();

		const MBSElement& FindSubEntryL( const TDesC& aElementId );

        void ExternalizeL( RWriteStream& aWriteStream );



	private: // data
		TBSElementType 			iElementType;
		HBufC8*					iElementId;
		TInt					iIntData;
		TBool					iIntDataInitialized;
		HBufC*					iTextData;
		HBufC8*					iBufferData;
		TBool					iArrayInitialized;
		RBSObjOwningPtrArray<MBSElement>	iArray;
		CBSBitmap*         		iBitmapData;

    };

#endif      //  __CBSELEMENT_H__


//  END OF FILE

