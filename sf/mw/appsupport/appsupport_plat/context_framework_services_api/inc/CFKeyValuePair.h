/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFKeyValuePair class declaration.
*
*/


#ifndef CFKEYVALUEPAIR_H
#define CFKEYVALUEPAIR_H


#include <e32base.h>

/**
* Encapsulates key value pair definition.
*/
class CCFKeyValuePair : public CBase
    {
    public:
    
    	/**
    	* Two phased constructor.
    	* 
    	* @since S60 5.0
    	* @param aKey Key.
    	* @param aValue Value.
    	* @return CCFKeyValuePair*
    	*/
        IMPORT_C static CCFKeyValuePair* NewL(
            const TDesC& aKey,
            const TDesC& aValue );
        IMPORT_C static CCFKeyValuePair* NewLC(
            const TDesC& aKey,
            const TDesC& aValue );

    	/**
    	* Two phased constructor.
    	* Ownership of parameters are transfered.
    	* 
    	* @since S60 5.0
    	* @param aKey Key.
    	* @param aValue Value.
    	* @return CCFKeyValuePair*
    	*/
        IMPORT_C static CCFKeyValuePair* NewL(
            HBufC* aKey,
            HBufC* aValue );
        IMPORT_C static CCFKeyValuePair* NewLC(
            HBufC* aKey,
            HBufC* aValue );

        IMPORT_C ~CCFKeyValuePair();
        
    public:
    
        /**
        * Returns key.
        * 
        * @since S60 5.0
        * @param None
        * @return const TDesC&
        */
        IMPORT_C const TDesC& Key() const;

        /**
        * Returns value.
        * 
        * @since S60 5.0
        * @param None
        * @return const TDesC&
        */
        IMPORT_C const TDesC& Value() const;
            
    private:
    
        void ConstructL( const TDesC& aKey, const TDesC& aValue );
            
        CCFKeyValuePair();
        CCFKeyValuePair( HBufC* aKey, HBufC* aValue );
        
    private: // Data
    
        // Own: Key
        HBufC* iKey;
        
        // Own: Value
        HBufC* iValue;
    };
    
typedef RPointerArray<CCFKeyValuePair> RKeyValueArray;

#endif
