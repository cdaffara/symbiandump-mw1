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
// MProtocolImDataHost::NewL( MXIMPHost& aHost) implementation
// Factory method to instantiate the platform default
// MProtocolImDataHost implementation through the ECom framework.
// -----------------------------------------------------------------------------
//
inline MProtocolImDataHost* MProtocolImDataHost::NewL(MXIMPHost& aHost )
    {
    _LIT8( KIMDefaultData, "" );

    TEComResolverParams resolvingParams;
    resolvingParams.SetDataType( KIMDefaultData );
    resolvingParams.SetWildcardMatch( ETrue );
    TAny* imp = REComSession::CreateImplementationL( TUid::Uid( MProtocolImDataHost::KInterfaceId),
                                                     _FOFF( MProtocolImDataHost, iEcomDtorID ),
                                                     &aHost, resolvingParams );
    return reinterpret_cast< MProtocolImDataHost* >( imp );
    }


// -----------------------------------------------------------------------------
// MProtocolImDataHost::MProtocolImDataHost() implementation
// Zero initializes the iEcomDtorID member.
// -----------------------------------------------------------------------------
//
inline MProtocolImDataHost::MProtocolImDataHost()
    {
    iEcomDtorID = TUid::Null();
    }


// -----------------------------------------------------------------------------
// MProtocolImDataHost::~MProtocolImDataHost()
// Destructor notifies the ECom from implementation destruction.
// -----------------------------------------------------------------------------
//
inline MProtocolImDataHost::~MProtocolImDataHost()
    {
    REComSession::DestroyedImplementation( iEcomDtorID );
    }


