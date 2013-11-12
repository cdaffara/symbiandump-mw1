/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFContextQuery class declaration.
*
*/


#ifndef C_CFCONTEXTQUERY_H
#define C_CFCONTEXTQUERY_H

// INCLUDES
#include <e32base.h>

/**
* Context query can be used to request context objects from
* Context Framework server. Context query consists of a context
* source and a context type.
*
* @lib CFServices.lib
* @since S60 5.0
*/
NONSHARABLE_CLASS( CCFContextQuery ): public CBase
    {
    public:
    
        /**
        * Two phased constructors without construction params.
        *
        * @since S60 5.0
        * @param None
        * @return CCFContextQuery*
        */
        IMPORT_C static CCFContextQuery* NewL();
        IMPORT_C static CCFContextQuery* NewLC();

        /**
        * Two phased constructor.
        * Takes a source and type of the queried context as
        * a parameter.
        *
        * @since S60 5.0
        * @param aSource Context source.
        * @param aType Context Type.
        * @return CCFContextQuery*
        */
        IMPORT_C static CCFContextQuery* NewL( const TDesC& aSource,
            const TDesC& aType );
        IMPORT_C static CCFContextQuery* NewLC( const TDesC& aSource,
            const TDesC& aType );
            
    public:
    
        /**
        * Sets desired context type.
        *
        * @since S60 5.0
        * @param aType The type string to be copied.
        * @return None
        */
        virtual void SetTypeL( const TDesC& aType ) = 0;
        
        /**
        * Returns context type.
        * 
        * @since S60 5.0
        * @param None
        * @return A constant reference to the context type descriptor.
        */
        virtual const TDesC& Type() const = 0;

        /**
        * Sets desired context source.
        *
        * @since S60 5.0
        * @param aSource The source string to be copied.
        * @return None
        */
        virtual void SetSourceL( const TDesC& aSource ) = 0;
        
        /**
        * Returns context source.
        *
        * @since S60 5.0
        * @param None
        * @return A constant reference to the context source descriptor.
        */
        virtual const TDesC& Source() const = 0;

		/**
		* Sets sub type match.
		*
		* @since S60 5.0
		* @param aMatchSubTypes Flag indicating sub type match.
		* @return None
		*/
		virtual void SetSubTypeMatch( TBool aMatchSubTypes ) = 0;

		/**
		* Checks if sub type match is set.
		* 
		* @since S60 5.0
		* @param None
		* @return ETrue if sub type match set.
		*/
		virtual TBool SubTypeMatch() const = 0;
    };

// Context query array type definition
typedef RPointerArray<CCFContextQuery> RContextQueryArray;

#endif
