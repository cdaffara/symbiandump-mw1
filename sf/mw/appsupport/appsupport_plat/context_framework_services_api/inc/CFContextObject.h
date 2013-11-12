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
* Description:  CCFContextObject class declaration.
*
*/


#ifndef C_CFCONTEXTOBJECT_H
#define C_CFCONTEXTOBJECT_H

#include <e32base.h>

class RReadStream;
class RWriteStream;

/**
* Context object is used to deliver context information.
* A single context object consists of source, type, value
* timestamp and confidence.
*
* Source:
* Source defines the origin of the context which can
* contain several different types.
*
* Type:
* Type describes a specific context from a certain source. 
* Type can have also sub types. Example:
* [TypeA]
* [TypeA.SubType]
*
* Value:
* Value defines the current value of the context. Value is always
* represneted as a descriuptor but it can be treated inside rules as
* in the form of string, int, float or boolean.
*
* Timestamp:
* Timestamp defines when the context value has been updated.
* 
* @lib CFServices.lib
* @since S60 5.0
*/
NONSHARABLE_CLASS( CCFContextObject ): public CBase
    {
    public:
    
        /**
        * Two phased constructors.
        * 
        * @since S60 5.0
        * @param None.
        * @return CCFContextObject*
        */
        IMPORT_C static CCFContextObject* NewL();
        IMPORT_C static CCFContextObject* NewLC();
        
        /**
        * Two phased constructors.
        * 
        * @since S60 5.0
        * @param aSource Initial source of the context.
        * @param aType Initial type of the context.
        * @param aValue Initial value of the context.
        * @return CCFContextObject*
        */
        IMPORT_C static CCFContextObject* NewL( const TDesC& aSource,
            const TDesC& aType,
            const TDesC& aValue );
        IMPORT_C static CCFContextObject* NewLC( const TDesC& aSource,
            const TDesC& aType,
            const TDesC& aValue );
        
    public:
    
        /**
        * Sets the context type.
        * Copies the entire argument to the context type member variable.
        *
        * @since S60 5.0
        * @param aType The type string to be copied.
		* @return None
        */
        virtual void SetTypeL( const TDesC& aType ) = 0;
        
        /**
        * Returns a constant reference to the context type descriptor.
        *
        * @since S60 5.0
        * @param None
        * @return A constant reference to the context type descriptor.
        */
        virtual const TDesC& Type() const = 0;

        /**
        * Sets the context value.
        * Copies the entire argument to the context value member variable.
		*
		* @since S60 5.0
        * @param aValue The value string to be copied.
		* @return None
        */
        virtual void SetValueL( const TDesC& aValue ) = 0;
        
        /**
        * Returns a constant reference to the context value descriptor.
        *
        * @since S60 5.0
        * @param None
        * @return A constant reference to the context value descriptor.
        */
        virtual const TDesC& Value() const = 0;

        /**
        * Sets the context source.
        * Copies the entire argument to the context source member variable.
        *
        * @since S60 5.0
        * @param aSource The source string to be copied.
		* @return None
        */
        virtual void SetSourceL( const TDesC& aSource ) = 0;
        
        /**
        * Returns a constant reference to the context source descriptor.
        *
        * @since S60 5.0
        * @param None
        * @return A constant reference to the context source descriptor.
        */
        virtual const TDesC& Source() const = 0;

        /**
        * Sets the context confidence.
        * Sets the confidence for this context as a percentage number which
        * should normally be between zero and one hundred (0 - 100%). The
        * actual value is not checked.
        *
        * @since S60 5.0
        * @param aConfidence The confidence for the context in percentages,
        *  from 0 to 100%.
        * @return None
        */
        virtual void SetConfidence( const TUint8& aConfidence ) = 0;
        
        /**
        * Returns the confidence value of the context in percentages which
        * should normally range between 0 and 100%.
        *
        * @since S60 5.0
        * @param None
        * @return The confidence value of the context.
        */
        virtual TUint8 Confidence() const = 0;

        /**
        * Sets the context time stamp to the home time.
        * Sets the time stamp of this context to equal the home time at the
        * time of calling this method.
        * 
        * @since S60 5.0
        * @param None
        * @return None
        */
        virtual void SetTimestampToHomeTime() = 0;
        
        /**
        * Sets the context time stamp.
        * Sets the time stamp for this context.
        *
        * @since S60 5.0
        * @param aTimestamp The time stamp for the context.
        * @return None
        */
        virtual void SetTimestamp( const TTime& aTimestamp ) = 0;
        
        /**
        * Sets the context time stamp.
        * Sets the time stamp for this context.
        *
        * @since S60 5.0
        * @param aTimestamp The time stamp for the context.
        * @return None
        */
        virtual void SetTimestamp( const TInt64& aTimestamp ) = 0;
        
        /**
        * Returns the time stamp of the context.
        *
        * @since S60 5.0
        * @param None
        * @return This context's time stamp.
        */
        virtual const TTime& Timestamp() const = 0;
        
        /**
        * Copies data from existing context object to this one.
        * 
        * @since S60 5.0
        * @param aContext Context object to copy.
        * @return None
        */
        virtual void CopyL( const CCFContextObject& aContext ) = 0;
    };
    
// Context object array type definition
typedef RPointerArray<CCFContextObject> RContextObjectArray;

#endif
