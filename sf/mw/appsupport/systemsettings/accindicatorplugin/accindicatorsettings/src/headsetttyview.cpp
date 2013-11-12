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
* Name        : headsetttyview.cpp
*
* Description: 
*        
*
*/

#include <hbview.h>
#include <hbdocumentloader.h>
#include <hblabel.h>
#include <hbcombobox.h>
#include <accpolaccessorymode.h>
#include <accpolgenericiddefinitions.h>
#include <accessoriescrkeys.h>
#include <AccessoryTypes.h>
#include <QDebug>

#include "headsetttyview.h"

HeadsetTtyView::HeadsetTtyView(HbMainWindow *window,int accessoryType)
    {
    // Handle to the central repository.
    iAccessoryRepository = CRepository::NewL( KCRUidAccessorySettings );
    
    TInt error = iAccServer.Connect();
    if(error != KErrNone)
        {
        qDebug() << "Failed to connect to accessory server"; 
        return;
        }
    error = iAccessorySettings.CreateSubSession( iAccServer );
    if(error != KErrNone)
        {
        qDebug() << "Failed to connect to accessory sub session"; 
        return;
        }

    HbDocumentLoader loader;
    bool viewLoaded(false);
    loader.load(":/xml/headset.docml", &viewLoaded);
    Q_ASSERT_X(viewLoaded, "MainView", "Invalid xml file");

    HbLabel *image;
    HbLabel *acctype;
    // set the image to be displayed.
    if( accessoryType == KPCWired || accessoryType == KPCUSB ) //wired
        {
        image = qobject_cast<HbLabel*>(loader.findWidget("image"));
        image->setIcon(HbIcon(":/images/wired_accessory.svg"));
        }
    else // wireless
        {
        image = qobject_cast<HbLabel*>(loader.findWidget("image"));
        image->setIcon(HbIcon(":/images/wireless_accessory.svg"));
        }

    HbComboBox *comboHandler = qobject_cast<HbComboBox*>(loader.findWidget("combobox"));
    QStringList comboItems;
    comboItems <<"HeadSet" <<"TTY";
    comboHandler->addItems(comboItems);
    
    connect(comboHandler , SIGNAL(currentIndexChanged(int)) , this , SLOT(currentIndexModified(int))); 

    User::LeaveIfError( iAccessoryRepository->Get(KSettingsAccDefaultInfo, defaultaccessory ));    
    
    // set the name to be displayed along with the image.
    if( defaultaccessory == 0) // HeadSet
        {
        acctype = qobject_cast<HbLabel*>(loader.findWidget("label"));
        acctype->setPlainText("HeadSet");
        acctype->setTextWrapping(Hb::TextWordWrap);
        comboHandler->setCurrentIndex(0); // set headset as default
        }
    else
        {
        acctype = qobject_cast<HbLabel*>(loader.findWidget("label"));
        acctype->setPlainText("TTY");
        acctype->setTextWrapping(Hb::TextWordWrap);
        comboHandler->setCurrentIndex(1); // set TTY as default
        }    
    
    // heading for the combobox for user selection
    acctype = qobject_cast<HbLabel*>(loader.findWidget("label_2"));
    acctype->setPlainText("Accessory Type");
    acctype->setTextWrapping(Hb::TextWordWrap);
    
    window->addView(loader.findWidget("view"));
    }
    
HeadsetTtyView::~HeadsetTtyView()
    {
    iAccessorySettings.CloseSubSession();
    iAccServer.Disconnect();
    delete iAccessoryRepository;
    }

void HeadsetTtyView::currentIndexModified(int var)
    {
    if(var == 0) // make headset as default
        {
        User::LeaveIfError((iAccessoryRepository->Set(KSettingsAccDefaultInfo, 0))); // 0 is for Headset
        iAccessorySettings.SetHWDeviceSettingsL( KASHeadset );
        }
    else
        {
        User::LeaveIfError((iAccessoryRepository->Set(KSettingsAccDefaultInfo, 7))); // 7 is for TTY
        iAccessorySettings.SetHWDeviceSettingsL( KASTTY );
        }
    }
