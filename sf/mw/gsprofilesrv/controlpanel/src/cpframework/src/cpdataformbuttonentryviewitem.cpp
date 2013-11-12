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
* Description:  View item implementation for button entry item.
*
*/
#include "cpdataformbuttonentryviewitem.h"
#include <hbdataformmodelitem.h>
#include <hbpushbutton.h>
#include <hbdataformmodel.h>
#include <hbabstractitemview.h>
#include <QMetaProperty>
#include <cpsettingformentryitemdata.h>
/*!
    \class CpDataFormButtonEntryViewItem
    \brief The CpDataFormButtonEntryViewItem is a item proto type of HbDataForm which is supplied by control panel.
	When you want use setting items which are come from control panel, you should append this class to the proto type list of your data form's instance.
	Code example:
	\code
	HbDataForm *form = new HbDataForm();
	QList<HbAbstractViewItem *> protoType = form->itemProtoTypetypes();
	protoType.append(CpDataFormButtonEntryViewItem::createCpItemProtoType());
	form->setItemPrototypes();
	\endcode
*/
class CpDataFormButtonEntryViewItemPrivate 
{
public:
	CpDataFormButtonEntryViewItemPrivate() : mWidget(0)
	{
	}
	~CpDataFormButtonEntryViewItemPrivate()
	{
	}
	HbWidget *mWidget;
};

/*!
    Constructor
 */
CpDataFormButtonEntryViewItem::CpDataFormButtonEntryViewItem(QGraphicsItem *parent)
: HbDataFormViewItem(parent),d_ptr(new CpDataFormButtonEntryViewItemPrivate())
{
}

/*!
	Destructor of CpDataFormButtonEntryViewItem
 */
CpDataFormButtonEntryViewItem::~CpDataFormButtonEntryViewItem()
{
	delete d_ptr;
}

/*!
	Creates CpDataFormButtonEntryViewItem. This function is called form HbAbstractItemContainer when model is getting parsed for creating items.
 */
HbAbstractViewItem *CpDataFormButtonEntryViewItem::createItem()
{
	return new CpDataFormButtonEntryViewItem(*this);
}

/*!
	Copy constructor of CpDataFormButtonEntryViewItem
 */
CpDataFormButtonEntryViewItem::CpDataFormButtonEntryViewItem(const CpDataFormButtonEntryViewItem &other)
: HbDataFormViewItem(other), d_ptr(new CpDataFormButtonEntryViewItemPrivate(*other.d_ptr))
{
	
}

/*!
	Assignment operator
 */
CpDataFormButtonEntryViewItem &CpDataFormButtonEntryViewItem::operator=(const CpDataFormButtonEntryViewItem &other)
{
	if (&other == this) {
		return *this;
	}

	*d_ptr = *(other.d_ptr);
	
	return *this;
}

/*!
	Inherit from HbAbstractViewItem, return true if the model item can be supported by CpDataFormButtonEntryViewItem.
 */
bool CpDataFormButtonEntryViewItem::canSetModelIndex(const QModelIndex &index) const
{
	int itemTypeId = index.data(HbDataFormModelItem::ItemTypeRole).toInt();
	
	return (itemTypeId == CpSettingFormEntryItemData::ButtonEntryItem);
}

/*!
	Inherit from HbDataFormViewItem, return the setting item's widget of control panel
 */
HbWidget *CpDataFormButtonEntryViewItem::createCustomWidget()
{
    HbDataFormModelItem::DataItemType itemType = static_cast<HbDataFormModelItem::DataItemType>(
        modelIndex().data(HbDataFormModelItem::ItemTypeRole).toInt());

	if (itemType == CpSettingFormEntryItemData::ButtonEntryItem) {
		HbPushButton *button = new HbPushButton(QString("Push button"));
		d_ptr->mWidget = button;
		button->setTextAlignment( Qt::AlignLeft );
		return button;
	}
    
    return 0;
}

/*!
    Inherit from HbDataForm. This function is called by hbdataform's framework, 
    for supporting to load entry item's text, icon and additional text dynamically.
    It is not recommended to call this function manually.
 */
void CpDataFormButtonEntryViewItem::restore()
{
    HbDataFormViewItem::restore();

    // Set widget's properties according to model item's content widget data
    // so that the widget can be refreshed
    if (d_ptr->mWidget) {
        HbDataFormModelItem::DataItemType itemType = static_cast<HbDataFormModelItem::DataItemType>(
            modelIndex().data(HbDataFormModelItem::ItemTypeRole).toInt());

        if(itemType == CpSettingFormEntryItemData::ButtonEntryItem) {

            QModelIndex itemIndex = modelIndex();
            HbDataFormModel *model = static_cast<HbDataFormModel*>(itemView()->model());;
            HbDataFormModelItem *modelItem = static_cast<HbDataFormModelItem*>(
                model->itemFromIndex(itemIndex));

            const QMetaObject *metaObj = d_ptr->mWidget->metaObject();
            int count = metaObj->propertyCount();
            for (int i = 0; i < count; i++) {
                QMetaProperty metaProperty = metaObj->property(i);
                if (metaProperty.isValid() && metaProperty.isWritable()) {
                    metaProperty.write(d_ptr->mWidget,modelItem->contentWidgetData(metaProperty.name()));
                }

            }
        }
    }
}

//End of File
