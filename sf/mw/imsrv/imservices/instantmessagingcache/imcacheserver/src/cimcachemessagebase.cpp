/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  a message class implementation 
*
*/


#include "cimcachemessagebase.h"
 //logs
#include "imcachedebugtrace.h"

	
//-----------------------------------------------------------------------------
// CIMCacheMessageBase::NewL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCacheMessageBase* CIMCacheMessageBase::NewL( 
                                        const TDesC& aData )
	{
	TRACE( T_LIT( "CIMCacheMessageBase::NewL begin") );
	CIMCacheMessageBase* self = new ( ELeave ) CIMCacheMessageBase();
	CleanupStack::PushL( self );
	self->ConstructL(aData );
	CleanupStack::Pop( self );
	TRACE( T_LIT( "CIMCacheMessageBase::NewL end") );
	return self;
	}
//-----------------------------------------------------------------------------
// CIMCacheMessageBase::CIMCacheMessageBase
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCacheMessageBase::CIMCacheMessageBase()
	{
	TRACE( T_LIT( "CIMCacheMessageBase::CIMCacheMessageBase") );
	}
	
	
//-----------------------------------------------------------------------------
// CIMCacheMessageBase::ConstructL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CIMCacheMessageBase::ConstructL(const TDesC& aData  )
    {
    TRACE( T_LIT( "CIMCacheMessageBase::NewL start") );
    iText =  aData.AllocL(); 
    iMessagerType = EIMCMessageOther;
    iTime.HomeTime();
    iMessageType = EIMCMessagePTOP;
    iUnread = ETrue;
    TRACE( T_LIT( "CIMCacheMessageBase::NewL end") );
    }

//-----------------------------------------------------------------------------
// CIMCacheMessageBase::~CIMCacheMessageBase
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCacheMessageBase::~CIMCacheMessageBase()
	{
	TRACE( T_LIT( "CIMCacheMessageBase::~CIMCacheMessageBase start") );
	delete iText;	
	TRACE( T_LIT( "CIMCacheMessageBase::~CIMCacheMessageBase end") );
	}
	
//-----------------------------------------------------------------------------
// CIMCacheMessageBase::TimeStamp
// ( Other items commented in header )
//-----------------------------------------------------------------------------
const TTime& CIMCacheMessageBase::TimeStamp() const
	{
	return iTime;
	}

//-----------------------------------------------------------------------------
// CIMCacheMessageBase::MessageType
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
TIMCacheMessageType CIMCacheMessageBase::MessageType() const
	{
	return iMessageType;
	}

//-----------------------------------------------------------------------------
// CIMCacheMessageBase::MessagerType
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
TIMCacheMessagerType CIMCacheMessageBase::MessagerType() const
	{
	return iMessagerType;
	}


//-----------------------------------------------------------------------------
// CIMCacheMessageBase::Text
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
const TDesC& CIMCacheMessageBase::Text() const
    {
    TRACE( T_LIT( "CIMCacheMessageBase::Text start") );
    if( iText )
        {
        return *iText;
        }
    TRACE( T_LIT( "CIMCacheMessageBase::Text end") );
    return KNullDesC;
    }
      
//-----------------------------------------------------------------------------
// CIMCacheMessageBase::SetRead
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
void CIMCacheMessageBase::SetRead() 
    {
    TRACE( T_LIT( "CIMCacheMessageBase::SetUnread start") );
    iUnread = EFalse;
    TRACE( T_LIT( "CIMCacheMessageBase::SetUnread end") );
    
    }

//-----------------------------------------------------------------------------
// CIMCacheMessageBase::IsUnread
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
TBool CIMCacheMessageBase::IsUnread() const
    {
    TRACE( T_LIT( "CIMCacheMessageBase::IsUnread ") );
    return iUnread;
   
    }     
// -----------------------------------------------------------------------------
// CIMCacheMessageBase::SetMessageType
// -----------------------------------------------------------------------------
//
void CIMCacheMessageBase::SetMessageType( TIMCacheMessageType aNewType )
    {
    TRACE( T_LIT( "CIMCacheMessageBase::SetMessageType start") );
    iMessageType = aNewType;
    TRACE( T_LIT( "CIMCacheMessageBase::SetMessageType end") );
    }

// -----------------------------------------------------------------------------
// CIMCacheMessageBase::SetMessagerType
// -----------------------------------------------------------------------------
//
void CIMCacheMessageBase::SetMessagerType( TIMCacheMessagerType aNewType )
    {
    TRACE( T_LIT( "CIMCacheMessageBase::SetMessagerType start") );
    iMessagerType = aNewType;
    TRACE( T_LIT( "CIMCacheMessageBase::SetMessagerType end") );
    }

 //-----------------------------------------------------------------------------
// CIMCacheMessageBase::ContentType
// ( Other items commented in header )
//-----------------------------------------------------------------------------	
TIMCacheContentType CIMCacheMessageBase::ContentType() const
	{
	return EIMCContentText;
	}   

    
// end of file
