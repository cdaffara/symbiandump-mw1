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


#include "hbdevicepowermenuwidegt_p.h"
#include "hbdevicepowermenupluginerrors_p.h"
#include <HbInstance.h>

//resuorces
QString POWER_MENU_DOCML(":/xml/powermenu.docml");
QString VOLUME_DECREASE_ICON(":/images/decrement.svg");
QString VOLUME_INCREASE_ICON(":/images/increment.svg");
QString VOLUME_UNMUTE_ICON(":/images/unmuted.svg");
QString VOLUME_MUTE_ICON(":/images/muted.svg");
//For Localization
QString LOCALE_PATH("/resource/qt/translations/"); 
QString TS_FILE("power_menu");

//Widget names
QString MAIN_DLG("maindialog");
QString PWR_SILENCE("Silence");
QString PWR_VOLUME("Volume");
QString PWR_VIBARTE("Vibrate");
QString PWR_OFFLINE("Offline");
QString PWR_POWEROFF("PowerOff");
QString PWR_CONTAINER("container_1");

//DO not modify these strings
QString KVolumeLevel("VolumeLevel");
QString KSilenceLatchOn("SilenceLatchOn");            //Used to latch silencebutton	
QString KVibrationEnable("VibrationEnable");
QString KOfflineEnable("OfflineEnable");
QString KCypherOff("CypherOff");            //used to poweroffButtonEnabled/Disabled 		
QString KShutDown("ShutDown");

//Used for Orientation of PowerMenu
qreal KVerticalX = 175;
//width of PowerMenu
qreal KPMWidth = 360;

/**
 * Constructor
 */
HbDevicePowerMenuWidget::HbDevicePowerMenuWidget(const QVariantMap &parameters)
	:mDeviceDialogError(NoError), 
	 mVolumeLevel(0),
	 mShowVolume(false), 
	 mVibrationChecked(true),
	 mOfflineChecked(true),
	 mSilencelatched(true), 
	 mCypherOff(true),
	 mDialogWidget(NULL),
	 mSilenceButton(NULL),
	 mVolumeSlider(NULL), 
	 mVibarte(NULL),
	 mOffline(NULL),
	 mPowerOffButton(NULL),
	 mLoader(NULL), 
	 mHbTranslator(NULL)
    {
    // mMainWindow(NULL),
	TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::Constructor:Begin") ) );
    if (preConstructDialog())
        {
        //Translator loaded succesfully then olny construct widgets
        constructDialog(parameters);
        }
    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::Constructor:End") ) );
    }
    
/**
 * Destructor
 */
HbDevicePowerMenuWidget::~HbDevicePowerMenuWidget()
    {
    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::Destructor()") ) );
    cleanupMemory();
    }

/**
 * Implementation of HbDeviceDialogInterface method
 * sets parameters of custom widget
 */
bool HbDevicePowerMenuWidget::setDeviceDialogParameters(const QVariantMap &parameters)
	{
	TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::setDeviceDialogParameters:Begin") ) );
 	if(parameters.contains(KVolumeLevel))
		{
		mVolumeLevel = parameters.value(KVolumeLevel).toInt();
		}
	if(parameters.contains(KSilenceLatchOn))
		{
		mSilencelatched = parameters.value(KSilenceLatchOn).toBool();
		//If SilenceMode is Enabled then MasterVolume control is Disabled 
		mShowVolume = mSilencelatched? false:true;
		}
	if(parameters.contains(KVibrationEnable))
		{
		mVibrationChecked = parameters.value(KVibrationEnable).toBool();
		}
	if(parameters.contains(KOfflineEnable))
		{
		mOfflineChecked = parameters.value(KOfflineEnable).toBool();
		}
	if(parameters.contains(KCypherOff))
		{
		mCypherOff = parameters.value(KCypherOff).toBool();
		}
    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::setDeviceDialogParameters:End") ) );
	return true;
	}

/**
 * Returns error code
 */
