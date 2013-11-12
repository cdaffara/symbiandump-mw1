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
* Description:  
*
*/
#include "cpsettingformentryitemdata.h"
#include "cpsettingformentryitemdata_p.h"
#include "cpviewlauncher.h"
#include <cpbasesettingview.h>


/*!
    \class CpSettingFormEntryItemData
    \brief The CpSettingFormEntryItemData can be used to control panel plugin which appears as a setting item. And after clicking
    it, a new setting view will open.
 */

/*
    \fn CpSettingFormEntryItemData::createSettingView
    
    Create a new setting view.
    
    createSettingView will be called automatically when user click a setting item represented by a CpSettingFormEntryItemData.
    
    In most cases, you do not need to write a derived class from CpSettingFormEntryItemData, instead, you can simply use template class CpSettingFormEntryItemDataImpl.
*/


/*!
    Construct a new CpSettingFormEntryItemData with the given parent.
*/
CpSettingFormEntryItemData::CpSettingFormEntryItemData(const HbDataFormModelItem *parent /* = 0*/)
: CpSettingFormItemData(parent),  d_ptr(new CpSettingFormEntryItemDataPrivate())
{
    d_ptr->init(this);
}


/*!
    Construct a new CpSettingFormEntryItemData with the given text,description and parent.
*/

CpSettingFormEntryItemData::CpSettingFormEntryItemData(CpItemDataHelper &itemDataHelper,
		const QString &text /* = QString()*/,
        const QString &description /* = QString()*/,
		const HbIcon &icon,
        const HbDataFormModelItem *parent /*= 0*/) : 
        CpSettingFormItemData(HbDataFormModelItem::CustomItemBase,QString(),parent),
        d_ptr(new CpSettingFormEntryItemDataPrivate(&itemDataHelper))
{
    setType ( static_cast<HbDataFormModelItem::DataItemType> (CpSettingFormEntryItemData::ListEntryItem) );
	
    d_ptr->init(this);
    
	setText(text);
	setDescription(description);
	setIcon(icon.iconName());
}

/*!
    Construct a new CpSettingFormEntryItemData with the given text,description and parent.
*/

CpSettingFormEntryItemData::CpSettingFormEntryItemData(HbDataForm *dataForm,
        const QString &text /*= QString()*/,
        const QString &description /*= QString()*/,
        const HbIcon &icon /*= HbIcon()*/,
        const HbDataFormModelItem *parent/* = 0*/) :
        CpSettingFormItemData(HbDataFormModelItem::CustomItemBase,QString(),parent),
        d_ptr(new CpSettingFormEntryItemDataPrivate(dataForm))
{
	setType ( static_cast<HbDataFormModelItem::DataItemType> (CpSettingFormEntryItemData::ListEntryItem) );
	
	d_ptr->init(this);
	
    setText(text);
    setDescription(description);
    setIcon(icon.iconName());
}

/*!
    Construct a new CpSettingFormEntryItemData with the given type, text, description, icon name, and parent.
*/
CpSettingFormEntryItemData::CpSettingFormEntryItemData(
         EntryItemType type,
         CpItemDataHelper &itemDataHelper,
         const QString &text /*= QString()*/,
         const QString &description /*= QString()*/,
         const QString &iconName /*= QString()*/,
         const HbDataFormModelItem *parent /*= 0*/) :
         CpSettingFormItemData(HbDataFormModelItem::CustomItemBase,QString(),parent),
         d_ptr(new CpSettingFormEntryItemDataPrivate(&itemDataHelper))
{
    setType ( static_cast<HbDataFormModelItem::DataItemType> (type) );
    
    d_ptr->init(this);
    
    setText(text);
    setDescription(description);
    setIcon(iconName);
}

/*!
    Construct a new CpSettingFormEntryItemData with the given type, text, description, icon name, and parent.
*/
CpSettingFormEntryItemData::CpSettingFormEntryItemData(
         EntryItemType type,
         HbDataForm *dataForm,
         const QString &text /*= QString()*/,
         const QString &description /*= QString()*/,
         const QString &iconName /*= QString()*/,
         const HbDataFormModelItem *parent /*= 0*/) : 
         CpSettingFormItemData(HbDataFormModelItem::CustomItemBase,QString(),parent),
         d_ptr(new CpSettingFormEntryItemDataPrivate(dataForm))
{
    setType ( static_cast<HbDataFormModelItem::DataItemType> (type) );
    
    d_ptr->init(this);
    
    setText(text);
    setDescription(description);
    setIcon(iconName);
}


/*!
     Destructor.
*/
CpSettingFormEntryItemData::~CpSettingFormEntryItemData()
{
    delete d_ptr;
}


/*!
     Get the text string of the entry item.
*/
QString CpSettingFormEntryItemData::text() const
{
    return d_ptr->text();
}

/*!
     Set the text string of the entry item.
*/
void CpSettingFormEntryItemData::setText(const QString &text)
{
    d_ptr->setText(text);
}

/*!
     Get the description string of the entry item.
*/
QString CpSettingFormEntryItemData::description() const
{
    return d_ptr->description();
}

/*!
     Set the description string of the entry item.
*/
void CpSettingFormEntryItemData::setDescription(const QString &description)
{
    d_ptr->setDescription(description);
}

/*!
    Get the icon name of the entry item.
*/
QString CpSettingFormEntryItemData::iconName() const
{
    return d_ptr->iconName();
}

/*!
    Set the icon name of the entry item.
*/
void CpSettingFormEntryItemData::setIconName(const QString &icon)
{
    d_ptr->setIconName(icon);
}

/*!
	Return the icon of the entry item
*/
HbIcon CpSettingFormEntryItemData::entryItemIcon()const
{
    return d_ptr->entryItemIcon();
}

/*!
	Set the icon for the entry item
*/
void CpSettingFormEntryItemData::setEntryItemIcon(const HbIcon& icon)
{
    d_ptr->setEntryItemIcon(icon);
}

/*!
    Launch the setting view when user clicks the entry item.
*/
void CpSettingFormEntryItemData::onLaunchView() 
{
    //avoid being launched more than one times
    if (d_ptr->mSettingViewPtr.isNull()) {
        d_ptr->mSettingViewPtr = createSettingView();
        if (!d_ptr->mSettingViewPtr.isNull()) {
            CpViewLauncher::launchView(d_ptr->mSettingViewPtr.data());
        }
    }
}

//End of File
