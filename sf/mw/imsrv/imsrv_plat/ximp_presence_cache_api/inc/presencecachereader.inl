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
* Description:  Presence Cache reader interface used by XIMP Presence clients.
*
*/

#include <ecom/ecom.h>

// -----------------------------------------------------------------------------
// MPresenceCacheReader::NewL() implementation
// Factory method to instantiate the platform default
// MPresenceCacheReader implementation through the ECom framework.
// -----------------------------------------------------------------------------
//
inline MPresenceCacheReader* MPresenceCacheReader::NewL()
    {
    _LIT8( KXIMPPresenceDefaultData, XIMP_ECOM_DEFAULT_DATA_PRESENCE_INTERFACE );

    TEComResolverParams resolvingParams;
    resolvingParams.SetDataType( KXIMPPresenceDefaultData );
    resolvingParams.SetWildcardMatch( ETrue );

    TAny* imp = REComSession::CreateImplementationL( TUid::Uid( MPresenceCacheReader::KInterfaceId ),
                                                     _FOFF( MPresenceCacheReader, iEcomDtorID ), 
                                                                            resolvingParams );

    return reinterpret_cast< MPresenceCacheReader* >( imp );
    }


// -----------------------------------------------------------------------------
// MPresenceCacheReader::MPresenceCacheReader() implementation
// Zero initializes the iEcomDtorID member.
// -----------------------------------------------------------------------------
//
inline MPresenceCacheReader::MPresenceCacheReader()
    {
    iEcomDtorID = TUid::Null();
    }


// -----------------------------------------------------------------------------
// MPresenceCacheReader::~MPresenceCacheReader()
// Destructor notifies the ECom from implementation destruction.
// -----------------------------------------------------------------------------
//
inline MPresenceCacheReader::~MPresenceCacheReader()
    {
    REComSession::DestroyedImplementation( iEcomDtorID );
    }


