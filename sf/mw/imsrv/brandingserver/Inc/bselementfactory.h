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
* Description:  bselementfactory.h 
*
*/



#ifndef __BSELEMENTFACTORY_H
#define __BSELEMENTFACTORY_H

#include <e32std.h>
#include <mbselement.h>


class CBSElement;


/**
 * Factory for creating branding elements
 *
 * @since
 */
class BSElementFactory
    {
    public:

    	/*
    	* Create integer type element.
    	* @param aElementId id of the element
    	* @param aElementType type of the element
    	* @param aElementData the element data
    	*/
		IMPORT_C static MBSElement* CreateBSElementL( const TDesC8& aElementId,
											 TBSElementType aElementType,
											 TInt aElementData );

    	/*
    	* Create text or file type element.
    	* @param aElementId id of the element
    	* @param aElementType type of the element
    	* @param aElementData the element data, if the element is file type
    	*		 then this should contain the file name
    	*/
		IMPORT_C static MBSElement* CreateBSElementL( const TDesC8& aElementId,
											 TBSElementType aElementType,
											 const TDesC& aElementData );

    	/*
    	* Create buffer type element.
    	* @param aElementId id of the element
    	* @param aElementType type of the element
    	* @param aElementData the element data
    	*/
		IMPORT_C static MBSElement* CreateBSElementL( const TDesC8& aElementId,
											 TBSElementType aElementType,
											 const TDesC8& aElementData );

    	/*
    	* Create bitmap type element.
    	* @param aElementId id of the element
    	* @param aElementType type of the element
    	* @param aElementData the element data
    	*/
		IMPORT_C static MBSElement* CreateBSElementL( const TDesC8& aElementId,
											 TBSElementType aElementType,
											 CBSBitmap* aElementData );

    	/*
    	* Create an element containing a list of other elements.
    	* @param aElementId id of the element
    	* @param aElementType type of the element
    	* @param aElementData the element data
    	*/
		IMPORT_C static MBSElement* CreateBSElementL( const TDesC8& aElementId,
											 TBSElementType aElementType,
											 RBSObjOwningPtrArray<MBSElement>& aElementData );

    };

#endif //__BSELEMENTFACTORY_H
