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

#ifndef HBDEVICEPOWERMENUSYMBIAN_H
#define HBDEVICEPOWERMENUSYMBIAN_H

#if defined(__SYMBIAN32__) || defined(SYMBIAN) || defined(HB_DOXYGEN)

#include <e32base.h>
#include <centralrepository.h>
#include <settingsinternalcrkeys.h>
#include <ProfileEngineInternalCRKeys.h>
#include <ProfileEngineSDKCRKeys.h>
#include <MProfileEngineExtended2.h>
#include "SysApAppUi.h"

class CHbDevicePowerMenuPrivate;

class CHbDevicePowerMenuSymbian : public CBase
{
public:

	static CHbDevicePowerMenuSymbian* NewL(CSysApAppUi& aSysApServer );
    virtual ~CHbDevicePowerMenuSymbian();
    
    //Displays Dialog
    void ShowL();
    //Getters and Setters
    void SetVolume(TInt aVolume);
    void SetSilenceMode(TBool aEnable);
    void SetOfflineMode(TBool aChecked);
    void SetVibrationEnabled(TBool aEnable);
    void SetPowerOffEnabled(TBool aEnable);

private:

    CHbDevicePowerMenuSymbian(CSysApAppUi& aSysApServer);
    void ConstructL();

private:
    friend class CHbDevicePowerMenuPrivate;
    //Used to set master Volume/Vibrate/Silence Mode in Control Panel
    MProfileEngineExtended2* 	iProfileEngine;
    //Used to Set and Get OfflineMode to and from Control Panel
    CRepository* 				iCenrepOffline;
    //Used to Get Master Volume/Vibrate/Silence Mode in Control Panel
    CRepository* 				iCenrepProfile;
    //Used to handle ShutDown
    CSysApAppUi& 				iSysApServer;
    CHbDevicePowerMenuPrivate* 	iPowerMenuPrivate;
};

#endif // defined(__SYMBIAN32__) || defined(SYMBIAN) || defined(HB_DOXYGEN)

#endif // HBDEVICEPOWERMENUSYMBIAN_H


