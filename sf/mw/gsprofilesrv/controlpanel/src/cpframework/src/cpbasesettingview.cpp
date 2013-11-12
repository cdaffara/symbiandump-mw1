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
* Description:  Base class for controlpane plugin views.
*
*/

#include "cpbasesettingview.h"
#include "cpbasesettingview_p.h"
#include <hbdataform.h>
#include "cpitemdatahelper.h"


/*!
    \class CpBaseSettingView
    \brief The CpBaseSettingView is the base class for all setting views in controlpanel application.
    This class is responsible for processing some common properties, such as setting view's title, content widget and back key action.
 */


/*!
    \fn void returnValueDelivered(const QVariant &returnValue)
    
    This signal is emitted when the setting view need to deliver return value to caller. Derived class can 
    emit the signal in some proper time.
*/

/*!
    \fn void aboutToClose()
    
    This signal is emitted when the setting view is about to close.
*/

/*!
    Constructor of CpBaseSettingView.
 */
CpBaseSettingView::CpBaseSettingView(QGraphicsWidget *widget /*= 0*/,QGraphicsItem *parent /*= 0*/) :
    HbView(parent),d_ptr(new CpBaseSettingViewPrivate())
{
    d_ptr->init(widget,this);
}

/*!
    Destructor of CpBaseSettingView.
 */
CpBaseSettingView::~CpBaseSettingView()
{
    delete d_ptr;
}


/*!
    Emit aboutToClose() signal to indicate that the view is about to close.
    Derived class can override it to do some specified work before exit.
 */
void CpBaseSettingView::close()
{
    emit aboutToClose();
}

// End of File
