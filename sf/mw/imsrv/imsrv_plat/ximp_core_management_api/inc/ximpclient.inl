/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Root interface used by Presence Framework clients.
*
*/

#include <ecom/ecom.h>


// -----------------------------------------------------------------------------
// MXIMPClient::NewClientL() implementation
// Factory method to instantiate the platform default
// MXIMPClient implementation through the ECom framework.
// -----------------------------------------------------------------------------
//
inline MXIMPClient* MXIMPClient::NewClientL()
    {
    _LIT8( KXimpFwDefaultData, XIMP_ECOM_DEFAULT_DATA_CLIENT_INTERFACE );

    TEComResolverParams resolvingParams;
    resolvingParams.SetDataType( KXimpFwDefaultData );
    resolvingParams.SetWildcardMatch( ETrue );

    TAny* imp = REComSession::CreateImplementationL( TUid::Uid( MXIMPClient::KInterfaceId ),
                                                     _FOFF( MXIMPClient, iEcomDtorID ),
                                                     resolvingParams );

    return reinterpret_cast< MXIMPClient* >( imp );
    }


// -----------------------------------------------------------------------------
// MXIMPClient::MXIMPClient() implementation
// Zero initializes the iEcomDtorID member.
// -----------------------------------------------------------------------------
//
inline MXIMPClient::MXIMPClient()
    {
    iEcomDtorID = TUid::Null();
    }


// -----------------------------------------------------------------------------
// MXIMPClient::~MXIMPClient()
// Destructor notifies the ECom from implementation destruction.
// -----------------------------------------------------------------------------
//
inline MXIMPClient::~MXIMPClient()
    {
    REComSession::DestroyedImplementation( iEcomDtorID );
    }


