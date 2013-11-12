/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 *   
 */

#ifndef CP_RFS_SETTINGSFORM_H
#define CP_RFS_SETTINGSFORM_H

#include <hbdataform.h>
#include <hbtranslator.h>

class HbDataFormModelItem;

class CpRfsSettingsForm : public HbDataForm
{
Q_OBJECT
public:
    explicit CpRfsSettingsForm(QGraphicsItem *parent = 0);
    ~CpRfsSettingsForm();
private:
    void initRfsSettingModel();
    // call these function when activated signal is emitted
    void activateNormalRfs(HbWidget *widget);
    void activateDeepRfs(HbWidget *widget);
    
private slots:
    void onItemActivated(const QModelIndex &index);
    void onPressedNormalRfs();
    void onPressedDeepRfs();

private:

    //DataForm List Items
    HbDataFormModelItem *mNormalRfs;
    HbDataFormModelItem *mDeepRfs;
    HbTranslator mTrans;

};
#endif // CP_RFS_SETTINGSFORM_H
