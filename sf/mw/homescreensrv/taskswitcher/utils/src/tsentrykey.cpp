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


#include "tsentrykey.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TTsEntryKey::TTsEntryKey(TInt parentId)
:
    mParentId(parentId)
{}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool TTsEntryKey::operator ==(const TTsEntryKey& key) const
{
    return (mParentId == key.mParentId);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt TTsEntryKey::WindowGroupId() const
{
    return mParentId;
}
