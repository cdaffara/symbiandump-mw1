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
* Description:  CBSAccess.cpp
*
*/



//  INCLUDE FILES

#include <e32base.h>
#include <AknsSkinInstance.h>	// skins 
#include <AknsUtils.h>
#include <coemain.h>

#include "cbsaccess.h"
#include "cbsbitmap.h"
#include "DebugTrace.h"
#include "mbsaccess.h"
#include "mbsupdater.h"
#include "cbsiconfileprovider.h"

void Panic(TInt aPanic)
    {
    _LIT( KPanic, "CBS" );
    User::Panic( KPanic, aPanic );
    }

// Two-phased constructor.
CBSAccess* CBSAccess::NewL( const TDesC8& aBrandId,
							const TDesC8& aAppId,
    					 	const TDesC8& aDefaultBrandId,
							TLanguage aLanguageId,
    					  	TBool aCacheData,
							TInt aReserved )
    {
    CBSAccess* self = new ( ELeave ) CBSAccess( aLanguageId, aCacheData, aReserved) ;
    CleanupStack::PushL( self );
    self->ConstructL( aBrandId, aAppId, aDefaultBrandId );
    CleanupStack::Pop( self );  //self
    return self;
    }

// Symbian OS default constructor can leave.
void CBSAccess::ConstructL( const TDesC8& aBrandId, const TDesC8& aAppId, 
    					 	const TDesC8& aDefaultBrandId )
    {
    iBrand = aBrandId.AllocL();
    iDefaultBrand = aDefaultBrandId.AllocL();
    iAppId = aAppId.AllocL();
    User::LeaveIfError( iClient.Connect() );
    
	iClient.StartTransactionL( *iAppId, *iBrand, aDefaultBrandId,
								iLanguageId, EBSTxAccess, iReserved );
    }

// Destructor
CBSAccess::~CBSAccess()
    {
    delete iBrand;
    delete iDefaultBrand;
    delete iAppId;
    iClient.Close();
    }

// C++ default constructor can NOT contain any code, that
// might leave.
//
CBSAccess::CBSAccess( TLanguage aLanguageId, TBool aCacheData, TInt aReserved )
: iLanguageId( aLanguageId ), iUseCache( aCacheData ), iReserved( aReserved )
	{
	}

// -----------------------------------------------------------------------------
// CBSAccess::Close()
// -----------------------------------------------------------------------------
//
void CBSAccess::Close()
	{
	delete this;
	}

// -----------------------------------------------------------------------------
// CBSAccess::GetTextL()
// -----------------------------------------------------------------------------
//
HBufC* CBSAccess::GetTextL( const TDesC8& aId )
	{
	HBufC* buffer = NULL;

	iClient.GetTextL( aId, buffer );
		
    TRACE( T_LIT("iClient.GetText( iBrandId, aId, buffer )") );
    return buffer;
	}

// -----------------------------------------------------------------------------
// CBSAccess::GetTextLC()
// -----------------------------------------------------------------------------
//
HBufC* CBSAccess::GetTextLC( const TDesC8& aId )
	{
	HBufC* buffer = NULL;
    iClient.GetTextL( aId, buffer );
    TRACE( T_LIT("iClient.GetText( aBrandId, aId, buffer )") );

    CleanupStack::PushL( buffer );
    return buffer;
	}


// -----------------------------------------------------------------------------
// CBSAccess::GetIntL()
// -----------------------------------------------------------------------------
//
TInt CBSAccess::GetIntL( const TDesC8& aId )
	{
	TInt value = 0;

	iClient.GetIntL( aId, value );
    return value;
	}

TInt CBSAccess :: isBrandUpdateRequiredL ()
{
	TInt updateRequired = -1; 
	iClient.isBrandUpdateRequiredL (updateRequired);
 	return updateRequired;

}

// -----------------------------------------------------------------------------
// CBSAccess::GetSeveralIntL()
// -----------------------------------------------------------------------------
//
MBSElement* CBSAccess::GetSeveralL( MDesC8Array& aIds )
	{
	MBSElement* returnValue = iClient.GetSeveralL( aIds );
	return returnValue;
	}

// -----------------------------------------------------------------------------
// CBSAccess::GetFileL()
// -----------------------------------------------------------------------------
//
void CBSAccess::GetFileL( const TDesC8& aId,
					   	  RFile& aFile )
	{
	iClient.GetFileL( aId, aFile );
	}

