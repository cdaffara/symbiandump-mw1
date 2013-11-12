/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class handles columns.
*
*/


#include <centralrepository.h>

//XMPP includes
//#include "// GFLOGger.h"

#include "xmppsettingscolumn.h"
#include "xmppsettingsapicommon.h"

// ======== MEMBER FUNCTIONS ========

CXmppSettingsColumn::CXmppSettingsColumn(
    CRepository& aRepository ):
    iRepository( aRepository )
    {

    }


void CXmppSettingsColumn::ConstructL()
    {

    }


CXmppSettingsColumn* CXmppSettingsColumn::NewL(
    CRepository& aRepository )
    {
    CXmppSettingsColumn* self = CXmppSettingsColumn::NewLC( aRepository );
    CleanupStack::Pop( self );
    return self;
    }


CXmppSettingsColumn* CXmppSettingsColumn::NewLC(
    CRepository& aRepository )
    {
    CXmppSettingsColumn* self
        = new( ELeave ) CXmppSettingsColumn( aRepository );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


CXmppSettingsColumn::~CXmppSettingsColumn()
    {

    }


// ---------------------------------------------------------------------------
// Checks does given column exist. If not, it creates new entry.
// ---------------------------------------------------------------------------
//
void CXmppSettingsColumn::GetColumnKeyL(
    const TDesC& aAttrName,
    TBool aCreateColumnIfNotExist,
    TUint32& aColKey )
    {
    TRAPD( error, FindByNameL( aAttrName,
                               aColKey ) );

    // LOGERR("CXmppSettingsColumn::GetColumnKeyL - error: %d",error);

    // Column was not found, but user want's to create new one.
    if ( error == KErrNotFound  && aCreateColumnIfNotExist )
        {
       // GFLOG1("CXmppSettingsColumn::GetColumnKeyL - column is not found => Create new");
        //create new
        TUint32 newColKey( 0 );
        NextFreeSlotL( newColKey );
       // GFLOG2("CXmppSettingsColumn::GetColumnKeyL - newColKey: 0x%08x",newColKey);

        // Set column name to COLUMN-table.
        User::LeaveIfError( iRepository.Create( newColKey, aAttrName ) );
        aColKey = newColKey;
        }
    // Column was not found, but user does not want to create new.
    else if( error == KErrNotFound  && !aCreateColumnIfNotExist )
        {
       // GFLOG1("CXmppSettingsColumn::GetColumnKeyL - column is not found => New not wanted - leaving");
        User::Leave( KErrNotFound );
        }
    // other error occured -> Leave.
    else if ( error != KErrNone )
        {
       // GFLOG1("CXmppSettingsColumn::GetColumnKeyL - error occured - leaving");
        User::Leave( error );
        }
    // Col found.
    else
        {
        // Everything ok.
        }
    }

// ---------------------------------------------------------------------------
// Tries to find a column key by searching with the column name.
// ---------------------------------------------------------------------------
//
void CXmppSettingsColumn::FindByNameL(
    const TDesC& aColumnName,
    TUint32& aColumnKey )
    {

    RArray<TUint32> keys;
    CleanupClosePushL( keys );

    TInt error( KErrNone );

    error = iRepository.FindEqL( KXmppKeyColumnTable,
                                 KXmppMaskColumns,
                                 aColumnName,
                                 keys );

    // LOGERR("CXmppSettingsColumn::FindByNameL - error: %d", error);
    User::LeaveIfError ( error );

   // GFLOG1("CXmppSettingsColumn::FindByColumnName - column found.");
    aColumnKey = keys[0];


//    DEBUG_BLOCK( 
    
    if ( keys.Count() > 1 )
                    {
                    // this should not happen. if it does, it is bug in somewhere in this component.
                   // GFLOG1("CXmppSettingsColumn::FindByColumnName - WHAAAAT! TO MANY RESULTS -> LEAVE!");
                    User::Leave( KErrGeneral );
                    }
 //               );
    CleanupStack::PopAndDestroy( &keys );

    }


// PRIVATE METHODS


// ---------------------------------------------------------------------------
// Returns the next free setting slot.
// ---------------------------------------------------------------------------
//
void CXmppSettingsColumn::NextFreeSlotL(
    TUint32& aColKey )
    {

    RArray<TUint32> results;
    CleanupClosePushL( results );

    // First, find all rows and get count.
    TInt error = iRepository.FindL( KXmppKeyColumnTable,
                                    KXmppMaskColumns,
                                    results );

    TInt count = results.Count();
   // GFLOG2("CXmppSettingsColumn::NextFreeSlotL - count: %d",count);
    CleanupStack::PopAndDestroy( &results );

    if ( count > 0 )
        {
        // Count can be used as new key
        aColKey = count;
        }
    else
        {
        // First column ever
        aColKey = 0;
        }

   // GFLOG2("CXmppSettingsColumn::NextFreeSlotL - aColKey: 0x%08x",aColKey);

    }
