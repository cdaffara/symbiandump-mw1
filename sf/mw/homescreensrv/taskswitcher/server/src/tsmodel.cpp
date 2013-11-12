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
* Description:
*
*/
#include "tsmodel.h"

// -----------------------------------------------------------------------------
/**
 * Create shallow copy of model item
 * @param aOffset - index of requested item
 * @return reference to item
 */
TTsModelItem MTsModel::ItemL( TInt aOffset ) const
{
    if( Count() <= aOffset )
        {
        User::Leave(KErrOverflow);
        }
    return TTsModelItem( *this, aOffset );
}
