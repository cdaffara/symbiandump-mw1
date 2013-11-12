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
* Description:  CBSUpdater.cpp
*
*/



//  INCLUDE FILES

#include "e32base.h"

#include "cbsupdater.h"
#include "DebugTrace.h"
#include "mbsaccess.h"
#include "bselementfactory.h"
#include "mbselement.h"
#include "bsserverdefs.h"

// Two-phased constructor.
CBSUpdater* CBSUpdater::NewL( const TDesC8& aApplicationId )
    {
    CBSUpdater* self = new ( ELeave ) CBSUpdater() ;
    CleanupStack::PushL( self );
    self->ConstructL( aApplicationId );
    CleanupStack::Pop( self );  //self
    return self;
    }

// Symbian OS default constructor can leave.
void CBSUpdater::ConstructL( const TDesC8& aApplicationId  )
    {
    iApplicationId = aApplicationId.AllocL();
    User::LeaveIfError( iClient.Connect() );
    }

// Destructor
CBSUpdater::~CBSUpdater()
    {
    delete iApplicationId;
    delete iBrandId;
    iClient.Close();
    }

// C++ default constructor can NOT contain any code, that
// might leave.
//
CBSUpdater::CBSUpdater()
	{
	}


// -----------------------------------------------------------------------------
// CBSUpdater::Close()
// -----------------------------------------------------------------------------
//
void CBSUpdater::Close()
	{
	delete this;
	}

// -----------------------------------------------------------------------------
// CBSUpdater::StartTransactionL()
// -----------------------------------------------------------------------------
//
void CBSUpdater::StartTransactionL( const TDesC8& aBrandId,
									TLanguage aLanguageId,
									TUpdateTransactionType aType, /*EUpdateInstall*/
									TInt aReserved)
	{
	if( iActive )
		{
		User::Leave( KErrAlreadyExists );
		}
	
	iActive = ETrue;
	HBufC8* tmp = aBrandId.AllocL();
	delete iBrandId;
	iBrandId = tmp;
	iLanguageId = aLanguageId;
	iReserved = aReserved;
	iTxType = aType;
	
	TTransactionType operation = EBSTxAccess;
	switch( aType )
		{
		case EUpdateInstall:
			{
			operation = EBSTxInstall;
			break;
			}
		case EUpdateAppend:
			{
			operation = EBSTxAppend;
			break;
			}
		case EUpdateReplace:
			{
			operation = EBSTxReplace;
			break;
			}
		case EUpdateUninstall:
			{
			operation = EBSTxUninstall;
			break;
			}
		default:
			break;
		}
	
	TRAPD( err, iClient.StartTransactionL( *iApplicationId, aBrandId, KNullDesC8,
									aLanguageId, operation, aReserved ) );
	if( err )
		{
		iActive = EFalse;
		User::Leave( err );
		}

	}
// -----------------------------------------------------------------------------
// CBSUpdater::StopTransactionL()
// -----------------------------------------------------------------------------
//
TInt CBSUpdater::StopTransactionL()
	{
	if( !iActive )
		{
		User::Leave( KErrNotFound );
		}
	
	TInt returnValue = iClient.StopTransactionL( *iApplicationId, 
												 *iBrandId, 
												 iLanguageId, 
												 iReserved );
	iActive = EFalse;
	return returnValue;
	}


// -----------------------------------------------------------------------------
// CBSUpdater::CancelTransactionL()
// -----------------------------------------------------------------------------
//
void CBSUpdater::CancelTransactionL()
	{
	if( !iActive )
		{
		User::Leave( KErrNotFound );
		}
	
	iClient.CancelTransactionL( *iApplicationId, *iBrandId, iLanguageId, iReserved );
	iActive = EFalse;
	}



//*** BRAND INSTALLING ***//
// -----------------------------------------------------------------------------
// CBSUpdater::InsertTextL()
// -----------------------------------------------------------------------------
//
void CBSUpdater::InsertTextL( const TDesC8& aId,
				  			  const TDesC& aText )
	{
	if( !iActive )
		{
		User::Leave( KErrNotReady );
		}
	if( iTxType != EUpdateInstall )
		{
		User::Leave( KErrArgument );
		}
	MBSElement* element = BSElementFactory::CreateBSElementL( aId, EBSText, aText );
	CleanupClosePushL( *element );
	InsertElementL( element );
	CleanupStack::PopAndDestroy(); // element
	}


// -----------------------------------------------------------------------------
// CBSUpdater::InsertBufferL()
// -----------------------------------------------------------------------------
//
void CBSUpdater::InsertBufferL( const TDesC8& aId,
								const TDesC8& aBuffer )
	{
	if( !iActive )
		{
		User::Leave( KErrNotReady );
		}
	if( iTxType != EUpdateInstall )
		{
		User::Leave( KErrArgument );
		}
	MBSElement* element = BSElementFactory::CreateBSElementL( aId, EBSBuffer, aBuffer );
	CleanupClosePushL( *element );
	InsertElementL( element );
	CleanupStack::PopAndDestroy(); // element
	}


