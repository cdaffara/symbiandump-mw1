/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Context data object test class.
*
*/


#ifndef C_CTESTCONTEXTDATA_H
#define C_CTESTCONTEXTDATA_H

// INCLUDES
#include <cfcontextdataobject.h>

const TUid KTestContextDataUid = { 0x12345678 };

NONSHARABLE_CLASS( CTestContextData ): public CCFContextDataObject
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static CTestContextData* NewL( const TDesC& aKey, const TDesC& aValue );
        static CTestContextData* NewLC( const TDesC& aKey, const TDesC& aValue );
        /**
         * Destructor
         */
        ~CTestContextData();
        
    public:
    
        void SetUid( const TUid& aUid );
        void SetKeyL( const TDesC& aKey );
        void SetValueL( const TDesC& aValue );
        const TDesC& Key() const;
        const TDesC& Value() const;

    private:

        CTestContextData();
        void ConstructL( const TDesC& aKey, const TDesC& aValue );

    private: // From CCFContextDataObject
    
        // @see CCFContextDataObject
        const TUid& Uid() const;

        // @see CCFContextDataObject
        void ExternalizeL( RWriteStream& aStream );

        // @see CCFContextDataObject
        void InternalizeL( RReadStream& aStream );

        // @see CCFContextDataObject
        TInt Size() const;

    private: // Data
    
        /** Key */
        HBufC* iKey;
        
        /** Value */
        HBufC* iValue;
        
        /** UID **/
        TUid iUid;
    };

#endif
