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
* Description:   Removes brands from branding server
*
*/


#include <utf.h>

#include <cbsfactory.h>
#include <mbsupdater.h>

#include "bsbrandremover.h"


// ======== MEMBER FUNCTIONS ========
    
// ---------------------------------------------------------------------------
// BSBrandRemover::RemoveBrand
// ---------------------------------------------------------------------------
//
void BSBrandRemover::RemoveBrandL( const TDesC& aBrandId,
                                   const TDesC& aApplicationId )
    {
    // 16bit -> 8bit conversion
    HBufC8* brandBuf = HBufC8::NewLC( aBrandId.Length() );
    HBufC8* appBuf = HBufC8::NewLC( aApplicationId.Length() );
    TPtr8 brand( brandBuf->Des() );
    TPtr8 app( appBuf->Des() );
    
    CnvUtfConverter::ConvertFromUnicodeToUtf8( brand, aBrandId );
    CnvUtfConverter::ConvertFromUnicodeToUtf8( app, aApplicationId );
    
    // Remove brand
    CBSFactory* factory = CBSFactory::NewL( brand, app );
    CleanupStack::PushL( factory );
    MBSUpdater* updater = factory->CreateUpdaterLC();
    //updater->StartTransactionL( brand, ELangOther, KBSAutomaticVersionUpdate );
    updater->StartTransactionL( brand, ELangOther, MBSUpdater::EUpdateUninstall);
    
    updater->RemoveBrandL( brand, app );
    
    updater->StopTransactionL();
    CleanupStack::PopAndDestroy( 4 ); // updater, factory, brandBuf, appBuf
    }

// End of file
