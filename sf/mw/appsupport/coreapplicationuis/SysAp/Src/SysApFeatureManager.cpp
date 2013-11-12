/*
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApFeatureManager implementation.
*
*/


// INCLUDES

#include <e32svr.h> // RDebug
#include <featmgr.h>
#include "SysApFeatureManager.h"
#include "SysAp.hrh"
#include <centralrepository.h>
#include "coreapplicationuisprivatecrkeys.h"
#include <hwrmlight.h>
#include <AknUtils.h>


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApFeatureManager::NewL() 
// ----------------------------------------------------------------------------
CSysApFeatureManager* CSysApFeatureManager::NewL()
    {
    CSysApFeatureManager* self = new(ELeave) CSysApFeatureManager;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApFeatureManager::ConstructL() 
// ----------------------------------------------------------------------------
void CSysApFeatureManager::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL()") ) );
    FeatureManager::InitializeLibL();
    iFeatMgrActive = ETrue;
    
    iSimSupported = FeatureManager::FeatureSupported( KFeatureIdSimCard );
    TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: SIM card supported=%d"), iSimSupported ) );

    iOfflineModeSupported = FeatureManager::FeatureSupported( KFeatureIdOfflineMode );
    TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: Offline mode supported=%d"), iOfflineModeSupported ) );
    
    iCoverDisplaySupported = FeatureManager::FeatureSupported( KFeatureIdCoverDisplay );
    TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: Cover display supported=%d"), iCoverDisplaySupported ) );
    
    iGripNotSupported = FeatureManager::FeatureSupported( KFeatureIdKeypadNoSlider );
    TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: iGripNotSupported=%d"), iGripNotSupported ) );
    
    iLocationPrivacySupported = FeatureManager::FeatureSupported( KFeatureIdPrivacyFramework );
    TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: Location Privacy supported=%d"), iLocationPrivacySupported ) );

    iMMCSupported = FeatureManager::FeatureSupported( KFeatureIdMmc );
    TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: MMC supported=%d"), iMMCSupported ) );
        
    iMMCHotSwapSupported = FeatureManager::FeatureSupported( KFeatureIdMmcHotswap );
    TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: MMC HotSwap supported=%d"), iMMCHotSwapSupported ) );
    
    iStarterExtensionsSupported = FeatureManager::FeatureSupported( KFeatureIdExtendedStartup );
    TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: Starter Extensions supported=%d"), iStarterExtensionsSupported ) );

    iCameraSupported = FeatureManager::FeatureSupported( KFeatureIdCamera );
    TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: Camera supported=%d"), iCameraSupported ) );

    iBtSupported = FeatureManager::FeatureSupported( KFeatureIdBt );
    TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: Bluetooth supported=%d"), iBtSupported ) );

    iLockSupported = FeatureManager::FeatureSupported( KFeatureIdMmcLock );
    TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: Memory card lock supported=%d"), iLockSupported ) );
    
    iEject = FeatureManager::FeatureSupported( KFeatureIdMmcEject );
    TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: Memory card sw eject supported=%d"), iEject ) );
    
    iPowerSaveSupported = FeatureManager::FeatureSupported( KFeatureIdPowerSave );
    TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: Power save supported=%d"), iPowerSaveSupported ) );

    iNoPowerKeySupported = FeatureManager::FeatureSupported( KFeatureIdNoPowerkey );
    TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: No power key supported=%d"), iNoPowerKeySupported ) );
    
//    iFmTxSupported = FeatureManager::FeatureSupported( KFeatureIdFmtx );
//    TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: FM TX supported=%d"), iFmTxSupported ) );

//    iPenEnabled = AknLayoutUtils::PenEnabled();
    TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: Pen enabled=%d"), iPenEnabled ) );
    
    iVmbxCallDivertIconSupported = FeatureManager::FeatureSupported( KFeatureIdVmbxCallDivertIcon );
    TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: VMBX call divert icon supported=%d"), iVmbxCallDivertIconSupported ) );
    
    iTouchUnlockStrokeSupported = FeatureManager::FeatureSupported( KFeatureIdFfTouchUnlockStroke );
    TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: Touch unlock stroke supported=%d"), iTouchUnlockStrokeSupported ) );
    
//    iFmTxRdsTextSupported = FeatureManager::FeatureSupported( KFeatureIdFfFmtxRdsText );
//    TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: FM TX RDS-TEXT supported=%d"), iFmTxRdsTextSupported ) );
    
    CRepository* repository = NULL;
    
    TRAPD( err, repository = CRepository::NewL( KCRUidCoreApplicationUIsConf ) );
    
    if ( err == KErrNone )
        {
        err = repository->Get( KCoreAppUIsSysAp, iSysApFeatures );
        if ( err != KErrNone || iSysApFeatures < 0 ) // ERROR
            {
            TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: ERROR: %d, KCoreAppUIsSysAp=%d"), err, iSysApFeatures ) );
            iSysApFeatures = 0;
            }

        TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: KCoreAppUIsSysAp=%d"), iSysApFeatures ) );

        err = repository->Get( KCoreAppUIsHotSwapMemoryCardHatch, iHatchSupported );
        if ( err != KErrNone || iHatchSupported < 0 ) // ERROR
            {
            TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: ERROR: %d, KCoreAppUIsHotSwapMemoryCardHatch=%d"), err, iHatchSupported ) );
            iHatchSupported = EFalse;
            }
            
        err = repository->Get( KCoreAppUIsLockedStateLights, iLockedStateLightsSupported );
        
        if ( err != KErrNone || iLockedStateLightsSupported < 0 ) // ERROR
            {
            TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: ERROR: %d, KCoreAppUIsLightsLocked=%d"), err, iLockedStateLightsSupported ) );
            iLockedStateLightsSupported = EFalse;
            iLockedStateLightsIntensity = KHWRMDefaultIntensity;
            }
        
        if ( iLockedStateLightsSupported ) // get intensity only if configuration is supported
            {
            err = repository->Get( KCoreAppUIsLockedStateLightsIntensity, iLockedStateLightsIntensity );
            
            if ( err != KErrNone || iLockedStateLightsIntensity < KHWRMLightMinIntensity || iLockedStateLightsIntensity > KHWRMLightMaxIntensity )
                {
                TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: ERROR: %d, KCoreAppUIsLightsLockedIntensity=%d"), err, iLockedStateLightsIntensity ) );
                iLockedStateLightsIntensity = KHWRMDefaultIntensity;
                }
            }        
        }
    else
        {
        TRACES( RDebug::Print( _L("CSysApFeatureManager::ConstructL: open KCRUidCoreApplicatinUIsConf failed, err=%d"), err ) );
        }        
    
    delete repository;
    }

