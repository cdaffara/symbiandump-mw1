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
* Description:  IM Features interface used by XIMP IM clients.
*
*/

#include <ecom/ecom.h>


// -----------------------------------------------------------------------------
// MProtocolImFeatures::NewL() implementation
// Factory method to instantiate the platform default
// MProtocolImFeatures implementation through the ECom framework.
// -----------------------------------------------------------------------------
//
inline MProtocolImFeatures* MProtocolImFeatures::NewL()
    {
    _LIT8( KXIMPIMDefaultData, "2219" );

    TEComResolverParams resolvingParams;
    resolvingParams.SetDataType( KXIMPIMDefaultData );
    resolvingParams.SetWildcardMatch( ETrue );
    TAny* imp = REComSession::CreateImplementationL( 
        TUid::Uid( MProtocolImFeatures::KInterfaceId), 
        _FOFF( MProtocolImFeatures, iEcomDtorID ), resolvingParams );

    return reinterpret_cast< MProtocolImFeatures* >( imp );
    }


// -----------------------------------------------------------------------------
// MProtocolImFeatures::NewL() implementation
// Factory method to instantiate the user specific implementation for 
// MProtocolImFeatures API through the ECom framework.
// -----------------------------------------------------------------------------
//
inline MProtocolImFeatures* MProtocolImFeatures::NewL( TUid aImplementationUid,
    TAny* aInitParams )
    {
    MProtocolImFeatures* ptr = reinterpret_cast<MProtocolImFeatures*>(
        REComSession::CreateImplementationL( 
        aImplementationUid, 
        _FOFF( MProtocolImFeatures, iEcomDtorID ), 
        aInitParams ) );

    return ptr;
    }


// -----------------------------------------------------------------------------
// MProtocolImFeatures::MProtocolImFeatures() implementation
// Zero initializes the iEcomDtorID member.
// -----------------------------------------------------------------------------
//
inline MProtocolImFeatures::MProtocolImFeatures()
    {
    iEcomDtorID = TUid::Null();
    }


// -----------------------------------------------------------------------------
// MProtocolImFeatures::~MProtocolImFeatures()
// Destructor notifies the ECom from implementation destruction.
// -----------------------------------------------------------------------------
//
inline MProtocolImFeatures::~MProtocolImFeatures()
    {
    REComSession::DestroyedImplementation( iEcomDtorID );
    }


// -----------------------------------------------------------------------------
// MProtocolImGroup& MProtocolImFeatures::ImGroup()
// Dummy Implementation
// -----------------------------------------------------------------------------
//
inline MProtocolImGroup& MProtocolImFeatures::ImGroup()
    {
    MProtocolImGroup* dummyGroup = NULL;
    return *dummyGroup;
    }

// -----------------------------------------------------------------------------
// MProtocolImInvitation& MProtocolImFeatures::ImInvitation()
// Dummy Implementation
// -----------------------------------------------------------------------------
//
inline MProtocolImInvitation& MProtocolImFeatures::ImInvitation()
    {
    MProtocolImInvitation* dummyInvitation = NULL;
    return *dummyInvitation;
    }

// -----------------------------------------------------------------------------
// MProtocolImSearch& MProtocolImFeatures::ImSearch()
// Dummy Implementation
// -----------------------------------------------------------------------------
//
inline MProtocolImSearch& MProtocolImFeatures::ImSearch()
    {
    MProtocolImSearch* dummySearch = NULL;
    return *dummySearch;
    }

