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
#ifndef CPBASESETTINGVIEW_P_H
#define CPBASESETTINGVIEW_P_H

#include <QVariant>

class CpBaseSettingView;
class HbAction;
class HbDataForm;
class CpItemDataHelper;
class QGraphicsWidget;

class CpBaseSettingViewPrivate
{
public:
    CpBaseSettingViewPrivate();
    ~CpBaseSettingViewPrivate();
    /*
     initialize 
     */
    void init(QGraphicsWidget *widget,CpBaseSettingView *baseSettingView);    
    
    //SLOTS
    void _q_softkeyClicked();
public:
    //DATA
    CpBaseSettingView *mBaseSettingView;
    HbAction *mSoftKeyBackAction;
    HbDataForm *mSettingForm;
};

#endif //CPBASESETTINGVIEW_P_H
