/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CSAnimPlugin class
*
*/


#include "sanimplugin.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Constructor
//
// -----------------------------------------------------------------------------
//
CSAnimPlugin::CSAnimPlugin() : iClientStatus( 0 )
    {
    }


// ---------------------------------------------------------------------------
// CSAnimPlugin::SetRepeatCount
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CSAnimPlugin::SetRepeatCount( const TInt aRepeatCount )
    {
    iRepeatCount = aRepeatCount;
    }


// ---------------------------------------------------------------------------
// CSAnimPlugin::SetClientRequest
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CSAnimPlugin::SetClientRequest( TRequestStatus& aStatus )
    {
    __ASSERT_ALWAYS( !iClientStatus, User::Invariant() );

    aStatus = KRequestPending;
    iClientStatus = &aStatus;
    }


// ---------------------------------------------------------------------------
// CSAnimPlugin::CompleteClientRequest
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CSAnimPlugin::CompleteClientRequest( const TInt aStatusCode )
    {
    if ( iClientStatus )
        {
        User::RequestComplete( iClientStatus, aStatusCode );
        iClientStatus = NULL;
        }
    }
