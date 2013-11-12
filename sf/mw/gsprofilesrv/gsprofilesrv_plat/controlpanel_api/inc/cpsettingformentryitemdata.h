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

#ifndef CPSETTINGFORMENTRYITEMDATA_H
#define CPSETTINGFORMENTRYITEMDATA_H

#include <cpglobal.h>
#include <QObject>
#include <cpsettingformitemdata.h>
#include <hbicon.h>

class CpItemDataHelper;
class CpBaseSettingView;
class HbDataForm;
class QModelIndex;
class CpSettingFormEntryItemDataPrivate;

class CP_EXPORT CpSettingFormEntryItemData : public CpSettingFormItemData 
{
	Q_OBJECT
public:
	enum EntryItemType {
	    ListEntryItem   = HbDataFormModelItem::CustomItemBase + 1,
	    ButtonEntryItem = HbDataFormModelItem::CustomItemBase + 2
	};

	explicit CpSettingFormEntryItemData(const HbDataFormModelItem *parent = 0);

	explicit CpSettingFormEntryItemData(
	        CpItemDataHelper &itemDataHelper,
			const QString &text = QString(),
	        const QString &description = QString(),
			const HbIcon &icon = HbIcon(),
			const HbDataFormModelItem *parent = 0);	 

	explicit CpSettingFormEntryItemData(
	        HbDataForm *dataForm,
			const QString &text = QString(),
	        const QString &description = QString(),
			const HbIcon &icon = HbIcon(),
			const HbDataFormModelItem *parent = 0);	
	
    explicit CpSettingFormEntryItemData(
            EntryItemType type,
            CpItemDataHelper &itemDataHelper,
            const QString &text = QString(),
            const QString &description = QString(),
            const QString &iconName = QString(),
            const HbDataFormModelItem *parent = 0);  
    
    explicit CpSettingFormEntryItemData(
            EntryItemType type,
            HbDataForm *dataForm,
            const QString &text = QString(),
            const QString &description = QString(),
            const QString &iconName = QString(),
            const HbDataFormModelItem *parent = 0); 

	virtual ~CpSettingFormEntryItemData();	
	
	QString text() const;
	void setText(const QString &text);
	
	QString description() const;
	void setDescription(const QString &description);

	QString iconName() const;
	void setIconName(const QString &icon);
	
	HbIcon entryItemIcon() const;
	void setEntryItemIcon(const HbIcon &icon);
		
public slots:
	virtual void onLaunchView();
private:
	virtual CpBaseSettingView *createSettingView() const = 0;
private:
    Q_DISABLE_COPY(CpSettingFormEntryItemData)
    Q_DECLARE_PRIVATE_D(d_ptr, CpSettingFormEntryItemData)
    Q_PRIVATE_SLOT(d_func(), void _q_itemPressed(const QModelIndex &index))
    Q_PRIVATE_SLOT(d_func(), void _q_itemActivated(const QModelIndex &index))
private:
	CpSettingFormEntryItemDataPrivate *d_ptr;
};

#endif  //CPSETTINGFORMENTRYITEMDATA_H
