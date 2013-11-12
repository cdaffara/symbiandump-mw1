/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Private implementatin for class CpSettingFormEntryItemData.
*
*/
#include "cpsettingformentryitemdata_p.h"
#include <QVariant>
#include <hbdataformviewitem.h>
#include <hbdataform.h>
#include <hbdataformmodel.h>
#include <hbdataformmodelitem.h>
#include <cpitemdatahelper.h>
#include <cpsettingformentryitemdata.h>

CpSettingFormEntryItemDataPrivate::CpSettingFormEntryItemDataPrivate()
: mDataForm(0),
  mItemDataHelper(0),
  mItemPressed(false)
{
    
}

CpSettingFormEntryItemDataPrivate::CpSettingFormEntryItemDataPrivate(CpItemDataHelper *itemDataHelper) 
: mDataForm(0),
  mItemDataHelper(itemDataHelper),
  mItemPressed(false)
{
    
}

CpSettingFormEntryItemDataPrivate::CpSettingFormEntryItemDataPrivate(HbDataForm *dataForm)
: mDataForm(dataForm),
  mItemDataHelper(0),
  mItemPressed(false)
{
    
}

void CpSettingFormEntryItemDataPrivate::init(CpSettingFormEntryItemData *parent)
{
    mParent = parent;
    
    HbDataFormModelItem::DataItemType itemType = mParent->type();
    switch (itemType)
    {
    default: break;
    case CpSettingFormEntryItemData::ListEntryItem:
        {
            if (mItemDataHelper) {
                mItemDataHelper->connectToForm(SIGNAL(pressed (QModelIndex)),mParent,SLOT(_q_itemPressed(QModelIndex)));
                mItemDataHelper->connectToForm(SIGNAL(activated(QModelIndex)),mParent,SLOT(_q_itemActivated(QModelIndex)));
            }
            else if (mDataForm) {
                QObject::connect( mDataForm,SIGNAL(pressed(QModelIndex)),mParent,SLOT(_q_itemPressed(QModelIndex)) );
                QObject::connect( mDataForm,SIGNAL(activated(QModelIndex)),mParent,SLOT(_q_itemActivated(QModelIndex)) );
            }
        }
        break;
    case CpSettingFormEntryItemData::ButtonEntryItem:
        {
            if (mItemDataHelper) {
                mItemDataHelper->addConnection(mParent,SIGNAL(clicked()),mParent,SLOT(onLaunchView()));
            }
            else if (mDataForm) {
                mDataForm->addConnection(mParent,SIGNAL(clicked()),mParent,SLOT(onLaunchView()));
            }  
        }
        break;
    }
}

QString CpSettingFormEntryItemDataPrivate::text() const
{
    HbDataFormModelItem::DataItemType itemType = mParent->type();
    switch (itemType)
    {
    default: break;
    case CpSettingFormEntryItemData::ListEntryItem:
        return mParent->label();
    case CpSettingFormEntryItemData::ButtonEntryItem:
        return mParent->contentWidgetData(QString("text")).toString();
    }
    return QString();
}

void CpSettingFormEntryItemDataPrivate::setText(const QString &text)
{
    HbDataFormModelItem::DataItemType itemType = mParent->type();
    switch (itemType)
    {
    default: break;
    case CpSettingFormEntryItemData::ListEntryItem:
        mParent->setLabel(text);
        break;
    case CpSettingFormEntryItemData::ButtonEntryItem:
        mParent->setContentWidgetData(QString("text"),QVariant(text));
        break;
    }
}

QString CpSettingFormEntryItemDataPrivate::description() const
{
    HbDataFormModelItem::DataItemType itemType = mParent->type();
    switch (itemType)
    {
    default: break;
    case CpSettingFormEntryItemData::ListEntryItem:
        return mParent->data(HbDataFormModelItem::DescriptionRole).toString();
    case CpSettingFormEntryItemData::ButtonEntryItem:
        return mParent->contentWidgetData(QString("additionalText")).toString();
    }
    return QString();
}

void CpSettingFormEntryItemDataPrivate::setDescription(const QString &description)
{
    HbDataFormModelItem::DataItemType itemType = mParent->type();
    switch (itemType)
    {
    default: break;
    case CpSettingFormEntryItemData::ListEntryItem:
        mParent->setData(HbDataFormModelItem::DescriptionRole , description);
		// set a useless property for model item to emit a datachanged signal
		// this is a workaround for informing the change event of description in list item 
        mParent->setContentWidgetData("Useless", true);
        break;
    case CpSettingFormEntryItemData::ButtonEntryItem:
        mParent->setContentWidgetData(QString("additionalText"),QVariant(description));
        break;
    }
}

QString CpSettingFormEntryItemDataPrivate::iconName() const
{
    HbDataFormModelItem::DataItemType itemType = mParent->type();
    switch (itemType)
    {
    default: break;
    case CpSettingFormEntryItemData::ListEntryItem:
        return mParent->icon();
    case CpSettingFormEntryItemData::ButtonEntryItem:
        return qvariant_cast<HbIcon>(mParent->contentWidgetData(QString("icon"))).iconName();
    }
    return QString();
}

void CpSettingFormEntryItemDataPrivate::setIconName(const QString &icon)
{
    HbDataFormModelItem::DataItemType itemType = mParent->type();
    switch (itemType)
    {
    default: break;
    case CpSettingFormEntryItemData::ListEntryItem:
        mParent->setIcon(icon);
        break;
    case CpSettingFormEntryItemData::ButtonEntryItem:
        mParent->setContentWidgetData(QString("icon"),HbIcon(icon));
        break;
    }
}

HbIcon CpSettingFormEntryItemDataPrivate::entryItemIcon() const
{
    HbDataFormModelItem::DataItemType itemType = mParent->type();
    switch (itemType)
    {
    default: break;
    case CpSettingFormEntryItemData::ListEntryItem:
        return HbIcon(mParent->icon());
    case CpSettingFormEntryItemData::ButtonEntryItem:
        return qvariant_cast<HbIcon>(mParent->contentWidgetData(QString("icon")));
    }
    return HbIcon();
}

void CpSettingFormEntryItemDataPrivate::setEntryItemIcon(const HbIcon &icon)
{
    HbDataFormModelItem::DataItemType itemType = mParent->type();
    switch (itemType)
    {
    default: break;
    case CpSettingFormEntryItemData::ListEntryItem:
        mParent->setIcon(icon.iconName());
        break;
    case CpSettingFormEntryItemData::ButtonEntryItem:
        mParent->setContentWidgetData(QString("icon"),icon);
        break;
    }
}


void CpSettingFormEntryItemDataPrivate::_q_itemPressed(const QModelIndex &index)
{
    mItemPressed = ( modelItemFromModelIndex(index) == mParent );
}


void CpSettingFormEntryItemDataPrivate::_q_itemActivated(const QModelIndex &index)
{
    if (mItemPressed && modelItemFromModelIndex(index) == mParent) {
        mParent->onLaunchView();
    }
    mItemPressed = false;
}


HbDataFormModelItem *CpSettingFormEntryItemDataPrivate::modelItemFromModelIndex(const QModelIndex &index)
{
    if (mItemDataHelper) {
        return mItemDataHelper->modelItemFromModelIndex(index);
    }
    else if (mDataForm){
        HbDataFormModel *dataFormModel = 
                qobject_cast<HbDataFormModel*>(mDataForm->model());
        if (dataFormModel) {
            return dataFormModel->itemFromIndex(index);
        }
    }
    
    return 0;
}

#include "moc_cpsettingformentryitemdata.cpp"

//End of File
