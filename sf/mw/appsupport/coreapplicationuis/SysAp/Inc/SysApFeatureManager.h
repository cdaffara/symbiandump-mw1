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
* Description:  CSysApFeatureManager
*                Reads SysAp settings from 101F864B.ini and offers
*                methods for getting them.
*
*/


#ifndef SYSAPFEATUREMANAGER_H
#define SYSAPFEATUREMANAGER_H

// INCLUDES
#include <e32base.h>

// CONSTANTS


// CLASS DECLARATION
/**
*  CSysApFeatureManager
*  
*  @lib   sysap
*  @since 2.1* 
*/

class CSysApFeatureManager: public CBase
{
public:

    /**
    * Constructor
    */         
    static CSysApFeatureManager* NewL();

    /**
    * Destructor.
    */
    virtual ~CSysApFeatureManager();

    /**
    * Used to inform that FeatureManager session is no longer needed.
    *
    * @since S60 3.2
    */
    void FeatureVariationCheckDone();
    
    /**
    * This method can
    * @param TInt aSysApFeatureId (defined in CoreApplicationUIsPrivateCRKeys.h)  
    * @return TInt: aSysApFeatureId supported if != 0
    */         
    TInt Supported( TInt aSysApFeatureId );

    /**
     * Checks whether SysAp has been configured to have Eject in powerkey menu.
     * @param None
     * @return TBool
     */                   
    TBool EjectRequiredInPowerMenu();
    
    /**
     * Checks whether SysAp has been configured to support locked memory cards
     * @param None
     * @return TBool
     */                   
    TBool MemoryCardLockSupported();
    
    /**
     * Checks whether SysAp has been configured to support memory card hatch
     *
     * @since S60 3.1
     * @param None
     * @return TBool
     */                   
    TBool MemoryCardHatchSupported();
    
    /**
    * Checks whether SysAp has been configured to lit lights when device or
    * keypad is locked.
    *
    * @since S60 3.1
    * @param None
    * @return TBool
    */                   
    TBool LockedStateLightsSupported();
    
    /**
    * Returns the intensity that should be used for lights when locked state light
    * handling is applied.
    *
    * @since S60 3.1
    * @param None
    * @return TInt value range defined in Light API (SDK)
    * @see CHWRMLight
    */                   
    TInt LockedStateLightsIntensity();
    
    /**
    * Returns whether memory card is supported.
    *
    * @since S60 3.2
    * @return ETrue if feature is supported
    */  
    TBool MmcSupported() const;
    
    /**
    * Returns whether memory card hot swap is supported.
    *
    * @since S60 3.2
    * @return ETrue if feature is supported
    */  
    TBool MmcHotSwapSupported() const;
    
    /**
    * Returns whether offline mode is supported.
    *
    * @since S60 3.2
    * @return ETrue if feature is supported
    */  
    TBool OfflineModeSupported() const;
    
    /**
    * Returns whether Starter extensions are supported.
    *
    * @since S60 3.2
    * @return ETrue if feature is supported
    */  
    TBool StarterExtensionsSupported() const;
    
    /**
    * Returns whether Location Privacy is supported.
    *
    * @since S60 3.2
    * @return ETrue if feature is supported
    */  
    TBool LocationPrivacySupported() const;
    
    /**
    * Returns whether camera is supported.
    *
    * @since S60 3.2
    * @return ETrue if feature is supported
    */  
    TBool CameraSupported() const;
    
    /**
    * Returns whether Bluetooth is supported.
    *
    * @since S60 3.2
    * @return ETrue if feature is supported
    */  
    TBool BtSupported() const;
    
    /**
    * Returns whether SIM is supported.
    *
    * @since S60 3.2
    * @return ETrue if feature is supported
    */  
    TBool SimSupported() const;

    /**
    * Returns whether GripNotSupported is set.
    *
    * @since S60 3.2
    * @return ETrue if feature is set
    */  
    TBool GripNotSupported() const;
    
    /**
    * Returns whether Cover Display is supported.
    *
    * @since S60 3.2
    * @return ETrue if feature is supported
    */  
    TBool CoverDisplaySupported() const;
    
