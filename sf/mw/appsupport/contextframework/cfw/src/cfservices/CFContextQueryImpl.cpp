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
* Description:  CCFContextQueryImpl class implementation.
*
*/


#include <s32strm.h>

#include "CFContextQueryImpl.h"
#include "cfserviceutils.h"
#include "cftrace.h"

// MEMBER FUNCTIONS

EXPORT_C CCFContextQueryImpl* CCFContextQueryImpl::NewL( const TDesC& aSource,
    const TDesC& aType )
    {
    FUNC_LOG;
    
    CCFContextQueryImpl* self = CCFContextQueryImpl::NewLC( aSource, aType );
    CleanupStack::Pop( self );
    
    return self;
    }

EXPORT_C CCFContextQueryImpl* CCFContextQueryImpl::NewLC( const TDesC& aSource,
    const TDesC& aType )
    {
    FUNC_LOG;
    
    CCFContextQueryImpl* self = new( ELeave ) CCFContextQueryImpl;
    CleanupStack::PushL( self );
    self->ConstructL( aSource, aType );
    
    return self;
    }
    
CCFContextQueryImpl::~CCFContextQueryImpl()
    {
    FUNC_LOG;
    
    delete iSource;
    delete iType;
    }

CCFContextQueryImpl::CCFContextQueryImpl()
    {
    FUNC_LOG;
    
    // Nothing to do
    }
    
void CCFContextQueryImpl::ConstructL( const TDesC& aSource,
    const TDesC& aType )
    {
    FUNC_LOG;
    
    // Initialize members
    iSource = aSource.AllocL();
    iType = aType.AllocL();
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFContextQueryImpl::SetTypeL
//-----------------------------------------------------------------------------
// 
void CCFContextQueryImpl::SetTypeL( const TDesC& aType )
    {
    FUNC_LOG;
    
    CFServiceUtils::UpdateBufferL( iType, aType );
    }

//-----------------------------------------------------------------------------
// CCFContextQueryImpl::Type
//-----------------------------------------------------------------------------
// 
const TDesC& CCFContextQueryImpl::Type() const
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
// CCFContextQueryImpl::SetSourceL
//-----------------------------------------------------------------------------
// 
void CCFContextQueryImpl::SetSourceL( const TDesC& aSource )
    {
    FUNC_LOG;

    CFServiceUtils::UpdateBufferL( iSource, aSource );
    }

//-----------------------------------------------------------------------------
// CCFContextQueryImpl::Source
//-----------------------------------------------------------------------------
// 
const TDesC& CCFContextQueryImpl::Source() const
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
// CCFContextQueryImpl::SubTypeMatch
//-----------------------------------------------------------------------------
//
TBool CCFContextQueryImpl::SubTypeMatch() const
	{
	FUNC_LOG;
	
	return iMatchSubTypes;
	}
		
//-----------------------------------------------------------------------------
// CCFContextQueryImpl::SetTypeMatch
//-----------------------------------------------------------------------------
//
void CCFContextQueryImpl::SetSubTypeMatch( TBool aMatchSubTypes )
    {
	FUNC_LOG;
	
	iMatchSubTypes = aMatchSubTypes;    	
    }

//-----------------------------------------------------------------------------
// CCFContextQueryImpl::Matches
//-----------------------------------------------------------------------------
// 
EXPORT_C TBool CCFContextQueryImpl::MatchesQuery(
    const CCFContextQuery& aQuery,
	const TDesC& aContextType,
	const TDesC& aContextSource,
	TBool aMatchSubTypes )
    {
    FUNC_LOG;

	// Source must be always completely the same
    if( aQuery.Source().Compare( aContextSource ) == KErrNone )
    	{
    	if ( aMatchSubTypes )
    		{
    		// beginning of the type must be same
    		if( aQuery.Type().Compare(
        	aContextType.Left( aQuery.Type().Length() ) ) == KErrNone )
        		{
            	return ETrue;
            	}
    		}
    	else
    		{
    		// type must be completely same
    		if( aQuery.Type().Compare( aContextType ) == KErrNone )
        		{
            	return ETrue;
            	}	
    		}
        }
    return EFalse;
    }

//-----------------------------------------------------------------------------
// CCFContextQueryImpl::Matches
//-----------------------------------------------------------------------------
// 
EXPORT_C TBool CCFContextQueryImpl::Matches( const TDesC& aContextType, 
    const TDesC& aContextSource ) const
    {
    FUNC_LOG;

	return MatchesQuery( *this, aContextType, aContextSource, iMatchSubTypes );
    }


//-----------------------------------------------------------------------------
// CCFContextQueryImpl::IsSame
//-----------------------------------------------------------------------------
// 
EXPORT_C TBool CCFContextQueryImpl::IsSame(
	const CCFContextQuery& aQuery1,
	const CCFContextQuery& aQuery2)
    {
    FUNC_LOG;

    if( aQuery1.Type().Compare( aQuery2.Type() ) == KErrNone &&
        aQuery1.Source().Compare( aQuery2.Source() ) == KErrNone )
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

//-----------------------------------------------------------------------------
// CCFContextQueryImpl::InternalizeL
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFContextQueryImpl::InternalizeL( RReadStream& aStream )
    {
    FUNC_LOG;
    
    // Source
    CFServiceUtils::ReadFromStreamL( iSource, aStream );
    
    // Type
    CFServiceUtils::ReadFromStreamL( iType, aStream );
    
    // Sub type match flag
    iMatchSubTypes = aStream.ReadInt16L();
    }

//-----------------------------------------------------------------------------
// CCFContextQueryImpl::ExternalizeL
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFContextQueryImpl::ExternalizeL( RWriteStream& aStream )
    {
    FUNC_LOG;

    // Source
    CFServiceUtils::WriteIntoStreamL( iSource, aStream );
    
    // Type
    CFServiceUtils::WriteIntoStreamL( iType, aStream );
    
    // Sub type match flag
    aStream.WriteInt16L( iMatchSubTypes );
    
    // Commit stream
    aStream.CommitL();
    }

//-----------------------------------------------------------------------------
// CCFContextQueryImpl::Size
//-----------------------------------------------------------------------------
//
EXPORT_C TInt CCFContextQueryImpl::Size() const
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
    
    // Sub type match flag
    size += sizeof( iMatchSubTypes );
    
    return size;
    }
