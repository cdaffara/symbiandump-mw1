/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include <hbdevicedialogsymbian.h>
#include <hbsymbianvariant.h>
#include <coreapplicationuisdomainpskeys.h>
#include <CoreApplicationUIsSDKCRKeys.h>
#include "hbdevicepowermenusymbian.h"


//These Keys will be used in CHbSymbianVariantMap
//Donot Change these strings
_LIT(KVolumeLevel, "VolumeLevel");
_LIT(KSilenceLatchOn, "SilenceLatchOn");        //Used to latch silencebutton
_LIT(KCypherOff, "CypherOff");                  //used to poweroffButtonEnabled/Disabled
_LIT(KVibrationEnable, "VibrationEnable");
_LIT(KOfflineEnable, "OfflineEnable");
_LIT(KShutDown, "ShutDown");

//used to load plugin 
_LIT(KPluginIdentifier, "com.nokia.hb.devicepowermenu/1.0");


NONSHARABLE_CLASS(CHbDevicePowerMenuPrivate) : public CBase, public MHbDeviceDialogObserver
{
public:
    static CHbDevicePowerMenuPrivate* NewL(CHbDevicePowerMenuSymbian* aDialog);
    virtual ~CHbDevicePowerMenuPrivate();

    void ShowL();
    void AddVariantL(const TDesC& aKey, const TAny* aData, CHbSymbianVariant::TType aDataType);

    //Set Methods
    void SetVolume(TInt aVolume);
    void SetSilenceMode(TBool aEnable);
    void SetOfflineMode(TBool aChecked);
    void SetVibrationEnabled(TBool aEnable);
    void SetPowerOffEnabled(TBool aEnable);

public:
    // MHbDeviceDialogObserver
    void DataReceived(CHbSymbianVariantMap& aData);
    void DeviceDialogClosed(TInt aCompletionCode);

private: 
    CHbDevicePowerMenuPrivate();
    void ConstructL(CHbDevicePowerMenuSymbian* aDialog);

private:
    //symbian FW client intreface to load Qt.plugin
    CHbDeviceDialogSymbian*         iDeviceDialog;
    //Parameter for loading qt.plugin
    CHbSymbianVariantMap*           iVariantMap;
    //Symbian client interface for Powermenu
    CHbDevicePowerMenuSymbian*      iPowerMenuManager;

    //Variants used in VariantMap
    TInt        iVolumeLevel;           //Volume level ranges 1 to 10
    TBool       iSilenceLatchOn;        //Silence Enabled or Disbaled
    TBool       iVibrationEnable;       //Master Vibarate enabled/Disabled
    TBool       iOfflineEnable;         //Oflline Mode enabled or Disabled
    TBool       iCypherOff;             //Encryption/Decryption ON/OFF
};

/**
 * Create an object of CHbDevicePowerMenuPrivate
 */
CHbDevicePowerMenuPrivate* CHbDevicePowerMenuPrivate::NewL(CHbDevicePowerMenuSymbian* aDialog)
    {
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::NewL: START") ) );
    CHbDevicePowerMenuPrivate* self = new (ELeave) CHbDevicePowerMenuPrivate();
    CleanupStack::PushL(self);
    self->ConstructL(aDialog);
    CleanupStack::Pop(self);
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::NewL: END") ) );
    return self;
    }

/**
 * Constructor
 */
CHbDevicePowerMenuPrivate::CHbDevicePowerMenuPrivate()
    {
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::Constructor") ) );
    //Do nothing
    }

/**
 * Destructor
 */
CHbDevicePowerMenuPrivate::~CHbDevicePowerMenuPrivate()
    {
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::Destructor: START") ) );
    delete iDeviceDialog;
    delete iVariantMap;
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::Destructor: END") ) );
    }

/**
 * Constrcuts all its member variables
 */
void CHbDevicePowerMenuPrivate::ConstructL(CHbDevicePowerMenuSymbian* aDialog)
    {
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::ConstructL: START") ) ); 
    iPowerMenuManager = aDialog;
    iDeviceDialog = CHbDeviceDialogSymbian::NewL();
    iVariantMap = CHbSymbianVariantMap::NewL();
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::ConstructL: END") ) ); 
    }

