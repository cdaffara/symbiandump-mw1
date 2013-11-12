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


#ifndef CIMIMCACHEBUFFERARRAY_H
#define CIMIMCACHEBUFFERARRAY_H

#include <e32base.h>
#include <badesca.h>
#include <imcachedefs.h>

//	FORWARD CLASS DECLERATIONS
// CLASS DECLARATION
// Definitions

/**
 *  Message Base class
 */
class CIMCacheBufferArray : public CBase
	{
                      	
    public: // Construction
    
    	
		/**
		 * Construction
		  * @param aData, text message
		 * @return Pointer to new created instance of CIMCacheBufferArray
		 */
		static CIMCacheBufferArray* NewL( const TDesC8& aPacketBuffer ,TIMCacheOperationsCode aOperationCode );

		
		/**
		 * Destruction
		 */
		~CIMCacheBufferArray();
		
	private : // Construction
	
		/**
		 * Constructor
		 */
		CIMCacheBufferArray( TIMCacheOperationsCode aOperationCode );
		
		/**
		 * two phase construction
		 */
		void ConstructL( const TDesC8& aPacketBuffer );

public:  
	
			
		/**
         * operatuin code stored
         * @return operation code
         */
		TIMCacheOperationsCode OperationCode();
		
		/**
         * Message
         * @return  message
         */
		const TDesC8& PacketData();
	                 
                                            
	protected: // Member variables
		    
	    // owned, message buffer
		HBufC8*	 iPacketBuffer;
		
		// owns : to identify type of message packed
		TIMCacheOperationsCode iOperationCode;
       
	}; 

#endif      // CIMIMCACHEMESSAGEBASE_H

// End of File
