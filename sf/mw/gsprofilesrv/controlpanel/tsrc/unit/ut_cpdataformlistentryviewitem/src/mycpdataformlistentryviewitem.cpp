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

#include "mycpdataformlistentryviewitem.h"

#include <QtTest/QtTest>

/*!
    \class MyCpDataFormListEntryViewItem
    \brief The MyCpDataFormListEntryViewItem class is designed for the unit testing for the protected member functions in CpDataFormListEntryViewItem class.
 */

/*!
 * Default constructor
 */
MyCpDataFormListEntryViewItem::MyCpDataFormListEntryViewItem(QGraphicsItem *parent) : CpDataFormListEntryViewItem(parent)
    {
    
    }

/*!
 * this is also for protected member functions testing, call base class implematation directly.
 */
HbWidget * MyCpDataFormListEntryViewItem::createCustomWidget()
    {
    return CpDataFormListEntryViewItem::createCustomWidget();
    }

void MyCpDataFormListEntryViewItem::pressStateChanged(bool pressed, bool animate)
    {
    CpDataFormListEntryViewItem::pressStateChanged(pressed, animate);
    }

void MyCpDataFormListEntryViewItem::gestureEvent(QGestureEvent *event)
    {
    CpDataFormListEntryViewItem::gestureEvent(event);
    }