/**
 * Builds all parameters required for showing the PowerMenu Dialog.
 * Loads the required powermenu plugin and calls show on DeviceDialog.
 * This Displays the Customized PowerMenu Widget. 
 */
void CHbDevicePowerMenuPrivate::ShowL()
    {
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::ShowL: START") ) ); 
    AddVariantL(KVolumeLevel, &iVolumeLevel, CHbSymbianVariant::EInt);
    AddVariantL(KSilenceLatchOn, &iSilenceLatchOn, CHbSymbianVariant::EBool);
    AddVariantL(KVibrationEnable, &iVibrationEnable, CHbSymbianVariant::EBool);
    AddVariantL(KOfflineEnable, &iOfflineEnable, CHbSymbianVariant::EBool);
    AddVariantL(KCypherOff, &iCypherOff, CHbSymbianVariant::EBool);
    //No shutdown varinat will be added to showL
    User::LeaveIfError(iDeviceDialog->Show(KPluginIdentifier, *iVariantMap, this));
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::ShowL: END") ) ); 
    }

/**
 * Sets Volume to aVolume
 */
void CHbDevicePowerMenuPrivate::SetVolume(TInt aVolume)
    {
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::SetVolume: START") ) ); 
    iVolumeLevel = aVolume;
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::SetVolume: END") ) ); 
    }

/**
 * Sets SilenceMode to Enable/Disable
 */
void CHbDevicePowerMenuPrivate::SetSilenceMode(TBool aEnable)
    {
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::SetSilenceMode: START") ) ); 
    iSilenceLatchOn = aEnable;
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::SetSilenceMode: END") ) ); 
    }

/**
 * Sets OfflineMode to Checked/UnChecked
 */
void CHbDevicePowerMenuPrivate::SetOfflineMode(TBool aChecked)
    {
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::SetOfflineMode: START") ) ); 
    iOfflineEnable = aChecked;
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::SetOfflineMode: END") ) ); 
    }

/**
 * Sets VibrationEnabled to Checked/UnChecked
 */
void CHbDevicePowerMenuPrivate::SetVibrationEnabled(TBool aChecked)
    {
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::SetVibrationEnabled: START") ) ); 
    iVibrationEnable = aChecked;
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::SetVibrationEnabled: END") ) ); 
    }

/**
 * Sets PowerOffEnabled to Enable/Disable
 */
void CHbDevicePowerMenuPrivate::SetPowerOffEnabled(TBool aEnable)
    {
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::SetPowerOffEnabled: START") ) ); 
    iCypherOff = aEnable;
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::SetPowerOffEnabled: END") ) ); 
    }

/**
 * Data received from HBDeviceDialog will hit by emit deviceDialogData in plugin
 * The values received from UI will set to ControlPanel.
 */