// -----------------------------------------------------------------------------
// CBSUpdater::InsertIntL()
// -----------------------------------------------------------------------------
//
void CBSUpdater::InsertIntL( const TDesC8& aId,
				 			 TInt aInt )
	{
	if( !iActive )
		{
		User::Leave( KErrNotReady );
		}
	if( iTxType != EUpdateInstall )
		{
		User::Leave( KErrArgument );
		}
	MBSElement* element = BSElementFactory::CreateBSElementL( aId, EBSInt, aInt );
	CleanupClosePushL( *element );
	InsertElementL( element );
	CleanupStack::PopAndDestroy(); // element
	}


// -----------------------------------------------------------------------------
// CBSUpdater::InsertFileL()
// -----------------------------------------------------------------------------
//
void CBSUpdater::InsertFileL( const TDesC8& aId,
				  			  const TDesC& aFileName )
	{
	if( !iActive )
		{
		User::Leave( KErrNotReady );
		}
	if( iTxType != EUpdateInstall )
		{
		User::Leave( KErrArgument );
		}
	MBSElement* element = BSElementFactory::CreateBSElementL( aId, EBSFile, aFileName );
	CleanupClosePushL( *element );
	InsertElementL( element );
	CleanupStack::PopAndDestroy(); // element
	}


// -----------------------------------------------------------------------------
// CBSUpdater::InsertElementL()
// -----------------------------------------------------------------------------
//
void CBSUpdater::InsertElementL( MBSElement* aElement )
	{
	if( !iActive )
		{
		User::Leave( KErrNotReady );
		}
	if( iTxType != EUpdateInstall )
		{
		User::Leave( KErrArgument );
		}
	iClient.InsertL( aElement );
	}




//*** BRAND UPDATING - replacing ***//
// -----------------------------------------------------------------------------
// CBSUpdater::ReplaceTextL()
// -----------------------------------------------------------------------------
//
void CBSUpdater::ReplaceTextL( const TDesC8& aId,
				   			   const TDesC& aText )
	{
	if( !iActive )
		{
		User::Leave( KErrNotReady );
		}
	if( iTxType != EUpdateReplace )
		{
		User::Leave( KErrArgument );
		}
	MBSElement* element = BSElementFactory::CreateBSElementL( aId, EBSText, aText );
	CleanupClosePushL( *element );
	ReplaceElementL( element );
	CleanupStack::PopAndDestroy(); // element
	}


// -----------------------------------------------------------------------------
// CBSUpdater::ReplaceBufferL()
// -----------------------------------------------------------------------------
//
void CBSUpdater::ReplaceBufferL( const TDesC8& aId,
					 			 const TDesC8& aBuffer )
	{
	if( !iActive )
		{
		User::Leave( KErrNotReady );
		}
	if( iTxType != EUpdateReplace )
		{
		User::Leave( KErrArgument );
		}
	MBSElement* element = BSElementFactory::CreateBSElementL( aId, EBSBuffer, aBuffer );
	CleanupClosePushL( *element );
	ReplaceElementL( element );
	CleanupStack::PopAndDestroy(); // element
	}


// -----------------------------------------------------------------------------
// CBSUpdater::ReplaceIntL()
// -----------------------------------------------------------------------------
//
void CBSUpdater::ReplaceIntL( const TDesC8& aId,
				  			  TInt aInt )
	{
	if( !iActive )
		{
		User::Leave( KErrNotReady );
		}
	if( iTxType != EUpdateReplace )
		{
		User::Leave( KErrArgument );
		}
	MBSElement* element = BSElementFactory::CreateBSElementL( aId, EBSInt, aInt );
	CleanupClosePushL( *element );
	ReplaceElementL( element );
	CleanupStack::PopAndDestroy(); // element
	}


// -----------------------------------------------------------------------------
// CBSUpdater::ReplaceFileL()
// -----------------------------------------------------------------------------
//
void CBSUpdater::ReplaceFileL( const TDesC8& aId,
				   			   const TDesC& aFileName )
	{
	if( !iActive )
		{
		User::Leave( KErrNotReady );
		}
	if( iTxType != EUpdateReplace )
		{
		User::Leave( KErrArgument );
		}
	MBSElement* element = BSElementFactory::CreateBSElementL( aId, EBSFile, aFileName );
	CleanupClosePushL( *element );
	ReplaceElementL( element );
	CleanupStack::PopAndDestroy(); // element
	}


