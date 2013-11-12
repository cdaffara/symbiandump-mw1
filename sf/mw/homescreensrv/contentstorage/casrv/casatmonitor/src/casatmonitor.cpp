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
* Description : Used for receive SIM Application name, icon or
*               visibility information.
*
*/

#include <e32property.h>
#include "cadef.h"
#include "ca2internalCRkeys.h"
#include "cainnerquery.h"
#include "cainnerentry.h"
#include "caarraycleanup.inl"
#include "castorageproxy.h"
#include "casatmonitor.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCaSatMonitor::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCaSatMonitor* CCaSatMonitor::NewL( TPluginParams* aPluginParams )
    {
    CCaSatMonitor* self = new( ELeave ) CCaSatMonitor( *aPluginParams->storageProxy );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

// -----------------------------------------------------------------------------
// CCaSatMonitor::~CCaSatMonitor
// Destructor.
// -----------------------------------------------------------------------------
CCaSatMonitor::~CCaSatMonitor()
    {
    delete iCaSatNotifierVis;
    delete iCaSatNotifierIcon;
    delete iCaSatNotifierName;
    }

// -----------------------------------------------------------------------------
// CCaSatMonitor::CCaSatMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCaSatMonitor::CCaSatMonitor( CCaStorageProxy& aCaStorageProxy ) :
    iCaStorageProxy( aCaStorageProxy )
    {
    }

// -----------------------------------------------------------------------------
// CCaSatMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCaSatMonitor::ConstructL()
    {
    iCaSatNotifierName = CCaSatNotifier::NewL(
            this, KCRUidCa, KCaSatUIName );
    iCaSatNotifierIcon = CCaSatNotifier::NewL(
            this, KCRUidCa, KCaSatUIIconId );
    iCaSatNotifierVis = CCaSatNotifier::NewL(
            this, KCRUidCa, KCaShowSatUI );
    //first run of sat monitor to update storage data
    SatChangeL();
    }

// -----------------------------------------------------------------------------
// CCaSatMonitor::SatChangeL
// -----------------------------------------------------------------------------
//
void CCaSatMonitor::SatChangeL()
    {
    CCaInnerQuery* satAppQuery = CCaInnerQuery::NewLC();
    satAppQuery->SetUid( KSatUid.iUid );
    RPointerArray<CCaInnerEntry> resultArray;
    CleanupResetAndDestroyPushL( resultArray );
    iCaStorageProxy.GetEntriesL( satAppQuery, resultArray );

    TInt visibility( 0 );
    //We handle error in case of not inserted sim card
    //( RProperty could be not created yet for sat )
    TInt error = GetSatVisibility( visibility );
    if( ( error == KErrNone ) && visibility )
        {
        RBuf satName;
        satName.CleanupClosePushL();
        satName.CreateL( KCenRepBufferSize );
        User::LeaveIfError( GetSatName( satName ) );

        if( !resultArray.Count() )
            {
            //sat app was added we ned to add new entry to storage
            //we will put new sat entry into array for simpler handling
            CCaInnerEntry* newSat = CCaInnerEntry::NewLC();
            resultArray.AppendL( newSat );
            CleanupStack::Pop( newSat );
            }
        resultArray[0]->SetUid( KSatUid.iUid );
        resultArray[0]->SetRole( EItemEntryRole );
        resultArray[0]->SetEntryTypeNameL( KCaTypeApp );
        resultArray[0]->SetTextL( satName );
        resultArray[0]->AddAttributeL( KCaAttrShortName, satName );
        resultArray[0]->SetFlags( resultArray[0]->GetFlags() | EVisible );

        iCaStorageProxy.AddL( resultArray[0] );

        CleanupStack::PopAndDestroy( &satName );
        }
    else
        {
        if( resultArray.Count() && resultArray[0]->GetFlags() & EVisible )
            {
            resultArray[0]->SetFlags(
                    resultArray[0]->GetFlags() & ~EVisible );
            iCaStorageProxy.AddL( resultArray[0] );
            }
        }

    CleanupStack::PopAndDestroy( &resultArray );
    CleanupStack::PopAndDestroy( satAppQuery );
    }

// ---------------------------------------------------------------------------
// CCaSatHandler::GetSatName
// ---------------------------------------------------------------------------
//
TInt CCaSatMonitor::GetSatName( TDes& aName )
    {
    return RProperty::Get( KCRUidCa, KCaSatUIName, aName );
    }

// ---------------------------------------------------------------------------
// CCaSatHandler::GetSatVisibility
// ---------------------------------------------------------------------------
//
TInt CCaSatMonitor::GetSatVisibility( TInt& aVisibility )
    {
    return RProperty::Get( KCRUidCa, KCaShowSatUI, aVisibility );
    }

//  End of File
