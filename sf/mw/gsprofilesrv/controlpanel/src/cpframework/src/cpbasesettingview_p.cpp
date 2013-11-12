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
 * Description:  Private implementaion for CpBaseSettingView
 *
 */

#include "cpbasesettingview_p.h"
#include "cpbasesettingview.h"
#include <hbinstance.h>
#include <hbaction.h>
#include <hbmainwindow.h>
#include <hbdataform.h>
#include <hbdataformmodel.h>
#include <QGraphicsLayout>
#include "cpitemdatahelper.h"
#include "cppluginutility.h"


CpBaseSettingViewPrivate::CpBaseSettingViewPrivate() :
    mBaseSettingView(0),
    mSoftKeyBackAction(0)
{
}

CpBaseSettingViewPrivate::~CpBaseSettingViewPrivate()
{
    if (mBaseSettingView) {
        mBaseSettingView->setNavigationAction (0);
    }
    
    delete mSoftKeyBackAction;
    mSoftKeyBackAction = 0;
}

void CpBaseSettingViewPrivate::init(QGraphicsWidget *widget,CpBaseSettingView *baseSettingView)
{
    mBaseSettingView = baseSettingView;
    
    if (!widget) {
        widget = new HbDataForm();
        // add item prototypes by default.
		CpPluginUtility::addCpItemPrototype(qobject_cast<HbDataForm *>(widget));
    }    
    
	mBaseSettingView->setWidget(widget);
    
    mBaseSettingView->setTitle(hbTrId("txt_cp_title_control_panel"));	//give a default title, sub classes need set it correctly

    mSoftKeyBackAction = new HbAction(Hb::BackNaviAction , mBaseSettingView);
    QObject::connect(mSoftKeyBackAction, 
            SIGNAL(triggered()), 
            mBaseSettingView, 
            SLOT(_q_softkeyClicked()) );

    mBaseSettingView->setNavigationAction (mSoftKeyBackAction);
}

void CpBaseSettingViewPrivate::_q_softkeyClicked()
{
    mBaseSettingView->close();
}

#include "moc_cpbasesettingview.cpp"

//End of File
