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
* Description:  a message class declaration 
*
*/


#ifndef CIMIMCACHEMESSAGEBASE_H
#define CIMIMCACHEMESSAGEBASE_H

#include <e32base.h>
#include <badesca.h>
#include <imcachedefs.h>

//	FORWARD CLASS DECLERATIONS
// CLASS DECLARATION
// Definitions

/**
 *  Message Base class
 */
class CIMCacheMessageBase : public CBase
	{
                      	
    public: // Construction
    
    	
		/**
		 * Construction
		  * @param aData, text message
		 * @return Pointer to new created instance of CIMCacheMessageBase
		 */
		static CIMCacheMessageBase* NewL( 
		                             const TDesC& aData );

		
		/**
		 * Destruction
		 */
		~CIMCacheMessageBase();
		
	private : // Construction
	
		/**
		 * Constructor
		 */
		CIMCacheMessageBase();
		
		/**
		 * Remember to call this one as last line of child ConstructL
		 * @param aSender, data 
		  */
		void ConstructL( const TDesC& aData  );
		

public:  
		/**
		 * @see TimeStamp
		* @return a time stamp
		 */
		const TTime& TimeStamp() const;

		/**
		 * @see MessageType
		 * @return a message type
		 */
		TIMCacheMessageType MessageType() const;
		
		/**
		 * @see MessagerType
		 * @return a messager type
		 */
		TIMCacheMessagerType MessagerType() const;
		
	             
		/**
		 * @see Text
		 * @return text message
		 */
        const TDesC& Text() const;
       	   		
				
		/**
		 * @see ContentType
		 * @return type of content
		 */
		TIMCacheContentType ContentType() const;
		
	        
        /**
         * Set message type
         * @param aNewType New message type.
         */
        void SetMessageType( TIMCacheMessageType aNewType );
        
        /**
         * Set messager type
         * @param aNewType New messager type.
         */
        void SetMessagerType( TIMCacheMessagerType aNewType );

		/**
         * Set messager read
         */
		void SetRead() ;
  
		/**
         * IsUnread
         * @return  ETrue if messgae is unread.
         */
		TBool IsUnread() const ;
	                 
                                            
	protected: // Member variables
	
	    /// Type of message
	    TIMCacheMessageType iMessageType;
	    // messager type
	    TIMCacheMessagerType iMessagerType;
	   
		/// Timestamp.
 		TTime iTime;
 	    /// Owns. Textual content.
	    HBufC* iText;
	    
	    // owned, message is unread
	    TBool iUnread;
       
	}; 

#endif      // CIMIMCACHEMESSAGEBASE_H

// End of File
