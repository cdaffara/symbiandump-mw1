/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFContextObjectImpl class declaration.
*
*/


#ifndef C_CFCONTEXTOBJECTIMPL_H
#define C_CFCONTEXTOBJECTIMPL_H

#include "cfcontextobject.h"

/**
* Context object implementation.
*
* @lib CFServices.lib
* @since S60 5.0
*/
NONSHARABLE_CLASS( CCFContextObjectImpl ): public CCFContextObject
    {
    public:
    
        /**
        * Two phased construction.
        * 
        * @since S60 5.0
        * @param None.
        * @return None.
        */    
        IMPORT_C static CCFContextObjectImpl* NewL();
        IMPORT_C static CCFContextObjectImpl* NewLC();
        
        /**
        * Two phased construction.
        * 
        * @since S60 5.0
        * @param aSource Initial source of the context.
        * @param aType Initial type of the context.
        * @param aValue Initial value of the context.
        * @return None.
        */    
        IMPORT_C static CCFContextObjectImpl* NewL( const TDesC& aSource,
            const TDesC& aType,
            const TDesC& aValue );
        IMPORT_C static CCFContextObjectImpl* NewLC( const TDesC& aSource,
            const TDesC& aType,
            const TDesC& aValue );
        
        // Destructior
        ~CCFContextObjectImpl();
        
    public: // From base classes
    
        // @see CCFContextObject
        void SetTypeL( const TDesC& aType );
            
        // @see CCFContextObject
        const TDesC& Type() const;
            
        // @see CCFContextObject
        void SetValueL( const TDesC& aValue );
            
        // @see CCFContextObject
        const TDesC& Value() const;
            
        // @see CCFContextObject
        void SetSourceL( const TDesC& aSource );
            
        // @see CCFContextObject
        const TDesC& Source() const;
            
        // @see CCFContextObject
        void SetConfidence( const TUint8& aConfidence );
            
        // @see CCFContextObject
        TUint8 Confidence() const;
            
        // @see CCFContextObject
        void SetTimestampToHomeTime();
            
        // @see CCFContextObject
        void SetTimestamp( const TTime& aTimestamp );
            
        // @see CCFContextObject
        void SetTimestamp( const TInt64& aTimestamp );
            
        // @see CCFContextObject
        const TTime& Timestamp() const;
        
        // @see CCFContextObject
        void CopyL( const CCFContextObject& aContext );
            
    public: // New methods

        /**
        * Internalizes object.
        * 
        * @since S60 5.0
        * @param aStream Read stream.
        * @return None.
        */    
        IMPORT_C void InternalizeL( RReadStream& aStream );
        
        /**
        * Externalizes object.
        *
        * @since S60 5.0
        * @param aStream Write stream.
        * @return None.
        */    
        IMPORT_C void ExternalizeL( RWriteStream& aStream );

        /**
        * Returns size of the object in bytes.
        *
        * @since S60 5.0
        * @param None.
        * @return TInt.
        */    
        IMPORT_C TInt Size() const;

        /**
        * Compares value member variables of context objects.
        * Compares this context objects value member to the arguments value
        * member.
        *
        * @since S60 5.0
        * @param aContext The context object to be compared with this one.
        * @return Positive if the value member of this is greater than
        *  aContext's. Negative if the value member of this is less than
        *  aContext's. Zero if both value members have the same length
        *  and the their contents are the same.
        */
        IMPORT_C TInt CompareValue( const CCFContextObject& aContext ) const;

        /**
        * Compares two context objects based on their type.
        *
        * @since S60 5.0
        * @param aFirst A context object to be compared with aSecond.
        * @param aSecond A context object to be compared with aFirst.
        * @return Positive if aFirst is greater than aSecond. Negative if aFirst
        *  is less than aSecond. Zero if both have the same length and
        *  the their contents are the same.
        */
        IMPORT_C static TInt CompareByType(	const CCFContextObject& aFirst,
            const CCFContextObject& aSecond );

        /**
        * Compares two context objects based on their type and source.
        *
        * @since S60 5.0
        * @param aFirst A context object to be compared with aSecond.
        * @param aSecond A context object to be compared with aFirst.
        * @return Positive if aFirst is greater than aSecond. Negative if aFirst
        *  is less than aSecond. Zero if both have the same length and
        *  the their contents are the same.
        */
        IMPORT_C static TInt CompareByTypeAndSource(
            const CCFContextObject& aFirst,
            const CCFContextObject& aSecond );

        /**
        * Compares two context objects based on their time.
        *
        * @since S60 5.0
        * @param aFirst A context object to be compared with aSecond.
        * @param aSecond A context object to be compared with aFirst.
        * @return Positive if aFirst is greater than aSecond. Negative if aFirst
        *  is less than aSecond. Zero if both have the same length and
        *  the their contents are the same.
        */
        IMPORT_C static TInt CompareByTimeDescending(
            const CCFContextObject& aFirst,
            const CCFContextObject& aSecond );

        /**
        * Checks whether the two context objects are the same.
        * Compares all member variables of the two context objects and if all
        * of them are equal, then the objects are the same.
        *
        * @param aFirst A context object to be compared with aSecond.
        * @param aSecond A context object to be compared with aFirst.
        * @return ETrue, if the objects are found to be equal.
        */
        IMPORT_C static TBool IsSame( const CCFContextObject& aFirst,
            const CCFContextObject& aSecond );
            
    private:
    
        CCFContextObjectImpl();
        void ConstructL( const TDesC& aSource,
            const TDesC& aType,
            const TDesC& aValue );
        
    private: // Data
    
        /** Source of the context */
        HBufC* iSource;

        /** Type of the context */
        HBufC* iType;

        /** iValue of the context */
        HBufC* iValue;

        /** Time stamp */
        TTime iTimestamp;

        /** Confidence of the context */
        TUint iConfidence;
    };

#endif
