/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CProtocolImFeatures::NewL() implementation
// Factory method to instantiate the platform default
// CProtocolImFeatures implementation through the ECom framework.
// -----------------------------------------------------------------------------
//
inline CProtocolImFeatures* CProtocolImFeatures::NewL()
    {
    _LIT8( KXIMPIMDefaultData, "2219" );

    TEComResolverParams resolvingParams;
    resolvingParams.SetDataType( KXIMPIMDefaultData );
    resolvingParams.SetWildcardMatch( ETrue );
    TAny* imp = REComSession::CreateImplementationL( 
        TUid::Uid( CProtocolImFeatures::KInterfaceId), 
        _FOFF( CProtocolImFeatures, iEcomDtorID ), resolvingParams );

    return reinterpret_cast< CProtocolImFeatures* >( imp );
    }


// -----------------------------------------------------------------------------
// CProtocolImFeatures::NewL() implementation
// Factory method to instantiate the user specific implementation for 
// CProtocolImFeatures API through the ECom framework.
// -----------------------------------------------------------------------------
//
inline CProtocolImFeatures* CProtocolImFeatures::NewL( TUid aImplementationUid,
    TAny* aInitParams )
    {
    CProtocolImFeatures* ptr = reinterpret_cast<CProtocolImFeatures*>(
        REComSession::CreateImplementationL( 
        aImplementationUid, 
        _FOFF( CProtocolImFeatures, iEcomDtorID ), 
        aInitParams ) );

    return ptr;
    }


// -----------------------------------------------------------------------------
// CProtocolImFeatures::CProtocolImFeatures() implementation
// Zero initializes the iEcomDtorID member.
// -----------------------------------------------------------------------------
//
inline CProtocolImFeatures::CProtocolImFeatures()
    {
    iEcomDtorID = TUid::Null();
    }


// -----------------------------------------------------------------------------
// CProtocolImFeatures::~CProtocolImFeatures()
// Destructor notifies the ECom from implementation destruction.
// -----------------------------------------------------------------------------
//
inline CProtocolImFeatures::~CProtocolImFeatures()
    {
    REComSession::DestroyedImplementation( iEcomDtorID );
    }
