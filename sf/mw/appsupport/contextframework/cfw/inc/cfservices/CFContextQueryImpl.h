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
* Description:  
*
*/


#ifndef C_CFCONTEXTQUERYIMPL_H
#define C_CFCONTEXTQUERYIMPL_H

#include "cfcontextquery.h"    // defs type & source lengths

class RWriteStream;
class RReadStream;

/**
* CCFContextQuery implementation.
*
* @lib CFServices.lib
* @since S60 5.0
*/
NONSHARABLE_CLASS( CCFContextQueryImpl ): public CCFContextQuery
    {
            
    public:
    
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
        IMPORT_C static CCFContextQueryImpl* NewL( const TDesC& aSource,
            const TDesC& aType );
        IMPORT_C static CCFContextQueryImpl* NewLC( const TDesC& aSource,
            const TDesC& aType );
            
        // Destructor
        ~CCFContextQueryImpl();
    
    public: // From base classes
    
        // @see CCFContextQuery
        void SetTypeL( const TDesC& aType );
			
        // @see CCFContextQuery
        const TDesC& Type() const;
			
        // @see CCFContextQuery
        void SetSourceL( const TDesC& aSource );
			
        // @see CCFContextQuery
        const TDesC& Source() const;
			
        // @see CCFContextQuery
        void SetSubTypeMatch( TBool aMatchSubTypes );
			
        // @see CCFContextQuery
        TBool SubTypeMatch() const;
			
    public:

        /**
        * Internalizes object.
        *
        * @since S60 5.0
        * @param aStream Stream from which to internalize.
        * @return None.
        */
        IMPORT_C void InternalizeL( RReadStream& aStream );

        /**
        * Externalizes object.
        * 
        * @since S60 5.0
        * @param aStream Stream where to externalize.
        * @return None
        */
        IMPORT_C void ExternalizeL( RWriteStream& aStream );

        /**
        * Gets object size in bytes.
        *
        * @since S60 5.0
        * @param None
        * @return TInt
        */
        IMPORT_C TInt Size() const;

        /**
        * Checks if a context source and context type matches this query.
        * 
        * @since S60 5.0
        * @param aContextType Context type to match.
        * @param aContextSource Context source to match.
        * @return ETrue if a match.
        */
        IMPORT_C TBool Matches( const TDesC& aContextType, 
            const TDesC& aContextSource ) const;

        /**
        * Checks if a particular query matches a context source,
        * context type and sub type match.
        * 
        * @since S60 5.0
        * @param aQuery Query to match with.
        * @param aContextType Context type to match with query.
        * @param aContextSource Context source to match with query.
        * @param aMatchSubTypes Match sub types flag.
        * @return ETrue if a match.
        */
        IMPORT_C static TBool MatchesQuery( const CCFContextQuery& aQuery,
			const TDesC& aContextType,
			const TDesC& aContextSource,
			TBool aMatchSubTypes );

        /**
        * Checks if tow queries are the same.
        *
        * @since S60 5.0
        * @param aQuery1 First query.
        * @param aQuery2 Second query.
        * @return ETrue if a match.
        */
        IMPORT_C static TBool IsSame( const CCFContextQuery& aQuery1,
			const CCFContextQuery& aQuery2 );
			
    private:
    
        CCFContextQueryImpl();
        void ConstructL( const TDesC& aSource, const TDesC& aType );

    private: // Data

        /** 
        * The (URI or semantic description of the) source from which the
        * context originates.
        */
        HBufC* iSource;

        /**
        * One context category description.
        * Example: <code>Environment.Temperature</code>
        */
        HBufC* iType;

        /**
        * Are subtypes are matched by this query.
        */
        TBool iMatchSubTypes;
    };

#endif
