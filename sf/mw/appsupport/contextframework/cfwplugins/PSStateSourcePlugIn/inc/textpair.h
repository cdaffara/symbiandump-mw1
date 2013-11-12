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


#ifndef C_TEXTPAIR_H
#define C_TEXTPAIR_H


#include <e32base.h>

/**
*  Encapsulates text-text pair definition.
*
*  @lib
*  @since S60 v4.0
*/
class CTextPair : public CBase
    {
    public:

        static CTextPair* NewL(
            const TDesC& aText1,
            const TDesC& aText2 );
        static CTextPair* NewLC(
            const TDesC& aText1,
            const TDesC& aText2 );

        virtual ~CTextPair();

    public:

        const TDesC& Text1() const;

        const TDesC& Text2() const;

    private:

        CTextPair( );

        void ConstructL( const TDesC& aText1, const TDesC& aText2 );

    private: // Data

        HBufC* iText1;

        HBufC* iText2;
    };

typedef RPointerArray< CTextPair > RTextPairPointerArray;

#endif // C_TEXTPAIR_H
