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

#include "mycpdataformbuttonentryviewitem.h"

/*!
    \class MyCpDataFormButtonEntryViewItem
    \brief The MyCpDataFormButtonEntryViewItem class is designed for the unit testing for the protected member functions in CpDataFormButtonEntryViewItem class.
 */

/*!
 * Default constructor
 */
MyCpDataFormButtonEntryViewItem::MyCpDataFormButtonEntryViewItem(QGraphicsItem *parent) : CpDataFormButtonEntryViewItem(parent)
    {
    
    }

/*!
 * Copy constructor
 */
MyCpDataFormButtonEntryViewItem::MyCpDataFormButtonEntryViewItem(const MyCpDataFormButtonEntryViewItem& other) :
CpDataFormButtonEntryViewItem(other)
    {
    
    }

/*!
 * This is designed for testing of the copy assignment operator = in class CpDataFormButtonEntryViewItem,
 * that operator was declared as protected.
 * the detection of self assignment was removed to enable the detection in base class implementation executed.
 */
MyCpDataFormButtonEntryViewItem& MyCpDataFormButtonEntryViewItem::operator=(const MyCpDataFormButtonEntryViewItem& other)
    {
//    if (&other == this)
//        {
//        return *this;
//        }
    
    CpDataFormButtonEntryViewItem::operator=(other);
    
    return *this;
    }

/*!
 * this is also for protected member functions testing, call base class implematation directly.
 */
HbWidget * MyCpDataFormButtonEntryViewItem::createCustomWidget()
    {
    return CpDataFormButtonEntryViewItem::createCustomWidget();
    }


