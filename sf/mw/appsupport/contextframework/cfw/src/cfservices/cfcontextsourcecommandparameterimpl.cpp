/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFContextSourceCommandParameterImpl class implementation.
 *
*/


// SYSTEM INCLUDES
#include <s32strm.h>

// USER INCLUDES
#include "cfcontextsourcecommandparameterimpl.h"
#include "cftrace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCFContextSourceCommandParameterImpl::CCFContextSourceCommandParameterImpl()
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CCFContextSourceCommandParameterImpl::ConstructL()
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CCFContextSourceCommandParameterImpl*
CCFContextSourceCommandParameterImpl::NewL()
    {
    FUNC_LOG;

    CCFContextSourceCommandParameterImpl* self
            = CCFContextSourceCommandParameterImpl::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CCFContextSourceCommandParameterImpl*
CCFContextSourceCommandParameterImpl::NewLC()
    {
    FUNC_LOG;

    CCFContextSourceCommandParameterImpl* self
            = new( ELeave ) CCFContextSourceCommandParameterImpl;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CCFContextSourceCommandParameterImpl::~CCFContextSourceCommandParameterImpl()
    {
    FUNC_LOG;

    iParameters.ResetAndDestroy();
    iAttributes.ResetAndDestroy();
    delete iName;
    }


// ---------------------------------------------------------------------------
// CCFContextSourceCommandParameterImpl::Name
// ---------------------------------------------------------------------------
//
const TDesC& CCFContextSourceCommandParameterImpl::Name() const
    {
    FUNC_LOG;

    if ( iName )
        {
        return *iName;
        }
    else
        {
        return KNullDesC;
        }
    }

// ---------------------------------------------------------------------------
// CCFContextSourceCommandParameterImpl::SetNameL
// ---------------------------------------------------------------------------
//
void CCFContextSourceCommandParameterImpl::SetNameL( const TDesC& aName )
    {
    FUNC_LOG;

    delete iName;
    iName = NULL;
    iName = aName.AllocL();
    }

// ---------------------------------------------------------------------------
// CCFContextSourceCommandParameterImpl::Attributes
// ---------------------------------------------------------------------------
//
const RKeyValueArray& CCFContextSourceCommandParameterImpl::Attributes() const
    {
    FUNC_LOG;

    return iAttributes;
    }

// ---------------------------------------------------------------------------
// CCFContextSourceCommandParameterImpl::AddAttributeL
// ---------------------------------------------------------------------------
//
void CCFContextSourceCommandParameterImpl::AddAttributeL(
    CCFKeyValuePair* aAttribute )
    {
    FUNC_LOG;

    iAttributes.AppendL( aAttribute );
    }

// ---------------------------------------------------------------------------
// CCFContextSourceCommandParameterImpl::AddAttributesL
// ---------------------------------------------------------------------------
//
void CCFContextSourceCommandParameterImpl::AddAttributesL(
    RKeyValueArray& aAttributes )
    {
    FUNC_LOG;

    TInt insertPos = iAttributes.Count();
    for ( TInt i = aAttributes.Count() - 1; i >= 0; --i )
        {
        CCFKeyValuePair* attribute = aAttributes[ i ];
        aAttributes.Remove( i );
        CleanupStack::PushL( attribute );       // CLEANUP<< attribute
        iAttributes.Insert( attribute, insertPos );
        CleanupStack::Pop( attribute );         // CLEANUP>> attribute
        }
    }

// ---------------------------------------------------------------------------
// CCFContextSourceCommandParameterImpl::Parameters
// ---------------------------------------------------------------------------
//
const RSourceCommandParameterArray&
CCFContextSourceCommandParameterImpl::Parameters() const
    {
    FUNC_LOG;

    return iParameters;
    }

// ---------------------------------------------------------------------------
// CCFContextSourceCommandParameterImpl::AddParameterL
// ---------------------------------------------------------------------------
//
void CCFContextSourceCommandParameterImpl::AddParameterL(
    CCFContextSourceCommandParameter* aParameter )
    {
    FUNC_LOG;

    iParameters.AppendL( aParameter );
    }

// ---------------------------------------------------------------------------
// CCFContextSourceCommandParameterImpl::Size
// ---------------------------------------------------------------------------
//
TInt CCFContextSourceCommandParameterImpl::Size() const
    {
    FUNC_LOG;

    TInt size = 0;

    // Name: length and size of iName
    size += sizeof( TInt32 );
    size += iName->Size();

    // Attributes: array count & data
    size += sizeof( TInt32 );
    TInt count = iAttributes.Count();
    for ( TInt i = 0; i < count; ++i )
        {
        // Key length & data
        size += sizeof( TInt32 );
        size += iAttributes[ i ]->Key().Size();

        // Value length & data
        size += sizeof( TInt32 );
        size += iAttributes[ i ]->Value().Size();
        }

    // Nested parameters: array count & data
    size += sizeof( TInt32 );
    count = iParameters.Count();
    for ( TInt i = 0; i < count; ++i )
        {
        size += iParameters[ i ]->Size();
        }

    return size;
    }

//-----------------------------------------------------------------------------
// CCFContextSourceCommandParameterImpl::ExternalizeL
//-----------------------------------------------------------------------------
//
void CCFContextSourceCommandParameterImpl::ExternalizeL(
    RWriteStream& aStream ) const
    {
    FUNC_LOG;

    // Name
    aStream.WriteInt32L( iName->Length() );
    aStream.WriteL( *iName );

    // Attributes
    TInt count = iAttributes.Count();
    aStream.WriteInt32L( count );
    for ( TInt i = 0; i < count; ++i )
        {
        TPtrC buf( KNullDesC );

        // Key length & data
        buf.Set( iAttributes[ i ]->Key() );
        aStream.WriteInt32L( buf.Length() );
        aStream.WriteL( buf, buf.Length() );

        // Value length & data
        buf.Set( iAttributes[ i ]->Value() );
        aStream.WriteInt32L( buf.Length() );
        aStream.WriteL( buf, buf.Length() );
        }

    // Nested parameters
    count = iParameters.Count();
    aStream.WriteInt32L( count );
    for ( TInt i = 0; i < count; ++i )
        {
        iParameters[ i ]->ExternalizeL( aStream );
        }

    // Commit stream
    aStream.CommitL();
    }

//-----------------------------------------------------------------------------
// CCFContextSourceCommandParameterImpl::InternalizeL
//-----------------------------------------------------------------------------
//
void CCFContextSourceCommandParameterImpl::InternalizeL( RReadStream& aStream )
    {
    FUNC_LOG;

    // Clean up just in case
    delete iName;
    iName = NULL;
    iAttributes.ResetAndDestroy();
    iParameters.ResetAndDestroy();

    // Name
    TInt len = aStream.ReadInt32L();
    iName = HBufC::NewL( len );
    TPtr namePtr = iName->Des();
    aStream.ReadL( namePtr, len );

    // Attributes
    TInt count = aStream.ReadInt32L();
    for ( TInt i = 0; i < count; ++i )
        {
        // Key length & data
        len = aStream.ReadInt32L();
        HBufC* key = HBufC::NewLC( len );           // CLEANUP<< key
        TPtr keyPtr = key->Des();
        aStream.ReadL( keyPtr, len );

        // Value length & data
        len = aStream.ReadInt32L();
        HBufC* value = HBufC::NewLC( len );         // CLEANUP<< value
        TPtr valuePtr = value->Des();
        aStream.ReadL( valuePtr, len );

        // Create & append key value pair
        CCFKeyValuePair* keyValuePair = CCFKeyValuePair::NewL( key, value );
        CleanupStack::Pop( value );                 // CLEANUP>> value
        CleanupStack::Pop( key );                   // CLEANUP>> key

        CleanupStack::PushL( keyValuePair );        // CLEANUP<< keyValuePair
        iAttributes.AppendL( keyValuePair );
        CleanupStack::Pop( keyValuePair );          // CLEANUP>> keyValuePair
        }

    // Nested parameters
    count = aStream.ReadInt32L();
    for ( TInt i = 0; i < count; ++i )
        {
        // below                                    // CLEANUP<< param
        CCFContextSourceCommandParameter* param
            = CCFContextSourceCommandParameter::NewLC();
        param->InternalizeL( aStream );
        iParameters.AppendL( param );
        CleanupStack::Pop( param );                 // CLEANUP>> param
        }
    }

// ---------------------------------------------------------------------------
// CCFContextSourceCommandParameterImpl::CloneLC
// ---------------------------------------------------------------------------
//
CCFContextSourceCommandParameter*
CCFContextSourceCommandParameterImpl::CloneLC() const
    {
    FUNC_LOG;

    // below                                        // CLEANUP<< parameter
    CCFContextSourceCommandParameterImpl* parameter
        = CCFContextSourceCommandParameterImpl::NewLC();

    // Copy name.
    if ( iName )
        {
        parameter->iName = iName->AllocL();
        }

    // Copy attributes.
    for ( TInt i = 0; i < iAttributes.Count(); ++i )
        {
        CCFKeyValuePair* keyValue = iAttributes[ i ];
        // below                                    // CLEANUP<< attribute
        CCFKeyValuePair* attribute = CCFKeyValuePair::NewLC( keyValue->Key(),
                keyValue->Value() );
        parameter->iAttributes.AppendL( attribute );
        CleanupStack::Pop( attribute );             // CLEANUP>> attribute
        }

    // Copy parameters.
    for ( TInt i = 0; i < iParameters.Count(); ++i )
        {
        CCFContextSourceCommandParameter* cmdParam = iParameters[ i ];
        // below                                    // CLEANUP<< param
        CCFContextSourceCommandParameter* param = cmdParam->CloneLC();
        parameter->iParameters.AppendL( param );
        CleanupStack::Pop( param );                 // CLEANUP>> param
        }

    return parameter; // Left on the cleanup stack.
    }

// End of file
