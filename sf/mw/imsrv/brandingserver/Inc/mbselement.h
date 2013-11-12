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
* Description:  MBSElement.h
*
*/



#ifndef __MBSELEMENT_H
#define __MBSELEMENT_H

#include <e32std.h>
#include <rbsobjowningptrarray.h>

enum TBSElementType
	{
	EBSInt,
	EBSText,
	EBSList,
	EBSBuffer,
	EBSBitmap,
	EBSFile
	};

/**
 * Class encapsulating the structure type branding items
 *
 * @since
 */

class RWriteStream;
class CBSBitmap;

NONSHARABLE_CLASS( MBSElement )
    {

    public:

    	/*
    	* Element type
    	* @return the element type
    	*/
		virtual TBSElementType ElementType() = 0;

    	/*
    	* Element id
    	* @return the element id
    	*/
		virtual const TDesC8& ElementId() = 0;

    	/*
    	* Integer data
    	* Panics if trying to get wrong type data.
    	* @return the integer data of the item
    	*/
		virtual TInt IntDataL() = 0;

    	/*
    	* Text data
    	* Panics if trying to get wrong type data.
    	* @return the text data of the item
    	*/
		virtual const TDesC& TextDataL() = 0;

    	/*
    	* 8-bit buffer data
    	* Panics if trying to get wrong type data.
    	* @return the text data of the item
    	*/
		virtual const TDesC8& BufferDataL() = 0;

    	/*
    	* Bitmap data
    	* Panics if trying to get wrong type data.
    	* @return the text data of the item
    	*/
        virtual const CBSBitmap& BitmapDataL() = 0;

    	/*
    	* Structure data
    	* Panics if trying to get wrong type data.
    	* @return the list data
    	*/
    	virtual TArray<MBSElement*> GetStructureL() = 0;

		/*
		* Finds the first entry from the list of a list type element with
		* the given element id. Note that if there are several subelements
		* with the same elementId then TBrandingElementIterator should be
		* used to iterate through them.
		* @param aElementId elementId of the subelement to be found.
		* @return the subelement
		*/
		virtual const MBSElement& FindSubEntryL( const TDesC& aElementId ) = 0;

        /*
        * Writes the internal state to stream.
        * @param aWriteStream output stream;
        */
        virtual void ExternalizeL( RWriteStream& aWriteStream ) = 0;

        /*
        * Deletes the element.
        */
		virtual void Close() = 0;

		/*
		* Destructor
		*/
	    virtual ~MBSElement() {};

    };




#endif //__MBSELEMENT_H
