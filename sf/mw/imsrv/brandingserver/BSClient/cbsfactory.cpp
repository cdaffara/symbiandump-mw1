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
* Description:   Factory for creating branding
*
*/


// INCLUDE FILES
#include    "cbsfactory.h"
#include	"cbsaccess.h"
#include	"cbsupdater.h"
#include	"mbsaccess.h"
#include	"mbsupdater.h"
#include	"cbsclient.h"
#include    "DebugTrace.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// BrandingFactory::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CBSFactory* CBSFactory::NewL( const TDesC8& aDefaultBrandId,
							  const TDesC8& aApplicationId )
	{
    TRACE( T_LIT("CBSFactory::NewL begin") );
    CBSFactory* self = new ( ELeave ) CBSFactory() ;
    CleanupStack::PushL( self );
    self->ConstructL( aDefaultBrandId, aApplicationId );
    CleanupStack::Pop( self );  //self
    TRACE( T_LIT("CBSFactory::NewL end") );
    return self;
	}


// C++ default constructor can NOT contain any code, that
// might leave.
//
CBSFactory::CBSFactory()
	{
	}

// destructor
CBSFactory::~CBSFactory()
	{
	delete iDefaultBrand;
	delete iApplicationId;
	if( iServerKeepAlive )
		{
		iServerKeepAlive->Close();	
		}
	delete iServerKeepAlive;
	}

// Symbian OS default constructor can leave.
void CBSFactory::ConstructL( const TDesC8& aDefaultBrandId,
							 const TDesC8& aApplicationId )
	{
	iDefaultBrand = aDefaultBrandId.AllocL();
	iApplicationId = aApplicationId.AllocL();
	iServerKeepAlive = new(ELeave) RBSClient(); // CSI: 74 # this needs to be like this
	User::LeaveIfError( iServerKeepAlive->Connect() );
	}


// -----------------------------------------------------------------------------
// CBSFactory::CreateAccessL()
// -----------------------------------------------------------------------------
//
EXPORT_C MBSAccess* CBSFactory::CreateAccessL( const TDesC8& aBrandId,
    					  						  TLanguage aLanguageId,
    					  						  TBool aCacheData, /* = EFalse */
    					  						  TInt aReserved /*= 0*/ )
    {
    TRACE( T_LIT("CBSFactory::CreateAccessL begin") );
    CBSAccess* access = CBSAccess::NewL( aBrandId, *iApplicationId, *iDefaultBrand,
    									 aLanguageId, aCacheData, aReserved );
    TRACE( T_LIT("CBSFactory::CreateAccessL end") );
    return access;
    }

// -----------------------------------------------------------------------------
// CBSFactory::CreateAccessL()
// -----------------------------------------------------------------------------
//
EXPORT_C MBSAccess* CBSFactory::CreateAccessLC( const TDesC8& aBrandId,
    					  						  TLanguage aLanguageId,
    					  						  TBool aCacheData, /* = EFalse */
    					  						  TInt aReserved /*= 0 */)
    {
    CBSAccess* access = CBSAccess::NewL( aBrandId, *iApplicationId, *iDefaultBrand,
    									 aLanguageId, aCacheData, aReserved );
	CleanupClosePushL( *access );
    return access;
    }

// -----------------------------------------------------------------------------
// CBSFactory::CreateUpdaterL()
// -----------------------------------------------------------------------------
//
EXPORT_C MBSUpdater* CBSFactory::CreateUpdaterL()
    {
    CBSUpdater* updater = CBSUpdater::NewL( *iApplicationId );
    return updater;
    }

// -----------------------------------------------------------------------------
// CBSFactory::CreateUpdaterLC()
// -----------------------------------------------------------------------------
//
EXPORT_C MBSUpdater* CBSFactory::CreateUpdaterLC()
    {
    CBSUpdater* updater = CBSUpdater::NewL( *iApplicationId );
    CleanupClosePushL( *updater );
    return updater;
    }

//  End of File

