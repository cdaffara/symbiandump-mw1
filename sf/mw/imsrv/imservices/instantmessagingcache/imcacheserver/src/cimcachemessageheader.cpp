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
* Description:  implementation of header of all convesation
*
*/


//  Include Files

#include "cimcachemessageheader.h"

#include "cimcachemessagebase.h"
 //logs
#include "imcachedebugtrace.h"


// CONSTRUCTION
// -----------------------------------------------------------------------------
// CIMCacheMessageHeader::NewL
// Static constructor
// -----------------------------------------------------------------------------
//
CIMCacheMessageHeader* CIMCacheMessageHeader::NewL(TInt aServiceId, const TDesC& aBuddyId )
    {
    TRACE( T_LIT( "CIMCacheMessageHeader::NewL begin") );
    CIMCacheMessageHeader* self = new (ELeave) CIMCacheMessageHeader;
    CleanupStack::PushL(self);
    self->ConstructL( aServiceId, aBuddyId );
	CleanupStack::Pop();
	TRACE( T_LIT( "CIMCacheMessageHeader::NewL end") );
    return self;
    }


// -----------------------------------------------------------------------------
// CIMCacheMessageHeader::~CIMCacheMessageHeader
// Destructor (virtual by CBase)
// -----------------------------------------------------------------------------
//
CIMCacheMessageHeader::~CIMCacheMessageHeader()
    {
    TRACE( T_LIT( "CIMCacheMessageHeader::~CIMCacheMessageHeader begin") );
	delete iBuddyId;
	iMessageArray.ResetAndDestroy() ; // all messages
	TRACE( T_LIT( "CIMCacheMessageHeader::~CIMCacheMessageHeader end") );
    }

// -----------------------------------------------------------------------------
// CIMCacheMessageHeader::ConstructL
// Second phase construct
// -----------------------------------------------------------------------------
//
void CIMCacheMessageHeader::ConstructL(TInt aServiceId, const TDesC& aBuddyId )
    {
    TRACE( T_LIT( "CIMCacheMessageHeader::ConstructL begin") );
	iServiceId = aServiceId;
	iBuddyId   = aBuddyId.AllocL();
    TRACE( T_LIT( "CIMCacheMessageHeader::ConstructL end") );
    }

// -----------------------------------------------------------------------------
// CIMCacheMessageHeader::CIMCacheMessageHeader
// Constructor
// -----------------------------------------------------------------------------
//
CIMCacheMessageHeader::CIMCacheMessageHeader():
    iChatStarted( EFalse)
    {
    TRACE( T_LIT( "CIMCacheMessageHeader::CIMCacheMessageHeader") );
    }


// -----------------------------------------------------------------------------
// CIMCacheMessageHeader::Recipient
// name of history data
// -----------------------------------------------------------------------------
//
const TDesC& CIMCacheMessageHeader::BuddyId() const
	{
	TRACE( T_LIT( "CIMCacheMessageHeader::Recipient begin") );
	if( iBuddyId )
        {
        return *iBuddyId;
        }
    TRACE( T_LIT( "CIMCacheMessageHeader::Recipient end") );
    return KNullDesC;
	
	}

// -----------------------------------------------------------------------------
// CIMCacheMessageHeader::ServiceId
// name of history data
// -----------------------------------------------------------------------------
//
 TInt CIMCacheMessageHeader::ServiceId() const
	{
	return iServiceId;
	}
	
// -----------------------------------------------------------------------------
// CIMCacheMessageHeader::MessageCount
// name of history data
// -----------------------------------------------------------------------------
//
 TInt CIMCacheMessageHeader::MessageCount() const
	{
	return iMessageArray.Count();
	}
// -----------------------------------------------------------------------------
// CIMCacheMessageHeader::GetMessage
// name of history data
// -----------------------------------------------------------------------------
//
 CIMCacheMessageBase* CIMCacheMessageHeader::GetMessage(TInt aIndex ) 
	{
	return iMessageArray[aIndex];
	}

// -----------------------------------------------------------------------------
// CIMCacheMessageHeader::AppendMessageL
// name of history data
// -----------------------------------------------------------------------------
//
void CIMCacheMessageHeader::AppendMessageL(CIMCacheMessageBase* aMessge) 
	{
	TRACE( T_LIT( "CIMCacheMessageHeader::AppendMessageL begin") );

    iMessageArray.AppendL( aMessge );
   	
	TRACE( T_LIT( "CIMCacheMessageHeader::AppendMessageL end") );
	}

// -----------------------------------------------------------------------------
// CIMCacheMessageHeader::UnreadMessageCount
// name of history data
// -----------------------------------------------------------------------------
//
TInt CIMCacheMessageHeader::UnreadMessageCount()  const
	{
	TRACE( T_LIT( "CIMCacheMessageHeader::UnreadMessageCount begin") );
	TInt ret = 0;
	TInt msgcount = iMessageArray.Count();
	for(TInt i=0; i<msgcount; i++ )
		{
		CIMCacheMessageBase* msg = 	iMessageArray[i];
		if(msg->IsUnread() )
			{
			ret++;	
			}
		}
	TRACE( T_LIT( "CIMCacheMessageHeader::UnreadMessageCount end") );
	return ret;
	}

// -----------------------------------------------------------------------------
// CIMCacheMessageHeader::IsChatStarted
// name of history data
//-----------------------------------------------------------------------------
TBool CIMCacheMessageHeader::IsChatStarted()
    {
    return iChatStarted;
    }

// -----------------------------------------------------------------------------
// CIMCacheMessageHeader::SetChatStarted
// name of history data
//-----------------------------------------------------------------------------
void CIMCacheMessageHeader::SetChatStarted(TBool aStarted )
    {
    iChatStarted = aStarted;
    }

 // -----------------------------------------------------------------------------
// CIMCacheMessageHeader::MarkAllRead
// name of history data
//-----------------------------------------------------------------------------
void CIMCacheMessageHeader::MarkAllRead()
    {
    TInt unreadCount = UnreadMessageCount();
    TInt count = MessageCount();
    
    for( TInt i = count-1; i >= count- unreadCount; i--  )
	    {
	    iMessageArray[i]->SetRead();	
	    }
    }
 
//  End of File
