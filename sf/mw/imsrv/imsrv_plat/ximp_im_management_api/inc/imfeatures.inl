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
// MImFeatures::NewL( MXIMPContext* aContext) implementation
// Factory method to instantiate the platform default
// MImFeatures implementation through the ECom framework.
// -----------------------------------------------------------------------------
//
inline MImFeatures* MImFeatures::NewL(MXIMPContext* aContext )
    {
    _LIT8( KXIMPIMDefaultData, XIMP_ECOM_DEFAULT_DATA_IM_INTERFACE );

    TEComResolverParams resolvingParams;
    resolvingParams.SetDataType( KXIMPIMDefaultData );
    resolvingParams.SetWildcardMatch( ETrue );
		//TInt auid = 0x2000B5F4; ///MImFeatures::KInterfaceId
    TAny* imp = REComSession::CreateImplementationL( TUid::Uid( MImFeatures::KInterfaceId),
                                                     _FOFF( MImFeatures, iEcomDtorID ),
                                                     aContext, resolvingParams );

    return reinterpret_cast< MImFeatures* >( imp );
    }


// -----------------------------------------------------------------------------
// MImFeatures::MImFeatures() implementation
// Zero initializes the iEcomDtorID member.
// -----------------------------------------------------------------------------
//
inline MImFeatures::MImFeatures()
    {
    iEcomDtorID = TUid::Null();
    }


// -----------------------------------------------------------------------------
// MImFeatures::~MImFeatures()
// Destructor notifies the ECom from implementation destruction.
// -----------------------------------------------------------------------------
//
inline MImFeatures::~MImFeatures()
    {
    REComSession::DestroyedImplementation( iEcomDtorID );
    }


