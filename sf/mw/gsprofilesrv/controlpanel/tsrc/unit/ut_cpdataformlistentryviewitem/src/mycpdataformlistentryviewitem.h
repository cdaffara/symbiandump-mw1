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

#ifndef _MY_CP_DATAFORM_LISTENTRY_VIEWITEM_H_
#define _MY_CP_DATAFORM_LISTENTRY_VIEWITEM_H_

#include "cpdataformlistentryviewitem.h"

#include <QObject>

class MyCpDataFormListEntryViewItem : public QObject, public CpDataFormListEntryViewItem
    {
public:
    MyCpDataFormListEntryViewItem(QGraphicsItem *parent = 0);
    
    virtual HbWidget * createCustomWidget();
    virtual void pressStateChanged(bool pressed, bool animate);
#ifdef HB_GESTURE_FW
    //from HbWidgetBase
    virtual void gestureEvent(QGestureEvent *event);
#endif
    };

#endif /* _MY_CP_DATAFORM_LISTENTRY_VIEWITEM_H_ */
