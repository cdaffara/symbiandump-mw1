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
 * Description:  Creates the view for "Reset" and "Delete Data and Resore" and activates the RFS operation based on the user selection.
 *   
 */

#include "cprfssettingsform.h"
#include "cprfssettingsdataformcustomitem.h"


#include <hbdataformmodelitem.h>
#include <hbdataformmodel.h>
#include <hbpushbutton.h>
#include <rfsHandler.h>

CpRfsSettingsForm::CpRfsSettingsForm(QGraphicsItem *parent) :
    HbDataForm(parent),mTrans("control_panel")
    {
    this->setHeading(hbTrId("txt_cp_subhead_reset"));

    //initialize the form model
    initRfsSettingModel();

    connect(this, SIGNAL(itemShown(QModelIndex)),this, SLOT(onItemActivated(QModelIndex)));
    }

CpRfsSettingsForm::~CpRfsSettingsForm()
    {
    
    }

void CpRfsSettingsForm::initRfsSettingModel()
    {
    HbDataFormModel *model = new HbDataFormModel(0);

		// Create the custom items because HbPushButton cannot be added to the HbDataFormModelItem
    HbDataFormModelItem::DataItemType customItem1 = static_cast<HbDataFormModelItem::DataItemType>(HbDataFormModelItem::CustomItemBase + 1);
    
    mNormalRfs = model->appendDataFormItem(customItem1, QString(), model->invisibleRootItem());
    mNormalRfs->setLabel(hbTrId("txt_cp_setlabel_settings_reset"));
    mNormalRfs->setDescription(hbTrId("txt_cp_info_original_settings_will_be_restored_no"));
    HbDataFormModelItem::DataItemType customItem2 = static_cast<HbDataFormModelItem::DataItemType>(HbDataFormModelItem::CustomItemBase + 2);
               
    mDeepRfs = model->appendDataFormItem(customItem2, QString(), model->invisibleRootItem());
    mDeepRfs->setLabel(hbTrId("txt_cp_setlabel_device_reset"));
    mDeepRfs->setDescription(hbTrId("txt_cp_info_all_data_will_be_deleted_and_factory_s")); 
    this->setModel(model);
    }

// On Item activated
void CpRfsSettingsForm::onItemActivated(const QModelIndex &index)
    {
    HbDataFormModelItem *itemData = static_cast<HbDataFormModelItem *> (index.internalPointer());
    // to deal with orbit change temparialy
    if (itemData->type() > HbDataFormModelItem::GroupPageItem)
        {
        //get the widget of setting item
        HbAbstractViewItem* viewitem  = static_cast<HbAbstractItemView*>(this)->itemByIndex(index);
        HbWidget* widget =static_cast<HbDataFormViewItem*>(viewitem)->dataItemContentWidget(); 
		if (itemData == mNormalRfs)
            {
            activateNormalRfs(widget);
            }
		if (itemData == mDeepRfs)
            {
            activateDeepRfs(widget);
            }
        }
    }

void CpRfsSettingsForm::activateNormalRfs(HbWidget* widget)
    {
    HbPushButton *advanced = qobject_cast<HbPushButton *> (widget);
    
    if(advanced)
        {
        //TODO Issue in DataForm Calling the itemActivated twice
        disconnect( advanced, SIGNAL(clicked()), this, SLOT(onPressedNormalRfs()) );

        connect( advanced, SIGNAL(clicked()), this, SLOT(onPressedNormalRfs()) );
        }
    }

void CpRfsSettingsForm::activateDeepRfs(HbWidget* widget)
    {
    HbPushButton *advanced = qobject_cast<HbPushButton *> (widget);
    
    if(advanced)
        {
        //TODO Issue in DataForm Calling the itemActivated twice
        disconnect( advanced, SIGNAL(clicked()), this, SLOT(onPressedDeepRfs()) );

        connect( advanced, SIGNAL(clicked()), this, SLOT(onPressedDeepRfs()) );
        }
    }

void CpRfsSettingsForm::onPressedNormalRfs()
    {
    CRfsHandler* rfsHandler = new ( ELeave ) CRfsHandler;
    CleanupStack :: PushL(rfsHandler);
    
    //activate the Normal RFS
    rfsHandler->ActivateRfsL( ERfsNormal );
    
    CleanupStack :: PopAndDestroy(rfsHandler);

    }

void CpRfsSettingsForm::onPressedDeepRfs()
    {
    CRfsHandler* rfsHandler = new ( ELeave ) CRfsHandler;
    CleanupStack :: PushL(rfsHandler);
    
    //activate the Deep RFS
    rfsHandler->ActivateRfsL( ERfsDeep );
    
    CleanupStack :: PopAndDestroy(rfsHandler);
    }
