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
* Description:  cbsbitmap.h
*
*/




#ifndef __CBSBITMAP_H__
#define __CBSBITMAP_H__

#include <e32std.h>

/**
 *
 */
NONSHARABLE_CLASS( CBSBitmap ): public CBase
    {
	public:
		/**
		* Two-phased constructor.
		* @param aBitmapId the bitmap id
		* @param aBitmapMaskId the bitmap mask id
		* @param aSkinId the skin id
		* @param aSkinMaskId the skin mask id
		* @param aBitmapFileId the bitmap file id
		*/
		IMPORT_C static CBSBitmap* NewL( TInt aBitmapId,
								TInt aBitmapMaskId,
								TInt aSkinIdMajor,
								TInt aSkinIdMinor,
								const TDesC8& aBitmapFileId );

		/**
		* Two-phased constructor.
		* @see NewL
		*/
		IMPORT_C static CBSBitmap* NewLC( TInt aBitmapId,
								 TInt aBitmapMaskId,
								 TInt aSkinIdMajor,
								 TInt aSkinIdMinor,
								 const TDesC8& aBitmapFileId );

		// destructor
    	IMPORT_C virtual ~CBSBitmap();

	private:
		/**
		* @see NewL
		*/
		CBSBitmap( TInt aBitmapId,
				   TInt aBitmapMaskId,
				   TInt aSkinIdMajor,
				   TInt aSkinIdMinor );

		/**
		* @see NewL
		*/
    	void ConstructL( const TDesC8& aBitmapFileId );

	public:
		/**
		* Getter for bitmap id
		* @return the bitmap id
		*/
		IMPORT_C TInt BitmapId() const;

		/**
		* Getter for bitmap mask id
		* @return the bitmap mask id
		*/
		IMPORT_C TInt BitmapMaskId() const;

		/**
		* Getter for skin id
		* @return the skin id
		*/
		IMPORT_C TInt SkinIdMajor() const;

		/**
		* Getter for skin mask id
		* @return the skin mask id
		*/
		IMPORT_C TInt SkinIdMinor() const;

		/**
		* Getter for bitmap file id
		* @return bitmap file id
		*/
		IMPORT_C const TDesC8& BitmapFileId() const;


	private: // data
		TInt	iBitmapId;
	    TInt	iBitmapMaskId;
	    TInt	iSkinIdMajor;
	    TInt	iSkinIdMinor;
	    HBufC8*	iBitmapFileId;
    };

#endif      //  __CBSBITMAP_H__


//  END OF FILE