void CHbDevicePowerMenuPrivate::DataReceived(CHbSymbianVariantMap& aData)
    {
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::DataReceived: START") ) ); 
    // Updates received from the plugin.
	//1. Volume
	const CHbSymbianVariant* volumeVariant =  aData.Get(KVolumeLevel);
	if(volumeVariant)
		{
        TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::DataReceived: volumeVariant::Start") ) );
		TInt* volumeValue = volumeVariant->Value<TInt>();
		if(volumeValue)
			{
			iVolumeLevel = *volumeValue;
			iPowerMenuManager->iProfileEngine->SetMasterVolumeL( iVolumeLevel );
			}
	    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::DataReceived:volumeVariant::End") ) );
		}
	
	//2. Silence
	const CHbSymbianVariant* SilenceVariant =  aData.Get(KSilenceLatchOn);
	if(SilenceVariant)
		{
        TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::DataReceived: SilenceVariant::Start") ) );
		TBool* LatchValue = SilenceVariant->Value<TBool>();
		if(LatchValue)
			{
			iSilenceLatchOn = *LatchValue;
			iPowerMenuManager->iProfileEngine->SetSilenceModeL( iSilenceLatchOn );//member in CHbDevicePowerMenuSymbian
			}
		TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::DataReceived: SilenceVariant::End") ) );
		}
	
	//3. Vibration
	const CHbSymbianVariant* VibrationVariant =  aData.Get(KVibrationEnable);
	if(VibrationVariant)
		{
        TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::DataReceived: VibrationVariant::Start") ) );
		TBool* VibrationValue = VibrationVariant->Value<TBool>();
		if(VibrationValue)
			{
			iVibrationEnable = *VibrationValue;
			iPowerMenuManager->iProfileEngine->SetMasterVibraL( iVibrationEnable );//member in CHbDevicePowerMenuSymbian
			}
        TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::DataReceived: VibrationVariant::End") ) );
		}
	
	//4.Offline
	const CHbSymbianVariant* OfflineVariant =  aData.Get(KOfflineEnable);
	if(OfflineVariant)
		{
        TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::DataReceived: OfflineVariant::Start") ) );
		TBool* OfflineValue = OfflineVariant->Value<TBool>();
		if(OfflineValue)
			{
                        iOfflineEnable = *OfflineValue;
                        // negate the value before storing in cen rep as the n/w conn allowed key is complement of offline mode
                        User::LeaveIfError( iPowerMenuManager->iCenrepOffline->Set(KCoreAppUIsNetworkConnectionAllowed, !iOfflineEnable )  );
			}
        TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::DataReceived: OfflineVariant::End") ) );
		}
	
	//5.shutdown
	const CHbSymbianVariant* ShutDownVariant =  aData.Get(KShutDown);
	if(ShutDownVariant)
		{
        TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::DataReceived: ShutDownVariant::Start") ) );
		TBool* ShutDownValue = ShutDownVariant->Value<TBool>();
		if(ShutDownValue)
			{
			TBool shutDown = *ShutDownValue;
			if (shutDown)
			    {
                iPowerMenuManager->iSysApServer.HandleLongPowerKeyPressedL();
			    }
			
			}
        TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::DataReceived: ShutDownVariant::End") ) );
		}
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::DataReceived: END") ) ); 
	return;
    }

/**
 * DeviceDialogClosed will be executed by the event emit deviceDialogClosed in plugin.
 * This will be executed when   1. Tap out side the Dialog to Close
 *                              2. When Power Off Button is clicked. 
 */
void CHbDevicePowerMenuPrivate::DeviceDialogClosed(TInt /*aCompletionCode*/)
    {
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::DeviceDialogClosed") ) ); 
    // Inform sysap to release the memory of power menu custom dialog.
    RProperty::Set( KPSUidCoreApplicationUIs, KCoreAppUIsPowerMenuCustomDialogStatus, ECoreAppUIsPowerMenuCustomDialogOn );
    }

/**
 * Contrcuts VariantMap
 */
void CHbDevicePowerMenuPrivate::AddVariantL(const TDesC& aKey, const TAny* aData, CHbSymbianVariant::TType aDataType)
    {
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::AddVariantL: START") ) ); 
    CHbSymbianVariant *variant = CHbSymbianVariant::NewL(aData, aDataType);
    CleanupStack::PushL(variant);
    //iVariantMap takes the ownerhip of variant
    User::LeaveIfError(iVariantMap->Add(aKey, variant));
    CleanupStack::Pop(variant);
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuPrivate::AddVariantL: End") ) ); 
    }

/*
 * Creates an object of CHbDevicePowerMenuSymbian
*/
CHbDevicePowerMenuSymbian* CHbDevicePowerMenuSymbian::NewL(CSysApAppUi& aSysApServer)
    {
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuSymbian::NewL: START") ) ); 
    CHbDevicePowerMenuSymbian* self = new (ELeave) CHbDevicePowerMenuSymbian(aSysApServer);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuSymbian::NewL: END") ) ); 
    return self;
    }

/*
 *  Destructs the class.
*/
CHbDevicePowerMenuSymbian::~CHbDevicePowerMenuSymbian()
    {
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuSymbian::Destructor: START") ) ); 
    delete iCenrepProfile;
    delete iCenrepOffline;
    iProfileEngine->Release();
    delete iPowerMenuPrivate;
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuSymbian::Destructor: End") ) ); 
    }

/*
 * Executes the dialog asynchronously.
*/
void CHbDevicePowerMenuSymbian::ShowL()
    {    
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuSymbian::ShowL: START") ) ); 
    iPowerMenuPrivate->ShowL();
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuSymbian::ShowL: End") ) ); 
    }

/**
 * Sets Volume to aVolume
 */