// -----------------------------------------------------------------------------
// CBSAccess::GetStructureL()
// -----------------------------------------------------------------------------
//
MBSElement* CBSAccess::GetStructureL( const TDesC8& aId )
	{
	MBSElement* returnValue = NULL;

	returnValue = iClient.GetStructureL( aId );
	
	return returnValue;
	
	}

// -----------------------------------------------------------------------------
// CBSAccess::GetBufferL()
// -----------------------------------------------------------------------------
//
HBufC8* CBSAccess::GetBufferL( const TDesC8& aId )
	{
	HBufC8* buffer = NULL;
	iClient.GetBufferL( aId, buffer );
	return buffer;
	}

// -----------------------------------------------------------------------------
// CBSAccess::GetBufferLC()
// -----------------------------------------------------------------------------
//
HBufC8* CBSAccess::GetBufferLC( const TDesC8& aId )
	{
	HBufC8* buffer = GetBufferL( aId );
	CleanupStack::PushL( buffer );
	return buffer;
	}

// -----------------------------------------------------------------------------
// CBSAccess::RegisterObserverL()
// -----------------------------------------------------------------------------
//
void CBSAccess::RegisterObserverL( MBSBrandChangeObserver* aObserver, MBSBackupRestoreStateObserver* aBackupObserver)
	{
	iClient.RegisterObserverL( aObserver, aBackupObserver );
	}

// -----------------------------------------------------------------------------
// CBSAccess::UnRegisterObserverL()
// -----------------------------------------------------------------------------
//
void CBSAccess::UnRegisterObserverL( MBSBrandChangeObserver* aObserver, MBSBackupRestoreStateObserver* aBackupObserver )
	{
	iClient.UnRegisterObserverL( aObserver, aBackupObserver );
	}

// -----------------------------------------------------------------------------
// CBSAccess::GetBitmapL()
// -----------------------------------------------------------------------------
//
void CBSAccess::GetBitmapL( const TDesC8& aBitmapId,
				 CFbsBitmap*& aBitmap,
				 CFbsBitmap*& aMask )
	{
	TBool useSkins = ETrue;
	CCoeEnv* coeEnv = CCoeEnv::Static();
	if( !coeEnv )
		{
		TRACE( T_LIT("GETBITMAPL NO COE ENV ") );
		// no CONE -> don't try to use skins
		useSkins = EFalse;
		}
	
	MBSElement* bitmapItem = NULL;

	// get the structure containing the info for the bitmap
	bitmapItem = GetStructureL( aBitmapId );
	CleanupClosePushL( *bitmapItem );

	// get the list from the bitmap element
	const CBSBitmap* bitmap = &bitmapItem->BitmapDataL();
	
	RFile bitmapFile;
	
	// we need to get the bitmap file from where we can load the bitmap
	GetFileL( bitmap->BitmapFileId(), bitmapFile );
	
	TInt err = KErrNotFound;
	CFbsBitmap* skinBitmap = NULL;
	if( useSkins )
		{
		MAknsSkinInstance* skin = AknsUtils::SkinInstance();	

		TAknsItemID itemId;
		itemId.Set( bitmap->SkinIdMajor(), bitmap->SkinIdMinor() );

	    // Codescanner warning: neglected to put variable on cleanup stack (id:35)
	    // This method cannot leave after this line if the bitmap is created
	    TRAP( err, skinBitmap = AknsUtils::CreateBitmapL( skin, itemId ) ); // CSI: 35 # See above	
		}
    // if useSkins is TRUE that means there is a COE Env, and icons can be created
	// otherwise, without CoeEnv, we cannot create icons, hence leave 
    if( err == KErrNotFound && useSkins)
    	{
		// no skin bitmap found
		
	    // icon server take ownership of of icon file provider
	    CBSIconFileProvider* ifp = CBSIconFileProvider::NewL( bitmapFile );
	    CleanupStack::PushL( ifp );
	
	    AknIconUtils::CreateIconLC( aBitmap, aMask,
                					*ifp,
                					bitmap->BitmapId(), bitmap->BitmapMaskId() ); 
                					  
		CleanupStack::Pop( 2 ); // aBitmap, aMask
		CleanupStack::Pop(ifp) ; 
    	}
    else if( err )
    	{
    	User::Leave( err );
    	}
	else
		{
		aBitmap = skinBitmap;
		aMask = NULL;
		}
	bitmapFile.Close() ;
	CleanupStack::PopAndDestroy(); // structure
	}

//  END OF FILE

