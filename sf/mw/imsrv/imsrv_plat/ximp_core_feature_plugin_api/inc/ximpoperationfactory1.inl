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
// MXIMPOperationFactory::NewClientL() implementation
// Factory method to instantiate the platform default
// MXIMPOperationFactory implementation through the ECom framework.
// -----------------------------------------------------------------------------
//
inline MXIMPOperationFactory1* MXIMPOperationFactory1::NewL()
    {
    _LIT8( KXimpFwDefaultData, "" );

    TEComResolverParams resolvingParams;
    resolvingParams.SetDataType( KXimpFwDefaultData );
    resolvingParams.SetWildcardMatch( ETrue );

    TAny* imp = REComSession::CreateImplementationL( TUid::Uid( MXIMPOperationFactory1::KInterfaceId ),
                                                     _FOFF( MXIMPOperationFactory1, iEcomDtorID ),
                                                     resolvingParams );

    return reinterpret_cast< MXIMPOperationFactory1* >( imp );
    }

inline MXIMPOperationFactory1* MXIMPOperationFactory1::NewL( TUid aImplUid )
	{
    TAny* imp = REComSession::CreateImplementationL( aImplUid,
                                                     _FOFF( MXIMPOperationFactory1, iEcomDtorID ));


    return reinterpret_cast< MXIMPOperationFactory1* >( imp );
	}
	
// -----------------------------------------------------------------------------
// MXIMPOperationFactory1::MXIMPOperationFactory1() implementation
// Zero initializes the iEcomDtorID member.
// -----------------------------------------------------------------------------
//
inline MXIMPOperationFactory1::MXIMPOperationFactory1()
    {
    iEcomDtorID = TUid::Null();
    }


// -----------------------------------------------------------------------------
// MXIMPOperationFactory1::~MXIMPOperationFactory1()
// Destructor notifies the ECom from implementation destruction.
// -----------------------------------------------------------------------------
//
inline MXIMPOperationFactory1::~MXIMPOperationFactory1()
    {
    REComSession::DestroyedImplementation( iEcomDtorID );
    }

inline void MXIMPOperationFactory1::ListAllImplementationsL(RImplInfoPtrArray& aImplInfoArray) 
	{
	REComSession::ListImplementationsL(TUid::Uid(MXIMPOperationFactory1::KInterfaceId), aImplInfoArray);
	}