// -----------------------------------------------------------------------------
// CBSUpdater::ReplaceElementL()
// -----------------------------------------------------------------------------
//
void CBSUpdater::ReplaceElementL( MBSElement* aElement )
	{
	if( !iActive )
		{
		User::Leave( KErrNotReady );
		}
	if( iTxType != EUpdateReplace )
		{
		User::Leave( KErrArgument );
		}
	iClient.ReplaceL( aElement );
	}




//*** BRAND UPDATING - appending ***//
// -----------------------------------------------------------------------------
// CBSUpdater::AppendTextL()
// -----------------------------------------------------------------------------
//
void CBSUpdater::AppendTextL( const TDesC8& aId,
				  			  const TDesC& aText )
	{
	if( !iActive )
		{
		User::Leave( KErrNotReady );
		}
	if( iTxType != EUpdateAppend )
		{
		User::Leave( KErrArgument );
		}
	MBSElement* element = BSElementFactory::CreateBSElementL( aId, EBSText, aText );
	CleanupClosePushL( *element );
	AppendElementL( element );
	CleanupStack::PopAndDestroy(); // element
	}


// -----------------------------------------------------------------------------
// CBSUpdater::AppendBufferL()
// -----------------------------------------------------------------------------
//
void CBSUpdater::AppendBufferL( const TDesC8& aId,
								const TDesC8& aBuffer )
	{
	if( !iActive )
		{
		User::Leave( KErrNotReady );
		}
	if( iTxType != EUpdateAppend )
		{
		User::Leave( KErrArgument );
		}
	MBSElement* element = BSElementFactory::CreateBSElementL( aId, EBSBuffer, aBuffer );
	CleanupClosePushL( *element );
	AppendElementL( element );
	CleanupStack::PopAndDestroy(); // element
	}


// -----------------------------------------------------------------------------
// CBSUpdater::AppendIntL()
// -----------------------------------------------------------------------------
//
void CBSUpdater::AppendIntL( const TDesC8& aId,
				 		     TInt aInt )
	{
	if( !iActive )
		{
		User::Leave( KErrNotReady );
		}
	if( iTxType != EUpdateAppend )
		{
		User::Leave( KErrArgument );
		}
	MBSElement* element = BSElementFactory::CreateBSElementL( aId, EBSInt, aInt );
	CleanupClosePushL( *element );
	AppendElementL( element );
	CleanupStack::PopAndDestroy(); // element
	}


// -----------------------------------------------------------------------------
// CBSUpdater::AppendFileL()
// -----------------------------------------------------------------------------
//
void CBSUpdater::AppendFileL( const TDesC8& aId,
				  			  const TDesC& aFileName )
	{
	if( !iActive )
		{
		User::Leave( KErrNotReady );
		}
	if( iTxType != EUpdateAppend )
		{
		User::Leave( KErrArgument );
		}
	MBSElement* element = BSElementFactory::CreateBSElementL( aId, EBSFile, aFileName );
	CleanupClosePushL( *element );
	AppendElementL( element );
	CleanupStack::PopAndDestroy(); // element
	}


// -----------------------------------------------------------------------------
// CBSUpdater::AppendElementL()
// -----------------------------------------------------------------------------
//
void CBSUpdater::AppendElementL( MBSElement* aElement )
	{
	if( !iActive )
		{
		User::Leave( KErrNotReady );
		}
	if( iTxType != EUpdateAppend )
		{
		User::Leave( KErrArgument );
		}
	iClient.AppendL( aElement );
	}


// -----------------------------------------------------------------------------
// CBSUpdater::AppendElementL()
// -----------------------------------------------------------------------------
//
void CBSUpdater::RemoveBrandL( const TDesC8& aApplicationId,
								   const TDesC8& aBrandId )
	{
	if( !iActive )
		{
		User::Leave( KErrNotReady );
		}
	iClient.RemoveBrandL( aApplicationId, aBrandId );
	}

// -----------------------------------------------------------------------------
// CBSUpdater::AppendElementL()
// -----------------------------------------------------------------------------
//
void CBSUpdater:: RemoveBrandsL( const TDesC8& aApplicationId )
	{
	if( !iActive )
		{
		User::Leave( KErrNotReady );
		}
	iClient.RemoveBrandsL( aApplicationId );
	}

// -----------------------------------------------------------------------------
// CBSUpdater::RegisterObserverL()
// -----------------------------------------------------------------------------
//
void CBSUpdater:: RegisterObserverL( MBSBackupRestoreStateObserver* aBackupObserver )
{
	iClient.RegisterObserverL(NULL, aBackupObserver) ;
}

// -----------------------------------------------------------------------------
// CBSUpdater::UnRegisterObserverL()
// -----------------------------------------------------------------------------
//
void CBSUpdater:: UnRegisterObserverL( MBSBackupRestoreStateObserver* aObserver )
{
	iClient.UnRegisterObserverL(NULL, aObserver) ;
}

//  END OF FILE

