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
* Description:  View item implementation for list entry item.
*
*/


#ifndef CPDATAFORMLISTENTRYVIEWITEM_H
#define CPDATAFORMLISTENTRYVIEWITEM_H

#include <hbdataformviewitem.h>

class CpDataFormListEntryViewItem : public HbDataFormViewItem
{
    Q_OBJECT
public:
    explicit CpDataFormListEntryViewItem(QGraphicsItem *parent = 0);
    virtual ~CpDataFormListEntryViewItem();
    // From HbAbstractViewItem
    virtual HbAbstractViewItem* createItem();
    // From HbAbstractViewItem
    virtual bool canSetModelIndex(const QModelIndex &index) const;
protected:
    // From HbDataFormViewItem
    virtual HbWidget* createCustomWidget();

    virtual void restore();

    // From HbAbstractViewItem
    virtual void pressStateChanged(bool pressed, bool animate);
#ifdef HB_GESTURE_FW
    // From HbWidgetBase
    virtual void gestureEvent(QGestureEvent *event);
#endif
};

#endif //CPDATAFORMLISTENTRYVIEWITEM_H

//End of File
