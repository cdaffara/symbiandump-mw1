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
* Description:  This class is used for data table manipulations.
*
*/


#include <centralrepository.h>

//XMPP includes
// #include "// GFLOGger.h"

#include "xmppsettingsrecord.h"
#include "xmppsettingsapicommon.h"

// ======== MEMBER FUNCTIONS ========

CXmppSettingsRecord::CXmppSettingsRecord(
    CRepository& aRepository ):
    iRepository( aRepository )
    {

    }

CXmppSettingsRecord* CXmppSettingsRecord::NewL(
    CRepository& aRepository )
    {
    CXmppSettingsRecord* self = CXmppSettingsRecord::NewLC( aRepository );
    CleanupStack::Pop( self );
    return self;
    }

CXmppSettingsRecord* CXmppSettingsRecord::NewLC(
    CRepository& aRepository )
    {
    CXmppSettingsRecord* self
        = new( ELeave ) CXmppSettingsRecord( aRepository );
    CleanupStack::PushL( self );
    return self;
    }


CXmppSettingsRecord::~CXmppSettingsRecord()
    {
   // GFLOG1("CXmppSettingsRecord::~CXmppSettingsRecord");
    iSettingsRecordKeyArray.Reset();
    iSettingsRecordKeyArray.Close();
    }


// ---------------------------------------------------------------------------
// Finds settingsRecord key. Leaves if not found.
// ---------------------------------------------------------------------------
//
void CXmppSettingsRecord::FindSettingsRecordKeyL(
    TUint32 aSettingsRecordId,
    TUint32& aSettingsRecordKey )
    {
    GetSettingsRecordkeysL();

    TInt counter( iSettingsRecordKeyArray.Count() );
    TInt key = KErrNotFound;
	for (TInt i = 0; i< counter; i++)
		{
		TInt id( 0 );
		User::LeaveIfError( iRepository.Get( iSettingsRecordKeyArray[i],
		                        id ) );
		if ( id == aSettingsRecordId )            
			{	
			key = iSettingsRecordKeyArray[i];          
			break;
			}
		}
	 aSettingsRecordKey = key;

    }

// ---------------------------------------------------------------------------
// returns list of all id keys.
// ---------------------------------------------------------------------------
//
void CXmppSettingsRecord::GetSettingsRecordkeysL(
    RArray<TUint32>& aResults )
    {
    GetSettingsRecordkeysL();

    TInt count( iSettingsRecordKeyArray.Count() );
    for ( TInt i=0; i < count; i++ )
        {
        aResults.Append( iSettingsRecordKeyArray[i] );
        }
    }

// ---------------------------------------------------------------------------
// This method creates new settings id.
// ---------------------------------------------------------------------------
//
void CXmppSettingsRecord::CreateSettingsRecordIdL(
    TUint32& aNewSettingId )
    {
   // GFLOG1("CXmppSettingsRecord::CreateSettingsRecordIdL - IN");

    TUint32 nextFreeSlot( 0 );

    NextFreeSlotL( nextFreeSlot );

    TInt settingId( 0 );

    TInt error( iRepository.Get( KCurrentSettingIdIndex, settingId ) );
    // LOGERR("CXmppSettingsRecord::CreateSettingsRecordIdL - iRepository.Get( KCurrentSettingIdIndex ) - error: %d", error);

    if ( !error )
        {
        settingId++;
        // GFLOG3("CXmppSettingsRecord::CreateSettingsRecordIdL - nextFreeSlot: 0x%08x, settingId: %d",nextFreeSlot, settingId);
        error = iRepository.Create( nextFreeSlot, settingId );
        // LOGERR("CXmppSettingsRecord::CreateSettingsRecordIdL - iRepository.Create( nextFreeSlot ) - error: %d", error);
        if ( !error )
            {
            error = iRepository.Set( KCurrentSettingIdIndex, settingId );
            aNewSettingId = settingId;
            }
        }
    User::LeaveIfError( error );
   // GFLOG1("CXmppSettingsRecord::CreateSettingsRecordIdL - OUT");
    }


// PRIVATE


// ---------------------------------------------------------------------------
// returns next free slot key.
// ---------------------------------------------------------------------------
//
void CXmppSettingsRecord::NextFreeSlotL(
    TUint32& aFreekey )
    {
    // First, find all rows and get count.
    const TUint32 shift = 0x00010000;

    GetSettingsRecordkeysL();

    const TInt count( iSettingsRecordKeyArray.Count() );
   // GFLOG2("CXmppSettingsRecord::NextFreeSlotL - count: %d",count);

    if ( count == 0 )
        {
        aFreekey = KXmppKeyDataTable;
        }
    // check for free slots
    else
        {
        TBool foundFreeSlot( EFalse );

        // go through all indexes and try to find free one.
        for ( TInt i=0; i < count; i++ )
            {
            TUint32 tmpKey( KXmppKeyDataTable | ( i * shift ) );
            TInt answer( iSettingsRecordKeyArray.Find( tmpKey ) );

            if ( answer == KErrNotFound )
                {
                foundFreeSlot = ETrue;
                aFreekey = tmpKey;
                break;
                }
            }
       if ( !foundFreeSlot )
            {
           // GFLOG1("CXmppSettingsRecord::NextFreeSlotL - no free slots, creating new");
            aFreekey = KXmppKeyDataTable | ( count * shift );
            }
        }
   // GFLOG2("CXmppSettingsRecord::NextFreeSlotL - aFreekey: 0x%08x",aFreekey);

    }

// ---------------------------------------------------------------------------
// Find all unique settings ids.
// ---------------------------------------------------------------------------
//
void CXmppSettingsRecord::GetSettingsRecordkeysL()
    {
    iSettingsRecordKeyArray.Reset();

    TInt error = iRepository.FindL( KXmppKeyDataTable,
                                    KXmppMaskAllRows,
                                    iSettingsRecordKeyArray );

   // GFLOG2("CXmppSettingsRecord::GetSettingsRecordkeysL - iSettingsRecordKeyArray.Count(): %d",iSettingsRecordKeyArray.Count());

    }
