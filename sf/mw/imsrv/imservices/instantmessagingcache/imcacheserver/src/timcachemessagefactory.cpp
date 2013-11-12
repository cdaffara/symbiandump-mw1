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
* Description:  factory helper class implementation  
*
*/


// INCLUDE FILES

#include "timcachemessagefactory.h"
#include "cimcachemessageheader.h"
#include "cimcachemessagebase.h"
 //logs
#include "imcachedebugtrace.h"


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// TIMCacheMessageFactory::CreateIMCacheMessageHeaderL
// Create instantmessagingcache message header
// -----------------------------------------------------------------------------
//
MIMCacheMessageHeader* TIMCacheMessageFactory::CreateIMCacheMessageHeaderL(TInt aServiceId, const TDesC& aBuddyId )
    {
    TRACE( T_LIT( "TIMCacheMessageFactory::CreateIMCacheMessageHeaderL") );
    return CIMCacheMessageHeader::NewL( aServiceId, aBuddyId );
    }


// -----------------------------------------------------------------------------
// TIMCacheMessageFactory::CreateCacheMessageL
// Create instantmessagingcache message header
// -----------------------------------------------------------------------------
//
CIMCacheMessageBase* TIMCacheMessageFactory::CreateCacheMessageL( 
                                        const TDesC& aData )
	{
	TRACE( T_LIT( "TIMCacheMessageFactory::CreateCacheMessageL") );
	return CIMCacheMessageBase::NewL(aData)	;
	}
  
 // -----------------------------------------------------------------------------
// TIMCacheMessageFactory::FindHeaderIndexL
// Create instantmessagingcache message header
// -----------------------------------------------------------------------------
//
TInt TIMCacheMessageFactory::FindHeaderIndexL(RPointerArray<MIMCacheMessageHeader>& aArray, 
												TInt aServiceId,
												const TDesC& aBuddyId )
	{
	TRACE( T_LIT( "TIMCacheMessageFactory::FindHeaderIndexL begin") );
	TInt count = aArray.Count();
	TRACE( T_LIT( "TIMCacheMessageFactory::FindHeaderIndexL count = %d"),count );
	
	for(TInt index = 0; index <count ; index++ )
		{
		MIMCacheMessageHeader* header = aArray[index];
		if(header )
			{
			if( ( aServiceId == header->ServiceId() ) && ( aBuddyId.CompareC( header->BuddyId() ) == 0  ) )	
				{
				return index;	
				}				
			}	
		}
		
	TRACE( T_LIT( "TIMCacheMessageFactory::FindHeaderIndexL end") );		
	return KErrNotFound;
	}
                     
// end of file