int HbDevicePowerMenuWidget::deviceDialogError() const
	{
	TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::deviceDialogError") ) );
	return mDeviceDialogError;
	}

/**
 * Close DeviceDialog
 */
void HbDevicePowerMenuWidget::closeDeviceDialog(bool byClient)
	{
	TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::closeDeviceDialog:Begin") ) );
    Q_UNUSED(byClient);
    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::closeDeviceDialog:End") ) );
    mDialogWidget->close();
	}

/**
 * Implementation of HbDeviceDialogInterface method
 */
HbPopup* HbDevicePowerMenuWidget::deviceDialogWidget() const
	{
	TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::deviceDialogWidget") ) );
	return mDialogWidget;
	}

/**
 * Implementation of HbDeviceDialogInterface method
 * to get signals from action events
 */
QObject* HbDevicePowerMenuWidget::signalSender() const
    {
    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::signalSender") ) );
    return const_cast<HbDevicePowerMenuWidget*>(this);
    }

/**
 * Before loading docml and widgets Translator must be loaded
 */
bool HbDevicePowerMenuWidget::preConstructDialog()
    {
    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::preConstructDialog:Begin") ) );
    //Translator
    mHbTranslator = new HbTranslator(LOCALE_PATH,TS_FILE);
    if(!mHbTranslator)
        {
        //Translator is failed to create
        mDeviceDialogError = UnknownDeviceDialogError;
        TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::preConstructDialog:MemoryAllocation Failed") ) );
        return false;
        }
    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::preConstructDialog:End") ) );
    return true;
    }

/**
 * Constrcuts customized widget dialog by loading the UI from docml and
 * initializes all parameters and attributes.
 * Throws bad-memory on Un-succesful memory allocations
 */
void HbDevicePowerMenuWidget::constructDialog(const QVariantMap &parameters)
	{
	TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::constructDialog:Begin") ) );
	bool dialogLoaded(false);
	//Docml loader
	mLoader = new HbDocumentLoader();
	if(!mLoader)
	    {
        //Document Loader failed to create
        mDeviceDialogError = UnknownDeviceDialogError;
        cleanupMemory();
        TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::constructDialog:mLoader::MemoryAllocation Failed") ) );        
        return;
	    }
	mLoader->load(POWER_MENU_DOCML, &dialogLoaded);
	Q_ASSERT_X(dialogLoaded, "HbDevicePowerMenuWidget", "Invalid xml file");
	//Widgets
	if (dialogLoaded)
		{
        //Dialog loaded succesfully from docml
		TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::constructDialog:dialogLoaded:Begin") ) );
        
		//Initialize Parameters received
        setDeviceDialogParameters(parameters);
		mDialogWidget = qobject_cast <HbDialog *>(mLoader->findWidget(MAIN_DLG));
		//Find the handles for all widget items
		mSilenceButton = qobject_cast <HbPushButton *>(mLoader->findWidget(PWR_SILENCE));
		mVolumeSlider = qobject_cast <HbSlider *>(mLoader->findWidget(PWR_VOLUME));
		mVibarte = qobject_cast <HbCheckBox *>(mLoader->findWidget(PWR_VIBARTE));
		mOffline = qobject_cast <HbCheckBox *>(mLoader->findWidget(PWR_OFFLINE));
		mPowerOffButton = qobject_cast <HbPushButton *>(mLoader->findWidget(PWR_POWEROFF));	
		QGraphicsWidget *widget(NULL);
		widget = mLoader->findWidget(PWR_CONTAINER);
		if (!(IsValidHandles()&&(widget)))
		    {
            //Atleast, One of the Handles are not allocated memory
            mDeviceDialogError = UnknownDeviceDialogError;
            cleanupMemory();
            TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::constructDialog:Widgets::MemoryAllocation Failed") ) );            
            return;
		    }
		//Sets Speaker icon
        setSpeakerIcon();
		//Customize slider to VolumeSlider
		customizeVolumeSlider();
		mDialogWidget->setContentWidget(widget);
		//SetWidget Attributes
		//1. Volume 
	    mVolumeSlider->setSliderPosition(mVolumeLevel);
	    mVolumeSlider->setEnabled(mShowVolume);
	    //This will avoid IPC calls while dragging the Slider
	    mVolumeSlider->setTracking(false);
	    bool ok = connect(mVolumeSlider,SIGNAL(valueChanged(int)) , this , SLOT(handleVolumeChanged(int)));
        Q_ASSERT_X(ok, "VolumeSlider", "Invalid IncrementDecrement Change");

		//2.Silence Mode
		mSilenceButton->setDown(mSilencelatched);
		ok = connect(mSilenceButton,SIGNAL(clicked(bool)) , this , SLOT(handleSilenceMode(bool)));
	    Q_ASSERT_X(ok, "SilenceButton", "Invalid Clicked");

	    //3.Master Vibration
	    mVibarte->setChecked(mVibrationChecked);
	    ok = connect(mVibarte,SIGNAL(stateChanged(int)) , this , SLOT(handleVibrationToggled(int)));
		Q_ASSERT_X(ok, "Vibrator", "Invalid Vibrator Toggeld");
		
		//4.Offline
	    mOffline->setChecked(mOfflineChecked);
	    ok = connect(mOffline,SIGNAL(stateChanged(int)) , this , SLOT(handleOfflineToggled(int)));
		Q_ASSERT_X(ok, "Vibrator", "Invalid Offline Toggeld");  
		
		//5.Power off
		mPowerOffButton->setEnabled(mCypherOff);
		ok = connect(mPowerOffButton,SIGNAL(clicked(bool)) , this , SLOT(handlePowerOff(bool)));
	    Q_ASSERT_X(ok, "PowerOffButton", "Invalid Clicked");	  
	    
        //Dialog dismiss only with Tap outside
        mDialogWidget->setTimeout(HbPopup::NoTimeout);
        mDialogWidget->setDismissPolicy(HbPopup::TapOutside);
        mDialogWidget->setPreferredWidth(KPMWidth);
        
        connect(mDialogWidget,SIGNAL(aboutToClose()),this, SLOT(closedialog()));
	    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::constructDialog:dialogLoaded:End") ) );
		}
    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::constructDialog:End") ) );
	}

