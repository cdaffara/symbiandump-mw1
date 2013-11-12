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
* Description:  CCFContextObjectImpl class description.
*
*/


#include <s32strm.h>

#include "CFContextObjectImpl.h"
#include "cfserviceutils.h"
#include "cftrace.h"

static const TInt KTimesSame = 0;
static const TInt KTimeFirstGreater = -1;
static const TInt KTimeSecondGreater = 1;

static const TUint KFullConfidence = 100;

// MEMBER FUNCTIONS

EXPORT_C CCFContextObjectImpl* CCFContextObjectImpl::NewL()
    {
    FUNC_LOG;
    
    CCFContextObjectImpl* self = CCFContextObjectImpl::NewLC();
    CleanupStack::Pop( self );
    
    return self;
    }
    
EXPORT_C CCFContextObjectImpl* CCFContextObjectImpl::NewLC()
    {
    FUNC_LOG;
    
    CCFContextObjectImpl* self = new( ELeave ) CCFContextObjectImpl;
    CleanupStack::PushL( self );
    self->ConstructL( KNullDesC, KNullDesC, KNullDesC );
    
    return self;
    }
    
EXPORT_C CCFContextObjectImpl* CCFContextObjectImpl::NewL( const TDesC& aSource,
    const TDesC& aType,
    const TDesC& aValue )
    {
    FUNC_LOG;
    
    CCFContextObjectImpl* self =
        CCFContextObjectImpl::NewLC( aSource, aType, aValue );
    CleanupStack::Pop( self );
    
    return self;
    }
    
EXPORT_C CCFContextObjectImpl* CCFContextObjectImpl::NewLC( const TDesC& aSource,
    const TDesC& aType,
    const TDesC& aValue )
    {
    FUNC_LOG;
    
    CCFContextObjectImpl* self = new( ELeave ) CCFContextObjectImpl;
    CleanupStack::PushL( self );
    self->ConstructL( aSource, aType, aValue );
    
    return self;
    }
    
CCFContextObjectImpl::~CCFContextObjectImpl()
    {
    FUNC_LOG;
    
    delete iSource;
    delete iType;
    delete iValue;
    }
    
CCFContextObjectImpl::CCFContextObjectImpl():
    iConfidence( KFullConfidence )
    {
    FUNC_LOG;
    
    // Nothing to do
    }
    
