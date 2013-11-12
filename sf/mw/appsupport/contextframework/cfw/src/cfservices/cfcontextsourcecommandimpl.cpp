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
* Description:  CCFContextSourceCommandImpl class implementation.
 *
*/


// SYSTEM INCLUDES
#include <s32strm.h>

// USER INCLUDES
#include "cfcontextsourcecommandimpl.h"
#include "cftrace.h"

// CONSTANTS
_LIT( KUidAttribute, "uid" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCFContextSourceCommandImpl::CCFContextSourceCommandImpl()
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CCFContextSourceCommandImpl::ConstructL()
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CCFContextSourceCommandImpl* CCFContextSourceCommandImpl::NewL()
    {
    FUNC_LOG;

    CCFContextSourceCommandImpl* self = CCFContextSourceCommandImpl::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CCFContextSourceCommandImpl* CCFContextSourceCommandImpl::NewLC()
    {
    FUNC_LOG;

    CCFContextSourceCommandImpl* self
            = new( ELeave ) CCFContextSourceCommandImpl;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CCFContextSourceCommandImpl::~CCFContextSourceCommandImpl()
    {
    FUNC_LOG;

    iParameters.ResetAndDestroy();
    iAttributes.ResetAndDestroy();
    delete iName;
    }


// ---------------------------------------------------------------------------
// CCFContextSourceCommandImpl::Sender
// ---------------------------------------------------------------------------
//
const TCFSourceCommandSenderId& CCFContextSourceCommandImpl::Sender() const
    {
    FUNC_LOG;

    return iSender;
    }

// ---------------------------------------------------------------------------
// CCFContextSourceCommandImpl::SetSender
// ---------------------------------------------------------------------------
//
void CCFContextSourceCommandImpl::SetSender(
    const TCFSourceCommandSenderId& aSender )
    {
    FUNC_LOG;

    iSender = aSender;
    }

// ---------------------------------------------------------------------------
// CCFContextSourceCommandImpl::Name
// ---------------------------------------------------------------------------
//
const TDesC& CCFContextSourceCommandImpl::Name() const
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
// CCFContextSourceCommandImpl::SetNameL
// ---------------------------------------------------------------------------
//
void CCFContextSourceCommandImpl::SetNameL( const TDesC& aName )
    {
    FUNC_LOG;

    delete iName;
    iName = NULL;
    iName = aName.AllocL();
    }

// ---------------------------------------------------------------------------
// CCFContextSourceCommandImpl::SourceUid
// ---------------------------------------------------------------------------
//
TUid CCFContextSourceCommandImpl::SourceUid() const
    {
    FUNC_LOG;

    TUid sourceUid = KNullUid;

    for ( TInt i = 0; i < iAttributes.Count(); ++i )
        {
        CCFKeyValuePair* keyValue = iAttributes[ i ];
        if ( keyValue->Key().CompareF( KUidAttribute ) == 0 )
            {
            TPtrC uidString( keyValue->Value() );
            TLex lex( uidString );

            // Get rid of '0x'.
            const TInt skip = 2;
            lex.Inc( skip );

            TUint32 uid = 0;
            const TUint KLimit = 0xFFFFFFFF;
            TInt err = lex.BoundedVal( uid, EHex, KLimit );
            if( err == KErrNone )
                {
                sourceUid = TUid::Uid( uid );
                }
            else
                {
                ERROR_1( err, "CCFContextSourceCommandImpl::SourceUid - Converting source uid from [%S] failed.", &uidString );
                }

            break;
            }
        }

    return sourceUid;
    }

// ---------------------------------------------------------------------------
// CCFContextSourceCommandImpl::Attributes
// ---------------------------------------------------------------------------
//
const RKeyValueArray& CCFContextSourceCommandImpl::Attributes() const
    {
    FUNC_LOG;

    return iAttributes;
    }

// ---------------------------------------------------------------------------
// CCFContextSourceCommandImpl::AddAttributeL
// ---------------------------------------------------------------------------
//
void CCFContextSourceCommandImpl::AddAttributeL(
    CCFKeyValuePair* aAttribute )
    {
    FUNC_LOG;

    iAttributes.AppendL( aAttribute );
    }

// ---------------------------------------------------------------------------
// CCFContextSourceCommandImpl::AddAttributesL
// ---------------------------------------------------------------------------
//
void CCFContextSourceCommandImpl::AddAttributesL( RKeyValueArray& aAttributes )
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
// CCFContextSourceCommandImpl::Parameters
// ---------------------------------------------------------------------------
//
const RSourceCommandParameterArray&
CCFContextSourceCommandImpl::Parameters() const
    {
    FUNC_LOG;

    return iParameters;
    }

// ---------------------------------------------------------------------------
// CCFContextSourceCommandImpl::AddParameterL
// ---------------------------------------------------------------------------
//
void CCFContextSourceCommandImpl::AddParameterL(
    CCFContextSourceCommandParameter* aParameter )
    {
    FUNC_LOG;

    iParameters.AppendL( aParameter );
    }

// ---------------------------------------------------------------------------
// CCFContextSourceCommandImpl::Size
// ---------------------------------------------------------------------------
//
TInt CCFContextSourceCommandImpl::Size() const
    {
    FUNC_LOG;

    TInt size = 0;

    // Sender identity
    size += sizeof( TCFSourceCommandSenderId );

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

    // Parameters: array count & data
    size += sizeof( TInt32 );
    count = iParameters.Count();
    for ( TInt i = 0; i < count; ++i )
        {
        size += iParameters[ i ]->Size();
        }

    return size;
    }

//-----------------------------------------------------------------------------
// CCFContextSourceCommandImpl::ExternalizeL
//-----------------------------------------------------------------------------
//
void CCFContextSourceCommandImpl::ExternalizeL( RWriteStream& aStream ) const
    {
    FUNC_LOG;

    // Sender identity
    const TUint8* senderId = reinterpret_cast< const TUint8* >( &iSender );
    aStream.WriteL( senderId, sizeof( TCFSourceCommandSenderId ) );

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

    // Parameters
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
// CCFContextSourceCommandImpl::InternalizeL
//-----------------------------------------------------------------------------
//
void CCFContextSourceCommandImpl::InternalizeL( RReadStream& aStream )
    {
    FUNC_LOG;

    // Clean up just in case
    delete iName;
    iName = NULL;
    iAttributes.ResetAndDestroy();
    iParameters.ResetAndDestroy();

    // Sender identity
    TUint8* senderId = reinterpret_cast< TUint8* >( &iSender );
    aStream.ReadL( senderId, sizeof( TCFSourceCommandSenderId ) );

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

    // Parameters
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
// CCFContextSourceCommandImpl::CloneL
// ---------------------------------------------------------------------------
//
CCFContextSourceCommand* CCFContextSourceCommandImpl::CloneL() const
    {
    FUNC_LOG;

    // below                                        // CLEANUP<< cmd
    CCFContextSourceCommandImpl* cmd = CCFContextSourceCommandImpl::NewLC();

    // Copy command sender identity.
    cmd->iSender = iSender;

    // Copy name.
    if ( iName )
        {
        cmd->iName = iName->AllocL();
        }

    // Copy attributes.
    for ( TInt i = 0; i < iAttributes.Count(); ++i )
        {
        CCFKeyValuePair* keyValue = iAttributes[ i ];
        // below                                    // CLEANUP<< attribute
        CCFKeyValuePair* attribute = CCFKeyValuePair::NewLC( keyValue->Key(),
                keyValue->Value() );
        cmd->iAttributes.AppendL( attribute );
        CleanupStack::Pop( attribute );             // CLEANUP>> attribute
        }

    // Copy parameters.
    for ( TInt i = 0; i < iParameters.Count(); ++i )
        {
        CCFContextSourceCommandParameter* cmdParam = iParameters[ i ];
        // below                                    // CLEANUP<< param
        CCFContextSourceCommandParameter* param = cmdParam->CloneLC();
        cmd->iParameters.AppendL( param );
        CleanupStack::Pop( param );                 // CLEANUP>> param
        }

    CleanupStack::Pop( cmd );                       // CLEANUP>> cmd
    return cmd;
    }

// End of file