void CHbDevicePowerMenuSymbian::SetVolume(TInt aVolume)
	{
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuSymbian::SetVolume: START") ) ); 
    iPowerMenuPrivate->SetVolume(aVolume);
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuSymbian::SetVolume: End") ) ); 
	}

/**
 * Sets SilenceMode to Enable/Disable
 */
void CHbDevicePowerMenuSymbian::SetSilenceMode(TBool aEnable)
	{
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuSymbian::SetSilenceMode: START") ) ); 
	iPowerMenuPrivate->SetSilenceMode(aEnable);
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuSymbian::SetSilenceMode: END") ) ); 
	}

/**
 * Sets OfflineMode to Checked/UnChecked
 */
void CHbDevicePowerMenuSymbian::SetOfflineMode(TBool aChecked)
	{
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuSymbian::SetOfflineMode: START") ) ); 
	iPowerMenuPrivate->SetOfflineMode(aChecked);
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuSymbian::SetOfflineMode: End") ) ); 
	}

/**
 * Sets VibrationEnabled to Checked/UnChecked
 */
void CHbDevicePowerMenuSymbian::SetVibrationEnabled(TBool aEnable)
	{
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuSymbian::SetVibrationEnabled: START") ) ); 
	iPowerMenuPrivate->SetVibrationEnabled(aEnable);
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuSymbian::SetVibrationEnabled: End") ) ); 
	}

/**
 * Sets PowerOffEnabled to Enable/Disable
 */
void CHbDevicePowerMenuSymbian::SetPowerOffEnabled(TBool aEnable)
	{
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuSymbian::SetPowerOffEnabled: START") ) ); 
	iPowerMenuPrivate->SetPowerOffEnabled(aEnable);
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuSymbian::SetPowerOffEnabled: END") ) ); 
	}

/*
 * Constructor.
*/
CHbDevicePowerMenuSymbian::CHbDevicePowerMenuSymbian(CSysApAppUi& aSysApServer)
	:iSysApServer(aSysApServer)
	{
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuSymbian::Constructor") ) ); 
	//Do Nothing
	}

/*
 * 2nd Constructor.
 * Constructs and initializes all its meber variables
*/
void CHbDevicePowerMenuSymbian::ConstructL()
    {
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuSymbian::ConstructL: START") ) ); 
    iProfileEngine = ::CreateProfileEngineExtended2L();
    iCenrepOffline = CRepository::NewL( KCRUidCoreApplicationUIs );
    iCenrepProfile = CRepository::NewL( KCRUidProfileEngine );
	iPowerMenuPrivate = CHbDevicePowerMenuPrivate::NewL(this);

    //To Sync with Contro panel 
    TBool networkConnectionAllowed(EFalse);
    TInt error = iCenrepOffline->Get( KCoreAppUIsNetworkConnectionAllowed, networkConnectionAllowed );
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: KCoreAppUIsNetworkConnectionAllowed = %d"), error ) );    
 
    // The value of KCoreAppUIsNetworkConnectionAllowed is Inverted status of Offline mode
    // Hence negate the value and use as Offline mode
    SetOfflineMode(!networkConnectionAllowed);
	
	TInt masterVolume(0);
    error = iCenrepProfile->Get( KProEngMasterVolume, masterVolume );
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: KProEngMasterVolume = %d"), error ) );    
    SetVolume(masterVolume);
	
	TBool masterVibrate(0);
    error = iCenrepProfile->Get( KProEngMasterVibra, masterVibrate );
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: KProEngMasterVibra = %d"), error ) );	
	SetVibrationEnabled(masterVibrate);
	
	TBool silenceMode(EFalse);
    error = iCenrepProfile->Get( KProEngSilenceMode, silenceMode );
    TRACES( RDebug::Print( _L("CSysApAppUi::ConstructL: KProEngSilenceMode = %d"), error ) );   
	SetSilenceMode(silenceMode);
	
	//Cypheroff is True when No encryption/Decryption is happening
	TBool cypherOff(!(iSysApServer.IsEncryptionOperationOngoingL()));
	SetPowerOffEnabled(cypherOff);
    TRACES( RDebug::Print( _L("CHbDevicePowerMenuSymbian::ConstructL: END") ) ); 
    }

