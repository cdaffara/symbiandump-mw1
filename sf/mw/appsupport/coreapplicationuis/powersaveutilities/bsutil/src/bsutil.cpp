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
* Description:  Battery state utility component providing percentual information
*                about battery capacity.
*
*/


// INCLUDE FILES
#include <e32svr.h>
#include <centralrepository.h>
#include <e32property.h>
#include <hwrmpowerstatesdkpskeys.h>
#include "bsutilprivatecrkeys.h"
#include "bsutil.h"
#include "trace.h"

// CONSTANTS
const TUint8 KMinBatteryCapacity = 0;       // 0 %
const TUint8 KMaxBatteryCapacity = 100;     // 100 %
const TUint8 KInitialBatteryCapacity = 100; // 100 %

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CBSUtil::CBSUtil
// C++ constructor
// -----------------------------------------------------------------------------
//
CBSUtil::CBSUtil() : 
    iBatteryCapacity( KInitialBatteryCapacity )
    {
    FUNC_LOG
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CBSUtil::~CBSUtil()
    {
    FUNC_LOG
    }

// -----------------------------------------------------------------------------
// CHWRMVibra::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CBSUtil* CBSUtil::NewL()
    {
    FUNC_LOG
    
    CBSUtil* self = NewLC();
    CleanupStack::Pop( self );

    return self;
    }
   
// -----------------------------------------------------------------------------
// CBSUtil::NewLC
// Two-phased constructor. Leaves instance to CleanupStack.
// -----------------------------------------------------------------------------
//
EXPORT_C CBSUtil* CBSUtil::NewLC()
    {
    FUNC_LOG
    
    CBSUtil* self = new( ELeave ) CBSUtil();
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }

// -----------------------------------------------------------------------------
// CBSUtil::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CBSUtil::ConstructL()
    {
    FUNC_LOG
    
    TInt err( KErrNone );

    // If reading of battery levels fails, use default levels.
    TRAP( err, ReadBatteryLevelsL() );

    if( err != KErrNone )
        {
        INFO_1( "CBSUtil::ConstructL - Reading battery levels err %d", err )
        
        const TUint8 defaultLevels[KBatteryLevelCount] =
            {
            0, 10, 20, 30, 40, 50, 70, 100
            };
        
        for( TInt i = 0; i < KBatteryLevelCount; i++ )
            {
            iBattelyLevels[i] = defaultLevels[i];
            }
        }

    // Update current battery level and capacity data
    UpdateCapacity();
    }
    
// -----------------------------------------------------------------------------
// CBSUtil::ReadBatteryLevelsL
// -----------------------------------------------------------------------------
//
void CBSUtil::ReadBatteryLevelsL()
    {
    FUNC_LOG

    CRepository* repository;
    
    repository = CRepository::NewLC( KCRUidBSUtilConf );
    
    User::LeaveIfError(repository->Get(KBSUtilBatteryLevel0, 
        iBattelyLevels[EBatteryLevelLevel0]));
    User::LeaveIfError(repository->Get(KBSUtilBatteryLevel1,
        iBattelyLevels[EBatteryLevelLevel1]));
    User::LeaveIfError(repository->Get(KBSUtilBatteryLevel2, 
        iBattelyLevels[EBatteryLevelLevel2]));
    User::LeaveIfError(repository->Get(KBSUtilBatteryLevel3, 
        iBattelyLevels[EBatteryLevelLevel3]));
    User::LeaveIfError(repository->Get(KBSUtilBatteryLevel4, 
        iBattelyLevels[EBatteryLevelLevel4]));
    User::LeaveIfError(repository->Get(KBSUtilBatteryLevel5, 
        iBattelyLevels[EBatteryLevelLevel5]));
    User::LeaveIfError(repository->Get(KBSUtilBatteryLevel6, 
        iBattelyLevels[EBatteryLevelLevel6]));
    User::LeaveIfError(repository->Get(KBSUtilBatteryLevel7, 
        iBattelyLevels[EBatteryLevelLevel7]));
        
    // Let's validate battery levels' configuration. I.e. check 
    // that in valid  range and in increasing percentual order.
    for( TInt i(0); i < KBatteryLevelCount; i++ )
        {
        if( ( i < (KBatteryLevelCount - 1) ) &&
          ( iBattelyLevels[i] < KMinBatteryCapacity || 
            iBattelyLevels[i] > KMaxBatteryCapacity || 
            iBattelyLevels[i] > iBattelyLevels[i+1] ) )
            {
            User::Leave( KErrArgument );
            }
        else if( ( i > 0 ) &&
            ( iBattelyLevels[i] < KMinBatteryCapacity || 
              iBattelyLevels[i] > KMaxBatteryCapacity || 
              iBattelyLevels[i-1] > iBattelyLevels[i] ) )
            {
            User::Leave( KErrArgument );
            }
        }
    
    CleanupStack::PopAndDestroy( repository );
    }

// -----------------------------------------------------------------------------
// CBSUtil::UpdateCapacity
//
// -----------------------------------------------------------------------------
// 
TUint8 CBSUtil::UpdateCapacity()
    {
    FUNC_LOG
    
    TInt value( 0 );
    TInt err( RProperty::Get( KPSUidHWRMPowerState, KHWRMBatteryLevel, value ) );
    
    INFO_2( "CBSUtil::UpdateCapacity - KHWRMBatteryLevel %d, err %d", value, err )
    
    if( err == KErrNone )
        {
        // Update battery level and capacity information
        if( value >= EBatteryLevelLevel0 &&
            value <= EBatteryLevelLevel7 )
            {
            iBatteryCapacity = iBattelyLevels[value];
            }
        }
    
    return iBatteryCapacity;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

EXPORT_C TUint8 CBSUtil::GetBatteryCapacity()
    {
    FUNC_LOG
    
    return UpdateCapacity();
    }
    
//  End of File  
