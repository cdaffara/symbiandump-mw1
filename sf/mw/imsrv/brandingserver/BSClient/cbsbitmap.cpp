/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  cbsbitmap.cpp
*
*/



//  INCLUDE FILES

#include "e32base.h"
#include "cbsbitmap.h"

// -----------------------------------------------------------------------------
// CBSBitmap::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CBSBitmap* CBSBitmap::NewLC( TInt aBitmapId,
							TInt aBitmapMaskId,
							TInt aSkinId,
							TInt aSkinMaskId,
							const TDesC8& aBitmapFileId )
    {
	CBSBitmap* self = new ( ELeave ) CBSBitmap( aBitmapId, aBitmapMaskId,
												aSkinId, aSkinMaskId );
    CleanupStack::PushL( self );
    self->ConstructL( aBitmapFileId );
    return self;
    }

// -----------------------------------------------------------------------------
// CBSBitmap::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CBSBitmap* CBSBitmap::NewL( TInt aBitmapId,
							TInt aBitmapMaskId,
							TInt aSkinIdMajor,
							TInt aSkinIdMinor,
							const TDesC8& aBitmapFileId )
    {
    CBSBitmap* self = NewLC( aBitmapId,
    						 aBitmapMaskId,
    						 aSkinIdMajor,
    						 aSkinIdMinor,
    						 aBitmapFileId );
    CleanupStack::Pop();
    return self;
    }


// Symbian OS default constructor can leave.
void CBSBitmap::ConstructL( const TDesC8& aBitmapFileId )
    {
	iBitmapFileId = aBitmapFileId.AllocL();
    }

// destructor
CBSBitmap::~CBSBitmap()
    {
	delete iBitmapFileId;
    }

// C++ default constructor can NOT contain any code, that
// might leave.
//
CBSBitmap::CBSBitmap( TInt aBitmapId,
					  TInt aBitmapMaskId,
					  TInt aSkinIdMajor,
					  TInt aSkinIdMinor )
: iBitmapId( aBitmapId), iBitmapMaskId( aBitmapMaskId ),
  iSkinIdMajor( aSkinIdMajor ), iSkinIdMinor( aSkinIdMinor )
	{
	}


// -----------------------------------------------------------------------------
// CBSBitmap::BitmapId()
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CBSBitmap::BitmapId() const
	{
	return iBitmapId;
	}

// -----------------------------------------------------------------------------
// CBSBitmap::BitmapMaskId()
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CBSBitmap::BitmapMaskId() const
	{
	return iBitmapMaskId;
	}

// -----------------------------------------------------------------------------
// CBSBitmap::SkinId()
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CBSBitmap::SkinIdMajor() const
	{
	return iSkinIdMajor;
	}

// -----------------------------------------------------------------------------
// CBSBitmap::SkinMaskId()
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CBSBitmap::SkinIdMinor() const
	{
	return iSkinIdMinor;
	}

// -----------------------------------------------------------------------------
// CBSBitmap::BitmapFileId()
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CBSBitmap::BitmapFileId() const
	{
	return *iBitmapFileId;
	}



//  END OF FILE
