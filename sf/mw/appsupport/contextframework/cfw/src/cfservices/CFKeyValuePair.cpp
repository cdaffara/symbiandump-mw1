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
* Description:  CCFKeyValuePair class implementation.
*
*/


#include "cfkeyvaluepair.h"
#include "cftrace.h"

// MEMBER FUNCTIONS

EXPORT_C CCFKeyValuePair* CCFKeyValuePair::NewL( const TDesC& aKey,
    const TDesC& aValue )
    {
    FUNC_LOG;
    
    CCFKeyValuePair* self = CCFKeyValuePair::NewLC( aKey, aValue );
    CleanupStack::Pop( self );

    return self;
    }

EXPORT_C CCFKeyValuePair* CCFKeyValuePair::NewLC( const TDesC& aKey,
    const TDesC& aValue )
    {
    FUNC_LOG;
    
    CCFKeyValuePair* self = new( ELeave ) CCFKeyValuePair;
    CleanupStack::PushL( self );
    self->ConstructL( aKey, aValue );

    return self;
    }
    
EXPORT_C CCFKeyValuePair* CCFKeyValuePair::NewL( HBufC* aKey, HBufC* aValue )
    {
    FUNC_LOG;
    
    CCFKeyValuePair* self = CCFKeyValuePair::NewLC( aKey, aValue );
    CleanupStack::Pop( self );

    return self;
    }

EXPORT_C CCFKeyValuePair* CCFKeyValuePair::NewLC( HBufC* aKey, HBufC* aValue )
    {
    FUNC_LOG;
    
    CCFKeyValuePair* self = new( ELeave ) CCFKeyValuePair( aKey, aValue );
    CleanupStack::PushL( self );

    return self;
    }
    
EXPORT_C CCFKeyValuePair::~CCFKeyValuePair()
    {
    FUNC_LOG;
    
    delete iKey;
    delete iValue;
    }
    
CCFKeyValuePair::CCFKeyValuePair()
    {
    FUNC_LOG;    
    }
    
CCFKeyValuePair::CCFKeyValuePair( HBufC* aKey, HBufC* aValue ):
    iKey( aKey ),
    iValue( aValue )
    {
    FUNC_LOG;    
    }
    
void CCFKeyValuePair::ConstructL( const TDesC& aKey,
    const TDesC& aValue )
    {
    FUNC_LOG;
    
    iKey = aKey.AllocL();
    iValue = aValue.AllocL();
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFKeyValuePair::Key
//-----------------------------------------------------------------------------
//
EXPORT_C const TDesC& CCFKeyValuePair::Key() const
    {
    FUNC_LOG;
    
    if( !iKey )
        {
        return KNullDesC;
        }
    else
        {
        return *iKey;
        }
    }

//-----------------------------------------------------------------------------
// CCFKeyValuePair::Value
//-----------------------------------------------------------------------------
//
EXPORT_C const TDesC& CCFKeyValuePair::Value() const
    {
    FUNC_LOG;
    
    if( !iValue ) 
        {
        return KNullDesC;
        }
    else
        {
        return *iValue;
        }
    }
