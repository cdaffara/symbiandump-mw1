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

#include "mycpbasesettingview.h"

/*!
    \class MyCpBaseSettingView
    \brief This is a derived class used for protected member functions testing \n
 */

/*!
 * public function, only provide a method for QTest to call the protected close() function. \n
 * directly call close() in base class.
 */
void MyCpBaseSettingView::close()
    {
    CpBaseSettingView::close();
    }

void MyCpBaseSettingView::testprivate()
{
    MyCpBaseSettingView *p = this;
    const QMetaObject *mobj = p->metaObject();
    CpBaseSettingView::qt_metacall( QMetaObject::InvokeMetaMethod,mobj->indexOfSlot("aboutToClose()"),0);
    CpBaseSettingView::qt_metacall( QMetaObject::InvokeMetaMethod,mobj->indexOfSlot("_q_softkeyClicked()"),0);
    CpBaseSettingView::qt_metacall( QMetaObject::InvokeMetaMethod,mobj->indexOfSlot("_q_viewChanged(HbView *view)"),(void**)(&p));
    QModelIndex idx;
    CpBaseSettingView::qt_metacall( QMetaObject::InvokeMetaMethod,mobj->indexOfSlot("_q_itemActivated(const QModelIndex &index)"), ((void **)(&idx)) );
}
