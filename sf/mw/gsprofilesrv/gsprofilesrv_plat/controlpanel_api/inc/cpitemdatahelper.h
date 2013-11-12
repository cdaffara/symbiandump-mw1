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
* Description:  Helper class by which a plugin can communicate with the associated dataform.
*
*/
#ifndef CPITEMDATAHELPER_H
#define CPITEMDATAHELPER_H

#include <QObject>
#include <cpglobal.h>

class QObject;
class HbWidget;
class QModelIndex;
class HbDataForm;
class HbAbstractViewItem;
class HbDataFormModelItem;
class CpItemDataHelperPrivate;

class CP_EXPORT CpItemDataHelper : public QObject
{
	Q_OBJECT
public:
    explicit CpItemDataHelper(HbDataForm *form = 0);
    virtual ~CpItemDataHelper();

	void bindToForm(HbDataForm *form);

	void addItemPrototype(HbAbstractViewItem *prototype);

    void addConnection(HbDataFormModelItem *item,
        const char *signal,
        QObject *receiver,
        const char *method);

    void removeConnection(HbDataFormModelItem *item,
        const char *signal,
        QObject *receiver,
        const char *method);

    void connectToForm(const char *signal,
        QObject *receiver,
        const char *method);

    void disconnectFromForm(const char *signal,
        QObject *receiver,
        const char *method);

    HbWidget *widgetFromModelIndex(const QModelIndex &index);
    
    HbDataFormModelItem *modelItemFromModelIndex(const QModelIndex &index);
   
private:
    CpItemDataHelperPrivate *d;
};

#endif
