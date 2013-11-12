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
* Description:   a header of all convesation
*
*/


#ifndef CIMCACHEMESSAGEHEADER_H
#define CIMCACHEMESSAGEHEADER_H

#include <e32base.h>
#include "mimcachemessageheader.h"

// CLASS DECLARATION

/**
 *  Message header for history data
 *
 *  @lib imcacheserver.exe
 *  @since 5.0
 */
 
class CIMCacheMessageHeader : public CBase, 
							  public MIMCacheMessageHeader
    {
    public: // Constructors and destructors

		/**
		 * NewL for CIMCacheMessageHeader
         * Two-phase constructor
		 * @param aServiceId , a service id .
		 * @param aBuddyId a buddy. 
		 * @return Instance of CIMCacheMessageHeader with header information.
		 */        
		static CIMCacheMessageHeader* NewL(TInt aServiceId, const TDesC& aBuddyId );

		/**
		 * Virtual destructor	
		 */
        virtual ~CIMCacheMessageHeader();

    private:	// Constructors

		/**
		 * Default constructor	
		 */
        CIMCacheMessageHeader();

		/**
		 * ConstructL for CIMCacheMessageHeader
		 * Part of Two-phase construction
         * @param aServiceId , a service id .
		 * @param aSender sender 
		 * @param aBuddyId a recipient 
		 */
        void ConstructL(TInt aServiceId, const TDesC& aBuddyId );
    

	public: //new MIMCacheMessageHeader

		/**
		 *	service id
		 *	@return a service id
		 */
		 TInt ServiceId() const;
		
		/**
		 *	user id of recipient 
		 *	@return recipient user id
		 */
		const TDesC& BuddyId() const;
		
		/**
		 *	return total message count
		 */
		TInt MessageCount() const ;

		/**
		 * get message at given index 
		 * @param aIndex index
		 */
		CIMCacheMessageBase* GetMessage(TInt aIndex ) ;

		/**
		 * append message
		 * @param aMessge message to append
		 */
		void AppendMessageL(CIMCacheMessageBase* aMessge)  ; 	
		
		/**
		 * UnreadMessageCount 
		 * @return unread message count
		 */
        TInt UnreadMessageCount() const ; 
        
        /**
        * check wheather chat started or not
        * @return ETrue started
        */
        TBool IsChatStarted();
        
		/**
        * set chat started 
        * @param aStarted, a boolean value to set
         */
        void SetChatStarted(TBool aStarted );
       
      	/**
        * mark all message read
        */
        void MarkAllRead();
        
    private:  // Data
	
        //owned, recipient
		HBufC* iBuddyId;
		
		//service id
		TInt iServiceId ;
		
		//owned,  array of all messages
		RPointerArray< CIMCacheMessageBase > iMessageArray;
		
		//true if streaming is not completely done.
		TBool iChatStarted;
			
	};

#endif      // CIMCACHEMESSAGEHEADER_H

// End of File