// ----------------------------------------------------------------------------
// CSysApFeatureManager::CSysApFeatureManager()
// ----------------------------------------------------------------------------

CSysApFeatureManager::CSysApFeatureManager() : iFeatMgrActive( EFalse ),
                                               iSysApFeatures( 0 ),
                                               iEject( EFalse ),
                                               iLockSupported( EFalse ),
                                               iHatchSupported( EFalse ),
                                               iLockedStateLightsSupported( EFalse ),
                                               iLockedStateLightsIntensity( KHWRMDefaultIntensity ),
                                               iMMCSupported( EFalse ),
                                               iMMCHotSwapSupported( EFalse ),
                                               iOfflineModeSupported( EFalse ),
                                               iStarterExtensionsSupported( EFalse ),
                                               iLocationPrivacySupported( EFalse ),
                                               iCameraSupported( EFalse ),
                                               iBtSupported( EFalse ),
                                               iSimSupported( ETrue ),
                                               iGripNotSupported( ETrue ),
                                               iPowerSaveSupported( EFalse ),
                                               iNoPowerKeySupported( EFalse ),
                                               //iFmTxSupported( EFalse ),
                                               iPenEnabled( EFalse ),
                                               iVmbxCallDivertIconSupported( EFalse ),
                                               iTouchUnlockStrokeSupported( EFalse )
                                               //iFmTxRdsTextSupported( EFalse )

    {
    }

// ----------------------------------------------------------------------------
// CSysApFeatureManager::~CSysApFeatureManager() 
// ----------------------------------------------------------------------------

CSysApFeatureManager::~CSysApFeatureManager() 
    {
    if ( iFeatMgrActive )
        {
        FeatureManager::UnInitializeLib();    
        }
    }

// ----------------------------------------------------------------------------
// CSysApFeatureManager::FeatureVariationCheckDone() 
// ----------------------------------------------------------------------------
//
void CSysApFeatureManager::FeatureVariationCheckDone()
    {
    TRACES( RDebug::Print( _L("CSysApFeatureManager::FeatureVariationCheckDone()") ) );
    FeatureManager::UnInitializeLib();
    iFeatMgrActive = EFalse;
    }


// ----------------------------------------------------------------------------
// CSysApFeatureManager::Supported() 
// ----------------------------------------------------------------------------

TInt CSysApFeatureManager::Supported( TInt aSysApFeatureId )
    {
    return aSysApFeatureId & iSysApFeatures;
    }

// ----------------------------------------------------------------------------
// CSysApFeatureManager::EjectRequiredInPowerMenu()
// ----------------------------------------------------------------------------
TBool CSysApFeatureManager::EjectRequiredInPowerMenu()
    {
    return iEject;
    }

// ----------------------------------------------------------------------------
// CSysApFeatureManager::MemoryCardLockSupported()
// ----------------------------------------------------------------------------
TBool CSysApFeatureManager::MemoryCardLockSupported()
    {
    return iLockSupported;
    }

// ----------------------------------------------------------------------------
// CSysApFeatureManager::MemoryCardHatchSupported()
// ----------------------------------------------------------------------------
TBool CSysApFeatureManager::MemoryCardHatchSupported()
    {
    return iHatchSupported;
    }

