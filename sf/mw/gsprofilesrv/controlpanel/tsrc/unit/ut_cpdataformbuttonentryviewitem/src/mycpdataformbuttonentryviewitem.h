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
*       test application for qt control panel public apis.
*/

#ifndef _MY_CP_DATAFORM_BUTTONENTRY_VIEWITEM_H_
#define _MY_CP_DATAFORM_BUTTONENTRY_VIEWITEM_H_

#include "cpdataformbuttonentryviewitem.h"

#include <QObject>

class MyCpDataFormButtonEntryViewItem : public QObject, public CpDataFormButtonEntryViewItem
    {
public:
    MyCpDataFormButtonEntryViewItem(QGraphicsItem *parent = 0);
    MyCpDataFormButtonEntryViewItem(const MyCpDataFormButtonEntryViewItem& other);
    
    MyCpDataFormButtonEntryViewItem& operator=(const MyCpDataFormButtonEntryViewItem& other);
    
    HbWidget * createCustomWidget();
    };

#endif /* _MY_CP_DATAFORM_BUTTONENTRY_VIEWITEM_H_ */
