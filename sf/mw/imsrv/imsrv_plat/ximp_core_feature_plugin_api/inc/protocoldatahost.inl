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
* Description:  protocol data host interface used by XIMP clients.
*
*/

#include <ecom/ecom.h>


// -----------------------------------------------------------------------------
// MProtocolDataHost::NewL( MXIMPHost& aHost) implementation
// Factory method to instantiate the platform default
// MProtocolDataHost implementation through the ECom framework.
// -----------------------------------------------------------------------------
//
inline MProtocolDataHost* MProtocolDataHost::NewL(MXIMPHost& aHost )
    {
    _LIT8( KIMDefaultData, "" );

    TEComResolverParams resolvingParams;
    resolvingParams.SetDataType( KIMDefaultData );
    resolvingParams.SetWildcardMatch( ETrue );
    TAny* imp = REComSession::CreateImplementationL( TUid::Uid( MProtocolDataHost::KInterfaceId),
                                                     _FOFF( MProtocolDataHost, iEcomDtorID ),
                                                     &aHost, resolvingParams );
    return reinterpret_cast< MProtocolDataHost* >( imp );
    }


inline MProtocolDataHost* MProtocolDataHost::NewL( TUid aImplUid , MXIMPHost& aHost)
	{
    TAny* imp = REComSession::CreateImplementationL( aImplUid,
                                                     _FOFF( MProtocolDataHost, iEcomDtorID ),&aHost);


    return reinterpret_cast< MProtocolDataHost* >( imp );
	}


// -----------------------------------------------------------------------------
// MProtocolDataHost::MProtocolDataHost() implementation
// Zero initializes the iEcomDtorID member.
// -----------------------------------------------------------------------------
//
inline MProtocolDataHost::MProtocolDataHost()
    {
    iEcomDtorID = TUid::Null();
    }


// -----------------------------------------------------------------------------
// MProtocolDataHost::~MProtocolDataHost()
// Destructor notifies the ECom from implementation destruction.
// -----------------------------------------------------------------------------
//
inline MProtocolDataHost::~MProtocolDataHost()
    {
    REComSession::DestroyedImplementation( iEcomDtorID );
    }

// -----------------------------------------------------------------------------
// MProtocolDataHost::ListAllImplementationsL()
// To get list of implementation plugins, who implements this interface
// -----------------------------------------------------------------------------
//
inline void MProtocolDataHost::ListAllImplementationsL(RImplInfoPtrArray& aImplInfoArray) 
	{
	REComSession::ListImplementationsL(TUid::Uid(MProtocolDataHost::KInterfaceId), aImplInfoArray);
	}

