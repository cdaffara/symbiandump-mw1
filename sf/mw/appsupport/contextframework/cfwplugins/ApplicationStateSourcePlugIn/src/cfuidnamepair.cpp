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


#include "cfuidnamepair.h"


// ======== MEMBER FUNCTIONS ========

CCFUidNamePair::CCFUidNamePair( const TUid& aUid ) : iUid( aUid )
    {
    }

void CCFUidNamePair::ConstructL( const TDesC& aName )
    {
    iName = aName.AllocL();
    }

CCFUidNamePair* CCFUidNamePair::NewL( const TUid& aUid,
    const TDesC& aName )
    {
    CCFUidNamePair* self = CCFUidNamePair::NewLC( aUid, aName );
    CleanupStack::Pop( self );

    return self;
    }

CCFUidNamePair* CCFUidNamePair::NewLC( const TUid& aUid,
    const TDesC& aName )
    {
    CCFUidNamePair* self = new( ELeave ) CCFUidNamePair( aUid );
    CleanupStack::PushL( self );
    self->ConstructL( aName );

    return self;
    }

CCFUidNamePair::~CCFUidNamePair()
    {
    delete iName;
    }


//-----------------------------------------------------------------------------
// CCFUidNamePair::Uid
//-----------------------------------------------------------------------------
//
const TUid& CCFUidNamePair::Uid() const
    {
    return iUid;
    }

//-----------------------------------------------------------------------------
// CCFUidNamePair::Name
//-----------------------------------------------------------------------------
//
const TDesC& CCFUidNamePair::Name() const
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
