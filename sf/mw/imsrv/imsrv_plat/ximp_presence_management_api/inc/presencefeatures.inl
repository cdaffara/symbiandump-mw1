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
* Description:  Presence Features interface used by XIMP Presence clients.
*
*/

#include <ecom/ecom.h>


// -----------------------------------------------------------------------------
// MPresenceFeatures::NewL( MXIMPContext* aContext) implementation
// Factory method to instantiate the platform default
// MPresenceFeatures implementation through the ECom framework.
// -----------------------------------------------------------------------------
//
inline MPresenceFeatures* MPresenceFeatures::NewL(MXIMPContext* aContext )
    {
    _LIT8( KXIMPPresenceDefaultData, XIMP_ECOM_DEFAULT_DATA_PRESENCE_INTERFACE );

    TEComResolverParams resolvingParams;
    resolvingParams.SetDataType( KXIMPPresenceDefaultData );
    resolvingParams.SetWildcardMatch( ETrue );

    TAny* imp = REComSession::CreateImplementationL( TUid::Uid( MPresenceFeatures::KInterfaceId ),
                                                     _FOFF( MPresenceFeatures, iEcomDtorID ),
                                                     aContext, resolvingParams );

    return reinterpret_cast< MPresenceFeatures* >( imp );
    }


// -----------------------------------------------------------------------------
// MPresenceFeatures::MPresenceFeatures() implementation
// Zero initializes the iEcomDtorID member.
// -----------------------------------------------------------------------------
//
inline MPresenceFeatures::MPresenceFeatures()
    {
    iEcomDtorID = TUid::Null();
    }


// -----------------------------------------------------------------------------
// MPresenceFeatures::~MPresenceFeatures()
// Destructor notifies the ECom from implementation destruction.
// -----------------------------------------------------------------------------
//
inline MPresenceFeatures::~MPresenceFeatures()
    {
    REComSession::DestroyedImplementation( iEcomDtorID );
    }


