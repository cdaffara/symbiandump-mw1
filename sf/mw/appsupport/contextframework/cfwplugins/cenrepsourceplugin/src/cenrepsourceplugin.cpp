/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCenRepSourcePlugIn class implementation.
*
*/


#include <e32cmn.h>
#include <cfcontextsourcesettingarray.h>

#include "cenrepsourceplugin.h"
#include "cenrephandler.h"
#include "cenreptrace.h"

// ======== LOCAL FUNCTIONS =========

// ======== MEMBER FUNCTIONS ========

CCenRepSourcePlugIn* CCenRepSourcePlugIn::NewL(
    TContextSourceInitParams* aParams )
    {
    FUNC_LOG;

    CCenRepSourcePlugIn* self = CCenRepSourcePlugIn::NewLC( aParams );
    CleanupStack::Pop( self );
    return self;
    }

CCenRepSourcePlugIn* CCenRepSourcePlugIn::NewLC(
    TContextSourceInitParams* aParams )
    {
    FUNC_LOG;

    CCenRepSourcePlugIn* self =
        new( ELeave ) CCenRepSourcePlugIn( aParams );
    CleanupStack::PushL( self );
    return self;
    }

CCenRepSourcePlugIn::~CCenRepSourcePlugIn()
    {
    FUNC_LOG;

    // Cleanup settings
    iCenRepHandlerArray.ResetAndDestroy();
    delete iSettings;
    }

CCenRepSourcePlugIn::CCenRepSourcePlugIn(
    TContextSourceInitParams* aParams ) :
    CCFContextSourcePlugIn( aParams )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// CCenRepSourcePlugIn::HandleSettingL
// ---------------------------------------------------------------------------
//
void CCenRepSourcePlugIn::HandleSettingL(
    CCFContextSourceSettingArray* aSettingList )
    {
    FUNC_LOG;

    // Take ownership from settings
    iSettings = aSettingList;
    }

// ---------------------------------------------------------------------------
// CCenRepSourcePlugIn::DefineContextsL
// ---------------------------------------------------------------------------
//
void CCenRepSourcePlugIn::DefineContextsL()
    {
    FUNC_LOG;

    // Definitions will be made during CCenRepAttributes object initialization
    }

// ---------------------------------------------------------------------------
// CCenRepSourcePlugIn::InitializeL
// ---------------------------------------------------------------------------
//
void CCenRepSourcePlugIn::InitializeL()
    {
    FUNC_LOG;

    if( iSettings )
        {
        TInt keyNumbers = iSettings->Count();
        iCenRepHandlerArray.ReserveL( keyNumbers );
        for ( TInt i=0; i<keyNumbers; i++ )
            {
            CCenRepHandler* localCenRepHandler = NULL;

            TRAPD( err,
                localCenRepHandler = CCenRepHandler::NewL( iCF, *iSettings, i ) );
            if( err == KErrNone )
                {
                CleanupStack::PushL( localCenRepHandler );
                iCenRepHandlerArray.AppendL( localCenRepHandler );
                CleanupStack::Pop( localCenRepHandler );
                }
            }
        }
    }
