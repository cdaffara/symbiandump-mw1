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
* Description:  Presence Cache writer interface used by XIMP Presence clients.
*
*/

#include <ecom/ecom.h>

// -----------------------------------------------------------------------------
// MPresenceCacheWriter::NewL() implementation
// Factory method to instantiate the platform default
// MPresenceCacheWriter implementation through the ECom framework.
// -----------------------------------------------------------------------------
//
inline MPresenceCacheWriter* MPresenceCacheWriter::NewL()
    {
    _LIT8( KXIMPPresenceDefaultData, XIMP_ECOM_DEFAULT_DATA_PRESENCE_INTERFACE );

    TEComResolverParams resolvingParams;
    resolvingParams.SetDataType( KXIMPPresenceDefaultData );
    resolvingParams.SetWildcardMatch( ETrue );

    TAny* imp = REComSession::CreateImplementationL( TUid::Uid( MPresenceCacheWriter::KInterfaceId ),
                                                     _FOFF( MPresenceCacheWriter, iEcomDtorID ), 
                                                                            resolvingParams);

    return reinterpret_cast< MPresenceCacheWriter* >( imp );
    }


// -----------------------------------------------------------------------------
// MPresenceCacheWriter::MPresenceCacheWriter() implementation
// Zero initializes the iEcomDtorID member.
// -----------------------------------------------------------------------------
//
inline MPresenceCacheWriter::MPresenceCacheWriter()
    {
    iEcomDtorID = TUid::Null();
    }


// -----------------------------------------------------------------------------
// MPresenceCacheWriter::~MPresenceCacheWriter()
// Destructor notifies the ECom from implementation destruction.
// -----------------------------------------------------------------------------
//
inline MPresenceCacheWriter::~MPresenceCacheWriter()
    {
    REComSession::DestroyedImplementation( iEcomDtorID );
    }


