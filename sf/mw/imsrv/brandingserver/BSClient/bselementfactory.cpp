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
* Description:  bselementfactory.cpp
*
*/



//  INCLUDE FILES

#include "DebugTrace.h"
#include "bselementfactory.h"
#include "cbselement.h"



EXPORT_C MBSElement* BSElementFactory::CreateBSElementL( const TDesC8& aElementId,
									 TBSElementType /*aElementType*/,
									 TInt aElementData )
	{
	return CBSElement::NewL( aElementId, aElementData );
	}


EXPORT_C MBSElement* BSElementFactory::CreateBSElementL( const TDesC8& aElementId,
									 TBSElementType aElementType,
									 const TDesC& aElementData )
	{
	return CBSElement::NewL( aElementId, aElementData, aElementType );
	}


EXPORT_C MBSElement* BSElementFactory::CreateBSElementL( const TDesC8& aElementId,
									 TBSElementType /*aElementType*/,
									 const TDesC8& aElementData )
	{
	return CBSElement::NewL( aElementId, aElementData );
	}


EXPORT_C MBSElement* BSElementFactory::CreateBSElementL( const TDesC8& aElementId,
									 TBSElementType /*aElementType*/,
									 CBSBitmap* aElementData )
	{
	return CBSElement::NewL( aElementId, aElementData );
	}



EXPORT_C MBSElement* BSElementFactory::CreateBSElementL( const TDesC8& aElementId,
									 TBSElementType /*aElementType*/,
									 RBSObjOwningPtrArray<MBSElement>& aElementData )
	{
	return CBSElement::NewL( aElementId, aElementData );
	}




//  END OF FILE

