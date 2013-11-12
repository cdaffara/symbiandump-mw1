/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: Implementation of ProEngMmfInfoUtility.
*
*/



// CLASS HEADER
#include    "ProEngMmfInfoUtility.h"

// INTERNAL INCLUDES

// EXTERNAL INCLUDES
#include <e32std.h>
#include <badesca.h>
#include <mmf/common/mmfcontrollerpluginresolver.h>

namespace
	{
	// CONSTANTS
	_LIT8( KProEngRngMimeType,    "application/vnd.nokia.ringing-tone" );
	//_LIT8( KProEngPlainTextType,  "text/plain" );
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngMmfInfoUtility::GetSupportedMimeTypesL
// -----------------------------------------------------------------------------
//
void ProEngMmfInfoUtility::GetSupportedMimeTypesL( CDesC8Array& aMimeArray )
    {

    aMimeArray.InsertIsqL( KProEngRngMimeType );

	CMMFFormatSelectionParameters* formatPrms =
        CMMFFormatSelectionParameters::NewLC();
	CMMFControllerPluginSelectionParameters* controllerPrms =
        CMMFControllerPluginSelectionParameters::NewLC();

    // Empty format parameters means: "get all the supported formats"
    controllerPrms->SetRequiredPlayFormatSupportL( *formatPrms );
    RMMFControllerImplInfoArray cntrlArray;
    controllerPrms->ListImplementationsL( cntrlArray );

    TInt index( 0 );
    for( TInt i( cntrlArray.Count() - 1 ); i >= 0 ; --i )
        {
        const RMMFFormatImplInfoArray&
            infoArray( cntrlArray[i]->PlayFormats() );

        for( TInt j( infoArray.Count() - 1 ); j >= 0; --j )
            {
            const CDesC8Array& mimes = (infoArray[j])->SupportedMimeTypes();
            for( TInt k( mimes.MdcaCount() - 1 ); k >= 0; --k )
                {
                if( aMimeArray.FindIsq( mimes[k], index ) != KErrNone )
                    {
                    aMimeArray.InsertIsqL( mimes[k] );
                    }
                }
            }
        }
	
	cntrlArray.ResetAndDestroy();
	cntrlArray.Close();
    CleanupStack::PopAndDestroy( 2, formatPrms );

    //if( aMimeArray.FindIsq( KProEngPlainTextType, index ) == KErrNone )
    //    {
    //    aMimeArray.Delete( index );
    //    }

    aMimeArray.Compress();
    }

//  End of File

