/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Encapsulates uid and name.
*
*/


#ifndef INTNAMEPAIR_H
#define INTNAMEPAIR_H


#include <e32base.h>

/**
*  Encapsulates int name pair definition.
*
*  @lib
*  @since S60 v4.0
*/
class CIntNamePair : public CBase
    {
    public:

        /**
        * Two phased constructor.
        *
        * @since S60 4.0
        * @param aUid Uid.
        * @param aName Name.
        * @return CIntNamePair*
        */
        static CIntNamePair* NewL(
            const TInt& aUid,
            const TDesC& aName );
        static CIntNamePair* NewLC(
            const TInt& aUid,
            const TDesC& aName );

        virtual ~CIntNamePair();

    public:

        /**
        * Returns uid.
        *
        * @since S60 4.0
        * @param None
        * @return const TInt&
        */
        const TInt& Int() const;

        /**
        * Returns name.
        *
        * @since S60 4.0
        * @param None
        * @return const TDesC&
        */
        const TDesC& Name() const;

    private:

        CIntNamePair( const TInt& aInt );

        void ConstructL( const TDesC& aName );

    private: // Data

        // Uid.
        TInt iInt;

        // Name.
        HBufC* iName;
    };

typedef RPointerArray< CIntNamePair > RIntNamePointerArray;

#endif // INTNAMEPAIR_H
