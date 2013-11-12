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
 * Description: Shows the view of the TV-OUT accessory. 
 *
 */

#include "tvoutview.h"

#include <hbdocumentloader.h>
#include <hbapplication.h>
#include <hblabel.h>
#include <hbcombobox.h>
#include <accpolaccessorymode.h>
#include <accpolgenericiddefinitions.h>

TvOutView::TvOutView(HbMainWindow *window,int accessoryType)
    {
    HbDocumentLoader loader;
    bool viewLoaded(false);
    loader.load(":/xml/tvout.docml", &viewLoaded);
    Q_ASSERT_X(viewLoaded, "AccSettings", "Invalid docml file");

    HbLabel *label;
    if( accessoryType == KPCWired || accessoryType == KPCUSB ) // wired
        {
        label = qobject_cast<HbLabel*>(loader.findWidget("image"));
        label->setIcon(HbIcon(":/images/wired_accessory.svg"));
        }
    else // wireless
        {
        label = qobject_cast<HbLabel*>(loader.findWidget("image"));
        label->setIcon(HbIcon(":/images/wireless_accessory.svg"));
        }
    
    label = qobject_cast<HbLabel*>(loader.findWidget("label"));
    label->setPlainText("Tv-Out");
    label->setTextWrapping(Hb::TextWordWrap);
    
    label = qobject_cast<HbLabel*>(loader.findWidget("label_4"));
    label->setPlainText("TV Aspect Ratio");
    label->setTextWrapping(Hb::TextWordWrap);

    HbComboBox *comboHandler = qobject_cast<HbComboBox*>(loader.findWidget("combobox"));
    
    // prepare the list of items to be there in combobox.
    QStringList comboItems;
    comboItems <<"4:3" <<"16:9";
    comboHandler->addItems(comboItems);
        
    //set the current index of combobox to the current AspectRatio.
    iServerEngine = CGSServerEngine::NewL();
    currentValue = iServerEngine->AspectRatioL();
    comboHandler->setCurrentIndex(currentValue);
    
    //If the index changed in the combobox update the AspectRatio of the TV-Out Settings. 
    QObject::connect(comboHandler , SIGNAL(currentIndexChanged(int)) , this , SLOT(currentIndexModified(int)));
    
    window->addView(loader.findWidget("view"));

    }
    
TvOutView::~TvOutView()
    {
    if(iServerEngine)
        {
        delete iServerEngine;
        }
    }

void TvOutView::currentIndexModified(int modifiedSlot)
    {
    iServerEngine->SetAspectRatioL( modifiedSlot );
    }
