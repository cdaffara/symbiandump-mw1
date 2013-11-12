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
*						This class is a control panel defined HbDataFormModelItem
*					
*						
*/
#ifndef CPSETTINGFORMITEMDATA_H
#define CPSETTINGFORMITEMDATA_H

#include <cpglobal.h>
#include <hbdataformmodelitem.h>
#include <QObject>
#include <QVariant>

class QString;
class CpItemDataHelper;
class CpSettingFormItemDataPrivate;

class CP_EXPORT CpSettingFormItemData : public QObject, public HbDataFormModelItem
{
	Q_OBJECT
public:
	CpSettingFormItemData(HbDataFormModelItem::DataItemType type,
		const QString &label,
		const HbDataFormModelItem *parent = 0);
	CpSettingFormItemData(const HbDataFormModelItem *parent = 0);
	virtual ~CpSettingFormItemData();
private:
	CpSettingFormItemDataPrivate *d_ptr; //reservered
};


#endif
