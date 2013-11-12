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
#include "cpitemdatahelper.h"
#include <QList>
#include <hbdataform.h>
#include <hbdataformmodelitem.h>
#include <hbdataformviewitem.h>
#include <hbdataformmodel.h>

/*!
\internal
*/
class CpItemDataHelperPrivate
{
public:
    /*!
    \internal
    */
    class ItemConnection
    {
    public:
        ItemConnection(HbDataFormModelItem *item,
            const char *signal,
            QObject *receiver,
            const char *method) :
            mItem(item),mSignal(signal),
            mReceiver(receiver),mMethod(method)
        {
        }

        bool operator == (const ItemConnection &other)
        {
            return mItem     == other.mItem &&
                   mSignal   == other.mSignal &&
                   mReceiver == other.mReceiver &&
                   mMethod   == other.mMethod;
        }

        HbDataFormModelItem *mItem;
        QString mSignal;
        QObject *mReceiver;
        QString mMethod;
    };

    class FormConnection
    {
    public:
        FormConnection(const char *signal,
            QObject *receiver,
            const char *method) :
            mSignal(signal),mReceiver(receiver),mMethod(method)
        {
        }

        bool operator == (const FormConnection &other)
        {
            return mSignal   == other.mSignal &&
                   mReceiver == other.mReceiver &&
                   mMethod   == other.mMethod;
        }
        QString mSignal;
        QObject *mReceiver;
        QString mMethod;
    };

	CpItemDataHelperPrivate(HbDataForm *form /*= 0*/)
        : mDataForm(form)
	{
	}

	~CpItemDataHelperPrivate()
	{
	}

	void addConnection(HbDataFormModelItem *item,
		const char *signal,
		QObject *receiver,
		const char *method)
	{
        if (mDataForm) {
            mDataForm->addConnection(item,signal,receiver,method);
        }
        else {
            mItemConnections.append(ItemConnection(item,signal,receiver,method));
        }
	}

	void removeConnection(HbDataFormModelItem *item,
		const char *signal,
		QObject *receiver,
		const char *method)
	{
        if (mDataForm) {
            mDataForm->removeConnection(item,signal,receiver,method);
        } 
        else {
            mItemConnections.removeAll(ItemConnection(item,signal,receiver,method));
        }
	}

    void connectToForm(const char *signal,
        QObject *receiver,
        const char *method)
    {
        if (mDataForm) {
            QObject::connect(mDataForm,signal,receiver,method);
        }
        else {
            mFormConnections.append(FormConnection(signal,receiver,method));
        }
    }

    void disconnectFromForm(const char *signal,
        QObject *receiver,
        const char *method)
    {
        if (mDataForm) {
            QObject::disconnect(mDataForm,signal,receiver,method);
        }
        else {
            mFormConnections.removeAll(FormConnection(signal,receiver,method));
        }
    }

    void addItemPrototype(HbAbstractViewItem *prototype)
    {
        if (prototype) {
            if (mDataForm) {
                QList<HbAbstractViewItem *> protoTypeList = mDataForm->itemPrototypes();
                protoTypeList.append(prototype);
                mDataForm->setItemPrototypes(protoTypeList);	
            }
            else {
                mItemPrototypes.append(prototype);
            }
        }
    }

	void bindToForm(HbDataForm *form)
	{
        mDataForm = form;

        if (mDataForm) {
            foreach(const ItemConnection &connection,mItemConnections) {
                mDataForm->addConnection(connection.mItem,connection.mSignal.toAscii(),
                    connection.mReceiver,connection.mMethod.toAscii());
            }
            mItemConnections.clear();

            foreach(const FormConnection &connection,mFormConnections) {
                QObject::connect(mDataForm,connection.mSignal.toAscii(),
                    connection.mReceiver,connection.mMethod.toAscii());
            }
            mFormConnections.clear();

            if (!mItemPrototypes.isEmpty()) {
                QList<HbAbstractViewItem *> protoTypeList = mDataForm->itemPrototypes();
                protoTypeList.append(mItemPrototypes);
                mDataForm->setItemPrototypes(protoTypeList);	
            }
            mItemPrototypes.clear();
        }
	}

    HbWidget *widgetFromModelIndex(const QModelIndex &index)
    {
        if (mDataForm) {
            HbDataFormViewItem *viewItem = qobject_cast<HbDataFormViewItem *>
                                                    (mDataForm->itemByIndex(index));
            if (viewItem) {
                return viewItem->dataItemContentWidget();
            }
        }
        return 0;
    }
    
    HbDataFormModelItem *modelItemFromModelIndex(const QModelIndex &index)
    {
        if (mDataForm) {
            HbDataFormModel *dataFormModel = 
                    qobject_cast<HbDataFormModel*>(mDataForm->model());
            if (dataFormModel) {
                return dataFormModel->itemFromIndex(index);
            }
        }
        return 0;
    }

public:
    HbDataForm *mDataForm;
    QList<ItemConnection> mItemConnections;
    QList<FormConnection> mFormConnections;
    QList<HbAbstractViewItem*> mItemPrototypes;
};


/*!
    \class CpItemDataHelper
    \brief The CpItemDataHelper class is a helper class to help controlpanel plugins to comminicate with the associated dataform,
    such as connecting slots to inner widgets of setting items, add item prototypes.
	 And add prototypes to dataform.
 */


/*!
    Constructor of CpItemDataHelper.
 */
CpItemDataHelper::CpItemDataHelper(HbDataForm *form/* = 0*/)
: d(new CpItemDataHelperPrivate(form))
{
}


/*!
    Destructor of CpItemDataHelper.
 */
CpItemDataHelper::~CpItemDataHelper()
{
    delete d;
}

/*!
    Connect a slot to inner widget's signal of setting item.
 */
void CpItemDataHelper::addConnection(HbDataFormModelItem *item,
									 const char *signal,
									 QObject *receiver,
									 const char *method)
{
	d->addConnection(item,signal,receiver,method);
}

/*!
    Disconnect a slot to inner widget's signal of setting item.
 */
void CpItemDataHelper::removeConnection(HbDataFormModelItem *item,
										const char *signal,
										QObject *receiver,
										const char *method)
{
	d->removeConnection(item,signal,receiver,method);
}

/*!
    Connect slot to data form.
*/
void CpItemDataHelper::connectToForm(const char *signal,
                                     QObject *receiver,
                                     const char *method)
{
    d->connectToForm(signal,receiver,method);
}

/*!
    Disconnect slot from data form.
*/
void CpItemDataHelper::disconnectFromForm(const char *signal,
                                          QObject *receiver,
                                          const char *method)
{
    d->disconnectFromForm(signal,receiver,method);
}


/*!
    Add a prototype to data form, to create custom widget for a custom setting item.
 */
void CpItemDataHelper::addItemPrototype(HbAbstractViewItem *prototype)
{
	d->addItemPrototype(prototype);
}

/*!
    Bind the connections and prototypes to a data form.
 */
void CpItemDataHelper::bindToForm(HbDataForm *form)
{
	d->bindToForm(form);
}


/*!
    Get the HbWidget instance from data form.
*/
HbWidget *CpItemDataHelper::widgetFromModelIndex(const QModelIndex &index)
{
    return d->widgetFromModelIndex(index);
}

/*!
    Get the HbDataFormModelItem with a QModelIndex from associated data form's model.
*/
HbDataFormModelItem *CpItemDataHelper::modelItemFromModelIndex(const QModelIndex &index)
{
    return d->modelItemFromModelIndex(index);
}

//End of File
