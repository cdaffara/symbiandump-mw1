/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of "Eclipse Public License v1.0""
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 *
 * Initial Contributors:
 * Nokia Corporation - initial contribution.
 *
 * Contributors:
 *
 * Description:  Creates the custom items required for HbDataFormModelItem.
 *   
 */

#include <cprfssettingsdataformcustomitem.h>
#include <hbdataformmodelitem.h>
#include <hbpushbutton.h>
#include <hbtranslator.h>

CpRfsSettingsDataFormCustomItem::CpRfsSettingsDataFormCustomItem(QGraphicsItem *parent) :
HbDataFormViewItem(parent)
    {
    }

CpRfsSettingsDataFormCustomItem::~CpRfsSettingsDataFormCustomItem()
    {
    }

//overridden method which is required in case of customwidget
bool CpRfsSettingsDataFormCustomItem::canSetModelIndex(const QModelIndex &index) const
        {
        HbDataFormModelItem::DataItemType itemType = 
        static_cast<HbDataFormModelItem::DataItemType>(
        index.data(HbDataFormModelItem::ItemTypeRole).toInt());
        //each custom item can be represented by customitembase+number and 10 is just in future if u have to add extra custom items
        if(itemType >= HbDataFormModelItem::CustomItemBase && 
        itemType <= HbDataFormModelItem::CustomItemBase  + 10 ) {
        return true;
        } 
        return false;
 
        }
//overridden method which is required in case of customwidget
HbAbstractViewItem* CpRfsSettingsDataFormCustomItem::createItem()
           {
           return new CpRfsSettingsDataFormCustomItem(*this);
           }
//overridden method which is required in case of customwidget
HbWidget* CpRfsSettingsDataFormCustomItem::createCustomWidget()
         {
         HbDataFormModelItem::DataItemType itemType = static_cast<HbDataFormModelItem::DataItemType>(
         modelIndex().data(HbDataFormModelItem::ItemTypeRole).toInt());
         HbTranslator trans("control_panel");
         switch(itemType)
                 {
                 case HbDataFormModelItem::CustomItemBase + 1:
                 	   {
                     //Create 1st button 
                     mButton = new HbPushButton();
                     mButton->setText(hbTrId("txt_cp_setlabel_settings_reset_val_reset_settings"));
                     return mButton;
                     }
                 case HbDataFormModelItem::CustomItemBase + 2:
                 	   {
                     //Create 2nd button
                     mButton = new HbPushButton();
                     mButton->setText(hbTrId("txt_cp_setlabel_device_reset_val_reset_device"));
                     return mButton;
                     }
                 //Some other cases can also be added if there are more than 2 custom data item
                 default:
                 return 0;
             }
         }