/**
 * Checks whether all Hadles for widgets DialogWidget, 
 * SilenceButton, Vibrate, Offline and Poweroff Button allocated memory.
 * Returns True, if all Handles allocated memory.
 * Flase, atleast one of them NOT allocated memory.
 */
bool HbDevicePowerMenuWidget::IsValidHandles()
    {
    if((!mDialogWidget) || (!mSilenceButton) || (!mVolumeSlider)||(!mVibarte) || (!mOffline) || (!mPowerOffButton))
        {
        return false;
        }
    else{
        //All handles are allocated memory
        return true;
        }
    }

/**
 * Sets Mute/Unmute icon for Volume slider
 * Mute/Unmute icon to display the current status of Silence Mode.
 * Mute icon dispalyed when Device in Silence mode.
 * Unmute icon dispalyed when Device Non-Silence mode. 
 */
void HbDevicePowerMenuWidget::setSpeakerIcon()
	{
	TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::setSpeakerIcon:Begin") ) );
	QString speakerIcon= ( mSilencelatched )? VOLUME_MUTE_ICON : VOLUME_UNMUTE_ICON ;
	mVolumeSlider->setElementIcon(HbSlider:: IconElement,HbIcon(speakerIcon));
    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::setSpeakerIcon:End") ) );
	}


/**
 * Customizes the slider to Vloume Slider 
 */
void HbDevicePowerMenuWidget::customizeVolumeSlider()
	{
	TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::customizeVolumeSlider:Begin") ) );
	//Construct Volume Slider from the HbSlider
	mVolumeSlider->setSliderElements(QList<QVariant>()
                                        << HbSlider::DecreaseElement 
                                        << HbSlider::TrackElement
                                        << HbSlider::IncreaseElement
                                        << HbSlider::IconElement     );
	
	// To set DecreaseIcon and IncreseIcon
	mVolumeSlider->setElementIcon(HbSlider::DecreaseElement, HbIcon(VOLUME_DECREASE_ICON));
	mVolumeSlider->setElementIcon(HbSlider:: IncreaseElement, HbIcon(VOLUME_INCREASE_ICON));
    //set Range of volume
	mVolumeSlider->setRange(1,10);
    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::customizeVolumeSlider:End") ) );
	}

