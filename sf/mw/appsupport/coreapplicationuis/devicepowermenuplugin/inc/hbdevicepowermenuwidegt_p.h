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

#ifndef HBDEVICEPOWERMENUWIDEGT_P_H
#define HBDEVICEPOWERMENUWIDEGT_P_H


#include <QObject>
#include <QVariantMap>
#include <hbdevicedialoginterface.h>
#include <hbdevicedialog.h>
#include <hbdialog.h>
#include <hbdocumentloader.h>
#include <HbTranslator.h>
//For widgets used
#include <hbpushbutton.h>
#include <hbslider.h>
#include <hbcheckBox.h>
#include <hbinstance.h>

class HbDevicePowerMenuWidget : public QObject, public HbDeviceDialogInterface
{
    Q_OBJECT
    
public:
    HbDevicePowerMenuWidget(const QVariantMap &parameters);
    ~HbDevicePowerMenuWidget();

public:    
	//HbDeviceDialogInterface methods to be implemented
    bool setDeviceDialogParameters(const QVariantMap &parameters);
    int deviceDialogError() const;
    void closeDeviceDialog(bool byClient);
    HbPopup *deviceDialogWidget() const;
    virtual QObject *signalSender() const;
    

private:    
    bool preConstructDialog();
    void constructDialog(const QVariantMap &parameters);
    void cleanupMemory();
    
private:
    void customizeVolumeSlider();
    void setSpeakerIcon();
    bool IsValidHandles();

private slots:    
    void handleVolumeChanged(int aVolume);
    void handleSilenceMode(bool aState);
    void handleVibrationToggled(int aVibrationToggel);
    void handleOfflineToggled(int aOfflineToggel);
    void handlePowerOff(bool aState);
    void closedialog();
    
signals:
    void deviceDialogClosed();
    void deviceDialogData(QVariantMap data);

private:
    Q_DISABLE_COPY(HbDevicePowerMenuWidget)

    int 				mDeviceDialogError;             //DeviceDialogError
    int                 mVolumeLevel;                   //shows current Volume Level
    bool 				mShowVolume;                   //used to Enable or Disbale Volume                   
    bool 				mVibrationChecked;             //Vibration enabled or Disabled
    bool 				mOfflineChecked;               //offline enabled or Disabled
    bool				mSilencelatched;                //SilenceMode Enabled or Disabled 
    bool				mCypherOff;                     //Shows Encryption/Decryption ON/OFF
    
    HbDialog*			mDialogWidget;                  //Handle for Powermenu Dialog
    HbPushButton*		mSilenceButton;                  //Handle for Silence button
    HbSlider*			mVolumeSlider;                  //Handle for Slider
    HbCheckBox*			mVibarte;                         //Handle for Vibration check Box
    HbCheckBox*			mOffline;                         //Handle for Offline CheckBox
    HbPushButton *		mPowerOffButton;                //Handle for PowerOff Button
    HbDocumentLoader* 	mLoader;                         //Hnadle for Documnet Loader to load docml 
    HbTranslator*       mHbTranslator;                  //Translator used for localization; loads translation files
};

#endif // HBDEVICEPOWERMENUWIDEGT_P_H
