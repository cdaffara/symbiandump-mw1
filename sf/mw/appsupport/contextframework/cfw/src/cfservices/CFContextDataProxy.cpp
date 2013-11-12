/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFContextDataProxy class implementation.
*
*/


#include "CFContextDataProxy.h"

#include <s32mem.h>

#include "cftrace.h"

// MEMBER FUNCTIONS

EXPORT_C CCFContextDataProxy* CCFContextDataProxy::NewL()
    {
    FUNC_LOG;

    CCFContextDataProxy* self  = CCFContextDataProxy::NewLC();
    CleanupStack::Pop( self );
    
    return self;
    }

EXPORT_C CCFContextDataProxy* CCFContextDataProxy::NewLC()
    {
    FUNC_LOG;

    CCFContextDataProxy* self = new( ELeave ) CCFContextDataProxy;
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

CCFContextDataProxy::~CCFContextDataProxy()
    {
    FUNC_LOG;
    
    delete iData;
    }

CCFContextDataProxy::CCFContextDataProxy()
    {
    FUNC_LOG;
    }

void CCFContextDataProxy::ConstructL()
    {
    FUNC_LOG;
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFContextDataProxy::SetContextDataUid
//-----------------------------------------------------------------------------
// 
EXPORT_C void CCFContextDataProxy::SetContextDataUid( const TUid& aUid )
    {
    FUNC_LOG;
    
    iUid = aUid;
    }

//-----------------------------------------------------------------------------
// CCFContextDataProxy::SetContextData
//-----------------------------------------------------------------------------
// 
EXPORT_C void CCFContextDataProxy::SetContextData( HBufC8* aData )
    {
    FUNC_LOG;

    if( iData )
        {
        delete iData;
        iData = NULL;
        }
    iData = aData;    
    }

//-----------------------------------------------------------------------------
// CCFContextDataProxy::Uid
//-----------------------------------------------------------------------------
// 
const TUid& CCFContextDataProxy::Uid() const
    {
    FUNC_LOG;
    
    return iUid;
    }

//-----------------------------------------------------------------------------
// CCFContextDataProxy::ExternalizeL
//-----------------------------------------------------------------------------
// 
void CCFContextDataProxy::ExternalizeL( RWriteStream& aStream )
    {
    FUNC_LOG;
    
    if( iData )
        {
        TPtrC8 dataPtrC( *iData );
        aStream.WriteL( dataPtrC, dataPtrC.Length() );
        aStream.CommitL();
        }
    }

//-----------------------------------------------------------------------------
// CCFContextDataProxy::InternalizeL
//-----------------------------------------------------------------------------
// 
void CCFContextDataProxy::InternalizeL( RReadStream& aStream )
    {
    FUNC_LOG;

    TInt size = aStream.Source()->SizeL();
    if( size )
        {
        delete iData;
        iData = NULL;
        
        HBufC8* buf = HBufC8::NewLC( size );
        TPtr8 bufPtr( buf->Des() );
        aStream.ReadL( bufPtr, size );
        CleanupStack::Pop( buf );
        
        iData = buf;
        }
    }

//-----------------------------------------------------------------------------
// CCFContextDataProxy::Size
//-----------------------------------------------------------------------------
// 
TInt CCFContextDataProxy::Size() const
    {
    FUNC_LOG;
    
    TInt size = 0;
    if( iData )
        {
        size = iData->Size();
        }
        
    return size;
    }
