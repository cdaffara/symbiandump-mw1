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
* Description:  This class handles dynamic name-value pair storing.
*                Notice that "setting id"-is parameter used by users of this
*                lib. The "Setting key" is used for Central Repository
*                operations.
*
*/



#include <centralrepository.h>

//XMPP includes
// #include "GFLOGger.h"
#include "xmppservicesettingsapi.h"

#include "xmppsettingsapicommon.h"
#include "xmppsettingscolumn.h"
#include "xmppsettingsrecord.h"

const TUint32 KMaxDescLength = 255;

// ======== MEMBER FUNCTIONS ========


CXmppSettingsApi::CXmppSettingsApi()
    {

    }


void CXmppSettingsApi::ConstructL()
    {
    iRepository = CRepository::NewL( KXmppSettingsApiCenRep );
    iRecords = CXmppSettingsRecord::NewL( *iRepository );
    iColumns = CXmppSettingsColumn::NewL( *iRepository );
    }


EXPORT_C CXmppSettingsApi* CXmppSettingsApi::NewL()
    {
    CXmppSettingsApi* self = CXmppSettingsApi::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


EXPORT_C CXmppSettingsApi* CXmppSettingsApi::NewLC()
    {
    CXmppSettingsApi* self = new( ELeave ) CXmppSettingsApi;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


EXPORT_C CXmppSettingsApi::~CXmppSettingsApi()
    {
    delete iRepository;
    delete iRecords;
    delete iColumns;
    }


// ---------------------------------------------------------------------------
// This method creates new setting record.
// ---------------------------------------------------------------------------
//
EXPORT_C void CXmppSettingsApi::CreateSettingsRecordL(
    TUint32& aSettingsRecordId )
    {
   // GFLOG1("CXmppSettingsApi::CreateSettingsRecordL - IN");

    RArray<TUint32> array;
    CleanupClosePushL( array );

    iRecords->GetSettingsRecordkeysL( array );

    if( 0 == array.Count() )
        {
        RBuf tmp;
        tmp.CreateL( KMaxDescLength );

        TInt error = iRepository->Get( KXmppKeyColumnTable,
                                       tmp );
        tmp.Close();
       // GFLOG2("CXmppSettingsApi::CreateSettingsrecord - error: %d",error);
        if ( error )
            {
           // GFLOG1("CXmppSettingsApi::CreateSettingsrecord - first time ever!");
           // GFLOG1("CXmppSettingsApi::CreateSettingsrecord - creating KColSettingId");

            // create column for KColSettingId.
            User::LeaveIfError( iRepository->Create( KXmppKeyColumnTable,
                                                     KColSettingId() ) );

           // GFLOG1("CXmppSettingsApi::CreateSettingsrecord - Initialize settingId record");
            // Initialize settingId record. Initial value is zero
            User::LeaveIfError( iRepository->Create( KCurrentSettingIdIndex,
                                                     0 ) );
            }
        }

    iRecords->CreateSettingsRecordIdL( aSettingsRecordId );

    CleanupStack::PopAndDestroy( &array );

   // GFLOG1("CXmppSettingsApi::CreateSettingsRecordL - OUT");
    }

// ---------------------------------------------------------------------------
// This method removes settings record from the central repository.
// ---------------------------------------------------------------------------
//
EXPORT_C void CXmppSettingsApi::RemoveSettingsRecordL(
    TUint32 aSettingsRecordId )
    {
   // GFLOG1("CXmppSettingsApi::RemoveSettingsRecordL - IN");

    TUint32 settingKey( 0 );
    // First, check that this setting id does exist
    iRecords->FindSettingsRecordKeyL( aSettingsRecordId,
                                      settingKey );

    TUint32 key( KXmppKeyDataTable | settingKey );

    TUint32 errorKey(0);

    User::LeaveIfError( iRepository->Delete( key, KXmppMaskColumns, errorKey ) );

   // GFLOG2("CXmppSettingsApi::RemoveSettingStorageL - errorKey: 0x%08x",errorKey);
   // GFLOG2("CXmppSettingsApi::RemoveSettingStorageL - errorKey: %d",errorKey);
   // GFLOG1("CXmppSettingsApi::RemoveSettingsRecordL - OUT");
    }

// ---------------------------------------------------------------------------
// This method returns the default settings record (the first).
// ---------------------------------------------------------------------------
//
EXPORT_C void CXmppSettingsApi::DefaultSettingsRecordL(
    TUint32& aSettingsRecordId)
    {
   // GFLOG1("CXmppSettingsApi::DefaultSettingsRecordL - IN");
    RArray<TUint32> crArray;
    CleanupClosePushL( crArray );

    iRecords->GetSettingsRecordkeysL( crArray );

    TInt settingId( 0 );

    if( crArray.Count() > 0 )
        {
        User::LeaveIfError( iRepository->Get( crArray[0], settingId ) );
        }
    

    CleanupStack::PopAndDestroy( &crArray );

    aSettingsRecordId = settingId;

   // GFLOG1("CXmppSettingsApi::DefaultSettingsRecordL - OUT");
    }

// ---------------------------------------------------------------------------
// This method returns all setting id.s
// ---------------------------------------------------------------------------
//
EXPORT_C void CXmppSettingsApi::GetSettingsRecordIdArrayL(
    RArray<TUint32>& aArray )
    {
   // GFLOG1("CXmppSettingsApi::GetSettingsRecordIdArrayL - IN");

    RArray<TUint32> crArray;
    CleanupClosePushL( crArray );

    iRecords->GetSettingsRecordkeysL( crArray );

    TInt id( 0 );
    for ( TInt i=0; i < crArray.Count(); i++ )
        {
        User::LeaveIfError( iRepository->Get( crArray[i], id ) );
        aArray.Append( id );
        }

    CleanupStack::PopAndDestroy( &crArray );
   // GFLOG1("CXmppSettingsApi::GetSettingsRecordIdArrayL - OUT");
    }

// ---------------------------------------------------------------------------
// This method sets the parameter. If column does not exist. it creates new
// one.
// ---------------------------------------------------------------------------
//
EXPORT_C void CXmppSettingsApi::SetParamL(
    TUint32 aSettingsRecordId,
    const TDesC& aAttrName,
    const TDesC& aAttrValue )
    {
   // GFLOG1("CXmppSettingsApi::SetParamL - IN");
    // GFLOG4("    CXmppSettingsApi::SetParamL - aSettingsRecordId: %d: PARAM[ %S ][ %S ]",aSettingsRecordId,&aAttrName,&aAttrValue);
    TUint32 theKey( 0 );

    GetColumnKeyL( aSettingsRecordId,
                  aAttrName,
                  ETrue, // create column if not found.
                  theKey );

    RBuf buf;
    buf.CreateL( KMaxDescLength );

    // We do this just make sure that the key exists. Overflow is just one of
    // the indicators.
    TInt error = iRepository->Get( theKey, buf );

    buf.Close();

    // If column is not created for this setting id, then create it (to SETTING-table).
    if ( error != KErrNotFound &&
         error != KErrNone &&
         error != KErrOverflow )
        {
       // GFLOG1("CXmppSettingsApi::SetParamL - leaving...");
        User::Leave( error );
        }
    else if ( error == KErrNotFound )
        {
       // GFLOG1("CXmppSettingsApi::SetParamL - Creating new column..");
        User::LeaveIfError( iRepository->Create( theKey, aAttrValue ) );
        }
    else
        {
       // GFLOG1("CXmppSettingsApi::SetParamL - Setting value to the column..");
        // And finally, set the value to the column id in the setting id's row ( SETTING-table).
        User::LeaveIfError( iRepository->Set( theKey, aAttrValue ) );
        }

   // GFLOG1("CXmppSettingsApi::SetParamL - OUT");
    }

// ---------------------------------------------------------------------------
// This method sets the parameter. If column does not exist. it creates new
// one.
// ---------------------------------------------------------------------------
//
EXPORT_C void CXmppSettingsApi::SetParamL(
    TUint32 aSettingsRecordId,
    const TDesC& aAttrName,
    TInt aAttrValue )
    {
   // GFLOG1("CXmppSettingsApi::SetParamL - IN");
    // GFLOG4("    CXmppSettingsApi::SetParamL - aSettingsRecordId: %d: PARAM[ %S ][ %d ]",aSettingsRecordId,&aAttrName,aAttrValue);
    TUint32 theKey( 0 );

    GetColumnKeyL( aSettingsRecordId,
                  aAttrName,
                  ETrue,
                  theKey );

    TInt paramValue( 0 );

    TInt error = iRepository->Get( theKey, paramValue );

    // If column is not created for this setting id, then create it (to SETTING-table).
    if ( error != KErrNotFound &&
         error != KErrNone )
        {
       // GFLOG1("CXmppSettingsApi::SetParamL - leaving...");
        User::Leave( error );
        }
    else if ( error == KErrNotFound )
        {
       // GFLOG1("CXmppSettingsApi::SetParamL - Creating new column..");
        User::LeaveIfError( iRepository->Create( theKey, aAttrValue ) );
        }
    else
        {
       // GFLOG1("CXmppSettingsApi::SetParamL - Setting value to the column..");
        // And finally, set the value to the column id in the setting id's row ( SETTING-table).
        User::LeaveIfError( iRepository->Set( theKey, aAttrValue ) );
        }
   // GFLOG1("CXmppSettingsApi::SetParamL - OUT");
    }

// ---------------------------------------------------------------------------
// This method gets the value. If column does not exits it leaves.
// ---------------------------------------------------------------------------
//
EXPORT_C void CXmppSettingsApi::GetParamL(
    TUint32 aSettingsRecordId,
    const TDesC& aAttrName,
    TDes& aAttrValue )
    {
   // GFLOG1("CXmppSettingsApi::GetParamL - IN");
    // GFLOG3("    CXmppSettingsApi::GetParamL - aSettingsRecordId: %d: PARAM[ %S ]",aSettingsRecordId,&aAttrName);
    TUint32 theKey( 0 );

    GetColumnKeyL( aSettingsRecordId,
                  aAttrName,
                  EFalse, // Don't create column but leave if not found.
                  theKey );

    User::LeaveIfError( iRepository->Get( theKey, aAttrValue ) );
   // GFLOG1("CXmppSettingsApi::GetParamL - OUT");
    }

// ---------------------------------------------------------------------------
// This method gets the value. If column does not exits it leaves.
// ---------------------------------------------------------------------------
//
EXPORT_C void CXmppSettingsApi::GetParamL(
    TUint32 aSettingsRecordId,
    const TDesC& aAttrName,
    TInt& aAttrValue )
    {
   // GFLOG1("CXmppSettingsApi::GetParamL - IN");
    // GFLOG3("    CXmppSettingsApi::GetParamL - aSettingsRecordId: %d: PARAM[ %S ]",aSettingsRecordId,&aAttrName);
    TUint32 theKey( 0 );

    GetColumnKeyL( aSettingsRecordId,
                  aAttrName,
                  EFalse, // Don't create column but leave if not found.
                  theKey );

    User::LeaveIfError( iRepository->Get( theKey, aAttrValue ) );
   // GFLOG1("CXmppSettingsApi::GetParamL - OUT");
    }

// ---------------------------------------------------------------------------
// This method removes the value of the param from given settings record.
// ---------------------------------------------------------------------------
//
EXPORT_C void CXmppSettingsApi::RemoveParamL(
    TUint32 aSettingsRecordId,
    const TDesC& aAttrName )
    {
   // GFLOG1("CXmppSettingsApi::RemoveParamL - IN");
    TUint32 settingKey( 0 );
    // First, check that this setting id does exist
    iRecords->FindSettingsRecordKeyL( aSettingsRecordId,
                                      settingKey );

    TUint32 colKey( 0 );

    // Find out the id of the column (and does the column exist)
    iColumns->FindByNameL( aAttrName,
                                 colKey );

   // GFLOG1("CXmppSettingsApi::RemoveParamL - Delete item");
    TUint32 removeKey( settingKey | colKey );

   // GFLOG2("CXmppSettingsApi::RemoveParamL - colKey:        0x%08x", colKey );
   // GFLOG2("CXmppSettingsApi::RemoveParamL - settingKey:    0x%08x", settingKey );
   // GFLOG2("CXmppSettingsApi::RemoveParamL - removeKey:     0x%08x", removeKey );

    User::LeaveIfError( iRepository->Delete( removeKey ) );
   // GFLOG1("CXmppSettingsApi::RemoveParamL - OUT");
    }


// ---------------------------------------------------------------------------
// Do-method for all the SetParam-methods.
// ---------------------------------------------------------------------------
//
EXPORT_C void CXmppSettingsApi::GetRepositoryKeyL(
    TUint32 aSettingsRecordId,
    const TDesC& aAttrName,
    TBool aCreateColumnIfNotExist,
    TUint32& aTheKey )
    {
	 GetColumnKeyL( aSettingsRecordId,
                  aAttrName,
                  aCreateColumnIfNotExist , // create column if not found.
                  aTheKey );
    }
    
// PRIVATE


// ---------------------------------------------------------------------------
// Do-method for all the SetParam-methods.
// ---------------------------------------------------------------------------
//
void CXmppSettingsApi::GetColumnKeyL(
    TUint32 aSettingsRecordId,
    const TDesC& aAttrName,
    TBool aCreateColumnIfNotExist,
    TUint32& aTheKey )
    {

    TUint32 settingKey( 0 );
    // First, check that this setting id does exist.
    iRecords->FindSettingsRecordKeyL( aSettingsRecordId,
                                      settingKey );

    // Try to find Column number from the COLUMN-TABLE.
    TUint32 col( 0 );
    iColumns->GetColumnKeyL( aAttrName,
                            aCreateColumnIfNotExist,
                            col );

    // Find that is column already created to this setting id in SETTING-table.

    aTheKey = KXmppKeyDataTable | settingKey | col;

   // GFLOG2("CXmppSettingsApi::GetColumnKey - table | settingKey | aTheKey: 0x%08x", aTheKey);

    }
