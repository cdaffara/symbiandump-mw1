/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ?Description
*
*/


#include <s32strm.h>

#include "CFActionSubscriptionImpl.h"
#include "cftrace.h"

EXPORT_C CCFActionSubscriptionImpl* CCFActionSubscriptionImpl::NewL()
    {
    FUNC_LOG;
    
    CCFActionSubscriptionImpl* self =
        CCFActionSubscriptionImpl::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

EXPORT_C CCFActionSubscriptionImpl* CCFActionSubscriptionImpl::NewLC()
    {
    FUNC_LOG;
    
    CCFActionSubscriptionImpl* self =
        new( ELeave ) CCFActionSubscriptionImpl;
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

CCFActionSubscriptionImpl::~CCFActionSubscriptionImpl()
    {
    FUNC_LOG;
    
    delete iIdentifier;
    }
    
void CCFActionSubscriptionImpl::ConstructL()
    {
    FUNC_LOG;
    
    iIdentifier = KNullDesC().AllocL();
    }

CCFActionSubscriptionImpl::CCFActionSubscriptionImpl()
    {
    FUNC_LOG;
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFActionSubscriptionImpl::SetActionIdentifierL
//-----------------------------------------------------------------------------
//
void CCFActionSubscriptionImpl::SetActionIdentifierL(
    const TDesC& aIdentifier )
    {
    FUNC_LOG;
    
    TPtr identifier = iIdentifier->Des();
    if( aIdentifier.Length() < identifier.MaxLength() )
        {
        // Just copy
        identifier.Copy( aIdentifier );
        }
    else
        {
        // Delete and re-allocate
        delete iIdentifier;
        iIdentifier = 0;
        iIdentifier = aIdentifier.AllocL();
        }
    }
    
//-----------------------------------------------------------------------------
// CCFActionSubscriptionImpl::ActionIdentifier
//-----------------------------------------------------------------------------
//
const TDesC& CCFActionSubscriptionImpl::ActionIdentifier() const
    {
    FUNC_LOG;
    
    if( iIdentifier )
        {
        return *iIdentifier;
        }
    else
        {
        return KNullDesC;
        }
    }

//-----------------------------------------------------------------------------
// CCFActionSubscriptionImpl::ExternalizeL
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFActionSubscriptionImpl::ExternalizeL( RWriteStream& aStream )
    {
    FUNC_LOG;
    
    aStream.WriteInt16L( iIdentifier->Length() );
    aStream.WriteL( *iIdentifier, iIdentifier->Length() );

    // Commit stream
    aStream.CommitL();
    }

//-----------------------------------------------------------------------------
// CCFActionSubscriptionImpl::InternalizeL
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFActionSubscriptionImpl::InternalizeL( RReadStream& aStream )
    {
    FUNC_LOG;
    
    delete iIdentifier;
    iIdentifier = NULL;
    
    TInt length = aStream.ReadInt16L();
    if( length > 0 )
        {
        iIdentifier = HBufC::NewL( length );
        TPtr identifierPtr = iIdentifier->Des();
        aStream.ReadL( identifierPtr, length );
        }
    }

//-----------------------------------------------------------------------------
// CCFActionSubscriptionImpl::SetListener
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFActionSubscriptionImpl::SetListener(
    MCFActionSubscriptionListener* aListener )
    {
    FUNC_LOG;
    
    iListener = aListener;
    }

//-----------------------------------------------------------------------------
// CCFActionSubscriptionImpl::Listener
//-----------------------------------------------------------------------------
//
EXPORT_C MCFActionSubscriptionListener&
    CCFActionSubscriptionImpl::Listener() const
    {
    FUNC_LOG;
    
    return *iListener;
    }

//-----------------------------------------------------------------------------
// CCFActionSubscriptionImpl::Size
//-----------------------------------------------------------------------------
//
EXPORT_C TInt CCFActionSubscriptionImpl::Size() const
    {
    FUNC_LOG;
    
    TInt size = 0;
    
    // Action id length and data
    size += sizeof( TInt );
    size += iIdentifier->Size();
    
    return size;
    }
