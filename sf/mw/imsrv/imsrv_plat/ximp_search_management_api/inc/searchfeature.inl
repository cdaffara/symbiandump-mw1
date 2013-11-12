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
* Description:  Search Feature interface used by XIMP  clients.
*
*/

#include <ecom/ecom.h>


// -----------------------------------------------------------------------------
// MSearchFeature::NewL( MXIMPContext* aContext) implementation
// Factory method to instantiate the platform default
// MSearchFeature implementation through the ECom framework.
// -----------------------------------------------------------------------------
//
inline MSearchFeature* MSearchFeature::NewL(MXIMPContext* aContext )
    {
    _LIT8( KXIMPSearchDefaultData, XIMP_ECOM_DEFAULT_DATA_SEARCH_INTERFACE );

    TEComResolverParams resolvingParams;
    resolvingParams.SetDataType( KXIMPSearchDefaultData );
    resolvingParams.SetWildcardMatch( ETrue );
	
    TAny* imp = REComSession::CreateImplementationL( TUid::Uid( MSearchFeature::KInterfaceId),
                                                     _FOFF( MSearchFeature, iEcomDtorID ),
                                                     aContext, resolvingParams );

    return reinterpret_cast< MSearchFeature* >( imp );
    }


// -----------------------------------------------------------------------------
// MSearchFeature::MSearchFeature() implementation
// Zero initializes the iEcomDtorID member.
// -----------------------------------------------------------------------------
//
inline MSearchFeature::MSearchFeature()
    {
    iEcomDtorID = TUid::Null();
    }


// -----------------------------------------------------------------------------
// MSearchFeature::~MSearchFeature()
// Destructor notifies the ECom from implementation destruction.
// -----------------------------------------------------------------------------
//
inline MSearchFeature::~MSearchFeature()
    {
    REComSession::DestroyedImplementation( iEcomDtorID );
    }


