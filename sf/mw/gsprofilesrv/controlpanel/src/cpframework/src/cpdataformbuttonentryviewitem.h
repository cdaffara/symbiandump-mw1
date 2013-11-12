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
* Description:  View item implementation for button entry item.
*
*/
#ifndef CPDATAFORMBUTTONENTRYVIEWITEM_H
#define CPDATAFORMBUTTONENTRYVIEWITEM_H

#include <hbdataformviewitem.h>

class CpDataFormButtonEntryViewItemPrivate;

class CpDataFormButtonEntryViewItem : public HbDataFormViewItem
{
    Q_OBJECT
public:
    explicit CpDataFormButtonEntryViewItem(QGraphicsItem *parent = 0);
    virtual ~CpDataFormButtonEntryViewItem();
    
    // From HbAbstractViewItem
    virtual HbAbstractViewItem *createItem();
    // From HbAbstractViewItem
	virtual bool canSetModelIndex(const QModelIndex &index) const;
public slots:
    // From HbDataFormViewItem
    virtual void restore();
protected:
	CpDataFormButtonEntryViewItem(const CpDataFormButtonEntryViewItem &other);
	CpDataFormButtonEntryViewItem &operator = (const CpDataFormButtonEntryViewItem &other);
	// From HbAbstractViewItem
    virtual HbWidget *createCustomWidget();
private:
	CpDataFormButtonEntryViewItemPrivate *d_ptr;
};

#endif // CPDATAFORMBUTTONENTRYVIEWITEM_H

//End of File
