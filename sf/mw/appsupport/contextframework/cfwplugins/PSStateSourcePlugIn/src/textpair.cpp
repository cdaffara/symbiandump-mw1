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


#include "textpair.h"


// ======== MEMBER FUNCTIONS ========

CTextPair::CTextPair( ) 
    {
    }

void CTextPair::ConstructL( const TDesC& aText1, const TDesC& aText2 )
    {
    iText1 = aText1.AllocL();
    iText2 = aText2.AllocL();
    }

CTextPair* CTextPair::NewL( const TDesC& aText1, const TDesC& aText2 )
    {
    CTextPair* self = CTextPair::NewLC( aText1, aText2 );
    CleanupStack::Pop( self );

    return self;
    }

CTextPair* CTextPair::NewLC( const TDesC& aText1, const TDesC& aText2 )
    {
    CTextPair* self = new( ELeave ) CTextPair( );
    CleanupStack::PushL( self );
    self->ConstructL( aText1, aText2 );

    return self;
    }

CTextPair::~CTextPair()
    {
    delete iText1;
    delete iText2;
    }


//-----------------------------------------------------------------------------
// CTextPair::Text1
//-----------------------------------------------------------------------------
//
const TDesC& CTextPair::Text1() const
    {
    if( !iText1 )
        {
        return KNullDesC;
        }
    else
        {
        return *iText1;
        }
    }

//-----------------------------------------------------------------------------
// CTextPair::Text2
//-----------------------------------------------------------------------------
//
const TDesC& CTextPair::Text2() const
    {
    if( !iText2 )
        {
        return KNullDesC;
        }
    else
        {
        return *iText2;
        }
    }
