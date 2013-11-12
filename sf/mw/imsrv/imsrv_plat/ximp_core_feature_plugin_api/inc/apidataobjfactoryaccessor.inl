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
* Description:  Internal Interface for event codec
*
*/

#include <ecom/ecom.h>
#include <ximpfeaturepluginifids.hrh>

// Factory interface for internal data structures
#define XIMP_ECOM_IMPL_UID_PRESENCE_DATAMODEL_INTERFACE   0x2000B197

// -----------------------------------------------------------------------------
// MApiDataObjFactoryAccessor::NewPresenceAccessorL( ) implementation
// -----------------------------------------------------------------------------
//
inline MApiDataObjFactoryAccessor* MApiDataObjFactoryAccessor::NewPresenceAccessorL( )
    {
    return NewL(TUid::Uid( XIMP_ECOM_IMPL_UID_PRESENCE_DATAMODEL_INTERFACE ));
    }

// -----------------------------------------------------------------------------
// MApiDataObjFactoryAccessor::NewL( ) implementation
// -----------------------------------------------------------------------------
//
inline MApiDataObjFactoryAccessor* MApiDataObjFactoryAccessor::NewL( TUid aImplUid )
    {
    
    TAny* imp = REComSession::CreateImplementationL( aImplUid,
                                                     _FOFF( MApiDataObjFactoryAccessor, iEcomDtorID ));


    return reinterpret_cast< MApiDataObjFactoryAccessor* >( imp );
    }

// Added : to get list of implementations plugins, this should be common for presence, im datamodels
// -----------------------------------------------------------------------------
// MApiDataObjFactoryAccessor::NewL( ) implementation
// -----------------------------------------------------------------------------
//
inline void MApiDataObjFactoryAccessor::ListAllImplementationsL(RImplInfoPtrArray& aImplInfoArray)
	{
	REComSession::ListImplementationsL(TUid::Uid( XIMP_ECOM_IF_UID_PROTOCOL_DATAMODEL_CODEC_INTERFACE), aImplInfoArray);
	}

// -----------------------------------------------------------------------------
// MApiDataObjFactoryAccessor::MApiDataObjFactoryAccessor() implementation
// -----------------------------------------------------------------------------
//
inline MApiDataObjFactoryAccessor::MApiDataObjFactoryAccessor()
    {
    iEcomDtorID = TUid::Null();
    }


// -----------------------------------------------------------------------------
// MApiDataObjFactoryAccessor::~MApiDataObjFactoryAccessor()
// Destructor notifies the ECom from implementation destruction.
// -----------------------------------------------------------------------------
//
inline MApiDataObjFactoryAccessor::~MApiDataObjFactoryAccessor()
    {
    REComSession::DestroyedImplementation( iEcomDtorID );
    }