void CCFContextObjectImpl::ConstructL( const TDesC& aSource,
    const TDesC& aType,
    const TDesC& aValue )
    {
    FUNC_LOG;
    
    // Initialize pointers
    iSource = aSource.AllocL();
    iType = aType.AllocL();
    iValue = aValue.AllocL();
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::SetTypeL
//-----------------------------------------------------------------------------
//
void CCFContextObjectImpl::SetTypeL( const TDesC& aType )
    {
    FUNC_LOG;

    CFServiceUtils::UpdateBufferL( iType, aType );
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::Type
//-----------------------------------------------------------------------------
//
const TDesC& CCFContextObjectImpl::Type() const
    {
    FUNC_LOG;
    
    if( iType )
        {
        return *iType;
        }
    else
        {
        return KNullDesC;
        }
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::SetValueL
//-----------------------------------------------------------------------------
//
void CCFContextObjectImpl::SetValueL( const TDesC& aValue )
    {
    FUNC_LOG;

    CFServiceUtils::UpdateBufferL( iValue, aValue );
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::Value
//-----------------------------------------------------------------------------
//
const TDesC& CCFContextObjectImpl::Value() const
    {
    FUNC_LOG;
    
    if( iValue )
        {
        return *iValue;
        }
    else
        {
        return KNullDesC;
        }
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::SetSourceL
//-----------------------------------------------------------------------------
//
void CCFContextObjectImpl::SetSourceL( const TDesC& aSource )
    {
    FUNC_LOG;

    CFServiceUtils::UpdateBufferL( iSource, aSource );
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::Source
//-----------------------------------------------------------------------------
//
const TDesC& CCFContextObjectImpl::Source() const
    {
    FUNC_LOG;

    if( iSource )
        {
        return *iSource;
        }
    else
        {
        return KNullDesC;
        }
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::SetConfidence
//-----------------------------------------------------------------------------
//
void CCFContextObjectImpl::SetConfidence( const TUint8& aConfidence )
    {
    FUNC_LOG;

    iConfidence = aConfidence;
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::Confidence
//-----------------------------------------------------------------------------
//
TUint8 CCFContextObjectImpl::Confidence() const
    {
    FUNC_LOG;
    
    return iConfidence;
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::SetTimestampToHomeTime
//-----------------------------------------------------------------------------
//
void CCFContextObjectImpl::SetTimestampToHomeTime()
    {
    FUNC_LOG;
    
    iTimestamp.HomeTime();
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::SetTimestamp
//-----------------------------------------------------------------------------
//
void CCFContextObjectImpl::SetTimestamp( const TTime& aTimestamp )
    {
    FUNC_LOG;
    
    iTimestamp = aTimestamp;
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::SetTimestamp
//-----------------------------------------------------------------------------
//
void CCFContextObjectImpl::SetTimestamp( const TInt64& aTimestamp )
    {
    FUNC_LOG;

    iTimestamp = aTimestamp;
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::Timestamp
//-----------------------------------------------------------------------------
//
const TTime& CCFContextObjectImpl::Timestamp() const
    {
    FUNC_LOG;
    
    return iTimestamp;
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::CopyL
//-----------------------------------------------------------------------------
//
void CCFContextObjectImpl::CopyL( const CCFContextObject& aContext )
    {
    FUNC_LOG;

    SetSourceL( aContext.Source() );
    SetTypeL( aContext.Type() );
    SetValueL( aContext.Value() );
    SetTimestamp( aContext.Timestamp() );
    SetConfidence( aContext.Confidence() );
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::InternalizeL
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFContextObjectImpl::InternalizeL( RReadStream& aStream )
    {
    FUNC_LOG;
    
    // Source
    CFServiceUtils::ReadFromStreamL( iSource, aStream );
    
    // Type
    CFServiceUtils::ReadFromStreamL( iType, aStream );
    
    // Value
    CFServiceUtils::ReadFromStreamL( iValue, aStream );
    
    // Timestamp
    TPckg<TTime> timeStamp( iTimestamp );
    aStream.ReadL( timeStamp, sizeof( TTime ) );
    
    // Confidence
    iConfidence = aStream.ReadUint16L();
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::ExternalizeL
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFContextObjectImpl::ExternalizeL( RWriteStream& aStream )
    {
    FUNC_LOG;
    
    // Source
    CFServiceUtils::WriteIntoStreamL( iSource, aStream );
    
    // Type
    CFServiceUtils::WriteIntoStreamL( iType, aStream );
    
    // Value
    CFServiceUtils::WriteIntoStreamL( iValue, aStream );
    
    // Timestamp
    TPckg<TTime> timestamp( iTimestamp );
    aStream.WriteL( timestamp, sizeof( TTime ) );
    
    // Confidence
    aStream.WriteUint16L( iConfidence );

    // Commit stream
    aStream.CommitL();
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::Size
//-----------------------------------------------------------------------------
//
EXPORT_C TInt CCFContextObjectImpl::Size() const
    {
    FUNC_LOG;
    
    TInt size = 0;

    // Source
    size += sizeof( TInt );
    if( iSource )
        {
        size += ( *iSource ).Size();
        }
    
    // Type
    size += sizeof( TInt );
    if( iType )
        {
        size += ( *iType ).Size();
        }
    
    // Value
    size += sizeof( TInt );
    if( iValue )
        {
        size += ( *iValue ).Size();
        }
    
    // Timestamp
    size += sizeof( TTime );
    
    // Confidence
    size += sizeof( TUint );

    return size;
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::CompareValue
//-----------------------------------------------------------------------------
//
EXPORT_C TInt CCFContextObjectImpl::CompareValue(
    const CCFContextObject& aContext ) const
    {
    FUNC_LOG;
    
    return Value().Compare( aContext.Value() );
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::CompareByType
//-----------------------------------------------------------------------------
//
EXPORT_C TInt CCFContextObjectImpl::CompareByType(
    const CCFContextObject& aFirst,
    const CCFContextObject& aSecond )
    {
    FUNC_LOG;
    
    return aFirst.Type().Compare( aSecond.Type() );
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::CompareByTypeAndSource
//-----------------------------------------------------------------------------
//
EXPORT_C TInt CCFContextObjectImpl::CompareByTypeAndSource(
    const CCFContextObject& aFirst,
    const CCFContextObject& aSecond )
    {
    FUNC_LOG;
    
    TInt typeCompareResult = aFirst.Type().Compare( aSecond.Type() );
    if( typeCompareResult != 0 )
        {
        return typeCompareResult;
        }
    else
        {
        return aFirst.Source().Compare( aSecond.Source() );
        }
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::CompareByTimeDescending
//-----------------------------------------------------------------------------
//
EXPORT_C TInt CCFContextObjectImpl::CompareByTimeDescending( 
    const CCFContextObject& aFirst,
    const CCFContextObject& aSecond )
    {
    FUNC_LOG;
    
    TInt retVal = KTimeSecondGreater;

    // Both same
    if( aFirst.Timestamp() == aSecond.Timestamp() )
        {
        retVal = KTimesSame;
        }
    // First greater than second
    else if( aFirst.Timestamp() > aSecond.Timestamp() )
        {
        retVal = KTimeFirstGreater;
        }

    return retVal;
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl::IsSame
//-----------------------------------------------------------------------------
//
EXPORT_C TBool CCFContextObjectImpl::IsSame(
    const CCFContextObject& aFirst,
    const CCFContextObject& aSecond )
    {
    FUNC_LOG;
    
    if( aFirst.Timestamp() == aSecond.Timestamp() )
        {
        if( aFirst.Type().Compare( aSecond.Type() ) == KErrNone )
            {
            if( aFirst.Value().Compare( aSecond.Value() ) == KErrNone )
                {
                if( aFirst.Source().Compare( aSecond.Source() ) == KErrNone )
                    {
                    if( aFirst.Confidence() == aSecond.Confidence() )
                        {
                        return ETrue;
                        }
                    }
                }
            }
        }
    return EFalse;
    }
