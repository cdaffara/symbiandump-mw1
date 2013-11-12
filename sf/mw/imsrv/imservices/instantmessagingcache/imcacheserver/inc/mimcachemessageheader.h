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
* Description:   base call of header class
*
*/


#ifndef MIMCACHEMESSAGEHEADER_H
#define MIMCACHEMESSAGEHEADER_H

//INCLUDES

#include <e32std.h>
#include <e32base.h>

//FORWARD DECLARATIONS
class CIMCacheMessageBase;

// CLASS DECLARATION

/**
 *  Interface for header information of history file
 *  @lib imcacheserver.exe
 *  @since 5.0
 */
class MIMCacheMessageHeader
    {
	public:

		/**
		 * Destructor.
		 */
		virtual ~MIMCacheMessageHeader() {};

    public: // Interface
	
		/**
		 *	service id
		 *	@return a service id
		 */
		 virtual TInt ServiceId() const = 0;

		/**
		 *	user id of recipient 
		 *	@return recipient user id
		 */
		virtual const TDesC& BuddyId() const = 0;
		
		/**
		 *	return total message count
		 */
		virtual TInt MessageCount() const = 0 ;
		
		/**
		 * get message at given index 
		 * @param aIndex index
		 */
		virtual CIMCacheMessageBase* GetMessage(TInt aIndex )  = 0;
	
		/**
		 * append message
		 * @param aMessge message to append
		 */
        virtual void AppendMessageL(CIMCacheMessageBase* aMessge)  = 0 ; 
     
     	/**
		 * UnreadMessageCount 
		 * @return unread message count
		 */
        virtual TInt UnreadMessageCount() const = 0 ;  
        
         /**
        * check wheather chat started or not
        * @return ETrue started
        */
        virtual TBool IsChatStarted() = 0 ;
        
        /**
        * set chat started 
        * @param aStarted, a boolean value to set
         */
        virtual void SetChatStarted(TBool aStarted ) = 0 ;
       
      	/**
        * mark all message read
        */
        virtual void MarkAllRead() = 0 ;
             

	    };

#endif      // MIMCACHEMESSAGEHEADER_H

// End of File
