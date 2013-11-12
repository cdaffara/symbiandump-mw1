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
 * Constructor
 * @param aModel - data owner
 * @param aOffset - data index
 */
TTsModelItem::TTsModelItem( const MTsModel& aModel, TInt aOffset )
:
iModel( aModel ),
iIndex( aOffset )

    {
    //No implementation required
    }

// -----------------------------------------------------------------------------
/**
 * Copy constructor
 * @param aItem - template item
 */
TTsModelItem::TTsModelItem( const TTsModelItem& aItem )
:
iModel( aItem.iModel ),
iIndex( aItem.iIndex )
    {
    //No implementation required
    }

// -----------------------------------------------------------------------------
/**
 * Validate item instance and retrieve display name data
 * @return item display name
 */
const TDesC& TTsModelItem::DisplayNameL() const
    {
    ValidateL();
    return iModel.DisplayNameL( iIndex );
    }

// -----------------------------------------------------------------------------
/**
 * Validate item instance and retrieve icon handle ( CFbsBitmap handle )
 * @return item icon handle
 */
TInt TTsModelItem::IconHandleL() const
    {
    ValidateL();
    return iModel.IconHandleL( iIndex );
    }

// -----------------------------------------------------------------------------
/**
 * Validate item instance and retrieve entry key
 * @return item key
 */
TTsModelItemKey TTsModelItem::KeyL() const
    {
    ValidateL();
    return iModel.KeyL( iIndex );
    }

// -----------------------------------------------------------------------------
/**
 * Validate item instance and retrieve entry timestamp
 * @return item timestamp
 */
TTime TTsModelItem::TimestampL() const
    {
    ValidateL();
    return iModel.TimestampL( iIndex );
    }

// -----------------------------------------------------------------------------
/**
 * Validate item instance and retrieve timestamp
 * with latest update time
 * @return item timestamp
 */
TTime TTsModelItem::TimestampUpdateL() const 
{
    ValidateL();
    return iModel.TimestampUpdateL(iIndex);
}

// -----------------------------------------------------------------------------
/**
 * Validate item instance and retrieve activity status 
 * @return activity status
 */
TBool TTsModelItem::IsActiveL() const
    {
    ValidateL();
    return iModel.IsActiveL( iIndex );
    }

// -----------------------------------------------------------------------------
/**
 * Validate item instance and retrieve closable status 
 * @return closable status
 */
TBool TTsModelItem::IsClosableL() const
    {
    ValidateL();
    return iModel.IsClosableL( iIndex );
    }

// -----------------------------------------------------------------------------
/**
 * Validate item instance and forward close request to its owner 
 * @return EFalse on failure
 */
TBool TTsModelItem::CloseL() const
    {
    ValidateL();
    return iModel.CloseL( KeyL() );
    }

// -----------------------------------------------------------------------------
/**
 * Validate item instance and forward launch request to its owner 
 * @return EFalse on failure
 */
TBool TTsModelItem::LaunchL() const
    {
    ValidateL();
    return iModel.LaunchL( KeyL() );
    }

// -----------------------------------------------------------------------------
/**
 * @return EFalse if entry is not mandatory, other value in other cases  
 */
TBool TTsModelItem::IsMandatoryL() const
    {
    ValidateL();
    return iModel.IsMandatoryL( iIndex );
    }

// -----------------------------------------------------------------------------
/**
 * Validate item instance 
 */
void TTsModelItem::ValidateL() const
    {
    if(iModel.Count() <= iIndex )
        {
        User::Leave( KErrOverflow );
        }
    }

// -----------------------------------------------------------------------------
/**
 * Serialize item into destination stream
 * @param aStream - output stream 
 */

void TTsModelItem::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteInt32L( DisplayNameL().Length() );
    if( 0 < DisplayNameL().Length() )
        {
        aStream << DisplayNameL();
        }
    TPckgBuf<TTime> timestamp(TimestampUpdateL()); 
    aStream.WriteL(timestamp);
    aStream.WriteInt32L( IconHandleL() );
    aStream.WriteInt32L( TTsModelItemKey::Size() );
    aStream << KeyL();
    aStream.WriteInt32L( IsActiveL() );
    aStream.WriteInt32L( IsClosableL() );
    }

