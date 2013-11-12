/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Task list entry
*
*/
#include <s32mem.h>
#include "tsmodelitemkeymsg.h"

// -----------------------------------------------------------------------------
CTsModelItemKeyMsg* CTsModelItemKeyMsg::NewLC( RReadStream& aStream )
    {
    CTsModelItemKeyMsg* self = new (ELeave)CTsModelItemKeyMsg();
    CleanupStack::PushL( self );
    self->ConstructL( aStream );
    return self;
    }

// -----------------------------------------------------------------------------
CTsModelItemKeyMsg::~CTsModelItemKeyMsg()
    {
    //No implementation required
    }

// -----------------------------------------------------------------------------
TTsModelItemKey CTsModelItemKeyMsg::Key() const
{
    return iKey;
}

// -----------------------------------------------------------------------------
void CTsModelItemKeyMsg::InternalizeL( RReadStream& aStream )
    {
    aStream >> iKey;
    }

// -----------------------------------------------------------------------------
CTsModelItemKeyMsg::CTsModelItemKeyMsg()
    {
    //No implementation required
    }

// -----------------------------------------------------------------------------
void CTsModelItemKeyMsg::ConstructL( RReadStream& aStream )
{
    aStream >> ( *this );
}