/**
 * Slot
 * Widget is about to Close. 
 */
void HbDevicePowerMenuWidget::closedialog()
    {
    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::closedialog:Begin") ) );
    emit deviceDialogClosed();
    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::closedialog:Begin") ) );
    }

/**
 * Slot
 */
void HbDevicePowerMenuWidget::handleVolumeChanged(int aVolume)
    {
    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::handleVolumeChanged:Begin") ) );
    mVolumeLevel = aVolume;
    //inform the client may be through data changed etc.
    QVariantMap mDataToSend;
    QVariant volume(mVolumeLevel);
    mDataToSend.insert(KVolumeLevel, volume);
    emit deviceDialogData(mDataToSend);
    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::handleVolumeChanged:Begin") ) );
    }

/**
 * Slot
 * Switch between the states to latch down and latch up.
 * In case of Silence mode, The Silence button id latched down and Disables Volume control.
 * In case of Non-Silence mode, The Silence button id latched Up and enables Volume control.
 */
void HbDevicePowerMenuWidget::handleSilenceMode(bool aState)
	{
	TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::handleSilenceMode:Begin") ) );
	Q_UNUSED(aState);
	mShowVolume = !(mShowVolume);
	mSilencelatched = !(mSilencelatched);
	mSilenceButton->setDown(mSilencelatched);
	mVolumeSlider->setEnabled(mShowVolume);	
    setSpeakerIcon();
	//inform the client may be through data changed etc.
    QVariantMap mDataToSend;
    QVariant silence(mSilencelatched);
    mDataToSend.insert(KSilenceLatchOn, silence);
    emit deviceDialogData(mDataToSend);
    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::handleSilenceMode:End") ) );
	}

/**
 * Slot
 */
void HbDevicePowerMenuWidget::handlePowerOff(bool aState)
	{
	TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::handlePowerOff:Begin") ) );
	Q_UNUSED(aState);
    QVariantMap mDataToSend;
    QVariant shutdown(true);
    mDataToSend.insert(KShutDown, shutdown);
    emit deviceDialogData(mDataToSend);
    emit deviceDialogClosed();
    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::handlePowerOff:End") ) );
	}

/**
 * Slot
 */
void HbDevicePowerMenuWidget::handleOfflineToggled(int aOfflineToggel)
	{
	TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::handleOfflineToggled:Begin") ) );
	mOfflineChecked = aOfflineToggel;
    QVariantMap mDataToSend;
    QVariant offline(mOfflineChecked);
    mDataToSend.insert(KOfflineEnable, offline);
    emit deviceDialogData(mDataToSend);
    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::handleOfflineToggled:End") ) );
	}

/**
 * Slot
 */
void HbDevicePowerMenuWidget::handleVibrationToggled(int aVibrationToggel)
	{
	TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::handleVibrationToggled:Begin") ) );
	mVibrationChecked = aVibrationToggel;
    QVariantMap mDataToSend;
    QVariant vibration(mVibrationChecked);
    mDataToSend.insert(KVibrationEnable, vibration);
    emit deviceDialogData(mDataToSend);
    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::handleVibrationToggled:End") ) );
	}

/**
 * Cleans up memory allocations
 */
void HbDevicePowerMenuWidget::cleanupMemory()
    {
    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::cleanupMemory:Begin") ) );
    if(mLoader)
        {
        delete mLoader;
        }
    if(mHbTranslator)
        {
        delete mHbTranslator;    
        }
    TRACES( RDebug::Print( _L("HbDevicePowerMenuWidget::cleanupMemory:End") ) );
    }

