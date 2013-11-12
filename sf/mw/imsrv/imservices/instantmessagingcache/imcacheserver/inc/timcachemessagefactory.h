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


#ifndef TIMCHACHEMESSAGEFACTORY_H
#define TIMCHACHEMESSAGEFACTORY_H

//INCLUDES

#include <e32std.h>
#include "mimcachemessagefactory.h"

//  FORWARD DECLERATIONS

class MIMCacheMessageHeader;
class CIMCacheMessageBase;
// CLASS DECLARATION

/**
 *  Implementation for creator of MCAIMCacheMessages.
 *
 *  T-class, because there is no dynamic member variables and because
 *  there is no other class which suites in this case.
 *  Class methods cannot be static, because pointer to MIMCacheMessageFactory
 *  is passed to client.
 */
class TIMCacheMessageFactory : public MIMCacheMessageFactory
    {
    public: // Interface

        
        /**
		 *	Creates cahe message header
		 *	@since 5.0
		 *  @param aServiceId service Id
         *  @param aBuddyId sender.
         *	@return Instance to MIMCacheMessageHeader
		 */
        MIMCacheMessageHeader* CreateIMCacheMessageHeaderL(TInt aServiceId, 
        													const TDesC& aBuddyId );
        
         
         /**
         *  Creates cashe message
         *  @param aSender sender of message.
         *  @param aRecipient recipient of message.
         *  @param aData content data.
         *  @return Instance to CIMCacheMessageBase
         */
        CIMCacheMessageBase* CreateCacheMessageL(const TDesC& aData );
                                        
                                        
        /**
         *  Find Index
         *  @param aArray array
         *  @param aRecipient recipient of message.
         *  @param aServiceId service Id
         */
        TInt FindHeaderIndexL( RPointerArray<MIMCacheMessageHeader>& aArray, 
								TInt aServiceId,
		 						const TDesC& aBuddyId );
	                               
        
    };

#endif      // TIMCHACHEMESSAGEFACTORY_H

// End of File
