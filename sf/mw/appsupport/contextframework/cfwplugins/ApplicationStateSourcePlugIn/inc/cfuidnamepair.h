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


#ifndef CFUIDNAMEPAIR_H
#define CFUIDNAMEPAIR_H


#include <e32base.h>

/**
*
*/
/**
*  Encapsulates uid name pair definition.
*
*  @lib
*  @since S60 v4.0
*/
class CCFUidNamePair : public CBase
    {
    public:

        /**
        * Two phased constructor.
        *
        * @since S60 4.0
        * @param aUid Uid.
        * @param aName Name.
        * @return CCFUidNamePair*
        */
        static CCFUidNamePair* NewL(
            const TUid& aUid,
            const TDesC& aName );
        static CCFUidNamePair* NewLC(
            const TUid& aUid,
            const TDesC& aName );

        virtual ~CCFUidNamePair();

    public:

        /**
        * Returns uid.
        *
        * @since S60 4.0
        * @param None
        * @return const TUid&
        */
        const TUid& Uid() const;

        /**
        * Returns name.
        *
        * @since S60 4.0
        * @param None
        * @return const TDesC&
        */
        const TDesC& Name() const;

    private:

        CCFUidNamePair( const TUid& aUid );

        void ConstructL( const TDesC& aName );

    private: // Data

        // Uid.
        TUid iUid;

        // Name.
        HBufC* iName;
    };

typedef RPointerArray< CCFUidNamePair > RUidNamePointerArray;

#endif // CFUIDNAMEPAIR_H
