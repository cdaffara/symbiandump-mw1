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


#include "intnamepair.h"


// ======== MEMBER FUNCTIONS ========

CIntNamePair::CIntNamePair( const TInt& aInt ) : iInt( aInt )
    {
    }

void CIntNamePair::ConstructL( const TDesC& aName )
    {
    iName = aName.AllocL();
    }

CIntNamePair* CIntNamePair::NewL( const TInt& aInt,
    const TDesC& aName )
    {
    CIntNamePair* self = CIntNamePair::NewLC( aInt, aName );
    CleanupStack::Pop( self );

    return self;
    }

CIntNamePair* CIntNamePair::NewLC( const TInt& aInt,
    const TDesC& aName )
    {
    CIntNamePair* self = new( ELeave ) CIntNamePair( aInt );
    CleanupStack::PushL( self );
    self->ConstructL( aName );

    return self;
    }

CIntNamePair::~CIntNamePair()
    {
    delete iName;
    }


//-----------------------------------------------------------------------------
// CIntNamePair::Uid
//-----------------------------------------------------------------------------
//
const TInt& CIntNamePair::Int() const
    {
    return iInt;
    }

//-----------------------------------------------------------------------------
// CIntNamePair::Name
//-----------------------------------------------------------------------------
//
const TDesC& CIntNamePair::Name() const
    {
    if( !iName )
        {
        return KNullDesC;
        }
    else
        {
        return *iName;
        }
    }
