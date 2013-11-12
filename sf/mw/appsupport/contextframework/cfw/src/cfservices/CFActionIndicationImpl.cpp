/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFActionIndicationImpl class implementation.
*
*/


#include <s32strm.h>

#include "CFActionIndicationImpl.h"
#include "cftrace.h"

EXPORT_C CCFActionIndicationImpl* CCFActionIndicationImpl::NewL()
    {
    FUNC_LOG;
    
    CCFActionIndicationImpl* self = CCFActionIndicationImpl::NewLC();
    CleanupStack::Pop( self );
    
    return self;
    }

EXPORT_C CCFActionIndicationImpl* CCFActionIndicationImpl::NewLC()
    {
    FUNC_LOG;
    
    CCFActionIndicationImpl* self = new( ELeave ) CCFActionIndicationImpl;
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

CCFActionIndicationImpl::~CCFActionIndicationImpl()
    {
    FUNC_LOG;
    
    delete iIdentifier;
    iParameters.ResetAndDestroy();
    }
    
CCFActionIndicationImpl::CCFActionIndicationImpl()
    {
    FUNC_LOG;    
    }
    
void CCFActionIndicationImpl::ConstructL()
    {
    FUNC_LOG;
    
    iIdentifier = KNullDesC().AllocL();
    }
    
// METHODS

//-----------------------------------------------------------------------------
// CCFActionIndicationImpl::SetIdentifierL
//-----------------------------------------------------------------------------
//
void CCFActionIndicationImpl::SetIdentifierL(
    const TDesC& aIdentifier )
    {
    FUNC_LOG;
    
    TPtr identifierPtr = iIdentifier->Des();
    if( identifierPtr.MaxLength() >= aIdentifier.Length() )
        {
        // Just copy
        identifierPtr.Copy( aIdentifier );
        }
    else
        {
        // Delete and create a new one
        delete iIdentifier;
        iIdentifier = NULL;
        iIdentifier = aIdentifier.AllocL();
        }
    }

//-----------------------------------------------------------------------------
// CCFActionIndicationImpl::Identifier
//-----------------------------------------------------------------------------
//
const TDesC& CCFActionIndicationImpl::Identifier() const
    {
    FUNC_LOG;
    
    return *iIdentifier;
    }

//-----------------------------------------------------------------------------
// CCFActionIndicationImpl::AddParameterL
//-----------------------------------------------------------------------------
//
void CCFActionIndicationImpl::AddParameterL( const TDesC& aKey,
    const TDesC& aValue )
    {
    FUNC_LOG;
    
    CCFKeyValuePair* obj = CCFKeyValuePair::NewLC( aKey, aValue );
    iParameters.AppendL( obj );
    CleanupStack::Pop( obj );
    }

//-----------------------------------------------------------------------------
// CCFActionIndicationImpl::Parameters
//-----------------------------------------------------------------------------
//
const RKeyValueArray& CCFActionIndicationImpl::Parameters() const
    {
    FUNC_LOG;
    
    return iParameters;
    }

//-----------------------------------------------------------------------------
// CCFActionIndicationImpl::Size
//-----------------------------------------------------------------------------
//
EXPORT_C TInt CCFActionIndicationImpl::Size() const
    {
    FUNC_LOG;
    
    TInt size = 0;
    
    // Identifier:
    // length of iIdentifier
    // Size of iIdentifier
    size += sizeof( TInt );
    size += iIdentifier->Size();
    
    // Parameters:
    // array count
    size += sizeof( TInt );
    TInt count = iParameters.Count();
    for( TInt i = 0; i < count; i++ )
        {
        // key length, key data
        size += sizeof( TInt );
        size += iParameters[i]->Key().Size();

        // value len, value data
        size += sizeof( TInt );
        size += iParameters[i]->Value().Size();
        }
    
    return size;
    }

//-----------------------------------------------------------------------------
// CCFActionIndicationImpl::ExternalizeL
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFActionIndicationImpl::ExternalizeL( RWriteStream& aStream )
    {
    FUNC_LOG;
    
    // Identifier
    aStream.WriteInt16L( iIdentifier->Length() );
    aStream.WriteL( *iIdentifier );
    
    // Parameters
    TPtrC buf( KNullDesC );
    TInt count = iParameters.Count();
    aStream.WriteInt16L( count );
    for( TInt i = 0; i < count; i++ )
        {
        // Key length and data
        buf.Set( iParameters[i]->Key() );
        aStream.WriteInt16L( buf.Length() );
        aStream.WriteL( buf, buf.Length() );
        
        // Value length and data
        buf.Set( iParameters[i]->Value() );
        aStream.WriteInt16L( buf.Length() );
        aStream.WriteL( buf, buf.Length() );
        }
    
    // Commit stream
    aStream.CommitL();
    }

//-----------------------------------------------------------------------------
// CCFActionIndicationImpl::InternalizeL
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFActionIndicationImpl::InternalizeL( RReadStream& aStream )
    {
    FUNC_LOG;
    
    // Clean up just in case
    delete iIdentifier;
    iIdentifier = NULL;
    iParameters.ResetAndDestroy();
    
    TInt len = 0;

    // Identifier
    len = aStream.ReadInt16L();
    iIdentifier = ReadBufferFromStreamLC( len, aStream );
    CleanupStack::Pop( iIdentifier );
    
    // Parameters
    HBufC* key = NULL;
    HBufC* value = NULL;
    TInt count = aStream.ReadInt16L();
    for( TInt i = 0; i < count; i++ )
        {
        // Key length and data
        len = aStream.ReadInt16L();
        key = ReadBufferFromStreamLC( len, aStream );
        
        // Value length and data
        len = aStream.ReadInt16L();
        value = ReadBufferFromStreamLC( len, aStream );
        
        // Append
        CCFKeyValuePair* keyValuePair = CCFKeyValuePair::NewL( key, value );
        CleanupStack::Pop( value );
        CleanupStack::Pop( key );

        CleanupStack::PushL( keyValuePair );
        iParameters.AppendL( keyValuePair );
        CleanupStack::Pop( keyValuePair );
        }
    }

//-----------------------------------------------------------------------------
// CCFActionIndicationImpl::ReadBufferFromStreamLC
//-----------------------------------------------------------------------------
//
HBufC* CCFActionIndicationImpl::ReadBufferFromStreamLC( TInt aLength,
    RReadStream& aStream )
    {
    FUNC_LOG;
    
    HBufC* buffer = HBufC::NewLC( aLength );
    TPtr bufferPtr = buffer->Des();
    aStream.ReadL( bufferPtr, aLength );
    
    return buffer;
    }
