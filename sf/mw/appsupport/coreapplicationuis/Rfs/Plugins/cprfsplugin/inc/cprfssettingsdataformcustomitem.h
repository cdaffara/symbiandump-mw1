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
 * Description:  
 *   
 */

#ifndef CP_RFS_SETTINGS_DATAFORM_CUSTOMITEM_H
#define CP_RFS_SETTINGS_DATAFORM_CUSTOMITEM_H

#include <hbdataformviewitem.h>


class HbPushButton;

class CpRfsSettingsDataFormCustomItem : public HbDataFormViewItem
{
Q_OBJECT

public:
    CpRfsSettingsDataFormCustomItem(QGraphicsItem *parent);
    ~CpRfsSettingsDataFormCustomItem();
    virtual  bool canSetModelIndex(const QModelIndex &index) const;
    virtual HbAbstractViewItem* createItem();
protected:
    //Override createCustomWidget API
    virtual HbWidget* createCustomWidget();   
public:
    HbPushButton *mButton;

};

#endif // CP_RFS_SETTINGS_DATAFORM_CUSTOMITEM_H
