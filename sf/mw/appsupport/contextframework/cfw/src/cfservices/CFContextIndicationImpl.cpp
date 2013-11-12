/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFContextIndicationImpl class implementation.
*
*/


#include <s32strm.h>
#include <s32mem.h>

#include <cfcontextdataobject.h>

#include "CFContextIndicationImpl.h"
#include "CFContextObjectImpl.h"
#include "cftrace.h"

EXPORT_C CCFContextIndicationImpl* CCFContextIndicationImpl::NewL(
    const TUid& aUid )
    {
    FUNC_LOG;
    
    CCFContextIndicationImpl* self =
        CCFContextIndicationImpl::NewLC( aUid );
    CleanupStack::Pop( self );
    
    return self;
    }

EXPORT_C CCFContextIndicationImpl* CCFContextIndicationImpl::NewLC(
    const TUid& aUid )
    {
    FUNC_LOG;
    
    CCFContextIndicationImpl* self =
        new( ELeave ) CCFContextIndicationImpl( aUid );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

EXPORT_C CCFContextIndicationImpl* CCFContextIndicationImpl::NewL(
    RReadStream& aStream )
    {
    FUNC_LOG;
    
    CCFContextIndicationImpl* self =
        CCFContextIndicationImpl::NewLC( aStream );
    CleanupStack::Pop( self );
    
    return self;
    }

EXPORT_C CCFContextIndicationImpl* CCFContextIndicationImpl::NewLC(
    RReadStream& aStream)
    {
    FUNC_LOG;
    
    CCFContextIndicationImpl* self =
        new( ELeave ) CCFContextIndicationImpl( KNullUid );
    CleanupStack::PushL( self );
    self->ConstructL();
    self->InternalizeL( aStream );
    
    return self;
    }

CCFContextIndicationImpl::~CCFContextIndicationImpl()
    {
    FUNC_LOG;
    
    delete iContext;
    delete iData;
    }

CCFContextIndicationImpl::CCFContextIndicationImpl( const TUid& aUid ):
    iUid( aUid )
    {
    FUNC_LOG;    
    }

void CCFContextIndicationImpl::ConstructL()
    {
    FUNC_LOG;
    
    iContext = CCFContextObjectImpl::NewL();
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFContextIndicationImpl::Context
//-----------------------------------------------------------------------------
//
const CCFContextObject& CCFContextIndicationImpl::Context() const
    {
    FUNC_LOG;
    
    return *iContext;
    }

//-----------------------------------------------------------------------------
// CCFContextIndicationImpl::Uid
//-----------------------------------------------------------------------------
//
const TUid& CCFContextIndicationImpl::Uid() const
    {
    FUNC_LOG;
    
    return iUid;
    }

//-----------------------------------------------------------------------------
// CCFContextIndicationImpl::CreateDataObjectL
//-----------------------------------------------------------------------------
//
void CCFContextIndicationImpl::CreateDataObjectL(
    CCFContextDataObject& aDataObject )
    {
    FUNC_LOG;

    if( iUid != KNullUid && iData )
        {
        RDesReadStream readStream( *iData );
        readStream.PushL();
        aDataObject.InternalizeL( readStream );
        
        // Cleanup
        CleanupStack::PopAndDestroy( &readStream );
        }
    else
        {
        // Leave since client has requested the operation incorrectly.
        User::Leave( KErrNotFound );
        }
    }

//-----------------------------------------------------------------------------
// CCFContextIndicationImpl::ExternalizeL
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFContextIndicationImpl::ExternalizeL( RWriteStream& aStream )
    {
    FUNC_LOG;
    
    // Write context object
    iContext->ExternalizeL( aStream );
    
    // Write data object
    TPtrC8 data;
    TInt length = 0;
    if( iData )
        {
        data.Set( *iData );
        length = data.Length();
        }
    aStream.WriteInt32L( iUid.iUid );
    aStream.WriteInt16L( length );
    if( length )
        {
        aStream.WriteL( data, length );
        }
        
    // Commit data in the stream
    aStream.CommitL();
    }

//-----------------------------------------------------------------------------
// CCFContextIndicationImpl::InternalizeL
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFContextIndicationImpl::InternalizeL( RReadStream& aStream )
    {
    FUNC_LOG;
    
    // Read context object
    iContext->InternalizeL( aStream );
    
    // Read data object
    iUid.iUid = aStream.ReadInt32L();
    TInt length = aStream.ReadInt16L();
    if( length )
        {
        HBufC8* data = HBufC8::NewLC( length );
        TPtr8 dataPtr = data->Des();
        aStream.ReadL( dataPtr, length );
        CleanupStack::Pop( data );
        iData = data;
        }
    }

//-----------------------------------------------------------------------------
// CCFContextIndicationImpl::Size
//-----------------------------------------------------------------------------
//
EXPORT_C TInt CCFContextIndicationImpl::Size() const
    {
    FUNC_LOG;
    
    TInt size = 0;
    
    // Context object
    size += iContext->Size();
    
    // UID size
    size += sizeof( TInt32 );
    
    // Length of the data
    size += sizeof( TInt );
    if( iData )
        {
        size += iData->Size();
        }
    
    return size;
    }

//-----------------------------------------------------------------------------
// CCFContextIndicationImpl::SetContextL
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFContextIndicationImpl::SetContextL(
    const CCFContextObject& aContext )
    {
    FUNC_LOG;
    
    iContext->CopyL( aContext );
    }

//-----------------------------------------------------------------------------
// CCFContextIndicationImpl::SetDataObjectUid
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFContextIndicationImpl::SetDataObjectUid( const TUid& aUid )
    {
    FUNC_LOG;
    
    iUid = aUid;
    }

//-----------------------------------------------------------------------------
// CCFContextIndicationImpl::SetDataObject
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFContextIndicationImpl::SetDataObject( HBufC8* aData )
    {
    FUNC_LOG;
    
    if( iData )
        {
        delete iData;
        iData = NULL;
        }
    iData = aData;
    }
