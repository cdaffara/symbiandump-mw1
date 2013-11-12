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

#ifndef CPSETTINGFORMENTRYITEMDATAIMPL_H
#define CPSETTINGFORMENTRYITEMDATAIMPL_H

#include "cpsettingformentryitemdata.h"


/*!
    \class CpSettingFormEntryItemDataImpl
    \brief The template class CpSettingFormEntryItemDataImpl provide a defaut implementation of CpSettingFormEntryItemData. 
    By implementing pure virtual function CpSettingFormEntryItemData::createSettingView.
 */
class CpItemDataHelper;
template<typename PLUGIN_VIEW>
class CpSettingFormEntryItemDataImpl : public CpSettingFormEntryItemData
{
public:

    /*!
        Construct a new CpSettingFormEntryItemDataImpl with the given text,description and parent.
    */
    explicit CpSettingFormEntryItemDataImpl(CpItemDataHelper &itemDataHelper,
			const QString &text = QString(),
            const QString &description = QString(),
			const HbIcon &icon = HbIcon(),
            const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(itemDataHelper,text,description,icon,parent)
    { }

	/*!
        Construct a new CpSettingFormEntryItemDataImpl with the given text,description and parent.
    */
	explicit CpSettingFormEntryItemDataImpl(HbDataForm *dataForm,
			const QString &text = QString(),
	        const QString &description = QString(),
			const HbIcon &icon = HbIcon(),
			const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(dataForm,text,description,icon,parent)

	{ }
	
	/*!
	    Construct a new CpSettingFormEntryItemData with the given type, text, description, icon name, and parent.
	*/
    explicit CpSettingFormEntryItemDataImpl(
            EntryItemType type,
            CpItemDataHelper &itemDataHelper,
            const QString &text = QString(),
            const QString &description = QString(),
            const QString &icon = QString(),
            const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(type,itemDataHelper,text,description,icon,parent)
    { }
    
    /*!
        Construct a new CpSettingFormEntryItemData with the given type, text, description, icon name, and parent.
    */
    explicit CpSettingFormEntryItemDataImpl(
            EntryItemType type,
            HbDataForm *dataForm,
            const QString &text = QString(),
            const QString &description = QString(),
            const QString &icon = QString(),
            const HbDataFormModelItem *parent = 0) : 
            CpSettingFormEntryItemData(type,dataForm,text,description,icon,parent)
    { }

    /*!
         Destructor.
    */
    virtual ~CpSettingFormEntryItemDataImpl()
    { }
    
    /*!
         Implement CpSettingFormEntryItemData::createSettingView
    */
    virtual CpBaseSettingView *createSettingView() const
    { return new PLUGIN_VIEW(); }
};

#endif  // CP_SETTINGFORM_ENTRY_ITEM_DATA_IMPL_H