// ----------------------------------------------------------------------------
// CSysApFeatureManager::LockedStateLightsSupported()
// ----------------------------------------------------------------------------
TBool CSysApFeatureManager::LockedStateLightsSupported()
    {
    return iLockedStateLightsSupported;
    }

// ----------------------------------------------------------------------------
// CSysApFeatureManager::LockedStateLightsIntensity()
// ----------------------------------------------------------------------------
TInt CSysApFeatureManager::LockedStateLightsIntensity()
    {
    return iLockedStateLightsIntensity;
    }

// ----------------------------------------------------------------------------
// CSysApFeatureManager::LockedStateLightsIntensity()
// ----------------------------------------------------------------------------
//  
TBool CSysApFeatureManager::MmcSupported() const
    {
    return iMMCSupported;
    }

// ----------------------------------------------------------------------------
// CSysApFeatureManager::LockedStateLightsIntensity()
// ----------------------------------------------------------------------------
//
TBool CSysApFeatureManager::MmcHotSwapSupported() const
    {
    return iMMCHotSwapSupported;
    }

// ----------------------------------------------------------------------------
// CSysApFeatureManager::OfflineModeSupported()
// ----------------------------------------------------------------------------
//
TBool CSysApFeatureManager::OfflineModeSupported() const
    {
    return iOfflineModeSupported;
    }

// ----------------------------------------------------------------------------
// CSysApFeatureManager::StarterExtensionsSupported()
// ----------------------------------------------------------------------------
//
TBool CSysApFeatureManager::StarterExtensionsSupported() const
    {
    return iStarterExtensionsSupported;
    }

// ----------------------------------------------------------------------------
// CSysApFeatureManager::LocationPrivacySupported()
// ----------------------------------------------------------------------------
//
TBool CSysApFeatureManager::LocationPrivacySupported() const
    {
    return iLocationPrivacySupported;
    }

// ----------------------------------------------------------------------------
// CSysApFeatureManager::CameraSupported()
// ----------------------------------------------------------------------------
//
TBool CSysApFeatureManager::CameraSupported() const
    {
    return iCameraSupported;
    }

// ----------------------------------------------------------------------------
// CSysApFeatureManager::BtSupported()
// ----------------------------------------------------------------------------
//
TBool CSysApFeatureManager::BtSupported() const
    {
    return iBtSupported;
    }

// ----------------------------------------------------------------------------
// CSysApFeatureManager::SimSupported()
// ----------------------------------------------------------------------------
//
TBool CSysApFeatureManager::SimSupported() const
    {
    return iSimSupported;
    }

// ----------------------------------------------------------------------------
// CSysApFeatureManager::GripNotSupported()
// ----------------------------------------------------------------------------
//
TBool CSysApFeatureManager::GripNotSupported() const
    {
    return iGripNotSupported;
    }
    
// ----------------------------------------------------------------------------
// CSysApFeatureManager::CoverDisplaySupported()
// ----------------------------------------------------------------------------
//
TBool CSysApFeatureManager::CoverDisplaySupported() const
    {
    return iCoverDisplaySupported;
    }

// ----------------------------------------------------------------------------
// CSysApFeatureManager::PowerSaveSupported()
// ----------------------------------------------------------------------------
//
TBool CSysApFeatureManager::PowerSaveSupported() const
    {
    return iPowerSaveSupported;
    }
    
// ----------------------------------------------------------------------------
// CSysApFeatureManager::NoPowerKeySupported()
// ----------------------------------------------------------------------------
//  
TBool CSysApFeatureManager::NoPowerKeySupported() const
    {
    return iNoPowerKeySupported;
    }
/*
// ----------------------------------------------------------------------------
// CSysApFeatureManager::FmTxSupported()
// ----------------------------------------------------------------------------
//    
TBool CSysApFeatureManager::FmTxSupported() const
    {
    return iFmTxSupported;
    }
*/

// ----------------------------------------------------------------------------
// CSysApFeatureManager::PenEnabled()
// ----------------------------------------------------------------------------
//
TBool CSysApFeatureManager::PenEnabled() const
    {
    return iPenEnabled;
    }

// ----------------------------------------------------------------------------
// CSysApFeatureManager::VmbxCallDivertIconSupported()
// ----------------------------------------------------------------------------
//     
TBool CSysApFeatureManager::VmbxCallDivertIconSupported() const
    {
    return iVmbxCallDivertIconSupported;
    }        

// ----------------------------------------------------------------------------
// CSysApFeatureManager::TouchUnlockStrokeSupported()
// ----------------------------------------------------------------------------
//     
TBool CSysApFeatureManager::TouchUnlockStrokeSupported() const
    {
    return iTouchUnlockStrokeSupported;
    }
/*
// ----------------------------------------------------------------------------
// CSysApFeatureManager::FmTxRdsTextSupported()
// ----------------------------------------------------------------------------
//     
TBool CSysApFeatureManager::FmTxRdsTextSupported() const
    {
    return iFmTxRdsTextSupported;
    }
*/
// End of File