    /**
    * Returns whether Power Save Mode is supported.
    *
    * @since S60 3.2
    * @return ETrue if feature is supported
    */  
    TBool PowerSaveSupported() const;

    /**
    * Returns whether No power key is supported.
    *
    * @since S60 3.2
    * @return ETrue if feature is supported
    */  
    TBool NoPowerKeySupported() const;
    
    /**
    * Returns whether FM TX is supported.
    *
    * @since S60 3.2
    * @return ETrue if feature is supported
    */ 
    // TBool FmTxSupported() const;

    /**
    * Returns whether pen is enabled.
    *
    * @since S60 5.0
    * @return ETrue if feature is enabled
    */
    TBool PenEnabled() const;
    
    /**
    * Returns whether voice mail box call divert icon
    * is supported.
    *
    * @since S60 3.2
    * @return ETrue if feature is supported
    */
    TBool VmbxCallDivertIconSupported() const;
    
    /**
    * Returns whether touch unlock stroke feature
    * is supported.
    *
    * @since S60 5.0
    * @return ETrue if feature is supported
    */
    TBool TouchUnlockStrokeSupported() const;
    
    /**
    * Returns whether FM TX RDS Text feature
    * is supported.
    *
    * @since S60 5.1
    * @return ETrue if feature is supported
    */
    // TBool FmTxRdsTextSupported() const;

private:

    /**
    * Constructors
    */         
    CSysApFeatureManager();
    void ConstructL();

private:
    /**
    * FeatureManager session state, if ETrue FeatureManager is active
    * i.e. FeatureManager::InitializeLibL has been called.
    */
    TBool iFeatMgrActive;
    
    TInt  iSysApFeatures; // value of KCoreAppUIsSysAp (0x1) in 101F864B.txt
    TBool iEject;         // value of KCoreAppUIsHotSwapEjectAvailable (0x7) in 101F864B.txt
    TBool iLockSupported; // value of RKCoreAppUIsMemoryCardLockSupported (0x9) in 101F864B.txt
    TBool iHatchSupported; // value of KCoreAppUIsHotSwapMemoryCardHatch (0x08)
    
    /**
    * Value of KCoreAppUIsLockedStateLights
    */
    TBool iLockedStateLightsSupported;
    
    /**
    * Value of KCoreAppUIsLockedStateLightsIntensity
    */
    TInt iLockedStateLightsIntensity;
    
    /** 
    * Memory card support status.
    */
    TBool iMMCSupported;
    
    /**
    * Memory card hot swap support status.
    */
    TBool iMMCHotSwapSupported;
    
    /**
    * Offline mode support status.
    */
    TBool iOfflineModeSupported;
    
    /**
    * Starter Extensions support v.
    */
    TBool iStarterExtensionsSupported;
    
    /**
    * Location Privacy FW support status.
    */
    TBool iLocationPrivacySupported;
    
    /**
    * Camera support status.
    */
    TBool iCameraSupported;
    
    /**
    * Bluetooth support status.
    */
    TBool iBtSupported;
    
    /**
    * SIM support status.
    */
    TBool iSimSupported;
    
    /**
    * Grip not supported status.
    */ 
    TBool iGripNotSupported;
    
    /**
    * Cover display support status.
    */
    TBool iCoverDisplaySupported;

    /**
    * Power save mode support status.
    */
    TBool iPowerSaveSupported;
    
    /**
    * No power key supported status
    */
    TBool iNoPowerKeySupported;
    
    /**
    * FM TX supported status
    */
    // TBool iFmTxSupported;
    
    /**
    * Pen enabled status.
    */
    TBool iPenEnabled;
    
    /**
    * Voice mail box call divert supported status.
    */ 
    TBool iVmbxCallDivertIconSupported;
	    
    /**
    * Touch unlock stroke supported status.
    */
    TBool iTouchUnlockStrokeSupported;
    
    /**
    * FM TX RDS Text support status.
    */
    // TBool iFmTxRdsTextSupported;
};

#endif // SYSAPFEATUREMANAGER_H

// End of File
