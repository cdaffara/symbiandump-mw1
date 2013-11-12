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
* Description:  factory helper class declaration  
*
*/


#ifndef __MIMCACHEMESSAGEFACTORY_H__
#define __MIMCACHEMESSAGEFACTORY_H__

//INCLUDES

#include <e32std.h>

//	FORWARD DECLERATIONS

class CIMCacheMessageBase;
class MIMCacheMessageHeader;

// CLASS DECLARATION

/**
 *  Interface for creator of MCAIMCacheMessages.
 *  imcacheserver.exe
 *  @since 5.0
 */
class MIMCacheMessageFactory
    {
	protected:

		/**
		 * Destructor.
		 */
		virtual ~MIMCacheMessageFactory() {};

    public: // Interface

	
		/**
		 *	Creates cahe message header
		 *	@since 5.0
		 *  @param aServiceId service Id
         *  @param aBuddyId sender.
         * 	@return Instance to MIMCacheMessageHeader
		 */
		virtual MIMCacheMessageHeader* CreateIMCacheMessageHeaderL(TInt aServiceId, const TDesC& aBuddyId ) = 0;
								 
	    /**
         *  Creates cashe message
         *  @param aSender sender of message.
         *  @param aRecipient recipient of message.
         *  @param aData content data.
         *  @return Instance to CIMCacheMessageBase
         */
         virtual CIMCacheMessageBase* CreateCacheMessageL(
                                        const TDesC& aData ) = 0;
                                        
	    /**
         *  Find Index
         *  @param aArray array
         *  @param aRecipient recipient of message.
         *  @param aServiceId service Id
         *  @param aSender sender.
         */
         virtual TInt FindHeaderIndexL( RPointerArray<MIMCacheMessageHeader>& aArray, 
										TInt aServiceId,
										const TDesC& aBuddyId ) = 0 ;
    };

#endif      // __MIMCACHEMESSAGEFACTORY_H__

// End of File